"""Owned authored-child client for the reusable physical debugger engine.

No user PID interface. Fixture launch/control markers/termination remain here;
event receipt, generations, breakpoint stepping and cleanup live in cmre.
"""
from __future__ import annotations

import json
from pathlib import Path
import struct
import subprocess

from cmre import debug_observe, env
from cmre.debug_observe import _exit_transition_candidate
from native_debug_fixture import SOURCE, DEFINITION, SCENARIOS, _line_with_timeout


class _FixtureProfile:
    source, definition, scenarios = SOURCE, DEFINITION, SCENARIOS
    manifest_version = 1
    # Preserve the original default fixture's lack of an event-count ceiling;
    # the physical engine still enforces its wall-clock and cleanup deadlines.
    event_budget = float("inf")
    workdir = None
    warning = "Authored surrogate call observations; not game captures or API semantics"

    def bind(self, manifest, process, k, memory, exact, result):
        if (exact(manifest["entry"], 1) != b"\x90"
                or exact(manifest["return"], 2) != b"\x90\xc3"
                or exact(manifest["probe"], 1) != b"\x90"):
            raise RuntimeError("external fixture bytes do not match source contract")
        self.manifest = manifest
        return (manifest["entry"],)

    def entry(self, obs, tid, generation, address, ctx, snapshot, memory, exact, facts):
        caller, mode, depth, out, token = struct.unpack("<5I", exact(ctx.Esp, 20))
        return_code = exact(self.manifest["return"], 2)
        if return_code != b"\x90\xc3":
            raise RuntimeError("fixture NOP/RET contract mismatch")
        invocation = obs.entry(
            tid, "authored-fixture-scope", "fixture!fixture_call",
            snapshot(ctx, ip=address, reads=(
                memory(ctx.Esp, 4, "return-word"),
                memory(ctx.Esp + 4, 16, "fixture-stack-arguments"),
                memory(self.manifest["return"], 2, "fixture-NOP-RET-basis"),
                memory(out, 4, "fixture-output-before"))),
            generation=generation, return_site=caller, return_sp=ctx.Esp + 4,
            return_sp_basis=f"Authored fixture verified NOP C3 at process VA {self.manifest['return']:#x}")
        if invocation is None:
            raise RuntimeError("entry event rejected its stamped generation")
        facts[invocation] = dict(mode=mode, depth=depth, out=out, token=token)
        return invocation

    def outputs(self, fact, ctx, memory):
        return (memory(fact["out"], 4, "fixture-output-after"),), ()


class _OwnedChildClient:
    """Fixture-specific lifetime policy, never a general attach/kill capability."""

    ownership = "owned-authored-child"

    def __init__(self, exe, scenario, profile):
        self.exe, self.scenario, self.profile = Path(exe), scenario, profile
        build = json.loads((self.exe.parent / "build.json").read_text(encoding="utf-8"))
        if (build.get("source_sha256") != env.digest(profile.source)
                or build.get("definition_sha256") != env.digest(profile.definition)
                or build.get("exe_sha256") != env.digest(self.exe)
                or build.get("external_breakpoints") is not True):
            raise ValueError("requires matching authored external-breakpoint fixture receipt")
        self.child = None
        self.manifest_version, self.event_budget = profile.manifest_version, profile.event_budget

    def start(self):
        self.child = subprocess.Popen([str(self.exe)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                      stderr=subprocess.PIPE, cwd=self.profile.workdir)

    @property
    def pid(self):
        return None if self.child is None else self.child.pid

    @property
    def returncode(self):
        return self.child.returncode

    @property
    def source(self):
        return dict(
            kind="authored-native-debugger", pid=self.pid, executable_sha256=env.digest(self.exe),
            source_sha256=env.digest(self.profile.source),
            fixture_definition_sha256=env.digest(self.profile.definition),
            thread_identity="Generation stamped on receipt from one serialized Windows debug-event stream",
            warning=self.profile.warning)

    def manifest(self):
        return json.loads(_line_with_timeout(self.child.stdout, 5))

    def bind(self, *args):
        return self.profile.bind(*args)

    def entry(self, *args):
        return self.profile.entry(*args)

    def outputs(self, *args):
        return self.profile.outputs(*args)

    def done_address(self, manifest):
        return manifest["done"]

    def probe_address(self, manifest):
        return manifest["probe"]

    def release(self):
        self.child.stdin.write(self.profile.scenarios[self.scenario].encode("ascii"))
        self.child.stdin.flush()

    def prepare_detach(self, early, manifest, exact, write, result):
        if early:
            before = exact(manifest["done"], 1)
            if before != b"\xcc":
                raise RuntimeError("unexpected fixture done marker")
            write(manifest["done"], b"\x90")
            result["fixture_controls"].append(dict(
                address=manifest["done"], before_hex="cc", after_hex="90",
                purpose="allow authored child to complete after in-flight detach"))

    def detached(self):
        self.child.stdin.write(b"!")
        self.child.stdin.flush()

    def finish(self):
        stdout, stderr = self.child.communicate(timeout=7)
        return dict(child_returncode=self.child.returncode, stdout=stdout.decode(), stderr=stderr.decode())

    def running(self):
        return self.child is not None and self.child.poll() is None

    def abort_owned_child(self):
        self.child.kill()  # ONLY the child this exact client created.

    def wait_after_abort(self):
        self.child.communicate(timeout=5)

    def retain_and_close(self, result):
        self.child.stdin.close()
        self.child.stdout.close()
        self.child.stderr.close()
        path = self.exe.parent / f"protocol-{self.scenario}-{result['run_id']}.json"
        result["evidence_path"] = str(path)
        path.write_text(json.dumps(result, indent=2), encoding="utf-8")


def observe_calls(exe: Path, scenario: str, *, detach_at: str | None = None,
                  fail_at: str | None = None, report: dict | None = None, seconds=12,
                  _profile=None) -> dict:
    """Existing fixture interface; physical hazards are owned by the backend."""
    profile = _FixtureProfile() if _profile is None else _profile
    if scenario not in profile.scenarios or detach_at not in (None, "entry-hit", "step-prepared", "step-stop"):
        raise ValueError("unsupported fixture scenario/detach point")
    if fail_at not in (None, "step-prepared", "step-stop"):
        raise ValueError("unsupported failure injection")
    client = _OwnedChildClient(exe, scenario, profile)
    try:
        return debug_observe.run_session(client, detach_at=detach_at, fail_at=fail_at,
                                         report=report, seconds=seconds)
    finally:
        # Covers failures before the physical engine's cleanup scope exists,
        # e.g. construction of its initial evidence after process creation.
        if client.running():
            client.abort_owned_child()
            client.wait_after_abort()
