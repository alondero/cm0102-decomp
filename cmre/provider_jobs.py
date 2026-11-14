"""Durable metadata store for provider-led campaigns.

Layout of a store directory:

``genesis.json``
    Immutable manifest, identity and job definitions, pinned by ``genesis_hash``.
``events/NNNNNNNN.json``
    The complete hash-chained change log.  Every event is retained.
``state.json``
    A snapshot at ``revision``.  The snapshot is never trusted: every load
    rebuilds the state from genesis, replays the whole chain and compares the
    rebuilt snapshot with the stored one, so a well-formed edit of
    ``state.json`` is detected instead of trusted.
"""

from __future__ import annotations

import errno
import hashlib
import json
import math
import time
from pathlib import Path

from .coordination import atomic_json, lock

SCHEMA = "cmre-provider-jobs/1"
GENESIS_HASH = "0" * 64
ALLOWED_STATES = frozenset(
    {
        "prepared",
        "running",
        "needs-review",
        "reviewed",
        "awaiting-execution",
        "ready",
        "accepted",
        "held",
        "exhausted",
        "cancelled",
    }
)
TERMINAL_STATES = frozenset({"ready", "accepted", "exhausted", "cancelled"})
PROTECTED_JOB_FIELDS = frozenset({"id", "config", "revision", "history"})
RESERVED_RETRY_FIELDS = frozenset(
    {
        "results",
        "attempts",
        "probes",
        "reviews",
        "round",
        "action_count",
        "round_actions",
        "provider_retries",
    }
)
MAX_HISTORY = 64
MAX_HISTORY_FIELDS = 16
READ_ATTEMPTS = 5
READ_RETRY_DELAY = 0.01
_TRANSIENT_READ_WINERRORS = frozenset({5, 32, 33})
_TRANSIENT_READ_ERRNOS = frozenset(
    code
    for code in (
        getattr(errno, "EACCES", None),
        getattr(errno, "EPERM", None),
        getattr(errno, "EBUSY", None),
    )
    if code is not None
)

__all__ = [
    "ProviderJobsError",
    "CorruptStore",
    "StaleRevision",
    "UnknownJob",
    "InvalidChange",
    "create",
    "load",
    "update",
]


class ProviderJobsError(ValueError):
    """Base error for provider job store failures."""


class CorruptStore(ProviderJobsError):
    """The on-disk store is missing, inconsistent, or tampered with."""


class StaleRevision(ProviderJobsError):
    """The caller supplied an outdated job revision."""


class UnknownJob(ProviderJobsError):
    """The requested job does not exist."""


class InvalidChange(ProviderJobsError):
    """A change or JSON value is not acceptable."""


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


def _validate_json(value, path="value"):
    if value is None or isinstance(value, (bool, str)):
        return
    if isinstance(value, int) and not isinstance(value, bool):
        return
    if isinstance(value, float):
        if not math.isfinite(value):
            raise InvalidChange(f"{path} must be finite")
        return
    if isinstance(value, list):
        for index, item in enumerate(value):
            _validate_json(item, f"{path}[{index}]")
        return
    if isinstance(value, dict):
        for key, item in value.items():
            if not isinstance(key, str):
                raise InvalidChange(f"{path} keys must be strings")
            _validate_json(item, f"{path}.{key}")
        return
    raise InvalidChange(f"{path} is not JSON serializable")


def _clone(value):
    _validate_json(value)
    try:
        text = json.dumps(
            value,
            allow_nan=False,
            sort_keys=True,
            separators=(",", ":"),
            ensure_ascii=False,
        )
        return json.loads(text)
    except (TypeError, ValueError) as exc:
        raise InvalidChange(f"value is not JSON serializable: {exc}") from exc


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


def _is_transient_read_error(exc):
    if isinstance(exc, PermissionError):
        return True
    if isinstance(exc, OSError):
        if getattr(exc, "winerror", None) in _TRANSIENT_READ_WINERRORS:
            return True
        if getattr(exc, "errno", None) in _TRANSIENT_READ_ERRNOS:
            return True
    return False


