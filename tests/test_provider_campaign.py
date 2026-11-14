"""Asset-free tests for durable provider campaign orchestration.

The fixtures follow the production broker protocol: actor actions carry only
the documented keys for their kind, executor responses are
``{kind, ok, result, artifacts, artifact_ids}`` and artifact identifiers are
action ids (``a0001``) or action prefixed file ids
(``a0001/artifacts/raw.json.gz``).
"""

from __future__ import annotations

import hashlib
import json
import sys
import threading
import time
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre import provider_campaign as pc  # noqa: E402
from cmre import provider_jobs  # noqa: E402
from cmre.coordination import lock  # noqa: E402

MODELS = {
    "deepseek": "deepseek-observed-v1",
    "minimax": "minimax-observed-v2",
}
FILE_ID = "a0001/artifacts/raw.json.gz"


class FakeClock:
    def __init__(self, value=1000.0):
        self.value = float(value)

    def __call__(self):
        return self.value

    def sleep(self, seconds):
        self.value += float(seconds)


def make_manifest(
    jobs=None,
    *,
    mode="input",
    concurrency=2,
    duration=60,
    max_rounds=30,
    models=None,
):
    if jobs is None:
        jobs = [
            {
                "id": "alpha",
                "config": {
                    "function": "0x401000",
                    "mode": mode,
                    "goal": "implement alpha",
                    "max_rounds": max_rounds,
                },
            }
        ]
    roles = dict(models or MODELS)
    return {
        "schema": pc.SCHEMA,
        "duration_seconds": duration,
        "concurrency": concurrency,
        "roles": {role: {"model": roles[role]} for role in ("deepseek", "minimax")},
        "jobs": jobs,
    }


def init_campaign(tmp_path, manifest=None, identity=None, name="camp"):
    directory = tmp_path / name
    pc.initialize(
        directory,
        manifest or make_manifest(),
        identity if identity is not None else {"provider": "deepseek"},
    )
    return directory


def clone(value):
    return json.loads(json.dumps(value))


def evidence(function="0x401000", **extra):
    action = {"kind": "evidence", "function": function}
    action.update(extra)
    return action


def memory(address=0x401000, size=16):
    return {"kind": "memory", "address": address, "size": size}


def probe(request=None, base=None):
    action = {"kind": "probe", "request": request or {"entry": "0x401010"}}
    if base is not None:
        action["base"] = base
    return action


def inspect(artifact=FILE_ID, section="disassembly", **extra):
    action = {"kind": "inspect", "artifact": artifact, "section": section}
    action.update(extra)
    return action


def build():
    return {"kind": "build"}


def screen():
    return {"kind": "screen"}


def run_test():
    return {"kind": "test"}


def controls():
    return {"kind": "controls"}


def propose(artifacts=("a0001",), summary="candidate ready"):
    return {"kind": "propose", "artifacts": list(artifacts), "summary": summary}


def hold(reason="needs human triage"):
    return {"kind": "hold", "reason": reason}


def pass_review(context, job):
    return {
        "kind": "review",
        "verdict": "pass",
        "bundle_sha256": context["bundle"]["sha256"],
        "checks": list(context.get("required_checks", [])),
        "unknowns": [],
    }


def revise_review(feedback="repair the failing path"):
    def build_action(context, job):
        return {
            "kind": "review",
            "verdict": "revise",
            "bundle_sha256": context["bundle"]["sha256"],
            "checks": list(context.get("required_checks", [])),
            "unknowns": [],
            "reason": feedback,
        }

    return build_action


class Response:
    def __init__(self, action, model=None, receipt=None):
        self.action = action
        self.model = model
        self.receipt = receipt


class RawResponse:
    def __init__(self, payload):
        self.payload = payload


