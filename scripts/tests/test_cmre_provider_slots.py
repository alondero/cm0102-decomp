"""Provider slot provisioning uses fake credentials and temporary directories."""
import importlib.util
import json
from pathlib import Path

import pytest

MODULE = Path(__file__).resolve().parents[1] / 'cmre_provider_slots.py'
spec = importlib.util.spec_from_file_location('provider_slots', MODULE)
slots = importlib.util.module_from_spec(spec)
spec.loader.exec_module(slots)


def profiles(tmp_path):
    value = {}
    for role in ('deepseek', 'minimax'):
        data = tmp_path / 'base' / role / 'data'
        (data / 'opencode').mkdir(parents=True, exist_ok=True)
        (data / 'opencode' / 'auth.json').write_text(f'{{"token":"{role}"}}')
        value[role] = {
            'executable': 'C:/fake/opencode.exe',
            'model': f'{role}/model',
            'client_version': '1.18.3',
            'environment': {
                'XDG_DATA_HOME': str(data),
                'XDG_STATE_HOME': str(tmp_path / 'base' / role / 'state'),
                'XDG_CONFIG_HOME': str(tmp_path / 'base' / role / 'config'),
                'XDG_CACHE_HOME': str(tmp_path / 'base' / role / 'cache'),
            },
        }
    return value


def test_provision_isolates_every_slot(tmp_path):
    report = slots.provision(profiles(tmp_path), tmp_path / 'slots', 3)
    assert report['slots'] == 3
    seen = set()
    for role in ('deepseek', 'minimax'):
        for slot in range(3):
            auth = tmp_path / 'slots' / role / str(slot) / 'data' / 'opencode' / 'auth.json'
            assert auth.is_file()
            assert auth.read_text() == f'{{"token":"{role}"}}'
    for entry in report['profiles']:
        value = json.loads(Path(entry['path']).read_text())
        for role in ('deepseek', 'minimax'):
            data = value[role]['environment']['XDG_DATA_HOME']
            assert data.endswith(f'{role}\\{entry["slot"]}\\data') or data.endswith(
                f'{role}/{entry["slot"]}/data')
            assert data not in seen
            seen.add(data)
    assert len(seen) == 6


def test_provision_refuses_missing_credentials(tmp_path):
    value = profiles(tmp_path)
    (tmp_path / 'base' / 'minimax' / 'data' / 'opencode' / 'auth.json').unlink()
    with pytest.raises(ValueError, match='credential'):
        slots.provision(value, tmp_path / 'slots', 2)


def test_provision_allows_missing_credentials_when_not_required(tmp_path):
    value = profiles(tmp_path)
    (tmp_path / 'base' / 'minimax' / 'data' / 'opencode' / 'auth.json').unlink()
    report = slots.provision(value, tmp_path / 'slots', 2, require_credentials=False)
    assert report['roles']['minimax']['copied'] is False
    assert report['roles']['deepseek']['copied'] is True


def test_split_is_disjoint_and_concurrency_one(tmp_path):
    manifest = {
        'schema': 'cmre-provider-campaign/1',
        'duration_seconds': 3600,
        'concurrency': 2,
        'roles': {'deepseek': {'model': 'a'}, 'minimax': {'model': 'b'}},
        'jobs': [{'id': f'job-{index}', 'config': {'function': '0x1'}} for index in range(5)],
    }
    report = slots.split(manifest, tmp_path / 'manifests', 2)
    seen = []
    for entry in report['manifests']:
        payload = json.loads(Path(entry['path']).read_text())
        assert payload['concurrency'] == 1
        assert payload['duration_seconds'] == 3600
        assert payload['roles'] == manifest['roles']
        seen.extend(job['id'] for job in payload['jobs'])
    assert sorted(seen) == sorted(job['id'] for job in manifest['jobs'])
    assert len(seen) == len(set(seen))


def test_slot_bounds(tmp_path):
    with pytest.raises(ValueError, match='slots'):
        slots.provision(profiles(tmp_path), tmp_path / 'slots', 0)
    with pytest.raises(ValueError, match='slots'):
        slots.provision(profiles(tmp_path), tmp_path / 'slots', 9)
