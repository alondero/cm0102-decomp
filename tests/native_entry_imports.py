"""Owned-child scope client. No user PID, original game, or replay conversion."""
import struct
import uuid

from cmre import debug_observe, env
from cmre.capture_entry_observe import EntryImportScopes
from native_import_observer import _OwnedChildClient
from native_import_results import NativeResultsProfile


class _EntryProfile(NativeResultsProfile):
    def bind(self, manifest, process, k, memory, exact, result):
        entries = super().bind(manifest, process, k, memory, exact, result)
        entry, ret = manifest["function_entry"], manifest["function_return"]
        # Authored assembly is straight-line pushes/CALLs/stores, final C3.
        # The inherited binding independently checks both CALL [IAT] sequences.
        if (entry != manifest["HeapAlloc"]["pushes"]
                or ret != manifest["ReadFile"]["continuation"] + 5
                or exact(ret, 1) != b"\xc3"
                or exact(ret - 5, 1) != b"\xa3"):
            raise ValueError("authored function endpoint contract differs")
        self.function, self.result = entry, result
        self.basis = (
            f"Hash-bound authored native_calls straight-line assembly, final C3 "
            f"at process VA {ret:#x}; no arguments or caller-frame adjustment. "
            "Endpoint pairing is not instruction tracing of RET.")
        return (*entries, entry)

    def entry(self, obs, tid, generation, address, ctx, snapshot, memory, exact, facts):
        if address == self.function:
            if not hasattr(self, "scopes"):
                self.scopes = EntryImportScopes(obs, max_scopes=1)
            raw = snapshot(ctx, ip=address)
            root = self.scopes.function_entry(
                tid, generation, raw, function=address,
                return_word=memory(ctx.Esp, 4, "function-return-word"),
                return_sp=ctx.Esp + 4, return_sp_basis=self.basis)
            self.result["entry_snapshot"] = raw.report()
            facts[root] = dict(kind="function-scope")
            return root
        # Retain inherited caller/IAT/ABI/output checks, but enroll in the
        # observed outer function's scope rather than a fixture constant.
        binding = self.by_address[address]
        caller = struct.unpack("<I", exact(ctx.Esp, 4))[0]
        if caller != binding["continuation"]:
            return super().entry(obs, tid, generation, address, ctx, snapshot, memory, exact, facts)
        scope = self.scopes.scope_for(tid, generation) if hasattr(self, "scopes") else None
        if scope is None:
            raise RuntimeError("selected import outside observed function generation/scope")
        # A narrowly scoped facade changes only the scope argument used by the
        # existing definition; pairing, generations and leases use the SAME obs.
        class ScopedEntry:
            def entry(self, entry_tid, ignored_scope, *args, **kwargs):
                if entry_tid != tid or kwargs["generation"] != generation:
                    raise ValueError("entry identity changed")
                return obs.entry(entry_tid, scope, *args, **kwargs)

        return super().entry(ScopedEntry(), tid, generation, address, ctx,
                             snapshot, memory, exact, facts)

    def outputs(self, fact, ctx, memory):
        if fact.get("kind") == "function-scope":
            return (), ()  # no assertion about complete function memory effects
        return super().outputs(fact, ctx, memory)


class _ScopeChild(_OwnedChildClient):
    def retain_and_close(self, result):
        if hasattr(self.profile, "scopes"):
            result["function_scopes"] = self.profile.scopes.report()
        super().retain_and_close(result)


def observe_entry_imports(exe, **options):
    directory = exe.parent / ("scope-run-" + uuid.uuid4().hex)
    directory.mkdir()
    payload = b"CMRE-real-ReadFile\x00\x7f\x80\xff!\n"
    path = directory / "authored-input.bin"
    path.write_bytes(payload)
    report = options.pop("report", {})
    report.update(authored_data_path=str(path), authored_data_sha256=env.digest(path),
                  effects_scope="Native endpoint observations, not capture/replay")
    client = _ScopeChild(exe, "exact", _EntryProfile(exe, directory))
    try:
        return debug_observe.run_session(client, report=report, **options)
    finally:
        if client.running():
            client.abort_owned_child()
            client.wait_after_abort()
