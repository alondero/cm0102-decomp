"""Reviewed bounded dispatch and exact observer-storage compatibility; fail closed."""
from __future__ import annotations

import ast
from pathlib import Path

from . import coordination, env, ledger, session_evidence, validation


def _input_only(node: ast.FunctionDef) -> None:
    """Masked dispatch functions may bind locals, never patch comparison state."""
    for child in ast.walk(node):
        if isinstance(child, (ast.Global, ast.Nonlocal, ast.Import)):
            raise ValueError('Input dispatch has unknown global side effects; full sweep required')
        if isinstance(child, ast.ImportFrom) and (child.level != 1 or child.module not in {'lift', 'build', 'x87_transport'}):
            raise ValueError('Input dispatch imports an unknown execution dependency')
        targets = (child.targets if isinstance(child, ast.Assign) else
                   [child.target] if isinstance(child, (ast.AnnAssign, ast.AugAssign)) else [])
        if any(not isinstance(target, ast.Name) for target in targets):
            raise ValueError('Input dispatch writes nonlocal/comparison state; full sweep required')
        if isinstance(child, ast.Call):
            name = ast.unparse(child.func)
            direct = {'load_hints', 'validate_hints', 'ValueError', 'cls', 'isinstance', 'set', 'type',
                      'any', 'all', 'int', 'len', 'str', 'enumerate', 'bytes', '_fixture_case',
                      'validate_table_hints', 'validate_declarations', 'validate_lift_calls'}
            if name not in direct and not (isinstance(child.func, ast.Attribute) and child.func.attr in {'get', 'items', 'pop', 'update'}):
                raise ValueError('Input dispatch calls unknown execution/comparison behavior')


def _protected(source: str, module: str) -> str:
    tree = ast.parse(source)
    if module == 'difftest.py':
        # These two entry points select/validate hints. Case generation, RNG,
        # profiles, thresholds, emulation and normalization remain bytecode
        # equivalent at the AST level. Effective options are checked per item.
        for node in tree.body:
            if isinstance(node, ast.ClassDef) and node.name == 'Options':
                for method in node.body:
                    if isinstance(method, ast.FunctionDef) and method.name == 'for_function':
                        _input_only(method)
                node.body = [n for n in node.body if not isinstance(n, ast.FunctionDef) or n.name != 'for_function']
            if isinstance(node, ast.FunctionDef) and node.name == 'validate_hints':
                _input_only(node)
        tree.body = [n for n in tree.body if not isinstance(n, ast.FunctionDef) or n.name != 'validate_hints']
    else:
        class Dispatch(ast.NodeTransformer):
            def visit_Call(self, node):
                self.generic_visit(node)
                if ast.unparse(node.func) == 'difftest.Options.for_function':
                    node.keywords = [k for k in node.keywords if k.arg != 'tier']
                return node

            def visit_Assign(self, node):
                # The #23 acceptance change records the hint path, not results.
                if len(node.targets) == 1 and ast.unparse(node.targets[0]) == "record['hints']":
                    return None
                return self.generic_visit(node)
        tree = Dispatch().visit(tree)
    return ast.dump(tree, include_attributes=False)


def bounded(before: dict, after: dict) -> None:
    a, b = dict(before), dict(after)
    old_sources, new_sources = a.pop('dispatch_sources'), b.pop('dispatch_sources')
    old_components, new_components = a.pop('components'), b.pop('components')
    a.pop('harness', None)
    b.pop('harness', None)
    if a != b or set(old_components) != set(new_components):
        raise ValueError('Global policies, tools, target or dependency coverage changed; full sweep required')
    changed = {n for n in old_components if old_components[n] != new_components[n]}
    if not changed or not changed <= {'difftest.py', 'recover.py'}:
        raise ValueError('Only bounded input dispatch changes support compatibility')
    for name in changed:
        if (env.digest_bytes(old_sources[name].encode()) != old_components[name]
                or env.digest_bytes(new_sources[name].encode()) != new_components[name]
                or _protected(old_sources[name], name) != _protected(new_sources[name], name)):
            raise ValueError('Execution/comparison behavior changed outside input dispatch; full sweep required')


