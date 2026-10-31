"""Dependency-scoped revalidation. Receipts prove tests, never recovery or integration.

This orchestration layer deliberately leaves recover's run keys and acceptance
records alone. Old evidence keeps its original execution identity and location.
"""
from __future__ import annotations

from dataclasses import dataclass
import functools
import json
import os
from pathlib import Path
import re
import time
import uuid

from . import coordination, difftest, env, ledger, queue, recover

SCHEMA = 'cmre-validation/2'
BUILD_ENVIRONMENT = ('CPATH', 'C_INCLUDE_PATH', 'CPLUS_INCLUDE_PATH', 'GCC_EXEC_PREFIX',
                     'COMPILER_PATH', 'LIBRARY_PATH', 'SOURCE_DATE_EPOCH', 'GCC_COMPARE_DEBUG',
                     'DEPENDENCIES_OUTPUT', 'SUNPRO_DEPENDENCIES')
LOCAL_IDENTITY_FIELDS = {'components', 'planner', 'dispatch_sources', 'workspace', 'build_environment'}
COUNT_KEYS = ('inventory_functions', 'inventory_body_bytes', 'legacy_functions', 'legacy_body_bytes',
              'source_functions', 'source_body_bytes', 'c_tier_functions', 'c_tier_body_bytes',
              'lifted_functions', 'recovered_functions', 'recovered_body_bytes', 'integrated_functions')


@dataclass(frozen=True)
class TierKey:
    va: int
    tier: str

    def __post_init__(self):
        if not 0 <= self.va <= 0xffffffff or self.tier not in {'lifted', 'c'}:
            raise ValueError('Expected a 32-bit function VA and lifted/c tier')

    def __str__(self) -> str:
        return f'{self.va:08x}-{self.tier}'

    @property
    def function(self) -> str:
        return f'fn_{self.va:08x}'

    @classmethod
    def parse(cls, value: str) -> TierKey:
        match = re.fullmatch(r'([0-9a-f]{8})-(lifted|c)', value)
        if not match:
            raise ValueError('Invalid validation tier key')
        return cls(int(match[1], 16), match[2])


def coverage_counts() -> dict:
    """Actual whole-workspace recovery/integration counts, separate from selection."""
    coverage = queue.coverage_summary()
    return {key: coverage[key] for key in COUNT_KEYS}


def digest(value) -> str:
    return env.digest_bytes(json.dumps(value, sort_keys=True).encode())


def effective_options(va: int, tier: str) -> dict:
    opts = difftest.Options.for_function(va, tier=tier)
    if tier == 'c':
        opts.inputs = recover.c_inputs(va)
    return opts.__dict__


def compact_options(options: dict) -> dict:
    """Hash fixture payloads once per tier; retain policy and shapes for review."""
    return {**{k: val for k, val in options.items() if k != 'hints'},
            'effective_hints_sha256': digest(options['hints']),
            'fixture_count': len(options['hints'].get('fixtures', []))}


def read(path: Path) -> dict:
    return json.loads(path.read_text(encoding='utf-8'))


def receipt_schema(path: Path) -> str | None:
    # All cmre receipts put schema first; unknown layouts cannot seed reuse.
    with path.open(encoding='utf-8') as stream:
        header = stream.read(2048)
    match = re.search(r'"schema"\s*:\s*"([^"]+)"', header)
    return match[1] if match else None


def global_identity() -> dict:
    identity = dict(recover.execution_identity())
    # Header dependence belongs to each compiler-selected translation unit.
    identity.pop('headers', None)
    identity['components'] = {n: env.digest(Path(recover.__file__).parent / n)
                              for n in recover.HARNESS_MODULES}
    identity['planner'] = env.digest(Path(__file__))
    identity['workspace'] = str(env.ROOT.resolve())
    identity['build_environment'] = {name: os.environ[name] for name in BUILD_ENVIRONMENT if name in os.environ}
    identity['dispatch_sources'] = {n: (Path(recover.__file__).parent / n).read_bytes().decode('utf-8')
                                    for n in ('difftest.py', 'recover.py')}
    return identity


@functools.cache
def worker_identity() -> dict:
    """Inputs are frozen for a job; the parent checks again at its final gate."""
    return global_identity()


def _file_digest(path: Path) -> str | None:
    return env.digest(path) if path.is_file() else None


def _cacheable_build(files: dict) -> bool:
    # Re-preprocess volatile macros, computed macro names and external include
    # availability tests. A file hash alone cannot capture their effective input.
    uncertain = re.compile(rb'__DATE__|__TIME__|__TIMESTAMP__|__FILE__|__BASE_FILE__|__has_include|##|\?\?')
    return all(not uncertain.search((env.ROOT / name).read_bytes()) for name in files)


