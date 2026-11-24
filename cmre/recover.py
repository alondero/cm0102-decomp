"""Pipeline: lift/build -> differential test -> (accept | block), with cached reports.

Run reports live in CM0102/Analysis/cmre/runs/fn_<va>/<key>/ (ignored, local).
The key hashes the harness code, recovered source, hints and options, so an
identical request reuses its report and any change produces a fresh run.
"""
from __future__ import annotations

import functools
import importlib.metadata
import json
import os
import re
import sys
import tempfile
import time
from pathlib import Path

import capstone

from . import HARNESS_VERSION, build, coordination, difftest, env, inventory, jump_transport, ledger, lift, return_transport, semantic, x87_transport

from . import acceptance_request, operand_evidence

LIFTED_PROFILE = "exact"
C_PROFILE = "semantic"  # default for new C; existing records keep theirs (ADR 0002)
C_PROFILES = ("abi", "semantic")
C_INPUTS = "v2"  # input policy for new C; accepted records keep theirs (difftest.INPUT_POLICIES)
C_OPT_LEVELS = ("-O0", "-O2")


# Modules whose code decides test results. Guard/CLI/queue/ledger changes do not
# change what a run proves, so they do not change the recorded harness identity.
HARNESS_MODULES = ("__init__.py", "env.py", "image.py", "inventory.py", "lift.py", "build.py", "emu.py",
                   "imports.py", "difftest.py", "recover.py", "return_transport.py", "jump_transport.py", "x87_transport.py",
                   "semantic.py", "csource.py", "acceptance_request.py", "operand_evidence.py")


@functools.cache
def harness_digest() -> str:
    """Identity of the harness code as loaded by this process (read once, at first use)."""
    here = Path(__file__).parent
    return env.digest_bytes(b"".join(n.encode() + (here / n).read_bytes() for n in HARNESS_MODULES))


def _key(*parts) -> str:
    return env.digest_bytes(json.dumps(parts, sort_keys=True, default=str).encode())[:16]


def execution_identity() -> dict:
    """Hash actual inputs, not just declared pins or compiler version strings."""
    return {"target": env.digest(env.TARGET),
            "inventory": {env.rel(p): env.digest(p) for p in env.INVENTORY_SHA256},
            "harness": env.digest_bytes(b"".join(n.encode() + (Path(__file__).parent / n).read_bytes() for n in HARNESS_MODULES)), "python": sys.version,
            "dependencies": {name: importlib.metadata.version(name) for name in ("capstone", "unicorn", "pefile")},
            "toolchain": {name: {**value, "sha256": env.digest(Path(env.tool(name)))}
                          for name, value in env.toolchain_identity().items()},
            "headers": {env.rel(p): env.digest(p) for p in sorted((env.ROOT / "recovery/include").rglob("*")) if p.is_file()}}


def _call_returns(lifted: lift.Lifted, built: build.Built) -> dict[int, int]:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = {}
    for va, rec in lifted.insns.items():
        if rec.kind in ("call", "import_call", "indirect_call"):
            addr = built.symbols[lift.label(va)]
            ins = next(md.disasm(built.image.read_upto(addr, 16), addr, count=1))
            out[addr + ins.size] = va + rec.size
    return out


def _lift_summary(lifted: lift.Lifted) -> dict:
    return {
        "instructions": len(lifted.insns),
        "instruction_bytes": lifted.instruction_bytes,
        "ghidra_body_bytes": lifted.ghidra_body_bytes,
        "byte_fallbacks": [f"{i.va:#010x}" for i in lifted.insns.values() if i.as_bytes],
        "jump_tables": [f"{t.va:#010x}:{len(t.targets)}" for t in lifted.tables.values()],
        "data_runs": [f"{d.va:#010x}:{len(d.raw)}" for d in lifted.data],
        "callees": [f"{c:#010x}" for c in lifted.callees],
        "imports": lifted.imports,
    }


def _data_spans(lifted: lift.Lifted) -> list[tuple[int, int]]:
    spans = [(d.va, d.va + len(d.raw)) for d in lifted.data]
    return spans + [(t.va, t.va + 4 * len(t.targets)) for t in lifted.tables.values()]


def extent(lifted: lift.Lifted) -> tuple[int, int]:
    end = max(i.va + i.size for i in lifted.insns.values())
    end = max([end] + [t.va + 4 * len(t.targets) for t in lifted.tables.values()])
    return lifted.va, end


