"""Opt-in debugger observation protocol, NOT a capture/replay implementation.

No process access, breakpoint writes, API models, output-size inference or
``import_transcript`` conversion happens here. The adapter supplies observations
and acknowledges physical breakpoint operations while the debuggee is stopped.
Addresses and registers are raw 32-bit process VAs/values, not preferred VAs.
Even paired endpoints do not establish complete effects or replayability.
"""
from __future__ import annotations

from copy import deepcopy
from dataclasses import dataclass
import json


REGISTERS = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp", "eip", "eflags")


def _u32(value: int) -> int:
    if type(value) is not int or not 0 <= value <= 0xffffffff:
        raise ValueError("expected raw unsigned 32-bit value")
    return value


def _text(value: str) -> str:
    if not isinstance(value, str) or not value.strip():
        raise ValueError("nonempty evidence description required")
    return value


@dataclass(frozen=True)
class ThreadInstance:
    """A thread in one observation session, including its recorded generation."""

    tid: int
    generation: int

    def __post_init__(self):
        _u32(self.tid)
        if self.tid == 0 or type(self.generation) is not int or self.generation < 1:
            raise ValueError("thread instance requires a registered positive generation")


@dataclass(frozen=True)
class MemoryRead:
    address: int
    requested_size: int
    data: bytes
    purpose: str
    error: str | None = None

    def __post_init__(self):
        _u32(self.address)
        if (type(self.requested_size) is not int or self.requested_size < 0
                or self.address + self.requested_size > 0x100000000):
            raise ValueError("invalid memory read range")
        if not isinstance(self.data, bytes) or len(self.data) > self.requested_size:
            raise ValueError("read bytes exceed requested range")
        _text(self.purpose)
        if self.error is not None:
            _text(self.error)

    @property
    def complete(self) -> bool:
        return self.error is None and len(self.data) == self.requested_size

    def report(self) -> dict:
        return dict(address=self.address, requested_size=self.requested_size,
                    data_hex=self.data.hex(), purpose=self.purpose, error=self.error,
                    complete=self.complete)


@dataclass(frozen=True)
class Snapshot:
    registers: dict[str, int]
    memory: tuple[MemoryRead, ...] = ()
    gaps: tuple[str, ...] = ()

    def report(self) -> dict:
        # Copy at the protocol boundary; never invent missing register values.
        registers = dict(self.registers)
        for name, value in registers.items():
            _text(name)
            _u32(value)
        gaps = list(self.gaps)
        for gap in gaps:
            _text(gap)
        gaps.extend(f"missing-register:{name}" for name in REGISTERS if name not in registers)
        memory = [read.report() for read in self.memory]
        gaps.extend(f"incomplete-read:{index}" for index, read in enumerate(memory) if not read["complete"])
        return dict(registers=registers, memory=memory, gaps=gaps)


