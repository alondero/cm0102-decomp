"""Asset-free regressions for stateless-owner context memory.

Each actor turn is a fresh session, so the durable campaign store is the only
memory the owner has.  These tests drive the real core with a fake actor and
a fake executor that retains a real probe fixture file, and assert that the
next owner sees its own previous action and the verified input fixture of the
latest successfully executed probe.
"""

from __future__ import annotations

import hashlib
import json
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre import provider_campaign as pc  # noqa: E402
from cmre import provider_jobs  # noqa: E402

MODELS = {"deepseek": "deepseek-observed-v1", "minimax": "minimax-observed-v2"}
MARKER = "ACTOR-RESPONSE-MARKER"
FIXTURE = {
    "registers": {"eax": "0x65", "ebx": "0x0"},
    "memory": ["00", "11", "22", "33"],
    "seed": "original-seed",
}
PATCH = {
    "regs": {"eax": "0x66", "ecx": "0x0"},
    "flags": ["trap", "carry"],
    "depth": 3,
}
REQUEST = {"entry": "0x401010", "patch": PATCH, "note": "n" * 400}


class Clock:
    def __init__(self, value=1000.0):
        self.value = float(value)

    def __call__(self):
        return self.value


def clone(value):
    return json.loads(json.dumps(value))


def make_manifest(**config):
    job_config = {
        "function": "0x401000",
        "mode": "input",
        "goal": "implement alpha",
    }
    job_config.update(config)
    return {
        "schema": pc.SCHEMA,
        "duration_seconds": 600,
        "concurrency": 1,
        "roles": {role: {"model": MODELS[role]} for role in ("deepseek", "minimax")},
        "jobs": [{"id": "alpha", "config": job_config}],
    }


def make_campaign(tmp_path, name="camp", **config):
    directory = tmp_path / name
    pc.initialize(directory, make_manifest(**config), {"provider": "deepseek"})
    return directory


def probe_action(request=None, base=None):
    action = {"kind": "probe", "request": clone(request or REQUEST)}
    if base is not None:
        action["base"] = base
    return action


def evidence_action(**extra):
    action = {"kind": "evidence", "function": "0x401000"}
    action.update(extra)
    return action


class Harness:
    """A fake actor and fake executor that retain a real probe fixture."""

    def __init__(
        self,
        directory,
        *,
        fixture=None,
        outcome="return",
        fixture_location=None,
        write_fixture=True,
        failing_kinds=(),
    ):
        self.directory = Path(directory)
        self.fixture = clone(fixture if fixture is not None else FIXTURE)
        self.outcome = outcome
        self.fixture_location = fixture_location
        self.write_fixture = write_fixture
        self.failing_kinds = set(failing_kinds)
        self.script = []
        self.contexts = []
        self.executed = []

    def script_actions(self, *actions):
        self.script.extend(clone(action) for action in actions)

    def actor(self, job, role, context, stage_path):
        self.contexts.append(clone(context))
        if self.script:
            action = self.script.pop(0)
        else:
            action = {"kind": "hold", "reason": "script exhausted"}
        return {
            "action": clone(action),
            "model": MODELS[role],
            "receipt": {"marker": MARKER},
        }

    def execute(self, job, action, output_path, prior=None):
        self.executed.append(clone(action))
        kind = action.get("kind")
        if kind == "probe":
            return self._probe(action, output_path)
        if kind in self.failing_kinds:
            return {
                "kind": kind,
                "ok": False,
                "error": "range rejected",
                "result": {"error": "range rejected"},
            }
        return {"kind": kind, "ok": True, "result": {"kind": kind}}

    def _probe(self, action, output_path):
        payload = json.dumps(self.fixture, sort_keys=True).encode("utf-8")
        if self.fixture_location is not None:
            path = Path(self.fixture_location)
        else:
            path = self.directory / output_path / "artifacts" / "fixture.json"
        if self.write_fixture:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(payload)
        return {
            "kind": "probe",
            "ok": True,
            "result": {
                "name": "probe_target",
                "summary": {"outcome": self.outcome, "coverage": 0.5},
            },
            "artifacts": {
                "artifacts/fixture.json": {
                    "path": str(path),
                    "sha256": hashlib.sha256(payload).hexdigest(),
                    "kind": "fixture",
                }
            },
            "artifact_ids": ["artifacts/fixture.json"],
        }


def job_state(directory, job_id="alpha"):
    return provider_jobs.load(directory)["jobs"][job_id]


def step(directory, harness, clock):
    """Run exactly one campaign step and return the actor calls it made."""
    before = len(harness.contexts)
    pc.run(
        directory,
        actor=harness.actor,
        execute=harness.execute,
        clock=clock,
        once=True,
    )
    return harness.contexts[before:]


def fixture_file(directory, action_id="a0001"):
    return (
        Path(directory)
        / "jobs"
        / "alpha"
        / "rounds"
        / "1"
        / f"{action_id}-output"
        / "artifacts"
        / "fixture.json"
    )