def test_lifted(va: int, overrides: dict | None = None, fresh: bool = False) -> dict:
    opts = difftest.Options.for_function(va, overrides)
    identity = execution_identity()
    request = _key(identity, va, opts.__dict__, LIFTED_PROFILE)
    index = env.RUNS / f"fn_{va:08x}" / f"request-{request}.json"
    if index.exists() and not fresh:
        try:
            entry = json.loads(index.read_text(encoding="utf-8"))
            key = entry["key"]
            if not isinstance(key, str) or len(key) != 16 or any(c not in "0123456789abcdef" for c in key):
                raise ValueError("invalid report key")
            directory = index.parent / key
            report = json.loads((directory / "report.json").read_text(encoding="utf-8"))
            if (report["key"] == key and report["execution_identity"] == identity
                    and report["source_sha256"] == env.digest(directory / f"fn_{va:08x}.S")):
                return report
        except (OSError, ValueError, KeyError, TypeError):
            pass  # An interrupted or stale publication is a cache miss.
    with tempfile.TemporaryDirectory(prefix="cmre-") as tmp:
        lifted, text, built = build.lift_and_build(va, Path(tmp))
        base = {
            "harness_version": HARNESS_VERSION,
            "va": f"{va:#010x}",
            "name": lifted.name,
            "tier": "lifted",
            "lift": _lift_summary(lifted),
            "problems": lifted.problems,
        }
        if built is None:
            return {**base, "status": "lift-blocked"}
        key = _key(identity, text, opts.__dict__, LIFTED_PROFILE)
        run_dir = env.RUNS / f"fn_{va:08x}" / key
        cached = run_dir / "report.json"
        if cached.exists() and not fresh:
            return json.loads(cached.read_text(encoding="utf-8"))
        result = difftest.run(va, (lifted.va, lifted.region_end), set(lifted.insns), built,
                              _call_returns(lifted, built), opts, LIFTED_PROFILE, _data_spans(lifted))
        status = "pass"
        if result["mismatches"]:
            status = "fail"
        elif result["code_as_data_cases"] > difftest.CODE_AS_DATA_LIMIT * opts.cases:
            status = "code-as-data"
        elif result["coverage"] < opts.min_coverage:
            status = "low-coverage"
        report = {**base, "status": status, "key": key, "execution_identity": identity, "source_sha256": env.digest_bytes(text.encode()),
                  "return_transport": return_transport.report(built.image.return_bindings),
                  "harness_sha256": harness_digest(), "input_sha256": env.TARGET_SHA256,
                  "python": sys.version.split()[0], "toolchain": env.toolchain_identity(),
                  "build_commands": [[Path(a).name if os.sep in a or "/" in a else a for a in c] for c in built.log],
                  "min_coverage": opts.min_coverage, "difftest": result}
        run_dir.mkdir(parents=True, exist_ok=True)
        coordination.atomic_bytes(run_dir / f"fn_{va:08x}.S", text.encode("utf-8"))
        ledger.write_json(cached, report)
        ledger.write_json(index, {"key": key})
        return report


def explain(va: int, case_index: int | None, overrides: dict | None = None, *, window: int = 6) -> dict:
    opts = difftest.Options.for_function(va, overrides)
    with tempfile.TemporaryDirectory(prefix="cmre-") as tmp:
        lifted, text, built = build.lift_and_build(va, Path(tmp))
        if built is None:
            return {"status": "lift-blocked", "problems": lifted.problems}
        result = difftest.explain(va, (lifted.va, lifted.region_end), set(lifted.insns), built,
                                  _call_returns(lifted, built), opts, LIFTED_PROFILE, case_index,
                                  window=window)
        result['build'] = {
            'entry': f'{built.entry:#010x}',
            'sections': [{'name': section.name, 'va': f'{section.va:#010x}',
                          'end': f'{section.end:#010x}', 'writable': section.writable,
                          'executable': section.executable}
                         for section in built.image.sections],
            'return_transport': return_transport.report(built.image.return_bindings),
        }
        return result


