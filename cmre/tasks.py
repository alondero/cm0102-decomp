"""Bounded C translation packets, exclusive leases and coordinator publication.

Workers write only a task's candidate source. Submission runs the fixed C gate;
only publication invokes acceptance. State and failure evidence survive restarts.
"""
from __future__ import annotations

from contextlib import nullcontext
import json
import math
import re
import secrets
import time
from pathlib import Path

from . import coordination, env, image, inventory, ledger, lift, recover

SCHEMA = "cmre-task/1"
MAX_ATTEMPTS = 5
MAX_BODY_BYTES = 600
TASK_ID = re.compile(r"c-[0-9a-f]{8}-[0-9a-f]{16}\Z")
TOKEN_METRICS = ("input_tokens", "output_tokens", "coordinator_tokens", "reviewer_tokens")
NUMERIC_METRICS = (*TOKEN_METRICS, "worker_seconds")
METRICS = (*NUMERIC_METRICS, "model")


def task_dir(task_id: str) -> Path:
    if not isinstance(task_id, str) or not TASK_ID.fullmatch(task_id):
        raise ValueError("Invalid task ID")
    path = env.ANALYSIS / "cmre/tasks" / task_id
    if path.resolve() != env.ANALYSIS.resolve() / "cmre/tasks" / task_id:
        raise ValueError("Task directory may not redirect through a symbolic link")
    return path


def _candidate(task_id: str, va: int) -> Path:
    task_dir(task_id)
    path = env.C_DIR / "tasks" / task_id / f"fn_{va:08x}.c"
    if path.resolve() != env.C_DIR.resolve() / "tasks" / task_id / f"fn_{va:08x}.c":
        raise ValueError("Candidate path may not redirect through a symbolic link")
    return path


def _hash(value) -> str:
    return env.digest_bytes(json.dumps(value, sort_keys=True, separators=(",", ":")).encode())


def _optional_hash(path: Path):
    return env.digest(path) if path.exists() else None


def _contract_path(va: int) -> Path:
    path = env.C_DIR / "contracts" / f"fn_{va:08x}.json"
    if path.resolve() != env.C_DIR.resolve() / "contracts" / f"fn_{va:08x}.json":
        raise ValueError("ABI contract path may not redirect through a symbolic link")
    return path


def validate_abi_contract(va: int, contract: dict) -> None:
    """Validate an ABI contract object against the pinned bytes and schema."""
    required = {"schema", "target_sha256", "address_convention", "va", "declaration",
                "calling_convention", "evidence", "unknowns"}
    if not isinstance(contract, dict) or set(contract) != required:
        raise ValueError("ABI contract must contain exactly the documented schema fields")
    if (contract["schema"] != "cmre-abi-contract/1" or contract["target_sha256"] != env.TARGET_SHA256
            or contract["address_convention"] != env.ADDRESS_CONVENTION or contract["va"] != f"{va:#010x}"):
        raise ValueError("ABI contract target/function identity mismatch")
    declaration = contract["declaration"]
    if (not isinstance(declaration, str) or len(declaration) > 2000
            or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_ *(),\[\]]*;", declaration)
            or not re.search(rf"\bcm_fn_{va:08x}\s*\(", declaration)):
        raise ValueError("ABI declaration must be one plain function declaration without directives or bodies")
    if contract["calling_convention"] not in ("cdecl", "stdcall", "thiscall", "fastcall"):
        raise ValueError("Unknown ABI calling convention")
    unknowns = contract["unknowns"]
    if not isinstance(unknowns, list) or any(not isinstance(s, str) or not s.strip() or len(s) > 2000 for s in unknowns):
        raise ValueError("ABI unknowns must be an explicit list of nonempty descriptions")
    evidence = contract["evidence"]
    if not isinstance(evidence, list) or not 1 <= len(evidence) <= 64:
        raise ValueError("ABI contract requires 1..64 original-byte evidence entries")
    original = image.primary()
    for item in evidence:
        if not isinstance(item, dict) or set(item) != {"va", "bytes", "supports"}:
            raise ValueError("Invalid ABI evidence fields")
        if not isinstance(item["va"], str) or not re.fullmatch(r"0x[0-9a-fA-F]{8}", item["va"]):
            raise ValueError("ABI evidence requires preferred-image VA")
        if not isinstance(item["bytes"], str) or not re.fullmatch(r"(?:[0-9a-fA-F]{2}){1,256}", item["bytes"]):
            raise ValueError(f"ABI evidence at {item['va']} needs 1..256 exact bytes as complete hex pairs, without separators")
        if not isinstance(item["supports"], str) or not item["supports"].strip() or len(item["supports"]) > 2000:
            raise ValueError(f"ABI evidence at {item['va']} needs a nonempty supports claim of at most 2000 characters")
        expected = bytes.fromhex(item["bytes"])
        try:
            actual = original.read(int(item["va"], 16), len(expected))
        except KeyError as exc:
            raise ValueError("ABI evidence is outside original image") from exc
        if actual != expected:
            raise ValueError(f"ABI evidence bytes differ from pinned original at {item['va']}: supplied {item['bytes']}, expected {actual.hex()}")


