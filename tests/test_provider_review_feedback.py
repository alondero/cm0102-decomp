"""Asset-free regressions for partial reviewer verdicts and required-PC hits.

The reviewer protocol lets a revise or hold review report a subset of the
bundle's required checks.  The durable core must accept that subset, keep the
reviewer's own reason and the normal revision or hold state, and must still
refuse to finalize a pass review that misses a check, names an unknown check
label or retains unknowns.

The second regression covers the required-PC projection: the broker reports
native addresses while a configuration may pin the padded form of the same
address, so the comparison is numeric and never a string match.
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
REQUIRED_CHECKS = ["pc", "coverage"]
NATIVE_PC = "0x4d16ea"
PADDED_PC = "0x004d16ea"


def clone(value):
    return json.loads(json.dumps(value))


def make_manifest(*, required_pcs=None, max_rounds=30):
    config = {
        "function": "0x401000",
        "mode": "input",
        "goal": "implement alpha",
        "max_rounds": max_rounds,
    }
    if required_pcs is not None:
        config["required_pcs"] = list(required_pcs)
    return {
        "schema": pc.SCHEMA,
        "duration_seconds": 60,
        "concurrency": 1,
        "roles": {role: {"model": MODELS[role]} for role in ("deepseek", "minimax")},
        "jobs": [{"id": "alpha", "config": config}],
    }


def propose(artifacts=("a0001",), summary="candidate ready"):
    return {"kind": "propose", "artifacts": list(artifacts), "summary": summary}


def probe():
    return {"kind": "probe", "request": {"entry": "0x401010"}}


def review(verdict, checks, *, unknowns=(), reason=None, sha=None):
    action = {
        "kind": "review",
        "verdict": verdict,
        "bundle_sha256": sha,
        "checks": list(checks),
        "unknowns": list(unknowns),
    }
    if reason is not None:
        action["reason"] = reason
    return action


def pass_review(context, job):
    return review(
        "pass",
        context["required_checks"],
        sha=context["bundle"]["sha256"],
    )


class Harness:
    """A scripted actor and a minimal broker for one campaign."""

    def __init__(
        self,
        owner_actions,
        review_actions,
        *,
        covered_pcs=None,
        required_checks=None,
    ):
        self.owner_actions = list(owner_actions)
        self.review_actions = list(review_actions)
        self.covered_pcs = list(covered_pcs or [])
        self.required_checks = list(required_checks or REQUIRED_CHECKS)
        self.actor_calls = []
        self.execute_calls = []

    def actor(self, job, role, context, stage_path):
        self.actor_calls.append(
            {"role": role, "phase": context["phase"], "context": clone(context)}
        )
        script = self.owner_actions if context["phase"] == "owner" else self.review_actions
        if not script:
            raise RuntimeError("the script is exhausted")
        action = script.pop(0)
        if callable(action):
            action = action(context, job)
        return {
            "action": clone(action),
            "model": MODELS[role],
            "receipt": {"role": role},
        }

    def execute(self, job, action, output_path, prior=None):
        self.execute_calls.append(clone(action))
        kind = action.get("kind")
        if kind == "bundle":
            payload = {
                "artifacts": list(action["artifacts"]),
                "summary": action["summary"],
            }
            sha = hashlib.sha256(
                json.dumps(payload, sort_keys=True).encode("utf-8")
            ).hexdigest()
            return {
                "ready": True,
                "sha256": sha,
                "required_checks": list(self.required_checks),
                "payload": payload,
            }
        if kind == "finalize":
            return {"state": "ready", "bundle_sha256": action["bundle"]["sha256"]}
        if kind == "probe":
            return {
                "kind": "probe",
                "ok": True,
                "result": {
                    "summary": {
                        "outcome": "return",
                        "covered_pcs": list(self.covered_pcs),
                        "coverage": 1.0,
                    }
                },
            }
        return {"kind": kind, "ok": True, "result": {}}


def run_campaign(tmp_path, harness, manifest=None, name="camp"):
    directory = tmp_path / name
    pc.initialize(
        directory,
        manifest if manifest is not None else make_manifest(),
        {"provider": "deepseek"},
    )
    pc.run(
        directory,
        actor=harness.actor,
        execute=harness.execute,
        identity={"provider": "deepseek"},
    )
    return directory


def job_state(directory, job_id="alpha"):
    return provider_jobs.load(directory)["jobs"][job_id]


def finalize_calls(harness):
    return [call for call in harness.execute_calls if call.get("kind") == "finalize"]


def owner_turns(harness):
    return [call for call in harness.actor_calls if call["phase"] == "owner"]


def record(directory, action_id):
    path = Path(directory) / "jobs" / "alpha" / f"{action_id}.json"
    return json.loads(path.read_text(encoding="utf-8"))


def test_revise_with_a_subset_of_checks_reopens_the_owner_with_the_feedback(tmp_path):
    def revise(context, job):
        return review(
            "revise",
            ["pc"],
            unknowns=["coverage witness"],
            reason="the coverage witness is still missing",
            sha=context["bundle"]["sha256"],
        )

    harness = Harness(
        [propose(("a0001",)), propose(("a0003",))],
        [revise, pass_review],
    )
    directory = run_campaign(tmp_path, harness)

    job = job_state(directory)
    assert job["state"] == "ready"
    assert job["round"] == 2
    assert job["reviews"] == 2
    assert "the coverage witness is still missing" in job["feedback"]

    turns = owner_turns(harness)
    assert len(turns) == 2
    assert turns[1]["context"]["round"] == 2
    assert turns[1]["context"]["feedback"] == [
        "the coverage witness is still missing"
    ]

    revise_record = record(directory, "a0002")
    assert revise_record["ok"] is True
    assert revise_record["action"]["verdict"] == "revise"
    assert revise_record["action"]["checks"] == ["pc"]
    assert revise_record["action"]["reason"] == "the coverage witness is still missing"
    assert finalize_calls(harness)


def test_hold_with_a_subset_of_checks_keeps_the_review_reason(tmp_path):
    def hold_review(context, job):
        return review(
            "hold",
            [],
            unknowns=["pc witness"],
            reason="the pc witness must be rebuilt",
            sha=context["bundle"]["sha256"],
        )

    harness = Harness([propose(("a0001",))], [hold_review])
    directory = run_campaign(tmp_path, harness)

    job = job_state(directory)
    assert job["state"] == "held"
    assert job["hold_reason"] == "the pc witness must be rebuilt"
    assert job["escalation"]["adapter"] == "actor"
    assert job["escalation"]["reason"] == "the pc witness must be rebuilt"
    assert job["reviews"] == 1
    assert job["review"]["verdict"] == "hold"
    assert job["review"]["checks"] == []
    assert not finalize_calls(harness)


def test_pass_with_a_missing_check_cannot_finalize(tmp_path):
    def partial_pass(context, job):
        return review("pass", ["pc"], sha=context["bundle"]["sha256"])

    harness = Harness([propose(("a0001",))], [partial_pass])
    directory = run_campaign(tmp_path, harness)

    job = job_state(directory)
    assert job["state"] == "held"
    assert "missing required checks" in job["hold_reason"]
    assert "coverage" in job["hold_reason"]
    assert not finalize_calls(harness)


def test_pass_with_a_failed_check_cannot_finalize(tmp_path):
    def failed_pass(context, job):
        return review(
            "pass",
            ["pc", "coverage", "pc-witness"],
            sha=context["bundle"]["sha256"],
        )

    harness = Harness([propose(("a0001",))], [failed_pass])
    directory = run_campaign(tmp_path, harness)

    job = job_state(directory)
    assert job["state"] == "held"
    assert "unknown review checks" in job["hold_reason"]
    assert "pc-witness" in job["hold_reason"]
    assert not finalize_calls(harness)


def test_pass_with_nonempty_unknowns_cannot_finalize(tmp_path):
    def unknown_pass(context, job):
        return review(
            "pass",
            context["required_checks"],
            unknowns=["unresolved path"],
            sha=context["bundle"]["sha256"],
        )

    harness = Harness([propose(("a0001",))], [unknown_pass])
    directory = run_campaign(tmp_path, harness)

    job = job_state(directory)
    assert job["state"] == "held"
    assert "unknowns" in job["hold_reason"]
    assert not finalize_calls(harness)


@pytest.mark.parametrize("verdict", ["revise", "hold"])
def test_unknown_check_labels_are_rejected_for_every_verdict(tmp_path, verdict):
    def labelled(context, job):
        return review(
            verdict,
            ["pc", "pc-witness"],
            reason="a concrete reason",
            sha=context["bundle"]["sha256"],
        )

    harness = Harness([propose(("a0001",))], [labelled])
    directory = run_campaign(tmp_path, harness, name=f"camp-{verdict}")

    job = job_state(directory)
    assert job["state"] == "held"
    assert "unknown review checks" in job["hold_reason"]
    assert "pc-witness" in job["hold_reason"]
    assert not finalize_calls(harness)


def test_padded_required_pc_hits_the_broker_native_covered_pc(tmp_path):
    harness = Harness(
        [probe(), propose(("a0002",))],
        [pass_review],
        covered_pcs=[NATIVE_PC],
    )
    directory = run_campaign(
        tmp_path,
        harness,
        manifest=make_manifest(required_pcs=[PADDED_PC]),
    )

    job = job_state(directory)
    assert job["state"] == "ready"
    context = harness.actor_calls[-1]["context"]
    projections = [item for item in context["results"] if item["kind"] == "probe"]
    assert projections
    pcs = projections[0]["summary"]["pcs"]
    assert pcs["own"] == [NATIVE_PC]
    assert pcs["required_hits"] == [PADDED_PC]


def test_an_integer_covered_pc_hits_the_padded_required_pc(tmp_path):
    harness = Harness(
        [probe(), propose(("a0002",))],
        [pass_review],
        covered_pcs=[int(NATIVE_PC, 16)],
    )
    directory = run_campaign(
        tmp_path,
        harness,
        manifest=make_manifest(required_pcs=[PADDED_PC]),
    )

    job = job_state(directory)
    assert job["state"] == "ready"
    context = harness.actor_calls[-1]["context"]
    projections = [item for item in context["results"] if item["kind"] == "probe"]
    assert projections[0]["summary"]["pcs"]["required_hits"] == [PADDED_PC]


def test_malformed_required_pcs_never_match(tmp_path):
    harness = Harness(
        [probe(), propose(("a0002",))],
        [pass_review],
        covered_pcs=[NATIVE_PC, "0xzz"],
    )
    directory = run_campaign(
        tmp_path,
        harness,
        manifest=make_manifest(
            required_pcs=[PADDED_PC, "not-a-pc", "0xzz", "0x4d16eb"],
        ),
    )

    job = job_state(directory)
    assert job["state"] == "ready"
    context = harness.actor_calls[-1]["context"]
    projections = [item for item in context["results"] if item["kind"] == "probe"]
    assert projections[0]["summary"]["pcs"]["required_hits"] == [PADDED_PC]