def explain_c(va: int, case_index: int | None, overrides: dict | None = None, *, source: Path | None = None,
              profile: str | None = None, opt: str = "-O2", window: int = 6) -> dict:
    """One C case side by side (or the original's stop summary), under the function's C profile."""
    source = Path(source) if source is not None else env.C_DIR / f"fn_{va:08x}.c"
    profile = profile or c_profile(va)
    opts = difftest.Options.for_function(va, overrides, tier="c")
    opts.inputs = c_inputs(va)
    lifted = lift.discover(va)
    returns = _semantic_contract(va, source.read_text(encoding="utf-8"))[0] if profile == "semantic" else "int"
    with tempfile.TemporaryDirectory(prefix="cmre-") as tmp:
        try:
            built = build.build(source, lift.symbol(va), Path(tmp), opt)
        except build.BuildError as exc:
            return {"status": "build-failed", "detail": str(exc), "stderr": exc.stderr[-2000:]}
        return difftest.explain(va, (lifted.va, lifted.region_end), set(lifted.insns), built, {}, opts, profile,
                                case_index, window=window, returns=returns, indirect=_indirect_calls(lifted))


def _indirect_calls(lifted) -> dict:
    """Argument shapes of the original's indirect CALL/JMPs, for the semantic indirect stops."""
    labels = {i.target for i in lifted.insns.values()
              if i.target is not None and lifted.va <= i.target < lifted.region_end}
    labels |= {target for table in lifted.tables.values() for target in table.targets}
    return semantic.indirect_calls(((i.va, i.raw) for i in lifted.insns.values()), lifted.va, labels)


def infer_hints(va: int) -> dict:
    lifted = lift.discover(va)
    return difftest.infer(va, (lifted.va, lifted.region_end), difftest.Options.for_function(va))


@coordination.coordinated
def auto_hint(va: int) -> bool:
    """Write inferred hints if they add information. Returns True when changed."""
    current = difftest.load_hints(va)
    if current and not current.get("auto"):
        return False  # an agent owns this hints file
    new = infer_hints(va)
    if {k: v for k, v in new.items() if k != "auto"} == {k: v for k, v in current.items() if k != "auto"}:
        return False
    ledger.write_json(env.HINTS_DIR / f"fn_{va:08x}.json", new)
    return True


def c_dependencies(source: Path) -> dict:
    """Compiler-selected inputs for the bounded C lane, before any cache lookup.

    Only the translation unit and recovery/include are supported. Preprocessing
    observes macro includes and line continuations rather than guessing from C
    text. Both optimization modes can select different conditional includes.
    """
    source = source.resolve()
    include = (env.ROOT / "recovery/include").resolve()
    dependencies, preprocessed = {}, {}
    for opt in C_OPT_LEVELS:
        result = env.run_tool([env.tool("gcc"), "-m32", "-E", opt,
                               "-fno-asynchronous-unwind-tables", "-ffreestanding", "-fno-builtin",
                               f"-I{include}", str(source)])
        if result.returncode:
            raise build.BuildError("C dependency preprocessing failed", result.stderr)
        preprocessed[opt] = env.digest_bytes(result.stdout.encode("utf-8"))
        files = {source}
        for match in re.finditer(r'^#\s+\d+\s+("(?:[^"\\]|\\.)*")', result.stdout, re.MULTILINE):
            name = json.loads(match.group(1))
            if name.startswith("<") and name.endswith(">"):
                continue
            files.add(Path(name).resolve())
        for path in files:
            if path != source and not path.is_relative_to(include):
                raise ValueError(f"C lane includes only its source and recovery/include; unsupported dependency: {path}")
            dependencies[env.rel(path)] = env.digest(path)
    return {"files": dict(sorted(dependencies.items())), "preprocessed": preprocessed}


def jump_contract(va: int) -> dict | None:
    """Pin parent declarations independently of source/include marker spelling."""
    path = env.HINTS_DIR / f'fn_{va:08x}.json'
    if not path.exists() or not json.loads(path.read_text(encoding='utf-8')).get('lift_jump_tables'):
        return None
    return {'path': env.rel(path), 'sha256': env.digest(path)}


def x87_contract(va: int) -> dict | None:
    path = env.HINTS_DIR / f'fn_{va:08x}.json'
    if not path.exists() or not json.loads(path.read_text(encoding='utf-8')).get('original_x87'):
        return None
    return {'path': env.rel(path), 'sha256': env.digest(path)}