def _abi_contract(va: int, expected_hash: str | None) -> dict | None:
    """Check analyst evidence identity, without claiming to prove its semantics."""
    path = _contract_path(va)
    if _optional_hash(path) != expected_hash:
        raise ValueError("ABI contract changed while preparing packet")
    if expected_hash is None:
        return None
    if path.stat().st_size > 65536:
        raise ValueError("ABI contract exceeds 64 KiB")
    raw = path.read_bytes()
    if env.digest_bytes(raw) != expected_hash:
        raise ValueError("ABI contract changed while preparing packet")
    contract = json.loads(raw)
    validate_abi_contract(va, contract)
    return {"path": env.rel(path), "sha256": expected_hash, "claims": contract,
            "verification": "Evidence bytes match the pinned original image. Analyst ABI claims are not "
                            "machine-proven semantics; byte agreement alone does not prove the declaration."}


def _inputs(va: int) -> dict:
    path = ledger.record_path(va)
    if not path.exists():
        raise ValueError("Task requires an accepted lifted function")
    record = json.loads(path.read_text(encoding="utf-8"))
    if not record.get("lifted") or record.get("c"):
        raise ValueError("Task requires lifted acceptance and no C-tier acceptance")
    if env.digest(env.TARGET) != env.TARGET_SHA256:
        raise ValueError("Pinned target hash mismatch")
    source = env.LIFTED_DIR / f"fn_{va:08x}.S"
    if env.digest(source) != record["lifted"]["source_sha256"]:
        raise ValueError("Accepted lifted source changed")
    return {
        "execution": recover.execution_identity(),
        "ledger": env.digest(path),
        "lifted_source": env.digest(source),
        "hints": _optional_hash(env.HINTS_DIR / f"fn_{va:08x}.json"),
        "canonical_source": _optional_hash(env.C_DIR / f"fn_{va:08x}.c"),
        "abi_contract": _optional_hash(_contract_path(va)),
        "policy": {"schema": SCHEMA, "max_attempts": MAX_ATTEMPTS, "max_body_bytes": MAX_BODY_BYTES},
    }


def save(manifest: dict) -> None:
    """Save under the task lock; callers own serialization."""
    coordination.atomic_json(task_dir(manifest["task_id"]) / "task.json", manifest)


def _load(task_id: str) -> dict:
    manifest = json.loads((task_dir(task_id) / "task.json").read_text(encoding="utf-8"))
    if manifest.get("schema") != SCHEMA or manifest.get("task_id") != task_id:
        raise ValueError("Invalid task manifest")
    va = int(manifest["va"], 16)
    if manifest.get("candidate_path") != env.rel(_candidate(task_id, va)):
        raise ValueError("Candidate path differs from fixed task path")
    if manifest.get("publish_path") != env.rel(env.C_DIR / f"fn_{va:08x}.c"):
        raise ValueError("Publication path differs from fixed function path")
    return manifest


def validate_current(manifest: dict) -> None:
    if _hash(_inputs(int(manifest["va"], 16))) != manifest["input_fingerprint"]:
        raise ValueError("Stale task inputs; prepare a new packet")