class ImportObserver:
    """Single serialized debugger-event stream; disabled unless explicitly opted in.

    ``return_sp`` is an adapter-supplied exact endpoint discriminator with retained
    evidence basis, never a cleanup size inferred from an import name. Unknown
    values are allowed at entry but cannot be paired. ``scope_id`` binds the
    observation to the adapter's enclosing capture/invocation, not to an emulator
    replay. A breakpoint hit is only an endpoint observation, not proof of RET.

    ``thread_start`` returns the generation token required by every thread-scoped
    event API. The adapter stamps that token at event ingress and carries it
    through buffering; it must never relabel an old event from a current TID
    lookup. Tokens enforce lifecycle consistency, not authenticity of the input
    stream. One observer owns one trusted, serialized process event stream.
    """

    def __init__(self, *, enabled: bool = False, source: dict):
        if type(enabled) is not bool:
            raise ValueError("enabled must be boolean")
        self.enabled = enabled
        try:
            self._source = json.loads(json.dumps(source, allow_nan=False))
            _text(self._source["kind"])
            _text(self._source["session"])
        except (KeyError, TypeError, ValueError) as exc:
            raise ValueError("source must be JSON provenance with kind and session") from exc
        self._closed = False
        self._process_exited = False
        self._threads: dict[int, list[int]] = {}
        self._generations: dict[int, int] = {}
        self._records: list[dict] = []
        self._sites: dict[int, dict] = {}
        self._events: list[dict] = []
        self._sequence = 0
        self._revision = 0

    def _active(self):
        if not self.enabled:
            raise RuntimeError("import observation is not enabled")
        if self._closed:
            raise RuntimeError("observation session is closed")

    def _event(self, kind: str, **fields) -> int:
        self._sequence += 1
        self._events.append(dict(sequence=self._sequence, kind=kind, **fields))
        return self._sequence

    def thread_start(self, tid: int) -> int:
        self._active()
        _u32(tid)
        if tid in self._threads:
            raise ValueError("thread is already active")
        self._generations[tid] = self._generations.get(tid, 0) + 1
        self._threads[tid] = []
        self._event("thread-start", tid=tid, generation=self._generations[tid])
        return self._generations[tid]

    def _thread_event(self, tid: int, generation: int, event_kind: str, **evidence) -> bool:
        """Reject stale/inactive identities before touching any current frame."""
        _u32(tid)
        if type(generation) is not int or generation < 1:
            raise ValueError("positive thread generation required")
        active = self._generations.get(tid) if tid in self._threads else None
        if active != generation:
            self._event("rejected-thread-event", event_kind=event_kind, tid=tid,
                        generation=generation, active_generation=active,
                        gaps=["stale-or-inactive-thread-generation"], **evidence)
            return False
        return True

    def _request(self, site: dict, operation: str):
        self._revision += 1
        site["physical"] = "unknown"
        site["request"] = dict(address=site["address"], revision=self._revision, operation=operation)
        self._event("breakpoint-request", **site["request"], owners=sorted(site["owners"]))

    def entry(self, tid: int, scope_id: str, import_name: str, snapshot: Snapshot, *,
              generation: int, return_site: int | None, return_sp: int | None = None,
              return_sp_basis: str | None = None) -> int | None:
        self._active()
        _u32(tid)
        _text(scope_id)
        _text(import_name)
        raw = snapshot.report()
        if return_site is not None:
            _u32(return_site)
        if return_sp is not None:
            _u32(return_sp)
            _text(return_sp_basis)
        elif return_sp_basis is not None:
            raise ValueError("return_sp_basis without return_sp")
        if not self._thread_event(tid, generation, "entry", scope_id=scope_id,
                                  import_name=import_name, snapshot=raw, return_site=return_site,
                                  return_sp=return_sp, return_sp_basis=return_sp_basis):
            return None
        stack = self._threads[tid]
        gaps = list(raw["gaps"])
        sp = raw["registers"].get("esp")
        words = set()
        for read in raw["memory"]:
            if read["address"] == sp and read["requested_size"] == 4 and read["complete"]:
                words.add(int.from_bytes(bytes.fromhex(read["data_hex"]), "little"))
        verified_word = return_site is not None and words == {return_site}
        if verified_word:
            expected = return_site.to_bytes(4, "little")
            for read in raw["memory"]:
                blob = bytes.fromhex(read["data_hex"])
                lo, hi = max(sp, read["address"]), min(sp + 4, read["address"] + len(blob))
                if lo < hi and blob[lo - read["address"]:hi - read["address"]] != expected[lo - sp:hi - sp]:
                    verified_word = False
        if not verified_word:
            gaps.append("unverified-return-word")
        if return_sp is None:
            gaps.append("unknown-return-sp")
        invocation = len(self._records) + 1
        sequence = self._event("entry", tid=tid, generation=generation, invocation=invocation)
        record = dict(invocation=invocation, tid=tid, thread_generation=generation,
                      scope_id=scope_id, import_name=import_name, parent=stack[-1] if stack else None,
                      entry=raw, entry_sequence=sequence, return_site=return_site,
                      return_sp=return_sp, return_sp_basis=return_sp_basis,
                      state="pending", gaps=gaps, replayable=False, breakpoint_ready=False)
        self._records.append(record)
        stack.append(invocation)
        if return_site is not None and "unverified-return-word" not in gaps:
            site = self._sites.setdefault(return_site, dict(address=return_site, owners=set(),
                                                           physical="clear", request=None))
            site["owners"].add(invocation)
            if site["physical"] == "armed" and site["request"] is None:
                record["breakpoint_ready"] = True
            elif site["request"] is None or site["request"]["operation"] != "arm":
                self._request(site, "arm")
        return invocation

    def breakpoint_requests(self) -> list[dict]:
        return deepcopy([site["request"] for site in self._sites.values() if site["request"] is not None])

    def acknowledge_breakpoint(self, address: int, revision: int, *, success: bool,
                               error: str | None = None, memory: tuple[MemoryRead, ...] = ()):
        # Acks remain legal after detach so restoration can finish.
        if not self.enabled:
            raise RuntimeError("import observation is not enabled")
        _u32(address)
        if type(revision) is not int:
            raise ValueError("invalid breakpoint revision")
        site = self._sites[address]
        request = site["request"]
        if request is None or request["revision"] != revision:
            raise ValueError("stale breakpoint acknowledgment")
        if type(success) is not bool:
            raise ValueError("success must be boolean")
        if not success:
            _text(error)
        elif error is not None:
            raise ValueError("successful breakpoint acknowledgment cannot have an error")
        raw = [read.report() for read in memory]
        self._event("breakpoint-ack", **request, success=success, error=error, memory=raw)
        site["request"] = None
        if success:
            site["physical"] = "armed" if request["operation"] == "arm" else "clear"
            if request["operation"] == "arm":
                for invocation in site["owners"]:
                    self._records[invocation - 1]["breakpoint_ready"] = True
        else:
            site["physical"] = "unknown"
            for invocation in site["owners"]:
                self._records[invocation - 1]["gaps"].append("breakpoint-operation-failed")

    def breakpoint_lost(self, address: int, *, reason: str):
        """Report a physical instrumentation gap, e.g. an interrupted step/rearm."""
        self._active()
        _u32(address)
        _text(reason)
        site = self._sites[address]
        self._event("breakpoint-lost", address=address, reason=reason)
        for invocation in site["owners"]:
            record = self._records[invocation - 1]
            record["breakpoint_ready"] = False
            record["gaps"].append("breakpoint-lost:" + reason)
        self._request(site, "arm" if site["owners"] else "remove")

    def _release(self, record: dict):
        site = self._sites.get(record["return_site"])
        if site is not None and record["invocation"] in site["owners"]:
            site["owners"].remove(record["invocation"])
            if (not self._process_exited and not site["owners"]
                    and (site["physical"] != "clear" or site["request"] is not None)):
                self._request(site, "remove")

    def return_hit(self, tid: int, address: int, snapshot: Snapshot, *, generation: int) -> int | None:
        self._active()
        _u32(tid)
        _u32(address)
        raw = snapshot.report()
        if not self._thread_event(tid, generation, "return", address=address, snapshot=raw):
            return None
        stack = self._threads[tid]
        matches = [self._records[index - 1] for index in stack
                   if self._records[index - 1]["thread_generation"] == generation
                   and self._records[index - 1]["return_site"] == address
                   and self._records[index - 1]["return_sp"] is not None
                   and self._records[index - 1]["return_sp"] == raw["registers"].get("esp")]
        reason = None
        if raw["registers"].get("eip") != address:
            reason = "endpoint-ip-mismatch"
        elif len(matches) != 1:
            reason = "ambiguous-return" if matches else "unmatched-return"
        elif matches[0]["invocation"] != stack[-1]:
            reason = "non-lifo-return"
        elif not matches[0]["breakpoint_ready"]:
            reason = "unarmed-return"
        if reason is not None:
            self._event(reason, tid=tid, generation=generation, address=address, snapshot=raw)
            for invocation in stack:
                self._records[invocation - 1]["gaps"].append(reason)
            return None
        record = matches[0]
        record["return"] = raw
        record["return_sequence"] = self._event("return", tid=tid, generation=generation,
                                                invocation=record["invocation"])
        record["gaps"].extend(raw["gaps"])
        record["state"] = "returned-incomplete" if record["gaps"] else "paired-endpoints"
        stack.pop()
        self._release(record)
        return record["invocation"]

    def exception(self, tid: int, *, generation: int, code: int, address: int, first_chance: bool,
                  snapshot: Snapshot) -> bool:
        """Retain a non-protocol exception; do not decide debugger continuation."""
        self._active()
        _u32(tid)
        _u32(code)
        _u32(address)
        if type(first_chance) is not bool:
            raise ValueError("first_chance must be boolean")
        raw = snapshot.report()
        if not self._thread_event(tid, generation, "exception", code=code, address=address,
                                  first_chance=first_chance, snapshot=raw):
            return False
        sequence = self._event("exception", tid=tid, generation=generation, code=code, address=address,
                               first_chance=first_chance, snapshot=raw)
        for invocation in self._threads[tid]:
            self._records[invocation - 1]["gaps"].append(f"exception:{sequence}")
        return True

    def _abort_suffix(self, stack: list[int], start: int, reason: str, sequence: int):
        while len(stack) > start:
            record = self._records[stack.pop() - 1]
            record["state"] = "incomplete"
            record["gaps"].append(reason)
            record["terminal_sequence"] = sequence
            self._release(record)

    def unwind(self, tid: int, through_invocation: int, *, generation: int,
               reason: str, snapshot: Snapshot) -> bool:
        """Abort only a suffix explicitly identified by the adapter, never inferred."""
        self._active()
        _u32(tid)
        if type(through_invocation) is not int or through_invocation < 1:
            raise ValueError("invalid invocation identifier")
        _text(reason)
        raw = snapshot.report()
        if not self._thread_event(tid, generation, "unwind", through_invocation=through_invocation,
                                  reason=reason, snapshot=raw):
            return False
        stack = self._threads[tid]
        if through_invocation not in stack:
            raise ValueError("unwind target is not active on this thread")
        sequence = self._event("unwind", tid=tid, generation=generation, through_invocation=through_invocation,
                               reason=reason, snapshot=raw)
        self._abort_suffix(stack, stack.index(through_invocation), "unwind:" + reason, sequence)
        return True

    def thread_exit(self, tid: int, *, generation: int, exit_code: int) -> bool:
        self._active()
        _u32(tid)
        _u32(exit_code)
        if not self._thread_event(tid, generation, "thread-exit", exit_code=exit_code):
            return False
        sequence = self._event("thread-exit", tid=tid, generation=generation,
                               exit_code=exit_code)
        self._abort_suffix(self._threads.pop(tid), 0, "thread-exit", sequence)
        return True

    def process_exit(self, *, exit_code: int):
        # The debuggee can exit during cleanup, after the logical session closed.
        if not self.enabled or self._process_exited:
            raise RuntimeError("no active process observation")
        _u32(exit_code)
        self._closed = self._process_exited = True
        sequence = self._event("process-exit", exit_code=exit_code)
        for stack in self._threads.values():
            self._abort_suffix(stack, 0, "process-exit", sequence)
        self._threads.clear()
        for site in self._sites.values():
            site["request"] = None
            site["physical"] = "process-exited"  # NOT an acknowledgment of restoration

    def retry_cleanup(self):
        """Reissue failed removals with fresh revisions; never re-arm on retry."""
        if not self.enabled or self._process_exited:
            raise RuntimeError("no active process observation")
        for site in self._sites.values():
            if not site["owners"] and site["physical"] != "clear" and site["request"] is None:
                self._request(site, "remove")

    def detach(self, reason: str):
        _text(reason)
        if not self.enabled or self._closed:
            return
        self._closed = True
        sequence = self._event("detach", reason=reason)
        for stack in self._threads.values():
            self._abort_suffix(stack, 0, "detach:" + reason, sequence)
        self.retry_cleanup()

    def report(self) -> dict:
        return deepcopy(dict(schema=2, evidence_kind="import-endpoint-observations",
                             thread_identity="adapter-stamped-generation",
                             source=self._source, enabled=self.enabled, closed=self._closed,
                             process_exited=self._process_exited,
                             cleanup_evidence="adapter-acknowledgments",
                             replayable=False, invocations=self._records, events=self._events,
                             breakpoints=[dict(address=site["address"], owners=sorted(site["owners"]),
                                               physical=site["physical"], request=site["request"])
                                          for site in self._sites.values()],
                             cleanup_complete=self._closed and all(
                                 site["physical"] == "clear" and site["request"] is None
                                 for site in self._sites.values())))
