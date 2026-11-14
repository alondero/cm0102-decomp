"""Scoped, original-only provider broker.

The broker executes a small set of read-only actions against the pinned
original target, retains the complete raw artifacts on disk and returns
bounded JSON summaries.  It never publishes canonical data, never accepts
model chosen compiler options and never performs shell or network actions.

Public surface::

    dispatch(job, action, output, *, prior=None, backend=None, candidate=None)

``backend`` defaults to :class:`LocalBackend`, which lazily imports the
existing ``cmre`` modules.  Tests inject an asset free fake backend.
"""

from __future__ import annotations

import gzip
import hashlib
import importlib
import json
import os
import shutil
from collections.abc import Mapping
from pathlib import Path
from typing import Any, Callable, Optional

__all__ = ["BrokerError", "LocalBackend", "dispatch"]

_MAX_LIMIT = 120
_DEFAULT_LIMIT = 80
_MAX_MEMORY = 128
_MAX_SUMMARY_BYTES = 256 * 1024
_MAX_DEPTH = 12
_U32_MAX = 0xFFFFFFFF

_RECEIPT_NAME = "receipt.json"
_RESPONSE_NAME = "response.json"
_ARTIFACT_DIR = "artifacts"

_ACTION_KEYS = {
    "evidence": frozenset({"kind", "function", "start", "limit"}),
    "memory": frozenset({"kind", "address", "size"}),
    "probe": frozenset({"kind", "request", "base"}),
    "inspect": frozenset({"kind", "artifact", "section", "start", "limit"}),
    "build": frozenset({"kind"}),
    "screen": frozenset({"kind"}),
}

_SECTION_PATHS = {
    "instructions": ("instructions", "insns", "result.instructions", "result.insns"),
    "events": ("result.events", "events"),
    "arrivals": ("result.arrivals", "arrivals"),
    "returns": ("result.returns", "returns"),
    "registered_helper_states": (
        "result.registered_helper_states",
        "registered_helper_states",
    ),
    "disassembly": ("disassembly", "result.disassembly"),
}

_KIND_BY_NAME = {
    "raw.json.gz": "raw",
    "input.json.gz": "input",
    "fixture.json": "fixture",
    "execution.json": "execution",
    "instructions.json": "instructions",
}

_PROVIDER_PROBE: Any = None


class BrokerError(RuntimeError):
    """Raised for every refusal or validation failure of the broker."""


# strict JSON helpers


def _reject_constant(name: str) -> Any:
    raise BrokerError(f"JSON contains the non finite constant {name!r}")


def _unique_pairs(pairs: list[tuple[str, Any]]) -> dict[str, Any]:
    result: dict[str, Any] = {}
    for key, value in pairs:
        if key in result:
            raise BrokerError(f"JSON contains the duplicate key {key!r}")
        result[key] = value
    return result


def _strict_loads(text: str) -> Any:
    try:
        return json.loads(
            text, object_pairs_hook=_unique_pairs, parse_constant=_reject_constant
        )
    except BrokerError:
        raise
    except ValueError as exc:
        raise BrokerError(f"invalid JSON: {exc}") from exc


def _canonical_bytes(obj: Any) -> bytes:
    try:
        text = json.dumps(
            obj,
            sort_keys=True,
            allow_nan=False,
            separators=(",", ":"),
        )
    except (TypeError, ValueError) as exc:
        raise BrokerError(f"value is not strict JSON: {exc}") from exc
    return text.encode("utf-8")


def _sanitize(value: Any, depth: int = 0) -> Any:
    """Return a strict JSON safe copy of ``value``."""

    if depth > _MAX_DEPTH:
        return "<max depth>"
    if value is None or isinstance(value, (str, bool, int)):
        return value
    if isinstance(value, float):
        if value != value or value in (float("inf"), float("-inf")):
            return repr(value)
        return value
    if isinstance(value, (bytes, bytearray, memoryview)):
        return bytes(value).hex()
    if isinstance(value, Mapping):
        return {str(key): _sanitize(item, depth + 1) for key, item in value.items()}
    if isinstance(value, (set, frozenset)):
        return [_sanitize(item, depth + 1) for item in sorted(value, key=repr)]
    if isinstance(value, (list, tuple)):
        return [_sanitize(item, depth + 1) for item in value]
    return repr(value)


def _sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


# small validation helpers


def _parse_address(value: Any, label: str) -> int:
    if isinstance(value, bool) or not isinstance(value, (int, str)):
        raise BrokerError(f"{label} must be an address")
    if isinstance(value, int):
        address = value
    else:
        text = value.strip()
        if not text:
            raise BrokerError(f"{label} must not be empty")
        try:
            address = int(text, 0)
        except ValueError:
            try:
                address = int(text, 16)
            except ValueError:
                raise BrokerError(f"{label} {value!r} is not an address") from None
    if address < 0 or address > _U32_MAX:
        raise BrokerError(f"{label} must be within 0..0x{_U32_MAX:X}")
    return address


def _limit(action: Mapping[str, Any], label: str, default: int = _DEFAULT_LIMIT) -> int:
    value = action.get("limit", default)
    if isinstance(value, bool) or not isinstance(value, int):
        raise BrokerError(f"{label} must be an integer")
    if value < 1 or value > _MAX_LIMIT:
        raise BrokerError(f"{label} must be within 1..{_MAX_LIMIT}")
    return value


def _is_within(path: Path, root: Path) -> bool:
    """Resolve both paths (following symlinks) and test containment."""

    try:
        return Path(path).resolve().is_relative_to(Path(root).resolve())
    except (OSError, RuntimeError, ValueError):
        return False