def prepare(va: int) -> dict:
    """Freeze complete instruction evidence for one accepted lifted function."""
    if type(va) is not int or not 0 <= va <= 0xFFFFFFFF:
        raise ValueError("Invalid function VA")
    with coordination.writer():
        inputs = _inputs(va)
        fingerprint = _hash(inputs)
        task_id = f"c-{va:08x}-{fingerprint[:16]}"
        directory = task_dir(task_id)
        directory.mkdir(parents=True, exist_ok=True)
        with coordination.lock(directory / ".lock"):
            if (directory / "task.json").exists():
                return status(task_id)
            function = inventory.load().functions[va]
            if function.body_bytes > MAX_BODY_BYTES:
                raise ValueError(f"Initial C task lane is bounded to {MAX_BODY_BYTES} body bytes")
            lifted = lift.discover(va)
            if lifted.problems:
                raise ValueError(f"Unresolved lift boundaries: {lifted.problems}")
            contract = _abi_contract(va, inputs["abi_contract"])
            candidate = _candidate(task_id, va)
            candidate.parent.mkdir(parents=True, exist_ok=True)
            instructions = [{"va": f"{i.va:#010x}", "bytes": i.raw.hex(),
                             "asm": f"{i.mnemonic} {i.op_str}".rstrip(), "kind": i.kind}
                            for _, i in sorted(lifted.insns.items())]
            manifest = {
                "schema": SCHEMA, "task_id": task_id, "kind": "c", "va": f"{va:#010x}",
                "status": "prepared", "created_at": time.time(), "inputs": inputs,
                "input_fingerprint": fingerprint,
                "target": {"path": env.rel(env.TARGET), "sha256": env.TARGET_SHA256,
                           "address_convention": env.ADDRESS_CONVENTION},
                "candidate_path": env.rel(candidate),
                "publish_path": env.rel(env.C_DIR / f"fn_{va:08x}.c"),
                "allowed_worker_files": [env.rel(candidate)],
                "evidence": {"name": function.name, "region_end": f"{lifted.region_end:#010x}",
                             "instructions": instructions,
                             "assembly": (env.LIFTED_DIR / f"fn_{va:08x}.S").read_text(encoding="utf-8"),
                             "support_header": (env.ROOT / "recovery/include/cmre.h").read_text(encoding="utf-8"),
                             "callees": [f"{x:#010x}" for x in lifted.callees],
                             "imports": lifted.imports},
                "abi": {"inventory_signature_hypothesis": function.signature,
                        "known": {"entry_symbol": f"cm_fn_{va:08x}", "architecture": "x86-32",
                                  "header": "recovery/include/cmre.h",
                                  "return_instructions": [i for i in instructions if i["kind"] == "ret"]},
                        "unknown": ["calling convention until established from instructions",
                                    "argument types and count", "return type", "semantic field names"],
                        "rule": "Preserve instruction behavior; inventory signatures are hypotheses. "
                                "Original callees remain explicit boundaries; never invent initialization."},
                "routing": {"simple_leaf_candidate": not lifted.callees and not lifted.imports
                            and len(instructions) <= 32 and all(i["kind"] not in ("indirect_call", "indirect_jmp", "table_jmp") for i in instructions),
                            "note": "Scheduling signal only, not proof of ABI or semantic simplicity. "
                                    "If ABI cannot be established from instructions, submit evidence-needed."},
                "policy": {"max_attempts": MAX_ATTEMPTS,
                           "max_body_bytes": MAX_BODY_BYTES,
                           "validation": f"py -3.13 -m cmre task submit {task_id} --owner OWNER --token TOKEN",
                           "gate": "unchanged C differential gate, both O0/O2; coordinator accepts separately"},
                "lease": None, "attempts": [], "journal": [],
            }
            if contract is not None:
                manifest["abi"]["analyst_contract"] = contract
                manifest["abi"]["unknown"] = list(contract["claims"]["unknowns"])
            save(manifest)
            return manifest


def _lease(manifest: dict, owner: str, token: str) -> None:
    lease = manifest.get("lease")
    if not lease or lease["owner"] != owner or not secrets.compare_digest(lease["token"], token):
        raise ValueError("Task lease owner/token mismatch")
    if lease["expires_at"] <= time.time():
        raise ValueError("Task lease expired; claim again")
    if manifest["status"] != "claimed":
        raise ValueError(f"Task is {manifest['status']}, not claimed")