def _withdrawn_c_contract(record: dict) -> dict | None:
    """Inactive history preserves policy, never acceptance, after C withdrawal."""
    if 'c_retractions' not in record:
        return None
    history = record['c_retractions']
    try:
        entry = history[-1]
        contract = entry['validation_contract']
        valid = (isinstance(history, list) and entry['schema'] == 'cmre-c-retraction/1'
                 and contract['profile'] in C_PROFILES and contract['inputs'] in difftest.INPUT_POLICIES
                 and isinstance(contract['effective_options_sha256'], str)
                 and len(contract['effective_options_sha256']) == 64
                 and all(c in '0123456789abcdef' for c in contract['effective_options_sha256']))
        reference = entry['archive_manifest']
        relative = Path(reference)
        manifest_path = (env.ROOT / relative).resolve()
        archive_base = (env.ANALYSIS / 'cmre/c-retractions' / f"fn_{int(record['va'], 16):08x}").resolve()
        valid = (valid and isinstance(reference, str) and '\\' not in reference
                 and not relative.is_absolute() and '..' not in relative.parts
                 and relative.as_posix() == reference
                 and manifest_path.is_relative_to(env.ROOT.resolve())
                 and manifest_path.is_relative_to(archive_base)
                 and manifest_path.name == 'manifest.json')
        if not valid or env.digest(manifest_path) != entry['archive_manifest_sha256']:
            valid = False
        else:
            manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
            valid = (manifest.get('schema') == entry['schema']
                     and manifest.get('va') == record['va']
                     and manifest.get('validation_contract') == contract
                     and env.digest(manifest_path.parent / 'validation-options-before.json') == contract['effective_options_sha256'])
    except (KeyError, IndexError, TypeError, OSError, ValueError):
        valid = False
    if not valid:
        raise ValueError('Withdrawn C validation contract is missing or malformed or changed; preserve and review its original policy')
    return contract


def c_profile(va: int) -> str:
    """An accepted C record keeps the profile it was accepted under; new C uses C_PROFILE."""
    path = ledger.record_path(va)
    if path.exists():
        record = json.loads(path.read_text(encoding="utf-8"))
        if not record.get('c') and (contract := _withdrawn_c_contract(record)):
            return contract['profile']
        validation = (record.get("c") or {}).get("validation") or {}
        recorded = {v.get("profile") for v in validation.values()}
        if len(recorded) == 1 and recorded <= set(C_PROFILES):
            return recorded.pop()
    return C_PROFILE


def c_inputs(va: int) -> str:
    """An accepted C record keeps the input policy it was accepted under (v1 when
    unrecorded, before v2 existed); new C uses C_INPUTS."""
    path = ledger.record_path(va)
    if path.exists():
        record = json.loads(path.read_text(encoding="utf-8"))
        c = record.get("c")
        if not c and (contract := _withdrawn_c_contract(record)):
            return contract['inputs']
        if c:
            recorded = {v.get("inputs", "v1") for v in (c.get("validation") or {}).values()}
            if len(recorded) == 1 and recorded <= set(difftest.INPUT_POLICIES):
                return recorded.pop()
            return "v1"
    return C_INPUTS


def _semantic_contract(va: int, text: str) -> tuple[str, list[str]]:
    """Declared return kind, plus problems when callers observe bits it does not return."""
    try:
        returns = semantic.declared_return(text, va)
    except ValueError as exc:
        return "int", [str(exc)]
    problems = []
    if returns == "void" and (readers := semantic.eax_readers(va)):
        sites = " ".join(f"{s:#010x}" for s in readers[:6])
        problems.append(f"callers read EAX after the call ({sites}); declare a return value")
    if returns != "int64" and (readers := semantic.edx_readers(va)):
        sites = " ".join(f"{s:#010x}" for s in readers[:6])
        problems.append(f"callers read EDX after the call ({sites}); declare a 64-bit return")
    width = {"int8": 1, "int16": 2}.get(returns)
    if width and (readers := semantic.wide_eax_readers(va, width)):
        sites = " ".join(f"{s:#010x}" for s in readers[:6])
        problems.append(f"callers read EAX beyond the declared {8 * width}-bit return ({sites}); widen it")
    return returns, problems


