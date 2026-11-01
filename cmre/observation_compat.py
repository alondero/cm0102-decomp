"""Fail-closed source proof for one host read-buffer refactor.

This audit does not grant acceptance or relabel old validation identities.
It recognizes an exact transformation, not arbitrary 'instrumentation changes'.
"""
import ast
import inspect
import textwrap


class Scope(set):
    """Explicit proof scope; acceptance metadata is provenance, not guest input."""
    observation = True


def same_inputs(before, after):
    if not isinstance(before, dict) or not isinstance(after, dict):
        return False
    required = {'options', 'source', 'source_path', 'profile', 'optimizations', 'boundary'}
    if not required <= before.keys() or not required <= after.keys():
        return False
    return ({k: v for k, v in before.items() if k != 'accepted'} ==
            {k: v for k, v in after.items() if k != 'accepted'})


POOL = '''self._read_buffers = {}
for size, word in ((1, ctypes.c_uint8), (2, ctypes.c_uint16),
                   (4, ctypes.c_uint32), (8, ctypes.c_uint64)):
    buffer = ctypes.create_string_buffer(size)
    self._read_buffers[size] = buffer, word.__ctype_le__.from_buffer(buffer)'''

READER = '''def _read_value(self, address: int, size: int) -> int:
    storage = self._read_buffers.get(size)
    if storage is None:
        return int.from_bytes(self.uc.mem_read(address, size), "little")
    buffer, word = storage
    status = uclib.uc_mem_read(self.uc._uch, address, buffer, size)
    if status != unicorn.UC_ERR_OK:
        raise unicorn.UcError(status, address, size)
    return word.value'''

PUBLIC_READ_BODY = '''data = ctypes.create_string_buffer(size)
status = uclib.uc_mem_read(self._uch, address, data, size)
if status != uc.UC_ERR_OK:
    raise UcError(status, address, size)
return bytearray(data)'''


def _dump(value):
    if isinstance(value, list):
        return [_dump(v) for v in value]
    return ast.dump(value, include_attributes=False)


def check_public_binding():
    """Require the native read, error and copy contract used by this proof."""
    import unicorn
    source = textwrap.dedent(inspect.getsource(unicorn.Uc.mem_read))
    body = ast.parse(source).body[0].body
    if body and isinstance(body[0], ast.Expr) and isinstance(body[0].value, ast.Constant):
        body = body[1:]
    if _dump(body) != _dump(ast.parse(PUBLIC_READ_BODY).body):
        raise ValueError('Unknown Unicorn public read contract; storage proof unavailable')
    return source


def check_sources(before: str, after: str) -> None:
    """Reverse only the exact reviewed change, then compare the entire module."""
    original, candidate = ast.parse(before), ast.parse(after)
    if any(isinstance(n, ast.Name) and n.id in {'ctypes', 'uclib'} for n in ast.walk(original)):
        raise ValueError('Storage imports would change existing name resolution')
    if any(isinstance(n, ast.Attribute) and n.attr in {'_read_buffers', '_read_value'}
           for n in ast.walk(original)):
        raise ValueError('Original observer already uses the reserved storage/helper names')
    imports = ast.parse('import ctypes\nfrom unicorn.unicorn_py3.unicorn import uclib').body
    for node in imports:
        matches = [n for n in candidate.body if _dump(n) == _dump(node)]
        if len(matches) != 1:
            raise ValueError('Expected exactly the two storage imports')
        candidate.body.remove(matches[0])
    machines = [n for n in candidate.body if isinstance(n, ast.ClassDef) and n.name == 'Machine']
    if len(machines) != 1:
        raise ValueError('Unknown machine class')
    machine = machines[0]
    readers = [n for n in machine.body if isinstance(n, ast.FunctionDef) and n.name == '_read_value']
    if len(readers) != 1 or _dump(readers[0]) != _dump(ast.parse(READER).body[0]):
        raise ValueError('Native read, width, endian, fallback or error behavior differs')
    machine.body.remove(readers[0])
    init = next(n for n in machine.body if isinstance(n, ast.FunctionDef) and n.name == '__init__')
    pool = ast.parse(POOL).body
    matches = [i for i in range(len(init.body) - len(pool) + 1)
               if _dump(init.body[i:i + len(pool)]) == _dump(pool)]
    if len(matches) != 1:
        raise ValueError('Unknown host storage initialization')
    suffix = init.body[matches[0] + len(pool):]
    if any(isinstance(n, ast.Name) and isinstance(n.ctx, ast.Load)
           and n.id in {'size', 'word', 'buffer'} for stmt in suffix for n in ast.walk(stmt)):
        raise ValueError('Storage locals would overwrite a live constructor value')
    if any(isinstance(n, ast.Call) and isinstance(n.func, ast.Name)
           and n.func.id in {'locals', 'eval', 'exec'} for n in ast.walk(init)):
        raise ValueError('Reflective constructor cannot establish storage equivalence')
    del init.body[matches[0]:matches[0] + len(pool)]
    access = next(n for n in machine.body if isinstance(n, ast.FunctionDef) and n.name == '_access')
    new = ast.parse('observed = self._read_value(address, size)').body[0]
    old = ast.parse('observed = int.from_bytes(uc.mem_read(address, size), "little")').body[0]
    matches = [n for n in ast.walk(access) if _dump(n) == _dump(new)]
    if len(matches) != 1:
        raise ValueError('Unknown observation call')
    class Restore(ast.NodeTransformer):
        def visit_Assign(self, node):
            return old if node is matches[0] else self.generic_visit(node)
    Restore().visit(access)
    if _dump(original) != _dump(candidate):
        raise ValueError('Additional execution, observation, or policy changes; proof rejected')


