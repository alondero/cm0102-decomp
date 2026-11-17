"""Apply a reviewed complete hint proposal; no semantic or acceptance claim."""
from __future__ import annotations

import argparse
import json
import math
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import zlib

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from cmre import coordination, difftest, env, ledger


def require(condition, reason):
    if not condition:
        raise ValueError(reason)


def same(left, right):
    # JSON-type-sensitive deep equality (Python considers True == 1).
    return json.dumps(left, sort_keys=True) == json.dumps(right, sort_keys=True)


def strict_json(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, "duplicate JSON key")
            result[key] = value
        return result

    def invalid(value):
        raise ValueError("nonfinite JSON value")

    def finite(value):
        number = float(value)
        require(math.isfinite(number), "nonfinite JSON value")
        return number

    return json.loads(raw.decode("utf-8"), object_pairs_hook=pairs,
                      parse_constant=invalid, parse_float=finite)


def decode(raw):
    value = strict_json(raw)
    difftest.validate_hints(value)
    return value


def ledger_scope(fn):
    """'unaccepted-top-level' or 'accepted-lifted-c-only'; accepted C stays refused."""
    va = int(fn[3:], 16)
    require(not any(entry.va == va for entry in ledger.legacy_entries()), "legacy function")
    record_path = env.LEDGER_DIR / f"{fn}.json"
    if not record_path.exists() and not record_path.is_symlink():
        return "unaccepted-top-level", None
    require(not record_path.is_symlink() and record_path.is_file(), "ledger path is not a regular file")
    raw = record_path.read_bytes()
    try:
        record = strict_json(raw)
    except (UnicodeError, ValueError) as exc:
        raise ValueError(f"invalid ledger record: {exc}") from exc
    require(isinstance(record, dict), "ledger record must be an object")
    require(record.get("schema") == ledger.SCHEMA, "invalid ledger schema")
    require(record.get("va") == f"0x{va:08x}", "ledger VA differs")
    if "c" in record and record["c"] is not None:
        raise ValueError("accepted C requires the separate review/adopt lane")
    lifted = record.get("lifted")
    require(isinstance(lifted, dict) and isinstance(lifted.get("validation"), dict),
            "validated lifted ledger required")
    require(isinstance(record.get("status"), dict) and record["status"].get("validated") is True,
            "validated lifted ledger required")
    return "accepted-lifted-c-only", env.digest_bytes(raw)


def identity(path, expected):
    require(not path.is_symlink() and path.resolve() ==
            env.ROOT.resolve() / "recovery" / "hints" / path.name, "noncanonical hint path")
    if expected == "absent":
        require(not path.exists() and not path.is_symlink(), "canonical hint exists")
        return "absent", None
    require(path.is_file(), "canonical hint missing")
    stat = path.stat()
    raw = path.read_bytes()
    return (stat.st_dev, stat.st_ino, stat.st_size, stat.st_mtime_ns), raw


def retain(directory, prior, proposal, manifest):
    """New exclusive evidence bundle; never reuse or overwrite prior evidence."""
    directory.mkdir(parents=True, exist_ok=False)
    for name, raw in (("prior.json", prior), ("proposal.json", proposal),
                      ("manifest.json", (json.dumps(manifest, sort_keys=True) + "\n").encode())):
        with (directory / name).open("xb") as stream:
            stream.write(raw)
            stream.flush()
            os.fsync(stream.fileno())
        (directory / name).chmod(0o444)


def _prefix_added(old, new, key, label):
    prefix, values = old.get(key, []), new.get(key, [])
    require(key not in old or key in new, f"removed input field: {label}")
    require(isinstance(prefix, list) and isinstance(values, list), f"{label} must be lists")
    require(len(values) >= len(prefix) and same(values[:len(prefix)], prefix),
            f"{label} prefix changed")
    return len(values) - len(prefix)