def _provider_probe() -> Any:
    global _PROVIDER_PROBE
    if _PROVIDER_PROBE is None:
        try:
            _PROVIDER_PROBE = importlib.import_module("cmre.provider_probe")
        except Exception as exc:
            raise BrokerError(f"cmre.provider_probe is unavailable: {exc}") from exc
    return _PROVIDER_PROBE


# job / action validation


def _validate_job(job: Any) -> dict[str, Any]:
    if not isinstance(job, Mapping):
        raise BrokerError("job must be a mapping")
    if "function" not in job:
        raise BrokerError("job.function is required")
    function = _parse_address(job["function"], "job.function")

    allowed_raw = job.get("allowed_functions")
    if not isinstance(allowed_raw, (list, tuple)) or not allowed_raw:
        raise BrokerError("job.allowed_functions must be a non empty list")
    allowed = [
        _parse_address(entry, "job.allowed_functions entry") for entry in allowed_raw
    ]

    seed = job.get("seed_fixture")
    if not isinstance(seed, Mapping):
        raise BrokerError("job.seed_fixture must be a mapping")
    seed_path = seed.get("path")
    seed_sha = seed.get("sha256")
    if not isinstance(seed_path, str) or not seed_path:
        raise BrokerError("job.seed_fixture.path must be an absolute path")
    if not os.path.isabs(seed_path):
        raise BrokerError("job.seed_fixture.path must be absolute")
    if not isinstance(seed_sha, str) or len(seed_sha) != 64:
        raise BrokerError("job.seed_fixture.sha256 must be a sha256 hex digest")
    try:
        int(seed_sha, 16)
    except ValueError:
        raise BrokerError("job.seed_fixture.sha256 must be a sha256 hex digest") from None

    directory_raw = job.get("directory")
    if not isinstance(directory_raw, (str, Path)) or not str(directory_raw):
        raise BrokerError("job.directory must be an absolute path")
    directory = Path(str(directory_raw))
    if not directory.is_absolute():
        raise BrokerError("job.directory must be absolute")
    directory = directory.resolve()

    return {
        "function": function,
        "allowed": allowed,
        "seed_path": Path(seed_path),
        "seed_sha256": seed_sha.lower(),
        "directory": directory,
    }


def _resolve_output(output: Any, directory: Path) -> Path:
    if output is None:
        raise BrokerError("output is required")
    path = Path(str(output))
    if not path.is_absolute():
        path = directory / path
    resolved = path.resolve()
    if not _is_within(resolved, directory):
        raise BrokerError("output must be located under job.directory")
    return resolved


def _verified_seed(job: Mapping[str, Any]) -> dict[str, Any]:
    path = job["seed_path"]
    if not path.is_file():
        raise BrokerError(f"seed fixture {path} is missing")
    actual = _sha256_file(path)
    if actual != job["seed_sha256"]:
        raise BrokerError(f"seed fixture {path} hash mismatch")
    data = _read_json_artifact(path)
    if not isinstance(data, Mapping):
        raise BrokerError("the seed fixture must be a JSON object")
    return dict(data)


def _read_json_artifact(path: Path) -> Any:
    try:
        raw = path.read_bytes()
    except OSError as exc:
        raise BrokerError(f"artifact {path.name} cannot be read: {exc}") from exc
    if path.name.endswith(".gz") or raw[:2] == b"\x1f\x8b":
        try:
            raw = gzip.decompress(raw)
        except OSError as exc:
            raise BrokerError(f"artifact {path.name} is not valid gzip: {exc}") from exc
    try:
        text = raw.decode("utf-8")
    except UnicodeDecodeError as exc:
        raise BrokerError(f"artifact {path.name} is not UTF-8: {exc}") from exc
    return _strict_loads(text)


# prior artifact resolution


def _resolve_prior_artifact(
    prior: Any, artifact_id: Any, job: Mapping[str, Any]
) -> dict[str, Any]:
    if not isinstance(artifact_id, str) or not artifact_id:
        raise BrokerError("the artifact id must be a non empty string")
    if not isinstance(prior, Mapping):
        raise BrokerError("prior must be a mapping of artifact ids to records")
    record = prior.get(artifact_id)
    if record is None:
        raise BrokerError(f"artifact {artifact_id!r} is not present in prior")
    if not isinstance(record, Mapping):
        raise BrokerError(f"prior record for {artifact_id!r} must be a mapping")

    descriptor: Any = None
    artifacts = record.get("artifacts")
    if isinstance(artifacts, Mapping):
        if artifact_id in artifacts:
            descriptor = artifacts[artifact_id]
        else:
            matches = [
                value
                for value in artifacts.values()
                if isinstance(value, Mapping) and value.get("kind") == artifact_id
            ]
            if len(matches) == 1:
                descriptor = matches[0]
    if descriptor is None and isinstance(record.get("path"), str):
        descriptor = record
    if not isinstance(descriptor, Mapping):
        raise BrokerError(f"artifact {artifact_id!r} has no retained descriptor")

    path_raw = descriptor.get("path")
    sha_raw = descriptor.get("sha256")
    if not isinstance(path_raw, str) or not path_raw:
        raise BrokerError(f"artifact {artifact_id!r} has no retained path")
    if not isinstance(sha_raw, str) or len(sha_raw) != 64:
        raise BrokerError(f"artifact {artifact_id!r} has no retained hash")
    path = Path(path_raw)
    if not path.is_absolute():
        raise BrokerError(f"artifact {artifact_id!r} path must be absolute")

    root = Path(job["directory"]).resolve()
    # A prior record may describe an artifact, never expand the trusted root.
    for key in ("root", "directory", "job_directory"):
        value = record.get(key)
        if value is None:
            continue
        if not isinstance(value, (str, Path)) or not str(value):
            raise BrokerError(f"artifact {artifact_id!r} {key} is malformed")
        if not _is_within(Path(str(value)), root):
            raise BrokerError(
                f"artifact {artifact_id!r} {key} escapes the job root"
            )

    path = path.resolve()
    if not _is_within(path, root):
        raise BrokerError(f"artifact {artifact_id!r} path escapes the job root")

    if not path.is_file():
        raise BrokerError(f"artifact {artifact_id!r} is missing")
    actual = _sha256_file(path)
    if actual != sha_raw.lower():
        raise BrokerError(f"artifact {artifact_id!r} hash mismatch")

    return {
        "id": artifact_id,
        "path": path,
        "sha256": actual,
        "kind": descriptor.get("kind"),
    }