def _accepted_build(va: int, record: dict, execution: dict | None) -> dict:
    """Existing compiler-selected coverage is usable only under its exact build world."""
    if execution is None:
        return {}
    try:
        key = record['run_key']
        if not isinstance(key, str) or len(key) != 16 or any(c not in '0123456789abcdef' for c in key):
            return {}
        report = read(env.RUNS / f'fn_{va:08x}' / f'c-{key}' / 'report.json')
        if (report['execution_identity'] != execution or report['status'] != 'pass'
                or report['source_dependencies'] != record['source_dependencies']
                or report['preprocessed_sha256'] != record['preprocessed_sha256']):
            return {}
        return {'files': report['source_dependencies'], 'preprocessed': report['preprocessed_sha256']}
    except (OSError, ValueError, KeyError, TypeError):
        return {}


def snapshot(va: int, tier: str, previous: dict | None = None, *, execution: dict | None = None) -> dict:
    """Complete bounded single-function dependencies; missing coverage is an error.

    Outside callees execute pinned original instructions, so other authored C
    sources aren't dependencies here. Compositions have their own claim below.
    """
    record = read(ledger.record_path(va))
    options = effective_options(va, tier)
    source = (env.C_DIR / f'fn_{va:08x}.c' if tier == 'c'
              else env.LIFTED_DIR / f'fn_{va:08x}.S')
    source_hash = _file_digest(source)
    if source_hash is None:
        raise ValueError(f'Missing accepted {tier} source: {source}')
    accepted = {k: record[tier].get(k) for k in ('source_sha256', 'run_key', 'harness_sha256')}
    result = {'options': compact_options(options), 'source': source_hash,
              'source_path': env.rel(source), 'hints_path': env.rel(env.HINTS_DIR / f'fn_{va:08x}.json'),
              'accepted': {**accepted, 'sha256': digest(record[tier])},
              'profile': recover.c_profile(va) if tier == 'c' else recover.LIFTED_PROFILE,
              'optimizations': list(recover.C_OPT_LEVELS) if tier == 'c' else ['assembler'],
              'boundary': 'outside callees execute pinned original code'}
    if tier == 'c':
        # Reuse preprocessing only under the same compiler/global identity (the
        # caller enforces that) and unchanged complete dependency coverage.
        members = sorted(env.rel(p) for p in (env.ROOT / 'recovery/include').rglob('*') if p.is_file())
        prior = (previous or {}).get('build', {}) or _accepted_build(va, record[tier], execution)
        files = prior.get('files', {})
        if (files and (previous is None or previous.get('include_members') == members)
                and all(_file_digest(env.ROOT / name) == sha for name, sha in files.items())
                and files.get(env.rel(source)) == source_hash and _cacheable_build(files)):
            dependencies = prior
        else:
            dependencies = recover.c_dependencies(source)
        result.update(build=dependencies, include_members=members,
                      jump_contract=recover.jump_contract(va), x87_contract=recover.x87_contract(va))
    return result


def _load_results(path: Path) -> dict:
    data = read(path)
    results = dict(data.get('results', {}))
    for item in path.with_suffix('').glob('*.json'):
        results[item.stem] = read(item)
    return {**data, 'results': results}


def _prior(path: str | None) -> tuple[Path | None, dict]:
    root = (env.ANALYSIS / 'cmre/jobs').resolve()
    paths = [Path(path).resolve()] if path else sorted(root.glob('*.json'), key=lambda p: p.stat().st_mtime_ns, reverse=True)
    for candidate in paths:
        if not candidate.is_relative_to(root):
            raise ValueError('Validation receipt must be under CM0102/Analysis/cmre/jobs')
        # Inspect the manifest before touching completion files. Legacy jobs
        # can have thousands of them and cannot supply dependency coverage.
        if receipt_schema(candidate) == SCHEMA:
            return candidate, _load_results(candidate)
        if path:
            raise ValueError('A dependency-aware receipt is required; legacy jobs use --resume')
    return None, {}