class Backend:
    def __init__(
        self,
        owner_actions,
        review_actions,
        *,
        mode="input",
        delay=0.0,
        bundle_ready=None,
        execute_error=None,
        execute_hook=None,
    ):
        self.owner_actions = owner_actions
        self.review_actions = review_actions
        self.mode = mode
        self.delay = delay
        self.bundle_ready = list(bundle_ready or [])
        self.execute_error = execute_error
        self.execute_hook = execute_hook
        self.actor_calls = []
        self.execute_calls = []
        self.lock = threading.Lock()
        self.active = 0
        self.max_active = 0
        self.active_jobs = set()
        self.duplicate_jobs = False

    def _script(self, job, phase):
        source = self.owner_actions if phase == "owner" else self.review_actions
        if isinstance(source, dict):
            return source[job["id"]]
        return source

    def actor(self, job, role, context, stage_path):
        with self.lock:
            self.active += 1
            self.max_active = max(self.max_active, self.active)
            if job["id"] in self.active_jobs:
                self.duplicate_jobs = True
            self.active_jobs.add(job["id"])
            self.actor_calls.append(
                {
                    "job": clone(job),
                    "role": role,
                    "phase": context["phase"],
                    "context": clone(context),
                    "stage": str(stage_path),
                }
            )
        if self.delay:
            time.sleep(self.delay)
        script = self._script(job, context["phase"])
        if not script:
            raise RuntimeError(f"no scripted action for {job['id']} {context['phase']}")
        item = script.pop(0)
        if callable(item) and not isinstance(item, (Response, RawResponse)):
            item = item(context, job)
        if isinstance(item, RawResponse):
            return clone(item.payload)
        if isinstance(item, Response):
            action = item.action
            if callable(action) and not isinstance(action, dict):
                action = action(context, job)
            payload = {
                "action": clone(action),
                "model": item.model or MODELS[role],
            }
            if item.receipt is not None:
                payload["receipt"] = clone(item.receipt)
            return payload
        return {
            "action": clone(item),
            "model": MODELS[role],
            "receipt": {"role": role},
        }

    def execute(self, job, action, output_path, prior=None):
        with self.lock:
            self.execute_calls.append(
                {
                    "job": job["id"],
                    "action": clone(action),
                    "output_path": str(output_path),
                    "prior": clone(prior or []),
                }
            )
        try:
            if self.execute_error is not None:
                raise self.execute_error
            result = self._result(job, action, output_path)
            if self.execute_hook is not None:
                hooked = self.execute_hook(action, result)
                if hooked is not None:
                    result = hooked
            return result
        finally:
            with self.lock:
                self.active -= 1
                self.active_jobs.discard(job["id"])

    def _result(self, job, action, output_path):
        kind = action.get("kind")
        if kind == "bundle":
            sha = hashlib.sha256(
                json.dumps(
                    {"artifacts": action["artifacts"], "summary": action["summary"]},
                    sort_keys=True,
                ).encode("utf-8")
            ).hexdigest()
            ready = self.bundle_ready.pop(0) if self.bundle_ready else True
            result = {
                "ready": ready,
                "sha256": sha,
                "required_checks": ["pc", "coverage"],
                "payload": {
                    "artifacts": list(action["artifacts"]),
                    "summary": action["summary"],
                    "checks": ["pc", "coverage"],
                },
            }
            if not ready:
                result["feedback"] = "missing snapshot"
            return result
        if kind == "finalize":
            return {
                "state": "ready" if self.mode == "input" else "accepted",
                "bundle_sha256": action["bundle"]["sha256"],
            }
        if kind == "probe":
            return {
                "kind": "probe",
                "ok": True,
                "result": {
                    "name": "probe_target",
                    "function": job["config"]["function"],
                    "summary": {
                        "outcome": "return",
                        "detail": [],
                        "regs": {"eax": "0x65"},
                        "covered_pcs": ["0x004d1600"],
                        "coverage": 0.02,
                        "import_count": 0,
                        "return_fp": [895, 0],
                        "arrivals": [],
                        "returns": [],
                        "execution_identity": {},
                    },
                },
                "artifacts": {
                    "artifacts/raw.json.gz": {
                        "path": str(Path(output_path) / "raw.json.gz"),
                        "sha256": "ab" * 32,
                        "kind": "raw",
                    }
                },
                "artifact_ids": ["artifacts/raw.json.gz"],
            }
        if kind == "evidence":
            return {
                "kind": "evidence",
                "ok": True,
                "result": {
                    "function": job["config"]["function"],
                    "instructions": [{"address": "0x004d1600", "text": "push ebp"}],
                    "body_bytes": 32,
                    "callees": ["0x004d1700"],
                    "total": 1,
                    "remaining": 0,
                },
            }
        if kind == "memory":
            return {
                "kind": "memory",
                "ok": True,
                "result": {
                    "address": action["address"],
                    "size": action["size"],
                    "data": "00" * action["size"],
                },
            }
        if kind == "inspect":
            return {
                "kind": "inspect",
                "ok": True,
                "result": {
                    "section": action["section"],
                    "items": [{"address": "0x004d1600", "text": "push ebp"}],
                    "total": 1,
                    "remaining": 0,
                },
            }
        if kind == "test":
            return {
                "kind": "test",
                "ok": True,
                "source_sha256": "cd" * 32,
                "task_id": "task-1",
                "report_status": "pass",
                "first_failure": None,
                "artifacts": {
                    "artifacts/report.json": {
                        "path": str(Path(output_path) / "report.json"),
                        "sha256": "ef" * 32,
                        "kind": "report",
                    }
                },
            }
        return {"kind": kind, "ok": True, "result": {"kind": kind}}


def job_state(directory, job_id="alpha"):
    return provider_jobs.load(directory)["jobs"][job_id]


def owner_call(backend):
    return [call for call in backend.actor_calls if call["phase"] == "owner"]


def review_calls(backend):
    return [call for call in backend.actor_calls if call["phase"] == "review"]


def test_initialize_starts_no_clock_and_validates(tmp_path):
    directory = init_campaign(tmp_path)
    assert not (directory / "run.json").exists()
    summary = pc.status(directory)
    assert summary["started_at"] is None
    assert summary["deadline"] is None
    assert summary["elapsed_seconds"] == 0.0
    assert summary["counts"]["pending"] == 1

    bad = make_manifest()
    bad["duration_seconds"] = True
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad1", bad, {})

    bad = make_manifest()
    bad["concurrency"] = True
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad2", bad, {})

    bad = make_manifest(models={"deepseek": "same", "minimax": "same"})
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad3", bad, {})

    bad = make_manifest(
        jobs=[
            {"id": "x", "config": {"function": "0x1", "mode": "input", "goal": "g"}},
            {"id": "x", "config": {"function": "0x2", "mode": "input", "goal": "g"}},
        ]
    )
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad4", bad, {})

    bad = make_manifest()
    bad["jobs"][0]["config"]["max_rounds"] = True
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad5", bad, {})

    bad = make_manifest()
    bad["jobs"][0]["config"]["owner"] = "minimax"
    bad["jobs"][0]["config"]["reviewer"] = "minimax"
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad6", bad, {})

    bad = make_manifest()
    bad["jobs"][0]["id"] = "Not-Safe"
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad7", bad, {})

    bad = make_manifest()
    bad["jobs"][0]["config"]["required_pcs"] = "0x401000"
    with pytest.raises(pc.InvalidManifest):
        pc.initialize(tmp_path / "bad8", bad, {})


