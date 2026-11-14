"""Durable provider-owned campaign orchestration.

The public surface is intentionally small:

``initialize(directory, manifest, identity)``
    Validate a campaign manifest and create the supplied provider job store.
``run(directory, *, actor, execute, identity=None, once=False, clock, sleep)``
    Drive prepared/review jobs through an injected actor/executor pair.
``status(directory)``
    Return a compact, read-only campaign summary.

The module owns orchestration, durable checkpoints and validation only.  It
does not spawn providers, shells, game harnesses, or acceptance logic.

Protocol shapes implemented here (production broker protocol):

* An actor action is ``{"kind": ..., ...}`` carrying only the documented keys
  for that kind.  Unknown keys are rejected; documented optional keys are
  accepted.
* An executor response is
  ``{"kind": ..., "ok": ..., "result": {...}, "artifacts": {...},
  "artifact_ids": [...]}``.  ``result`` holds the per-kind payload and
  ``result["summary"]`` holds the probe summary.
* Artifact identifiers are action ids (``a0001``) or file ids
  (``a0001/artifacts/raw.json.gz``).  There is no ``FILE:`` prefix.

An action record is hashed with the canonical JSON digest (sorted keys, no
whitespace, UTF-8) of the record with its own ``hash`` field removed.  That
digest is stored under ``hash`` in the record and in the durable reference; it
is not the SHA-256 of the record file.
"""

from __future__ import annotations

import hashlib
import json
import math
import re
import time
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, wait
from pathlib import Path

from . import provider_jobs
from .coordination import BusyError, atomic_json, lock

SCHEMA = "cmre-provider-campaign/1"
RUN_SCHEMA = "cmre-provider-campaign-run/1"
JOB_ID_RE = re.compile(r"^[a-z0-9][a-z0-9_-]{0,63}$")
HEX_VA_RE = re.compile(r"^0[xX][0-9a-fA-F]+$")
PC_RE = re.compile(r"^0[xX][0-9a-fA-F]{1,16}$")
ACTION_ID_RE = re.compile(r"^a\d{4,}$")
FILE_ID_RE = re.compile(r"^a\d{4,}/[A-Za-z0-9._\-/]+$")
ROLES = ("deepseek", "minimax")
MODES = ("input", "c")
INSPECT_SECTIONS = (
    "instructions",
    "events",
    "arrivals",
    "returns",
    "registered_helper_states",
    "disassembly",
)
VERDICTS = ("pass", "revise", "hold")

# Production action schemas: kind -> (required keys, optional keys).
ACTION_SCHEMAS = {
    "evidence": ((), ("function", "start", "limit")),
    "memory": (("address", "size"), ()),
    "probe": (("request",), ("base",)),
    "inspect": (("artifact", "section"), ("start", "limit")),
    "build": ((), ()),
    "screen": ((), ()),
    "test": ((), ()),
    "controls": ((), ()),
    "propose": (("artifacts", "summary"), ("resolution",)),
    "control-plan": (("controls",), ()),
    "hold": (("reason",), ()),
    "review": (("verdict", "bundle_sha256", "checks", "unknowns"), ("reason",)),
}
ACTION_VALUE_TYPES = {
    "function": str,
    "start": int,
    "limit": int,
    "address": int,
    "size": int,
    "request": dict,
    "base": str,
    "artifact": str,
    "section": str,
    "artifacts": list,
    "summary": str,
    "reason": str,
    "verdict": str,
    "bundle_sha256": str,
    "checks": list,
    "unknowns": list,
    "resolution": dict,
    "controls": list,
}
OWNER_ACTIONS = frozenset(ACTION_SCHEMAS) - {"review"}
REVIEW_ACTIONS = frozenset(
    {"evidence", "memory", "probe", "inspect", "screen", "review", "hold"}
)
MUTATING_ACTIONS = frozenset({"build", "test", "controls", "control-plan"})

MAX_ACTIONS_PER_ROUND = 64
MAX_CONTEXT_RESULTS = 8
MAX_FEEDBACK = 8
MAX_STORED_FEEDBACK = 16
MAX_STORED_FAILURES = 8
MAX_SUMMARY = 200
MAX_ERROR = 300
MAX_INSTRUCTIONS = 80
MAX_PC_LIST = 120
MAX_IMPORTS = 40
MAX_ARTIFACTS = 32
MAX_REGS = 32
MAX_ITEMS = 20
MAX_DEPTH = 3
DEFAULT_PROVIDER_RETRIES = 2
UPDATE_ATTEMPTS = 30
UPDATE_DELAY = 0.02

__all__ = ["CampaignError", "InvalidManifest", "SCHEMA", "initialize", "run", "status"]


class CampaignError(ValueError):
    """Base error for campaign orchestration failures."""


class InvalidManifest(CampaignError):
    """The supplied campaign manifest is invalid."""


def _reject_constant(value):
    raise ValueError(f"non-finite JSON constant: {value}")


def _unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise ValueError(f"duplicate JSON key: {key}")
        result[key] = value
    return result


def _loads(text):
    return json.loads(
        text,
        object_pairs_hook=_unique_object,
        parse_constant=_reject_constant,
    )


def _canonical(value):
    return json.dumps(
        value,
        allow_nan=False,
        sort_keys=True,
        separators=(",", ":"),
        ensure_ascii=False,
    )


def _clone(value):
    try:
        return json.loads(_canonical(value))
    except (TypeError, ValueError) as exc:
        raise CampaignError(f"value is not canonical JSON: {exc}") from exc


def _sha256_json(value):
    return hashlib.sha256(_canonical(value).encode("utf-8")).hexdigest()


def _read_json(path):
    path = Path(path)
    try:
        text = path.read_text(encoding="utf-8")
    except FileNotFoundError:
        return None
    except OSError as exc:
        raise CampaignError(f"cannot read {path}: {exc}") from exc
    try:
        return _loads(text)
    except (TypeError, ValueError) as exc:
        raise CampaignError(f"invalid JSON in {path}: {exc}") from exc


