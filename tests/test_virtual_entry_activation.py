"""Activation path/pin contract; real export checks require local evidence."""
import os
from pathlib import Path
import pytest
from cmre import env, inventory

PREFIX = Path('cmre/supply-sol-commandcode-20261115/parallel-virtual-entry')
EXPECTED = {
    PREFIX / 'current-identity-fresh-f/exports/functions.tsv': '6727ff30e8b301650223cce367dc477e4529a7da29e6991ce7f8da03b8b1d6a1',
    PREFIX / 'current-identity-map-f/calls.tsv': 'b4d4c0fa39a8612ebed37c5aa20be8168cb4c751356b2f8b3c964e10f42d873e',
    PREFIX / 'current-identity-map-f/module-candidates.json': '3bc01bd5092f0106a9c5416f9cd083697e884c7a44f0e3d1416ee59e56bd5704',
}

def test_selected_paths_and_pins_are_one_coherent_pair():
    assert {p.relative_to(env.ANALYSIS): h for p, h in env.INVENTORY_SHA256.items()} == EXPECTED
    assert env.FUNCTIONS_TSV == env.ANALYSIS / next(iter(EXPECTED))
    assert env.CALLS_TSV == env.ANALYSIS / list(EXPECTED)[1]
    assert env.MODULES_JSON == env.ANALYSIS / list(EXPECTED)[2]


def test_each_pin_drift_fails_closed(monkeypatch, tmp_path):
    for index in range(3):
        files = [tmp_path / str(n) for n in range(3)]
        for p in files: p.write_bytes(b'good')
        pins = {p: env.digest(p) for p in files}
        files[index].write_bytes(b'drift')
        monkeypatch.setattr(env, 'INVENTORY_SHA256', pins)
        monkeypatch.setattr(env, 'rel', lambda p: str(p))
        with pytest.raises(SystemExit, match='Pinned inventory hash differs'):
            inventory._check_pins()


def test_real_exports_entry_extent_and_body(monkeypatch, tmp_path):
    source = os.environ.get('CMRE_ACTIVATION_ANALYSIS')
    if not source:
        pytest.skip('Set CMRE_ACTIVATION_ANALYSIS to retained local analysis evidence')
    analysis = Path(source)
    paths = [analysis / x for x in EXPECTED]
    monkeypatch.setattr(env, 'INVENTORY_SHA256', dict(zip(paths, EXPECTED.values())))
    monkeypatch.setattr(env, 'FUNCTIONS_TSV', paths[0])
    monkeypatch.setattr(env, 'CALLS_TSV', paths[1])
    monkeypatch.setattr(env, 'MODULES_JSON', paths[2])
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    inventory.load.cache_clear()
    try:
        inventory._check_pins()
        inv = inventory.load()
        assert len(inv.functions) == 9242
        assert sum(f.body_bytes for f in inv.functions.values()) == 4310884
        assert inv.functions[0x51a150].body_bytes == 7873
        assert inv.functions[0x51c020].body_bytes == 1000
        assert inv.next_entry_after(0x51a150) == 0x51c020
        assert inv.next_entry_after(0x51c020) == 0x51c410
        seed = (analysis / PREFIX / 'current-identity-fresh-f/exports/seed-result.txt').read_text()
        assert 'target_owner_before=none' in seed
        assert 'target_body_ranges=[[0051c020, 0051c407]]' in seed
        assert 'root_before_ranges=[[0051a150, 0051c010]]' in seed
        assert 'root_after_ranges=[[0051a150, 0051c010]]' in seed
        assert 'status=created' in seed
    finally:
        inventory.load.cache_clear()

def test_cached_inventory_still_rejects_input_drift(monkeypatch, tmp_path):
    """Cached inventory must not bypass input hash validation after drift."""
    functions = tmp_path / 'functions.tsv'
    calls = tmp_path / 'calls.tsv'
    modules = tmp_path / 'modules.json'
    functions.write_text('entry_va\tname\tbody_bytes\tsignature\n0051a150\troot\t1\tvoid\n', encoding='utf-8')
    calls.write_text('function_va\ttarget_address\n', encoding='utf-8')
    modules.write_text('[]', encoding='utf-8')
    monkeypatch.setattr(env, 'FUNCTIONS_TSV', functions)
    monkeypatch.setattr(env, 'CALLS_TSV', calls)
    monkeypatch.setattr(env, 'MODULES_JSON', modules)
    monkeypatch.setattr(env, 'INVENTORY_SHA256', {p: env.digest(p) for p in (functions, calls, modules)})
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'rel', lambda p: str(p))
    inventory.load.cache_clear()
    try:
        inventory.load()
        functions.write_text('drift', encoding='utf-8')
        inventory.load.cache_clear()  # fresh process, existing on-disk cache
        with pytest.raises(SystemExit, match='Pinned inventory hash differs'):
            inventory.load()
    finally:
        inventory.load.cache_clear()
