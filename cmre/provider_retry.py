"""Safe retry preparation for one held provider campaign job.

This module is post-experiment: it prepares a fresh attempt for a job that was
held after a *clean terminal actor failure*.  It never runs inference, never
resets a budget and never touches the campaign clock: the original deadline
and the persisted run metadata stay byte identical.

Every refusal is explicit.  A retry is prepared only when all of the
following are proven from the durable evidence on disk:

* the store identity equals the current controller identity;
* ``run.json`` exists, validates against the store and is not expired;
* the job is ``held`` and carries no active checkpoint;
* the current failure is retryable, belongs to the owner or the review phase,
  is the latest committed action and agrees with the action count;
* the hold was raised by the actor adapter and not by the controller, the
  executor, a finalize step, an interruption or a deliberate review hold;
* the retained stage still lives under that job and its terminal pair and
  stage receipts are present, ended and clean: the requested model is the
  expected role model, no other model was observed and neither scope nor pin
  drift was recorded.

The prepared job keeps its action references, counters, attempts, failure
history, bundle and model pins.  An owner retry returns the job to
``prepared``/``owner``; a review retry returns it to ``needs-review``/``review``
with the existing bundle in a single store event.
"""

from __future__ import annotations

import hashlib
import json
import math
import time
from pathlib import Path

from . import provider_campaign, provider_jobs
from .coordination import lock

SCHEMA = "cmre-provider-retry/1"
OWNER_PHASE = "owner"
REVIEW_PHASE = "review"
TERMINAL_RECEIPT_STATES = frozenset(
    {"completed", "timeout", "step_limit", "incomplete"}
)
PAIR_SCHEMA = "cmre-provider-pair/1"
WORKER_SCHEMA = "cmre-provider-worker/1"

__all__ = ["RetryError", "RetryRefused", "SCHEMA", "retry_held"]


class RetryError(ValueError):
    """Base error for a refused or failed retry preparation."""


class RetryRefused(RetryError):
    """The held job cannot be retried safely."""


def _canonical(value):
    return json.dumps(
        value,
        allow_nan=False,
        sort_keys=True,
        separators=(",", ":"),
        ensure_ascii=False,
    )


def _sha256_json(value):
    return hashlib.sha256(_canonical(value).encode("utf-8")).hexdigest()


def _record_hash(record):
    """Canonical action record hash over the record without its ``hash``."""
    return _sha256_json({key: value for key, value in record.items() if key != "hash"})


def _read_json(path):
    """Strictly read one JSON file; ``None`` marks a missing file."""
    path = Path(path)
    try:
        text = path.read_text(encoding="utf-8")
    except FileNotFoundError:
        return None
    except OSError as exc:
        raise RetryRefused(f"cannot read {path}: {exc}") from exc
    try:
        return provider_campaign._loads(text)
    except (TypeError, ValueError) as exc:
        raise RetryRefused(f"invalid JSON in {path}: {exc}") from exc


def _is_number(value):
    return (
        isinstance(value, (int, float))
        and not isinstance(value, bool)
        and math.isfinite(value)
    )


def _under(root, path):
    return root == path or root in path.parents


def _run_timing(directory, state):
    """Validated run metadata of the original campaign run."""
    metadata = _read_json(directory / "run.json")
    if metadata is None:
        raise RetryRefused("run metadata is missing; the campaign never started")
    try:
        return provider_campaign._validate_run(metadata, state)
    except provider_campaign.CampaignError as exc:
        raise RetryRefused(f"run metadata is not valid: {exc}") from exc


def _expected_role(job, phase):
    config = job.get("config")
    if not isinstance(config, dict):
        raise RetryRefused("the job retains no configuration")
    role = config.get("owner") if phase == OWNER_PHASE else config.get("reviewer")
    if not isinstance(role, str) or not role.strip():
        raise RetryRefused("the job configuration names no role for the phase")
    return role