def create(before_path: Path, after_path: Path, regression: Path, reviewer: str, reason: str, output: Path,
           *, observer_before: Path | None = None, comparison: Path | None = None) -> dict:
    if observer_before is not None:
        if comparison is None:
            raise ValueError('Observation proof requires a real-case comparison')
        from . import observation_compat
        return observation_compat.create(before_path, after_path, regression, reviewer, reason,
                                         output, observer_before, comparison)
    if comparison is not None:
        raise ValueError('Comparison requires --observer-before')
    before = validation._load_results(before_path)
    after = validation.read(after_path)
    if not reviewer.strip() or not reason.strip():
        raise ValueError('Explicit reviewer and scope justification are required')
    if before.get('schema') != validation.SCHEMA or after.get('schema') != validation.SCHEMA:
        raise ValueError('Dependency-aware receipts/plans required')
    if before.get('identity_drift') or after['identity'] != validation.global_identity():
        raise ValueError('Stale or drifting execution identity')
    bounded(before['identity'], after['identity'])
    entries = ledger.cmre_entries()
    selection = [f'{e.va:08x}' for e in entries]
    if before['functions'] != selection or after['functions'] != selection:
        raise ValueError('Compatibility review must cover the complete accepted inventory')
    tiers = {str(validation.TierKey(e.va, tier)) for e in entries
             for tier in (('lifted', 'c') if e.record.get('c') else ('lifted',))}
    if set(after['items']) != tiers:
        raise ValueError('Compatibility plan must cover every accepted tier')
    expected = session_evidence.identity(env.ROOT)
    tests = session_evidence.successful_step(regression, expected)
    if not tests:
        raise ValueError('Matching completed full pytest with positive/negative regressions is required')
    scope = {}
    for key, item in after['items'].items():
        old = before.get('results', {}).get(key, {})
        # Recheck *every* effective input and compiler-selected build in the
        # proposed unaffected set. Hints alone can never seed compatibility.
        current = validation.snapshot(item['va'], item['tier'])
        if current != item['dependencies']:
            raise ValueError('Plan inputs changed during compatibility review')
        if old.get('ok') and old.get('dependencies') == current:
            scope[key] = validation.digest(current)
    receipt = {'schema': 'cmre-validation-compatibility/1', 'before': before['identity'],
               'after': after['identity'], 'before_receipt': str(before_path.resolve()),
               'before_sha256': env.digest(before_path), 'after_plan': str(after_path.resolve()),
               'after_sha256': env.digest(after_path), 'scope': scope, 'reviewer': reviewer,
               'reason': reason, 'regressions': tests, 'selection': selection,
               'limitations': 'Input-dispatch compatibility only. Original results and identities retained.'}
    coordination.atomic_json(output, receipt)
    return receipt


def check(path: Path, prior: dict, identity: dict) -> set[str]:
    data = validation.read(path)
    from . import observation_compat
    if data.get('schema') == observation_compat.SCHEMA:
        return observation_compat.check(data, prior, identity)
    if (data.get('schema') != 'cmre-validation-compatibility/1'
            or data['before'] != prior.get('identity') or data['after'] != identity
            or not data.get('reviewer') or not data.get('reason') or prior.get('identity_drift')):
        raise ValueError('Compatibility receipt does not match both execution identities')
    bounded(data['before'], data['after'])
    for name in ('before_receipt', 'after_plan'):
        if env.digest(Path(data[name])) != data['before_sha256' if name == 'before_receipt' else 'after_sha256']:
            raise ValueError('Compatibility evidence changed')
    expected = session_evidence.identity(env.ROOT)
    if session_evidence.successful_step(Path(data['regressions']['receipt']), expected) != data['regressions']:
        raise ValueError('Compatibility regression inputs changed')
    if data['selection'] != [f'{e.va:08x}' for e in ledger.cmre_entries()]:
        raise ValueError('Compatibility inventory changed')
    for key, sha in data['scope'].items():
        old = prior.get('results', {}).get(key, {})
        if not old.get('ok') or validation.digest(old.get('dependencies')) != sha:
            raise ValueError('Compatibility scope does not match original evidence')
    return set(data['scope'])


def command(args) -> int:
    result = create(Path(args.before), Path(args.after), Path(args.regressions), args.reviewer, args.reason, Path(args.output),
                    observer_before=Path(args.observer_before) if getattr(args, 'observer_before', None) else None,
                    comparison=Path(args.comparison) if getattr(args, 'comparison', None) else None)
    print(f"Compatibility receipt: {args.output}; {len(result['scope'])} unaffected tiers; original hashes retained")
    return 0
