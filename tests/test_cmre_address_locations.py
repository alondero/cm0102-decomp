"""Data addresses compare raw identity before the established relocation relation."""
import pytest

from cmre import difftest, emu, env


ORIGINAL = 0x00450A70
LABEL = env.LINK_BASE + 0x1010


def address_map():
    return emu.AddressMap({'v_00450a70': LABEL, 'v_00450a73': LABEL + 3}, {},
                          env.LINK_BASE + 0x1000, env.LINK_BASE + 0x2000)


def fault(address, access=22, width=4, eip=ORIGINAL):
    return emu.Result('fault', ((access, address, width), eip), (0,) * 9, [], {}, set())


def difference(a, b, profile='exact'):
    mapping = address_map()
    return difftest._diff(difftest._normalize(a, None, profile),
                          difftest._normalize(b, mapping, profile), mapping)


@pytest.mark.parametrize('profile', ['exact', 'abi', 'semantic'])
@pytest.mark.parametrize('original, recovered', [
    (LABEL, LABEL), (LABEL + 1, LABEL + 1),
    (ORIGINAL, LABEL), (ORIGINAL + 1, LABEL + 1),
])
def test_fault_data_address_accepts_raw_identity_or_exact_location(profile, original, recovered):
    assert difference(fault(original), fault(recovered, eip=LABEL), profile) == {}


@pytest.mark.parametrize('profile', ['exact', 'abi', 'semantic'])
@pytest.mark.parametrize('a, b', [
    (fault(LABEL + 1), fault(LABEL, eip=LABEL)),
    (fault(ORIGINAL + 1), fault(LABEL, eip=LABEL)),
    (fault(ORIGINAL), fault(LABEL + 1, eip=LABEL)),
    (fault(LABEL, width=2), fault(LABEL, eip=LABEL)),
    (fault(LABEL, access=20), fault(LABEL, eip=LABEL)),
])
def test_fault_location_near_misses_remain_failures(profile, a, b):
    assert difference(a, b, profile)


@pytest.mark.parametrize('profile', ['exact', 'abi'])
def test_exact_fault_eip_mismatch_remains_failure(profile):
    assert difference(fault(LABEL, eip=ORIGINAL + 1), fault(LABEL, eip=LABEL), profile)


@pytest.mark.parametrize('kind', ['r', 'w'])
@pytest.mark.parametrize('original, recovered', [
    (LABEL, LABEL), (LABEL + 1, LABEL + 1),
    (ORIGINAL, LABEL), (ORIGINAL + 1, LABEL + 1),
])
def test_event_address_accepts_raw_identity_or_exact_location(kind, original, recovered):
    a = emu.Result('return', (), (0,) * 9, [(kind, original, 4, 17)], {}, set())
    b = emu.Result('return', (), (0,) * 9, [(kind, recovered, 4, 17)], {}, set())
    assert difference(a, b) == {}


@pytest.mark.parametrize('event', [
    ('w', LABEL + 1, 4, 17), ('w', LABEL, 2, 17),
    ('r', LABEL, 4, 17), ('w', LABEL, 4, 18),
])
def test_event_address_kind_width_and_value_near_misses_remain_failures(event):
    a = emu.Result('return', (), (0,) * 9, [('w', LABEL, 4, 17)], {}, set())
    b = emu.Result('return', (), (0,) * 9, [event], {}, set())
    assert difference(a, b)

