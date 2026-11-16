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


def decode(raw):
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

    value = json.loads(raw.decode("utf-8"), object_pairs_hook=pairs,
                       parse_constant=invalid, parse_float=finite)
    difftest.validate_hints(value)
    return value


def unaccepted(fn):
    record = env.LEDGER_DIR / f"{fn}.json"
    require(not record.exists() and not record.is_symlink(), "ledger exists")
    require(not any(entry.va == int(fn[3:], 16) for entry in ledger.legacy_entries()),
            "legacy function")


def identity(path):
    require(not path.is_symlink() and path.resolve() ==
            env.ROOT.resolve() / "recovery" / "hints" / path.name, "noncanonical hint path")
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


def apply_proposal(fn, proposal_path, expected, evidence):
    require(re.fullmatch(r"fn_[0-9a-f]{8}", fn), "invalid function ID")
    require(re.fullmatch(r"[0-9a-f]{64}", expected), "invalid expected hash")
    path = env.HINTS_DIR / f"{fn}.json"
    evidence = evidence.absolute()
    resolved = evidence.resolve()
    require(resolved.is_relative_to(env.ANALYSIS.resolve()) and
            resolved != env.ANALYSIS.resolve(), "evidence must be under analysis")
    require(not evidence.exists(), "evidence already exists")
    ignored = subprocess.run(["git", "check-ignore", "--quiet", "--no-index", "--", str(resolved)],
                             cwd=env.ROOT, capture_output=True)
    require(ignored.returncode == 0, "evidence is not ignored")
    with coordination.writer():
        unaccepted(fn)
        before, prior = identity(path)
        require(env.digest_bytes(prior) == expected, "current hash differs")
        require(proposal_path.resolve() != path.resolve(), "proposal aliases canonical hint")
        proposal = proposal_path.read_bytes()
        old, new = decode(prior), decode(proposal)
        require(new.get("auto") is not True, "proposal must remove auto:true to claim hint")
        mutable = {"fixtures", "captures", "fixture_provenance"}
        old_policy = {k: v for k, v in old.items() if k not in mutable}
        new_policy = {k: v for k, v in new.items() if k not in mutable}
        if old_policy.get("auto") is True and "auto" not in new_policy:
            del old_policy["auto"]
        require(same(old_policy, new_policy), "nonappend policy or generator change")
        counts = {}
        for key in ("fixtures", "captures"):
            prefix, values = old.get(key, []), new.get(key, [])
            require(key not in old or key in new, "removed input field")
            require(len(values) >= len(prefix) and same(values[:len(prefix)], prefix),
                    "input prefix changed")
            counts[f"{key}_added"] = len(values) - len(prefix)
        for name in new.get("captures", []):
            difftest.load_capture(int(fn[3:], 16), name)
        manifest = {"function": fn, "canonical_path": str(path),
                    "proposal_path": str(proposal_path), "prior_sha256": expected,
                    "prior_identity": list(before),
                    "proposal_sha256": env.digest_bytes(proposal),
                    "validation": "schema-and-append-only", "acceptance": "not-checked",
                    **counts}
        retain(evidence, prior, proposal, manifest)
        manifest_hash = env.digest(evidence / "manifest.json")
        unaccepted(fn)
        require(identity(path) == (before, prior), "canonical identity changed")
        coordination.atomic_bytes(path, proposal)
    return {"status": "applied", "function": fn, "path": str(path),
            "evidence_dir": str(evidence), "sha256": manifest["proposal_sha256"],
            "manifest_sha256": manifest_hash,
            "prior_sha256": expected, "acceptance": "not-checked", **counts}


class Parser(argparse.ArgumentParser):
    def error(self, message):
        raise ValueError("invalid command line")


def main(argv=None):
    parser = Parser(description=__doc__, add_help=False)
    parser.add_argument("fn")
    parser.add_argument("--proposal", type=Path, required=True)
    parser.add_argument("--expected-sha256", required=True)
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