def test_full_flow_evidence_probe_propose_review_ready(tmp_path):
    clock = FakeClock()
    backend = Backend(
        [
            evidence(),
            probe(base=FILE_ID),
            propose(("a0002",)),
        ],
        [pass_review],
    )
    directory = init_campaign(tmp_path, make_manifest(duration=60))
    result = pc.run(
        directory,
        actor=backend.actor,
        execute=backend.execute,
        identity={"provider": "deepseek"},
        clock=clock,
    )

    job = job_state(directory)
    assert job["state"] == "ready"
    assert job["phase"] == "done"
    assert job["author_model"] == MODELS["deepseek"]
    assert job["review_model"] == MODELS["minimax"]
    assert job["probes"] == 1
    assert job["round"] == 1
    assert job["report"]["final_state"] == "ready"
    assert result["executed"] == 4
    assert result["counts"]["ready"] == 1

    review = review_calls(backend)[0]
    assert review["context"]["phase"] == "review"
    assert review["context"]["bundle"]["sha256"]
    assert review["context"]["bundle"]["payload"]["checks"] == ["pc", "coverage"]
    assert review["context"]["required_checks"] == ["pc", "coverage"]
    assert review["context"]["remaining_seconds"] > 0
    bundle_projection = [
        item for item in review["context"]["results"] if item["kind"] == "bundle"
    ]
    assert bundle_projection
    assert bundle_projection[0]["ready"] is True
    assert bundle_projection[0]["payload"]["checks"] == ["pc", "coverage"]
    assert bundle_projection[0]["required_checks"] == ["pc", "coverage"]

    finalize = [c for c in backend.execute_calls if c["action"]["kind"] == "finalize"]
    assert finalize and finalize[0]["action"]["review_model"] == MODELS["minimax"]
    result_files = sorted((directory / "jobs" / "alpha").glob("a*.json"))
    assert len(result_files) == 4
    record = json.loads(result_files[-1].read_text(encoding="utf-8"))
    assert record["output_path"].endswith("a0004-output")
    assert record["actor_response"]["model"] == MODELS["minimax"]

    status = pc.status(directory)
    assert status["started_at"] == 1000.0
    assert status["deadline"] == 1060.0
    assert status["counts"]["ready"] == 1


def test_c_mode_accepts_via_finalize(tmp_path):
    backend = Backend([propose()], [pass_review], mode="c")
    directory = init_campaign(
        tmp_path,
        make_manifest(
            jobs=[
                {
                    "id": "alpha",
                    "config": {
                        "function": "0x402000",
                        "mode": "c",
                        "goal": "recover alpha",
                    },
                }
            ]
        ),
    )
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "accepted"
    assert job["report"]["final_state"] == "accepted"
    assert job["config"]["owner"] == "minimax"
    assert job["config"]["reviewer"] == "deepseek"


def test_reviewer_revise_then_author_repair(tmp_path):
    backend = Backend(
        [
            propose(("a0001",)),
            evidence(),
            propose(("a0003",)),
        ],
        [revise_review("fix missing branch"), pass_review],
    )
    directory = init_campaign(tmp_path)
    result = pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "ready"
    assert job["round"] == 2
    assert "fix missing branch" in job["feedback"]
    assert len(review_calls(backend)) == 2
    assert len(owner_call(backend)) == 3
    assert result["executed"] == 5
    assert job["bundle"]["payload"]["artifacts"] == ["a0003"]
    assert job["reviews"] == 2


def test_rejected_bundle_returns_feedback_without_ending_job(tmp_path):
    backend = Backend(
        [propose(("a0001",)), propose(("a0002",))],
        [pass_review],
        bundle_ready=[False, True],
    )
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "ready"
    assert job["round"] == 1
    assert "missing snapshot" in job["feedback"]
    assert len(owner_call(backend)) == 2


def test_reject_wrong_model_missing_checks_stale_bundle(tmp_path):
    def missing_checks(context, job):
        return {
            "kind": "review",
            "verdict": "pass",
            "bundle_sha256": context["bundle"]["sha256"],
            "checks": [],
            "unknowns": [],
        }

    def stale(context, job):
        action = pass_review(context, job)
        action["bundle_sha256"] = "0" * 64
        return action

    reviews = [
        Response(pass_review, model="not-a-configured-model"),
        missing_checks,
        stale,
    ]
    expectations = [
        "model mismatch",
        "missing required checks",
        "stale bundle",
    ]
    for index, (review, expected) in enumerate(zip(reviews, expectations)):
        backend = Backend([propose()], [review])
        directory = init_campaign(tmp_path, name=f"camp{index}")
        pc.run(directory, actor=backend.actor, execute=backend.execute)
        job = job_state(directory)
        assert job["state"] == "held", (index, job)
        assert expected in job["hold_reason"]
        assert job["escalation"]["reason"] == job["hold_reason"]


def test_pass_with_unknowns_and_forbidden_actions(tmp_path):
    def pass_unknown(context, job):
        action = pass_review(context, job)
        action["unknowns"] = ["unresolved path"]
        return action

    backend = Backend([propose()], [pass_unknown])
    directory = init_campaign(tmp_path, name="unknowns")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    assert job_state(directory)["state"] == "held"
    assert "unknowns" in job_state(directory)["hold_reason"]

    backend = Backend([{"kind": "review", "verdict": "pass"}], [])
    directory = init_campaign(tmp_path, name="owner-review")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    assert job_state(directory)["state"] == "held"
    assert "unsupported owner action" in job_state(directory)["hold_reason"]
    assert not backend.execute_calls

    for kind in ("build", "test", "controls"):
        backend = Backend([propose()], [{"kind": kind}])
        directory = init_campaign(tmp_path, name=f"reviewer-{kind}")
        pc.run(directory, actor=backend.actor, execute=backend.execute)
        assert job_state(directory)["state"] == "held"
        assert "forbidden reviewer action" in job_state(directory)["hold_reason"]

    backend = Backend([propose()], [{"kind": "propose", "artifacts": [], "summary": "x"}])
    directory = init_campaign(tmp_path, name="reviewer-propose")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    assert job_state(directory)["state"] == "held"
    assert "forbidden reviewer action" in job_state(directory)["hold_reason"]