def _cipow_calls(built) -> int:
    # A tail jump also transfers control into the bridge. Inspect the linked
    # image so preprocessor aliases cannot hide all source-level call sites.
    targets = {built.symbols[name] for name in ('_cm_x87_CIpow', '_cm_fn_00945ff0')
               if name in built.symbols}
    if not targets:
        return 0
    section = built.image.text
    bridge = built.symbols.get('_cm_x87_CIpow')
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.skipdata = True
    count = 0
    for address, _, mnemonic, operand in md.disasm_lite(section.raw, section.va):
        if mnemonic not in ('call', 'jmp') or not re.fullmatch(r'0x[0-9a-f]+', operand):
            continue
        target = int(operand, 16)
        # The supplied bridge itself loads two 80-bit operands and calls the
        # original helper once. Only hand/transformed caller sites need proof.
        if bridge is not None and bridge <= address < bridge + 14 and target != bridge:
            continue
        if target in targets:
            count += 1
    return count


def test_c(va: int, overrides: dict | None = None, fresh: bool = False, *, source: Path | None = None,
           profile: str | None = None) -> dict:
    source = Path(source) if source is not None else env.C_DIR / f"fn_{va:08x}.c"
    if not source.exists():
        raise SystemExit(f"No tier-2 source {env.rel(source)}; write it first (see docs/agent/PLAYBOOK.md)")
    profile = profile or c_profile(va)
    if profile not in C_PROFILES:
        raise SystemExit(f"Unknown C profile {profile}; use one of {', '.join(C_PROFILES)}")
    opts = difftest.Options.for_function(va, overrides, tier="c")
    opts.inputs = c_inputs(va)
    lifted = lift.discover(va)
    source_bytes = source.read_bytes()
    text = source_bytes.decode("utf-8")
    returns, contract_problems = _semantic_contract(va, text) if profile == "semantic" else ("int", [])
    contract = jump_contract(va)
    fp_contract = x87_contract(va)
    header = (env.ROOT / "recovery/include/cmre.h").read_text(encoding="utf-8")
    identity = execution_identity()
    try:
        dependencies = c_dependencies(source)
    except (build.BuildError, ValueError, OSError) as exc:
        return {"harness_version": HARNESS_VERSION, "va": f"{va:#010x}", "name": lifted.name,
                "tier": "c", "profile": profile, "status": "fail", "problems": [str(exc)],
                "builds": {"dependencies": {"status": "build-failed", "stderr": getattr(exc, "stderr", "")[-2000:]}}}
    key = _key(identity, text, header, env.rel(source), dependencies, opts.__dict__,
               profile if profile == "abi" else [profile, returns],
               C_OPT_LEVELS, contract, fp_contract)
    run_dir = env.RUNS / f"fn_{va:08x}" / f"c-{key}"
    cached = run_dir / "report.json"
    if cached.exists() and not fresh:
        return json.loads(cached.read_text(encoding="utf-8"))
    builds = {}
    status = "pass"
    with tempfile.TemporaryDirectory(prefix="cmre-") as tmp:
        for opt in C_OPT_LEVELS:
            work = Path(tmp) / opt.strip("-")
            work.mkdir()
            try:
                built = build.build(source, lift.symbol(va), work, opt)
                if any(binding.owner != va for binding in built.image.jump_bindings):
                    raise build.BuildError('single-function C validation excludes extra indexed-JMP owners; use selected composition')
                if fp_contract or built.image.x87_bindings:
                    try:
                        x87_transport.require_complete(built.image.x87_bindings, {va: lifted})
                    except ValueError as exc:
                        raise build.BuildError(str(exc)) from exc
            except build.BuildError as exc:
                builds[opt] = {"status": "build-failed", "detail": str(exc), "stderr": exc.stderr[-2000:]}
                status = "fail"
                continue
            result = difftest.run(va, (lifted.va, lifted.region_end), set(lifted.insns), built, {}, opts, profile,
                                  data_spans=_data_spans(lifted), returns=returns, indirect=_indirect_calls(lifted))
            ok = (not result["mismatches"] and result["inconclusive"] < opts.cases / 2
                  and result["coverage"] >= opts.min_coverage
                  and result["code_as_data_cases"] <= difftest.CODE_AS_DATA_LIMIT * opts.cases)
            builds[opt] = {"status": "pass" if ok else "fail", "difftest": result,
                           "return_transport": return_transport.report(built.image.return_bindings),
                           "jump_transport": jump_transport.report(built.image.jump_bindings),
                           "x87_transport": x87_transport.report(built.image.x87_bindings),
                           "cipow_calls": _cipow_calls(built)}
            if not ok:
                status = "fail"
    if contract_problems:
        status = "fail"
    report = {"harness_version": HARNESS_VERSION, "va": f"{va:#010x}", "name": lifted.name, "tier": "c", "execution_identity": identity,
              "profile": profile, **({"returns": returns} if profile == "semantic" else {}),
              "problems": contract_problems, "status": status, "key": key, "source": env.rel(source), "source_sha256": env.digest_bytes(source_bytes),
              "harness_sha256": harness_digest(), "input_sha256": env.TARGET_SHA256,
              "toolchain": env.toolchain_identity(), "builds": builds,
              "jump_contract": contract,
              "x87_contract": fp_contract,
              "ftol_sites": ftol_sites(lifted),
              "effective_options_sha256": acceptance_request.digest_json(opts.__dict__),
              "source_dependencies": dependencies["files"], "preprocessed_sha256": dependencies["preprocessed"]}
    run_dir.mkdir(parents=True, exist_ok=True)
    ledger.write_json(cached, report)
    return report


