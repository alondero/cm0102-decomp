"""Sensitivity diagnostics; acceptance and its differential gate are unchanged."""
from __future__ import annotations

import re
import json
import tempfile
from contextlib import nullcontext
from pathlib import Path


CONTROL_SCHEMA = 'cmre-control-plan/1'
_OPTS = ('-O0', '-O2')
CONTROL_FIELDS = ('outcome', 'detail', 'regs', 'events', 'memory')
_EXIT_KIND = 'API gate status: pass=0, fail=1, exception=2'
_TRACE = {'status': 'trace_missing', 'complete': False,
          'reason': 'Current supported gate retains summaries; explain slices are not full Result traces.'}


def _json(path):
    raw = Path(path).read_bytes()
    try:
        if raw.startswith(b'\xef\xbb\xbf') or b'\x00' in raw:
            raise ValueError('BOM or NUL')
        def unique(pairs):
            result = {}
            for key, value in pairs:
                if key in result:
                    raise ValueError('duplicate JSON key')
                result[key] = value
            return result
        return json.loads(raw.decode('utf-8'), object_pairs_hook=unique,
                          parse_constant=lambda x: (_ for _ in ()).throw(ValueError(x)))
    except (UnicodeError, ValueError) as exc:
        raise ValueError(f'Strict UTF-8 JSON required: {path}: {exc}') from exc


def _fields(value, fields, label, *, optional=()):
    if not isinstance(value, dict) or not set(fields) <= set(value) or set(value) - set(fields) - set(optional):
        raise ValueError(f'Invalid {label} fields; expected {sorted(fields)}')


def _pin(path):
    from . import env
    path = Path(path).resolve()
    return {'path': str(path), 'sha256': env.digest(path)}


def _check_pin(pin, *, absent=False):
    _fields(pin, ('path', 'sha256'), 'artifact pin')
    if not isinstance(pin['path'], str) or not Path(pin['path']).is_absolute():
        raise ValueError('artifact path must be absolute')
    path = Path(pin['path'])
    if absent and pin['sha256'] is None and not path.exists():
        return path
    if (not isinstance(pin['sha256'], str) or not re.fullmatch('[0-9a-f]{64}', pin['sha256'])
            or not path.is_file() or _pin(path)['sha256'] != pin['sha256']):
        raise ValueError(f'Missing or changed artifact: {path}')
    return path


def control_identity(va, source):
    """Capture pins for a plan; no gate, queue update, or acceptance occurs."""
    import os
    from . import difftest, recover, validation, env
    if recover.c_profile(va) != 'semantic':
        raise ValueError('Control plans require the accepted semantic profile')
    opts = difftest.Options.for_function(va, tier='c')
    opts.inputs = recover.c_inputs(va)
    if tuple(recover.C_OPT_LEVELS) != _OPTS:
        raise ValueError('Control plans require the normal O0/O2 gate')
    return {'execution_identity': recover.execution_identity(), 'options': opts.__dict__,
            'dependencies': recover.c_dependencies(Path(source)),
            'environment': {k: v for k, v in os.environ.items()
                            if k in validation.BUILD_ENVIRONMENT or k == 'PATH'
                            or k.startswith(('CMRE_', 'PYTHON'))},
            'controller_sha256': env.digest(Path(__file__)),
            'hints_path': str((env.HINTS_DIR / f'fn_{va:08x}.json').resolve()),
            'profile': 'semantic', 'optimizations': list(_OPTS), 'cases': opts.cases}


def _control_cases(identity):
    cases = identity.get('cases') if isinstance(identity, dict) else None
    options = identity.get('options') if isinstance(identity, dict) else None
    if (type(cases) is not int or not 1 <= cases <= 1_000_000_000
            or not isinstance(options, dict) or type(options.get('cases')) is not int
            or options['cases'] != cases):
        raise ValueError('Plan must pin consistent normal case counts in its execution identity')
    return cases