def test_input_owner_may_not_build_test_or_control(tmp_path):
    for kind in ("build", "test", "controls"):
        backend = Backend([{"kind": kind}], [])
        directory = init_campaign(tmp_path, name=f"input-owner-{kind}")
        pc.run(directory, actor=backend.actor, execute=backend.execute)
        job = job_state(directory)
        assert job["state"] == "held"
        assert "forbidden input owner action" in job["hold_reason"]
        assert not backend.execute_calls


def test_c_owner_may_build_test_and_control(tmp_path):
    backend = Backend([build(), run_test(), controls()], [])
    directory = init_campaign(
        tmp_path,
        make_manifest(
            jobs=[
                {
                    "id": "alpha",
                    "config": {
                        "function": "0x402000",
                        "mode": "c",
                        "goal": "recover alpha",
                    },
                }
            ]
        ),
        name="c-owner-actions",
    )
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    refs = job["results"]
    assert [ref["id"] for ref in refs][:3] == ["a0001", "a0002", "a0003"]
    assert all(ref["ok"] for ref in refs[:3])
    assert len(backend.execute_calls) == 3


def test_bad_action_and_adapter_errors_are_durable_held(tmp_path):
    backend = Backend([{"kind": "shell", "command": "rm -rf /"}], [])
    directory = init_campaign(tmp_path, name="bad-action")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    assert job_state(directory)["state"] == "held"
    assert "unsupported owner action" in job_state(directory)["hold_reason"]
    assert not backend.execute_calls

    backend = Backend([RawResponse({"action": "PASS", "model": MODELS["deepseek"]})], [])
    directory = init_campaign(tmp_path, name="prose")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    assert job_state(directory)["state"] == "held"
    assert "structured object" in job_state(directory)["hold_reason"]

    backend = Backend([RuntimeError("provider crashed")], [])
    directory = init_campaign(tmp_path, name="actor-error")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "actor error" in job["hold_reason"]
    assert job["escalation"]["adapter"] == "actor"
    assert job["checkpoint"] is None

    backend = Backend([evidence()], [], execute_error=RuntimeError("boom"))
    directory = init_campaign(tmp_path, name="execute-error")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "execute error" in job["hold_reason"]
    assert job["escalation"]["adapter"] == "execute"

    backend = Backend([hold("needs human triage")], [])
    directory = init_campaign(tmp_path, name="owner-hold")
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert job["hold_reason"] == "needs human triage"


def test_self_approval_guard_rejects_equal_observed_model(tmp_path):
    backend = Backend([propose()], [pass_review])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute, once=True)
    job = job_state(directory)
    assert job["state"] == "needs-review"
    job = provider_jobs.update(
        directory,
        "alpha",
        {"author_model": MODELS["minimax"]},
        expected_revision=job["revision"],
    )
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "self approval" in job["hold_reason"]


def test_reviewer_readonly_evidence_action_is_allowed(tmp_path):
    backend = Backend([propose()], [evidence(), pass_review])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "ready"
    evidence_calls = [
        call for call in backend.execute_calls if call["action"]["kind"] == "evidence"
    ]
    assert len(evidence_calls) == 1


def test_held_retry_preserves_rounds_and_attempts(tmp_path):
    backend = Backend([evidence(), probe(base=FILE_ID), hold("wait for input")], [])
    directory = init_campaign(tmp_path, make_manifest(max_rounds=5))
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    attempts_before = job["attempts"]
    assert attempts_before == 3
    assert [ref["id"] for ref in job["results"]] == ["a0001", "a0002", "a0003"]
    assert job["results"][2]["ok"] is False
    assert job["results"][2]["kind"] == "hold"

    provider_jobs.update(
        directory,
        "alpha",
        {"state": "prepared", "retry_reason": "input supplied"},
        expected_revision=job["revision"],
    )
    retry = Backend([evidence(), propose(("a0005",))], [pass_review])
    pc.run(directory, actor=retry.actor, execute=retry.execute)
    job = job_state(directory)
    assert job["state"] == "ready"
    assert job["round"] == 1
    assert job["attempts"] == attempts_before + 3
    assert [ref["id"] for ref in job["results"]][:3] == ["a0001", "a0002", "a0003"]


def test_terminal_jobs_are_not_dispatched_and_opaque_config_survives(tmp_path):
    jobs = [
        {
            "id": "alpha",
            "config": {
                "function": "0x401000",
                "mode": "input",
                "goal": "goal",
                "seed_fixture": "fixture-1",
                "min_coverage": 0.75,
            },
        }
    ]
    backend = Backend([propose()], [pass_review])
    directory = init_campaign(tmp_path, make_manifest(jobs=jobs))
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["config"]["seed_fixture"] == "fixture-1"
    assert job["config"]["min_coverage"] == 0.75
    assert backend.actor_calls[0]["job"]["config"]["seed_fixture"] == "fixture-1"

    calls = len(backend.actor_calls)
    idle = Backend([], [])
    result = pc.run(directory, actor=idle.actor, execute=idle.execute)
    assert result["executed"] == 0
    assert idle.actor_calls == []
    assert len(backend.actor_calls) == calls
    assert pc.status(directory)["counts"]["ready"] == 1


def test_deadline_stops_dispatch_and_resume_does_not_reset(tmp_path):
    clock = FakeClock(0.0)

    def advance(_action, _result):
        clock.value = 10.0
        return None

    backend = Backend([evidence()], [], execute_hook=advance)
    directory = init_campaign(tmp_path, make_manifest(duration=5))
    first = pc.run(
        directory, actor=backend.actor, execute=backend.execute, clock=clock
    )
    assert first["executed"] == 1
    assert first["stopped"] == "deadline"
    assert job_state(directory)["state"] == "prepared"
    metadata = json.loads((directory / "run.json").read_text(encoding="utf-8"))
    assert metadata["started_at"] == 0.0
    assert metadata["deadline"] == 5.0
    calls_before = len(backend.actor_calls)

    clock.value = 100.0
    second = pc.run(
        directory, actor=backend.actor, execute=backend.execute, clock=clock
    )
    assert second["executed"] == 0
    assert second["stopped"] == "deadline"
    assert len(backend.actor_calls) == calls_before
    assert job_state(directory)["state"] == "prepared"
    metadata_after = json.loads((directory / "run.json").read_text(encoding="utf-8"))
    assert metadata_after == metadata
    summary = pc.status(directory)
    assert summary["started_at"] == 0.0
    assert summary["deadline"] == 5.0


