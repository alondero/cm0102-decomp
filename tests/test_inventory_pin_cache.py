"""Inventory pin checks cover in-process and on-disk cache hits."""
import json
import subprocess
import sys
from pathlib import Path

import pytest

from cmre import env, inventory


@pytest.fixture
def pinned_inventory(monkeypatch, tmp_path):
    files = [tmp_path / n for n in ('functions.tsv', 'calls.tsv', 'modules.json')]
    files[0].write_text('entry_va\tname\tbody_bytes\tsignature\n0051a150\troot\t1\tvoid\n', encoding='utf-8')
    files[1].write_text('function_va\ttarget_address\n', encoding='utf-8')
    files[2].write_text('[]', encoding='utf-8')
    monkeypatch.setattr(env, 'FUNCTIONS_TSV', files[0])
    monkeypatch.setattr(env, 'CALLS_TSV', files[1])
    monkeypatch.setattr(env, 'MODULES_JSON', files[2])
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {p: env.digest(p) for p in files})
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'rel', lambda p: str(p))
    inventory.load.cache_clear()
    yield files
    inventory.load.cache_clear()


def test_unchanged_memory_and_disk_cache_succeed(pinned_inventory):
    first = inventory.load()
    assert inventory.load() is first
    inventory.load.cache_clear()
    assert inventory.load().functions == first.functions


@pytest.mark.parametrize('index', range(3))
@pytest.mark.parametrize('state', ['modified', 'missing'])
@pytest.mark.parametrize('cache_mode', ['memory', 'disk', 'none'])
def test_load_rejects_changed_or_missing_input(pinned_inventory, index, state, cache_mode):
    files = pinned_inventory
    if cache_mode != 'none':
        inventory.load()
    if state == 'missing':
        files[index].unlink()
    else:
        data = files[index].read_bytes()
        files[index].write_bytes(bytes([data[0] ^ 1]) + data[1:])
    if cache_mode == 'disk':
        inventory.load.cache_clear()
    with pytest.raises(SystemExit, match='Missing pinned inventory|Pinned inventory hash differs'):
        inventory.load()


def test_changed_pin_tuple_selects_new_data_without_clearing_memory(monkeypatch, pinned_inventory):
    files = pinned_inventory
    assert inventory.load().functions[0x51a150].body_bytes == 1
    files[0].write_text('entry_va\tname\tbody_bytes\tsignature\n0051a150\troot\t2\tvoid\n', encoding='utf-8')
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {p: env.digest(p) for p in files})
    assert inventory.load().functions[0x51a150].body_bytes == 2


def test_real_fresh_process_rejects_populated_disk_cache(pinned_inventory):
    files = pinned_inventory
    inventory.load()
    pins = {str(p): h for p, h in env.INVENTORY_SHA256.items()}
    files[1].write_bytes(b'drift')
    script = '''import json,sys
from pathlib import Path
from cmre import env,inventory
p=json.loads(sys.argv[1]); env.INVENTORY_SHA256={Path(k):v for k,v in p.items()};env.ANALYSIS=Path(sys.argv[2]);env.rel=lambda p:str(p)
inventory.load()
'''
    code = 'import sys;sys.path.insert(0,sys.argv.pop(1));exec(' + repr(script) + ')'
    r = subprocess.run([sys.executable, '-c', code, str(Path(inventory.__file__).resolve().parents[1]), json.dumps(pins), str(env.ANALYSIS)], capture_output=True, text=True)
    assert r.returncode != 0 and 'Pinned inventory hash differs' in r.stderr


def test_full_pin_tuple_not_truncated_for_disk_cache(monkeypatch, pinned_inventory):
    files = pinned_inventory
    original_digest = env.digest
    pin_a = 'a' * 12 + '0' * 52
    pin_b = 'a' * 12 + '1' * 52
    monkeypatch.setattr(env, 'digest', lambda p: (pin_b if b'\t2\t' in p.read_bytes() else pin_a) if p == files[0] else original_digest(p))
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {p: env.digest(p) for p in files})
    assert inventory.load().functions[0x51a150].body_bytes == 1
    files[0].write_text('entry_va\tname\tbody_bytes\tsignature\n0051a150\troot\t2\tvoid\n', encoding='utf-8')
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {p: env.digest(p) for p in files})
    inventory.load.cache_clear()
    assert inventory.load().functions[0x51a150].body_bytes == 2


@pytest.mark.parametrize('cache_mode', ['memory', 'disk'])
def test_equal_size_and_restored_mtime_still_rejects_drift(pinned_inventory, cache_mode):
    import os
    files = pinned_inventory
    inventory.load()
    cache_before = {p.name: p.read_bytes() for p in (env.ANALYSIS / 'cmre').glob('inventory-*.json')}
    stat = files[0].stat()
    data = files[0].read_bytes()
    files[0].write_bytes(bytes([data[0] ^ 1]) + data[1:])
    os.utime(files[0], ns=(stat.st_atime_ns, stat.st_mtime_ns))
    assert files[0].stat().st_size == stat.st_size
    assert files[0].stat().st_mtime_ns == stat.st_mtime_ns
    if cache_mode == 'disk':
        inventory.load.cache_clear()
    with pytest.raises(SystemExit, match='Pinned inventory hash differs'):
        inventory.load()
    assert {p.name: p.read_bytes() for p in (env.ANALYSIS / 'cmre').glob('inventory-*.json')} == cache_before


def test_legacy_short_key_cache_is_not_trusted_or_deleted(pinned_inventory):
    old_key = '-'.join(h[:12] for h in env.INVENTORY_SHA256.values())
    old = env.ANALYSIS / 'cmre' / f'inventory-{old_key}.json'
    old.parent.mkdir(parents=True)
    old.write_text(json.dumps({'functions': {'0x0051a150': ['stale', 999, 'void']}, 'callees': {}, 'modules': {}}), encoding='utf-8')
    before = old.read_bytes()
    assert inventory.load().functions[0x51a150].body_bytes == 1
    assert old.read_bytes() == before
    assert len(list(old.parent.glob('inventory-v2-*.json'))) == 1
