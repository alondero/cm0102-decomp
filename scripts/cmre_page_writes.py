"""Observe guest writes during one cmre explain case without changing the gate."""
from __future__ import annotations

import argparse
from contextlib import nullcontext
import json
from pathlib import Path
import sys
from unittest.mock import patch


def _run_on_page(run, machine, entry, case, budget, page, limit):
    from unicorn import UC_HOOK_MEM_WRITE
    from unicorn.x86_const import UC_X86_REG_EIP

    end = page + 4096
    writes, observed = [], 0

    def record(uc, _access, address, size, value, _data):
        nonlocal observed
        if address >= end or address + size <= page:
            return
        observed += 1
        if len(writes) < limit:
            writes.append({
                "ip": f"{uc.reg_read(UC_X86_REG_EIP):#010x}",
                "address": f"{address:#010x}",
                "size": size,
                "value": hex(value & ((1 << (8 * size)) - 1)) if size <= 8 else None,
            })

    hook = machine.uc.hook_add(UC_HOOK_MEM_WRITE, record)
    try:
        result = run(machine, entry, case, budget)
    finally:
        machine.uc.hook_del(hook)
    return result, {"observed": observed, "truncated": observed > len(writes), "writes": writes}


def replay(va, case_index, page, *, c=False, source=None, opt="-O2", limit=256,
           cases=None, budget=None, profile=None, window=6):
    """Run a single-threaded diagnostic replay, keeping only the final retry."""
    from cmre import emu, recover
    from cmre.cli import _diagnostic_source

    if type(case_index) is not int or case_index < 0:
        raise ValueError("case must be a nonnegative index")
    if type(page) is not int or not 0 <= page <= 0xfffff000 or page % 4096:
        raise ValueError("page must be a 4 KiB-aligned 32-bit address")
    if type(limit) is not int or not 1 <= limit <= 256:
        raise ValueError("limit must be between 1 and 256")
    if not c and (source is not None or profile is not None):
        raise ValueError("--source and --profile require --c")
    overrides = {key: val for key, val in (("cases", cases), ("budget", budget)) if val is not None}

    base_run = emu.Machine.run
    original_index = 0
    traces = {}

    def observed_run(machine, entry, case, budget):
        nonlocal original_index
        side = "recovered" if machine.recovered is not None else "original"
        if side == "original":
            index = original_index
            original_index += 1
            if index != case_index:
                return base_run(machine, entry, case, budget)
        elif original_index != case_index + 1:
            raise RuntimeError("unexpected recovered replay order")
        result, traces[side] = _run_on_page(base_run, machine, entry, case, budget, page, limit)
        return result

    fence = _diagnostic_source(va, source) if source is not None else nullcontext()
    with fence as checked_source, patch.object(emu.Machine, "run", observed_run):
        report = (recover.explain_c(va, case_index, overrides, source=checked_source, profile=profile,
                                    opt=opt, window=window) if c
                  else recover.explain(va, case_index, overrides, window=window))
    if report.get("case") == case_index:
        expected = {side for side in ("original", "recovered") if side in report}
        if original_index != case_index + 1 or set(traces) != expected:
            raise RuntimeError("explain replay does not match the observed run sequence")
        report["page_writes"] = {
            "page": f"{page:#010x}",
            "scope": "guest write attempts (including faulting stores); host writes are excluded",
            **traces,
        }
    return report


def main(argv=None):
    if not __package__:
        sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from cmre.cli import _va

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("function", type=_va)
    parser.add_argument("--case", type=int, required=True)
    parser.add_argument("--page", type=lambda value: int(value, 0), required=True)
    parser.add_argument("--c", action="store_true")
    parser.add_argument("--source", type=Path)
    parser.add_argument("--opt", choices=("-O0", "-O2"), default="-O2")
    parser.add_argument("--profile", choices=("semantic", "abi"))
    parser.add_argument("--cases", type=int)
    parser.add_argument("--budget", type=int)
    parser.add_argument("--window", type=int, default=6)
    args = parser.parse_args(argv)
    try:
        report = replay(args.function, args.case, args.page, c=args.c, source=args.source, opt=args.opt,
                        cases=args.cases, budget=args.budget, profile=args.profile, window=args.window)
    except (ValueError, RuntimeError) as exc:
        parser.error(str(exc))
    print(json.dumps(report, indent=2))
    return 0 if "page_writes" in report else 1


if __name__ == "__main__":
    raise SystemExit(main())