def test_interrupted_running_is_held_and_stage_retained(tmp_path):
    directory = init_campaign(tmp_path)
    provider_jobs.update(
        directory,
        "alpha",
        {
            "state": "running",
            "phase": "owner",
            "round": 1,
            "checkpoint": {
                "stage": "jobs/alpha/rounds/1/a0001-owner",
                "action": "a0001",
                "execute_stage": "jobs/alpha/rounds/1/a0001-output",
            },
        },
        expected_revision=0,
    )
    stage = directory / "jobs" / "alpha" / "rounds" / "1" / "a0001-owner"
    stage.mkdir(parents=True)
    retained = stage / "retained.txt"
    retained.write_text("do not rerun", encoding="utf-8")

    backend = Backend([evidence()], [])
    result = pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "interrupted" in job["hold_reason"]
    assert job["escalation"]["checkpoint"]["stage"].endswith("a0001-owner")
    assert backend.actor_calls == []
    assert result["executed"] == 0
    assert retained.read_text(encoding="utf-8") == "do not rerun"


def test_wrong_identity_rejected_before_actor_or_executor(tmp_path):
    backend = Backend([evidence()], [])
    directory = init_campaign(tmp_path, identity={"provider": "deepseek"})
    with pytest.raises(pc.CampaignError):
        pc.run(
            directory,
            actor=backend.actor,
            execute=backend.execute,
            identity={"provider": "minimax"},
        )
    assert backend.actor_calls == []
    assert backend.execute_calls == []
    assert job_state(directory)["state"] == "prepared"
    assert not (directory / "run.json").exists()


def test_once_executes_one_action_globally(tmp_path):
    backend = Backend([evidence(), propose(("a0002",))], [pass_review])
    directory = init_campaign(tmp_path)
    first = pc.run(
        directory, actor=backend.actor, execute=backend.execute, once=True
    )
    assert first["executed"] == 1
    job = job_state(directory)
    assert job["state"] == "prepared"
    assert job["phase"] == "owner"
    assert len(list((directory / "jobs" / "alpha").glob("a*.json"))) == 1

    second = pc.run(
        directory, actor=backend.actor, execute=backend.execute, once=True
    )
    assert second["executed"] == 1
    assert job_state(directory)["state"] == "needs-review"


def test_round_budget_exhausts_without_spinning(tmp_path):
    backend = Backend(
        [propose((f"a{index:04d}",)) for index in range(10)],
        [revise_review("still wrong") for _ in range(10)],
    )
    directory = init_campaign(tmp_path, make_manifest(max_rounds=2))
    result = pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "exhausted"
    assert "max_rounds" in job["hold_reason"]
    assert job["round"] == 2
    assert len(owner_call(backend)) == 2
    assert len(review_calls(backend)) == 2
    assert result["executed"] == 4


def test_bounded_concurrency_and_unique_job_dispatch(tmp_path):
    job_ids = ["j1", "j2", "j3"]
    jobs = [
        {
            "id": job_id,
            "config": {
                "function": "0x401000",
                "mode": "input",
                "goal": f"goal {job_id}",
            },
        }
        for job_id in job_ids
    ]
    backend = Backend(
        {job_id: [propose(("a0001",))] for job_id in job_ids},
        {job_id: [pass_review] for job_id in job_ids},
        delay=0.05,
    )
    directory = init_campaign(
        tmp_path, make_manifest(jobs=jobs, concurrency=2, duration=60)
    )
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    state = provider_jobs.load(directory)
    assert all(job["state"] == "ready" for job in state["jobs"].values())
    assert backend.max_active <= 2
    assert backend.duplicate_jobs is False
    assert len(backend.actor_calls) == 6


def test_duplicate_controller_is_prevented(tmp_path):
    gate = threading.Event()
    entered = threading.Event()
    directory = init_campaign(tmp_path)

    def first_actor(job, role, context, stage_path):
        if context["phase"] == "owner":
            entered.set()
            if not gate.wait(5):
                raise RuntimeError("gate timeout")
            action = propose()
        else:
            action = pass_review(context, job)
        return {"action": action, "model": MODELS[role]}

    def first_execute(job, action, output_path, prior=None):
        if action["kind"] == "bundle":
            sha = hashlib.sha256(
                json.dumps(
                    {
                        "artifacts": action["artifacts"],
                        "summary": action["summary"],
                    },
                    sort_keys=True,
                ).encode("utf-8")
            ).hexdigest()
            return {
                "ready": True,
                "sha256": sha,
                "required_checks": ["pc"],
                "payload": {"artifacts": action["artifacts"]},
            }
        if action["kind"] == "finalize":
            return {"state": "ready"}
        return {"kind": action["kind"], "ok": True, "result": {}}

    second_calls = []

    def second_actor(job, role, context, stage_path):
        second_calls.append(context["phase"])
        return {"action": propose(), "model": MODELS[role]}

    errors = []

    def start(target):
        def wrapped():
            try:
                target()
            except Exception as exc:
                errors.append(exc)

        return wrapped

    first = threading.Thread(
        target=start(
            lambda: pc.run(directory, actor=first_actor, execute=first_execute)
        )
    )
    first.start()
    assert entered.wait(5)

    second = threading.Thread(
        target=start(
            lambda: pc.run(directory, actor=second_actor, execute=first_execute)
        )
    )
    second.start()
    time.sleep(0.25)
    assert second_calls == []
    gate.set()
    first.join(10)
    second.join(10)
    assert not first.is_alive()
    assert not second.is_alive()
    assert second_calls == []
    assert job_state(directory)["state"] == "ready"