# artifacts / receipts


def _artifact_kind(relative: str) -> str:
    name = relative.rsplit("/", 1)[-1]
    if name in _KIND_BY_NAME:
        return _KIND_BY_NAME[name]
    if relative.startswith(f"{_ARTIFACT_DIR}/build/"):
        return "build"
    suffix = Path(name).suffix.lower().lstrip(".")
    return suffix or "file"


def _collect_artifacts(out: Path) -> dict[str, Any]:
    artifacts: dict[str, Any] = {}
    root = out.resolve()
    for path in sorted(out.rglob("*")):
        if not path.is_file():
            continue
        if path.name in (_RECEIPT_NAME, _RESPONSE_NAME):
            continue
        resolved = path.resolve()
        if not _is_within(resolved, root):
            raise BrokerError(f"retained artifact {path} escapes the output root")
        relative = path.relative_to(out).as_posix()
        artifacts[relative] = {
            "path": str(resolved),
            "sha256": _sha256_file(resolved),
            "kind": _artifact_kind(relative),
            "bytes": resolved.stat().st_size,
        }
    return artifacts


def _write_bytes(path: Path, data: bytes, *, overwrite: bool = False) -> str:
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists() and not overwrite:
        raise BrokerError(f"refusing to overwrite the existing output file {path}")
    path.write_bytes(data)
    return _sha256_bytes(data)


def _load_receipt(path: Path) -> dict[str, Any]:
    try:
        text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeDecodeError) as exc:
        raise BrokerError(f"the existing receipt cannot be read: {exc}") from exc
    data = _strict_loads(text)
    if not isinstance(data, Mapping):
        raise BrokerError("the existing receipt is corrupt")
    return dict(data)


def _replay_or_refuse(
    receipt: Mapping[str, Any],
    receipt_file: Path,
    response_file: Path,
    action_fingerprint: str,
    job_fingerprint: str,
    root: Path,
) -> dict[str, Any]:
    state = receipt.get("state")
    if state == "running":
        raise BrokerError(
            "the existing output is in the running state: refusing to retry"
        )
    if state == "failed":
        raise BrokerError(
            "the existing output failed previously: refusing to retry silently"
        )
    if state != "finished":
        raise BrokerError(f"the existing receipt has the unknown state {state!r}")

    if receipt.get("action_fingerprint") != action_fingerprint:
        raise BrokerError("the existing output was produced by a different action")
    if receipt.get("job_fingerprint") != job_fingerprint:
        raise BrokerError("the existing output was produced by a different job")

    artifacts = receipt.get("artifacts")
    if not isinstance(artifacts, Mapping):
        raise BrokerError("the finished receipt retains no artifact hashes")
    for name, descriptor in artifacts.items():
        if not isinstance(descriptor, Mapping):
            raise BrokerError(f"the retained artifact {name!r} has no descriptor")
        path = Path(str(descriptor.get("path", "")))
        if not path.is_absolute():
            raise BrokerError(f"the retained artifact {name!r} path is not absolute")
        path = path.resolve()
        if not _is_within(path, root):
            raise BrokerError(f"the retained artifact {name!r} escapes the job root")
        if not path.is_file():
            raise BrokerError(f"the retained artifact {name!r} is missing")
        if _sha256_file(path) != descriptor.get("sha256"):
            raise BrokerError(f"the retained artifact {name!r} hash mismatch")

    if not _is_within(response_file, root):
        raise BrokerError("the retained response.json escapes the job root")
    if not response_file.is_file():
        raise BrokerError("the finished receipt has no response.json")
    payload = response_file.read_bytes()
    if _sha256_bytes(payload) != receipt.get("response_sha256"):
        raise BrokerError("the retained response.json hash mismatch")
    return _strict_loads(payload.decode("utf-8"))


# action runners


