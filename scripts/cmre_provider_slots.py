"""Provision isolated provider worker slots and split a campaign across them.

One OpenCode client state database cannot serve two concurrent workers for the
same provider: the second client fails with ``database is locked``. Each slot
gets its own XDG data/state/config/cache tree and a copy of the provider's
credential file, so N workers can run at once without sharing a database.

The credential file is copied, never read into output: this module only records
paths and sizes, and refuses to print file contents.

Usage (from the repository root):

    py -3.13 scripts/cmre_provider_slots.py provision --profiles P.json \
        --output SLOT_ROOT --slots 4
    py -3.13 scripts/cmre_provider_slots.py split --manifest M.json \
        --output MANIFEST_ROOT --slots 4

``provision`` writes ``SLOT_ROOT/<role>/<slot>/`` and one
``SLOT_ROOT/profiles-<slot>.json`` per slot. ``split`` writes
``MANIFEST_ROOT/manifest-<slot>.json`` with disjoint, round-robin jobs and the
same frozen duration and roles.
"""
from __future__ import annotations

import argparse
import json
import shutil
import sys
from pathlib import Path

#: Environment variables that select a client's state tree. Every slot gets its
#: own value for each, so no two workers open the same database.
XDG_VARIABLES = ("XDG_DATA_HOME", "XDG_STATE_HOME", "XDG_CONFIG_HOME", "XDG_CACHE_HOME")
SUBDIRECTORIES = {"XDG_DATA_HOME": "data", "XDG_STATE_HOME": "state",
                  "XDG_CONFIG_HOME": "config", "XDG_CACHE_HOME": "cache"}
#: Credentials live under this relative path inside XDG_DATA_HOME.
CREDENTIAL_PATH = Path("opencode") / "auth.json"
MAX_SLOTS = 8


def _load(path: Path) -> dict:
    value = json.loads(Path(path).read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise ValueError(f"{path} must contain a JSON object")
    return value


def _write(path: Path, value: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")


def _slots(value: int) -> int:
    if not 1 <= value <= MAX_SLOTS:
        raise ValueError(f"slots must be 1..{MAX_SLOTS}")
    return value


def provision(profiles: dict, output: Path, slots: int, *, require_credentials: bool = True,
              effort: str | None = None, output_tokens: int | None = None) -> dict:
    """Create ``slots`` isolated profile files, each with its own state tree."""
    slots = _slots(slots)
    if effort is not None and effort not in ('low', 'medium', 'high', 'xhigh', 'max'):
        raise ValueError('Unsupported provider effort')
    if output_tokens is not None and (type(output_tokens) is not int or not 1024 <= output_tokens <= 131072):
        raise ValueError('output_tokens must be 1024..131072')
    output = Path(output).resolve()
    if set(profiles) != {"deepseek", "minimax"}:
        raise ValueError("profiles must contain exactly deepseek and minimax")
    report = {"schema": "cmre-provider-slots/1", "slots": slots, "roles": {}, "profiles": []}
    for role, profile in profiles.items():
        if not isinstance(profile, dict) or not isinstance(profile.get("environment"), dict):
            raise ValueError(f"profile {role} needs an environment mapping")
        entry = {"root": str(output / role), "credentials": None, "copied": False}
        for slot in range(slots):
            root = output / role / str(slot)
            environment = dict(profile["environment"])
            for variable, subdirectory in SUBDIRECTORIES.items():
                environment[variable] = str(root / subdirectory)
            if slot == 0:
                source = None
                base_data = profile["environment"].get("XDG_DATA_HOME")
                if base_data:
                    candidate = Path(base_data) / CREDENTIAL_PATH
                    if candidate.is_file():
                        source = candidate
                if source is None and require_credentials:
                    raise ValueError(
                        f"profile {role} has no credential file at "
                        f"XDG_DATA_HOME/{CREDENTIAL_PATH.as_posix()}")
                entry["credentials"] = str(source) if source else None
            source = Path(entry["credentials"]) if entry["credentials"] else None
            destination = root / SUBDIRECTORIES["XDG_DATA_HOME"] / CREDENTIAL_PATH
            if source is not None:
                destination.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(source, destination)
                entry["copied"] = True
            else:
                destination.parent.mkdir(parents=True, exist_ok=True)
            for subdirectory in SUBDIRECTORIES.values():
                (root / subdirectory).mkdir(parents=True, exist_ok=True)
        report["roles"][role] = entry
    for slot in range(slots):
        slot_profiles = {}
        for role, profile in profiles.items():
            environment = dict(profile["environment"])
            root = output / role / str(slot)
            for variable, subdirectory in SUBDIRECTORIES.items():
                environment[variable] = str(root / subdirectory)
            slot_profiles[role] = {**profile, "environment": environment}
            if effort is not None:
                slot_profiles[role]['effort'] = effort
            if output_tokens is not None:
                slot_profiles[role]['environment']['OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX'] = str(output_tokens)
        path = output / f"profiles-{slot}.json"
        _write(path, slot_profiles)
        report["profiles"].append({"slot": slot, "path": str(path)})
    _write(output / 'slots.json', report)
    return report


def split(manifest: dict, output: Path, slots: int) -> dict:
    """Write one manifest per slot with disjoint jobs."""
    slots = _slots(slots)
    if manifest.get("schema") != "cmre-provider-campaign/1":
        raise ValueError("not a provider campaign manifest")
    jobs = manifest.get("jobs")
    if not isinstance(jobs, list) or not jobs:
        raise ValueError("manifest needs at least one job")
    output = Path(output).resolve()
    report = {"schema": "cmre-provider-manifests/1", "slots": slots, "manifests": []}
    for slot in range(slots):
        selected = jobs[slot::slots]
        if not selected:
            continue
        payload = dict(manifest)
        payload["jobs"] = selected
        payload["concurrency"] = 1
        path = output / f"manifest-{slot}.json"
        _write(path, payload)
        report["manifests"].append({"slot": slot, "path": str(path),
                                    "jobs": [job["id"] for job in selected]})
    return report


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("provision", help="create isolated worker slots")
    p.add_argument("--profiles", type=Path, required=True)
    p.add_argument("--output", type=Path, required=True)
    p.add_argument("--slots", type=int, default=4)
    p.add_argument('--effort', choices=('low', 'medium', 'high', 'xhigh', 'max'))
    p.add_argument('--output-token-cap', type=int)
    s = sub.add_parser("split", help="split a campaign manifest across slots")
    s.add_argument("--manifest", type=Path, required=True)
    s.add_argument("--output", type=Path, required=True)
    s.add_argument("--slots", type=int, default=4)
    args = parser.parse_args(argv)
    try:
        if args.command == "provision":
            report = provision(_load(args.profiles), args.output, args.slots,
                               effort=args.effort, output_tokens=args.output_token_cap)
        else:
            report = split(_load(args.manifest), args.output, args.slots)
    except (OSError, ValueError) as error:
        print(f"{type(error).__name__}: {error}", file=sys.stderr)
        return 1
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