def ftol_sites(lifted) -> list[str]:
    """Original call/tail-jump sites of the __ftol helper (ADR 0006 condition 4 census)."""
    return [f"{ins.va:#010x}" for va, ins in sorted(lifted.insns.items())
            if ins.target == operand_evidence.FTOL_HELPER and ins.kind in ("call", "tail_jmp")]


def _digests(report: dict) -> list[str]:
    if report["tier"] == "c":
        return [b.get("difftest", {}).get("result_digest") for _, b in sorted(report["builds"].items())]
    return [report["difftest"]["result_digest"]]


@coordination.coordinated
def accept(va: int, tier: str = "lifted", *, refresh: bool = False, profile: str | None = None) -> tuple[bool, dict]:
    return _accept(va, tier, refresh=refresh, profile=profile, request=None)


@coordination.coordinated
def accept_farm(va: int, request_path: Path) -> tuple[bool, dict]:
    """Consume a mandatory farm request inside the same lease as the ledger write."""
    try:
        request = acceptance_request.FarmAcceptance.load(request_path)
        if request.va != va:
            raise ValueError('Farm acceptance request names a different function')
        request.check_inputs()
    except (OSError, ValueError, KeyError, TypeError) as exc:
        return False, {"status": "acceptance-input-drift", "detail": str(exc)}
    return _accept(va, "c", refresh=False, profile=None, request=request)