def _read_error_detail(exc):
    name = type(exc).__name__
    parts = []
    winerror = getattr(exc, "winerror", None)
    if winerror is not None:
        parts.append(f"winerror={winerror}")
    code = getattr(exc, "errno", None)
    if code is not None:
        parts.append(f"errno={code}")
    if parts:
        return f"{name}: {', '.join(parts)}"
    return name


def _read_json(path):
    path = Path(path)
    for attempt in range(READ_ATTEMPTS):
        try:
            text = path.read_text(encoding="utf-8")
        except FileNotFoundError as exc:
            raise CorruptStore(f"missing file: {path}") from exc
        except OSError as exc:
            if _is_transient_read_error(exc) and attempt + 1 < READ_ATTEMPTS:
                time.sleep(READ_RETRY_DELAY)
                continue
            raise CorruptStore(
                f"unreadable file: {path} ({_read_error_detail(exc)})"
            ) from exc
        break
    try:
        return _loads(text)
    except (TypeError, ValueError) as exc:
        raise CorruptStore(f"invalid JSON in {path}: {exc}") from exc


def _genesis_payload(manifest, identity, definitions):
    return {
        "schema": SCHEMA,
        "manifest": manifest,
        "identity": identity,
        "manifest_hash": _sha256_json(manifest),
        "identity_hash": _sha256_json(identity),
        "jobs": definitions,
    }


def _state_from_genesis(genesis):
    jobs = {}
    for definition in genesis["jobs"]:
        jobs[definition["id"]] = {
            "id": definition["id"],
            "config": _clone(definition["config"]),
            "revision": 0,
            "state": "prepared",
            "history": [],
        }
    return {
        "schema": SCHEMA,
        "revision": 0,
        "genesis_hash": genesis["genesis_hash"],
        "manifest": _clone(genesis["manifest"]),
        "identity": _clone(genesis["identity"]),
        "manifest_hash": genesis["manifest_hash"],
        "identity_hash": genesis["identity_hash"],
        "jobs": jobs,
    }


def _load_genesis(directory):
    genesis = _read_json(directory / "genesis.json")
    if not isinstance(genesis, dict) or genesis.get("schema") != SCHEMA:
        raise CorruptStore("invalid genesis")
    payload = {key: value for key, value in genesis.items() if key != "genesis_hash"}
    try:
        digest = _sha256_json(payload)
    except (TypeError, ValueError) as exc:
        raise CorruptStore("genesis is not canonical JSON") from exc
    if genesis.get("genesis_hash") != digest:
        raise CorruptStore("genesis hash mismatch")
    manifest = genesis.get("manifest")
    identity = genesis.get("identity")
    if not isinstance(manifest, dict) or not isinstance(identity, dict):
        raise CorruptStore("invalid genesis manifest or identity")
    if genesis.get("manifest_hash") != _sha256_json(manifest):
        raise CorruptStore("genesis manifest hash mismatch")
    if genesis.get("identity_hash") != _sha256_json(identity):
        raise CorruptStore("genesis identity hash mismatch")
    definitions = genesis.get("jobs")
    if not isinstance(definitions, list):
        raise CorruptStore("invalid genesis jobs")
    for definition in definitions:
        if (
            not isinstance(definition, dict)
            or not isinstance(definition.get("id"), str)
            or not definition["id"]
            or not isinstance(definition.get("config"), dict)
        ):
            raise CorruptStore("invalid genesis job definition")
    return genesis