SCHEMA = 'cmre-observation-compatibility/1'


def _execution(identity):
    # Planner code is local orchestration, pinned by the new receipt identity.
    # Workspace, build environment, dispatch sources and tools still must match.
    return {k: v for k, v in identity.items() if k not in {'harness', 'components', 'planner'}}


def _proof(before, after, source):
    from . import env
    from pathlib import Path
    if _execution(before) != _execution(after):
        raise ValueError('Execution environment or policy changed')
    old, new = before['components'], after['components']
    if set(old) != set(new) or {k for k in old if old[k] != new[k]} != {'emu.py'}:
        raise ValueError('Observation proof covers only the exact emu.py storage change')
    current = Path(__file__).with_name('emu.py').read_bytes()
    if env.digest_bytes(source.encode()) != old['emu.py'] or env.digest_bytes(current) != new['emu.py']:
        raise ValueError('Observer source does not match both recorded identities')
    check_sources(source, current.decode('utf-8'))
    return check_public_binding()


def _check_comparison(report, before, after, prior):
    from . import validation as v
    if (report.get('state') != 'finished' or not report.get('cases')
            or report.get('old_observer_sha256') != before['components']['emu.py']
            or report.get('new_observer_sha256') != after['components']['emu.py']
            or report.get('target_sha256') != after['target']
            or {r.get('artifact') for r in report['cases']} != {'original', '-O0', '-O2'}
            or not all(r.get('all_Result_fields_equal') is True and r.get('fp_equal') is True
                       for r in report['cases'])):
        raise ValueError('Missing or mismatched old/new real-case equivalence evidence')
    if not any(old.get('ok') and old.get('dependencies', {}).get('source') == report.get('source_sha256')
               and old['dependencies'].get('options') == v.compact_options(report.get('options', {}))
               for old in prior.get('results', {}).values()):
        raise ValueError('Comparison does not use retained source, cases, budget and input policy')


def _regression_files(identity):
    # The storage proof is independent of which C source/fixture a tier tests.
    # Pin its Python implementation and regression code. Tier inputs are checked
    # separately by the planner, allowing changed tiers to remain pending.
    return {name: sha for name, sha in identity.get('files', {}).items()
            if name.endswith('.py') and name.startswith(('cmre/', 'scripts/', 'tests/'))
            or name == 'pytest.ini'}


def _regression_environment(identity):
    # Other projects can install unrelated packages in the shared interpreter.
    # Pin the proof's engine/test dependencies, plus Python, tools and variables.
    required = {'unicorn', 'capstone', 'pefile', 'pytest', 'pluggy', 'packaging', 'iniconfig'}
    return {**{k: val for k, val in identity.items() if k not in {'files', 'packages'}},
            'packages': [p for p in identity.get('packages', []) if p[0].lower() in required]}