def _control_case_numbers(identity):
    """Gate case numbers a witness may name: the pinned random cases, then one per pinned
    hinted input fixture or capture, which the gate runs as case numbers cases + i."""
    cases = _control_cases(identity)
    hints = identity['options'].get('hints', {})
    inputs = [hints.get(key, []) if isinstance(hints, dict) else None for key in ('fixtures', 'captures')]
    if any(not isinstance(rows, list) for rows in inputs):
        raise ValueError('Plan must pin its hinted inputs as lists in its execution identity')
    return cases + sum(len(rows) for rows in inputs)


def _plan(path):
    plan = _json(path)
    _fields(plan, ('schema', 'va', 'source', 'hints', 'identity', 'controls'), 'plan', optional=('required_stages',))
    required = plan.get('required_stages', ['controls'])
    if not isinstance(required, list) or any(s not in ('controls', 'diagnostic-pe') for s in required):
        raise ValueError('Unsupported required stage; only controls and diagnostic-pe are available')
    if plan['schema'] != CONTROL_SCHEMA or not isinstance(plan['va'], str) or not re.fullmatch('0x[0-9a-f]{8}', plan['va']):
        raise ValueError('Unsupported control plan schema or VA')
    source = _check_pin(plan['source'])
    _check_pin(plan['hints'], absent=True)
    _control_cases(plan['identity'])
    if 'hints_path' in plan['identity'] and Path(plan['identity']['hints_path']).resolve() != Path(plan['hints']['path']).resolve():
        raise ValueError('Plan hints differ from the execution hint path')
    raw = source.read_bytes()
    try:
        original = raw.decode('utf-8')
        if original.startswith('\ufeff') or '\x00' in original:
            raise ValueError('BOM or NUL')
    except (UnicodeError, ValueError) as exc:
        raise ValueError('Source must be strict UTF-8') from exc
    if not isinstance(plan['controls'], list) or not plan['controls']:
        raise ValueError('Plan requires a finite named control list')
    names, texts = {'baseline'}, {'baseline': raw}
    for row in plan['controls']:
        mode = 'transform' if isinstance(row, dict) and 'transform' in row else 'source'
        _fields(row, ('name', mode, 'expect'), 'control')
        name = row['name']
        if not isinstance(name, str) or not re.fullmatch('[a-zA-Z0-9][a-zA-Z0-9_-]{0,63}', name) or name in names:
            raise ValueError('Control names must be unique safe path components')
        names.add(name)
        expect = row['expect']
        if not isinstance(expect, dict) or expect.get('kind') not in ('mismatch', 'equivalent'):
            raise ValueError('Control expectation must be mismatch or equivalent')
        if expect['kind'] == 'mismatch':
            _fields(expect, ('kind', 'field'), 'mismatch expectation')
            if expect['field'] not in CONTROL_FIELDS:
                raise ValueError(f'Unsupported observable field; choose one of {CONTROL_FIELDS}')
        else:
            _fields(expect, ('kind', 'proof'), 'equivalence expectation')
            _check_pin(expect['proof'])
        if mode == 'transform':
            transform = row[mode]
            _fields(transform, ('old', 'new', 'count'), 'exact transform')
            old, new, count = (transform[k] for k in ('old', 'new', 'count'))
            if (not isinstance(old, str) or not old or not isinstance(new, str) or new == old
                    or type(count) is not int or count < 1 or original.count(old) != count):
                raise ValueError('Exact transform occurrence count differs')
            texts[name] = original.replace(old, new).encode('utf-8')
        else:
            private = _check_pin(row['source'])
            if private.samefile(source):
                raise ValueError('A frozen private control must differ from the baseline')
            texts[name] = private.read_bytes()
            if texts[name] == raw:
                raise ValueError('Control bytes must differ from the baseline')
        text = texts[name].decode('utf-8')
        if text.startswith('\ufeff') or '\x00' in text:
            raise ValueError('Control source must be strict UTF-8')
    return plan, texts