def _validate_changes(changes, job):
    if not isinstance(changes, dict):
        raise InvalidChange("changes must be an object")
    for field in PROTECTED_JOB_FIELDS:
        if field in changes:
            raise InvalidChange(f"cannot change {field}")
    if "retry_reason" in changes and not isinstance(changes["retry_reason"], str):
        raise InvalidChange("retry_reason must be a string")
    if "state" not in changes:
        return
    new_state = changes["state"]
    if not isinstance(new_state, str) or new_state not in ALLOWED_STATES:
        raise InvalidChange("invalid state")
    current = job["state"]
    if current in TERMINAL_STATES and new_state != current:
        raise InvalidChange("terminal job cannot reopen")
    if current == "held" and new_state != "held":
        if new_state == "prepared":
            pass
        elif new_state == "needs-review":
            # A narrow reopen for a held review: the reviewer keeps the very
            # same bundle, so no intermediate owner event is needed.
            if job.get("phase") != "review":
                raise InvalidChange(
                    "held job reopen to needs-review requires the review phase"
                )
            bundle = job.get("bundle")
            if not isinstance(bundle, dict) or not bundle:
                raise InvalidChange(
                    "held job reopen to needs-review requires a retained bundle"
                )
        else:
            raise InvalidChange("held job may only reopen to prepared")
        reason = changes.get("retry_reason")
        if not isinstance(reason, str) or not reason.strip():
            raise InvalidChange("held job reopen requires retry_reason")
        for field in sorted(RESERVED_RETRY_FIELDS & set(changes)):
            raise InvalidChange(f"held job reopen must preserve {field}")


def _history_entry(changes, revision, sequence, state):
    fields = sorted(changes)
    return {
        "revision": revision,
        "sequence": sequence,
        "state": state,
        "fields": fields[:MAX_HISTORY_FIELDS],
        "field_count": len(fields),
    }


def _apply_changes_to_job(job, changes, revision, sequence):
    for field, value in changes.items():
        job[field] = _clone(value)
    job["revision"] = revision
    job["history"].append(
        _history_entry(changes, revision, sequence, job["state"])
    )
    if len(job["history"]) > MAX_HISTORY:
        del job["history"][:-MAX_HISTORY]


def _validate_event(event, index, prior_hash):
    if not isinstance(event, dict):
        raise CorruptStore("event must be an object")
    if event.get("schema") != SCHEMA:
        raise CorruptStore("unsupported event schema")
    for field in (
        "sequence",
        "prior_revision",
        "revision",
        "prior_job_revision",
        "job_revision",
    ):
        value = event.get(field)
        if not isinstance(value, int) or isinstance(value, bool):
            raise CorruptStore(f"invalid event {field}")
    if event["sequence"] != index or event["revision"] != index:
        raise CorruptStore("event sequence mismatch")
    if event["prior_revision"] != index - 1:
        raise CorruptStore("event prior revision mismatch")
    if event["job_revision"] != event["prior_job_revision"] + 1:
        raise CorruptStore("event job revision mismatch")
    if not isinstance(event.get("job_id"), str) or not event["job_id"]:
        raise CorruptStore("invalid event job id")
    if not isinstance(event.get("changes"), dict):
        raise CorruptStore("invalid event changes")
    if event.get("prior_hash") != prior_hash:
        raise CorruptStore("event hash chain mismatch")
    digest = event.get("hash")
    if not isinstance(digest, str) or len(digest) != 64:
        raise CorruptStore("invalid event hash")
    payload = {key: value for key, value in event.items() if key != "hash"}
    try:
        actual = _sha256_json(payload)
    except (TypeError, ValueError) as exc:
        raise CorruptStore("event is not canonical JSON") from exc
    if actual != digest:
        raise CorruptStore("event hash mismatch")