def top_level_append(old, new, va):
    """Existing unaccepted behavior: top-level fixtures/captures/provenance append."""
    require(new.get("auto") is not True, "proposal must remove auto:true to claim hint")
    mutable = {"fixtures", "captures", "fixture_provenance"}
    old_policy = {k: v for k, v in old.items() if k not in mutable}
    new_policy = {k: v for k, v in new.items() if k not in mutable}
    if old_policy.get("auto") is True and "auto" not in new_policy:
        del old_policy["auto"]
    require(same(old_policy, new_policy), "nonappend policy or generator change")
    counts = {"fixtures_added": _prefix_added(old, new, "fixtures", "fixtures"),
              "captures_added": _prefix_added(old, new, "captures", "captures")}
    for name in new.get("captures", []):
        difftest.load_capture(va, name)
    return counts


def c_only_append(old, new, va):
    """Accepted lifted/no-C: only effective C fixtures/captures/provenance may append."""
    require(new.get("auto") is not True, "proposal must remove auto:true to claim hint")
    top_old = {k: v for k, v in old.items() if k != "c_hints"}
    top_new = {k: v for k, v in new.items() if k != "c_hints"}
    if top_old.get("auto") is True and "auto" not in top_new:
        del top_old["auto"]
    require(same(top_old, top_new), "shared hints or generator changed")
    old_c, new_c = old.get("c_hints", {}), new.get("c_hints", {})
    require(isinstance(old_c, dict) and isinstance(new_c, dict), "c_hints must be objects")
    metadata = {"fixture_evidence", "notes", "input_probe", "input_probe_sha256"}
    mutable = {"fixtures", "captures", "fixture_provenance"} | metadata
    require(same({k: v for k, v in old_c.items() if k not in mutable},
                 {k: v for k, v in new_c.items() if k not in mutable}),
            "C shared input, policy or generator changed")
    for key in metadata:
        if key in old_c:
            require(key in new_c and same(old_c[key], new_c[key]),
                    f"existing C metadata changed: {key}")
        elif key in old:
            require(same(old[key], new_c.get(key, new[key])),
                    f"inherited C metadata changed: {key}")
    old_fixtures = old_c.get("fixtures", old.get("fixtures", []))
    new_fixtures = new_c.get("fixtures", new.get("fixtures", []))
    old_captures = old_c.get("captures", old.get("captures", []))
    new_captures = new_c.get("captures", new.get("captures", []))
    for key in ("fixtures", "captures"):
        if key in old_c:
            require(key in new_c, f"removed C input field: {key}")
    require(isinstance(old_fixtures, list) and isinstance(new_fixtures, list),
            "C fixtures must be lists")
    require(len(new_fixtures) >= len(old_fixtures)
            and same(new_fixtures[:len(old_fixtures)], old_fixtures),
            "C fixtures prefix changed")
    require(isinstance(old_captures, list) and isinstance(new_captures, list),
            "C captures must be lists")
    require(len(new_captures) >= len(old_captures)
            and same(new_captures[:len(old_captures)], old_captures),
            "C captures prefix changed")
    old_provenance = old_c.get("fixture_provenance", old.get("fixture_provenance", {}))
    new_provenance = new_c.get("fixture_provenance", new.get("fixture_provenance", {}))
    require(isinstance(old_provenance, dict) and isinstance(new_provenance, dict),
            "fixture_provenance must be objects")
    if "fixture_provenance" in old_c:
        require("fixture_provenance" in new_c, "removed C input field: fixture_provenance")
    require(all(key in new_provenance and same(value, new_provenance[key])
                for key, value in old_provenance.items()),
            "C fixture provenance prefix changed")
    for name in new_captures:
        difftest.load_capture(va, name)
    return {"c_fixtures_added": len(new_fixtures) - len(old_fixtures),
            "c_captures_added": len(new_captures) - len(old_captures),
            "c_fixture_provenance_added": len(new_provenance) - len(old_provenance)}