class _Runner:
    """Per dispatch state for one action."""

    def __init__(
        self,
        job: Mapping[str, Any],
        backend: Any,
        prior: Any,
        candidate: Any,
        out: Path,
    ) -> None:
        self.job = job
        self.backend = backend
        self.prior = prior
        self.candidate = candidate
        self.out = out
        self.artifacts_dir = out / _ARTIFACT_DIR

    # -- helpers ---------------------------------------------------------

    def prepare(self, kind: str, action: Mapping[str, Any]) -> Callable[[], Any]:
        handler = getattr(self, f"_prepare_{kind}", None)
        if handler is None:
            raise BrokerError(f"unsupported action kind {kind!r}")
        return handler(action)

    def _retain(self, summary: Any, name: str) -> Any:
        """Return a bounded summary, retaining the complete copy on disk."""

        safe = _sanitize(summary)
        payload = _canonical_bytes(safe)
        if len(payload) <= _MAX_SUMMARY_BYTES:
            return safe
        self.artifacts_dir.mkdir(parents=True, exist_ok=True)
        _write_bytes(self.artifacts_dir / name, payload)
        return {
            "retained": f"{_ARTIFACT_DIR}/{name}",
            "sha256": _sha256_bytes(payload),
            "bytes": len(payload),
            "truncated": True,
        }

    # -- evidence --------------------------------------------------------

    def _prepare_evidence(self, action: Mapping[str, Any]) -> Callable[[], Any]:
        function = (
            _parse_address(action["function"], "action.function")
            if action.get("function") is not None
            else self.job["function"]
        )
        if function not in self.job["allowed"]:
            raise BrokerError(
                f"function 0x{function:x} is not in job.allowed_functions"
            )
        start = (
            _parse_address(action["start"], "action.start")
            if action.get("start") is not None
            else None
        )
        limit = _limit(action, "action.limit")

        def execute() -> Any:
            payload = self.backend.evidence(function, start, limit)
            rows, total, body, callees, target_sha = _normalize_evidence(payload)
            scoped = _scope_rows(rows, start, limit)
            cursor = _cursor_index(rows, start)
            self.artifacts_dir.mkdir(parents=True, exist_ok=True)
            _write_bytes(
                self.artifacts_dir / "instructions.json",
                _canonical_bytes(
                    _sanitize({"function": f"0x{function:x}", "instructions": rows})
                ),
            )
            return {
                "function": f"0x{function:x}",
                "instructions": _sanitize(scoped),
                "body": _sanitize(body),
                "body_bytes": _sanitize(body),
                "callees": _sanitize(callees),
                "total": total,
                "remaining": max(0, total - cursor - len(scoped)),
                "limit": limit,
                "start": f"0x{start:x}" if start is not None else None,
                "provenance": {
                    "target": "original",
                    "source": "pinned-original-image",
                    "function": f"0x{function:x}",
                    "scope": "job.allowed_functions",
                    "target_sha256": target_sha
                    or getattr(self.backend, "target_sha256", None),
                },
            }

        return execute

    # -- memory ----------------------------------------------------------

    def _prepare_memory(self, action: Mapping[str, Any]) -> Callable[[], Any]:
        if action.get("address") is None:
            raise BrokerError("action.address is required")
        address = _parse_address(action["address"], "action.address")
        size = action.get("size")
        if isinstance(size, bool) or not isinstance(size, int):
            raise BrokerError("action.size must be an integer")
        if size < 1 or size > _MAX_MEMORY:
            raise BrokerError(f"action.size must be within 1..{_MAX_MEMORY}")

        def execute() -> Any:
            data = self.backend.memory(address, size)
            if not isinstance(data, (bytes, bytearray, memoryview)):
                raise BrokerError("backend.memory must return bytes")
            raw = bytes(data)
            if len(raw) != size:
                raise BrokerError(
                    f"backend.memory returned {len(raw)} bytes for a {size} byte request"
                )
            return {
                "address": f"0x{address:x}",
                "size": size,
                "data": raw.hex(),
                "provenance": {"target": "original", "source": "pinned-original-image"},
            }

        return execute

    # -- probe -----------------------------------------------------------

    def _prepare_probe(self, action: Mapping[str, Any]) -> Callable[[], Any]:
        provider = _provider_probe()
        request = action.get("request")
        if not isinstance(request, Mapping):
            raise BrokerError("action.request must be a mapping")
        validated = provider.validate_request(
            request, allowed_functions=list(self.job["allowed"])
        )

        base_id = action.get("base")
        if base_id is None:
            base = _verified_seed(self.job)
        else:
            base = self._load_prior_fixture(base_id)

        image = getattr(self.backend, "image", None)
        if image is None:
            raise BrokerError("backend.image is required for a probe")

        def _validate(fixture: dict[str, Any]) -> None:
            provider.validate_global_writes(fixture, image)
            return None

        fixture = provider.apply_patch(base, validated.get("patch") or {}, validate=_validate)
        provider.validate_global_writes(fixture, image)

        function = int(validated["function"])
        budget = int(validated["budget"])
        shapes = dict(validated["shapes"])

        def execute() -> Any:
            self.artifacts_dir.mkdir(parents=True, exist_ok=True)
            summary = self.backend.probe(
                function, fixture, budget, shapes, self.artifacts_dir
            )
            if not isinstance(summary, Mapping):
                raise BrokerError("backend.probe must return a mapping")
            fixture_path = self.artifacts_dir / "fixture.json"
            if not fixture_path.exists():
                _write_bytes(fixture_path, _canonical_bytes(_sanitize(fixture)))
            return {
                "name": validated["name"],
                "function": f"0x{function:x}",
                "budget": budget,
                "shapes": _sanitize(shapes),
                "base": base_id if base_id is not None else "seed_fixture",
                "summary": self._retain(summary, "summary.json"),
                "provenance": {
                    "target": "original",
                    "source": "pinned-original-image",
                    "base": base_id if base_id is not None else "seed_fixture",
                },
            }

        return execute

    def _load_prior_fixture(self, base_id: Any) -> dict[str, Any]:
        resolved = _resolve_prior_artifact(self.prior, base_id, self.job)
        kind = resolved.get("kind")
        name = resolved["path"].name
        if kind not in (None, "fixture") and name != "fixture.json":
            raise BrokerError("the probe base must be a retained fixture artifact")
        data = _read_json_artifact(resolved["path"])
        if not isinstance(data, Mapping):
            raise BrokerError("the retained fixture artifact must be a JSON object")
        return dict(data)

    # -- inspect ---------------------------------------------------------

    def _prepare_inspect(self, action: Mapping[str, Any]) -> Callable[[], Any]:
        artifact_id = action.get("artifact")
        section = action.get("section")
        if not isinstance(section, str) or section not in _SECTION_PATHS:
            raise BrokerError(
                "action.section must be one of: " + ", ".join(sorted(_SECTION_PATHS))
            )
        start = action.get("start", 0)
        if isinstance(start, bool) or not isinstance(start, int) or start < 0:
            raise BrokerError("action.start must be an integer >= 0")
        limit = _limit(action, "action.limit")

        resolved = _resolve_prior_artifact(self.prior, artifact_id, self.job)
        data = _read_json_artifact(resolved["path"])
        items = _extract_section(data, section)
        total = len(items)
        window = items[start : start + limit]

        def execute() -> Any:
            return {
                "artifact": artifact_id,
                "section": section,
                "path": str(resolved["path"]),
                "sha256": resolved["sha256"],
                "start": start,
                "limit": limit,
                "items": _sanitize(window),
                "total": total,
                "remaining": max(0, total - len(window)),
            }

        return execute

    # -- build -----------------------------------------------------------

    def _prepare_build(self, action: Mapping[str, Any]) -> Callable[[], Any]:
        candidate = self.candidate
        if candidate is None:
            raise BrokerError("build requires a trusted candidate argument")
        if isinstance(candidate, Mapping):
            candidate = candidate.get("path")
        if isinstance(candidate, Path):
            trusted: Any = candidate
        elif isinstance(candidate, str) and candidate.strip():
            trusted = candidate
        else:
            raise BrokerError("the trusted candidate argument must be a source path")
        source = Path(os.path.normpath(str(trusted)))
        if not source.is_absolute():
            source = Path.cwd() / source

        def execute() -> Any:
            build_dir = self.artifacts_dir / "build"
            build_dir.mkdir(parents=True, exist_ok=True)
            summary = self.backend.build(trusted, build_dir)
            if not isinstance(summary, Mapping):
                raise BrokerError("backend.build must return a mapping")
            return {
                "candidate": str(source),
                "build": self._retain(summary, "summary.json"),
                "provenance": {"target": "candidate", "options": ["-O0", "-O2"]},
            }

        return execute

    # -- screen ----------------------------------------------------------

    def _prepare_screen(self, action: Mapping[str, Any]) -> Callable[[], Any]:
        function = self.job["function"]

        def execute() -> Any:
            summary = self.backend.screen(function)
            if not isinstance(summary, Mapping):
                raise BrokerError("backend.screen must return a mapping")
            return {
                "function": f"0x{function:x}",
                "screen": self._retain(summary, "summary.json"),
                "provenance": {"target": "original", "source": "full-original-screen"},
            }

        return execute


