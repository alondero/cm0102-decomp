"""Recovered test-image mappings must not change absolute input-pointer faults."""
import json
from pathlib import Path

import pytest

from cmre import build, difftest, emu, env, image, recover


@pytest.mark.parametrize('va, case', [(0x00578780, 586), (0x0065C8E0, 581)])
def test_absolute_pointer_collision_retains_original_fault(va, case):
    path = Path(__file__).parent / 'fixtures/recovered-mapping' / f'fn_{va:08x}.json'
    (env.HINTS_DIR / path.name).write_bytes(path.read_bytes())
    result = recover.explain(va, case)
    assert result['difference'] == {}, result['difference']


def test_original_image_permission_fault_is_preserved(tmp_path):
    va = 0x00768290  # mov eax,ecx; xor ecx,ecx; mov [eax],0x96da10
    lifted, _text, compiled = build.lift_and_build(va, tmp_path)
    assert compiled is not None, lifted.problems
    case = emu.Case({'ecx': 0x00401000, 'eflags': 0x202}, [0] * 10,
                    bytes(emu.ARENA_SIZE))
    a = emu.Machine(image.primary()).run(va, case, 20)
    b = emu.Machine(image.primary(), compiled.image).run(compiled.entry, case, 20)
    assert a.outcome == b.outcome == 'fault'
    assert a.detail[0] == b.detail[0] == (22, 0x00401000, 4)
    amap = emu.AddressMap(compiled.symbols, {}, compiled.image.text.va, compiled.image.text.end)
    assert difftest._diff(difftest._normalize(a, None, 'exact'),
                          difftest._normalize(b, amap, 'exact'), amap) == {}


@pytest.mark.parametrize('original, recovered', [
    ((20, 0x6E5D2C24, 4), (20, 0x6E5D2C23, 4)),
    ((20, 0x6E5D2C23, 2), (20, 0x6E5D2C23, 4)),
    ((22, 0x00401000, 4), (20, 0x00401000, 4)),
])
def test_unrelated_address_width_and_original_permission_mismatches_remain_failures(original, recovered):
    a = emu.Result('fault', (original, 0x0065C94A), (0,) * 9, [], {}, set())
    b = emu.Result('fault', (recovered, 0x0065C94A), (0,) * 9, [], {}, set())
    assert difftest._diff(difftest._normalize(a, None, 'exact'),
                          difftest._normalize(b, None, 'exact'))

