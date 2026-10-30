"""Original CALL execution declarations are parent-owned, pinned build inputs."""
import json
import random

import pytest

from cmre import build, difftest, env, image


CALL = 0x005230C3


def declaration():
    return {f'0x{CALL:08x}': {'bytes': image.primary().read(CALL, 5).hex(),
                             'evidence': 'Pinned caller-frame return-byte exposure.'}}


@pytest.mark.parametrize('rows', [
    [], True, {'bad': {}}, {12: {}}, {'0x005230c3': {}},
    {'0x005230c3': {'bytes': 'e838020000', 'evidence': ''}},
    {'0x005230c3': {'bytes': 'E838020000', 'evidence': 'pin'}},
    {'0x005230c3': {'bytes': 'e938020000', 'evidence': 'pin'}},
    {'0x005230c3': {'bytes': 'e838020000', 'evidence': 'pin', 'extra': 0}},
    {'0x005230c3': {'bytes': 'e838020000', 'evidence': 'pin'},
     '0x5230c3': {'bytes': 'e838020000', 'evidence': 'pin'}},
])
def test_malformed_or_ambiguous_declarations_are_rejected(rows):
    with pytest.raises(ValueError):
        difftest.validate_hints({'lift_original_calls': rows})


def test_workers_cannot_change_original_call_bindings():
    with pytest.raises(ValueError, match='worker hints cannot change original CALL'):
        difftest.validate_hints({'lift_original_calls': declaration()}, worker=True)


@pytest.mark.parametrize('site, raw', [
    (CALL, 'e800000000'),  # valid E8 form, different original instruction
    (0x005230C0, 'e838020000'),  # original PUSH site
    (0x00570DD3, 'e818000000'),  # another original owner's CALL
])
def test_declaration_must_match_its_own_original_call(tmp_path, site, raw):
    hints = {'lift_original_calls': {f'0x{site:08x}': {'bytes': raw, 'evidence': 'test pin'}}}
    (env.HINTS_DIR / 'fn_005230c0.json').write_text(json.dumps(hints), encoding='utf-8')
    lifted, _text, compiled = build.lift_and_build(0x005230C0, tmp_path)
    assert compiled is None
    assert any('return transport declaration' in problem for problem in lifted.problems)


@pytest.mark.parametrize('inputs', ['v1', 'v2'])
def test_execution_declaration_preserves_case_inputs_and_rng(inputs):
    plain = difftest.Options(**difftest.DEFAULTS, seed=CALL, hints={}, inputs=inputs)
    declared = difftest.Options(**difftest.DEFAULTS, seed=CALL,
                                hints={'lift_original_calls': declaration()}, inputs=inputs)
    rng_plain = random.Random(CALL)
    rng_declared = random.Random(CALL)
    templates = difftest.arena_templates()
    for _ in range(12):
        assert difftest.make_case(rng_plain, plain, [], templates) == difftest.make_case(
            rng_declared, declared, [], templates)
        assert rng_plain.getstate() == rng_declared.getstate()