def _normalize_evidence(
    payload: Any,
) -> tuple[list[dict[str, Any]], int, Any, list[Any], Optional[str]]:
    if isinstance(payload, Mapping):
        rows = payload.get("insns")
        if rows is None:
            rows = payload.get("instructions")
        if rows is None:
            rows = []
        total = payload.get("total", len(rows))
        body = payload.get("body_bytes", payload.get("body"))
        callees = payload.get("callees", [])
        target_sha = payload.get("target_sha256")
    elif isinstance(payload, (list, tuple)):
        rows = list(payload)
        total = len(rows)
        body = None
        callees = []
        target_sha = None
    else:
        raise BrokerError("backend.evidence returned an unsupported payload")
    if not isinstance(rows, (list, tuple)):
        raise BrokerError("backend.evidence must return instruction rows")
    normalized: list[dict[str, Any]] = []
    for row in rows:
        if not isinstance(row, Mapping):
            raise BrokerError("every evidence row must be a mapping")
        normalized.append(dict(row))
    try:
        count = int(total)
    except (TypeError, ValueError):
        count = len(normalized)
    if count < len(normalized):
        count = len(normalized)
    if not isinstance(callees, (list, tuple)):
        callees = [callees] if callees else []
    if target_sha is not None and not isinstance(target_sha, str):
        raise BrokerError("backend.evidence target_sha256 must be a string")
    return normalized, count, body, list(callees), target_sha


def _row_address(row: Mapping[str, Any]) -> Optional[int]:
    address = row.get("va", row.get("address"))
    if isinstance(address, bool):
        return None
    if isinstance(address, str):
        try:
            address = int(address, 0)
        except ValueError:
            return None
    return address if isinstance(address, int) else None


def _cursor_index(rows: list[dict[str, Any]], start: Optional[int]) -> int:
    if start is None:
        return 0
    for index, row in enumerate(rows):
        address = _row_address(row)
        if address is not None and address >= start:
            return index
    return len(rows)


def _scope_rows(
    rows: list[dict[str, Any]], start: Optional[int], limit: int
) -> list[dict[str, Any]]:
    scoped: list[dict[str, Any]] = []
    for row in rows:
        if start is not None:
            address = _row_address(row)
            if address is not None and address < start:
                continue
        scoped.append(row)
        if len(scoped) >= limit:
            break
    return scoped


def _dig(data: Any, dotted: str) -> Any:
    current = data
    for part in dotted.split("."):
        if isinstance(current, Mapping) and part in current:
            current = current[part]
        else:
            return None
    return current


def _extract_section(data: Any, section: str) -> list[Any]:
    for dotted in _SECTION_PATHS[section]:
        value = _dig(data, dotted)
        if value is not None:
            return _as_items(value)
    if isinstance(data, list) and section == "instructions":
        return list(data)
    raise BrokerError(f"the artifact does not contain the {section!r} section")


