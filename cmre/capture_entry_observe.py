"""Bounded native function scopes over the existing endpoint protocol (tooling).

No attach, replay, emulator completion, or inferred function ABI. One physical
debugger owns this observer and ALL its breakpoint requests. Function endpoints
use the protocol's ordinary nested leases, explicitly labelled as function
scopes, not API calls. A native client must establish the endpoint SP contract.
The legacy game Recorder does not use this module.
"""
from __future__ import annotations

from .import_observe import Snapshot


class EntryImportScopes:
    """One non-overlapping function scope per thread generation, bounded per run.

    The owner passes the SAME observer to its import-entry definitions. It uses
    ``scope_for`` rather than a constant scope label. Function return hits,
    exceptions, thread/process exits and detach go through that observer, whose
    LIFO matching and shared breakpoint leases remain authoritative.
    """

    def __init__(self, observer, *, max_scopes=24):
        if type(max_scopes) is not int or not 0 < max_scopes <= 4096:
            raise ValueError("positive bounded scope budget required")
        self.observer = observer
        self.max_scopes = max_scopes
        self._scopes = []

    def _records(self):
        report = self.observer.report()
        return report, {r["invocation"]: r for r in report["invocations"]}

    def scope_for(self, tid, generation):
        """Return only a still-pending scope for this exact ingress identity."""
        if type(tid) is not int or not 0 <= tid <= 0xffffffff:
            return None
        if type(generation) is not int or generation < 1:
            return None
        report, records = self._records()
        if report["closed"] or report["process_exited"]:
            return None
        for scope in reversed(self._scopes):
            if (scope["tid"], scope["thread_generation"]) == (tid, generation):
                if records[scope["invocation"]]["state"] == "pending":
                    return scope["scope_id"]
        return None

    def function_entry(self, tid, generation, snapshot, *, function, return_word,
                       return_sp, return_sp_basis):
        """Enroll a stopped native entry; never infer a cdecl/stdcall delta.

        The complete exact return-word sample MUST be from entry ESP. The
        independently supplied endpoint SP/basis is retained, not authenticated.
        Snapshot/raw byte provenance and image identity remain owner obligations.
        Requests must be physically acknowledged before continuing the process.
        """
        if type(tid) is not int or not 0 <= tid <= 0xffffffff:
            raise ValueError("expected raw unsigned 32-bit integer thread ID")
        if self.scope_for(tid, generation) is not None:
            raise ValueError("overlapping function scope")
        _, records = self._records()
        if any(r["tid"] == tid and r["thread_generation"] == generation
               and r["state"] == "pending" for r in records.values()):
            raise ValueError("function entry overlaps a pending protocol invocation")
        if len(self._scopes) >= self.max_scopes:
            raise ValueError("function scope budget exhausted")
        raw = snapshot.report()
        if (type(function) is not int or not 0 <= function <= 0xffffffff
                or raw["gaps"] or raw["registers"]["eip"] != function):
            raise ValueError("complete normalized function entry required")
        if raw["registers"]["eflags"] & 0x100:
            raise ValueError("function entry has pending debugger trap flag")
        if (not return_word.complete or return_word.address != raw["registers"]["esp"]
                or return_word.requested_size != 4):
            raise ValueError("exact function-entry return word required")
        sp = return_word.address
        for read in snapshot.memory:
            lo, hi = max(sp, read.address), min(sp + 4, read.address + len(read.data))
            if lo < hi and read.data[lo - read.address:hi - read.address] != return_word.data[lo - sp:hi - sp]:
                raise ValueError("contradictory function-entry return sample")
        if (type(return_sp) is not int or not 0 <= return_sp <= 0xffffffff
                or not isinstance(return_sp_basis, str) or not return_sp_basis.strip()):
            raise ValueError("independently established function endpoint SP required")
        scope_id = f"native-function-scope-{len(self._scopes) + 1}"
        invocation = self.observer.entry(
            tid, scope_id, f"function-entry!{function:08x}",
            Snapshot(dict(snapshot.registers), (*snapshot.memory, return_word), snapshot.gaps),
            generation=generation, return_site=int.from_bytes(return_word.data, "little"),
            return_sp=return_sp, return_sp_basis=return_sp_basis)
        if invocation is None:
            raise ValueError("stale or inactive function-entry generation")
        self._scopes.append(dict(
            kind="function-scope", scope_id=scope_id, invocation=invocation,
            tid=tid, thread_generation=generation, function=function))
        return invocation

    def report(self):
        """Detached metadata; raw entry/endpoints remain in the protocol report."""
        report, records = self._records()
        scopes = []
        for scope in self._scopes:
            owned = {scope["invocation"]}
            imports = []
            # Protocol insertion order places every parent before its children.
            # Do not associate a foreign invocation merely by its scope label.
            for record in records.values():
                if (record["parent"] in owned
                        and record["tid"] == scope["tid"]
                        and record["thread_generation"] == scope["thread_generation"]
                        and record["scope_id"] == scope["scope_id"]):
                    owned.add(record["invocation"])
                    imports.append(record["invocation"])
            root = records[scope["invocation"]]
            scopes.append(dict(scope, state=root["state"], gaps=list(root["gaps"]), imports=imports))
        return dict(
            schema=1, replayable=False, effects_complete=False, game_recorder_wired=False,
            cleanup_complete=report["cleanup_complete"], max_scopes=self.max_scopes,
            scopes=scopes)