def _write_json(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    atomic_json(path, _clone(value))


def _is_number(value):
    return (
        isinstance(value, (int, float))
        and not isinstance(value, bool)
        and math.isfinite(value)
    )


def _is_int(value):
    return isinstance(value, int) and not isinstance(value, bool)


def _require_int(value, name, low, high):
    if not _is_int(value):
        raise InvalidManifest(f"{name} must be an integer")
    if value < low or value > high:
        raise InvalidManifest(f"{name} must be between {low} and {high}")
    return value


def _compact(value, depth=0):
    """Bound a JSON value so a context projection stays small."""
    if depth > MAX_DEPTH:
        return None
    if value is None or isinstance(value, bool):
        return value
    if isinstance(value, int):
        return value
    if isinstance(value, float):
        return value if math.isfinite(value) else None
    if isinstance(value, str):
        return value[:MAX_SUMMARY]
    if isinstance(value, list):
        return [_compact(item, depth + 1) for item in value[:MAX_ITEMS]]
    if isinstance(value, dict):
        return {
            str(key): _compact(item, depth + 1)
            for key, item in list(value.items())[:MAX_ITEMS]
        }
    return str(value)[:MAX_SUMMARY]


def _normalize_config(job_id, config):
    function = config.get("function")
    if _is_int(function) and function >= 0:
        function = f"0x{function:x}"
    if not isinstance(function, str) or not HEX_VA_RE.fullmatch(function):
        raise InvalidManifest(f"jobs.{job_id}.config.function must be a hex VA")

    mode = config.get("mode")
    if mode not in MODES:
        raise InvalidManifest(f"jobs.{job_id}.config.mode must be input or c")

    goal = config.get("goal")
    if not isinstance(goal, str) or not goal.strip():
        raise InvalidManifest(f"jobs.{job_id}.config.goal must be a nonempty string")

    default_owner = "deepseek" if mode == "input" else "minimax"
    owner = config.get("owner", default_owner)
    if owner not in ROLES:
        raise InvalidManifest(f"jobs.{job_id}.config.owner must be a role")

    other = "minimax" if owner == "deepseek" else "deepseek"
    reviewer = config.get("reviewer", other)
    if reviewer not in ROLES:
        raise InvalidManifest(f"jobs.{job_id}.config.reviewer must be a role")
    if reviewer == owner:
        raise InvalidManifest(f"jobs.{job_id}.config owner and reviewer must differ")

    max_rounds = _require_int(
        config.get("max_rounds", 30), f"jobs.{job_id}.config.max_rounds", 1, 100
    )
    provider_retries = _require_int(
        config.get("provider_retries", DEFAULT_PROVIDER_RETRIES),
        f"jobs.{job_id}.config.provider_retries",
        0,
        5,
    )

    normalized = dict(config)
    for flag in ('defer_publication', 'defer_execution', 'reserve_task', 'self_service_controls',
                 'auto_test_after_build'):
        if flag in config and type(config[flag]) is not bool:
            raise InvalidManifest(f'jobs.{job_id}.config.{flag} must be boolean')
    normalized["function"] = function
    normalized["mode"] = mode
    normalized["goal"] = goal
    normalized["owner"] = owner
    normalized["reviewer"] = reviewer
    normalized["max_rounds"] = max_rounds
    normalized["provider_retries"] = provider_retries
    required_pcs = config.get("required_pcs")
    if required_pcs is not None:
        if not isinstance(required_pcs, list) or not all(
            isinstance(item, str) for item in required_pcs
        ):
            raise InvalidManifest(
                f"jobs.{job_id}.config.required_pcs must be a list of strings"
            )
        normalized["required_pcs"] = list(required_pcs)
    return _clone(normalized)


def _validate_manifest(manifest):
    if not isinstance(manifest, dict):
        raise InvalidManifest("manifest must be an object")
    if manifest.get("schema") != SCHEMA:
        raise InvalidManifest(f"manifest schema must be {SCHEMA}")

    duration = manifest.get("duration_seconds")
    if not _is_number(duration) or duration <= 0 or duration > 86400:
        raise InvalidManifest("duration_seconds must be positive finite <= 86400")
    duration = float(duration)

    concurrency = _require_int(manifest.get("concurrency", 2), "concurrency", 1, 16)
    capacities = manifest.get('role_capacity', {role: 1 for role in ROLES})
    if not isinstance(capacities, dict) or set(capacities) != set(ROLES):
        raise InvalidManifest('role_capacity must contain deepseek and minimax')
    for role, value in capacities.items():
        _require_int(value, f'role_capacity.{role}', 1, 8)
    _require_int(manifest.get('input_ready_limit', concurrency * 2), 'input_ready_limit', 1, 512)

    roles = manifest.get("roles")
    if not isinstance(roles, dict) or set(roles) != set(ROLES):
        raise InvalidManifest("roles must contain exactly deepseek and minimax")
    normalized_roles = {}
    for role in ROLES:
        entry = roles[role]
        if not isinstance(entry, dict):
            raise InvalidManifest(f"roles.{role} must be an object")
        model = entry.get("model")
        if not isinstance(model, str) or not model.strip():
            raise InvalidManifest(f"roles.{role}.model must be a nonempty string")
        normalized_roles[role] = {"model": model}
    if normalized_roles["deepseek"]["model"] == normalized_roles["minimax"]["model"]:
        raise InvalidManifest("role models must be distinct")

    jobs = manifest.get("jobs")
    if not isinstance(jobs, list) or not jobs:
        raise InvalidManifest("jobs must be a nonempty list")
    normalized_jobs = []
    seen = set()
    for item in jobs:
        if not isinstance(item, dict):
            raise InvalidManifest("each job must be an object")
        job_id = item.get("id")
        if not isinstance(job_id, str) or not JOB_ID_RE.fullmatch(job_id):
            raise InvalidManifest(f"invalid job id: {job_id!r}")
        if job_id in seen:
            raise InvalidManifest(f"duplicate job id: {job_id}")
        seen.add(job_id)
        config = item.get("config")
        if not isinstance(config, dict):
            raise InvalidManifest(f"jobs.{job_id}.config must be an object")
        normalized_jobs.append({"id": job_id, "config": _normalize_config(job_id, config)})

    normalized = dict(manifest)
    normalized["duration_seconds"] = duration
    normalized["concurrency"] = concurrency
    normalized['role_capacity'] = capacities
    normalized["roles"] = normalized_roles
    normalized["jobs"] = normalized_jobs
    return _clone(normalized)


def initialize(directory, manifest, identity):
    """Validate a manifest and create the provider job store."""
    normalized = _validate_manifest(manifest)
    if not isinstance(identity, dict):
        raise InvalidManifest("identity must be an object")
    _clone(identity)
    state = provider_jobs.create(Path(directory), normalized, identity)
    return {
        "schema": SCHEMA,
        "directory": str(Path(directory)),
        "jobs": list(state["jobs"]),
        "concurrency": normalized["concurrency"],
        "duration_seconds": normalized["duration_seconds"],
    }


def enqueue(directory, definition):
    """Append eligible work to this run without renewing its deadline."""
    state = provider_jobs.load(directory)
    job_id = definition.get('id')
    if not isinstance(job_id, str) or not JOB_ID_RE.fullmatch(job_id):
        raise InvalidManifest('Invalid replenishment job id')
    config = _normalize_config(job_id, definition.get('config', {}))
    for current in state['jobs'].values():
        if current['id'] == job_id:
            continue
        if int(current['config']['function'], 16) != int(config['function'], 16):
            continue
        if current['state'] == 'ready' and current.get('adoption_receipt') and config['mode'] == 'c':
            continue
        raise CampaignError('Function already has retained work; automatic retries are forbidden')
    return provider_jobs.enqueue(directory, {'id': job_id, 'config': config})


def _dispatch_phase(job):
    if job.get('state') == 'awaiting-execution':
        return 'execute'
    if job.get('state') == 'reviewed':
        return 'publish'
    if job.get("state") == "prepared":
        return "owner"
    if job.get("state") == "needs-review":
        return "review"
    return None


def _dispatch_role(job):
    """Provider role that would run the next stage of this job."""
    config = job.get("config") or {}
    if _dispatch_phase(job) in ('publish', 'execute'):
        return None
    if _dispatch_phase(job) == "review":
        return config.get("reviewer")
    return config.get("owner")


def _dispatch_batch(candidates, concurrency, *, role_capacity=None, owner_wip=None,
                    stage_capacity=None, provider_capacity=None, execution_capacity=None):
    """Drain review first and rotate a bounded owner set within isolated capacity."""
    if concurrency <= 0:
        return []
    capacities = role_capacity if role_capacity is not None else {role: 1 for role in ROLES}
    batch, used = [], {}
    owner_wip = owner_wip if owner_wip is not None else {
        role: max(1, count * 2) for role, count in capacities.items()}
    eligible = []
    for role in ROLES:
        owners = [j for j in candidates if _dispatch_phase(j) == 'owner'
                  and _dispatch_role(j) == role]
        if stage_capacity is None:
            groups = [owners]
        else:
            from .pipeline_policy import stage
            groups = [[j for j in owners if stage(j) == lane]
                      for lane in dict.fromkeys(stage(j) for j in owners)]
        for group in groups:
            # Appended jobs already have an enqueue history entry. Only an
            # actual actor attempt makes them started owner work.
            active = [j for j in group if j.get('attempts', 0) > 0]
            fresh = [j for j in group if not j.get('attempts', 0)]
            eligible.extend(active)
            eligible.extend(fresh[:max(0, owner_wip.get(role, 0) - len(active))])
    eligible.extend(j for j in candidates if _dispatch_phase(j) == 'review')
    eligible.extend(j for j in candidates if _dispatch_phase(j) == 'publish')
    eligible.extend(j for j in candidates if _dispatch_phase(j) == 'execute')
    positions = {job['id']: i for i, job in enumerate(candidates)}
    # Review drains first; least-served owners rotate instead of allowing the
    # first difficult job to consume every turn for its provider.
    ordered = sorted(eligible, key=lambda j: (
        0 if _dispatch_phase(j) == 'publish' else
        1 if _dispatch_phase(j) == 'review' else
        2 if _dispatch_phase(j) == 'execute' else 3, len(j.get('history', [])), positions[j['id']]))
    stages_used = {}
    for job in ordered:
        from .pipeline_policy import stage
        lane = stage(job)
        if stage_capacity is not None and stages_used.get(lane, 0) >= stage_capacity.get(lane, 0):
            continue
        role = _dispatch_role(job)
        if (role is None and execution_capacity is not None
                and used.get(None, 0) >= execution_capacity):
            continue
        if (role is not None and provider_capacity is not None
                and sum(n for r, n in used.items() if r is not None) >= provider_capacity):
            continue
        if role is not None and used.get(role, 0) >= capacities.get(role, 0):
            continue
        used[role] = used.get(role, 0) + 1
        stages_used[lane] = stages_used.get(lane, 0) + 1
        batch.append(job)
        if len(batch) >= concurrency:
            break
    return batch


def _job_view(job):
    return _clone(
        {
            "id": job["id"],
            "config": job["config"],
            "state": job.get("state"),
            "phase": job.get("phase"),
            "round": job.get("round", 1),
            "attempts": job.get("attempts", 0),
        }
    )


def _append_feedback(job, value):
    feedback = list(job.get("feedback") or [])
    if isinstance(value, list):
        feedback.extend(item for item in value if isinstance(item, str))
    elif isinstance(value, str):
        feedback.append(value)
    return feedback[-MAX_STORED_FEEDBACK:]


def _append_failure(job, failure):
    failures = list(job.get("failures") or [])
    failures.append(failure)
    return failures[-MAX_STORED_FAILURES:]


def _pc_value(value):
    """The numeric value of one PC entry, or None when it is malformed."""
    if _is_int(value):
        return value if value >= 0 else None
    if isinstance(value, str):
        text = value.strip()
        if PC_RE.fullmatch(text):
            return int(text, 16)
    return None


def _required_hits(covered, config):
    """The configured required PCs that the covered PCs hit.

    The broker reports native addresses (``0x4d16ea``) while a configuration
    may pin the padded form (``0x004d16ea``).  Both name the same address, so
    the comparison is numeric; the configured value and the configured order
    are kept.  A malformed value never matches anything.
    """
    required = config.get("required_pcs")
    if not isinstance(required, list):
        return []
    if not isinstance(covered, (list, tuple, set)):
        return []
    covered_values = set()
    for item in covered:
        value = _pc_value(item)
        if value is not None:
            covered_values.add(value)
    hits = []
    for item in required:
        value = _pc_value(item)
        if value is not None and value in covered_values:
            hits.append(item)
    return hits


def _guidance(record):
    error = record.get("error")
    base = "previous step failed"
    if isinstance(error, str) and error.strip():
        base = f"{base}: {error[:MAX_ERROR]}"
    return f"{base}; narrow the next step to one artifact or one check"[:MAX_SUMMARY]


def _prefixed(action_id, value):
    """Turn a broker relative path into the exact action prefixed file id."""
    if not isinstance(value, str) or not value.strip():
        return value
    if action_id and not value.startswith(f"{action_id}/"):
        return f"{action_id}/{value}"
    return value


def _artifact_refs(record, action, response):
    """Exact artifact identifiers: action ids and action prefixed file ids."""
    action_id = record.get("id")
    ids = []
    files = {}

    def add(value):
        if isinstance(value, str) and value.strip() and value not in ids:
            ids.append(value)

    listed = action.get("artifacts")
    if isinstance(listed, list):
        for item in listed:
            add(item)
    for key in ("base", "artifact"):
        value = action.get(key)
        if isinstance(value, str) and value.strip():
            add(value)
    artifact_ids = response.get("artifact_ids")
    if isinstance(artifact_ids, list):
        for item in artifact_ids:
            add(_prefixed(action_id, item))
    artifacts = response.get("artifacts")
    if isinstance(artifacts, dict):
        for key, entry in list(artifacts.items())[:MAX_ARTIFACTS]:
            file_id = _prefixed(action_id, key)
            add(file_id)
            if isinstance(entry, dict):
                files[file_id] = {
                    name: _compact(value) for name, value in entry.items()
                }
    return ids, files


def _project_summary(summary, config):
    """Bounded projection of the probe summary block."""
    out = {}
    outcome = summary.get("outcome")
    if isinstance(outcome, str) and outcome.strip():
        out["outcome"] = outcome[:MAX_SUMMARY]
    detail = summary.get("detail")
    if isinstance(detail, list):
        out["detail"] = _compact(detail)
    regs = summary.get("regs")
    if isinstance(regs, dict):
        out["regs"] = {
            str(key): _compact(regs[key]) for key in list(regs)[:MAX_REGS]
        }
    covered = summary.get("covered_pcs")
    if isinstance(covered, list):
        names = [str(item) for item in covered]
        out["pcs"] = {
            "own": names[:MAX_PC_LIST],
            "count": len(names),
            "required_hits": _required_hits(covered, config),
        }
    coverage = summary.get("coverage")
    if _is_number(coverage):
        out["coverage"] = float(coverage)
    imports = summary.get("import_count")
    if _is_int(imports):
        out["import_count"] = imports
    names = summary.get("imports")
    if isinstance(names, list):
        out["imports"] = [str(item)[:MAX_SUMMARY] for item in names[:MAX_IMPORTS]]
    frame = summary.get("return_fp")
    if isinstance(frame, list):
        out["return_fp"] = _compact(frame)
    for name in ("arrivals", "returns"):
        value = summary.get(name)
        if isinstance(value, list):
            out[f"{name}_count"] = len(value)
    identity = summary.get("execution_identity")
    if isinstance(identity, dict):
        out["execution_identity"] = _compact(identity)
    return out


def _project_result(result):
    """Bounded projection of the per kind result payload."""
    out = {}
    function = result.get("function")
    if isinstance(function, str) and function.strip():
        out["function"] = function[:MAX_SUMMARY]
    instructions = result.get("instructions")
    if isinstance(instructions, list):
        out["instructions"] = [
            _compact(item) for item in instructions[:MAX_INSTRUCTIONS]
        ]
    for name in ("body_bytes", "total", "remaining"):
        value = result.get(name)
        if _is_int(value):
            out[name] = value
    callees = result.get("callees")
    if isinstance(callees, list):
        out["callees"] = [str(item)[:MAX_SUMMARY] for item in callees[:MAX_IMPORTS]]
    section = result.get("section")
    if isinstance(section, str) and section.strip():
        out["section"] = section[:MAX_SUMMARY]
    items = result.get("items")
    if isinstance(items, list):
        out["items"] = [_compact(item) for item in items[:MAX_INSTRUCTIONS]]
    address = result.get("address")
    if _is_int(address):
        out["address"] = address
    size = result.get("size")
    if _is_int(size):
        out["size"] = size
    return out


def _project_bundle(response):
    """The bundle payload is the reviewer context and is preserved verbatim."""
    out = {}
    ready = response.get("ready")
    if isinstance(ready, bool):
        out["ready"] = ready
    sha = response.get("sha256")
    if isinstance(sha, str) and sha.strip():
        out["sha256"] = sha
    checks = response.get("required_checks")
    if isinstance(checks, list):
        out["required_checks"] = [str(item) for item in checks[:MAX_IMPORTS]]
    feedback = response.get("feedback")
    if isinstance(feedback, str) and feedback.strip():
        out["feedback"] = feedback[:MAX_SUMMARY]
    if response.get("payload") is not None:
        out["payload"] = response["payload"]
    return out


def _project_record(record, config):
    """Bounded structured projection of one action record."""
    response = record.get("response") if isinstance(record.get("response"), dict) else {}
    result = response.get("result") if isinstance(response.get("result"), dict) else {}
    action = record.get("action") if isinstance(record.get("action"), dict) else {}
    kind = record.get("kind")
    projection = {
        "id": record.get("id"),
        "phase": record.get("phase"),
        "kind": kind,
        "ok": bool(record.get("ok", True) and response.get('ok', True)),
    }
    if action:
        projection["action"] = _clone(action)
    for key in ('report_status', 'source_sha256', 'task_id', 'attempt', 'first_failure'):
        if key in response:
            projection[key] = _compact(response[key])
    if kind in ('controls', 'control-plan'):
        projection['control_status'] = result.get('status')
        projection['controls'] = {name: {key: _compact(value) for key, value in stage.items()
            if key in ('classification', 'reason', 'witnesses', 'detail')}
            for name, stage in list((result.get('stages') or {}).items())[:33]}
    response_kind = response.get("kind")
    if isinstance(response_kind, str) and response_kind:
        projection["response_kind"] = response_kind
    error = record.get("error") or response.get("error") or result.get("error")
    if isinstance(error, str) and error.strip():
        projection["error"] = error[:MAX_ERROR]
    if kind == "bundle":
        projection.update(_project_bundle(response))
    else:
        summary = result.get("summary")
        if isinstance(summary, dict):
            projection["summary"] = _project_summary(summary, config)
        elif isinstance(summary, str) and summary.strip():
            projection["summary"] = summary[:MAX_SUMMARY]
        projection.update(_project_result(result))
    artifacts, files = _artifact_refs(record, action, response)
    if artifacts:
        projection["artifacts"] = artifacts[:MAX_ARTIFACTS]
    if files:
        projection["artifact_files"] = files
    if not projection["ok"]:
        projection["guidance"] = _guidance(record)
    return projection


def _record_hash(record):
    """Canonical record hash over the record without its own ``hash`` field."""
    payload = {key: value for key, value in record.items() if key != "hash"}
    return _sha256_json(payload)


def _verified_records(directory, job):
    """Yield every retained action record after containment and hash checks."""
    job_id = job["id"]
    root = (directory / "jobs" / job_id).resolve()
    refs = [ref for ref in (job.get("results") or []) if isinstance(ref, dict)]
    for ref in refs:
        relative = ref.get("path")
        if not isinstance(relative, str) or not relative:
            raise CampaignError(f"action reference has no path: {ref.get('id')}")
        if not relative.startswith(f"jobs/{job_id}/"):
            raise CampaignError(f"action reference escapes the job root: {relative}")
        path = (directory / relative).resolve()
        if root not in path.parents:
            raise CampaignError(f"action reference escapes the job root: {relative}")
        record = _read_json(path)
        if not isinstance(record, dict):
            raise CampaignError(f"action record is missing or invalid: {relative}")
        digest = _record_hash(record)
        if digest != record.get("hash") or digest != ref.get("hash"):
            raise CampaignError(f"action record hash mismatch: {relative}")
        yield ref, record


def _context_results(directory, job):
    """Verify every retained action record and project the latest ones."""
    projections = [
        _project_record(record, job.get("config") or {})
        for _ref, record in _verified_records(directory, job)
    ]
    return projections[-MAX_CONTEXT_RESULTS:]


def _fixture_descriptor(response):
    """Locate the retained input fixture descriptor of one probe response."""
    artifacts = response.get("artifacts") if isinstance(response, dict) else None
    if not isinstance(artifacts, dict):
        return None, None
    fallback = None
    for key, entry in artifacts.items():
        if not isinstance(key, str):
            continue
        name = key.replace("\\", "/").strip("/")
        if name == "artifacts/fixture.json":
            return key, entry
        if fallback is None and (
            name == "fixture.json" or name.endswith("/fixture.json")
        ):
            fallback = (key, entry)
    return fallback if fallback is not None else (None, None)


def _resolve_fixture_path(directory, root, record, claimed):
    """Resolve a claimed fixture path; it must stay under the fixed job root."""
    path = Path(claimed)
    if path.is_absolute():
        resolved = path.resolve()
    else:
        candidates = [directory / path]
        output_rel = record.get("output_path")
        if isinstance(output_rel, str) and output_rel.strip():
            candidates.append(directory / output_rel / path)
        resolved = candidates[0].resolve()
        for candidate in candidates:
            probe = candidate.resolve()
            if probe.exists():
                resolved = probe
                break
    if resolved == root or root not in resolved.parents:
        raise CampaignError(f"fixture descriptor escapes the job root: {claimed}")
    return resolved


def _working_fixture(directory, job):
    """The verified input fixture of the latest successfully executed probe.

    The fixture is the initial input of that probe, not its final memory or
    startup state.  Every retained durable reference is considered so a probe
    stays usable after the display bound of the projected results.
    """
    latest = None
    for _ref, record in _verified_records(directory, job):
        action = record.get("action") if isinstance(record.get("action"), dict) else {}
        if action.get("kind") != "probe":
            continue
        if not record.get("ok", False):
            continue
        latest = record
    if latest is None:
        return None

    response = latest.get("response") if isinstance(latest.get("response"), dict) else {}
    key, descriptor = _fixture_descriptor(response)
    if key is None:
        return None
    if not isinstance(descriptor, dict):
        raise CampaignError(f"probe fixture descriptor is invalid: {key}")
    claimed = descriptor.get("path")
    if not isinstance(claimed, str) or not claimed.strip():
        raise CampaignError(f"probe fixture descriptor has no path: {key}")
    digest = descriptor.get("sha256")
    if not isinstance(digest, str) or not digest.strip():
        raise CampaignError(f"probe fixture descriptor has no sha256: {key}")

    root = (directory / "jobs" / job["id"]).resolve()
    path = _resolve_fixture_path(directory, root, latest, claimed)
    try:
        payload = path.read_bytes()
    except FileNotFoundError as exc:
        raise CampaignError(f"probe fixture is missing: {claimed}") from exc
    except OSError as exc:
        raise CampaignError(f"cannot read probe fixture {claimed}: {exc}") from exc
    if hashlib.sha256(payload).hexdigest() != digest.strip().lower():
        raise CampaignError(f"probe fixture hash mismatch: {claimed}")
    try:
        data = _loads(payload.decode("utf-8"))
    except (UnicodeDecodeError, TypeError, ValueError) as exc:
        raise CampaignError(f"invalid JSON in probe fixture {claimed}: {exc}") from exc
    if not isinstance(data, dict):
        raise CampaignError(f"probe fixture is not a JSON object: {claimed}")
    return {
        "artifact": _prefixed(latest.get("id"), key),
        "pin": _clone(descriptor),
        "data": data,
    }


def _ref_summary(record):
    """Short durable summary of one action record."""
    response = record.get("response") if isinstance(record.get("response"), dict) else {}
    result = response.get("result") if isinstance(response.get("result"), dict) else {}
    summary = result.get("summary")
    if isinstance(summary, dict):
        parts = []
        outcome = summary.get("outcome")
        if isinstance(outcome, str) and outcome.strip():
            parts.append(outcome[:MAX_SUMMARY])
        coverage = summary.get("coverage")
        if _is_number(coverage):
            parts.append(f"coverage={coverage}")
        if parts:
            return "; ".join(parts)[:MAX_SUMMARY]
    if isinstance(summary, str) and summary.strip():
        return summary[:MAX_SUMMARY]
    error = record.get("error")
    if isinstance(error, str) and error.strip():
        return error[:MAX_SUMMARY]
    kind = record.get("kind")
    return str(kind)[:MAX_SUMMARY] if kind else ""


def _write_record(directory, job, step, response=None, *, ok=True, error=None):
    """Write one immutable action record and return it with its durable ref."""
    action = step.get("action")
    record = {
        "id": step["action_id"],
        "phase": step["phase"],
        "round": int(job.get("round") or 1),
        "role": step.get("role"),
        "model": step.get("model"),
        "kind": action.get("kind") if isinstance(action, dict) else None,
        "action": _clone(action),
        "response": _clone(response),
        "result": _clone(response.get("result")) if isinstance(response, dict) else None,
        "ok": bool(ok),
        "output_path": step.get("output_rel"),
        "actor_response": _clone(step.get("actor_response")),
    }
    if isinstance(response, dict) and isinstance(response.get("ok"), bool):
        record["ok"] = bool(response["ok"])
    if step.get("receipt") is not None:
        record["receipt"] = _clone(step["receipt"])
    if error is not None:
        record["error"] = error
    record["hash"] = _record_hash(record)
    relative = f"jobs/{job['id']}/{step['action_id']}.json"
    path = directory / relative
    if path.exists():
        raise CampaignError(f"action record already exists: {relative}")
    _write_json(path, record)
    ref = {
        "id": step["action_id"],
        "kind": record["kind"],
        "phase": step["phase"],
        "ok": record["ok"],
        "path": relative,
        "hash": record["hash"],
        "summary": _ref_summary(record),
    }
    return record, ref


def _update(directory, job, changes, sleep):
    """Apply one change, retrying briefly on lock contention or a stale CAS."""
    last = None
    for attempt in range(UPDATE_ATTEMPTS):
        try:
            return provider_jobs.update(
                directory,
                job["id"],
                changes,
                expected_revision=job["revision"],
            )
        except (provider_jobs.StaleRevision, BusyError) as exc:
            last = exc
            if attempt == UPDATE_ATTEMPTS - 1:
                break
            state = provider_jobs.load(directory)
            job = state["jobs"][job["id"]]
            sleep(UPDATE_DELAY)
    raise CampaignError(f"could not update job {job.get('id')}: {last}")


def _hold(directory, job, reason, sleep, *, adapter=None, stage=None, results=None,
          extra=None):
    escalation = {"reason": reason}
    if adapter is not None:
        escalation["adapter"] = adapter
    if stage is not None:
        escalation["stage"] = stage
    changes = {
        "state": "held",
        "hold_reason": reason,
        "escalation": escalation,
        "checkpoint": None,
    }
    if results is not None:
        changes["results"] = results
    if extra:
        changes.update(extra)
    return _update(directory, job, changes, sleep)


def _validate_action(action, phase, mode):
    """Return an error message when the action does not match its schema."""
    kind = action["kind"]
    if phase == "review":
        allowed = REVIEW_ACTIONS
        label = "forbidden reviewer action"
    else:
        allowed = OWNER_ACTIONS
        label = "unsupported owner action"
    if kind not in allowed:
        return f"{label}: {kind}"
    if phase == "owner" and mode == "input" and kind in MUTATING_ACTIONS:
        return f"forbidden input owner action: {kind}"
    required, optional = ACTION_SCHEMAS[kind]
    allowed_keys = {"kind"} | set(required) | set(optional)
    unknown = sorted(set(action) - allowed_keys)
    if unknown:
        return f"unknown action key for {kind}: {', '.join(unknown)}"
    missing = [key for key in required if key not in action]
    if missing:
        return f"missing action key for {kind}: {', '.join(missing)}"
    for key, value in action.items():
        if key == "kind":
            continue
        if key in ('function', 'address') or (kind == 'evidence' and key == 'start'):
            if isinstance(value, bool) or not isinstance(value, (int, str)):
                return f'action {kind}.{key} must be an integer or hex address'
            try:
                address = int(value, 0) if isinstance(value, str) else value
            except ValueError:
                return f'action {kind}.{key} must be an integer or hex address'
            if not 0 <= address <= 0xFFFFFFFF:
                return f'action {kind}.{key} must fit an unsigned dword'
            continue
        expected = ACTION_VALUE_TYPES[key]
        if expected is int:
            if not _is_int(value):
                return f"action {kind}.{key} must be an integer"
        elif not isinstance(value, expected):
            return f"action {kind}.{key} must be a {expected.__name__}"
    if kind == "probe" and "base" in action:
        if not FILE_ID_RE.fullmatch(action["base"]):
            return "probe.base must be a file id (aNNNN/<relative path>)"
    if kind == "inspect":
        if not FILE_ID_RE.fullmatch(action["artifact"]):
            return "inspect.artifact must be a file id (aNNNN/<relative path>)"
        if action["section"] not in INSPECT_SECTIONS:
            return f"invalid inspect section: {action['section']}"
    if kind == "propose":
        for item in action["artifacts"]:
            if not isinstance(item, str) or not ACTION_ID_RE.fullmatch(item):
                return "propose.artifacts must list action ids (aNNNN)"
        if not action["summary"].strip():
            return "propose.summary must be nonempty"
    if kind == "hold" and not action["reason"].strip():
        return "hold.reason must be nonempty"
    if kind == "review":
        if action["verdict"] not in VERDICTS:
            return "invalid review verdict"
        for item in action["checks"]:
            if not isinstance(item, str) or not item.strip():
                return "review.checks must be a list of check names"
        for item in action["unknowns"]:
            if not isinstance(item, str) or not item.strip():
                return "review.unknowns must be a list of names"
    return None


def _parse_response(response, manifest, role):
    if not isinstance(response, dict):
        return None, "actor response must be an object", None, None
    model = response.get("model")
    expected = manifest["roles"][role]["model"]
    if not isinstance(model, str) or not model.strip():
        return None, "actor response model must be a nonempty string", None, None
    if model != expected:
        return (
            None,
            f"model mismatch: expected {expected!r} for {role}, observed {model!r}",
            None,
            None,
        )
    action = response.get("action")
    if not isinstance(action, dict):
        return None, "actor action must be a structured object", None, None
    kind = action.get("kind")
    if not isinstance(kind, str) or not kind.strip():
        return None, "actor action.kind must be a nonempty string", None, None
    receipt = response.get("receipt")
    if receipt is not None and not isinstance(receipt, dict):
        return None, "actor receipt must be an object", None, None
    try:
        action = _clone(action)
        if receipt is not None:
            receipt = _clone(receipt)
    except CampaignError as exc:
        return None, str(exc), None, None
    return action, None, model, receipt


class _DispatchExpired(Exception):
    """The executor was not invoked; its durable action remains pending."""


def _deadline_executor(execute, deadline, clock):
    def guarded(*args, **kwargs):
        # Check after context/record/state I/O, at the external call boundary.
        # This is the original run deadline, never a renewed action budget.
        if clock() >= deadline:
            raise _DispatchExpired
        return execute(*args, **kwargs)
    return guarded


def _execute_action(execute, job, action, output_rel, context):
    """Call the executor once and return its response object."""
    try:
        response = execute(
            _job_view(job),
            _clone(action),
            output_rel,
            prior=list(job.get("results") or []),
        )
    except _DispatchExpired:
        raise
    except Exception as exc:
        return None, f"execute error: {type(exc).__name__}: {exc}"
    try:
        response = _clone(response)
    except CampaignError as exc:
        return None, f"execute result is not JSON serializable: {exc}"
    if not isinstance(response, dict):
        return None, "execute response must be an object"
    return response, None


def _valid_bundle(response):
    if not isinstance(response, dict):
        return False
    if not isinstance(response.get("ready"), bool):
        return False
    sha = response.get("sha256")
    if not isinstance(sha, str) or not sha.strip():
        return False
    required = response.get("required_checks")
    if not isinstance(required, list) or not all(
        isinstance(item, str) and item.strip() for item in required
    ):
        return False
    return True


def _build_context(directory, job, phase, role, deadline, clock):
    config = job["config"]
    round_no = int(job.get("round") or 1)
    round_actions = int(job.get("round_actions") or 0)
    retries_used = int(job.get("provider_retries") or 0)
    retry_limit = int(config.get("provider_retries", DEFAULT_PROVIDER_RETRIES))
    context = {
        "phase": phase,
        "role": role,
        "round": round_no,
        "remaining_seconds": max(0.0, float(deadline) - float(clock())),
        "function": config["function"],
        "mode": config["mode"],
        "goal": config["goal"],
        "max_rounds": config["max_rounds"],
        "probe_count": int(job.get("probes") or 0),
        "results": _context_results(directory, job),
        "retained_results": len(job.get("results") or []),
        "feedback": list(job.get("feedback") or [])[-MAX_FEEDBACK:],
        "budget": {
            "round": round_no,
            "max_rounds": int(config["max_rounds"]),
            "actions_used": round_actions,
            "actions_remaining": max(0, MAX_ACTIONS_PER_ROUND - round_actions),
            "retries_used": retries_used,
            "retries_remaining": max(0, retry_limit - retries_used),
        },
    }
    failure = job.get("last_failure")
    previous_action = f"a{max(0, int(job.get('action_count') or 0) - 1):04d}"
    if isinstance(failure, dict) and failure.get("action") == previous_action:
        context["retry"] = {
            "action": failure.get("action"),
            "stage": failure.get("stage"),
            "reason": failure.get("error"),
            "hint": "retry with a smaller task: one artifact or one check",
            "retries_used": retries_used,
        }
    fixture = _working_fixture(directory, job)
    if fixture is not None:
        context["working_fixture"] = fixture
        context["guidance"] = [
            "use working_fixture.artifact "
            f"({fixture['artifact']}) as probe.base for cumulative edits",
            "an omitted probe.base resets the probe input to the original seed",
        ]
    else:
        context["guidance"] = [
            "an omitted probe.base resets the probe input to the original seed",
        ]
    if phase == "review":
        bundle = job.get("bundle")
        if isinstance(bundle, dict):
            context["bundle"] = _clone(bundle)
            context["required_checks"] = list(bundle.get("required_checks") or [])
    return context


def _actor_failure(directory, job, exc, step, sleep):
    """Hold, or retry the same phase when the adapter says the failure is retryable."""
    reason = f"actor error: {type(exc).__name__}: {exc}"
    retryable = getattr(exc, "retryable", False) is True
    limit = int(job["config"].get("provider_retries", DEFAULT_PROVIDER_RETRIES))
    used = int(job.get("provider_retries") or 0)
    failure = {
        "action": step["action_id"],
        "stage": step["stage_rel"],
        "phase": step["phase"],
        "role": step["role"],
        "error": reason[:MAX_ERROR],
        "retryable": retryable,
    }
    _, ref = _write_record(directory, job, step, ok=False, error=reason)
    results = [*list(job.get("results") or []), ref]
    if retryable and used < limit:
        _update(
            directory,
            job,
            {
                "state": "prepared" if step["phase"] == "owner" else "needs-review",
                "phase": step["phase"],
                "checkpoint": None,
                "results": results,
                "last_failure": failure,
                "failures": _append_failure(job, failure),
                "provider_retries": used + 1,
            },
            sleep,
        )
        return True
    _hold(
        directory,
        job,
        reason,
        sleep,
        adapter="actor",
        stage=step["stage_rel"],
        results=results,
        extra={"last_failure": failure, "failures": _append_failure(job, failure)},
    )
    return True


def _hold_action(directory, job, step, reason, sleep, *, adapter, extra=None):
    """Persist the failed action and hold the job."""
    results = list(job.get("results") or [])
    _, ref = _write_record(directory, job, step, ok=False, error=reason)
    return _hold(
        directory,
        job,
        reason,
        sleep,
        adapter=adapter,
        stage=step.get("stage"),
        results=[*results, ref],
        extra=extra,
    )


def _step_job(directory, job, manifest, actor, execute, deadline, clock, sleep):
    """Run at most one actor/executor action for one job."""
    try:
        return _dispatch_job(
            directory, job, manifest, actor, execute, deadline, clock, sleep
        )
    except Exception as exc:
        try:
            _hold(
                directory,
                job,
                f"controller error: {type(exc).__name__}: {exc}",
                sleep,
                adapter="controller",
            )
        except Exception:
            pass
        return True


def _dispatch_job(directory, job, manifest, actor, execute, deadline, clock, sleep):
    if clock() >= deadline:
        return False
    phase = _dispatch_phase(job)
    if phase is None:
        return False
    if phase == 'publish':
        return _publish_reviewed(directory, job, execute, deadline, clock, sleep)
    if phase == 'execute':
        return _execute_pending(directory, job, execute, deadline, clock, sleep)

    if int(job.get("round_actions") or 0) >= MAX_ACTIONS_PER_ROUND:
        checkpoint = job.get("checkpoint")
        stage = checkpoint.get("stage") if isinstance(checkpoint, dict) else None
        _hold(
            directory,
            job,
            "action budget exhausted; inspect retained rounds",
            sleep,
            adapter="controller",
            stage=stage,
        )
        return True

    config = job["config"]
    if phase == 'owner' and config['mode'] == 'c' and config.get('reserve_task'):
        try:
            execute.reserve(_job_view(job))
        except Exception as error:
            return _hold(directory, job, f'Task reservation failed before inference: {error}',
                         sleep, adapter='task-ownership')
    role = config["owner"] if phase == "owner" else config["reviewer"]
    round_no = int(job.get("round") or 1)
    action_no = int(job.get("action_count") or 0) + 1
    action_id = f"a{action_no:04d}"
    stage_rel = f"jobs/{job['id']}/rounds/{round_no}/{action_id}-{phase}"
    output_rel = f"jobs/{job['id']}/rounds/{round_no}/{action_id}-output"
    checkpoint = {
        "stage": stage_rel,
        "actor_stage": stage_rel,
        "action": action_id,
        "action_stage": f"jobs/{job['id']}/{action_id}.json",
        "execute_stage": output_rel,
        "phase": phase,
        "round": round_no,
        "role": role,
    }
    job = _update(
        directory,
        job,
        {
            "state": "running",
            "checkpoint": checkpoint,
            "phase": phase,
            "round": round_no,
            "action_count": action_no,
            "round_actions": int(job.get("round_actions") or 0) + 1,
            "attempts": int(job.get("attempts") or 0) + 1,
        },
        sleep,
    )

    stage_path = directory / stage_rel
    try:
        stage_path.mkdir(parents=True, exist_ok=False)
    except FileExistsError:
        _hold(
            directory,
            job,
            f"stage already exists: {stage_rel}",
            sleep,
            adapter="controller",
            stage=stage_rel,
        )
        return True

    try:
        context = _build_context(directory, job, phase, role, deadline, clock)
        _write_json(stage_path / "context.json", context)
    except CampaignError as exc:
        _hold(
            directory,
            job,
            f"context unavailable: {exc}",
            sleep,
            adapter="controller",
            stage=stage_rel,
        )
        return True

    step = {
        "action_id": action_id,
        "phase": phase,
        "role": role,
        "model": None,
        "action": None,
        "actor_response": None,
        "receipt": None,
        "output_rel": output_rel,
        "stage_rel": stage_rel,
        "stage": stage_rel,
    }

    try:
        response = actor(_job_view(job), role, context, str(stage_path))
    except Exception as exc:
        return _actor_failure(directory, job, exc, step, sleep)

    try:
        _clone(response)
    except CampaignError as exc:
        _hold(
            directory,
            job,
            f"actor response is not JSON serializable: {exc}",
            sleep,
            adapter="actor",
            stage=stage_rel,
        )
        return True

    step["actor_response"] = response
    if isinstance(response, dict):
        _write_json(stage_path / "response.json", response)

    action, error, model, receipt = _parse_response(response, manifest, role)
    step["model"] = model
    step["receipt"] = receipt
    step["action"] = action
    if error is not None:
        return _hold_action(
            directory, job, step, error, sleep, adapter="actor"
        )

    schema_error = _validate_action(action, phase, config["mode"])
    if schema_error is not None:
        return _hold_action(
            directory, job, step, schema_error, sleep, adapter="actor"
        )

    if config.get('defer_execution') and action['kind'] not in ('hold', 'review'):
        candidate = config.get('candidate')
        if candidate and Path(candidate).is_file():
            from .provider_runtime import _pin
            step['candidate_pin'] = _pin(candidate)
        _update(directory, job, {'state': 'awaiting-execution', 'phase': 'execute',
                                'pending_step': _clone(step), 'checkpoint': None}, sleep)
        return True

    if phase == "owner":
        return _owner_action(
            directory, job, step, execute, sleep, context
        )
    return _review_action(
        directory, job, step, execute, sleep, context
    )


def _execute_pending(directory, job, execute, deadline, clock, sleep):
    """Run a validated broker action without occupying a provider slot."""
    if clock() >= deadline:
        return False
    step = _clone(job['pending_step'])
    if step.get('candidate_pin'):
        from .provider_runtime import _verified
        _verified(step['candidate_pin'], directory / 'jobs' / job['id'])
    execute = _deadline_executor(execute, deadline, clock)
    try:
        if step.get('automatic'):
            context = _build_context(directory, job, step['phase'], step['role'], deadline, clock)
            _auto_bundle(directory, job, step, execute, sleep, context, prepared_step=step)
            return True
        job = _update(directory, job, {'state': 'running', 'phase': 'execute',
            'checkpoint': {'stage': step['output_rel'], 'phase': 'execute', 'action': step['action_id']}}, sleep)
        context = _build_context(directory, job, step['phase'], step['role'], deadline, clock)
        if step['phase'] == 'owner':
            return _owner_action(directory, job, step, execute, sleep, context)
        return _review_action(directory, job, step, execute, sleep, context)
    except _DispatchExpired:
        # No executor side effect occurred. Preserve the exact pending step,
        # candidate pin, task reservation, records and all lifetime counters.
        current = provider_jobs.load(directory)['jobs'][job['id']]
        _update(directory, current, {'state': 'awaiting-execution',
            'phase': 'execute', 'checkpoint': None}, sleep)
        return False


def _owner_action(directory, job, step, execute, sleep, context):
    action = step["action"]
    kind = action["kind"]
    results = list(job.get("results") or [])

    if kind == "hold":
        return _hold_action(
            directory, job, step, action["reason"], sleep, adapter="actor"
        )

    if kind == "propose":
        bundle_action = {
            "kind": "bundle",
            "artifacts": list(action["artifacts"]),
            "summary": action["summary"],
        }
        if 'resolution' in action:
            bundle_action['resolution'] = action['resolution']
        step["action"] = bundle_action
        step["stage"] = step["output_rel"]
        response, error = _execute_action(
            execute, job, bundle_action, step["output_rel"], context
        )
        if error is not None:
            return _hold_action(
                directory, job, step, error, sleep, adapter="execute"
            )
        if not _valid_bundle(response):
            return _hold_action(
                directory,
                job,
                step,
                "invalid bundle result: ready/sha256/required_checks required",
                sleep,
                adapter="execute",
            )
        _, ref = _write_record(directory, job, step, response)
        results = [*results, ref]
        if not response["ready"]:
            feedback = response.get("feedback") or response.get("reason")
            if not isinstance(feedback, str) or not feedback.strip():
                feedback = "bundle rejected; inspect required snapshots"
            _update(
                directory,
                job,
                {
                    "state": "prepared",
                    "phase": "owner",
                    "checkpoint": None,
                    "results": results,
                    "feedback": _append_feedback(job, feedback),
                },
                sleep,
            )
            return True
        _update(
            directory,
            job,
            {
                "state": "needs-review",
                "phase": "review",
                "checkpoint": None,
                "results": results,
                "bundle": _clone(response),
                "author_model": step["model"],
                "round": int(job.get("round") or 1),
            },
            sleep,
        )
        return True

    response, error = _execute_action(
        execute, job, action, step["output_rel"], context
    )
    if error is not None:
        step["stage"] = step["output_rel"]
        return _hold_action(
            directory, job, step, error, sleep, adapter="execute"
        )
    _, ref = _write_record(directory, job, step, response)
    results = [*results, ref]
    changes = {
        "state": "prepared",
        "phase": "owner",
        "checkpoint": None,
        "results": results,
        "probes": int(job.get("probes") or 0) + (1 if kind == "probe" else 0),
    }
    if kind == 'control-plan' and response.get('ok'):
        changes['next_stage'] = None
    updated = _update(directory, job, changes, sleep)
    automatic_gate = (kind == 'build' and job['config']['mode'] == 'c'
                      and job['config'].get('auto_test_after_build') is True)
    if (job['config'].get('auto_bundle') is True
            and (kind in ('probe', 'test', 'controls', 'control-plan') or automatic_gate)
            and isinstance(response, dict) and response.get('ok') is True):
        _auto_bundle(directory, updated, step, execute, sleep, context)
    return True


def _auto_bundle(directory, job, original_step, execute, sleep, context, *, prepared_step=None):
    """Run mechanical build/freeze steps without another paid planning turn.

    The existing executor validates the full bundle and independent review still
    owns approval. Each automatic action gets the same durable checkpoint and
    immutable record as an actor action; failures retain a precise hold.
    """
    actions = []
    if (original_step['action']['kind'] == 'build' and not original_step.get('automatic')
            and job['config']['mode'] == 'c' and job['config'].get('auto_test_after_build') is True):
        actions.append({'kind': 'test'})
    else:
        if original_step['action']['kind'] == 'test' and job['config']['mode'] == 'c':
            actions.append({'kind': 'build'})
        actions.append({'kind': 'bundle', 'artifacts': [],
                        'summary': 'Supervisor freezes all retained evidence after a successful action'})
    if prepared_step is not None:
        actions = [prepared_step['action']]
    for action in actions:
        if prepared_step is None and int(job.get('round_actions') or 0) >= MAX_ACTIONS_PER_ROUND:
            return
        action_id = f"a{int(job.get('action_count') or 0) + 1:04d}"
        output_rel = f"jobs/{job['id']}/rounds/{job.get('round', 1)}/{action_id}-output"
        if action['kind'] == 'bundle':
            refs = job.get('results', [])
            # Old builds/tests stay immutable history; a repaired source's
            # bundle uses its newest validation artifacts instead of mixing
            # incompatible successful builds from an earlier source.
            current_kinds = {'build', 'test', 'controls', 'control-plan'}
            latest = {ref['kind']: ref['id'] for ref in refs if ref['kind'] in current_kinds}
            records = {ref['id']: record for ref, record in _verified_records(directory, job)}
            test_record = records.get(latest.get('test'), {})
            current_hash = (test_record.get('response') or {}).get('source_sha256')
            action['artifacts'] = [ref['id'] for ref in refs
                if ref['kind'] not in ('bundle', 'finalize') and
                (ref['kind'] not in current_kinds or latest[ref['kind']] == ref['id']) and
                (ref['kind'] not in ('controls', 'control-plan') or current_hash and
                 (records[ref['id']].get('response') or {}).get('source_sha256') == current_hash)]
        step = {'action_id': action_id, 'phase': 'owner', 'role': original_step['role'],
                'model': original_step['model'], 'action': action,
                'automatic': True,
                'actor_response': None, 'output_rel': output_rel,
                'stage': output_rel,
                'receipt': {'supervisor': 'deterministic-build-and-freeze'}}
        if prepared_step is not None:
            step = prepared_step
            output_rel = step['output_rel']
        elif job['config'].get('defer_execution'):
            step['automatic'] = True
            candidate = job['config'].get('candidate')
            if candidate and Path(candidate).is_file():
                from .provider_runtime import _pin
                step['candidate_pin'] = _pin(candidate)
            _update(directory, job, {'state': 'awaiting-execution', 'phase': 'execute',
                'action_count': int(job.get('action_count') or 0) + 1,
                'round_actions': int(job.get('round_actions') or 0) + 1,
                'pending_step': step, 'checkpoint': None}, sleep)
            return
        job = _update(directory, job, {'state': 'running',
            'phase': 'execute' if prepared_step else 'owner',
            'action_count': int(job.get('action_count') or 0) + (0 if prepared_step else 1),
            'round_actions': int(job.get('round_actions') or 0) + (0 if prepared_step else 1),
            'checkpoint': {'stage': output_rel, 'phase': 'execute' if prepared_step else 'owner',
                           'action': step['action_id'], 'execute_stage': output_rel}}, sleep)
        context = dict(context, results=job.get('results', []))
        response, error = _execute_action(execute, job, action, output_rel, context)
        if error is not None:
            _hold_action(directory, job, step, error, sleep, adapter='execute')
            return
        _, ref = _write_record(directory, job, step, response)
        changes = {'state': 'prepared', 'phase': 'owner', 'checkpoint': None,
                   'results': [*job.get('results', []), ref]}
        if action['kind'] == 'bundle':
            if not _valid_bundle(response):
                _hold(directory, job, 'invalid automatic bundle result', sleep,
                      adapter='execute', stage=output_rel, results=changes['results'])
                return
            if response['ready']:
                changes.update(state='needs-review', phase='review', bundle=_clone(response),
                               author_model=original_step['model'])
            else:
                if (job['config'].get('require_controls') and not job['config'].get('control_plan')
                        and response.get('blocker') == 'controls-required'):
                    if job['config'].get('self_service_controls'):
                        changes.update(next_stage='controls', feedback=_append_feedback(job,
                            'Passing source/builds retained. Submit control-plan with a finite list '
                            'of named original source-site mutations; each must fail both normal '
                            'optimization gates. Every required helper site needs a control. '
                            'Each row: {site: preferred-image VA, name, transform: '
                            '{old, new, count}, expect: {kind: mismatch, field: regs/events/memory/detail/outcome}}. '
                            'No default outputs, fixture narrowing or canonical source edits.'))
                        _update(directory, job, changes, sleep)
                        return
                    _hold(directory, job,
                          'Passing source/builds retained; a pinned source-site control plan is required',
                          sleep, adapter='controls-plan', stage=output_rel,
                          results=changes['results'])
                    return
                changes['feedback'] = _append_feedback(job,
                    response.get('feedback') or 'Automatic bundle needs remaining evidence')
        elif not isinstance(response, dict) or not response.get('ok'):
            changes['feedback'] = _append_feedback(job,
                f"Automatic {action['kind']} failed; inspect retained result")
            _update(directory, job, changes, sleep)
            return
        job = _update(directory, job, changes, sleep)
        if action['kind'] == 'test' and response.get('ok'):
            _auto_bundle(directory, job, step, execute, sleep, context)
            return
        if prepared_step is not None and action['kind'] == 'build' and response.get('ok'):
            _auto_bundle(directory, job, step, execute, sleep, context)


def _review_action(directory, job, step, execute, sleep, context):
    action = step["action"]
    kind = action["kind"]
    results = list(job.get("results") or [])

    if kind == "hold":
        return _hold_action(
            directory, job, step, action["reason"], sleep, adapter="actor"
        )

    if kind != "review":
        response, error = _execute_action(
            execute, job, action, step["output_rel"], context
        )
        if error is not None:
            step["stage"] = step["output_rel"]
            return _hold_action(
                directory, job, step, error, sleep, adapter="execute"
            )
        _, ref = _write_record(directory, job, step, response)
        _update(
            directory,
            job,
            {
                "state": "needs-review",
                "phase": "review",
                "checkpoint": None,
                "results": [*results, ref],
                "probes": int(job.get("probes") or 0) + (1 if kind == "probe" else 0),
            },
            sleep,
        )
        return True

    bundle = job.get("bundle")
    if not isinstance(bundle, dict):
        return _hold_action(
            directory, job, step, "review without a stored bundle", sleep,
            adapter="actor",
        )

    if action.get("bundle_sha256") != bundle.get("sha256"):
        return _hold_action(
            directory,
            job,
            step,
            "stale bundle: bundle_sha256 does not match stored bundle",
            sleep,
            adapter="actor",
        )

    checks = action["checks"]
    unknowns = action["unknowns"]
    required = bundle.get("required_checks")
    if not isinstance(required, list):
        required = []
    unknown_checks = [name for name in checks if name not in required]
    if unknown_checks:
        return _hold_action(
            directory,
            job,
            step,
            "unknown review checks: " + ", ".join(unknown_checks),
            sleep,
            adapter="actor",
        )
    if action["verdict"] == "pass":
        missing = [name for name in required if name not in checks]
        if missing:
            return _hold_action(
                directory,
                job,
                step,
                "missing required checks: " + ", ".join(missing),
                sleep,
                adapter="actor",
            )

    reviews = int(job.get("reviews") or 0) + 1

    if action["verdict"] == "hold":
        reason = action.get("reason")
        if not isinstance(reason, str) or not reason.strip():
            reason = "reviewer hold"
        return _hold_action(
            directory,
            job,
            step,
            reason,
            sleep,
            adapter="actor",
            extra={
                "review": _clone(action),
                "review_model": step["model"],
                "reviews": reviews,
            },
        )

    if action["verdict"] == "revise":
        feedback = action.get("reason")
        if not isinstance(feedback, str) or not feedback.strip():
            return _hold_action(
                directory,
                job,
                step,
                "revise requires concrete feedback in reason",
                sleep,
                adapter="actor",
            )
        _, ref = _write_record(directory, job, step, {"verdict": "revise"})
        results = [*results, ref]
        new_round = int(job.get("round") or 1) + 1
        max_rounds = int(job["config"]["max_rounds"])
        if new_round > max_rounds:
            _update(
                directory,
                job,
                {
                    "state": "exhausted",
                    "hold_reason": "max_rounds exhausted",
                    "escalation": {
                        "reason": "max_rounds exhausted",
                        "round": int(job.get("round") or 1),
                        "max_rounds": max_rounds,
                    },
                    "review": _clone(action),
                    "review_model": step["model"],
                    "reviews": reviews,
                    "results": results,
                    "checkpoint": None,
                },
                sleep,
            )
            return True
        _update(
            directory,
            job,
            {
                "state": "prepared",
                "phase": "owner",
                "round": new_round,
                "round_actions": 0,
                "review": _clone(action),
                "review_model": step["model"],
                "reviews": reviews,
                "results": results,
                "feedback": _append_feedback(job, feedback),
                "checkpoint": None,
            },
            sleep,
        )
        return True

    if unknowns:
        return _hold_action(
            directory, job, step, "pass with nonempty unknowns", sleep, adapter="actor"
        )
    if step["model"] == job.get("author_model"):
        return _hold_action(
            directory,
            job,
            step,
            "self approval: reviewer model equals author model",
            sleep,
            adapter="actor",
        )

    if job['config'].get('defer_publication'):
        _, ref = _write_record(directory, job, step, {'verdict': 'pass'})
        _update(directory, job, {'state': 'reviewed', 'phase': 'publish',
            'review': _clone(action), 'review_model': step['model'], 'reviews': reviews,
            'results': [*results, ref], 'checkpoint': None}, sleep)
        return True

    finalize_action = {
        "kind": "finalize",
        "bundle": _clone(bundle),
        "review": _clone(action),
        "review_model": step["model"],
    }
    step["action"] = finalize_action
    step["stage"] = step["output_rel"]
    response, error = _execute_action(
        execute, job, finalize_action, step["output_rel"], context
    )
    if error is not None:
        return _hold_action(
            directory, job, step, error, sleep, adapter="execute"
        )
    _, ref = _write_record(directory, job, step, response)
    results = [*results, ref]
    final_state = response.get("state", response.get("final_state"))
    if final_state == "held":
        reason = response.get("reason")
        if not isinstance(reason, str) or not reason.strip():
            reason = "finalize held"
        _hold(
            directory,
            job,
            reason,
            sleep,
            adapter="execute",
            stage=step["output_rel"],
            results=results,
            extra={
                "review": _clone(action),
                "review_model": step["model"],
                "reviews": reviews,
            },
        )
        return True

    expected = "ready" if job["config"]["mode"] == "input" else "accepted"
    if final_state != expected:
        return _hold_action(
            directory,
            job,
            step,
            f"invalid finalize state {final_state!r}; expected {expected!r}",
            sleep,
            adapter="execute",
        )

    _update(
        directory,
        job,
        {
            "state": final_state,
            "phase": "done",
            "review": _clone(action),
            "review_model": step["model"],
            "reviews": reviews,
            "results": results,
            "checkpoint": None,
            "report": {
                "final_state": final_state,
                "bundle_sha256": bundle.get("sha256"),
                "author_model": job.get("author_model"),
                "review_model": step["model"],
                "rounds": int(job.get("round") or 1),
            },
        },
        sleep,
    )
    return True


def _publish_reviewed(directory, job, execute, deadline, clock, sleep):
    """Run the existing finalizer independently of paid reviewer capacity."""
    if clock() >= deadline:
        return False
    action_no = int(job.get('action_count') or 0) + 1
    action_id = f'a{action_no:04d}'
    output = f"jobs/{job['id']}/rounds/{job.get('round', 1)}/{action_id}-output"
    action = {'kind': 'finalize', 'bundle': job['bundle'],
              'review': job['review'], 'review_model': job['review_model']}
    step = {'action_id': action_id, 'phase': 'publish', 'role': None, 'model': None,
            'action': action, 'actor_response': None, 'receipt': None,
            'output_rel': output, 'stage_rel': output, 'stage': output}
    job = _update(directory, job, {'state': 'running', 'phase': 'publish',
        'action_count': action_no, 'checkpoint': {'stage': output, 'phase': 'publish'}}, sleep)
    try:
        response, error = _execute_action(
            _deadline_executor(execute, deadline, clock), job, action, output, {})
    except _DispatchExpired:
        # Keep publication approval and monotonic counters, not an interruption
        # hold: the finalizer has not run and can still be dispatched explicitly.
        _update(directory, job, {'state': 'reviewed', 'phase': 'publish',
            'checkpoint': None}, sleep)
        return False
    if error:
        return _hold_action(directory, job, step, error, sleep, adapter='execute')
    _, ref = _write_record(directory, job, step, response)
    expected = 'ready' if job['config']['mode'] == 'input' else 'accepted'
    if response.get('state') != expected:
        return _hold(directory, job, response.get('reason') or 'Publication gate refused',
                     sleep, results=[*job.get('results', []), ref], adapter='execute')
    _update(directory, job, {'state': expected, 'phase': 'done', 'checkpoint': None,
        'results': [*job.get('results', []), ref],
        'report': {'final_state': expected, 'bundle_sha256': job['bundle']['sha256'],
                   'author_model': job.get('author_model'), 'review_model': job['review_model']}}, sleep)
    return True


def _run_payload(state, started):
    duration = float(state["manifest"]["duration_seconds"])
    return {
        "schema": RUN_SCHEMA,
        "manifest_hash": state["manifest_hash"],
        "identity_hash": state["identity_hash"],
        "started_at": float(started),
        "deadline": float(started) + duration,
        "duration_seconds": duration,
    }


def _run_hash(payload):
    return _sha256_json({key: value for key, value in payload.items() if key != "hash"})


def _validate_run(metadata, state):
    """Validate run.json against the store; edited timing or identity is rejected."""
    if not isinstance(metadata, dict):
        raise CampaignError("run.json must be an object")
    if metadata.get("schema") != RUN_SCHEMA:
        raise CampaignError(f"run.json schema must be {RUN_SCHEMA}")
    if metadata.get("manifest_hash") != state["manifest_hash"]:
        raise CampaignError("run.json does not belong to this manifest")
    if metadata.get("identity_hash") != state["identity_hash"]:
        raise CampaignError("run.json does not belong to this identity")
    started = metadata.get("started_at")
    deadline = metadata.get("deadline")
    duration = metadata.get("duration_seconds")
    for value, name in (
        (started, "started_at"),
        (deadline, "deadline"),
        (duration, "duration_seconds"),
    ):
        if not _is_number(value):
            raise CampaignError(f"run.json {name} must be a finite number")
    expected = float(state["manifest"]["duration_seconds"])
    if float(duration) != expected:
        raise CampaignError("run.json duration_seconds does not match the manifest")
    if float(deadline) != float(started) + expected:
        raise CampaignError(
            "run.json deadline does not match started_at + duration_seconds"
        )
    if metadata.get("hash") != _run_hash(metadata):
        raise CampaignError("run.json hash mismatch")
    return {"started_at": float(started), "deadline": float(deadline)}


def _ensure_run(directory, state, clock):
    path = directory / "run.json"
    existing = _read_json(path)
    if existing is not None:
        _validate_run(existing, state)
        return existing
    payload = _run_payload(state, float(clock()))
    payload["hash"] = _run_hash(payload)
    _write_json(path, payload)
    return payload


def _hold_interrupted(directory, state, sleep):
    for job in list(state["jobs"].values()):
        if job.get("state") != "running":
            continue
        checkpoint = job.get("checkpoint")
        reason = "interrupted; inspect retained stage"
        escalation = {"reason": reason, "adapter": "controller"}
        if isinstance(checkpoint, dict):
            escalation["checkpoint"] = _clone(checkpoint)
        _update(
            directory,
            job,
            {
                "state": "held",
                "hold_reason": reason,
                "escalation": escalation,
                "checkpoint": None,
            },
            sleep,
        )


def _counts(state):
    counts = {
        "prepared": 0,
        "running": 0,
        "needs-review": 0,
        "ready": 0,
        "accepted": 0,
        "held": 0,
        "exhausted": 0,
        "cancelled": 0,
    }
    for job in state["jobs"].values():
        status = job.get("state")
        if status in counts:
            counts[status] += 1
    counts["pending"] = counts["prepared"]
    counts["needs_review"] = counts["needs-review"]
    return counts


def _status_jobs(state):
    jobs = []
    for job in state["jobs"].values():
        config = job.get("config", {})
        jobs.append(
            {
                "id": job["id"],
                "state": job.get("state"),
                "phase": job.get("phase"),
                "round": job.get("round", 1),
                "owner": config.get("owner"),
                "reviewer": config.get("reviewer"),
                "author_model": job.get("author_model"),
                "review_model": job.get("review_model"),
                "probes": job.get("probes", 0),
                "reviews": job.get("reviews", 0),
                "attempts": job.get("attempts", 0),
                "retries": job.get("provider_retries", 0),
                "hold_reason": job.get("hold_reason"),
                "report": job.get("report"),
                "results": [
                    ref.get("id")
                    for ref in (job.get("results") or [])
                    if isinstance(ref, dict)
                ],
            }
        )
    return jobs


def run(
    directory,
    *,
    actor,
    execute,
    identity=None,
    once=False,
    max_steps=None,
    service=None,
    limits=None,
    clock=time.time,
    sleep=time.sleep,
):
    """Run the durable provider orchestration loop."""
    directory = Path(directory)
    if not callable(actor) or not callable(execute):
        raise CampaignError("actor and execute must be callable")
    if identity is not None and not isinstance(identity, dict):
        raise CampaignError("identity must be an object")
    if max_steps is not None and (type(max_steps) is not int or max_steps < 1):
        raise CampaignError('max_steps must be a positive integer')

    with lock(directory / "run.lock"):
        state = provider_jobs.load(directory)
        if identity is not None and identity != state["identity"]:
            raise CampaignError("identity mismatch before actor/executor dispatch")
        metadata = _ensure_run(directory, state, clock)
        deadline = float(metadata["deadline"])
        _hold_interrupted(directory, state, sleep)

        executed = 0
        stopped = "idle"
        concurrency = int(state["manifest"].get("concurrency", 2))
        in_flight = {}
        with ThreadPoolExecutor(max_workers=concurrency + (16 if service else 0)) as pool:
            while True:
                state = provider_jobs.load(directory)
                expired = clock() >= deadline
                limited = max_steps is not None and executed >= max_steps
                pending_service = False
                if service is not None:
                    projection = service(state, min(deadline, clock()) if limited else deadline)
                    pending_service = bool(projection.get('pending'))
                    state = provider_jobs.load(directory)
                busy_ids = {job['id'] for job in in_flight.values()}
                candidates = [
                    job for job in state["jobs"].values()
                    if _dispatch_phase(job) and job['id'] not in busy_ids
                ]
                if (expired or limited or not candidates) and not in_flight and not pending_service:
                    stopped = "deadline" if expired else 'step-limit' if limited else "idle"
                    break
                manifest = state["manifest"]
                current_limits = limits(state) if limits else None
                if once:
                    batch = _dispatch_batch(candidates, 1,
                        role_capacity=manifest['role_capacity'],
                        stage_capacity=current_limits['workers'] if current_limits else None)
                    if not batch:
                        if pending_service:
                            sleep(0.1)
                            continue
                        stopped = 'capacity'
                        break
                    if _step_job(
                        directory,
                        batch[0],
                        manifest,
                        actor,
                        execute,
                        deadline,
                        clock,
                        sleep,
                    ):
                        executed += 1
                    stopped = "once"
                    break
                capacities = dict(manifest['role_capacity'])
                stage_capacity = dict(current_limits['workers']) if current_limits else None
                owner_wip = {role: n * 2 for role, n in capacities.items()}
                for job in in_flight.values():
                    role = _dispatch_role(job)
                    if role is not None:
                        capacities[role] -= 1
                    if stage_capacity is not None:
                        from .pipeline_policy import stage
                        stage_capacity[stage(job)] -= 1
                    if role is not None and _dispatch_phase(job) == 'owner':
                        owner_wip[role] -= 1
                ready_inputs = sum(j.get('state') == 'ready' and not j.get('adoption_receipt')
                                   and not j.get('pipeline_hold')
                                   and j['config']['mode'] == 'input'
                                   for j in state['jobs'].values())
                ready_limit = (current_limits['input_ready_limit'] if current_limits else
                               manifest.get('input_ready_limit', concurrency * 2))
                if ready_inputs >= ready_limit:
                    candidates = [j for j in candidates if _dispatch_phase(j) == 'review'
                                  or _dispatch_phase(j) in ('publish', 'execute') or j['config']['mode'] == 'c']
                if not expired:
                    execution_workers = current_limits['execution_workers'] if current_limits else 0
                    free = concurrency + (execution_workers if service else 0) - len(in_flight)
                    if max_steps is not None:
                        free = min(free, max_steps - executed - len(in_flight))
                    for job in _dispatch_batch(candidates, free,
                                               role_capacity=capacities, owner_wip=owner_wip,
                                               stage_capacity=stage_capacity,
                                               provider_capacity=concurrency - sum(
                                                   _dispatch_role(j) is not None for j in in_flight.values()),
                                               execution_capacity=(execution_workers - sum(
                                                   _dispatch_role(j) is None for j in in_flight.values()))
                                                   if service else None):
                        future = pool.submit(_step_job, directory, job, manifest,
                                             actor, execute, deadline, clock, sleep)
                        in_flight[future] = job
                if not in_flight:
                    if pending_service:
                        sleep(0.1)
                        continue
                    stopped = 'capacity'
                    break
                done, _ = wait(in_flight, timeout=0.25 if service or limits else None,
                               return_when=FIRST_COMPLETED)
                for future in done:
                    del in_flight[future]
                    if future.result():
                        executed += 1
        return _run_summary(directory, executed, stopped, clock)


def _run_summary(directory, executed, stopped, clock):
    state = provider_jobs.load(directory)
    metadata = _read_json(directory / "run.json") or {}
    timing = _validate_run(metadata, state)
    now = float(clock())
    return {
        "schema": SCHEMA,
        "directory": str(directory),
        "executed": executed,
        "stopped": stopped,
        "started_at": timing["started_at"],
        "deadline": timing["deadline"],
        "elapsed_seconds": max(0.0, now - timing["started_at"]),
        "counts": _counts(state),
        "jobs": {
            job_id: {
                "state": job.get("state"),
                "phase": job.get("phase"),
                "round": job.get("round", 1),
            }
            for job_id, job in state["jobs"].items()
        },
    }


def status(directory):
    """Return a compact, read-only campaign summary."""
    directory = Path(directory)
    state = provider_jobs.load(directory)
    metadata = _read_json(directory / "run.json")
    started = None
    deadline = None
    if metadata is not None:
        timing = _validate_run(metadata, state)
        started = timing["started_at"]
        deadline = timing["deadline"]
    now = time.time()
    elapsed = max(0.0, now - started) if started is not None else 0.0
    return {
        "schema": SCHEMA,
        "duration_seconds": float(state["manifest"]["duration_seconds"]),
        "started_at": started,
        "deadline": deadline,
        "elapsed_seconds": elapsed,
        "expired": deadline is not None and now >= deadline,
        "counts": _counts(state),
        "jobs": _status_jobs(state),
    }