def _as_items(value: Any) -> list[Any]:
    if isinstance(value, list):
        return list(value)
    if isinstance(value, tuple):
        return list(value)
    if isinstance(value, Mapping):
        return [
            {"key": key, "value": item}
            for key, item in sorted(value.items(), key=lambda pair: str(pair[0]))
        ]
    return [value]


# public entry point


def dispatch(
    job: dict,
    action: dict,
    output: Path,
    *,
    prior: Optional[Mapping[str, Any]] = None,
    backend: Any = None,
    candidate: Any = None,
) -> dict:
    """Execute one scoped action and return the bounded JSON response.

    ``output`` must be located under ``job.directory``.  A finished output is
    reused only when the action and job fingerprints and every retained
    artifact hash recheck; a running or corrupt output refuses execution.
    """

    job_info = _validate_job(job)
    if not isinstance(action, Mapping):
        raise BrokerError("action must be a mapping")
    kind = action.get("kind")
    if not isinstance(kind, str) or kind not in _ACTION_KEYS:
        raise BrokerError(f"unsupported action kind {kind!r}")
    unknown = sorted(str(key) for key in action if key not in _ACTION_KEYS[kind])
    if unknown:
        raise BrokerError(f"unknown {kind} action keys: {', '.join(unknown)}")

    out = _resolve_output(output, job_info["directory"])
    root = job_info["directory"]
    if out.exists() and not out.is_dir():
        raise BrokerError("the output location is not a directory")

    # Reject symlink escapes for the output and every retained child before
    # any mkdir or write can follow them.
    for guarded in (
        out,
        out / _ARTIFACT_DIR,
        out / _RECEIPT_NAME,
        out / _RESPONSE_NAME,
    ):
        if guarded.is_symlink() and not _is_within(guarded, root):
            raise BrokerError(f"output path {guarded} escapes job.directory")
        if guarded.exists() and not _is_within(guarded, root):
            raise BrokerError(f"output path {guarded} escapes job.directory")

    action_fingerprint = _sha256_bytes(_canonical_bytes(_sanitize(action)))
    job_fingerprint = _sha256_bytes(_canonical_bytes(_sanitize(job)))

    receipt_file = out / _RECEIPT_NAME
    response_file = out / _RESPONSE_NAME

    if receipt_file.exists():
        receipt = _load_receipt(receipt_file)
        return _replay_or_refuse(
            receipt,
            receipt_file,
            response_file,
            action_fingerprint,
            job_fingerprint,
            root,
        )
    if response_file.exists():
        raise BrokerError(
            "an existing response.json has no receipt: refusing to execute"
        )

    if backend is None:
        backend = LocalBackend(function=job_info["function"])
    runner = _Runner(job_info, backend, prior, candidate, out)
    execute = runner.prepare(kind, action)

    out.mkdir(parents=True, exist_ok=True)
    artifacts_dir = out / _ARTIFACT_DIR
    if artifacts_dir.exists():
        for path in artifacts_dir.rglob("*"):
            if path.is_file():
                raise BrokerError(
                    "existing artifacts without a receipt: refusing to execute"
                )

    receipt = {
        "state": "running",
        "kind": kind,
        "action_fingerprint": action_fingerprint,
        "job_fingerprint": job_fingerprint,
        "artifacts": {},
    }
    _write_bytes(receipt_file, _canonical_bytes(receipt))

    try:
        result = execute()
    except BaseException as exc:
        failed = dict(receipt)
        failed["state"] = "failed"
        failed["error"] = {"type": type(exc).__name__, "message": str(exc)[:2000]}
        try:
            failed["artifacts"] = _collect_artifacts(out)
        except Exception:
            failed["artifacts"] = {}
        try:
            _write_bytes(receipt_file, _canonical_bytes(failed), overwrite=True)
        except Exception:
            pass
        raise

    artifacts = _collect_artifacts(out)
    response = {
        "kind": kind,
        "ok": True,
        "action_fingerprint": action_fingerprint,
        "job_fingerprint": job_fingerprint,
        "result": _sanitize(result),
        "artifacts": artifacts,
    }
    payload = _canonical_bytes(response)
    _write_bytes(response_file, payload)
    finished = dict(receipt)
    finished["state"] = "finished"
    finished["artifacts"] = artifacts
    finished["response_sha256"] = _sha256_bytes(payload)
    _write_bytes(receipt_file, _canonical_bytes(finished), overwrite=True)
    return _strict_loads(payload.decode("utf-8"))


# default backend