def claim(task_id: str, owner: str, lease_seconds: int = 900) -> dict:
    if not isinstance(owner, str) or not owner.strip() or len(owner) > 128:
        raise ValueError("Owner must be a nonempty name of at most 128 characters")
    if type(lease_seconds) is not int or not 1 <= lease_seconds <= 3600:
        raise ValueError("Lease must be 1..3600 seconds")
    with coordination.lock(task_dir(task_id) / ".lock"):
        manifest = _load(task_id)
        validate_current(manifest)
        if manifest["status"] not in ("prepared", "claimed", "publication-failed"):
            raise ValueError(f"Task is {manifest['status']}")
        previous = manifest.get("lease")
        if previous and previous["expires_at"] > time.time():
            raise ValueError("Task already has a live lease")
        if len(manifest["attempts"]) >= MAX_ATTEMPTS:
            raise ValueError("Task attempts exhausted")
        for attempt in manifest["attempts"]:
            if attempt["status"] == "testing":
                attempt.update(status="interrupted", error="Worker ended before validation completed")
        manifest["lease"] = {"owner": owner, "token": secrets.token_hex(24),
                             "expires_at": time.time() + lease_seconds}
        manifest["status"] = "claimed"
        manifest["journal"].append({"event": "claimed", "at": time.time(), "owner": owner})
        save(manifest)
        return manifest


def release_claim(task_id: str, owner: str, token: str, *, reason: str) -> dict:
    """Release an owned C claim after its worker has reached a terminal process state.

    The caller must establish worker termination first. This keeps a failed or
    evidence-blocked worker from holding a live lease for the full hour while
    preserving all staged bytes and attempt reports.
    """
    if not isinstance(owner, str) or not owner.strip() or len(owner) > 128:
        raise ValueError("Owner must be a nonempty name of at most 128 characters")
    if not isinstance(token, str) or not token:
        raise ValueError("A live claim token is required")
    if not isinstance(reason, str) or not reason.strip() or len(reason) > 4000:
        raise ValueError("Release requires a nonempty reason of at most 4000 characters")
    with coordination.lock(task_dir(task_id) / ".lock"):
        manifest = _load(task_id)
        lease = manifest.get("lease")
        if (not lease or lease.get("owner") != owner
                or not secrets.compare_digest(lease.get("token", ""), token)):
            raise ValueError("Task lease owner/token mismatch")
        if lease["expires_at"] <= time.time():
            raise ValueError("Task lease expired; coordinator must reconcile terminal ownership")
        if manifest["status"] not in ("claimed", "exhausted"):
            raise ValueError(f"Task is {manifest['status']}, not a releasable claim")
        if any(attempt.get("status") == "testing" for attempt in manifest["attempts"]):
            raise ValueError("Cannot release while a candidate validation attempt is testing")
        if manifest["status"] == "claimed":
            manifest["status"] = "prepared" if len(manifest["attempts"]) < MAX_ATTEMPTS else "exhausted"
        manifest["lease"] = None
        manifest["journal"].append({"event": "claim-released", "at": time.time(),
            "owner": owner, "reason": reason, "attempt_count": len(manifest["attempts"])})
        save(manifest)
        return manifest


def _metrics(value: dict | None) -> dict:
    value = value or {}
    if not isinstance(value, dict) or set(value) - set(METRICS):
        raise ValueError("Unknown metric fields")
    result = {key: value.get(key) for key in METRICS}
    for key, number in result.items():
        if key == "model":
            if number is not None and (not isinstance(number, str) or not number.strip() or len(number) > 128):
                raise ValueError("Model must be a nonempty name of at most 128 characters or null")
            continue
        if number is not None and (type(number) not in (int, float) or not math.isfinite(number) or number < 0):
            raise ValueError(f"Invalid metric {key}")
        if key.endswith("tokens") and number is not None and type(number) is not int:
            raise ValueError(f"Token count {key} must be an integer or null")
    return result