def classify_control(report, expect, *, cases=None):
    """Require an observed mismatch with a numeric witness in each build."""
    if cases is None:
        from .difftest import DEFAULTS
        cases = DEFAULTS['cases']
    builds = report.get('builds', {})
    if (report.get('problems') or set(builds) != set(_OPTS)
            or any(builds[opt].get('status') not in ('pass', 'fail') for opt in _OPTS)):
        return 'unresolved', 'build-contract-or-report-error'
    if expect['kind'] == 'equivalent':
        if report.get('status') == 'pass' and all(builds[o]['status'] == 'pass' and not builds[o].get('difftest', {}).get('mismatches') for o in _OPTS):
            return 'expected-equivalent', None
        return 'unexpected-mismatch', 'equivalence-not-observed'
    witness_cases = []
    for opt in _OPTS:
        mismatches = builds[opt].get('difftest', {}).get('mismatches', [])
        if any(m.get('kind') == 'budget-asymmetry' for m in mismatches):
            return 'unresolved', 'budget-asymmetry'
        witness_cases.append({m['case'] for m in mismatches if type(m.get('case')) is int
                      and 0 <= m['case'] < cases and m.get('field') == expect['field']
                      and m.get('kind') != 'budget-asymmetry'})
    if report.get('status') == 'fail' and all(builds[o]['status'] == 'fail' for o in _OPTS) and all(witness_cases):
        return 'killed', 'differential-mismatch-both-optimizations'
    if report.get('status') == 'pass' and all(builds[o]['status'] == 'pass' for o in _OPTS):
        return 'survivor', 'required-mismatch-not-observed'
    return 'unresolved', 'required-mismatch-not-observed-both-optimizations'


def _retain_control_build(va, source, directory):
    from . import build, lift
    result = {}
    for opt in _OPTS:
        work = directory / opt[1:]
        work.mkdir()
        built = build.build(source, lift.symbol(va), work, opt)
        result[opt] = {**_pin(work / (source.stem + '.exe')), 'commands': built.log}
    return result


def _execute_control_stage(va, candidate, directory, gate, retain_build, expected_identity, check):
    """The pending receipt is already durable when execution enters this function."""
    from . import coordination
    check()
    source_pin = _pin(candidate)
    report = gate(va, source=candidate, profile='semantic', fresh=True)
    coordination.atomic_json(directory / 'report.json', report)
    check()
    _validate_control_report(report, va, source_pin['sha256'], expected_identity)
    if _pin(candidate) != source_pin:
        raise ValueError('Private candidate changed during gate')
    builds = retain_build(va, candidate, directory)
    if set(builds) != set(_OPTS):
        raise ValueError('Missing retained optimization build')
    for row in builds.values():
        _check_pin({k: row[k] for k in ('path', 'sha256')})
    builds = {opt: {**row, 'role': 'diagnostic-rebuild', 'optimization': opt,
                    'source': _pin(directory / 'source.c'), 'execution_identity': expected_identity,
                    'gate_pe_retained': False} for opt, row in builds.items()}
    _validate_control_builds(builds, directory, _pin(directory / 'source.c'), expected_identity)
    coordination.atomic_json(directory / 'builds.json', builds)
    check()
    if _pin(candidate) != source_pin:
        raise ValueError('Private candidate changed during execution or diagnostic rebuild')
    return report, builds


def _validate_control_report(report, va, source_sha256, identity):
    """Validate the existing gate's recorded execution scope without changing it."""
    cases = _control_cases(identity)
    numbers = _control_case_numbers(identity)
    if (not isinstance(report, dict) or report.get('execution_identity') != identity['execution_identity']
            or report.get('source_sha256') != source_sha256 or report.get('profile') != 'semantic'
            or report.get('va') != f'{va:#010x}' or report.get('tier') != 'c'
            or not isinstance(report.get('key'), str) or not re.fullmatch('[0-9a-f]{16}', report['key'])
            or report.get('status') not in ('pass', 'fail') or not isinstance(report.get('problems'), list)
            or not isinstance(report.get('builds'), dict) or set(report['builds']) != set(_OPTS)):
        raise ValueError('Gate report identity or normal case scope differs')
    for row in report['builds'].values():
        result = row.get('difftest', {})
        if (row.get('status') not in ('pass', 'fail') or type(result.get('cases')) is not int
                or result['cases'] != cases or result.get('input_cases', 0) != numbers - cases
                or result.get('budget') != identity['options']['budget']
                or type(result.get('seed')) is not int or result['seed'] != identity['options']['seed']
                or result.get('inputs', 'v1') != identity['options']['inputs'] or result.get('profile') != 'semantic'
                or not isinstance(result.get('mismatches'), list)
                or any(not isinstance(m, dict) for m in result['mismatches'])):
            raise ValueError('Gate report optimization or effective options differ')


