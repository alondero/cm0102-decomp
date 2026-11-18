"""Typed farm evidence consumed only inside the authoritative acceptance lease."""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import json
import os
from pathlib import Path
import re

from . import env

SCHEMA = 'cmre-farm-acceptance/2'
DISPATCH_SCHEMA = 'cmre-farm-dispatch/1'


def digest_json(value) -> str:
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(',', ':')).encode()).hexdigest()


def validate_worker_hints(before: bytes | None, after: bytes | None) -> None:
    """Shared hints remain byte-independent policy; only reviewed C input may change."""
    if before == after:
        return
    if after is None:
        raise ValueError('worker deleted the hints file')
    old = json.loads(before) if before is not None else {}
    new = json.loads(after)
    if not isinstance(old, dict) or not isinstance(new, dict):
        raise ValueError('hints must be objects')

    def shared(hints):
        return json.dumps({k: v for k, v in hints.items() if k != 'c_hints'}, sort_keys=True, allow_nan=False)

    if shared(old) != shared(new):
        raise ValueError('worker changed shared hints outside c_hints')
    from . import difftest

    dimensions = {k: new[k] for k in ('arg_count', 'arena_size') if k in new}
    difftest.validate_hints({**dimensions, 'c_hints': new.get('c_hints', {})}, worker=True)


def optional_bytes(path: Path) -> bytes | None:
    try:
        return path.read_bytes()
    except FileNotFoundError:
        return None


def optional_digest(data: bytes | None) -> str | None:
    return env.digest_bytes(data) if data is not None else None


def _sha(value) -> bool:
    return isinstance(value, str) and re.fullmatch('[0-9a-f]{64}', value) is not None


@dataclass(frozen=True)
class Artifact:
    path: Path
    sha256: str | None

    @classmethod
    def parse(cls, value, run: Path) -> Artifact:
        if not isinstance(value, dict) or set(value) != {'path', 'sha256'}:
            raise ValueError('Malformed farm acceptance artifact')
        if (not isinstance(value['path'], str) or not Path(value['path']).is_absolute()
                or (value['sha256'] is not None and not _sha(value['sha256']))):
            raise ValueError('Malformed farm acceptance artifact identity')
        path = Path(value['path']).resolve()
        if not path.is_relative_to(run):
            raise ValueError('Farm acceptance artifact escapes the run')
        return cls(path, value['sha256'])

    def checked_bytes(self) -> bytes | None:
        raw = optional_bytes(self.path)
        if optional_digest(raw) != self.sha256:
            raise ValueError(f'Farm acceptance artifact changed: {self.path}')
        return raw


def _in_run(run: Path, root: Path, value: str) -> Path:
    path = (root / value).resolve()
    if not path.is_relative_to(run.resolve()):
        raise ValueError('Farm authority artifact escapes the run')
    return path


def _identity(snapshot: dict) -> dict:
    return {key: optional_digest(snapshot[key]) for key in ('c', 'hint')}


def _audit_review(run, root, fn, identity, authority, *, confirming=False, expected_verdict='defect'):
    """Validate the existing audit dispatch format and its bound verdict."""
    if not isinstance(authority, dict):
        raise ValueError('Origin audit authority must be an object')
    directory = _in_run(run, root, authority['snapshot'])
    raw = (directory / 'identity.json').read_bytes()
    if optional_digest(raw) != authority['snapshot_metadata_sha256']:
        raise ValueError('Origin review dispatch metadata changed')
    metadata = json.loads(raw)
    if (not isinstance(metadata, dict) or metadata.get('kind') != 'audit' or metadata.get('fn') != fn
            or metadata.get('identity') != identity
            or type(metadata.get('n')) is not int
            or directory != (run / 'audit/snapshots' / str(metadata['n'])).resolve()
            or _identity({'c': optional_bytes(directory / 'source.c'),
                          'hint': optional_bytes(directory / 'hints.json')}) != identity
            or identity.get('c') is None):
        raise ValueError('Origin review function or snapshot identity mismatch')
    if (metadata.get('bundle') != authority.get('bundle')
            or metadata.get('bundle_sha256') != authority.get('bundle_sha256')):
        raise ValueError('Origin review bundle was not bound at dispatch')
    if metadata.get('bundle'):
        bundle = _in_run(run, root, metadata['bundle'])
        if optional_digest(bundle.read_bytes()) != metadata['bundle_sha256']:
            raise ValueError('Origin review bundle changed')
    verdict_path = _in_run(run, root, authority['audit_path'])
    verdict_raw = verdict_path.read_bytes()
    if optional_digest(verdict_raw) != authority['verdict_sha256']:
        raise ValueError('Origin review verdict changed')
    verdict = json.loads(verdict_raw)
    if (not isinstance(verdict, dict) or verdict.get('verdict') != expected_verdict
            or not isinstance(verdict.get('findings', []), list)
            or (expected_verdict == 'defect' and not verdict.get('findings'))):
        raise ValueError('Origin review lacks the required verdict and findings')
    if metadata.get('confirm'):
        if confirming:
            raise ValueError('Recursive origin confirmation is invalid')
        _audit_review(run, root, fn, identity, metadata['audit_state']['review'], confirming=True)
    return verdict.get('findings', [])