def _accept(va: int, tier: str, *, refresh: bool, profile: str | None,
            request: acceptance_request.FarmAcceptance | None) -> tuple[bool, dict]:
    path = ledger.record_path(va)
    existing = json.loads(path.read_text(encoding="utf-8")) if path.exists() else None
    if tier == "lifted" and refresh and existing and existing.get("c"):
        return False, {"status": "has-c-tier", "note": "refresh must preserve and revalidate the existing C tier"}
    if tier == "lifted" and existing and not refresh:
        return True, {"status": "already-accepted", "record": ledger.record_path(va).name}
    if va in {e.va for e in ledger.legacy_entries()}:
        return True, {"status": "legacy-recorded", "note": "recovered by a legacy hand-built ledger"}
    if tier == "c" and not existing:
        return False, {"status": "needs-lifted", "note": "accept the lifted tier first"}
    initial_options = difftest.Options.for_function(va, tier=tier).__dict__
    profile = (profile or c_profile(va)) if tier == "c" else None
    first = test_lifted(va) if tier == "lifted" else test_c(va, profile=profile)
    if first["status"] != "pass":
        return False, first
    if refresh and existing and first["source_sha256"] != (existing.get(tier) or {}).get("source_sha256"):
        return False, {**first, "status": "source-drift"}
    hint_path = env.HINTS_DIR / f"fn_{va:08x}.json"
    initial_hint = acceptance_request.optional_digest(acceptance_request.optional_bytes(hint_path))
    proof = None
    if request is not None:
        try:
            request.check_gate(first)
        except (OSError, ValueError, KeyError, TypeError) as exc:
            return False, {"status": "acceptance-input-drift", "detail": str(exc)}
    if tier == "c" and not refresh:
        proof, blocker = _operand_proof(va, first, initial_hint, request)
        if blocker:
            return False, {"status": "operand-evidence-needed", "detail": blocker}
    # Fresh machines, same process: catches state leaking between runs. Cross-process
    # and hash-seed determinism is a harness property covered by the test suite.
    second = test_lifted(va, fresh=True) if tier == "lifted" else test_c(va, fresh=True, profile=profile)
    if second["status"] != "pass" or _digests(first) != _digests(second) or first["source_sha256"] != second["source_sha256"]:
        return False, {**first, "status": "nondeterministic",
                       "second": {"status": second["status"], "digests": _digests(second)}}
    if (first["key"] != second["key"] or first["execution_identity"] != second["execution_identity"]
            or second["execution_identity"] != execution_identity()
            or initial_options != difftest.Options.for_function(va, tier=tier).__dict__
            or initial_hint != acceptance_request.optional_digest(acceptance_request.optional_bytes(hint_path))
            or (tier == "c" and env.digest(env.C_DIR / f"fn_{va:08x}.c") != second["source_sha256"])):
        return False, {**first, "status": "input-drift"}
    if tier == "lifted":
        lifted = lift.discover(va)
        lo, hi = extent(lifted)
        from . import image

        src = env.LIFTED_DIR / f"fn_{va:08x}.S"
        run_src = env.RUNS / f"fn_{va:08x}" / first["key"] / src.name
        src.parent.mkdir(parents=True, exist_ok=True)
        coordination.atomic_bytes(src, run_src.read_bytes())
        inv = inventory.load()
        record = {
            "schema": ledger.SCHEMA,
            "va": f"{va:#010x}",
            "name": lifted.name,
            "input_sha256": env.TARGET_SHA256,
            "address_convention": env.ADDRESS_CONVENTION,
            "category": "application",
            "module_candidates": inv.modules.get(va, []),
            "extent": {"start": f"{lo:#010x}", "end": f"{hi:#010x}", "bytes": hi - lo,
                       "sha256": env.digest_bytes(image.primary().read(lo, hi - lo))},
            "status": {"identified": True, "decompiled": False, "compiled": True, "validated": True, "integrated": False},
            "boundaries": {"original_callees": first["lift"]["callees"], "imports": first["lift"]["imports"]},
            "hints": env.rel(env.HINTS_DIR / f"fn_{va:08x}.json") if (env.HINTS_DIR / f"fn_{va:08x}.json").exists() else None,
            "lifted": _tier_record(first, src),
        }
    else:
        record = dict(existing)
        record["status"] = {**record["status"], "decompiled": True}
        record["c"] = _tier_record(first, env.ROOT / first["source"])
        if proof is not None:
            record["c"]["operand_evidence"] = {
                "path": env.rel(proof.path), "sha256": proof.sha256, "status": "current",
            }
        elif refresh and (retained := existing["c"].get("operand_evidence")):
            record["c"]["operand_evidence"] = {**retained, "status": "historical"}
        record["hints"] = env.rel(hint_path) if hint_path.exists() else None
    if existing and existing.get('c_retractions'):
        record['c_retractions'] = existing['c_retractions']
    if request is not None:
        try:
            request.check_gate(second)
        except (OSError, ValueError, KeyError, TypeError) as exc:
            return False, {"status": "acceptance-input-drift", "detail": str(exc)}
    if proof is not None:
        blocker = _check_operand_proof(va, second, initial_hint, request, proof)
        if blocker:
            return False, {"status": "operand-evidence-needed", "detail": blocker}
    ledger.write_json(path, record)
    bp = ledger.blocked_path(va)
    if bp.exists():
        bp.unlink()
    return True, {"status": "accepted", "record": env.rel(path)}


def _check_operand_proof(va, gate, hints_sha, request, proof):
    try:
        proof.checked_bytes()
        raw = (env.C_DIR / f"fn_{va:08x}.c").read_bytes()
        if env.digest_bytes(raw) != gate['source_sha256']:
            return 'Canonical source changed after operand validation'
        return operand_evidence.verify_receipt(
            proof.path, raw, fn=f"fn_{va:08x}", job_number=request.job if request else None,
            hints_sha=hints_sha, current_execution=execution_identity, active_gate=gate,
        )
    except (OSError, ValueError, KeyError, TypeError) as exc:
        return f'Invalid retained operand evidence: {exc}'