class LocalBackend:
    """Backend that runs the existing original-only tooling."""

    def __init__(self, function: Optional[int] = None, target_sha256: Optional[str] = None):
        self.function = function
        self.target_sha256 = target_sha256
        self._modules: dict[str, Any] = {}

    @property
    def image(self):
        self._verify_target()
        return self._module('image').primary()

    # -- module plumbing -------------------------------------------------

    def _module(self, name: str) -> Any:
        module = self._modules.get(name)
        if module is None:
            try:
                module = importlib.import_module(f"cmre.{name}")
            except Exception as exc:
                raise BrokerError(f"cmre.{name} is unavailable: {exc}") from exc
            self._modules[name] = module
        return module

    def _harness(self, name: str) -> Any:
        module = self._modules.get(name)
        if module is None:
            try:
                module = importlib.import_module(name)
            except Exception as exc:
                raise BrokerError(f"{name} is unavailable: {exc}") from exc
            self._modules[name] = module
        return module

    def _verify_target(self) -> str:
        env = self._module("env")
        expected = self.target_sha256
        if expected is None:
            expected = getattr(env, "TARGET_SHA256", None)
        if not isinstance(expected, str) or len(expected) != 64:
            raise BrokerError("env.TARGET_SHA256 is unavailable")
        try:
            data = bytes(env.target_bytes())
        except Exception as exc:
            raise BrokerError(f"env.target_bytes failed: {exc}") from exc
        actual = _sha256_bytes(data)
        if actual != expected.lower():
            raise BrokerError("env.TARGET hash mismatch")
        return actual

    # -- actions ---------------------------------------------------------

    def evidence(self, function: int, start: Optional[int], limit: int) -> dict[str, Any]:
        target_sha = self._verify_target()
        lift = self._module("lift")
        discovered = lift.discover(function)
        insns = getattr(discovered, "insns", None) or {}
        rows: list[dict[str, Any]] = []
        for address in sorted(insns):
            instruction = insns[address]
            rows.append(
                {
                    "va": f"0x{int(address):x}",
                    "raw": _sanitize(getattr(instruction, "raw", None)),
                    "mnemonic": _sanitize(getattr(instruction, "mnemonic", None)),
                    "op_str": _sanitize(getattr(instruction, "op_str", None)),
                    "kind": _sanitize(getattr(instruction, "kind", None)),
                }
            )
        inventory = self._module("inventory")
        entry = inventory.load().functions[function]
        body = getattr(entry, "body_bytes", None)
        callees = getattr(discovered, "callees", None) or []
        # The runner owns scoping: this backend returns the complete decode.
        return {
            "insns": rows,
            "total": len(rows),
            "body_bytes": body,
            "callees": [
                f"0x{int(value):x}" if isinstance(value, int) else _sanitize(value)
                for value in callees
            ],
            "target_sha256": target_sha,
        }

    def memory(self, address: int, size: int) -> bytes:
        self._verify_target()
        image = self._module("image")
        return bytes(image.primary().read(address, size))

    def screen(self, function: int) -> Mapping[str, Any]:
        self._verify_target()
        screen = self._module("screen")
        return screen.run(function)

    def build(self, candidate: Any, output: Any) -> Mapping[str, Any]:
        target_sha = self._verify_target()
        if self.function is None:
            raise BrokerError("build requires a configured function")
        build_module = self._module("build")
        lift = self._module("lift")
        env = self._module("env")
        outdir = Path(str(output))
        outdir.mkdir(parents=True, exist_ok=True)
        source = Path(str(candidate))
        if not source.is_file():
            raise BrokerError(f"candidate source {source} is missing")
        source_hash = _sha256_bytes(source.read_bytes())
        symbol = lift.symbol(self.function)
        records: list[dict[str, Any]] = []
        build_commands: list[list[str]] = []
        for option in ("-O0", "-O2"):
            target_dir = outdir / option.lstrip("-")
            target_dir.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, target_dir / source.name)
            try:
                built = build_module.build(source, symbol, target_dir, option)
            except build_module.BuildError as error:
                stderr = error.stderr
                _write_bytes(target_dir / 'error.json', _canonical_bytes({
                    'source_sha256': source_hash, 'option': option,
                    'error': str(error), 'stderr': stderr}))
                raise BrokerError(f'{error}\n{stderr[-4000:]}') from error
            log = getattr(built, "log", None)
            if not isinstance(log, (list, tuple)):
                raise BrokerError("build.build must expose its command log")
            commands = [[str(part) for part in command] for command in log]
            build_commands.extend(commands)
            _write_bytes(
                target_dir / "commands.json",
                _canonical_bytes(_sanitize(commands)),
            )
            records.append(
                {"option": option, "directory": str(target_dir), "commands": commands}
            )
        disassembly_commands: list[list[str]] = []
        for exe in sorted(outdir.rglob("*.exe")):
            command = [str(env.tool("objdump")), "-d", "-Mintel", str(exe)]
            disassembly_commands.append(command)
            completed = env.run_tool(command)
            if getattr(completed, "returncode", 0):
                raise BrokerError(
                    f"objdump failed for {exe}: {getattr(completed, 'stderr', '')}"
                )
            text = completed.stdout
            if not isinstance(text, str):
                raise BrokerError("env.run_tool must expose text stdout")
            # Keep both optimization disassemblies: one file beside each exe.
            (exe.parent / f"{exe.name}.disasm.txt").write_text(
                text, encoding="utf-8"
            )
        _write_bytes(
            outdir / "commands.json",
            _canonical_bytes(_sanitize(build_commands + disassembly_commands)),
        )
        files = sorted(str(path) for path in outdir.rglob("*") if path.is_file())
        return {
            "candidate": str(source),
            "symbol": str(symbol),
            "builds": records,
            "commands": build_commands + disassembly_commands,
            "source_sha256": source_hash,
            "target_sha256": target_sha,
            "files": files,
        }

    def probe(
        self,
        function: int,
        fixture: Mapping[str, Any],
        budget: int,
        shapes: Mapping[str, Any],
        output: Any,
    ) -> dict[str, Any]:
        target_sha = self._verify_target()
        outdir = Path(str(output))
        outdir.mkdir(parents=True, exist_ok=True)
        difftest = self._module("difftest")
        emu = self._module("emu")
        image = self._module("image")
        lift = self._module("lift")
        recover = self._module("recover")
        diagnostics = self._harness("scripts.cmre_paired_diagnostics")
        img = image.primary()

        stack = list(fixture.get("stack") or [])
        # Match the existing core's canonical C-input arena capacity. The
        # provider cannot choose this through a patch or change the VM policy.
        options = difftest.Options.for_function(function, tier='c')
        arena_size = emu.arena_capacity(options.hints.get('arena_size', emu.ARENA_SIZE))
        hints = {"fixtures": [fixture], "arg_count": len(stack), "arena_size": arena_size}
        # Native fixture/policy validation runs before any VM execution.
        difftest.validate_hints(hints, worker=True)
        templates = [t + bytes(arena_size-len(t)) for t in difftest.arena_templates()]
        case = difftest._fixture_case(fixture, templates, len(stack))
        lifted = lift.discover(function)
        machine = emu.Machine(img, watch=(function, lifted.region_end), arena_size=arena_size)
        observer = diagnostics.Observer(
            machine, {int(key): int(key) for key in shapes}, shapes
        )
        import unicorn
        fp_pcs = {pc for pc, insn in lifted.insns.items()
                  if insn.mnemonic.startswith('f')} | {int(key) for key in shapes}
        helper_states = []

        def observe_fp(uc, address, _size, _data):
            if observer.enabled and address in fp_pcs:
                helper_states.append({'pc': address,
                                      'instruction_index': len(observer.instructions) - 1,
                                      'event_index': len(machine.events),
                                      **_fp_snapshot(uc)})

        # Read-only diagnostics: entry and original x87 instruction states.
        machine.uc.hook_add(unicorn.UC_HOOK_CODE, observe_fp)

        identity = recover.execution_identity()
        fixture_sha = _sha256_bytes(_canonical_bytes(fixture))
        shapes_sha = _sha256_bytes(_canonical_bytes(_sanitize(dict(shapes))))
        observer_sha = None
        observer_file = getattr(diagnostics, "__file__", None)
        if observer_file:
            observer_sha = _sha256_file(Path(observer_file))
        metadata = {
            "execution_identity": identity,
            "target_sha256": target_sha,
            "source_sha256": target_sha,
            "observer_sha256": observer_sha,
            "fixture_sha256": fixture_sha,
            "shapes_sha256": shapes_sha,
            "arg_count": len(stack),
            "arena_size": arena_size,
            "input_hint_tier": "c",
            "budget": budget,
            "function": f"0x{function:x}",
            "shapes": dict(shapes),
            "own_instruction_count": len(lifted.insns),
        }
        # The input witness and pins are retained before the run, so a fault
        # or budget stop still keeps them on disk.
        diagnostics.save(
            outdir / "input.json.gz", {"fixture": fixture, "case": case}
        )
        _write_bytes(outdir / "fixture.json", _canonical_bytes(fixture))
        _write_bytes(
            outdir / "execution.json",
            _canonical_bytes(_sanitize(metadata)),
        )

        try:
            result, frame_pointer, raw = observer.run(function, case, budget)
        except BaseException as exc:
            failure = {
                "error": {"type": type(exc).__name__, "message": str(exc)[:2000]},
                "instructions": getattr(observer, "instructions", []),
                "arrivals": getattr(observer, "arrivals", []),
                "returns": getattr(observer, "returns", []),
                "registered_helper_states": helper_states,
            }
            diagnostics.save(outdir / "raw.json.gz", failure)
            raise
        raw['registered_helper_states'] = helper_states
        raw['stop_fp'] = _fp_snapshot(machine.uc)
        diagnostics.save(outdir / "raw.json.gz", raw)

        registers = {}
        for name, value in zip(emu.REG_NAMES, getattr(result, "regs", None) or []):
            registers[name] = f"0x{int(value) & 0xFFFFFFFF:x}"
        covered = getattr(result, "covered", None)
        if not isinstance(covered, (set, frozenset)):
            covered = set(getattr(result, "covered_pcs", None) or [])
        covered_pcs = sorted(
            {
                int(pc)
                for pc in covered
                if isinstance(pc, int) and not isinstance(pc, bool)
            }
            & set(lifted.insns)
        )
        coverage = len(covered_pcs) / len(lifted.insns) if lifted.insns else 0.0
        events = getattr(result, "events", None) or []
        import_count = sum(
            1
            for event in events
            if isinstance(event, (tuple, list)) and event and event[0] == "c"
        )
        if isinstance(frame_pointer, (tuple, list)):
            return_fp: Any = [int(value) & 0xFFFFFFFF for value in frame_pointer]
        elif isinstance(frame_pointer, int) and not isinstance(frame_pointer, bool):
            return_fp = int(frame_pointer) & 0xFFFFFFFF
        else:
            return_fp = _sanitize(frame_pointer)
        raw_map = raw if isinstance(raw, Mapping) else {}
        return {
            "outcome": _sanitize(getattr(result, "outcome", None)),
            "detail": _sanitize(getattr(result, "detail", None)),
            "regs": registers,
            "covered_pcs": [f"0x{pc:x}" for pc in covered_pcs],
            "coverage": coverage,
            "import_count": import_count,
            "return_fp": return_fp,
            "arrivals": _sanitize(raw_map.get("arrivals", [])),
            "returns": _sanitize(raw_map.get("returns", [])),
            "registered_helper_states": _sanitize(helper_states[:120]),
            "registered_helper_state_count": len(helper_states),
            "stop_fp": raw['stop_fp'],
            "execution_identity": _sanitize(identity),
            "target_sha256": target_sha,
            "limits": {
                "original_callees": "executed by the existing VM",
                "import_scaffolding": "modelled import stubs",
            },
        }


def _fp_snapshot(uc):
    from unicorn import x86_const as registers
    from . import difftest
    cw, top, st0 = difftest.semantic_fp(uc, 'float')
    return {'control_word': cw, 'top': top, 'st0_raw80': f'{st0:020x}',
            'status_word': uc.reg_read(registers.UC_X86_REG_FPSW),
            'tag_word': uc.reg_read(registers.UC_X86_REG_FPTAG)}