def check_origin(run: Path, root: Path, fn: str, identity: dict, kind: str, origin: dict | None):
    if kind == 'translate':
        if origin is not None:
            raise ValueError('Translation cannot carry an origin review')
        return
    if not isinstance(origin, dict):
        raise ValueError('Review-derived dispatch requires an immutable origin')
    if not isinstance(origin.get('findings'), list):
        raise ValueError('Review-derived dispatch requires bound findings')
    if kind == 'fix':
        findings = _audit_review(run, root, fn, identity, origin['review'])
        if findings != origin['findings']:
            raise ValueError('Origin review does not authorize the dispatched findings')
    elif kind == 'adopt':
        artifact = Artifact.parse(origin['receipt'], run.resolve())
        raw = artifact.checked_bytes()
        receipt = json.loads(raw)
        directory = artifact.path.parent
        if not isinstance(receipt, dict):
            raise ValueError('Adoption review must be an object')
        view = receipt.get('review', {})
        if (receipt.get('schema') != 'cmre-farm-review/2' or receipt.get('fn') != fn
                or receipt.get('identity') != identity or identity.get('c') is None
                or directory.parent != (run / 'reviews').resolve()
                or artifact.path.name != 'receipt.json'
                or _identity({'c': optional_bytes(directory / 'source.c'),
                              'hint': optional_bytes(directory / 'hints.json')}) != identity
                or not isinstance(view, dict) or view.get('fn') != fn or view.get('identity') != identity
                or view.get('status') not in ('bound', 'fresh-review-required')
                or view.get('open_findings') != origin['findings']):
            raise ValueError('Adoption origin review schema/function/identity mismatch')
        # A fresh manual claim names inputs, not a machine audit verdict. Bound
        # audit claims must additionally carry a real, input-bound verdict.
        if view['status'] == 'bound':
            if view.get('verdict') not in ('ok', 'defect', 'unconfirmed'):
                raise ValueError('Adoption audit claim lacks a valid verdict')
            if receipt.get('verdict') != view['verdict']:
                raise ValueError('Adoption claim verdict differs from the reviewed origin')
            _audit_review(run, root, fn, identity, receipt['audit_origin'],
                          expected_verdict='defect' if view['verdict'] == 'unconfirmed' else view['verdict'])
        elif receipt.get('verdict') != 'input-claim' or receipt.get('audit_origin') is not None:
            raise ValueError('Fresh manual input claims cannot assert an audit verdict')
    else:
        raise ValueError('Unknown dispatch kind')


def register_dispatch(run: Path, root: Path, fn: str, job: int, kind: str,
                      snapshot: dict, *, origin: dict | None = None) -> dict:
    """Controller-only, write-once admission BEFORE model dispatch/input gates.

    Acceptance never calls this function, and old pending work must not acquire
    a new authority from its already modified canonical inputs.
    """
    run, root = Path(run).resolve(), Path(root).resolve()
    check_origin(run, root, fn, _identity(snapshot), kind, origin)
    directory = run / 'dispatch' / f'{job:08d}'
    directory.mkdir(parents=True, exist_ok=False)
    for key, name in (('c', 'source.c'), ('hint', 'hints.json')):
        if snapshot[key] is not None:
            with (directory / name).open('xb') as stream:
                stream.write(snapshot[key])
                stream.flush()
                os.fsync(stream.fileno())
    receipt = {'schema': DISPATCH_SCHEMA, 'fn': fn, 'job': job, 'kind': kind,
               'root': str(root), 'identity': _identity(snapshot), 'origin': origin}
    path = directory / 'receipt.json'
    with path.open('xb') as stream:
        stream.write(json.dumps(receipt, sort_keys=True).encode())
        stream.flush()
        os.fsync(stream.fileno())
    return {'path': str(path), 'sha256': env.digest(path)}


