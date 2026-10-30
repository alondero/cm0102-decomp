"""Real exact/v1 cases that expose bytes of a relocated CALL return word."""
import json
from pathlib import Path

import pytest

from cmre import env, image, recover


@pytest.mark.parametrize('va, case, call', [
    (0x005230C0, 556, 0x005230C3),
    (0x00570DD0, 365, 0x00570DD3),
    (0x00632250, 306, 0x00632253),
])
def test_original_call_return_bytes_survive_array_aliasing(va, case, call):
    # Preserve the complete existing case domain, fixtures, seed and budget.
    path = Path(__file__).parent / 'fixtures/original-call' / f'fn_{va:08x}.json'
    hints = json.loads(path.read_text(encoding='utf-8'))
    hints['lift_original_calls'] = {
        f'0x{call:08x}': {
            'bytes': image.primary().read(call, 5).hex(),
            'evidence': ('The original destructor list can alias the saved caller frame; '
                         'the original CALL return bytes are observable data.'),
        },
    }
    (env.HINTS_DIR / path.name).write_text(json.dumps(hints), encoding='utf-8')
    result = recover.explain(va, case)
    assert result['difference'] == {}, result['difference']