def create(before_path, after_path, regression, reviewer, reason, output, observer_before, comparison):
    from pathlib import Path
    from . import coordination, env, ledger, session_evidence, validation as v
    before, plan = v._load_results(before_path), v.read(after_path)
    current = v.global_identity()
    if not reviewer.strip() or not reason.strip() or before.get('identity_drift'):
        raise ValueError('Reviewer, justification and non-drifting prior evidence required')
    if before.get('schema') != v.SCHEMA or plan.get('schema') != v.SCHEMA:
        raise ValueError('Dependency-aware receipts required')
    if {k: val for k, val in plan['identity'].items() if k != 'planner'} != {
            k: val for k, val in current.items() if k != 'planner'}:
        raise ValueError('Plan execution identity is stale')
    source = observer_before.read_bytes().decode('utf-8')
    binding = _proof(before['identity'], current, source)
    evidence = v.read(comparison)
    _check_comparison(evidence, before['identity'], current, before)
    regression_identity = session_evidence.identity(env.ROOT)
    recorded = v.read(regression)
    witnesses = [s.get('inputs_before', {}) for s in recorded.get('steps', [])
                 if s.get('command') == ['py', '-3.13', '-m', 'pytest']]
    if not witnesses:
        raise ValueError('No recorded full pytest step in regression receipt')
    witness = witnesses[-1]
    tests = (session_evidence.successful_step(regression, witness)
             if _regression_files(witness) == _regression_files(regression_identity)
             and _regression_environment(witness) == _regression_environment(regression_identity) else None)
    if not tests:
        raise ValueError('Matching completed pytest evidence including proof regressions required')
    entries = ledger.cmre_entries()
    selection = [f'{e.va:08x}' for e in entries]
    tiers = {str(v.TierKey(e.va, t)) for e in entries
             for t in (('lifted', 'c') if e.record.get('c') else ('lifted',))}
    if before['functions'] != selection or plan['functions'] != selection or set(plan['items']) != tiers:
        raise ValueError('Proof review must cover the complete current inventory')
    scope = {}
    for key, item in plan['items'].items():
        old = before['results'].get(key, {})
        try:
            actual = v.snapshot(item['va'], item['tier'], item['dependencies'])
        except (ValueError, OSError, v.recover.build.BuildError):
            continue
        if old.get('ok') and same_inputs(old.get('dependencies'), actual) and same_inputs(item['dependencies'], actual):
            scope[key] = v.digest(old['dependencies'])
    hashes = {p.name: env.digest(p) for p in (Path(__file__), Path(__file__).with_name('validation_compat.py'))}
    result = {'schema': SCHEMA, 'before': before['identity'], 'after': current,
              'before_receipt': str(before_path.resolve()), 'before_sha256': env.digest(before_path),
              'prior_results_sha256': v.digest(before['results']),
              'after_plan': str(after_path.resolve()), 'after_sha256': env.digest(after_path),
              'observer_before_source': source, 'public_binding_sha256': env.digest_bytes(binding.encode()),
              'comparison': str(comparison.resolve()), 'comparison_sha256': env.digest(comparison),
              'checker_sha256': hashes, 'regressions': tests, 'selection': selection,
              'proof_files': _regression_files(regression_identity),
              'scope': scope, 'reviewer': reviewer, 'reason': reason,
              'limitations': 'Exact host-storage transformation only. Original outcomes, identities and origins retained; no new execution or acceptance.'}
    coordination.atomic_json(output, result)
    return result


def check(data, prior, identity):
    from pathlib import Path
    from . import env, ledger, session_evidence, validation as v
    if (data['before'] != prior.get('identity') or data['after'] != identity
            or prior.get('identity_drift') or not data.get('reviewer') or not data.get('reason')):
        raise ValueError('Observation receipt does not match both identities')
    binding = _proof(data['before'], identity, data['observer_before_source'])
    if env.digest_bytes(binding.encode()) != data['public_binding_sha256']:
        raise ValueError('Native public binding changed')
    for name, sha in data['checker_sha256'].items():
        if name not in {'observation_compat.py', 'validation_compat.py'} or env.digest(Path(__file__).with_name(name)) != sha:
            raise ValueError('Compatibility checker changed')
    if set(data['checker_sha256']) != {'observation_compat.py', 'validation_compat.py'}:
        raise ValueError('Incomplete checker identity')
    for field in ('before_receipt', 'after_plan', 'comparison'):
        sha = data[{'before_receipt': 'before_sha256', 'after_plan': 'after_sha256',
                    'comparison': 'comparison_sha256'}[field]]
        if env.digest(Path(data[field])) != sha:
            raise ValueError('Compatibility evidence changed')
    _check_comparison(v.read(Path(data['comparison'])), data['before'], identity, prior)
    if v.digest(prior.get('results', {})) != data['prior_results_sha256']:
        raise ValueError('Original results changed')
    if data['selection'] != [f'{e.va:08x}' for e in ledger.cmre_entries()]:
        raise ValueError('Compatibility inventory changed')
    regression = Path(data['regressions']['receipt'])
    witness = v.read(regression)['steps'][data['regressions'].get('step', 0)]['inputs_before']
    current_tests = session_evidence.identity(env.ROOT)
    if (_regression_files(current_tests) != data['proof_files']
            or _regression_environment(witness) != _regression_environment(current_tests)
            or session_evidence.successful_step(regression, witness) != data['regressions']):
        raise ValueError('Compatibility regression evidence changed')
    for key, sha in data['scope'].items():
        old = prior.get('results', {}).get(key, {})
        if not old.get('ok') or v.digest(old.get('dependencies')) != sha:
            raise ValueError('Scope does not match prior successful evidence')
    return Scope(data['scope'])