@dataclass(frozen=True)
class FarmAcceptance:
    va: int
    job: int
    kind: str
    source_sha256: str
    hint_sha256: str | None
    dispatch: Artifact
    canonical_gate: Artifact
    operand_proof: Artifact | None

    @classmethod
    def load(cls, path: Path) -> FarmAcceptance:
        path = path.resolve()
        if path.parent.name != 'acceptance':
            raise ValueError('Expected a local RUN/acceptance request')
        raw = json.loads(path.read_bytes())
        fields = {'schema', 'fn', 'job', 'kind', 'source_sha256', 'hint_sha256', 'dispatch',
                  'canonical_gate', 'operand_proof'}
        if not isinstance(raw, dict) or set(raw) != fields or raw['schema'] != SCHEMA:
            raise ValueError('Unknown farm acceptance request schema or fields')
        if (not isinstance(raw['fn'], str) or not re.fullmatch(r'fn_[0-9a-f]{8}', raw['fn'])
                or type(raw['job']) is not int or raw['job'] < 0
                or raw['kind'] not in ('translate', 'fix', 'adopt')
                or not _sha(raw['source_sha256'])
                or (raw['hint_sha256'] is not None and not _sha(raw['hint_sha256']))
                ):
            raise ValueError('Invalid farm acceptance request identity')
        run = path.parent.parent
        gate = Artifact.parse(raw['canonical_gate'], run)
        operand = None if raw['operand_proof'] is None else Artifact.parse(raw['operand_proof'], run)
        dispatch = Artifact.parse(raw['dispatch'], run)
        if (gate.sha256 is None or (operand is not None and operand.sha256 is None)
                or dispatch.sha256 is None
                or dispatch.path != (run / 'dispatch' / f"{raw['job']:08d}" / 'receipt.json').resolve()):
            raise ValueError('Incomplete farm acceptance evidence')
        return cls(int(raw['fn'][3:], 16), raw['job'], raw['kind'], raw['source_sha256'],
                   raw['hint_sha256'], dispatch, gate, operand)

    def check_inputs(self) -> None:
        fn = f'fn_{self.va:08x}'
        source = env.C_DIR / f'{fn}.c'
        hints = optional_bytes(env.HINTS_DIR / f'{fn}.json')
        if optional_digest(optional_bytes(source)) != self.source_sha256:
            raise ValueError('Canonical source changed before farm acceptance')
        if optional_digest(hints) != self.hint_sha256:
            raise ValueError('Canonical hints changed before farm acceptance')
        registered = json.loads(self.dispatch.checked_bytes())
        root = env.ROOT.resolve()
        if (not isinstance(registered, dict) or registered.get('schema') != DISPATCH_SCHEMA or registered.get('fn') != fn
                or registered.get('job') != self.job or registered.get('kind') != self.kind
                or registered.get('root') != str(root)):
            raise ValueError('Dispatch authority schema/function/job/kind/root mismatch')
        directory = self.dispatch.path.parent
        snapshot = {'c': optional_bytes(directory / 'source.c'),
                    'hint': optional_bytes(directory / 'hints.json')}
        if registered.get('identity') != _identity(snapshot):
            raise ValueError('Immutable dispatch snapshot changed')
        validate_worker_hints(snapshot['hint'], hints)
        check_origin(directory.parent.parent, root, fn, registered['identity'],
                     self.kind, registered['origin'])

    def check_gate(self, report: dict) -> None:
        self.check_inputs()
        saved = json.loads(self.canonical_gate.checked_bytes())
        fields = ('va', 'tier', 'source', 'source_sha256', 'execution_identity', 'key', 'profile',
                  'effective_options_sha256', 'source_dependencies', 'preprocessed_sha256')
        if (not isinstance(saved, dict) or saved.get('status') != 'pass'
                or any(field not in saved or saved[field] != report.get(field) for field in fields)
                or not _sha(saved.get('effective_options_sha256'))
                or saved['source_sha256'] != self.source_sha256
                or saved['va'] != f'0x{self.va:08x}' or saved['tier'] != 'c'):
            raise ValueError('Farm gate evidence differs from the current canonical C gate')
        for opt, build in report['builds'].items():
            before = saved.get('builds', {}).get(opt, {})
            tested = before.get('difftest', {})
            if (before.get('status') != 'pass' or not _sha(tested.get('result_digest'))
                    or tested.get('result_digest') != build['difftest']['result_digest']):
                raise ValueError(f'Farm gate differential evidence changed for {opt}')
        if self.operand_proof is not None:
            self.operand_proof.checked_bytes()