def _validate_control_builds(builds, directory, source, identity):
    if not isinstance(builds, dict) or set(builds) != set(_OPTS):
        raise ValueError('Missing retained diagnostic builds')
    for opt, row in builds.items():
        _fields(row, ('path', 'sha256', 'commands', 'role', 'optimization', 'source',
                      'execution_identity', 'gate_pe_retained'), 'diagnostic build')
        path = _check_pin({k: row[k] for k in ('path', 'sha256')})
        commands = row['commands']
        if (not path.resolve().is_relative_to(directory.resolve()) or row['source'] != source
                or row['execution_identity'] != identity or row['optimization'] != opt
                or row['role'] != 'diagnostic-rebuild' or row['gate_pe_retained'] is not False
                or not isinstance(commands, list) or not commands
                or any(not isinstance(c, list) or not c or any(not isinstance(a, str) for a in c) for c in commands)
                or not any(opt in c and str(directory.parent / 'candidate.c') in c for c in commands)):
            raise ValueError('Retained diagnostic build provenance differs')


def _control_outcome(report, expect, cases):
    if expect['kind'] == 'baseline':
        classification, reason = (('pass', None) if report['status'] == 'pass' and not report['problems'] and all(
            report['builds'][o]['status'] == 'pass' and not report['builds'][o]['difftest']['mismatches'] for o in _OPTS)
                                  else ('unresolved', 'baseline-failed'))
    else:
        classification, reason = classify_control(report, expect, cases=cases)
    witnesses = {opt: [m for m in report['builds'][opt]['difftest']['mismatches']
                       if type(m.get('case')) is int and 0 <= m['case'] < cases
                       and m.get('field') == expect.get('field') and m.get('kind') != 'budget-asymmetry'] for opt in _OPTS}
    return {'classification': classification, 'reason': reason, 'witnesses': witnesses,
            'gate_key': report['key'], 'exit_status': 0 if report['status'] == 'pass' else 1}


def _control_command(va, candidate):
    return {'api': 'recover.test_c', 'va': f'{va:#010x}', 'source': str(candidate), 'profile': 'semantic', 'fresh': True}