def submit(task_id: str, owner: str, token: str, *, outcome: str = "candidate",
           summary: str = "", metrics: dict | None = None, ownership_guard=None) -> dict:
    """Validate fixed staged source and journal a bounded attempt; never accept."""
    if outcome not in ("candidate", "failed", "evidence-needed", "harness-defect") or not isinstance(summary, str) or len(summary) > 4000:
        raise ValueError("Invalid submission outcome/summary")
    measured = _metrics(metrics)
    with coordination.lock(task_dir(task_id) / ".lock"):
        manifest = _load(task_id)
        _lease(manifest, owner, token)
        validate_current(manifest)
        if len(manifest["attempts"]) >= MAX_ATTEMPTS:
            raise ValueError("Task attempts exhausted")
        candidate = _candidate(task_id, int(manifest["va"], 16))
        attempt = {"number": len(manifest["attempts"]) + 1, "owner": owner, "at": time.time(),
                   "summary": summary, "metrics": measured, "status": "failed", "outcome": outcome}
        if outcome == "candidate":
            if not candidate.is_file():
                raise ValueError("Write the fixed candidate source before submission")
            candidate_hash = env.digest(candidate)
            attempt.update(status="testing", candidate_sha256=candidate_hash)
        manifest["attempts"].append(attempt)
        save(manifest)
        if outcome == "candidate":
            start = time.monotonic()
            try:
                report = recover.test_c(int(manifest["va"], 16), source=candidate)
                if env.digest(candidate) != candidate_hash:
                    raise ValueError("Candidate changed during validation")
                # External queue ownership is checked and held only for this
                # short commit, so disjoint workers may execute concurrently.
                with ownership_guard() if ownership_guard else nullcontext():
                    _lease(manifest, owner, token)
                    validate_current(manifest)
                    report_path = task_dir(task_id) / f"report-{attempt['number']}.json"
                    coordination.atomic_json(report_path, report)
                    attempt.update(status="pass" if report["status"] == "pass" else "failed",
                                   report_path=env.rel(report_path), report_sha256=env.digest(report_path))
                    if attempt['status'] == 'pass':
                        manifest['status'] = 'submitted'
                        manifest['lease'] = None
                    save(manifest)
            except BaseException as exc:
                attempt.update(status="failed", error=f"{type(exc).__name__}: {exc}")
                manifest["status"] = "exhausted" if len(manifest["attempts"]) >= MAX_ATTEMPTS else "claimed"
                save(manifest)
                raise
            finally:
                attempt["validation_seconds"] = time.monotonic() - start
                save(manifest)
        if attempt["status"] == "pass":
            manifest["status"] = "submitted"
            manifest["lease"] = None
        elif outcome in ("evidence-needed", "harness-defect"):
            manifest["status"] = "escalated"
            manifest["lease"] = None
        elif len(manifest["attempts"]) >= MAX_ATTEMPTS:
            manifest["status"] = "exhausted"
        manifest["journal"].append({"event": "submitted", "at": time.time(), "status": attempt["status"]})
        save(manifest)
        return manifest


def revise(task_id: str, reason: str) -> dict:
    """Coordinator reopens a reviewed submission without resetting its evidence or budget."""
    if not isinstance(reason, str) or not reason.strip() or len(reason) > 4000:
        raise ValueError('Revision requires a nonempty reason of at most 4000 characters')
    with coordination.writer(), coordination.lock(task_dir(task_id) / '.lock'):
        manifest = _load(task_id)
        validate_current(manifest)
        if manifest['status'] != 'submitted':
            raise ValueError('Only a submitted task can be revised')
        if len(manifest['attempts']) >= MAX_ATTEMPTS:
            raise ValueError('Task attempts exhausted')
        if not manifest['attempts'] or manifest['attempts'][-1]['status'] != 'pass':
            raise ValueError('Revision requires a validated submission')
        attempt = manifest['attempts'][-1]
        report = task_dir(task_id) / f"report-{attempt['number']}.json"
        if env.digest(report) != attempt['report_sha256']:
            raise ValueError('Submission report changed')
        manifest['status'] = 'prepared'
        manifest['lease'] = None
        manifest['journal'].append({'event': 'revision-requested', 'at': time.time(),
            'reason': reason, 'previous_candidate_sha256': attempt['candidate_sha256'],
            'candidate_sha256': _optional_hash(_candidate(task_id, int(manifest['va'], 16)))})
        save(manifest)
        return manifest


def resolve_for_publish(task_id: str) -> dict:
    """Caller holds writer and task locks."""
    manifest = _load(task_id)
    if manifest["status"] != "submitted":
        raise ValueError("Task has no validated submission")
    validate_current(manifest)
    attempt = manifest["attempts"][-1]
    if attempt["status"] != "pass" or env.digest(_candidate(task_id, int(manifest["va"], 16))) != attempt["candidate_sha256"]:
        raise ValueError("Submitted candidate changed")
    report = task_dir(task_id) / f"report-{attempt['number']}.json"
    if env.digest(report) != attempt["report_sha256"]:
        raise ValueError("Submission report changed")
    return manifest


