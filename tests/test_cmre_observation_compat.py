"""Narrow source proof, including mutations that sampled runs could miss."""
import pytest
from cmre import observation_compat as proof


def sources():
    before = 'import bisect\nclass Machine:\n def __init__(self):\n  self.read_sites = {}\n def _access(self, uc, address, size):\n  observed = int.from_bytes(uc.mem_read(address, size), "little")\n'
    after = ('import ctypes\nfrom unicorn.unicorn_py3.unicorn import uclib\nimport bisect\nclass Machine:\n'
             ' def __init__(self):\n  self.read_sites = {}\n' +
             '\n'.join('  ' + line for line in proof.POOL.splitlines()) + '\n' +
             '\n'.join(' ' + line for line in proof.READER.splitlines()) + '\n' +
             ' def _access(self, uc, address, size):\n  observed = self._read_value(address, size)\n')
    return before, after


def test_exact_storage_refactor_is_recognized():
    proof.check_sources(*sources())


def test_live_constructor_local_cannot_be_overwritten():
    before, after = sources()
    before = before.replace('self.read_sites = {}', 'size = 123\n  self.read_sites = {}\n  self.saved = size')
    after = after.replace('self.read_sites = {}', 'size = 123\n  self.read_sites = {}')
    after = after.replace(' def _read_value', '  self.saved = size\n def _read_value')
    with pytest.raises(ValueError, match='live constructor'):
        proof.check_sources(before, after)


def test_storage_imports_cannot_change_prior_global_resolution():
    before, after = sources()
    extra = '\ndef old_function():\n return ctypes\n'
    with pytest.raises(ValueError, match='name resolution'):
        proof.check_sources(before + extra, after + extra)


def test_public_native_read_contract_is_recognized():
    assert 'uc_mem_read' in proof.check_public_binding()


def test_unknown_public_binding_fails_closed(monkeypatch):
    monkeypatch.setattr(proof.inspect, 'getsource', lambda _: 'def mem_read(self, address, size):\n return bytes(size)\n')
    with pytest.raises(ValueError, match='Unknown Unicorn'):
        proof.check_public_binding()


def test_proof_environment_ignores_unrelated_packages_but_rejects_engine_or_test_changes():
    import copy
    before = {'python': 'same', 'packages': [['unicorn', '2.1.4'], ['pytest', '8.4'], ['other', '1']]}
    after = copy.deepcopy(before)
    after['packages'].append(['ziglang', '0.16'])
    assert proof._regression_environment(before) == proof._regression_environment(after)
    for index in (0, 1):
        changed = copy.deepcopy(after)
        changed['packages'][index][1] = 'changed'
        assert proof._regression_environment(before) != proof._regression_environment(changed)


@pytest.mark.parametrize('old,new', [
    ('return word.value', 'return word.value + 1'),
    ('address, buffer, size', 'address + 1, buffer, size'),
    ('__ctype_le__', '__ctype_be__'),
    ('raise unicorn.UcError(status, address, size)', 'return 0'),
    ('self._read_value(address, size)', 'self._read_value(address, size - 1)'),
    ('self.read_sites = {}', 'self.read_sites = {}; self.uc.reg_write(1, 0)'),
    ('import bisect', 'import bisect\nBUDGET = 1'),
])
def test_near_misses_fail_closed(old, new):
    before, after = sources()
    assert old in after
    with pytest.raises(ValueError):
        proof.check_sources(before, after.replace(old, new))