def _resume_controls(receipt, plan, texts, output, plan_sha256):
    """Reject missing evidence before any gate, and derive reused outcomes afresh."""
    from . import env
    _fields(receipt, ('schema', 'plan', 'plan_sha256', 'stages', 'readiness', 'independent_review',
                      'acceptance', 'trace', 'gate_pe_retained'), 'bundle', optional=('status', 'artifact_index'))
    if (receipt['schema'] != 'cmre-control-bundle/2' or receipt['plan'] != plan
            or receipt['plan_sha256'] != plan_sha256 or receipt['readiness'] != 'proposal'
            or receipt['independent_review'] != 'pending' or receipt['acceptance'] != 'pending'
            or receipt['trace'] != _TRACE or receipt['gate_pe_retained'] is not False):
        raise ValueError('Bundle schema, identity, or review state drift')
    if _json(output / 'plan.json') != plan:
        raise ValueError('Retained plan differs')
    rows = [{'name': 'baseline', 'expect': {'kind': 'baseline'}}] + plan['controls']
    stages = receipt['stages']
    if not isinstance(stages, dict) or list(stages) != [r['name'] for r in rows[:len(stages)]]:
        raise ValueError('Retained stage selection or order differs')
    common = ('schema', 'state', 'source', 'command', 'exit_kind', 'exit_status', 'identity', 'outcome',
              'classification', 'reason', 'artifacts')
    for row in rows[:len(stages)]:
        stage = stages[row['name']]
        if not isinstance(stage, dict) or stage.get('state') != 'terminal':
            raise ValueError('Retained pending stage requires investigation before another launch')
        error = stage.get('outcome') == 'error'
        _fields(stage, common + (('detail', 'error') if error else ('gate_key', 'report', 'builds', 'witnesses')), 'terminal stage')
        directory = output / row['name']
        source_path = _check_pin(stage['source'])
        if (stage['schema'] != 'cmre-control-stage/1' or stage['identity'] != plan['identity']
                or stage['command'] != _control_command(int(plan['va'], 16), output / 'candidate.c')
                or stage['exit_kind'] != _EXIT_KIND or source_path.resolve() != (directory / 'source.c').resolve()
                or stage['source']['sha256'] != env.digest_bytes(texts[row['name']])):
            raise ValueError('Retained stage source, identity, or command differs')
        manifest = [_pin(p) for p in sorted(directory.rglob('*')) if p.is_file()]
        if not manifest or stage['artifacts'] != manifest:
            raise ValueError('Retained artifact manifest missing or changed')
        if error:
            error_path = _check_pin(stage['error'])
            failure = _json(error_path)
            _fields(failure, ('class', 'detail', 'stderr'), 'retained error')
            if error_path.resolve() != (directory / 'error.json').resolve() or stage['exit_status'] != 2:
                raise ValueError('Retained error provenance differs')
            stage.update(classification='unresolved', reason=failure['class'], detail=failure['detail'])
        else:
            report_path = _check_pin(stage['report'])
            if stage['outcome'] != 'report' or report_path.resolve() != (directory / 'report.json').resolve():
                raise ValueError('Retained report provenance differs')
            report = _json(report_path)
            _validate_control_report(report, int(plan['va'], 16), stage['source']['sha256'], plan['identity'])
            _validate_control_builds(stage['builds'], directory, stage['source'], plan['identity'])
            if _json(directory / 'builds.json') != stage['builds']:
                raise ValueError('Retained build report differs')
            stage.update(_control_outcome(report, row['expect'], _control_case_numbers(plan['identity'])))
    if 'artifact_index' in receipt:
        index_path = _check_pin(receipt['artifact_index'])
        if (index_path.resolve() != (output / 'artifact-index.json').resolve()
                or _json(index_path) != _control_index(receipt, output)):
            raise ValueError('Retained artifact index differs')
    if ('status' in receipt) != ('artifact_index' in receipt):
        raise ValueError('Incomplete final receipt metadata')
    if 'status' in receipt and receipt['status'] != _control_status(receipt, len(rows)):
        raise ValueError('Retained controls status differs from evidence')


def _control_status(receipt, count):
    return 'controls-complete' if len(receipt['stages']) == count and all(
        s['classification'] in ('pass', 'killed', 'expected-equivalent') for s in receipt['stages'].values()) else 'incomplete'


def _control_index(receipt, output):
    return {'schema': 'cmre-control-artifacts/1', 'plan': _pin(output / 'plan.json'),
            'artifacts': [p for s in receipt['stages'].values() for p in s['artifacts']],
            'trace': receipt['trace'], 'gate_pe_retained': False,
            'independent_review': 'pending', 'acceptance': 'pending'}