def accepted_report(va: int, tier: str, dependencies: dict, execution: dict) -> dict | None:
    """Bootstrap from an accepted tier's exact current run key, without running it.

    Older reports without sufficient identity/dependency coverage are misses.
    In particular a ledger's harness label alone never proves current inputs.
    """
    try:
        opts = effective_options(va, tier)
        if compact_options(opts) != dependencies['options']:
            return None
        root = env.RUNS / f'fn_{va:08x}'
        if tier == 'lifted':
            request = recover._key(execution, va, opts, recover.LIFTED_PROFILE)
            key = read(root / f'request-{request}.json')['key']
            if not isinstance(key, str) or len(key) != 16 or any(c not in '0123456789abcdef' for c in key):
                return None
            path = root / key / 'report.json'
            source = root / key / f'fn_{va:08x}.S'
            text = source.read_bytes().decode('utf-8')
            expected = recover._key(execution, text, opts, recover.LIFTED_PROFILE)
        else:
            key = dependencies['accepted']['run_key']
            if not isinstance(key, str) or len(key) != 16 or any(c not in '0123456789abcdef' for c in key):
                return None
            path = root / f'c-{key}' / 'report.json'
            report = read(path)
            source = env.C_DIR / f'fn_{va:08x}.c'
            text = source.read_bytes().decode('utf-8')
            header = (env.ROOT / 'recovery/include/cmre.h').read_text(encoding='utf-8')
            profile = dependencies['profile']
            expected = recover._key(execution, text, header, env.rel(source), dependencies['build'], opts,
                                    profile if profile == 'abi' else [profile, report['returns']],
                                    recover.C_OPT_LEVELS, dependencies['jump_contract'], dependencies['x87_contract'])
        report = read(path)
        if (key != expected or report['key'] != key or report['status'] != 'pass'
                or report['tier'] != tier or report['execution_identity'] != execution
                or report['source_sha256'] != dependencies['source']
                or env.digest(source) != dependencies['source']
                or dependencies['accepted']['source_sha256'] != dependencies['source']):
            return None
        return {'ok': True, 'detail': 'existing accepted current run report', 'dependencies': dependencies,
                'identity': digest(execution), 'refresh': dependencies['accepted'].get('harness_sha256') == execution['harness'],
                'origin': {'report': str(path), 'sha256': env.digest(path)}, 'elapsed_seconds': None}
    except (OSError, ValueError, KeyError, TypeError):
        return None


def plan(vas: list[int], *, refresh: bool = False, reuse: str | None = None,
         resume: str | None = None, retry_failed: bool = False,
         audit: bool = False, compatibility: str | None = None) -> dict:
    started = time.monotonic()
    if resume and reuse:
        raise ValueError('Select either resume or reuse')
    if retry_failed and not resume:
        raise ValueError('Retry failed requires an explicit resume receipt')
    path, prior = _prior(resume or reuse)
    identity = global_identity()
    execution = {k: v for k, v in identity.items() if k not in LOCAL_IDENTITY_FIELDS}
    execution['headers'] = {env.rel(p): env.digest(p) for p in sorted((env.ROOT / 'recovery/include').rglob('*')) if p.is_file()}
    # Legacy run keys did not declare compiler environment overrides. Such a
    # run cannot establish the first dependency-aware receipt under overrides.
    legacy_build_covered = not identity.get('build_environment')
    same = identity == prior.get('identity')
    if prior.get('identity_drift'):
        same = False
    compatible = set()
    if compatibility:
        from . import validation_compat
        compatible = validation_compat.check(Path(compatibility), prior, identity)
    items = {}
    for va in vas:
        record = read(ledger.record_path(va))
        for tier in ('lifted', 'c') if record.get('c') else ('lifted',):
            key = str(TierKey(va, tier))
            old = prior.get('results', {}).get(key, {})
            adopted = None
            reason = []
            try:
                current = snapshot(va, tier, old.get('dependencies') if same else None,
                                   execution=execution if legacy_build_covered else None)
            except (OSError, ValueError, recover.build.BuildError) as exc:
                current = None
                reason.append(f'unknown dependency: {exc}')
            if not old and current is not None and not audit and legacy_build_covered:
                adopted = accepted_report(va, tier, current, execution)
                if adopted:
                    old = adopted
            if audit:
                reason.append('explicit full audit')
            if not same and key not in compatible and not adopted:
                reason.append('global execution/harness identity changed or unavailable')
            if not old.get('ok'):
                reason.append('no successful tier receipt (previous failures retained)')
            if refresh and not old.get('refresh'):
                reason.append('acceptance refresh not previously checked')
            before = old.get('dependencies', {})
            if current != before:
                reason.extend('changed ' + k for k in sorted(set(before) | set(current or {}))
                              if before.get(k) != (current or {}).get(k))
            action = 'execute' if reason else 'reuse'
            if old.get('ok') is False and not retry_failed and not audit:
                action = 'retain-failure'
                reason.append('previous failure retained; use --resume with --retry-failed to retry')
            items[key] = {'va': va, 'tier': tier, 'dependencies': current,
                          'action': action, 'reasons': reason,
                          'previous': old or None,
                          'estimated_seconds': old.get('elapsed_seconds')}
    composed = composition_impact(items, identity)
    return {'schema': SCHEMA, 'kind': 'revalidate', 'identity': identity, 'refresh': refresh,
            'retry_failed': retry_failed, 'resume_from': str(path) if resume else None,
            'counts': coverage_counts(), 'counts_scope': 'whole accepted workspace; independent of validation selection',
            'functions': [f'{va:08x}' for va in vas], 'items': items,
            'reuse_from': str(path) if path else None,
            'compatibility': ({'path': str(Path(compatibility).resolve()), 'sha256': env.digest(Path(compatibility))}
                              if compatibility else None),
            'planning_seconds': time.monotonic() - started, 'compositions': composed,
            'claims': {'recovery': 'unchanged; only cmre accept owns recovery',
                       'composition': 'reported separately; run compose-test for invalidated reports',
                       'live_integration': 'not checked', 'ledger_integrity': 'run cmre verify separately'}}