def _recover_publication(manifest: dict) -> dict:
    """Reconcile an interrupted publication without destroying accepted source."""
    va = int(manifest["va"], 16)
    destination = env.C_DIR / f"fn_{va:08x}.c"
    record = json.loads(ledger.record_path(va).read_text(encoding="utf-8"))
    publication = manifest["publication"]
    if record.get("c"):
        if record["c"]["source_sha256"] != publication["candidate_sha256"] or _optional_hash(destination) != publication["candidate_sha256"]:
            raise ValueError("Publication has conflicting accepted C evidence; manual review required")
        manifest["status"] = "published"
    else:
        current = _optional_hash(destination)
        if current not in (publication["candidate_sha256"], publication["previous_sha256"]):
            raise ValueError("Publication source changed externally; manual review required")
        if publication["previous_sha256"] is None:
            if destination.exists():
                destination.unlink()
        else:
            backup = task_dir(manifest["task_id"]) / "previous-source.c"
            if env.digest(backup) != publication["previous_sha256"]:
                raise ValueError("Publication backup changed")
            coordination.atomic_bytes(destination, backup.read_bytes())
        manifest["status"] = "publication-failed"
    manifest["journal"].append({"event": "publication-reconciled", "at": time.time(), "status": manifest["status"]})
    save(manifest)
    return manifest


def publish(task_id: str) -> dict:
    """Coordinator-only canonical copy and acceptance, with durable rollback state."""
    with coordination.writer(), coordination.lock(task_dir(task_id) / ".lock"):
        manifest = _load(task_id)
        if manifest["status"] == "publishing":
            return _recover_publication(manifest)
        if manifest["status"] == "published":
            return manifest
        manifest = resolve_for_publish(task_id)
        va = int(manifest["va"], 16)
        candidate = _candidate(task_id, va)
        destination = env.C_DIR / f"fn_{va:08x}.c"
        if not destination.resolve().is_relative_to(env.C_DIR.resolve()):
            raise ValueError("Publication path escapes recovery/c")
        previous_hash = _optional_hash(destination)
        if previous_hash is not None:
            coordination.atomic_bytes(task_dir(task_id) / "previous-source.c", destination.read_bytes())
        manifest["publication"] = {"previous_sha256": previous_hash,
                                   "candidate_sha256": manifest["attempts"][-1]["candidate_sha256"]}
        manifest["status"] = "publishing"
        save(manifest)
        try:
            source_bytes = candidate.read_bytes()
            if env.digest_bytes(source_bytes) != manifest["publication"]["candidate_sha256"]:
                raise ValueError("Candidate changed before publication")
            coordination.atomic_bytes(destination, source_bytes)
            accepted, report = recover.accept(va, "c")
            manifest["publication"].update(accepted=accepted, result=report)
            save(manifest)
            return _recover_publication(manifest)
        except BaseException:
            _recover_publication(manifest)
            raise


def status(task_id: str | None = None):
    """Read task state without exposing live bearer tokens to observers."""
    if task_id is None:
        root = env.ANALYSIS / "cmre/tasks"
        return [status(p.parent.name) for p in sorted(root.glob("*/task.json"))]
    manifest = _load(task_id)
    if manifest.get("lease"):
        manifest["lease"].pop("token", None)
        manifest["lease"]["expired"] = manifest["lease"]["expires_at"] <= time.time()
    return manifest


def summary(manifest: dict, *, include_token: bool = False) -> dict:
    """Small CLI response; instructions are read once from the packet path."""
    result = {key: manifest[key] for key in ("task_id", "va", "status", "candidate_path", "publish_path")}
    result.update(packet_path=env.rel(task_dir(manifest["task_id"]) / "task.json"),
                  attempt_count=len(manifest["attempts"]), max_attempts=MAX_ATTEMPTS,
                  latest_attempt=manifest["attempts"][-1] if manifest["attempts"] else None,
                  validation=manifest["policy"]["validation"])
    if manifest.get("lease"):
        result["lease"] = {key: value for key, value in manifest["lease"].items()
                           if include_token or key != "token"}
    if manifest.get("publication"):
        result["publication"] = manifest["publication"]
    return result