def run_plan(plan_path, output, *, resume=False, gate=None, identity=None, fence=None, retain_build=None):
    """Execute selected controls only. Never update work state, accept, or clear holds.

    Resume reuses terminal stages after verifying every artifact. An interrupted
    pending stage requires explicit investigation and a new output directory.
    """
    from . import coordination, env, recover
    plan, texts = _plan(plan_path)
    plan_sha256 = env.digest(Path(plan_path))
    va, source = int(plan['va'], 16), Path(plan['source']['path'])
    identity = identity or control_identity
    output = Path(output).resolve()
    if output.exists() and not resume:
        raise ValueError('Output already exists; use resume or a new directory')
    def check():
        current_plan, current_texts = _plan(plan_path)
        if current_plan != plan or current_texts != texts or env.digest(Path(plan_path)) != plan_sha256:
            raise ValueError('Plan or control evidence drift')
        if identity(va, source) != plan['identity']:
            raise ValueError('Execution identity or environment drift')
    check()
    if fence is None:
        from .cli import _diagnostic_source
        fence = _diagnostic_source
    receipt_path = output / 'controller.json'
    candidate = output / 'candidate.c'
    with fence(va, candidate):
        check()
        if resume:
            receipt = _json(receipt_path)
            _resume_controls(receipt, plan, texts, output, plan_sha256)
        else:
            output.mkdir(parents=True)
            receipt = {'schema': 'cmre-control-bundle/2', 'plan': plan,
                       'plan_sha256': plan_sha256, 'stages': {},
                       'readiness': 'proposal', 'independent_review': 'pending', 'acceptance': 'pending',
                       'trace': dict(_TRACE), 'gate_pe_retained': False}
            coordination.atomic_json(output / 'plan.json', plan)
        rows = [{'name': 'baseline', 'expect': {'kind': 'baseline'}}] + plan['controls']
        for row in rows:
            name = row['name']
            if name in receipt['stages']:
                if name == 'baseline' and receipt['stages'][name]['classification'] != 'pass':
                    break
                continue
            check()
            directory = output / name
            directory.mkdir()
            coordination.atomic_bytes(candidate, texts[name])
            coordination.atomic_bytes(directory / 'source.c', texts[name])
            stage = {'schema': 'cmre-control-stage/1', 'state': 'pending', 'source': _pin(directory / 'source.c'),
                     'command': _control_command(va, candidate), 'identity': plan['identity'],
                     'exit_kind': _EXIT_KIND, 'exit_status': None}
            receipt['stages'][name] = stage
            coordination.atomic_json(receipt_path, receipt)
            try:
                report, builds = _execute_control_stage(va, candidate, directory, gate or recover.test_c,
                                                        retain_build or _retain_control_build, plan['identity'], check)
                stage.update(_control_outcome(report, row['expect'], _control_case_numbers(plan['identity'])))
                stage.update(outcome='report', report=_pin(directory / 'report.json'), builds=builds)
            except (Exception, SystemExit) as exc:
                stage.update(outcome='error', classification='unresolved', reason=type(exc).__name__, detail=str(exc), exit_status=2)
                coordination.atomic_json(directory / 'error.json', {'class': type(exc).__name__, 'detail': str(exc),
                                                                   'stderr': getattr(exc, 'stderr', '')})
                stage['error'] = _pin(directory / 'error.json')
            stage['state'] = 'terminal'
            stage['artifacts'] = [_pin(p) for p in sorted(directory.rglob('*')) if p.is_file()]
            coordination.atomic_json(receipt_path, receipt)
            if name == 'baseline' and stage['classification'] != 'pass':
                break
        check()
        receipt['status'] = _control_status(receipt, len(rows))
        index = _control_index(receipt, output)
        index_path = output / 'artifact-index.json'
        if index_path.exists():
            if _json(index_path) != index:
                raise ValueError('Immutable artifact index differs')
        else:
            coordination.atomic_json(index_path, index)
        receipt['artifact_index'] = _pin(index_path)
        coordination.atomic_json(receipt_path, receipt)
        return receipt

_TOKEN = re.compile(r'''(?P<directive>^[ \t]*\#(?:[^\n]*\\\n)*[^\n]*)|(?P<comment>/\*.*?\*/|//[^\n]*)|(?P<string>"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')|(?P<identifier>[A-Za-z_]\w*)|(?P<number>\b(?:0[xX][0-9a-fA-F]+|[0-9]+)(?:[uUlL]*)\b)|(?P<op><=|>=|==|!=|(?<![<>=!])[<>](?![<>=]))''', re.M | re.S)
_OPS = {'<': '<=', '<=': '<', '>': '>=', '>=': '>', '==': '!=', '!=': '=='}