class Retryable(RuntimeError):
    """Adapter failure that explicitly asks for one bounded retry."""

    retryable = True


def test_context_results_are_structured_and_bounded(tmp_path):
    def enrich(action, result):
        if action.get("kind") != "probe":
            return None
        result["result"]["summary"].update(
            {
                "covered_pcs": [f"0x{index:08x}" for index in range(200)],
                "detail": [{"pc": f"0x{index:08x}"} for index in range(200)],
                "arrivals": [{"pc": "0x1"}] * 50,
                "returns": [{"pc": "0x2"}] * 50,
                "import_count": 7,
            }
        )
        return result

    jobs = [
        {
            "id": "alpha",
            "config": {
                "function": "0x401000",
                "mode": "input",
                "goal": "implement alpha",
                "required_pcs": ["0x00000003", "0x00009999"],
            },
        }
    ]
    backend = Backend(
        [probe(base=FILE_ID), evidence()],
        [],
        execute_hook=enrich,
    )
    directory = init_campaign(tmp_path, make_manifest(jobs=jobs))
    pc.run(directory, actor=backend.actor, execute=backend.execute)

    context = backend.actor_calls[1]["context"]
    assert len(context["results"]) == 1
    projection = context["results"][0]
    assert projection["id"] == "a0001"
    assert projection["kind"] == "probe"
    assert projection["response_kind"] == "probe"
    assert projection["ok"] is True
    assert projection["summary"]["outcome"] == "return"
    assert projection["summary"]["regs"] == {"eax": "0x65"}
    assert projection["summary"]["coverage"] == 0.02
    assert projection["summary"]["import_count"] == 7
    assert projection["summary"]["pcs"]["count"] == 200
    assert len(projection["summary"]["pcs"]["own"]) == 120
    assert projection["summary"]["pcs"]["required_hits"] == ["0x00000003"]
    assert projection["summary"]["arrivals_count"] == 50
    assert projection["summary"]["returns_count"] == 50
    assert len(projection["summary"]["detail"]) == 20
    assert projection["artifacts"] == [FILE_ID]
    assert projection["artifact_files"][FILE_ID]["kind"] == "raw"
    assert projection["artifact_files"][FILE_ID]["sha256"] == "ab" * 32
    assert "FILE:" not in json.dumps(projection)


def test_evidence_instructions_are_bounded_and_memory_dump_is_not_projected(tmp_path):
    def enrich(action, result):
        if action.get("kind") == "evidence":
            result["result"]["instructions"] = [
                {"address": f"0x{index:08x}", "text": "x" * 40} for index in range(200)
            ]
        return result

    backend = Backend([evidence(), memory(), evidence()], [], execute_hook=enrich)
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)

    last = backend.actor_calls[2]["context"]["results"]
    assert len(last) == 2
    evidence_projection = last[0]
    assert evidence_projection["kind"] == "evidence"
    assert evidence_projection["response_kind"] == "evidence"
    assert len(evidence_projection["instructions"]) == 80
    assert evidence_projection["body_bytes"] == 32
    assert evidence_projection["total"] == 1
    assert evidence_projection["remaining"] == 0
    assert evidence_projection["callees"] == ["0x004d1700"]

    memory_projection = last[1]
    assert memory_projection["kind"] == "memory"
    assert memory_projection["address"] == 0x401000
    assert memory_projection["size"] == 16
    assert "data" not in memory_projection
    assert "0000" not in json.dumps(memory_projection)


