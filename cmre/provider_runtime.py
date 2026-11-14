"""Trusted campaign adapters; providers propose actions, existing CMRE gates decide."""
from __future__ import annotations

import copy
import gzip
import hashlib
import json
import os
import re
from pathlib import Path
from threading import Lock

from . import coordination

_ACTION_ID_RE = re.compile(r'a\d{4}')
_OUTPUT_ID_RE = re.compile(r'(a\d{4})-output(?:\.json)?$')
_U32_MAX = 0xFFFFFFFF


def _json(path):
    from scripts.cmre_provider_artifacts import load
    return load(Path(path))


def _hash(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def _pin(path):
    path = Path(path).resolve(strict=True)
    return {'path': str(path), 'sha256': _hash(path)}


def _verified(pin, root=None):
    if not isinstance(pin, dict):
        raise ValueError('Artifact descriptor must be a mapping')
    raw_path = pin.get('path')
    expected = pin.get('sha256')
    if not isinstance(raw_path, str) or not raw_path:
        raise ValueError('Artifact descriptor needs a path')
    if not isinstance(expected, str) or len(expected) != 64:
        raise ValueError('Artifact descriptor needs a sha256')
    path = Path(raw_path).resolve(strict=True)
    if root is not None and not path.is_relative_to(Path(root).resolve()):
        raise ValueError('Artifact escapes the assigned job')
    if _hash(path) != expected.lower():
        raise ValueError(f'Artifact hash changed: {path.name}')
    return path


def _digest(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(',', ':'),
                                     allow_nan=False, ensure_ascii=False).encode()).hexdigest()


def _response(record):
    return record.get('response', record.get('result'))


def _to_int(value):
    if isinstance(value, bool):
        raise ValueError('address must not be a boolean')
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        try:
            return int(value, 0)
        except ValueError:
            return int(value, 16)
    raise ValueError('address is not an integer or hex string')


def _read_json_maybe_gzip(path):
    raw = Path(path).read_bytes()
    if path.suffix == '.gz' or raw[:2] == b'\x1f\x8b':
        raw = gzip.decompress(raw)
    return json.loads(raw.decode('utf-8'), parse_constant=lambda name: (_ for _ in ()).throw(
        ValueError(f'non-finite JSON constant: {name}')))


def _find_key(value, names, depth=0):
    if depth > 12:
        return None
    if isinstance(value, dict):
        for name in names:
            if name in value:
                return value[name]
        for item in value.values():
            found = _find_key(item, names, depth + 1)
            if found is not None:
                return found
    elif isinstance(value, list):
        for item in value[:8]:
            found = _find_key(item, names, depth + 1)
            if found is not None:
                return found
    return None


def _raw_facts(raw):
    facts = {}
    for key in ('outcome', 'covered_pcs', 'coverage', 'import_count', 'execution_identity'):
        value = _find_key(raw, (key,))
        if value is not None:
            facts[key] = value
    native = raw.get('result') if isinstance(raw, dict) else None
    if isinstance(native, dict):
        if isinstance(native.get('covered'), list):
            facts['covered_pcs'] = [f'{_to_int(pc):#x}' for pc in native['covered']]
        if isinstance(native.get('events'), list):
            facts['import_count'] = sum(isinstance(event, list) and bool(event)
                                       and event[0] == 'c' for event in native['events'])
    if 'import_count' not in facts:
        imports = _find_key(raw, ('imports',))
        if isinstance(imports, list):
            facts['import_count'] = len(imports)
    return facts


def _bounded(value, limit=20):
    if isinstance(value, list):
        return value[:limit]
    return value


def _passing(status):
    return isinstance(status, str) and status.lower() in ('pass', 'passed', 'ok')


def _task_owner(directory, job_id):
    identity = os.path.normcase(str(Path(directory).resolve()))
    digest = hashlib.sha256(identity.encode('utf-8')).hexdigest()[:24]
    return f'provider:{digest}:{job_id}'


def prepare_manifest(directory, manifest):
    """Resolve trusted paths without starting the clock or creating the store."""
    from . import env
    result = copy.deepcopy(manifest)
    directory = Path(directory).resolve()
    if not directory.is_relative_to(env.ANALYSIS.resolve()):
        raise ValueError('Campaign evidence must stay in the local analysis area')
    seen = set()
    for job in result['jobs']:
        config = job['config']
        va = int(config['function'], 16)
        if va in seen:
            raise ValueError('Only one active job per function is supported')
        seen.add(va)
        config['directory'] = str(directory / 'jobs' / job['id'])
        config['allowed_functions'] = list(config.get('allowed_functions', [config['function']]))
        _verified(config['seed_fixture'])
        for pin in config.get('context_files', []):
            _verified(pin)
        seed = config.get('candidate_seed')
        if seed is not None:
            if config['mode'] != 'c':
                raise ValueError('candidate_seed is only supported for C jobs')
            from .provider_opencode import _validate_source_bytes
            _validate_source_bytes(_verified(seed).read_bytes(), 'candidate seed')
        if config['mode'] == 'c':
            # Starting a campaign never adopts or overwrites pre-existing WIP.
            canonical = env.C_DIR / f'fn_{va:08x}.c'
            if canonical.exists() and (seed is None or _hash(canonical) != seed['sha256'].lower()):
                raise ValueError(f'Existing canonical source requires explicit reconciliation: {va:#x}')
            from . import work
            hold = work.dispatch_hold(va, work.read()['functions'])
            if hold:
                raise ValueError(f"C job fn_{va:08x} is not dispatch-ready: {hold['reason']}; "
                                 f"{hold['retry_when']}. Inspect and explicitly reconcile with cmre work retry")
            config['candidate'] = str(Path(config['directory']) / f'fn_{va:08x}.c')
            header = _pin(env.ROOT / 'recovery/include/cmre.h')
            config.setdefault('context_files', []).append(header)
    return result