def composition_impact(items: dict, identity: dict) -> list[dict]:
    """Account for recorded compositions without representing single-C as composed proof."""
    result = []
    changed = {TierKey.parse(k).function for k, v in items.items() if v['action'] == 'execute'}
    for path in sorted((env.ANALYSIS / 'cmre/compositions').glob('*.json')):
        try:
            report = read(path)
            members = set(report['campaign']['functions'])
            affected = sorted(members & changed)
            reasons = []
            execution = {k: val for k, val in identity.items() if k not in LOCAL_IDENTITY_FIELDS}
            recorded = {k: val for k, val in report.get('execution_identity', {}).items() if k != 'headers'}
            if affected or execution != recorded:
                reasons.append('selected tier or composition execution identity changed/unknown')
            if report.get('composition_sha256') != env.digest(Path(__file__).with_name('composition.py')):
                reasons.append('composition implementation changed or unknown')
            for name, sha in report.get('source_sha256', {}).items():
                if _file_digest(env.C_DIR / name) != sha:
                    reasons.append('changed member source ' + name)
            if not report.get('source_dependencies') or not report.get('options'):
                reasons.append('composition dependency coverage unavailable')
            for build_inputs in report.get('source_dependencies', {}).values():
                for name, sha in build_inputs.get('files', {}).items():
                    if _file_digest(env.ROOT / name) != sha:
                        reasons.append('changed composed build input ' + name)
            for function, options in report.get('options', {}).items():
                if difftest.Options.for_function(int(function.removeprefix('fn_'), 16), tier='c').__dict__ != options:
                    reasons.append('changed composed effective inputs ' + function)
            result.append({'receipt': str(path), 'members': sorted(members),
                           'action': 'revalidate separately' if reasons else 'outside selection; no new claim',
                           'affected': affected, 'status': report['status'], 'reasons': reasons})
        except (OSError, ValueError, KeyError, TypeError):
            result.append({'receipt': str(path), 'action': 'unknown dependencies; revalidate separately'})
    return result


def summary(data: dict) -> dict:
    items = data['items']
    results = data.get('results', {})
    return {'selected_functions': len(data['functions']), 'selected_tiers': len(items),
            'counts': data.get('counts'), 'counts_scope': data.get('counts_scope', 'not recorded in this historical receipt'),
            'reused': sum(v['action'] == 'reuse' for v in items.values()),
            'retained_failures': sum(v['action'] == 'retain-failure' for v in items.values()),
            'execute': sum(v['action'] == 'execute' for v in items.values()),
            'passed': 0 if data.get('identity_drift') else sum(v['ok'] for v in results.values()),
            'failed': sum(not v['ok'] for v in results.values()),
            'inconclusive': len(results) if data.get('identity_drift') else sum(v.get('inconclusive', False) for v in results.values()),
            'pending': len(items) - len(results), 'planning_seconds': data['planning_seconds'],
            'new_differential_executions': sum(r.get('differential_executions', 0) for k, r in results.items()
                                               if items[k]['action'] == 'execute'),
            'validation_seconds': sum(r.get('elapsed_seconds') or 0 for k, r in results.items()
                                      if items[k]['action'] == 'execute'),
            'recovery_seconds': 0,
            'estimated_execution_seconds': sum(v['estimated_seconds'] or 0 for v in items.values() if v['action'] == 'execute'),
            'unmeasured_items': sum(v['estimated_seconds'] is None for v in items.values() if v['action'] == 'execute'),
            'claims': data['claims']}