def _apply_event(state, event):
    if state["revision"] != event["prior_revision"]:
        raise CorruptStore("event does not follow state revision")
    operation = event.get('operation', 'update')
    if operation == 'enqueue':
        definition = event.get('definition')
        if (not isinstance(definition, dict) or set(definition) != {'id', 'config'}
                or definition['id'] != event['job_id']
                or not isinstance(definition['config'], dict)
                or event['job_id'] in state['jobs']
                or event['prior_job_revision'] != 0):
            raise CorruptStore('invalid or duplicate enqueued definition')
        state['jobs'][event['job_id']] = {
            'id': definition['id'], 'config': _clone(definition['config']),
            'revision': 0, 'state': 'prepared', 'history': []}
    elif operation != 'update':
        raise CorruptStore('unknown event operation')
    job = state["jobs"].get(event["job_id"])
    if job is None:
        raise CorruptStore("event references unknown job")
    if job["revision"] != event["prior_job_revision"]:
        raise CorruptStore("event job revision mismatch")
    try:
        _validate_changes(event["changes"], job)
        _apply_changes_to_job(
            job,
            event["changes"],
            event["job_revision"],
            event["sequence"],
        )
    except InvalidChange as exc:
        raise CorruptStore(f"invalid retained event: {exc}") from exc
    state["revision"] = event["revision"]


def _event_names(directory):
    events_directory = directory / "events"
    names = []
    if events_directory.exists():
        if not events_directory.is_dir():
            raise CorruptStore("events path is not a directory")
        for entry in events_directory.iterdir():
            if entry.name.startswith("."):
                continue
            if not entry.is_file():
                raise CorruptStore("invalid event entry")
            names.append(entry.name)
    names.sort()
    for index, name in enumerate(names, 1):
        if name != f"{index:08d}.json":
            raise CorruptStore("gapped or reordered events")
    return names


def _read_events(directory):
    events = []
    prior_hash = GENESIS_HASH
    for index, name in enumerate(_event_names(directory), 1):
        event = _read_json(directory / "events" / name)
        _validate_event(event, index, prior_hash)
        prior_hash = event["hash"]
        events.append(event)
    return events


def _load(directory):
    """Rebuild the store from genesis and the event chain, verifying the snapshot."""
    directory = Path(directory)
    genesis = _load_genesis(directory)
    snapshot = _read_json(directory / "state.json")
    if not isinstance(snapshot, dict):
        raise CorruptStore("state must be an object")
    if snapshot.get("schema") != SCHEMA:
        raise CorruptStore("unsupported schema")
    revision = snapshot.get("revision")
    if not isinstance(revision, int) or isinstance(revision, bool) or revision < 0:
        raise CorruptStore("invalid store revision")
    if snapshot.get("genesis_hash") != genesis["genesis_hash"]:
        raise CorruptStore("snapshot does not belong to this genesis")
    events = _read_events(directory)
    if revision > len(events):
        raise CorruptStore("state is ahead of event log")
    state = _state_from_genesis(genesis)
    for event in events[:revision]:
        _apply_event(state, event)
    if state != snapshot:
        raise CorruptStore("snapshot does not match the reconstructed event log")
    for event in events[revision:]:
        _apply_event(state, event)
    return state


def create(directory, manifest, identity):
    """Create a new store and return its initial state."""
    directory = Path(directory)
    manifest = _clone(manifest)
    identity = _clone(identity)
    if not isinstance(manifest, dict) or not isinstance(identity, dict):
        raise InvalidChange("manifest and identity must be objects")
    jobs_spec = manifest.get("jobs")
    if not isinstance(jobs_spec, list):
        raise InvalidChange("manifest.jobs must be a list")
    definitions = []
    seen = set()
    for item in jobs_spec:
        if not isinstance(item, dict):
            raise InvalidChange("each job must be an object")
        job_id = item.get("id")
        if not isinstance(job_id, str) or not job_id:
            raise InvalidChange("job id must be a nonempty string")
        if job_id in seen:
            raise InvalidChange(f"duplicate job id: {job_id}")
        seen.add(job_id)
        definitions.append({"id": job_id, "config": _clone(item.get("config", {}))})
    genesis = _genesis_payload(manifest, identity, definitions)
    genesis["genesis_hash"] = _sha256_json(genesis)
    state = _state_from_genesis(genesis)
    try:
        directory.mkdir(parents=True, exist_ok=False)
    except FileExistsError:
        raise FileExistsError(f"store already exists: {directory}") from None
    (directory / "events").mkdir()
    atomic_json(directory / "genesis.json", genesis)
    atomic_json(directory / "state.json", state)
    return _clone(state)