def mutants(source):
    for token in _TOKEN.finditer(source):
        old = token.group()
        if token.lastgroup == 'op':
            new = _OPS[old]
        elif token.lastgroup == 'number':
            parts = re.fullmatch(r'(0[xX][0-9a-fA-F]+|[0-9]+)([uUlL]*)', old)
            value, suffix = parts.groups()
            new = (hex(int(value, 16) + 1) if value.lower().startswith('0x') else str(int(value) + 1)) + suffix
        else:
            continue
        yield {'line': source.count('\n', 0, token.start()) + 1, 'column': token.start() - source.rfind('\n', 0, token.start()),
               'old': old, 'new': new, 'source': source[:token.start()] + new + source[token.end():]}


def classify(report):
    if report.get('status') == 'pass':
        return 'survivors', None
    builds = report.get('builds', {})
    if any(b.get('status') == 'build-failed' for b in builds.values()):
        return 'invalid', 'build-failed'
    if report.get('problems'):
        return 'unresolved', 'contract-or-report-error'
    mismatches = [m for b in builds.values() for m in b.get('difftest', {}).get('mismatches', [])]
    if any(m.get('field') or m.get('kind') not in (None, 'budget-asymmetry') for m in mismatches):
        return 'killed', 'differential-mismatch'
    return 'unresolved', 'gate-failed-without-mismatch'


def run(va, source, gate=None, *, fence=None):
    from . import env, recover
    source = Path(source)
    parent = env.ANALYSIS / 'cmre/mutations'
    parent.mkdir(parents=True, exist_ok=True)
    tmp = tempfile.mkdtemp(prefix=f'fn_{va:08x}-', dir=parent)
    candidate = Path(tmp) / source.name
    with (fence(va, candidate) if fence else nullcontext(candidate)) as candidate:
        if gate is None and recover.c_profile(va) != 'semantic':
            raise ValueError('Mutation diagnostics require the accepted semantic profile')
        return _run_fenced(va, source, candidate, gate or recover.test_c, tmp)


def _run_fenced(va, source, candidate, gate, tmp):
    from . import env
    original = source.read_bytes().decode('utf-8')
    baseline_sha = env.digest_bytes(original.encode('utf-8'))
    result = {'va': f'{va:#010x}', 'baseline_sha256': baseline_sha,
              'survivors': [], 'killed': [], 'invalid': [], 'unresolved': [], 'directory': tmp}
    candidate.write_text(original, encoding='utf-8', newline='')
    baseline = gate(va, source=candidate, profile='semantic', fresh=True)
    (Path(tmp) / 'baseline.json').write_text(json.dumps(baseline, indent=2), encoding='utf-8')
    (Path(tmp) / 'baseline.c').write_text(original, encoding='utf-8', newline='')
    result.update(baseline_gate_key=baseline.get('key'),
                  baseline_report_path=str(Path(tmp) / 'baseline.json'),
                  baseline_source_path=str(Path(tmp) / 'baseline.c'))
    if baseline.get('status') != 'pass':
        raise ValueError('Original source did not pass the unchanged semantic gate in the diagnostic workspace')
    for index, row in enumerate(mutants(original)):
        text = row.pop('source')
        candidate.write_text(text, encoding='utf-8', newline='')
        saved_source = Path(tmp) / f'{index:04d}.c'
        saved_source.write_text(text, encoding='utf-8', newline='')
        try:
            report = gate(va, source=candidate, profile='semantic', fresh=True)
            category, error = classify(report)
        except (Exception, SystemExit) as exc:
            category, error = 'unresolved', type(exc).__name__
            report = {'status': 'error', 'error_class': error, 'detail': str(exc)}
        report_path = Path(tmp) / f'{index:04d}.json'
        report_path.write_text(json.dumps(report, indent=2), encoding='utf-8')
        row.update(baseline_sha256=baseline_sha, source_sha256=env.digest(saved_source),
                   source_path=str(saved_source), report_path=str(report_path),
                   gate_key=report.get('key'), error_class=error)
        result[category].append(row)
    return result