def execute_item(va: int, tier: str, refresh: bool) -> tuple[bool, str]:
    record = read(ledger.record_path(va))
    if refresh and (tier == 'c' or not record.get('c')):
        ok, report = recover.accept(va, tier, refresh=True)
        return ok, report.get('status', 'unknown')
    report = recover.test_c(va, fresh=True) if tier == 'c' else recover.test_lifted(va, fresh=True)
    if report['status'] != 'pass':
        return False, report['status']
    if report['source_sha256'] != record[tier]['source_sha256']:
        return False, 'source-drift'
    return True, 'pass'


def execute(item: dict, identity: dict | str, refresh: bool) -> dict:
    started = time.monotonic()
    identity_hash = identity if isinstance(identity, str) else digest(identity)
    differential_executions = 0
    inconclusive_cases = 0
    original_run = difftest.run

    def counted(*args, **kwargs):
        nonlocal differential_executions, inconclusive_cases
        differential_executions += 1
        result = original_run(*args, **kwargs)
        inconclusive_cases += result.get('inconclusive', 0)
        return result

    try:
        if digest(worker_identity()) != identity_hash or snapshot(item['va'], item['tier'], item['dependencies']) != item['dependencies']:
            raise ValueError('inputs changed after planning')
        if item['dependencies'] is None:
            raise ValueError('unknown dependencies')
        # Each process executes items serially. Instrument calls without changing
        # case generation, results, acceptance or the harness's own cache keys.
        difftest.run = counted
        ok, detail = execute_item(item['va'], item['tier'], refresh)
        after = snapshot(item['va'], item['tier'], item['dependencies'])
        # Refresh may update only acceptance evidence, not the tested inputs.
        if ({k: v for k, v in after.items() if k != 'accepted'} !=
                {k: v for k, v in item['dependencies'].items() if k != 'accepted'}):
            raise ValueError('inputs changed during validation')
        return {'ok': ok, 'detail': detail, 'dependencies': after, 'identity': identity_hash,
                'refresh': refresh, 'elapsed_seconds': time.monotonic() - started,
                'differential_executions': differential_executions, 'inconclusive_cases': inconclusive_cases,
                'inconclusive': detail in {'inconclusive', 'input-drift', 'nondeterministic'}}
    except Exception as exc:
        return {'ok': False, 'detail': f'{type(exc).__name__}: {exc}', 'identity': identity_hash,
                'dependencies': item['dependencies'], 'refresh': refresh,
                'elapsed_seconds': time.monotonic() - started, 'inconclusive': True,
                'differential_executions': differential_executions, 'inconclusive_cases': inconclusive_cases}
    finally:
        difftest.run = original_run


class Run:
    def __init__(self, data: dict):
        self.path = env.ANALYSIS / 'cmre/jobs' / f'{time.time_ns()}-revalidate-{uuid.uuid4().hex[:8]}.json'
        self.data = {**data, 'state': 'running', 'started': time.time(), 'results': {},
                     'counts_before': dict(data['counts'])}
        for key, item in data['items'].items():
            if item['action'] in {'reuse', 'retain-failure'}:
                # Keep the result's original identity; the new plan records why
                # reuse is allowed. Never replace an old hash with a new one.
                self.data['results'][key] = item['previous']
        self.save()

    def save(self):
        coordination.atomic_json(self.path, self.data)

    def record(self, key: str, result: dict):
        result = {**result, 'origin': {'receipt': str(self.path), 'item': key}}
        coordination.atomic_json(self.path.with_suffix('') / f'{key}.json', result)
        self.data['results'][key] = result

    def finish(self):
        self.data['state'] = 'finished' if len(self.data['results']) == len(self.data['items']) else 'incomplete'
        # Reused and early completed inputs must stay frozen until the final
        # gate too; otherwise an all-reuse run could claim green after an edit.
        for key, result in list(self.data['results'].items()):
            if not result['ok']:
                continue
            item = self.data['items'][key]
            try:
                current = snapshot(item['va'], item['tier'], result['dependencies'])
                unchanged = current == result['dependencies']
            except (OSError, ValueError, recover.build.BuildError):
                unchanged = False
            if not unchanged:
                self.data.setdefault('invalidated_during_run', {})[key] = result
                self.record(key, {**result, 'ok': False, 'inconclusive': True,
                                  'detail': 'Inputs changed after execution/reuse; previous result retained'})
        if global_identity() != self.data['identity']:
            self.data['identity_drift'] = True
            self.data['state'] = 'incomplete'
        self.data['ended'] = time.time()
        self.data['counts'] = coverage_counts()
        self.data['summary'] = summary(self.data)
        self.save()