def load(directory):
    """Load a store, rebuilding it from genesis and replaying committed events."""
    return _clone(_load(Path(directory)))


def update(directory, job_id, changes, *, expected_revision):
    """Apply one revision-checked change to a job and return the updated job."""
    directory = Path(directory)
    if not isinstance(job_id, str) or not job_id:
        raise UnknownJob("unknown job")
    if (
        not isinstance(expected_revision, int)
        or isinstance(expected_revision, bool)
        or expected_revision < 0
    ):
        raise InvalidChange("expected_revision must be a nonnegative integer")
    changes = _clone(changes)
    if not isinstance(changes, dict):
        raise InvalidChange("changes must be an object")
    with lock(directory / ".lock"):
        state = _load(directory)
        job = state["jobs"].get(job_id)
        if job is None:
            raise UnknownJob(f"unknown job: {job_id}")
        if job["revision"] != expected_revision:
            raise StaleRevision(
                f"job {job_id} is at revision {job['revision']}, "
                f"expected {expected_revision}"
            )
        _validate_changes(changes, job)
        sequence = state["revision"] + 1
        revision = job["revision"] + 1
        prior_hash = GENESIS_HASH
        if sequence > 1:
            previous = _read_json(directory / "events" / f"{sequence - 1:08d}.json")
            digest = previous.get("hash") if isinstance(previous, dict) else None
            if not isinstance(digest, str) or len(digest) != 64:
                raise CorruptStore("invalid previous event hash")
            prior_hash = digest
        event = {
            "schema": SCHEMA,
            "sequence": sequence,
            "prior_revision": state["revision"],
            "revision": sequence,
            "job_id": job_id,
            "prior_job_revision": job["revision"],
            "job_revision": revision,
            "changes": changes,
            "prior_hash": prior_hash,
        }
        event["hash"] = _sha256_json(event)
        event_path = directory / "events" / f"{sequence:08d}.json"
        if event_path.exists():
            raise CorruptStore("event already exists")
        atomic_json(event_path, event)
        _apply_changes_to_job(job, changes, revision, sequence)
        state["revision"] = sequence
        atomic_json(directory / "state.json", state)
        return _clone(job)


def enqueue(directory, definition):
    """Append one frozen definition without changing genesis or the run clock.

    Exact replays are idempotent. Existing definitions can never be replaced.
    Campaign validation and eligibility are the caller's responsibility.
    """
    directory = Path(directory)
    definition = _clone(definition)
    if (not isinstance(definition, dict) or set(definition) != {'id', 'config'}
            or not isinstance(definition['id'], str) or not definition['id']
            or not isinstance(definition['config'], dict)):
        raise InvalidChange('A frozen job definition is required')
    with lock(directory / '.lock'):
        state = _load(directory)
        existing = state['jobs'].get(definition['id'])
        if existing:
            if existing['config'] != definition['config']:
                raise InvalidChange('Enqueued job definition changed')
            return _clone(existing)
        sequence = state['revision'] + 1
        prior_hash = GENESIS_HASH if sequence == 1 else _read_json(
            directory / 'events' / f'{sequence-1:08d}.json')['hash']
        event = {'schema': SCHEMA, 'operation': 'enqueue', 'sequence': sequence,
                 'prior_revision': state['revision'], 'revision': sequence,
                 'prior_job_revision': 0, 'job_revision': 1,
                 'job_id': definition['id'], 'definition': definition,
                 'changes': {'state': 'prepared'}, 'prior_hash': prior_hash}
        event['hash'] = _sha256_json(event)
        atomic_json(directory / 'events' / f'{sequence:08d}.json', event)
        _apply_event(state, event)
        atomic_json(directory / 'state.json', state)
        return _clone(state['jobs'][definition['id']])