def _seed_targets(directory, manifest):
    directory = Path(directory).resolve()
    targets = []
    for job in manifest['jobs']:
        config = job['config']
        if config.get('candidate_seed') is None:
            continue
        root = directory / 'jobs' / job['id']
        target = Path(config['candidate'])
        if target.is_symlink():
            raise ValueError('Seed candidate must not be a symbolic link')
        candidate = target.resolve()
        if config.get('mode') != 'c' or not candidate.is_relative_to(root):
            raise ValueError('Seed candidate escapes its assigned C job')
        targets.append((config['candidate_seed'], candidate))
    return directory, targets


def _seed_marker(targets):
    return {'schema': 'cmre-provider-seeds/1', 'candidates': [
        {'path': str(candidate), 'initial_sha256': seed['sha256'].lower()}
        for seed, candidate in targets]}


def seed_candidates(directory, manifest, *, marker=None):
    """Copy verified seeds once after initialization, leaving canonical C intact."""
    from .provider_opencode import _validate_source_bytes
    directory, targets = _seed_targets(directory, manifest)
    copies = []
    for seed, candidate in targets:
        source = _verified(seed)
        data = source.read_bytes()
        if hashlib.sha256(data).hexdigest() != seed['sha256'].lower():
            raise ValueError('Candidate seed hash changed before copy')
        _validate_source_bytes(data, 'candidate seed')
        if candidate.exists():
            raise ValueError(f'Seed candidate already exists: {candidate}')
        copies.append((candidate, data))
    for candidate, data in copies:
        candidate.parent.mkdir(parents=True, exist_ok=True)
        with candidate.open('xb') as stream:
            stream.write(data)
        if _hash(candidate) != hashlib.sha256(data).hexdigest():
            raise ValueError('Candidate seed copy did not preserve its bytes')
    if targets:
        coordination.atomic_json(Path(marker) if marker else directory / 'seed-candidates.json',
                                 _seed_marker(targets))


def verify_seeded_candidates(directory, manifest, *, marker=None):
    """Refuse incomplete initialization before the clock or any provider starts."""
    directory, targets = _seed_targets(directory, manifest)
    if not targets:
        return
    marker = Path(marker) if marker else directory / 'seed-candidates.json'
    if not marker.is_file() or _json(marker) != _seed_marker(targets):
        raise ValueError('Candidate seed initialization is incomplete; retain this queue and '
                         'reconcile its copy evidence before dispatch')
    if any(not candidate.is_file() for _, candidate in targets):
        raise ValueError('Initialized seed candidate is missing; reconcile retained evidence before dispatch')


