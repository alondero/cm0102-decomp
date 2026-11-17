"""Opt-in recorder observation adapter; no process control or replay.

The owner feeds ONE serialized, stopped debugger event stream with generation
tokens stamped at ingress. The I/O callbacks must read that same process and
acknowledge physical breakpoint operations only after verified readback. This
module neither attaches nor continues a process. It is not wired into Recorder.
"""
from __future__ import annotations

import json
from pathlib import Path
import struct

from . import env
from .import_observe import ImportObserver, MemoryRead, Snapshot


HEAP = "KERNEL32.dll!HeapAlloc"
READ = "KERNEL32.dll!ReadFile"
ARGUMENTS = {HEAP: 3, READ: 5}
OBSERVATION_STORE = env.ROOT / "local/import-observations"


class RecorderImportObserver:
    """Observe only evidenced FF15 callers within a pinned image's text range.

    ``imports`` and ``text_range`` must come from the owner's identity-checked
    pinned image. Live IAT and caller bytes are checked again at every entry.
    This local trusted-input contract is not an authenticity proof.
    """

    def __init__(self, *, enabled=False, source, imports, text_range, read, breakpoint,
                 recover_step=None):
        self.observer = ImportObserver(enabled=enabled, source=source)
        self.imports = dict(imports)
        self.text_range = tuple(text_range)
        self.read, self.breakpoint = read, breakpoint
        self.scopes = {}
        self.generations = {}
        self.used_scopes = set()
        self.entries = {}
        self.recover_step = recover_step
        self.stepping = None
        self.step_recovery = []

    def thread_start(self, tid):
        generation = self.observer.thread_start(tid)
        self.generations[tid] = generation
        return generation

    def _active(self):
        report = self.observer.report()
        if not report["enabled"] or report["closed"]:
            raise RuntimeError("observation disabled or closed")

    def begin_scope(self, tid, generation, scope_id):
        self._active()
        if (self.generations.get(tid) != generation or tid in self.scopes
                or not isinstance(scope_id, str) or not scope_id.strip()
                or scope_id in self.used_scopes):
            raise ValueError("inactive thread or overlapping/reused observation scope")
        self.scopes[tid] = (generation, scope_id)
        self.used_scopes.add(scope_id)

    def end_scope(self, tid, generation, scope_id, snapshot, *, reason):
        if self.scopes.get(tid) != (generation, scope_id):
            raise ValueError("scope identity mismatch")
        pending = [r for r in self.observer.report()["invocations"]
                   if r["tid"] == tid and r["thread_generation"] == generation
                   and r["state"] == "pending"]
        if pending:
            self.observer.unwind(tid, pending[0]["invocation"], generation=generation,
                                 reason=reason, snapshot=snapshot)
        del self.scopes[tid]
        self._drain()

    def thread_exit(self, tid, generation, *, exit_code):
        if self.observer.thread_exit(tid, generation=generation, exit_code=exit_code):
            self.scopes.pop(tid, None)
            self.generations.pop(tid, None)
        self._drain()

    def _exact(self, address, size, purpose):
        read = self.read(address, size, purpose)
        if not read.complete:
            raise ValueError("incomplete " + purpose)
        return read

    def _drain(self):
        for request in self.observer.breakpoint_requests():
            try:
                memory = tuple(self.breakpoint(dict(request)))
            except OSError as exc:
                self.observer.acknowledge_breakpoint(
                    request["address"], request["revision"], success=False, error=str(exc))
                raise
            self.observer.acknowledge_breakpoint(
                request["address"], request["revision"], success=True, memory=memory)

    def import_entry(self, tid, generation, snapshot, *, iat_slot, synchronous_file_basis=None):
        self._active()
        if self.stepping is not None:
            raise RuntimeError("step obligation must finish before processing endpoints")
        scope = self.scopes.get(tid)
        if scope is None or scope[0] != generation:
            raise ValueError("import outside active thread-generation scope")
        name = self.imports.get(iat_slot)
        if name not in ARGUMENTS:
            raise ValueError("unsupported observed import")
        raw = snapshot.report()
        if raw["gaps"]:
            raise ValueError("incomplete entry registers")
        sp = raw["registers"]["esp"]
        word = self._exact(sp, 4, "return-word")
        site = int.from_bytes(word.data, "little")
        lo, hi = self.text_range
        if not lo <= site - 6 < site <= hi:
            raise ValueError("return site outside pinned text")
        caller = self._exact(site - 6, 6, "caller-IAT-call")
        if caller.data != b"\xff\x15" + struct.pack("<I", iat_slot):
            raise ValueError("return word does not follow verified CALL [IAT]")
        iat = self._exact(iat_slot, 4, "resolved-IAT")
        if int.from_bytes(iat.data, "little") != raw["registers"]["eip"]:
            raise ValueError("entry IP differs from resolved IAT")
        argc = ARGUMENTS[name]
        arguments = self._exact(sp + 4, argc * 4, "import-arguments")
        args = struct.unpack("<" + "I" * argc, arguments.data)
        gaps = list(snapshot.gaps)
        if name == READ:
            if args[4]:
                gaps.append("overlapped-unsupported")
            if not isinstance(synchronous_file_basis, str) or not synchronous_file_basis.strip():
                gaps.append("synchronous-handle-unproven")
        enriched = Snapshot(dict(snapshot.registers), (*snapshot.memory, word, caller, iat, arguments),
                            tuple(gaps))
        invocation = self.observer.entry(
            tid, scope[1], name, enriched, generation=generation, return_site=site,
            return_sp=sp + 4 + 4 * argc,
            return_sp_basis="Win32 documented stdcall for selected HeapAlloc/ReadFile; "
                            "live CALL [verified IAT] and return word retained; RET path untraced")
        if invocation is not None:
            # Retain handle provenance supplied by the owner, never infer it from
            # NULL lpOverlapped. This evidence is not independently authenticated.
            self.entries[invocation] = (name, args, synchronous_file_basis)
        self._drain()
        return invocation

    def return_hit(self, tid, generation, address, snapshot):
        self._active()
        if self.stepping is not None:
            raise RuntimeError("step obligation must finish before processing endpoints")
        # Inspect only the protocol's top active invocation, before reading outputs.
        records = [r for r in self.observer.report()["invocations"]
                   if r["tid"] == tid and r["thread_generation"] == generation
                   and r["state"] == "pending"]
        candidate = records[-1] if records else None
        reads, gaps = (), ()
        if (not snapshot.report()["gaps"] and candidate is not None and candidate["return_site"] == address
                and candidate["return_sp"] == snapshot.registers.get("esp")
                and snapshot.registers.get("eip") == address and candidate["breakpoint_ready"]):
            name, args, _ = self.entries[candidate["invocation"]]
            eax = snapshot.registers["eax"]
            try:
                reads, gaps = self._outputs(name, args, eax)
            except ValueError as exc:
                # Invalid ranges cannot become valid API output by truncation.
                reads, gaps = (), ("invalid-output-range", str(exc))
        invocation = self.observer.return_hit(
            tid, address, Snapshot(dict(snapshot.registers), (*snapshot.memory, *reads),
                                   (*snapshot.gaps, *gaps)),
            generation=generation)
        self._drain()
        return invocation

    def report(self):
        report = self.observer.report()
        for record in report["invocations"]:
            record["synchronous_file_basis"] = self.entries[record["invocation"]][2]
        report["step_recovery"] = json.loads(json.dumps(self.step_recovery))
        report["pending_step"] = dict(self.stepping) if self.stepping is not None else None
        report["cleanup_complete"] &= self.stepping is None
        report["effects_complete"] = False
        report["recorder_wired"] = False
        return report

    def begin_step(self, tid, generation, address, snapshot):
        """Owner calls BEFORE changing context/unpatching for a single step.

        Only records a recovery obligation. The backend still owns peer suspension,
        original-byte restoration, exact instruction execution and physical rearm.
        """
        if self.stepping is not None or self.generations.get(tid) != generation:
            raise ValueError("step identity mismatch or step already pending")
        report = self.observer.report()
        if not report["enabled"] or report["closed"] or not callable(self.recover_step):
            raise RuntimeError("step requires active observation and recovery callback")
        if (snapshot.report()["gaps"] or snapshot.registers["eip"] != address
                or snapshot.registers["eflags"] & 0x100):
            raise ValueError("step requires observed endpoint with clear trap flag")
        pending = [r for r in report["invocations"] if r["tid"] == tid
                   and r["thread_generation"] == generation and r["state"] == "pending"]
        if not any(address in (r["return_site"], r["entry"]["registers"]["eip"]) for r in pending):
            raise ValueError("step address is not owned by an active invocation")
        self.stepping = dict(tid=tid, generation=generation, address=address)

    def finish_step(self, tid, generation, snapshot):
        """Backend asserts step/rearm finished; retain its observed cleared TF."""
        if self.stepping is None or (tid, generation) != (
                self.stepping["tid"], self.stepping["generation"]) or (
                self.generations.get(tid) != generation):
            raise ValueError("step identity mismatch")
        raw = snapshot.report()
        if raw["gaps"] or raw["registers"]["eflags"] & 0x100:
            raise ValueError("step recovery lacks complete cleared trap flag observation")
        self.step_recovery.append(dict(lease=dict(self.stepping), snapshot=raw))
        self.stepping = None

    def _bounded(self, address, size, purpose):
        if (type(address) is not int or type(size) is not int or size < 0
                or not 0 <= address <= address + size <= 0x100000000):
            raise ValueError("invalid output range")
        read = self.read(address, min(size, 64), purpose)
        errors = (["observation-limit:64"] if size > 64 else [])
        if read.error:
            errors.append(read.error)
        return MemoryRead(address, size, read.data, purpose, ";".join(errors) or None)

    def _outputs(self, name, args, eax):
        if name == HEAP:
            if not eax:
                return (), ("allocation-failed",)
            return (self._bounded(eax, args[2], "HeapAlloc.buffer"),), ()
        reads, gaps = [], []
        if not eax:
            gaps.append("api-error-or-pending")
        if args[3]:
            try:
                count = self._bounded(args[3], 4, "ReadFile.byte-count")
            except ValueError as exc:
                count = None
                gaps.extend(("invalid-output-range", str(exc)))
            if count is not None:
                reads.append(count)
            if count is not None and count.complete:
                actual = int.from_bytes(count.data, "little")
                if actual < args[2]:
                    gaps.append("short-read")
                elif actual > args[2]:
                    gaps.append("byte-count-exceeds-request")
            else:
                gaps.append("byte-count-unreadable")
        else:
            gaps.append("byte-count-pointer-null")
        # Observe the declared buffer, including unwritten tails; never label
        # bytes as file data when completion/count evidence is incomplete.
        try:
            reads.append(self._bounded(args[1], args[2], "ReadFile.buffer"))
        except ValueError as exc:
            gaps.extend(("invalid-output-range", str(exc)))
        return tuple(reads), tuple(gaps)

    def close(self, reason):
        self.observer.detach(reason)
        try:
            if self.stepping is not None:
                lease = dict(self.stepping)
                try:
                    if self.generations.get(lease["tid"]) != lease["generation"]:
                        raise RuntimeError("cannot recover step through stale thread generation")
                    snapshot = self.recover_step(dict(lease))
                    self.finish_step(lease["tid"], lease["generation"], snapshot)
                except (OSError, ValueError, RuntimeError) as exc:
                    self.step_recovery.append(dict(lease=lease, error=str(exc)))
                    raise
        finally:
            self.observer.retry_cleanup()
            self._drain()

    def save(self, path):
        """Exclusive immutable evidence, confined to the ignored observation store."""
        report = self.report()
        if not report["closed"]:
            raise ValueError("close observation before saving")
        path = Path(path).resolve()
        if not path.is_relative_to(OBSERVATION_STORE.resolve()):
            raise ValueError("observations must stay in the local observation store")
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open("xb") as stream:
            stream.write(json.dumps(report, sort_keys=True, allow_nan=False).encode("utf-8"))