def _check_hold(job):
    """The job must be a plain hold raised by the actor adapter."""
    if job.get("state") != "held":
        raise RetryRefused(f"job is {job.get('state')!r}, not held")
    if job.get("checkpoint") is not None:
        raise RetryRefused("the job retains an active checkpoint")
    escalation = job.get("escalation")
    if not isinstance(escalation, dict):
        raise RetryRefused("the hold carries no escalation record")
    if escalation.get("checkpoint") is not None:
        raise RetryRefused("refusing to retry an interrupted hold")
    adapter = escalation.get("adapter")
    if adapter != "actor":
        raise RetryRefused(f"refusing to retry a {adapter!r} hold")


def _check_failure(job):
    """The current failure must be the latest committed failed action."""
    failure = job.get("last_failure")
    if not isinstance(failure, dict):
        raise RetryRefused("the job has no current failure to retry")
    if failure.get("retryable") is not True:
        raise RetryRefused("the current failure is not retryable")
    phase = failure.get("phase")
    if phase not in (OWNER_PHASE, REVIEW_PHASE):
        raise RetryRefused(f"unsupported retry phase: {phase!r}")
    refs = job.get("results")
    if not isinstance(refs, list) or not refs:
        raise RetryRefused("the job retains no committed action references")
    last = refs[-1]
    if not isinstance(last, dict) or last.get("ok") is not False:
        raise RetryRefused("the latest committed action did not fail")
    action_id = failure.get("action")
    if not isinstance(action_id, str) or not action_id:
        raise RetryRefused("the current failure names no action")
    if last.get("id") != action_id:
        raise RetryRefused("the current failure is not the latest committed action")
    count = job.get("action_count")
    if not isinstance(count, int) or isinstance(count, bool) or count < 1:
        raise RetryRefused("the job retains no action count")
    if action_id != f"a{count:04d}":
        raise RetryRefused("the failed action id and the action count disagree")
    role = _expected_role(job, phase)
    if failure.get("role") not in (None, role):
        raise RetryRefused("the failed action was not produced by the expected role")
    return failure


def _check_record(directory, job, failure):
    """The retained record of the failed action must verify and be terminal."""
    job_id = job["id"]
    ref = (job.get("results") or [])[-1]
    relative = ref.get("path")
    if not isinstance(relative, str) or not relative.strip():
        raise RetryRefused("the failed action reference has no path")
    if not relative.startswith(f"jobs/{job_id}/"):
        raise RetryRefused("the failed action reference escapes the job root")
    root = (directory / "jobs" / job_id).resolve()
    path = (directory / relative).resolve()
    if not _under(root, path):
        raise RetryRefused("the failed action reference escapes the job root")
    record = _read_json(path)
    if not isinstance(record, dict):
        raise RetryRefused("the failed action record is missing or invalid")
    if record.get("hash") != _record_hash(record):
        raise RetryRefused("the failed action record hash mismatch")
    if record.get("hash") != ref.get("hash"):
        raise RetryRefused("the failed action reference hash mismatch")
    if record.get("ok") is not False:
        raise RetryRefused("the retained action record is not a failure")
    if record.get("kind") is not None:
        raise RetryRefused(
            "the failure produced an action; it is not a terminal actor failure"
        )
    if record.get("phase") != failure.get("phase"):
        raise RetryRefused("the retained record phase differs from the failure phase")
    error = record.get("error")
    if not isinstance(error, str) or not error.strip():
        raise RetryRefused("the retained record carries no failure reason")
    return record


def _check_pair(pair):
    """The pair receipt must describe a terminal attempt."""
    if not _is_number(pair.get("ended")):
        raise RetryRefused("the pair receipt has no ended time")
    state = pair.get("state")
    if not isinstance(state, str) or state not in TERMINAL_RECEIPT_STATES:
        raise RetryRefused("the pair receipt is not terminal")