def test_previous_probe_action_and_fixture_reach_the_next_owner(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory)
    clock = Clock()
    action = probe_action(base="a0000/artifacts/fixture.json")
    harness.script_actions(action)

    step(directory, harness, clock)
    contexts = step(directory, harness, clock)

    assert len(contexts) == 1
    context = contexts[0]
    assert len(context["results"]) == 1
    projection = context["results"][0]
    assert projection["id"] == "a0001"
    assert projection["kind"] == "probe"
    assert projection["action"] == action
    assert projection["action"]["request"] == REQUEST
    assert projection["action"]["request"]["patch"] == PATCH
    assert projection["action"]["request"]["note"] == "n" * 400
    assert projection["action"]["base"] == "a0000/artifacts/fixture.json"

    fixture = context["working_fixture"]
    assert fixture["artifact"] == "a0001/artifacts/fixture.json"
    assert fixture["data"] == FIXTURE
    assert fixture["pin"]["sha256"] == hashlib.sha256(
        json.dumps(FIXTURE, sort_keys=True).encode("utf-8")
    ).hexdigest()
    assert fixture["pin"]["path"].replace("\\", "/").endswith(
        "a0001-output/artifacts/fixture.json"
    )
    assert any("working_fixture.artifact" in item for item in context["guidance"])
    assert any("original seed" in item for item in context["guidance"])


@pytest.mark.parametrize("outcome", ["fault", "budget_exhausted", "return"])
def test_a_faulting_probe_is_still_the_next_input(tmp_path, outcome):
    directory = make_campaign(tmp_path, name=f"camp-{outcome}")
    harness = Harness(directory, outcome=outcome)
    clock = Clock()
    harness.script_actions(probe_action())

    step(directory, harness, clock)
    contexts = step(directory, harness, clock)

    context = contexts[0]
    assert context["results"][0]["summary"]["outcome"] == outcome
    assert context["working_fixture"]["data"] == FIXTURE
    assert context["working_fixture"]["artifact"] == "a0001/artifacts/fixture.json"


def test_probe_fixture_survives_more_than_eight_later_actions(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory)
    clock = Clock()
    harness.script_actions(probe_action())
    harness.script_actions(*[evidence_action() for _ in range(9)])

    pc.run(directory, actor=harness.actor, execute=harness.execute, clock=clock)

    context = harness.contexts[-1]
    assert len(context["results"]) == 8
    assert all(item["kind"] == "evidence" for item in context["results"])
    assert context["retained_results"] > 8
    assert context["working_fixture"]["data"] == FIXTURE
    assert context["working_fixture"]["artifact"] == "a0001/artifacts/fixture.json"


def test_tampered_fixture_rejects_before_the_next_actor(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory)
    clock = Clock()
    harness.script_actions(probe_action())

    step(directory, harness, clock)
    path = fixture_file(directory)
    path.write_bytes(json.dumps({"tampered": True}).encode("utf-8"))

    calls_before = len(harness.contexts)
    pc.run(directory, actor=harness.actor, execute=harness.execute, clock=clock)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "context unavailable" in job["hold_reason"]
    assert "fixture hash mismatch" in job["hold_reason"]
    assert len(harness.contexts) == calls_before


def test_missing_fixture_file_rejects_before_the_next_actor(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory, write_fixture=False)
    clock = Clock()
    harness.script_actions(probe_action())

    step(directory, harness, clock)
    calls_before = len(harness.contexts)
    pc.run(directory, actor=harness.actor, execute=harness.execute, clock=clock)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "probe fixture is missing" in job["hold_reason"]
    assert len(harness.contexts) == calls_before


def test_fixture_descriptor_outside_the_job_root_is_rejected(tmp_path):
    directory = make_campaign(tmp_path)
    outside = tmp_path / "outside" / "fixture.json"
    harness = Harness(directory, fixture_location=outside)
    clock = Clock()
    harness.script_actions(probe_action())

    step(directory, harness, clock)
    calls_before = len(harness.contexts)
    pc.run(directory, actor=harness.actor, execute=harness.execute, clock=clock)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "escapes the job root" in job["hold_reason"]
    assert len(harness.contexts) == calls_before


def test_tampered_probe_action_record_is_rejected(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory)
    clock = Clock()
    harness.script_actions(probe_action())

    step(directory, harness, clock)
    path = directory / "jobs" / "alpha" / "a0001.json"
    record = json.loads(path.read_text(encoding="utf-8"))
    record["action"]["request"]["patch"]["regs"]["eax"] = "0xdead"
    path.write_text(json.dumps(record), encoding="utf-8")

    calls_before = len(harness.contexts)
    pc.run(directory, actor=harness.actor, execute=harness.execute, clock=clock)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "hash mismatch" in job["hold_reason"]
    assert len(harness.contexts) == calls_before


def test_context_without_a_probe_has_no_working_fixture(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory)
    clock = Clock()
    action = evidence_action(start=16, limit=64)
    harness.script_actions(action)

    step(directory, harness, clock)
    contexts = step(directory, harness, clock)

    context = contexts[0]
    assert "working_fixture" not in context
    assert context["results"][0]["action"] == action
    assert context["results"][0]["kind"] == "evidence"


def test_a_failed_request_keeps_its_exact_action(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory, failing_kinds={"evidence"})
    clock = Clock()
    action = evidence_action(start=500, limit=2000)
    harness.script_actions(action)

    step(directory, harness, clock)
    contexts = step(directory, harness, clock)

    projection = contexts[0]["results"][0]
    assert projection["ok"] is False
    assert projection["action"] == action
    assert projection["action"]["start"] == 500
    assert projection["action"]["limit"] == 2000
    assert projection["guidance"]


def test_the_projection_does_not_duplicate_the_actor_response(tmp_path):
    directory = make_campaign(tmp_path)
    harness = Harness(directory)
    clock = Clock()
    harness.script_actions(evidence_action())

    step(directory, harness, clock)
    contexts = step(directory, harness, clock)

    context = contexts[0]
    assert "actor_response" not in context["results"][0]
    assert "receipt" not in context["results"][0]
    assert MARKER not in json.dumps(context)
