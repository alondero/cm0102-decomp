"""Typed farm evidence consumed only inside the authoritative acceptance lease."""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import json
from pathlib import Path
import re

from . import env

SCHEMA = 'cmre-farm-acceptance/1'


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


@dataclass(frozen=True)
class FarmAcceptance:
    va: int
    job: int
    kind: str
    source_sha256: str
    hint_sha256: str | None
    prior_hint: bytes | None
    canonical_gate: Artifact
    operand_proof: Artifact | None
    reviews: tuple[Artifact, ...]

    @classmethod
    def load(cls, path: Path) -> FarmAcceptance:
        path = path.resolve()
        if path.parent.name != 'acceptance':
            raise ValueError('Expected a local RUN/acceptance request')
        raw = json.loads(path.read_bytes())
        fields = {'schema', 'fn', 'job', 'kind', 'source_sha256', 'hint_sha256', 'prior_hint',
                  'canonical_gate', 'operand_proof', 'reviews'}
        if not isinstance(raw, dict) or set(raw) != fields or raw['schema'] != SCHEMA:
            raise ValueError('Unknown farm acceptance request schema or fields')
        if (not isinstance(raw['fn'], str) or not re.fullmatch(r'fn_[0-9a-f]{8}', raw['fn'])
                or type(raw['job']) is not int or raw['job'] < 0
                or raw['kind'] not in ('translate', 'fix', 'adopt')
                or not _sha(raw['source_sha256'])
                or (raw['hint_sha256'] is not None and not _sha(raw['hint_sha256']))
                or (raw['prior_hint'] is not None and not isinstance(raw['prior_hint'], str))
                or not isinstance(raw['reviews'], list)):
            raise ValueError('Invalid farm acceptance request identity')
        run = path.parent.parent
        gate = Artifact.parse(raw['canonical_gate'], run)
        operand = None if raw['operand_proof'] is None else Artifact.parse(raw['operand_proof'], run)
        reviews = tuple(Artifact.parse(item, run) for item in raw['reviews'])
        if (gate.sha256 is None or (operand is not None and operand.sha256 is None)
                or (raw['kind'] in ('fix', 'adopt') and not any(review.sha256 for review in reviews))
                or (raw['kind'] == 'translate' and reviews)
                or len({item.path for item in reviews}) != len(reviews)):
            raise ValueError('Incomplete farm acceptance evidence')
        prior_hint = bytes.fromhex(raw['prior_hint']) if raw['prior_hint'] is not None else None
        return cls(int(raw['fn'][3:], 16), raw['job'], raw['kind'], raw['source_sha256'],
                   raw['hint_sha256'], prior_hint, gate, operand, reviews)

    def check_inputs(self) -> None:
        fn = f'fn_{self.va:08x}'
        source = env.C_DIR / f'{fn}.c'
        hints = optional_bytes(env.HINTS_DIR / f'{fn}.json')
        if optional_digest(optional_bytes(source)) != self.source_sha256:
            raise ValueError('Canonical source changed before farm acceptance')
        if optional_digest(hints) != self.hint_sha256:
            raise ValueError('Canonical hints changed before farm acceptance')
        validate_worker_hints(self.prior_hint, hints)
        for review in self.reviews:
            review.checked_bytes()

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