class Executor:
    """Scoped evidence, task validation and serial publication for one campaign."""

    def __init__(self, directory, *, broker=None, task_api=None):
        self.directory = Path(directory).resolve()
        self.broker = broker
        self.task_api = task_api
        self._publication_serial = Lock()

    def _config(self, job):
        config = dict(job.get('config', job))
        root = Path(config['directory']).resolve()
        if root != self.directory / 'jobs' / job['id']:
            raise ValueError('Job directory differs from its fixed campaign path')
        return config, root

    def _records(self, root, prior, job_id=None):
        records = {}

        def load(path):
            path = Path(path).resolve(strict=True)
            if not path.is_relative_to(root):
                raise ValueError('Previous action escapes the assigned job')
            record = _json(path)
            if not isinstance(record, dict):
                raise ValueError('Previous action record is not an object')
            action_id = record.get('id')
            if not isinstance(action_id, str) or not _ACTION_ID_RE.fullmatch(action_id):
                raise ValueError('Previous action identity is invalid')
            if 'hash' in record:
                actual = _digest({key: value for key, value in record.items() if key != 'hash'})
                if actual != record['hash']:
                    raise ValueError('Previous action canonical hash changed')
            records[action_id] = record
            return record

        for ref in prior or []:
            if not isinstance(ref, dict):
                raise ValueError('Previous action reference is not an object')
            rel = ref.get('path')
            if not isinstance(rel, str) or not rel:
                raise ValueError('Previous action reference needs a path')
            path = (self.directory / rel).resolve(strict=True)
            if not path.is_relative_to(root):
                raise ValueError('Previous action escapes the assigned job')
            expected = ref.get('sha256')
            if expected is not None:
                actual = _hash(path)
                if not isinstance(expected, str) or actual != expected.lower():
                    raise ValueError('Previous action hash changed')
            record = load(path)
            if ref.get('hash') is not None and ref['hash'] != record.get('hash'):
                raise ValueError('Previous action reference hash changed')
            if job_id is not None and (not ref.get('hash') or not record.get('hash')):
                raise ValueError('Committed action hash is required')
            if record.get('id') != ref.get('id'):
                raise ValueError('Action identity changed')

        # The durable store keeps every action record, not only the bounded
        # context window, so early probes remain addressable by FILE ID.
        if job_id is None and root.is_dir():
            for path in sorted(root.iterdir()):
                if path.is_file() and _ACTION_ID_RE.fullmatch(path.stem):
                    load(path)
        return records

    def _artifact_map(self, root, records):
        artifacts = {}
        for action_id, record in records.items():
            result = _response(record)
            if not isinstance(result, dict):
                continue
            pins = result.get('artifacts')
            if not isinstance(pins, dict):
                continue
            for name, pin in pins.items():
                if not isinstance(name, str) or not name or name.startswith('/'):
                    raise ValueError('Artifact name is not a relative retained file')
                if '..' in Path(name).parts:
                    raise ValueError('Artifact name escapes the retained output')
                _verified(pin, root)
                artifacts[f'{action_id}/{name}'] = pin
        return artifacts

    def __call__(self, job, action, output, *, prior=None):
        config, root = self._config(job)
        output = Path(output)
        output = output if output.is_absolute() else self.directory / output
        output = output.resolve()
        if not output.is_relative_to(root):
            raise ValueError('Execution output escapes the assigned job')
        records = self._records(root, prior, job.get('id'))
        kind = action['kind']
        if kind == 'bundle':
            return self._bundle(job, config, root, action, output, records)
        if kind == 'finalize':
            if config['mode'] == 'c':
                # Reviewers may finish together; queue the normal publication
                # gate rather than turning file-lock contention into a hold.
                with self._publication_serial:
                    return self._finalize(job, config, root, action, output)
            return self._finalize(job, config, root, action, output)
        if kind == 'control-plan':
            return self._control_plan(job, config, root, action, output, records)
        if kind in ('test', 'controls'):
            return self._task_action(job, config, root, action, output)
        from . import provider_broker
        broker = self.broker or provider_broker.dispatch
        artifacts = self._artifact_map(root, records)
        try:
            response = broker(config, action, output, prior=artifacts,
                              candidate=config.get('candidate'))
        except Exception as error:
            # Invalid data requests remain actionable provider feedback. The
            # broker retains any execution failure receipt before raising.
            return {'kind': kind, 'ok': False, 'error': str(error),
                    'artifacts': {}, 'artifact_ids': []}
        names = sorted(response.get('artifacts', {}))
        match = _OUTPUT_ID_RE.search(output.name)
        action_id = match.group(1) if match else None
        response['artifact_ids'] = [f'{action_id}/{name}' if action_id else name
                                    for name in names]
        return response

    def _task_action(self, job, config, root, action, output):
        if set(action) != {'kind'} or config['mode'] != 'c':
            raise ValueError('C validation requires a C job and a parameter-free action')
        from . import env, tasks, worktrees
        api = self.task_api or tasks
        candidate = Path(config['candidate']).resolve(strict=True)
        if not candidate.is_relative_to(root):
            raise ValueError('Candidate escapes the assigned job')
        if action['kind'] == 'controls':
            if not config.get('control_plan'):
                return {'kind': 'controls', 'ok': False,
                        'error': 'A pinned named control plan is required; no controls were invented'}
            from . import mutate
            plan = _verified(config['control_plan'])
            parsed = _json(plan)
            if int(parsed['va'], 16) != int(config['function'], 16):
                raise ValueError('Control plan targets another function')
            if _hash(_verified(parsed['source'])) != _hash(candidate):
                raise ValueError('Control plan source differs from current candidate')
            result = mutate.run_plan(plan, output)
            return {'kind': 'controls', 'ok': result['status'] == 'controls-complete',
                    'source_sha256': _hash(candidate),
                    'result': result, 'artifacts': self._files(output)}
        va = int(config['function'], 16)
        lease = self._lease_task(job, config, root, retest=True)
        owner = lease['owner']
        task_id = lease['task_id']
        staged = api._candidate(task_id, va)
        coordination.atomic_bytes(staged, candidate.read_bytes())
        result = api.submit(task_id, owner, lease['token'], summary='Provider campaign candidate')
        worktrees._record_attempts(va, result)
        attempts = result.get('attempts') or []
        attempt = attempts[-1] if attempts else {}
        report_path = attempt.get('report_path')
        report_sha = attempt.get('report_sha256')
        if not isinstance(report_path, str) or not report_path or not isinstance(report_sha, str):
            output.mkdir(parents=True, exist_ok=False)
            error = ('task submit produced no compiled report_path; '
                     'inspect the retained compile exception before retrying')
            coordination.atomic_json(output / 'error.json', {
                'error': error,
                'task_id': task_id,
                'attempt': attempt.get('number'),
                'input_fingerprint': result.get('input_fingerprint'),
                'candidate_sha256': attempt.get('candidate_sha256'),
                'compile': attempt.get('compile'),
                'log': attempt.get('log'),
            })
            return {'kind': 'test', 'ok': False, 'error': error, 'task_id': task_id,
                    'artifacts': self._files(output)}
        report_pin = {'path': str(env.ROOT / report_path), 'sha256': report_sha}
        try:
            report = _json(_verified(report_pin))
        except (OSError, ValueError) as error:
            output.mkdir(parents=True, exist_ok=False)
            message = f'task report cannot be verified: {error}'
            coordination.atomic_json(output / 'error.json', {
                'error': message, 'task_id': task_id, 'attempt': attempt.get('number'),
                'report': report_pin})
            return {'kind': 'test', 'ok': False, 'error': message, 'task_id': task_id,
                    'artifacts': self._files(output)}
        output.mkdir(parents=True, exist_ok=False)
        coordination.atomic_json(output / 'report.json', report)
        coordination.atomic_json(output / 'submission.json', {
            'task_id': task_id, 'attempt': attempt['number'], 'input_fingerprint': result['input_fingerprint'],
            'source_sha256': attempt['candidate_sha256'], 'report': report_pin})
        return {'kind': 'test', 'ok': attempt['status'] == 'pass', 'task_id': task_id,
                'source_sha256': attempt['candidate_sha256'], 'report_status': report['status'],
                'attempt': attempt['number'], 'input_fingerprint': result['input_fingerprint'],
                'report': report_pin,
                **worktrees._report_feedback(report), 'artifacts': self._files(output)}

    def reserve(self, job):
        """Own the normal C task before paid authoring or a queued full test."""
        config, root = self._config(job)
        root.mkdir(parents=True, exist_ok=True)
        return self._lease_task(job, config, root, retest=False)['task_id']

    def _lease_task(self, job, config, root, *, retest):
        from . import tasks
        api = self.task_api or tasks
        va = int(config['function'], 16)
        owner = _task_owner(self.directory, job['id'])
        secret = root / 'task-lease.json'
        retained_lease = secret.exists()
        if retained_lease:
            lease = _json(secret)
            current = api.status(lease['task_id'])
        else:
            maximum = config.get('max_body_bytes', tasks.MAX_BODY_BYTES)
            current = api.prepare(va, max_body_bytes=maximum) if maximum != tasks.MAX_BODY_BYTES else api.prepare(va)
            lease = {'task_id': current['task_id'], 'owner': owner}
        if current['status'] == 'submitted':
            # Reconcile both reviewer revisions and submissions from prior jobs.
            # The task API preserves all evidence and the remaining attempt budget.
            attempts = current.get('attempts') or []
            if not attempts or attempts[-1].get('owner') != owner:
                raise ValueError(f"Task {lease['task_id']} is submitted by another owner; "
                                 'explicit coordinator reconciliation of the prior submission is required')
            if not retest:
                return lease
            try:
                api.revise(lease['task_id'], 'Provider requested a fresh validated candidate')
            except ValueError as error:
                raise ValueError(f"Task {lease['task_id']} is submitted and cannot be reopened: "
                                 f'{error}; inspect retained attempts before retrying') from error
            current = api.status(lease['task_id'])
        live = current.get('lease')
        import time
        if live and live.get('expires_at', 0) > time.time() and live.get('owner') != owner:
            raise ValueError('C task has a live lease owned by another worker')
        if not retained_lease or not live or live.get('expires_at', 0) <= time.time():
            current = api.claim(lease['task_id'], owner, 3600)
            lease.update(owner=owner, token=current['lease']['token'])
            coordination.atomic_json(secret, lease)
        return lease

    def _control_plan(self, job, config, root, action, output, records):
        """Pin data-only site mutations and run the existing control-plan gate."""
        if config['mode'] != 'c' or not config.get('self_service_controls'):
            raise ValueError('This C job is not authorized for source-site control proposals')
        from . import env, lift, mutate
        source = Path(config['candidate']).resolve(strict=True)
        if not source.is_relative_to(root):
            raise ValueError('Control source escapes the job')
        source_sha = _hash(source)
        tests = [_response(r) for r in records.values() if r.get('action', {}).get('kind') == 'test']
        if not any(r.get('ok') and r.get('source_sha256') == source_sha for r in tests):
            raise ValueError('Source-site controls require a passing current-source full test')
        rows = action.get('controls')
        if not isinstance(rows, list) or not 1 <= len(rows) <= 32:
            raise ValueError('Propose 1..32 finite named source-site controls')
        va = int(config['function'], 16)
        original = lift.discover(va)
        required = {pc for pc, insn in original.insns.items() if insn.target == 0x00945640}
        sites, controls = set(), []
        for row in rows:
            if (not isinstance(row, dict) or set(row) != {'site', 'name', 'transform', 'expect'}
                    or not isinstance(row['site'], str) or not re.fullmatch('0x[0-9a-fA-F]{8}', row['site'])
                    or not isinstance(row['expect'], dict) or row['expect'].get('kind') != 'mismatch'):
                raise ValueError('Each control needs an original site, exact transform and mismatch expectation')
            site = int(row['site'], 16)
            if site not in original.insns:
                raise ValueError('Control site is outside the original function instructions')
            sites.add(site)
            controls.append({key: row[key] for key in ('name', 'transform', 'expect')})
        if config.get('require_controls') and not required <= sites:
            raise ValueError('Every registered helper call site needs a named computation control')
        hints = env.HINTS_DIR / f'fn_{va:08x}.json'
        plan = {'schema': mutate.CONTROL_SCHEMA, 'va': f'{va:#010x}',
                'source': _pin(source), 'hints': _pin(hints) if hints.exists() else
                {'path': str(hints.resolve()), 'sha256': None},
                'identity': mutate.control_identity(va, source), 'controls': controls}
        output.mkdir(parents=True, exist_ok=False)
        plan_path = output / 'plan.json'
        coordination.atomic_json(plan_path, plan)
        coordination.atomic_json(output / 'sites.json', rows)
        # Validation precedes gate launch. Existing task leases and attempt
        # fences are checked by mutate.run_plan's ordinary diagnostic lane.
        mutate._plan(plan_path)
        result = mutate.run_plan(plan_path, output / 'gates')
        return {'kind': 'control-plan', 'ok': result['status'] == 'controls-complete',
                'source_sha256': source_sha, 'result': result,
                'artifacts': self._files(output)}

    @staticmethod
    def _files(directory):
        return {p.relative_to(directory).as_posix(): _pin(p)
                for p in sorted(Path(directory).rglob('*')) if p.is_file()}

    def _bundle(self, job, config, root, action, output, records):
        ids = action['artifacts']
        if not isinstance(ids, list) or not all(isinstance(item, str) for item in ids):
            return self._rejected('Proposal artifacts must be complete action IDs (aNNNN)')
        selected = []
        for action_id in ids:
            if action_id not in records:
                return self._rejected(f'Unknown action ID {action_id}; propose completed action IDs')
            selected.append(records[action_id])
        if not selected:
            return self._rejected('Select at least one completed probe or test action')
        artifacts = self._artifact_map(root, {r['id']: r for r in selected})
        checks = ['original-provenance', 'frozen-artifact-hashes', 'goal-and-unknowns']
        payload = {'job': job['id'], 'function': config['function'], 'mode': config['mode'],
                   'summary': action['summary'], 'artifacts': artifacts,
                   'execution': self._execution(), 'records': selected}
        if config['mode'] == 'input':
            result = self._input_bundle(payload, config, selected, artifacts)
            if result is not None:
                return result
            checks += ['original-return', 'required-path', 'import-boundaries', 'fixture-data-only']
            if config.get('adoption_guard') is not None:
                from .pipeline_adoption import validate_resolution
                try:
                    validate_resolution(config, action.get('resolution'))
                except (ValueError, KeyError) as error:
                    return self._rejected(str(error))
                payload['adoption_guard'] = config['adoption_guard']
                payload['resolution'] = action['resolution']
                checks += ['named-input-obligation-resolution', 'fixture-prefix-preservation']
                if config['adoption_guard'].get('source'):
                    checks.append('existing-source-reconciliation')
        else:
            result = self._c_bundle(payload, config, selected, artifacts, root)
            if result is not None:
                return result
            checks += ['instruction-source-equivalence', 'physical-abi', 'complete-emitted-O0-O2',
                       'full-differential-gate', 'sensitivity-obligations']
        payload['required_checks'] = checks
        output.mkdir(parents=True, exist_ok=False)
        coordination.atomic_json(output / 'bundle.json', payload)
        return {'ready': True, 'sha256': _hash(output / 'bundle.json'), 'pin': _pin(output / 'bundle.json'),
                'required_checks': checks, 'payload': payload}

    def _input_bundle(self, payload, config, selected, artifacts):
        function = _to_int(config['function'])
        required = {_to_int(value) for value in config.get('required_pcs', [])}
        minimum = config.get('min_coverage', 0)
        probes = []
        matching = []
        fixtures = []
        raws = []
        observations = []
        for record in selected:
            if record.get('action', {}).get('kind') != 'probe':
                continue
            result = _response(record)
            if not isinstance(result, dict) or not result.get('ok'):
                continue
            body = result.get('result') if isinstance(result.get('result'), dict) else {}
            summary = body.get('summary')
            if not isinstance(summary, dict):
                continue
            probes.append(record)
            request = record.get('action', {}).get('request', {})
            probe_function = body.get('function', request.get('function'))
            if _to_int(probe_function) != function:
                return self._rejected('Probe function differs from the configured goal function')
            pins = result.get('artifacts') if isinstance(result.get('artifacts'), dict) else {}
            raw_entry = next(((name, pin) for name, pin in pins.items()
                              if name.endswith('raw.json.gz') or name.endswith('raw.json')), None)
            fixture_entry = next(((name, pin) for name, pin in pins.items()
                                  if name.endswith('fixture.json')), None)
            if raw_entry is None:
                return self._rejected('A retained raw original result is required for review')
            raw_path = _verified(raw_entry[1], None)
            try:
                raw = _read_json_maybe_gzip(raw_path)
            except (OSError, ValueError, json.JSONDecodeError) as error:
                return self._rejected(f'Retained raw result cannot be inspected: {error}')
            facts = _raw_facts(raw)
            expected_identity = self._execution()
            identity = (facts.get('execution_identity')
                        or summary.get('execution_identity')
                        or body.get('execution_identity'))
            if identity is None:
                return self._rejected('The retained probe lacks an execution identity')
            if identity != expected_identity:
                return self._rejected('Execution identity differs from the pinned runtime')
            for key in ('outcome', 'covered_pcs', 'coverage', 'import_count'):
                original, described = facts.get(key), summary.get(key)
                if key == 'covered_pcs' and original is not None:
                    original = {_to_int(pc) for pc in original}
                    described = {_to_int(pc) for pc in described or []}
                if key in facts and original != described:
                    return self._rejected(f'Retained raw {key} differs from the mutable summary')
            if 'covered_pcs' not in facts or 'import_count' not in facts:
                return self._rejected('Retained raw witness lacks covered PCs or import observations')
            execution_pin = next((pin for name, pin in pins.items()
                                  if name.endswith('execution.json')), None)
            if execution_pin is not None:
                metadata = _json(_verified(execution_pin))
                if (metadata.get('execution_identity') != expected_identity
                        or _to_int(metadata.get('function')) != function):
                    return self._rejected('Retained execution metadata targets different inputs')
                starts = metadata.get('own_instruction_count')
                if starts:
                    measured = len({_to_int(pc) for pc in facts['covered_pcs']}) / starts
                    if abs(measured - summary.get('coverage', -1)) > 1e-12:
                        return self._rejected('Retained raw coverage differs from the mutable summary')
            pcs = {_to_int(value) for value in summary.get('covered_pcs', [])}
            imports = summary.get('import_count')
            if (summary.get('outcome') == 'return' and required <= pcs
                    and summary.get('coverage', 0) >= minimum
                    and (not config.get('require_no_imports', True) or imports == 0)):
                matching.append(summary)
                if fixture_entry is not None:
                    fixture_path = _verified(fixture_entry[1], None)
                    try:
                        fixture = _read_json_maybe_gzip(fixture_path)
                    except (OSError, ValueError, json.JSONDecodeError) as error:
                        return self._rejected(f'Retained fixture cannot be inspected: {error}')
                    fixtures.append({'action': record['id'],
                                     'id': f"{record['id']}/{fixture_entry[0]}",
                                     'pin': fixture_entry[1], 'data': fixture})
                raws.append({'action': record['id'], 'id': f"{record['id']}/{raw_entry[0]}",
                             'pin': raw_entry[1]})
                observations.append({
                    'action': record['id'],
                    'summary': summary,
                    'instructions': _bounded(_find_key(raw, ('instructions', 'insns')) or []),
                    'arrivals': _bounded(_find_key(raw, ('arrivals',)) or []),
                    'returns': _bounded(_find_key(raw, ('returns',)) or []),
                })
        if not probes:
            return self._rejected('A successful retained original probe is required')
        if not matching:
            return self._rejected('No retained return satisfies required PCs, coverage and import policy')
        if not fixtures:
            return self._rejected('The bundle needs the complete selected fixture for review')
        payload['observations'] = observations
        payload['fixtures'] = fixtures
        payload['raw'] = raws
        return None

    def _build_source_hash(self, result):
        body = result.get('result') if isinstance(result.get('result'), dict) else {}
        build = body.get('build') if isinstance(body.get('build'), dict) else {}
        for container in (body, build, result):
            if not isinstance(container, dict):
                continue
            for key in ('source_sha256', 'sourcehash', 'source_hash'):
                value = container.get(key)
                if isinstance(value, str) and len(value) == 64:
                    return value.lower()
        return None

    def _c_bundle(self, payload, config, selected, artifacts, root):
        candidate = _verified(_pin(config['candidate']), root)
        from .source_policy import inline_assembly_lines
        if inline_assembly_lines(candidate.read_text(encoding='utf-8')):
            return self._rejected('New pipeline C must be plain C; inline assembly belongs to '
                                  'reviewed legacy ABI records', blocker='plain-c-required')
        source_hash = _hash(candidate)
        tests = [r for r in selected
                 if r.get('action', {}).get('kind') == 'test'
                 and isinstance(_response(r), dict) and _response(r).get('ok')]
        builds = [r for r in selected
                  if r.get('action', {}).get('kind') == 'build'
                  and isinstance(_response(r), dict) and _response(r).get('ok')]
        if not tests or not builds:
            return self._rejected('Current source needs a passing full test and retained O0/O2 builds')
        test = _response(tests[-1])
        if test.get('source_sha256') != source_hash:
            return self._rejected('The passing task report is for a different source hash')
        if not _passing(test.get('report_status')):
            return self._rejected('The authoritative task report is not a pass')
        for build in builds:
            if self._build_source_hash(_response(build)) != source_hash:
                return self._rejected('Retained build source differs from current candidate')
        if config.get('require_controls'):
            matching = [r for r in selected
                if r.get('action', {}).get('kind') in ('controls', 'control-plan')
                and isinstance(_response(r), dict) and _response(r).get('ok')
                and _response(r).get('source_sha256') == source_hash]
            if not matching:
                return self._rejected('The assigned named sensitivity controls must complete',
                                      blocker='controls-required')
            from . import mutate
            payload['sensitivity'] = []
            for record in matching:
                body = _response(record)
                receipt = copy.deepcopy(body.get('result'))
                try:
                    controller = next(pin for name, pin in body['artifacts'].items()
                                      if name.endswith('/controller.json') or name == 'controller.json')
                    control_root = _verified(controller, root).parent
                    retained = _json(_verified(controller, root))
                    if retained != receipt:
                        raise ValueError('Control receipt differs from retained controller')
                    plan_path = control_root / 'plan.json'
                    plan, texts = mutate._plan(plan_path)
                    if (plan['source']['sha256'] != source_hash
                            or plan['identity'] != mutate.control_identity(
                                int(config['function'], 16), _verified(plan['source']))):
                        raise ValueError('Control source, inputs or execution identity changed')
                    mutate._resume_controls(receipt, plan, texts, control_root, receipt['plan_sha256'])
                    if receipt.get('status') != 'controls-complete':
                        raise ValueError('Source-site controls are incomplete')
                except (ValueError, KeyError, TypeError, StopIteration, OSError) as error:
                    return self._rejected(f'Current-source control evidence required: {error}',
                                          blocker='controls-required')
                payload['sensitivity'].append({'action': record['id'], 'plan': plan,
                    'controller': controller, 'stages': {name: {
                        key: value for key, value in stage.items()
                        if key in ('classification', 'reason', 'witnesses', 'report', 'source')}
                        for name, stage in receipt['stages'].items()}})
        payload['source'] = {'pin': _pin(candidate), 'text': candidate.read_text(encoding='utf-8')}
        payload['task_id'] = test.get('task_id')
        emitted = {key: _verified(pin, root).read_text(encoding='utf-8')
                   for key, pin in artifacts.items() if key.endswith('.disasm.txt')}
        if len(emitted) < 2:
            return self._rejected('Complete emitted O0/O2 disassembly is required for independent review')
        if len(set(emitted.values())) < 2:
            return self._rejected('The O0 and O2 emitted disassembly must be distinct and complete')
        lowered = [key.lower() for key in emitted]
        if not (any('o0' in key for key in lowered) and any('o2' in key for key in lowered)):
            return self._rejected('The retained disassembly must name both O0 and O2 outputs')
        task = {'task_id': test.get('task_id'), 'attempt': test.get('attempt'),
                'input_fingerprint': test.get('input_fingerprint'),
                'source_sha256': test.get('source_sha256'),
                'report': test.get('report')}
        authoritative = test.get('report')
        if not isinstance(authoritative, dict) or not isinstance(authoritative.get('sha256'), str):
            return self._rejected('Current test must pin its authoritative task report')
        report_pin = next((pin for key, pin in artifacts.items()
                           if key.endswith('/report.json')
                           and pin.get('sha256') == authoritative['sha256']), None)
        if report_pin is None:
            return self._rejected('Retained authoritative task report is required')
        report = _read_json_maybe_gzip(_verified(report_pin, root))
        if not _passing(report.get('status')):
            return self._rejected('The retained task report status is not a pass')
        if (report.get('source_sha256') != source_hash
                or _to_int(report.get('va', 0)) != _to_int(config['function'])):
            return self._rejected('Retained task report differs from the current source or function')
        from .semantic import RETURN_KINDS
        if report.get('returns') not in RETURN_KINDS:
            return self._rejected('Task report must identify the tested entry return type',
                                  blocker='return-provenance-needed')
        if report['returns'] == 'void':
            from . import inventory
            va = _to_int(config['function'])
            if not any(va in targets for targets in inventory.load().callees.values()):
                return self._rejected(
                    'Void entry with no indexed direct callers needs original-byte indirect-use '
                    'proof outside automatic publication; preserve original EAX or hold. '
                    'A destructor-shaped body is not return provenance.',
                    blocker='return-provenance-needed')
        submission_pin = next((pin for key, pin in artifacts.items()
                               if key.endswith('/submission.json')), None)
        if submission_pin is not None:
            submission = _read_json_maybe_gzip(_verified(submission_pin, root))
            if task.get('attempt') is None:
                task['attempt'] = submission.get('attempt')
            if task.get('input_fingerprint') is None:
                task['input_fingerprint'] = submission.get('input_fingerprint')
            if task.get('report') is None:
                task['report'] = submission.get('report')
        payload['task'] = task
        payload['emitted'] = emitted
        return None

    @staticmethod
    def _rejected(reason, *, blocker=None):
        result = {'ready': False, 'sha256': _digest({'rejected': reason}),
                  'required_checks': [], 'feedback': reason}
        if blocker is not None:
            result['blocker'] = blocker
        return result

    @staticmethod
    def _execution():
        from . import recover
        return recover.execution_identity()

    def _finalize(self, job, config, root, action, output):
        bundle = action['bundle']
        path = _verified(bundle['pin'], root)
        if _hash(path) != bundle['sha256']:
            raise ValueError('Frozen bundle hash changed')
        payload = _json(path)
        if bundle['required_checks'] != payload['required_checks']:
            raise ValueError('Review requirements differ from the frozen bundle')
        if payload['execution'] != self._execution():
            raise ValueError('Execution identity changed since proposal')
        for pin in payload['artifacts'].values():
            _verified(pin, root)
        review = action['review']
        if (review.get('verdict') != 'pass' or review.get('unknowns') != []
                or review.get('bundle_sha256') != bundle['sha256']
                or not set(bundle['required_checks']) <= set(review.get('checks', []))):
            raise ValueError('Independent review does not approve this exact bundle')
        from . import provider_jobs
        state = provider_jobs.load(self.directory)
        original = state['jobs'][job['id']]
        reviewer = action['review_model']
        expected = state['manifest']['roles'][config['reviewer']]['model']
        if reviewer != expected or reviewer == original.get('author_model'):
            raise ValueError('Independent review model identity is invalid')
        output.mkdir(parents=True, exist_ok=False)
        coordination.atomic_json(output / 'review.json', action)
        if config['mode'] == 'input':
            return {'state': 'ready', 'bundle': bundle['pin'], 'review': _pin(output / 'review.json'),
                    'credit': 'Reviewed original-only input evidence; no C acceptance or integration'}
        _verified(payload['source']['pin'], root)
        from . import tasks
        api = self.task_api or tasks
        task = payload.get('task') if isinstance(payload.get('task'), dict) else {}
        task_id = payload.get('task_id') or task.get('task_id')
        if not isinstance(task_id, str) or not task_id:
            raise ValueError('Frozen bundle has no authoritative task identity')
        source_sha = payload['source']['pin']['sha256']
        with coordination.lock(self.directory / 'publication.lock'):
            current = api.status(task_id)
            if current.get('status') != 'submitted':
                raise ValueError('Task is not awaiting publication')
            attempts = current.get('attempts') or []
            latest = attempts[-1] if attempts else None
            if not isinstance(latest, dict):
                raise ValueError('Reviewed task has no current attempt')
            if latest.get('number') != task.get('attempt'):
                raise ValueError('Reviewed task attempt is not the current attempt')
            if latest.get('status') != 'pass':
                raise ValueError('Current task attempt is not a pass')
            if latest.get('candidate_sha256') != source_sha:
                raise ValueError('Task source differs from the independently reviewed source')
            report = task.get('report')
            if not isinstance(report, dict):
                raise ValueError('Reviewed task report pin is required')
            report_path = _verified(report)
            report_sha = latest.get('report_sha256')
            if not isinstance(report_sha, str) or _hash(report_path) != report_sha:
                raise ValueError('Task report differs from the independently reviewed report')
            fingerprint = task.get('input_fingerprint')
            if not isinstance(fingerprint, str) or not fingerprint:
                raise ValueError('Reviewed task input fingerprint is required')
            if current.get('input_fingerprint') != fingerprint:
                raise ValueError('Task input fingerprint changed since review')
            if _hash(Path(config['candidate'])) != source_sha:
                raise ValueError('Candidate changed since the reviewed proposal')
            result = api.publish(task_id)
        accepted = result['status'] == 'published'
        return {'state': 'accepted' if accepted else 'held', 'task_id': task_id,
                'reason': 'Accepted by existing CMRE gate' if accepted else 'Publication gate refused acceptance'}