def _operand_proof(va, baseline, hints_sha, request):
    fn = f"fn_{va:08x}"
    source = env.C_DIR / f"{fn}.c"
    raw = source.read_bytes()
    existing_proof = request.operand_proof if request else None
    if existing_proof is not None:
        return existing_proof, _check_operand_proof(va, baseline, hints_sha, request, existing_proof)
    if not operand_evidence.operand_proof_needed(raw, baseline):
        return None, None
    if request is not None and (operand_evidence.bridge_call_sites(raw.decode('utf-8'))
                                or any(b.get('cipow_calls') for b in baseline['builds'].values())
                                or operand_evidence.ftol_mutants_required(raw.decode('utf-8'), baseline)):
        return None, 'A bridge or __ftol candidate from the farm requires its retained operand proof'
    path = env.RUNS / fn / f"c-{baseline['key']}" / 'operands' / str(time.time_ns()) / 'summary.json'

    def check_inputs():
        if source.read_bytes() != raw:
            return 'canonical source changed during operand validation'
        hint = acceptance_request.optional_bytes(env.HINTS_DIR / f"{fn}.json")
        if acceptance_request.optional_digest(hint) != hints_sha:
            return 'canonical hints changed during operand validation'
        return None

    blocker = operand_evidence.generate(
        path, fn, None, raw, baseline, hints_sha,
        gate=lambda candidate: test_c(va, source=candidate, profile=baseline['profile']),
        check_inputs=check_inputs, report_event=lambda status: None,
    )
    if blocker or not path.is_file():
        return None, blocker
    proof = acceptance_request.Artifact(path, env.digest(path))
    return proof, _check_operand_proof(va, baseline, hints_sha, request, proof)


def accept_with_inference(va: int, rounds: int = 2) -> tuple[bool, dict]:
    """accept(); on low coverage, infer pointer hints (deterministic) and retry."""
    ok, report = accept(va)
    for _ in range(rounds):
        if ok or report.get("status") != "low-coverage" or not auto_hint(va):
            break
        ok, report = accept(va)
    return ok, report


def _tier_record(report: dict, source: Path) -> dict:
    out = {"source": env.rel(source), "source_sha256": env.digest(source),
           "harness_version": report["harness_version"], "harness_sha256": report["harness_sha256"],
           "toolchain": {k: v["version"] for k, v in report["toolchain"].items()}, "run_key": report["key"]}
    if report["tier"] == "lifted":
        d = report["difftest"]
        out["validation"] = {k: d[k] for k in ("profile", "cases", "seed", "budget", "outcomes", "coverage",
                                                "import_calls", "code_as_data_cases", "result_digest")}
        out["byte_fallbacks"] = report["lift"]["byte_fallbacks"]
    else:
        out["source_dependencies"] = report["source_dependencies"]
        out["preprocessed_sha256"] = report["preprocessed_sha256"]
        out["validation"] = {opt: {k: b["difftest"][k] for k in ("profile", "returns", "inputs", "cases", "seed", "outcomes",
                                                                  "coverage", "inconclusive", "stack_layout_cases",
                                                                  "stack_layout_examples", "result_digest")
                                   if k in b["difftest"]}
                             for opt, b in report["builds"].items()}
        if any(b.get('return_transport') for b in report['builds'].values()):
            out['return_transport'] = {opt: b['return_transport'] for opt, b in report['builds'].items()}
        if any(b.get('jump_transport') for b in report['builds'].values()):
            out['jump_transport'] = {opt: b['jump_transport'] for opt, b in report['builds'].items()}
            out['jump_contract'] = report['jump_contract']
        if any(b.get('x87_transport') for b in report['builds'].values()):
            out['x87_transport'] = {opt: b['x87_transport'] for opt, b in report['builds'].items()}
            out['x87_contract'] = report['x87_contract']
            out['fp_registers_compared'] = {opt: b['difftest']['fp_registers_compared']
                                           for opt, b in report['builds'].items()}
    return out


@coordination.coordinated
def block(va: int, reason: str, report: dict | None = None, by: str = "agent") -> Path:
    path = ledger.blocked_path(va)
    record = {"va": f"{va:#010x}", "reason": reason, "by": by}
    if report:
        record["status"] = report.get("status")
        record["problems"] = report.get("problems", [])[:10]
        d = report.get("difftest")
        if d:
            record["coverage"] = d["coverage"]
            record["mismatches"] = d["mismatches"][:3]
    ledger.write_json(path, record)
    return path