def _check_stage_receipt(payload, expected_model):
    """One stage receipt must be terminal, on-model and drift free."""
    if not _is_number(payload.get("ended")):
        raise RetryRefused("a stage receipt has no ended time")
    state = payload.get("state")
    if not isinstance(state, str) or state not in TERMINAL_RECEIPT_STATES:
        raise RetryRefused("a stage receipt is not terminal")
    requested = payload.get("model_requested")
    if requested != expected_model:
        raise RetryRefused(f"the stage requested a foreign model: {requested!r}")
    observed = payload.get("models_observed")
    if observed is None and state == "timeout":
        observed = []
    if not isinstance(observed, list):
        raise RetryRefused("the stage receipt has no observed model record")
    foreign = [model for model in observed if model != expected_model]
    if foreign:
        raise RetryRefused(f"the stage observed a foreign model: {foreign[0]!r}")
    if not observed and state != "timeout":
        raise RetryRefused("the stage receipt observed no model")
    for field in ("out_of_scope", "pin_drift"):
        drift = payload.get(field)
        if not isinstance(drift, list):
            raise RetryRefused(f"the stage receipt has no {field} record")
        if drift:
            raise RetryRefused(f"the stage recorded {field}: {drift[0]!r}")


def _find_stage_receipt(stage_path, job_root, entry, workers):
    """Locate one stage receipt of the pair receipt."""
    if not isinstance(entry, dict):
        raise RetryRefused("the pair receipt has an invalid stage entry")
    claimed = entry.get("receipt")
    if isinstance(claimed, str) and claimed.strip():
        path = Path(claimed)
        if not path.is_absolute():
            path = stage_path / path
        path = path.resolve()
        if not (_under(stage_path, path) or _under(job_root, path)):
            raise RetryRefused("a stage receipt escapes the retained stage")
        payload = _read_json(path)
        if not isinstance(payload, dict):
            raise RetryRefused("a stage receipt is missing or invalid")
        return payload
    name = entry.get("stage")
    if isinstance(name, str) and name.strip():
        found = [
            payload for path, payload in workers if path.parent.name == name
        ]
        if len(found) == 1:
            return found[0]
        if len(found) > 1:
            raise RetryRefused("a stage receipt is ambiguous")
    raise RetryRefused("a stage receipt is missing")


def _collect_receipts(stage_path, job_root, record):
    """Return the terminal pair receipt and its stage receipts."""
    pairs = []
    workers = []
    for path in sorted(stage_path.rglob("*receipt*.json")):
        if not path.is_file():
            continue
        payload = _read_json(path)
        if not isinstance(payload, dict):
            raise RetryRefused(
                f"a receipt under the retained stage is not an object: {path.name}"
            )
        schema = payload.get("schema")
        if schema == PAIR_SCHEMA or isinstance(payload.get("stages"), list):
            pairs.append(payload)
        elif schema == WORKER_SCHEMA:
            workers.append((path, payload))
    if not pairs:
        embedded = record.get("receipt")
        if isinstance(embedded, dict) and (
            embedded.get("schema") == PAIR_SCHEMA
            or isinstance(embedded.get("stages"), list)
        ):
            pairs.append(embedded)
    if not pairs:
        raise RetryRefused("the retained stage has no terminal pair receipt")
    if len(pairs) > 1:
        raise RetryRefused("the retained stage has more than one pair receipt")
    pair = pairs[0]
    entries = pair.get("stages")
    if not isinstance(entries, list) or not entries:
        raise RetryRefused("the pair receipt lists no stages")
    stages = [
        _find_stage_receipt(stage_path, job_root, entry, workers) for entry in entries
    ]
    return pair, stages