def apply_proposal(fn, proposal_path, expected, evidence):
    require(re.fullmatch(r"fn_[0-9a-f]{8}", fn), "invalid function ID")
    require(expected == "absent" or re.fullmatch(r"[0-9a-f]{64}", expected),
            "invalid expected hash")
    path = env.HINTS_DIR / f"{fn}.json"
    evidence = evidence.absolute()
    resolved = evidence.resolve()
    require(resolved.is_relative_to(env.ANALYSIS.resolve()) and
            resolved != env.ANALYSIS.resolve(), "evidence must be under analysis")
    require(not evidence.is_symlink() and not evidence.exists(), "evidence already exists")
    ignored = subprocess.run(["git", "check-ignore", "--quiet", "--no-index", "--", str(resolved)],
                             cwd=env.ROOT, capture_output=True)
    require(ignored.returncode == 0, "evidence is not ignored")
    with coordination.writer():
        scope, ledger_sha = ledger_scope(fn)
        before, prior = identity(path, expected)
        if expected != "absent":
            require(env.digest_bytes(prior) == expected, "current hash differs")
        if expected == "absent":
            require(scope == "accepted-lifted-c-only",
                    "absent hints require a validated lifted ledger without accepted C")
        require(not proposal_path.is_symlink(), "proposal must not be a symlink")
        require(proposal_path.resolve() != path.resolve(), "proposal aliases canonical hint")
        proposal = proposal_path.read_bytes()
        old = decode(prior) if prior is not None else {}
        new = decode(proposal)
        if scope == "accepted-lifted-c-only":
            counts = c_only_append(old, new, int(fn[3:], 16))
        else:
            counts = top_level_append(old, new, int(fn[3:], 16))
        manifest = {"function": fn, "canonical_path": str(path),
                    "proposal_path": str(proposal_path), "prior_sha256": expected,
                    "prior_identity": before, "prior_absent": prior is None,
                    "proposal_sha256": env.digest_bytes(proposal),
                    "ledger_sha256": ledger_sha, "scope": scope,
                    "validation": "schema-and-append-only", "acceptance": "not-checked",
                    **counts}
        retain(evidence, prior if prior is not None else b"", proposal, manifest)
        manifest_hash = env.digest(evidence / "manifest.json")
        scope_after, ledger_after = ledger_scope(fn)
        require(scope_after == scope and ledger_after == ledger_sha, "ledger changed during application")
        require(identity(path, expected) == (before, prior), "canonical identity changed")
        coordination.atomic_bytes(path, proposal)
    return {"status": "applied", "function": fn, "path": str(path),
            "evidence_dir": str(evidence), "sha256": manifest["proposal_sha256"],
            "manifest_sha256": manifest_hash, "scope": scope,
            "prior_sha256": expected, "prior_absent": prior is None,
            "acceptance": "not-checked", **counts}

class Parser(argparse.ArgumentParser):
    def error(self, message):
        raise ValueError("invalid command line")


def main(argv=None):
    parser = Parser(description=__doc__, add_help=False)
    parser.add_argument("fn")
    parser.add_argument("--proposal", type=Path, required=True)
    parser.add_argument("--expected-sha256", required=True,
                        help="Exact prior byte hash, or 'absent' for missing accepted-lifted hints")
    parser.add_argument("--evidence-dir", type=Path, required=True)
    try:
        args = parser.parse_args(argv)
        result = apply_proposal(args.fn, args.proposal, args.expected_sha256, args.evidence_dir)
    except (OSError, ValueError, TypeError, KeyError, AttributeError,
            coordination.BusyError, zlib.error, struct.error, SystemExit,
            OverflowError, RecursionError) as exc:
        # Exception messages from schema/capture parsers can contain private payloads.
        result = {"status": "refused", "error_type": type(exc).__name__,
                  "acceptance": "not-checked"}
        print(json.dumps(result, sort_keys=True))
        return 2
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