def test_durable_results_keep_every_ref_but_context_is_capped(tmp_path):
    actions = [evidence() for _ in range(12)]
    backend = Backend(actions, [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)

    job = job_state(directory)
    ids = [ref["id"] for ref in job["results"]]
    assert ids[:12] == [f"a{index:04d}" for index in range(1, 13)]
    assert len(ids) == 13
    assert all(ref["hash"] and ref["path"] for ref in job["results"])
    context = backend.actor_calls[-1]["context"]
    assert len(context["results"]) == 8
    assert context["results"][0]["id"] == "a0005"
    assert context["results"][-1]["id"] == "a0012"


def test_store_lock_contention_is_retried(tmp_path):
    directory = init_campaign(tmp_path)
    backend = Backend([evidence(), evidence()], [])
    holding = threading.Event()

    def holder():
        with lock(directory / ".lock"):
            holding.set()
            time.sleep(0.3)

    thread = threading.Thread(target=holder)
    thread.start()
    assert holding.wait(5)
    result = pc.run(directory, actor=backend.actor, execute=backend.execute)
    thread.join(10)
    assert result["executed"] >= 2
    refs = job_state(directory)["results"]
    assert [ref["id"] for ref in refs][:2] == ["a0001", "a0002"]
    assert all(ref["ok"] for ref in refs[:2])


def test_actor_is_never_called_under_the_store_lock(tmp_path):
    directory = init_campaign(tmp_path)
    acquired = []

    def actor(job, role, context, stage_path):
        with lock(directory / ".lock"):
            acquired.append(context["phase"])
        return {"action": evidence(), "model": MODELS[role]}

    backend = Backend([], [])
    pc.run(directory, actor=actor, execute=backend.execute, once=True)
    assert acquired == ["owner"]


def test_retryable_actor_failure_retries_the_same_phase(tmp_path):
    calls = []
    backend = Backend([], [])

    def actor(job, role, context, stage_path):
        calls.append((role, context["phase"], context.get("retry")))
        if len(calls) == 1:
            raise Retryable("provider temporarily unavailable")
        if context["phase"] == "owner":
            return {"action": propose(), "model": MODELS[role]}
        return {"action": pass_review(context, job), "model": MODELS[role]}

    directory = init_campaign(tmp_path)
    pc.run(directory, actor=actor, execute=backend.execute)

    job = job_state(directory)
    assert job["state"] == "ready"
    assert job["provider_retries"] == 1
    assert job["attempts"] == 3
    assert calls[0][0] == "deepseek"
    assert calls[1][0] == "deepseek"
    assert calls[0][1] == calls[1][1] == "owner"
    assert calls[0][2] is None
    retry = calls[1][2]
    assert retry is not None
    assert retry["action"] == "a0001"
    assert "provider temporarily unavailable" in retry["reason"]
    assert retry["hint"]
    assert calls[2][0] == "minimax"
    assert calls[2][1] == "review"
    assert calls[2][2] is None
    assert job["last_failure"]["stage"].endswith("a0001-owner")
    assert job["failures"][0]["retryable"] is True


def test_retry_budget_is_bounded(tmp_path):
    calls = []

    def actor(job, role, context, stage_path):
        calls.append(context["phase"])
        raise Retryable("provider is down")

    backend = Backend([], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=actor, execute=backend.execute)

    job = job_state(directory)
    assert job["state"] == "held"
    assert job["provider_retries"] == 2
    assert len(job["failures"]) == 3
    assert job["attempts"] == 3
    assert calls == ["owner", "owner", "owner"]
    assert [ref["id"] for ref in job["results"]] == ["a0001", "a0002", "a0003"]


def test_non_retryable_actor_failure_holds_without_a_retry(tmp_path):
    def actor(job, role, context, stage_path):
        raise RuntimeError("provider crashed")

    backend = Backend([], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=actor, execute=backend.execute)

    job = job_state(directory)
    assert job["state"] == "held"
    assert job.get("provider_retries", 0) == 0
    assert job["last_failure"]["retryable"] is False


def test_run_json_tampering_is_rejected(tmp_path):
    backend = Backend([evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    path = directory / "run.json"

    original = json.loads(path.read_text(encoding="utf-8"))
    edited = dict(original)
    edited["deadline"] = original["deadline"] + 3600
    path.write_text(json.dumps(edited), encoding="utf-8")
    with pytest.raises(pc.CampaignError):
        pc.run(directory, actor=backend.actor, execute=backend.execute)
    with pytest.raises(pc.CampaignError):
        pc.status(directory)

    edited = dict(original)
    edited["manifest_hash"] = "0" * 64
    path.write_text(json.dumps(edited), encoding="utf-8")
    with pytest.raises(pc.CampaignError):
        pc.run(directory, actor=backend.actor, execute=backend.execute)


def test_run_json_binds_the_store_identity(tmp_path):
    backend = Backend([evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    path = directory / "run.json"
    original = json.loads(path.read_text(encoding="utf-8"))
    edited = dict(original)
    edited["identity_hash"] = "0" * 64
    path.write_text(json.dumps(edited), encoding="utf-8")
    with pytest.raises(pc.CampaignError):
        pc.run(directory, actor=backend.actor, execute=backend.execute)


def test_run_json_schema_and_duration_are_bound(tmp_path):
    backend = Backend([evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    path = directory / "run.json"
    original = json.loads(path.read_text(encoding="utf-8"))

    edited = dict(original)
    edited["schema"] = "cmre-provider-campaign-run/0"
    path.write_text(json.dumps(edited), encoding="utf-8")
    with pytest.raises(pc.CampaignError):
        pc.run(directory, actor=backend.actor, execute=backend.execute)

    edited = dict(original)
    edited["duration_seconds"] = original["duration_seconds"] + 1
    path.write_text(json.dumps(edited), encoding="utf-8")
    with pytest.raises(pc.CampaignError):
        pc.status(directory)

    edited = dict(original)
    edited["started_at"] = original["started_at"] + 5
    path.write_text(json.dumps(edited), encoding="utf-8")
    with pytest.raises(pc.CampaignError):
        pc.status(directory)


def test_unknown_action_key_is_rejected(tmp_path):
    backend = Backend([{"kind": "evidence", "summary": "ok", "secret": "value"}], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "unknown action key" in job["hold_reason"]


def test_documented_protocol_actions_are_accepted(tmp_path):
    actions = [
        evidence(),
        memory(),
        probe(base=FILE_ID),
        inspect(),
        screen(),
    ]
    backend = Backend(actions, [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    refs = job["results"]
    assert [ref["id"] for ref in refs][:5] == [
        f"a{index:04d}" for index in range(1, 6)
    ]
    assert all(ref["ok"] for ref in refs[:5])
    assert len(backend.execute_calls) == 5
    assert job["state"] == "held"
    assert "actor error" in job["hold_reason"]
    assert "unknown action key" not in job["hold_reason"]


def test_invented_action_fields_are_rejected(tmp_path):
    invented = [
        {"kind": "evidence", "summary": "ok"},
        {"kind": "probe", "target": "0x401010"},
        {"kind": "inspect", "artifact": FILE_ID, "address": "0x401010"},
        {"kind": "memory", "query": "0x401010"},
        {"kind": "screen", "artifact": "src/alpha.c"},
    ]
    for index, action in enumerate(invented):
        backend = Backend([action], [])
        directory = init_campaign(tmp_path, name=f"invented{index}")
        pc.run(directory, actor=backend.actor, execute=backend.execute)
        job = job_state(directory)
        assert job["state"] == "held", (index, job)
        assert "unknown action key" in job["hold_reason"]
        assert not backend.execute_calls


def test_malformed_protocol_values_are_rejected(tmp_path):
    malformed = [
        ({"kind": "memory", "address": [], "size": 16}, "must be an integer"),
        ({"kind": "probe", "request": "target"}, "must be a dict"),
        ({"kind": "inspect", "artifact": FILE_ID, "section": "nope"}, "invalid inspect section"),
        ({"kind": "propose", "artifacts": ["src/alpha.c"], "summary": "x"}, "action ids"),
        ({"kind": "hold", "reason": "  "}, "nonempty"),
        ({"kind": "evidence", "function": False}, "must be an integer"),
    ]
    for index, (action, expected) in enumerate(malformed):
        backend = Backend([action], [])
        directory = init_campaign(tmp_path, name=f"malformed{index}")
        pc.run(directory, actor=backend.actor, execute=backend.execute)
        job = job_state(directory)
        assert job["state"] == "held", (index, job)
        assert expected in job["hold_reason"]
        assert not backend.execute_calls


def test_tampered_action_record_is_detected_before_use(tmp_path):
    backend = Backend([evidence(), evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute, once=True)
    pc.run(directory, actor=backend.actor, execute=backend.execute, once=True)
    assert job_state(directory)["state"] == "prepared"

    path = directory / "jobs" / "alpha" / "a0001.json"
    record = json.loads(path.read_text(encoding="utf-8"))
    record["result"]["function"] = "0xdeadbeef"
    path.write_text(json.dumps(record), encoding="utf-8")

    follow = Backend([evidence()], [])
    pc.run(directory, actor=follow.actor, execute=follow.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "hash mismatch" in job["hold_reason"]


def test_action_reference_outside_the_job_root_is_rejected(tmp_path):
    backend = Backend([evidence(), evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute, once=True)
    pc.run(directory, actor=backend.actor, execute=backend.execute, once=True)

    job = job_state(directory)
    refs = list(job["results"])
    refs[0] = dict(refs[0], path="jobs/alpha/../beta/a0001.json")
    provider_jobs.update(
        directory,
        "alpha",
        {"results": refs},
        expected_revision=job["revision"],
    )

    follow = Backend([evidence()], [])
    pc.run(directory, actor=follow.actor, execute=follow.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "escapes the job root" in job["hold_reason"]


def test_existing_stage_is_never_overwritten(tmp_path):
    directory = init_campaign(tmp_path)
    stage = directory / "jobs" / "alpha" / "rounds" / "1" / "a0001-owner"
    stage.mkdir(parents=True)
    (stage / "keep.txt").write_text("keep", encoding="utf-8")

    backend = Backend([evidence()], [])
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "stage already exists" in job["hold_reason"]
    assert backend.actor_calls == []
    assert (stage / "keep.txt").read_text(encoding="utf-8") == "keep"


def test_executor_output_path_names_a_directory(tmp_path):
    backend = Backend([evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    call = backend.execute_calls[0]
    assert call["output_path"].endswith("a0001-output")
    assert not call["output_path"].endswith(".json")


def test_author_model_survives_review_queries(tmp_path):
    backend = Backend([propose()], [evidence(), pass_review])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "ready"
    assert job["author_model"] == MODELS["deepseek"]
    assert job["review_model"] == MODELS["minimax"]


def test_c_revise_preserves_attempt_counts(tmp_path):
    backend = Backend(
        [propose(("a0001",)), propose(("a0003",))],
        [revise_review("fix the C candidate"), pass_review],
        mode="c",
    )
    directory = init_campaign(
        tmp_path,
        make_manifest(
            jobs=[
                {
                    "id": "alpha",
                    "config": {
                        "function": "0x402000",
                        "mode": "c",
                        "goal": "recover alpha",
                    },
                }
            ]
        ),
    )
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    assert job["state"] == "accepted"
    assert job["round"] == 2
    assert job["attempts"] == 4
    assert job["reviews"] == 2
    assert "fix the C candidate" in job["feedback"]


def test_action_record_hash_is_the_canonical_hash_without_the_hash_field(tmp_path):
    backend = Backend([evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    path = directory / "jobs" / "alpha" / "a0001.json"
    record = json.loads(path.read_text(encoding="utf-8"))
    payload = {key: value for key, value in record.items() if key != "hash"}
    digest = hashlib.sha256(
        json.dumps(
            payload,
            allow_nan=False,
            sort_keys=True,
            separators=(",", ":"),
            ensure_ascii=False,
        ).encode("utf-8")
    ).hexdigest()
    assert record["hash"] == digest
    raw_digest = hashlib.sha256(path.read_bytes()).hexdigest()
    assert record["hash"] != raw_digest
    ref = job_state(directory)["results"][0]
    assert ref["hash"] == record["hash"]


def test_out_of_range_values_are_forwarded_to_the_broker(tmp_path):
    backend = Backend([evidence(limit=500), memory(size=4096)], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute)
    job = job_state(directory)
    refs = job["results"]
    assert [ref["id"] for ref in refs][:2] == ["a0001", "a0002"]
    assert all(ref["ok"] for ref in refs[:2])
    assert len(backend.execute_calls) == 2
    assert backend.execute_calls[0]["action"]["limit"] == 500
    assert backend.execute_calls[1]["action"]["size"] == 4096


def test_non_object_executor_response_is_held(tmp_path):
    directory = init_campaign(tmp_path)

    def execute(job, action, output_path, prior=None):
        return ["not", "an", "object"]

    backend = Backend([evidence()], [])
    pc.run(directory, actor=backend.actor, execute=execute)
    job = job_state(directory)
    assert job["state"] == "held"
    assert "execute response must be an object" in job["hold_reason"]
    assert job["escalation"]["adapter"] == "execute"


def test_status_counts_retries_separately(tmp_path):
    backend = Backend([evidence()], [])
    directory = init_campaign(tmp_path)
    pc.run(directory, actor=backend.actor, execute=backend.execute, once=True)
    summary = pc.status(directory)
    assert summary["jobs"][0]["retries"] == 0
    assert summary["jobs"][0]["attempts"] == 1