def _check_stage(directory, job, record, failure, manifest):
    """The retained stage must be inside the job and provably clean."""
    job_id = job["id"]
    stage = failure.get("stage")
    if not isinstance(stage, str) or not stage.strip():
        raise RetryRefused("the current failure retains no stage path")
    if not stage.startswith(f"jobs/{job_id}/"):
        raise RetryRefused("the retained stage path escapes the job root")
    root = (directory / "jobs" / job_id).resolve()
    stage_path = (directory / stage).resolve()
    if not _under(root, stage_path):
        raise RetryRefused("the retained stage path escapes the job root")
    if not stage_path.is_dir():
        raise RetryRefused("the retained stage directory is missing")
    action_id = failure.get("action")
    if not stage_path.name.startswith(str(action_id)):
        raise RetryRefused("the retained stage path does not belong to the failed action")
    role = _expected_role(job, failure["phase"])
    roles = manifest.get("roles")
    if not isinstance(roles, dict) or not isinstance(roles.get(role), dict):
        raise RetryRefused("the manifest names no model for the role")
    expected_model = roles[role].get("model")
    if not isinstance(expected_model, str) or not expected_model.strip():
        raise RetryRefused("the manifest names no model for the role")
    pair, stages = _collect_receipts(stage_path, root, record)
    _check_pair(pair)
    for payload in stages:
        _check_stage_receipt(payload, expected_model)


def _changes(job, failure, reason):
    """The single store event that prepares the retry."""
    phase = failure["phase"]
    if job.get("phase") != phase:
        raise RetryRefused("the job phase differs from the failed action phase")
    if phase == OWNER_PHASE:
        changes = {"state": "prepared", "phase": OWNER_PHASE}
    else:
        bundle = job.get("bundle")
        if not isinstance(bundle, dict) or not bundle:
            raise RetryRefused("the review retry requires the retained bundle")
        changes = {"state": "needs-review", "phase": REVIEW_PHASE}
    changes["retry_reason"] = reason
    changes["hold_reason"] = None
    changes["escalation"] = None
    return changes


def _summary(directory, updated, failure, timing, reason):
    return {
        "schema": SCHEMA,
        "directory": str(directory),
        "job": updated.get("id"),
        "state": updated.get("state"),
        "phase": updated.get("phase"),
        "retry_reason": reason,
        "failure": {
            "action": failure.get("action"),
            "phase": failure.get("phase"),
            "role": failure.get("role"),
            "stage": failure.get("stage"),
            "retryable": failure.get("retryable"),
        },
        "action_count": updated.get("action_count", 0),
        "attempts": updated.get("attempts", 0),
        "round": updated.get("round", 1),
        "results": len(updated.get("results") or []),
        "started_at": timing["started_at"],
        "deadline": timing["deadline"],
        "revision": updated.get("revision"),
    }


def _retry_locked(directory, job_id, reason, current_identity, clock):
    state = provider_jobs.load(directory)
    if state.get("identity") != current_identity:
        raise RetryRefused("controller identity changed before provider retry")
    job = state["jobs"].get(job_id)
    if job is None:
        raise RetryRefused(f"unknown job: {job_id}")
    timing = _run_timing(directory, state)
    if float(clock()) >= float(timing["deadline"]):
        raise RetryRefused("the original campaign deadline has expired")
    _check_hold(job)
    failure = _check_failure(job)
    record = _check_record(directory, job, failure)
    _check_stage(directory, job, record, failure, state["manifest"])
    changes = _changes(job, failure, reason)
    try:
        updated = provider_jobs.update(
            directory, job_id, changes, expected_revision=job["revision"]
        )
    except provider_jobs.StaleRevision as exc:
        raise RetryRefused(f"the job changed while the retry was prepared: {exc}") from exc
    return _summary(directory, updated, failure, timing, reason)


def retry_held(directory, job_id, reason, current_identity, *, clock=time.time):
    """Prepare one safe retry of a held job.  No inference is performed.

    The same nonblocking ``run.lock`` the controller uses is acquired first, so
    an active controller can never race the preparation.  On success the job
    is ready for a fresh attempt under the unchanged original deadline.
    """
    directory = Path(directory)
    if not isinstance(job_id, str) or not job_id.strip():
        raise RetryRefused("job id must be a nonempty string")
    if not isinstance(reason, str) or not reason.strip():
        raise RetryRefused("retry reason must be a nonempty string")
    if not isinstance(current_identity, dict):
        raise RetryRefused("current identity must be an object")
    job_id = job_id.strip()
    reason = reason.strip()
    with lock(directory / "run.lock"):
        return _retry_locked(directory, job_id, reason, current_identity, clock)
