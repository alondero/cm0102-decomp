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

from . import build, coordination, difftest, env, ledger, queue, recover

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
    if retry_failed and audit:
        raise ValueError('--audit re-executes every tier, so it cannot be a failures-only retry; '
                         'drop --audit to retry failures, or drop --retry-failed for a full audit')
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
    proof_origin = str(path) if path else None
    inherited = prior.get('compatibility') or {}
    if compatibility is None and not audit and inherited.get('kind') in {'observation-proof', 'known-data-proof'}:
        compatibility = inherited['path']
        if env.digest(Path(compatibility)) != inherited['sha256']:
            raise ValueError('Inherited compatibility receipt changed')
    if compatibility:
        from . import validation_compat
        from . import observation_compat, known_data_compat
        proof_data = read(Path(compatibility))
        proof_prior = prior
        if proof_data.get('schema') in {observation_compat.SCHEMA, known_data_compat.SCHEMA}:
            proof_origin = proof_data['before_receipt']
            if prior.get('identity') == proof_data['after']:
                if prior.get('identity_drift') or inherited.get('sha256') != env.digest(Path(compatibility)):
                    raise ValueError('Derived receipt has no matching observation proof')
                proof_prior = _load_results(Path(proof_origin))
        compatible = validation_compat.check(Path(compatibility), proof_prior, identity)
    items = {}
    for va in vas:
        record = read(ledger.record_path(va))
        for tier in ('lifted', 'c') if record.get('c') else ('lifted',):
            key = str(TierKey(va, tier))
            old = prior.get('results', {}).get(key, {})
            known = key in compatible and getattr(compatible, 'known_data', False)
            if known:
                old = compatible.origins[key]  # retain authenticated old report identity
            adopted = None
            reason = []
            try:
                current = snapshot(va, tier, old.get('dependencies') if same or key in compatible else None,
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
                reason.append('tier outside verified compatibility scope' if (getattr(compatible, 'observation', False)
                              or getattr(compatible, 'known_data', False))
                              else 'global execution/harness identity changed or unavailable')
            if not old.get('ok'):
                reason.append('no successful tier receipt (previous failures retained)')
            if refresh and not old.get('refresh'):
                reason.append('acceptance refresh not previously checked')
            before = old.get('dependencies', {})
            observation = key in compatible and getattr(compatible, 'observation', False)
            if known:
                from .known_data_compat import same_inputs
                unchanged = same_inputs(before, current)
            elif observation:
                from .observation_compat import same_inputs
                unchanged = same_inputs(before, current)
            else:
                unchanged = current == before
            if not unchanged:
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
            if (observation or known) and action == 'reuse':
                items[key]['evidence_adoption'] = {'kind': 'known-data-proof' if known else 'observation-proof',
                    'origin_identity': old.get('identity'), 'receipt': str(compatibility)}
    composed = composition_impact(items, identity)
    return {'schema': SCHEMA, 'kind': 'revalidate', 'identity': identity, 'refresh': refresh,
            'retry_failed': retry_failed, 'resume_from': str(path) if resume else None,
            'counts': coverage_counts(), 'counts_scope': 'whole accepted workspace; independent of validation selection',
            'functions': [f'{va:08x}' for va in vas], 'items': items,
            'reuse_from': str(path) if path else None,
            'compatibility': ({'path': str(Path(compatibility).resolve()), 'sha256': env.digest(Path(compatibility)),
                               'kind': ('known-data-proof' if getattr(compatible, 'known_data', False) else
                                        'observation-proof' if getattr(compatible, 'observation', False) else 'input-dispatch'),
                               'origin_receipt': proof_origin}
                              if compatibility else None),
            'planning_seconds': time.monotonic() - started, 'compositions': composed,
            'claims': {'recovery': 'unchanged; only cmre accept owns recovery',
                       'composition': 'reported separately; run compose-test for invalidated reports',
                       'live_integration': 'not checked', 'ledger_integrity': 'run cmre verify separately'}}


def composition_impact(items: dict, identity: dict) -> list[dict]:
    """Account for recorded compositions without representing single-C as composed proof."""
    from . import composition
    result = []
    dependency_cache = {}
    report_snapshots = {}
    policy_snapshots = {}
    legacy_snapshots = {}
    initial_composition = env.digest(Path(composition.__file__))
    inspector_snapshot = None
    observed_drift = False
    initial_environment = {name: os.environ[name] for name in BUILD_ENVIRONMENT if name in os.environ}
    environment_unknown = bool(identity.get('build_environment')) or initial_environment != identity.get('build_environment', {})
    changed = {TierKey.parse(k).function for k, v in items.items() if v['action'] == 'execute'}
    for path in sorted((env.ANALYSIS / 'cmre/compositions').glob('*.json')):
        try:
            report_snapshots[path] = env.digest_bytes(path.read_bytes())
            report = read(path)
            members = set(report['campaign']['functions'])
            affected = sorted(members & changed)
            reasons = []
            if environment_unknown:
                reasons.append('composition build environment overrides or drift')
            execution = {k: val for k, val in identity.items() if k not in LOCAL_IDENTITY_FIELDS}
            recorded = {k: val for k, val in report.get('execution_identity', {}).items() if k != 'headers'}
            if affected or execution != recorded:
                reasons.append('selected tier or composition execution identity changed/unknown')
            if report.get('composition_sha256') != env.digest(Path(__file__).with_name('composition.py')):
                reasons.append('composition implementation changed or unknown')
            objdump = report.get('objdump')
            if 'execution_tiers' in report and objdump is None:
                reasons.append('composition object inspector identity unavailable')
            if objdump is not None:
                tool = Path(env.tool('objdump'))
                version = env.run_tool([str(tool), '--version'])
                current_inspector = (str(tool), _file_digest(tool), version.returncode, version.stdout)
                if inspector_snapshot is None:
                    inspector_snapshot = current_inspector
                elif inspector_snapshot != current_inspector:
                    observed_drift = True
                    reasons.append('composition object inspector changed during evaluation')
                if (not isinstance(objdump, dict) or objdump.get('path') != str(tool)
                        or objdump.get('sha256') != _file_digest(tool) or version.returncode
                        or not version.stdout.splitlines() or objdump.get('version') != version.stdout.splitlines()[0]):
                    reasons.append('composition object inspector changed or unknown')
            tiers = report.get('execution_tiers')
            if tiers is None:
                # Older reports predate explicit companions and only admit C.
                tiers = {member: {'execution_tier': 'c'} for member in members}
            if not isinstance(tiers, dict) or set(tiers) != members:
                raise ValueError('composition tier coverage unavailable')
            expected_sources = set()
            for member in members:
                tier = tiers[member]
                if not isinstance(tier, dict):
                    raise ValueError('composition tier malformed')
                va = int(member.removeprefix('fn_'), 16)
                if tier.get('execution_tier') == 'c':
                    name = member + '.c'
                    expected_sources.add(name)
                    recorded_sha = report.get('source_sha256', {}).get(name)
                    actual_sha = _file_digest(env.C_DIR / name)
                    if not isinstance(recorded_sha, str) or actual_sha is None or actual_sha != recorded_sha:
                        reasons.append('changed member source ' + name)
                elif (tier.get('execution_tier') == 'lift-companion'
                      and tier.get('provenance') == 'fresh-original-derived-legacy'
                      and va == composition._LEGACY_LIFT):
                    name = member + '.S'
                    expected_sources.add(name)
                    if (tier.get('ledger') != 'recovery/date/ledger.json'
                            or tier.get('source_sha256') != composition._LEGACY_LIFT_SOURCE
                            or report.get('source_sha256', {}).get(name) != composition._LEGACY_LIFT_SOURCE
                            or tier.get('legacy_dependencies') != composition._LEGACY_FILES):
                        reasons.append('legacy companion provenance changed or unknown ' + member)
                    for legacy_name, sha in composition._LEGACY_FILES.items():
                        current_sha = _file_digest(env.ROOT / legacy_name)
                        if legacy_name in legacy_snapshots and legacy_snapshots[legacy_name] != current_sha:
                            observed_drift = True
                        legacy_snapshots[legacy_name] = current_sha
                        if current_sha != sha:
                            reasons.append('changed legacy input ' + legacy_name)
                else:
                    raise ValueError('unknown composition execution tier')
            if set(report.get('source_sha256', {})) != expected_sources:
                reasons.append('composition source coverage changed or unknown')
            if not isinstance(report.get('source_dependencies'), dict) or not report.get('options'):
                reasons.append('composition dependency coverage unavailable')
            elif set(report['options']) != members:
                reasons.append('composition input policy coverage unavailable')
            c_sources = {member + '.c' for member in members if tiers[member].get('execution_tier') == 'c'}
            if not c_sources <= set(report.get('source_dependencies', {})):
                reasons.append('composition source dependency coverage unavailable')
            for name in c_sources:
                dependency = report.get('source_dependencies', {}).get(name)
                if not isinstance(dependency, dict) or not isinstance(dependency.get('files'), dict):
                    reasons.append('composition source dependency malformed ' + name)
                    continue
                source_name = env.rel(env.C_DIR / name)
                if dependency['files'].get(source_name) != report.get('source_sha256', {}).get(name):
                    reasons.append('composition source dependency missing ' + name)
                source_path = env.C_DIR / name
                if not reasons and source_path not in dependency_cache:
                    try:
                        dependency_cache[source_path] = recover.c_dependencies(source_path)
                    except (build.BuildError, ValueError, OSError):
                        dependency_cache[source_path] = None
                if not reasons and (dependency_cache[source_path] is None or dependency_cache[source_path] != dependency):
                    reasons.append('composition full C dependencies changed or unknown ' + name)
            for build_inputs in report.get('source_dependencies', {}).values():
                if not isinstance(build_inputs, dict) or not isinstance(build_inputs.get('files'), dict):
                    raise ValueError('composition build inputs malformed')
                for name, sha in build_inputs.get('files', {}).items():
                    if _file_digest(env.ROOT / name) != sha:
                        reasons.append('changed composed build input ' + name)
            for function, options in report.get('options', {}).items():
                current_options = composition.composition_c_options(int(function.removeprefix('fn_'), 16)).__dict__
                if function in policy_snapshots and policy_snapshots[function] != current_options:
                    observed_drift = True
                policy_snapshots[function] = current_options
                if current_options != options:
                    reasons.append('changed composed effective inputs ' + function)
            result.append({'receipt': str(path), 'members': sorted(members),
                           'action': 'revalidate separately' if reasons else 'outside selection; no new claim',
                           'affected': affected, 'status': report['status'], 'reasons': reasons})
        except (OSError, ValueError, KeyError, TypeError):
            result.append({'receipt': str(path), 'action': 'unknown dependencies; revalidate separately'})
    final_environment = {name: os.environ[name] for name in BUILD_ENVIRONMENT if name in os.environ}
    drift = (observed_drift or final_environment != initial_environment
             or env.digest(Path(composition.__file__)) != initial_composition)
    if not drift:
        try:
            drift = any(env.digest_bytes(path.read_bytes()) != sha for path, sha in report_snapshots.items())
            drift |= any(_file_digest(env.ROOT / name) != sha for name, sha in legacy_snapshots.items())
            drift |= any(composition.composition_c_options(int(name.removeprefix('fn_'), 16)).__dict__ != options
                         for name, options in policy_snapshots.items())
            if inspector_snapshot is not None:
                tool = Path(env.tool('objdump'))
                version = env.run_tool([str(tool), '--version'])
                drift |= (str(tool), _file_digest(tool), version.returncode, version.stdout) != inspector_snapshot
            if 'toolchain' in identity:
                now = {k: v for k, v in recover.execution_identity().items() if k != 'headers'}
                drift |= now != {k: v for k, v in identity.items() if k not in LOCAL_IDENTITY_FIELDS}
        except (OSError, ValueError, KeyError, TypeError, build.BuildError):
            drift = True
    if not drift:
        for source_path, snapshot in dependency_cache.items():
            try:
                if snapshot is None or recover.c_dependencies(source_path) != snapshot:
                    drift = True
                    break
            except (build.BuildError, ValueError, OSError):
                drift = True
                break
    if drift:
        for row in result:
            if row.get('action') == 'outside selection; no new claim':
                row['action'] = 'revalidate separately'
                row.setdefault('reasons', []).append('composition impact inputs changed during evaluation')
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


class ToolchainEnvironmentError(RuntimeError):
    """The host, not the code under test, stopped the toolchain; the item proves nothing."""


# A toolchain that fails for a real reason prints a diagnostic. Exiting non-zero
# with no text, or reporting resource exhaustion, means the host failed.
# Only lines the toolchain itself prints count: diagnostics echo source lines, so
# resource words elsewhere in stderr say nothing about the host.
_ENVIRONMENT_STDERR = re.compile(
    r'^(?:\S*[\\/])?(?:cc1(?:plus)?|gcc|g\+\+|ld|as|nm|collect2)(?:\.exe)?: .*'
    r"(?:out of memory|virtual memory exhausted|cannot allocate memory|no space left|not enough (?:memory|space)"
    r"|insufficient (?:system )?resources|(?:cannot|can't|unable to) (?:create|open|write) temporary"
    r"|cannot fork|resource temporarily unavailable)", re.I | re.M)


def _environmental(stderr: str) -> bool:
    return not stderr.strip() or bool(_ENVIRONMENT_STDERR.search(stderr.lstrip()))


def toolchain_failure(report: dict) -> str | None:
    """Describe an environmental toolchain failure in a test report, else None.

    One environmental build among several makes the whole report environmental,
    even if another optimisation level shows a mismatch: the item is retried.
    """
    for problem in report.get('problems') or []:
        if isinstance(problem, str) and problem.startswith('assembler:') and _environmental(problem[len('assembler:'):]):
            return problem
    for opt, build_report in sorted((report.get('builds') or {}).items()):
        if (isinstance(build_report, dict) and build_report.get('status') == 'build-failed'
                and str(build_report.get('detail', '')).startswith(('compile failed', 'link failed'))
                and _environmental(build_report.get('stderr') or '')):
            return f"{opt} {build_report['detail']}: {(build_report.get('stderr') or '').strip()[:200] or 'no diagnostics'}"
    return None


def execute_item(va: int, tier: str, refresh: bool) -> tuple[bool, str]:
    record = read(ledger.record_path(va))
    if refresh and (tier == 'c' or not record.get('c')):
        ok, report = recover.accept(va, tier, refresh=True)
        failure = None if ok else toolchain_failure(report)
        if failure:  # accept returns before writing a ledger record on a failed test
            raise ToolchainEnvironmentError(failure)
        return ok, report.get('status', 'unknown')
    report = recover.test_c(va, fresh=True) if tier == 'c' else recover.test_lifted(va, fresh=True)
    if report['status'] != 'pass':
        failure = toolchain_failure(report)
        if failure:
            raise ToolchainEnvironmentError(failure)
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
        proof_drift = False
        verified_scope = None
        if self.data.get('compatibility'):
            try:
                from . import validation_compat
                proof = self.data['compatibility']
                if env.digest(Path(proof['path'])) != proof['sha256']:
                    raise ValueError('Compatibility receipt changed after planning')
                verified_scope = validation_compat.check(Path(proof['path']),
                    _load_results(Path(proof.get('origin_receipt') or self.data['reuse_from'])), self.data['identity'])
            except (OSError, ValueError, KeyError, TypeError):
                proof_drift = True
                self.data['compatibility_drift'] = True
                self.data['state'] = 'incomplete'
        # Reused and early completed inputs must stay frozen until the final
        # gate too; otherwise an all-reuse run could claim green after an edit.
        for key, result in list(self.data['results'].items()):
            if not result['ok']:
                continue
            item = self.data['items'][key]
            try:
                current = snapshot(item['va'], item['tier'], result['dependencies'])
                expected = (item['dependencies'] if item.get('evidence_adoption')
                            and item['action'] == 'reuse' else result['dependencies'])
                unchanged = current == expected and not (proof_drift and item['action'] == 'reuse')
                if (item.get('evidence_adoption', {}).get('kind') == 'known-data-proof'
                        and item['action'] == 'reuse' and key not in (verified_scope or set())):
                    unchanged = False
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
