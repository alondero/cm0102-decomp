"""Read-only hint-schema preflight; does not build or execute recovered code."""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from cmre import difftest


def inspect(path: Path) -> dict:
    hints = json.loads(path.read_text(encoding="utf-8"))
    difftest.validate_hints(hints)
    effective = {**{k: v for k, v in hints.items() if k != "c_hints"},
                 **hints.get("c_hints", {})}
    return {
        "status": "pass",
        "validation": "schema-only",
        "path": str(path),
        "c_arg_count": effective.get("arg_count", difftest.DEFAULTS["arg_count"]),
        "c_fixtures": len(effective.get("fixtures", [])),
        "c_captures": len(effective.get("captures", [])),
        "recovery_changed": False,
        "acceptance": "not-checked",
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("hints", type=Path, help="complete canonical or private hint JSON")
    args = parser.parse_args(argv)
    try:
        result = inspect(args.hints)
    except (OSError, UnicodeError, ValueError) as exc:
        reason = str(exc)
        print(json.dumps({"status": "fail", "validation": "schema-only",
                          "path": str(args.hints), "reason": reason[:600],
                          "reason_truncated": len(reason) > 600}, sort_keys=True))
        return 2
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