def _cost_totals(attempts: list[dict]) -> dict:
    """Keep partial observed costs distinct from unknown complete costs."""
    costs = {}
    for key in (*NUMERIC_METRICS, "validation_seconds"):
        values = [attempt.get(key) if key == "validation_seconds" else attempt.get("metrics", {}).get(key)
                  for attempt in attempts]
        known = [value for value in values if value is not None]
        costs[key] = {"known_subtotal": sum(known) if known else None,
                      "known_attempts": len(known), "missing_attempts": len(values) - len(known),
                      "total": sum(known) if values and len(known) == len(values) else None}
    costs["all_tokens"] = {
        "known_subtotal": sum(costs[key]["known_subtotal"] or 0 for key in TOKEN_METRICS)
                          if any(costs[key]["known_attempts"] for key in TOKEN_METRICS) else None,
        "total": sum(costs[key]["total"] for key in TOKEN_METRICS)
                 if attempts and all(costs[key]["total"] is not None for key in TOKEN_METRICS) else None,
        "missing_fields": sum(costs[key]["missing_attempts"] for key in TOKEN_METRICS),
    }
    return costs


def record_usage(task_id: str, attempt: int, metrics: dict) -> dict:
    """Correct measured usage after review/publication without changing evidence."""
    if type(attempt) is not int or attempt < 1:
        raise ValueError("Attempt must be a positive integer")
    if not isinstance(metrics, dict) or not metrics:
        raise ValueError("Supply at least one measured usage field")
    validated = _metrics(metrics)
    with coordination.lock(task_dir(task_id) / ".lock"):
        manifest = _load(task_id)
        if attempt > len(manifest["attempts"]):
            raise ValueError("Attempt does not exist")
        item = manifest["attempts"][attempt - 1]
        if item["number"] != attempt:
            raise ValueError("Attempt numbering is inconsistent")
        before = dict(item.get("metrics", {}))
        after = {**before, **{key: validated[key] for key in metrics}}
        item["metrics"] = after
        manifest["journal"].append({"event": "usage-corrected", "at": time.time(),
                                    "attempt": attempt, "before": before, "after": dict(after)})
        save(manifest)
        return manifest


def metrics_summary(task_id: str | None = None) -> dict:
    """Measured attempts and useful outcomes; unknown usage is never free usage.

    Publication credit belongs to the model on the last passing attempt. Tokens
    remain attributed to the model on each attempt, including failed attempts.
    These receipts support comparisons; they do not establish a speedup alone.
    """
    manifests = [status(task_id)] if task_id is not None else status()
    attempts = [attempt for manifest in manifests for attempt in manifest["attempts"]]
    models = {attempt.get("metrics", {}).get("model") for attempt in attempts}
    rows = []
    for model in sorted(models, key=lambda value: value or ""):
        selected = [attempt for attempt in attempts if attempt.get("metrics", {}).get("model") == model]
        published = [manifest for manifest in manifests if manifest["status"] == "published"
                     and manifest["attempts"] and manifest["attempts"][-1].get("metrics", {}).get("model") == model]
        rows.append({"model": model, "attempts": len(selected),
                     "useful_published": len(published),
                     "first_pass_published": sum(len(manifest["attempts"]) == 1 for manifest in published),
                     "retries": sum(attempt["number"] > 1 for attempt in selected),
                     "escalations": sum(attempt.get("outcome") in ("evidence-needed", "harness-defect") for attempt in selected),
                     "costs": _cost_totals(selected)})
    published = [manifest for manifest in manifests if manifest["status"] == "published"]
    return {"schema": "cmre-task-metrics/1", "tasks": len(manifests), "attempts": len(attempts),
            "useful_published": len(published),
            "first_pass_published": sum(len(manifest["attempts"]) == 1 for manifest in published),
            "retries": sum(max(0, len(manifest["attempts"]) - 1) for manifest in manifests),
            "escalations": sum(attempt.get("outcome") in ("evidence-needed", "harness-defect") for attempt in attempts),
            "costs": _cost_totals(attempts), "by_model": rows,
            "limitations": "Usage is caller-reported. Missing fields remain unknown. "
                           "Publication is local C acceptance, not subsystem closure or runtime integration."}
