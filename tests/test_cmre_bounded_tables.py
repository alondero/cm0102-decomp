"""Pinned nonzero/negative jump-table domains must relocate real slots only."""
import json
from copy import deepcopy
from dataclasses import replace

import pytest

from cmre import build, difftest, env, image, lift

VA = 0x0094F0F0
# Preferred-image VAs in env.TARGET_SHA256; independently reviewed original CFG.
DOMAINS = {
    0x94F13D: (0x94F150, 1, 3), 0x94F144: (0x94F248, -4, -1),
    0x94F14C: (0x94F1CC, 0, 7), 0x94F125: (0x94F238, 0, 3),
    0x94F2CD: (0x94F2D8, 1, 3), 0x94F2B2: (0x94F380, -7, 0),
    0x94F2A7: (0x94F3D0, 0, 3),
}


@pytest.fixture
def table_hints(tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    env.HINTS_DIR.mkdir()
    img = image.primary()
    rows = {}
    # Every dispatch at the same base shares its byte-pinned contiguous domain.
    dossier = lift.discover(VA)
    for rec in dossier.insns.values():
        ins = next(lift._cs().disasm(rec.raw, rec.va, count=1))
        if ins.mnemonic != 'jmp' or not ins.operands or ins.operands[0].type != lift.X.X86_OP_MEM:
            continue
        base = ins.operands[0].mem.disp
        domain = next((value for value in DOMAINS.values() if value[0] == base), None)
        if domain is None:
            continue
        _, lo, hi = domain
        rows[f'{rec.va:#010x}'] = dict(bytes=rec.raw.hex(), index_range=[lo, hi],
            table_sha256=env.digest_bytes(img.read(base+4*lo,4*(hi-lo+1))),
            evidence='Reviewed original branch guards, masked counts and negative index construction.')
    # Missing blocks include further dispatches; their exact original sites are pinned here.
    for site, base, lo, hi in (
        (0x94F182,0x94F238,0,3),(0x94F1A8,0x94F238,0,3),(0x94F1C2,0x94F238,0,3),
        (0x94F22F,0x94F238,0,3),(0x94F2D4,0x94F3D0,0,3),
        (0x94F2FE,0x94F3D0,0,3),(0x94F328,0x94F3D0,0,3),
        (0x94F35A,0x94F3D0,0,3),(0x94F3C7,0x94F3D0,0,3),
    ):
        ins = next(lift._cs().disasm(img.read_upto(site,16),site,count=1))
        assert ins.mnemonic == 'jmp' and ins.operands[0].mem.disp == base
        rows[f'{site:#010x}'] = dict(bytes=bytes(ins.bytes).hex(), index_range=[lo,hi],
            table_sha256=env.digest_bytes(img.read(base+4*lo,4*(hi-lo+1))),
            evidence='Same original masked/count-bounded table dispatch domain.')
    hints = {'lift_jump_tables': rows}
    path = env.HINTS_DIR / f'fn_{VA:08x}.json'
    path.write_text(json.dumps(hints),encoding='utf-8')
    return path,hints


def test_copy_tables_decode_complete_body_without_fake_zero_slots(table_hints, tmp_path):
    work = tmp_path/'build'
    work.mkdir()
    lifted, text, built = build.lift_and_build(VA,work)
    assert not lifted.problems
    assert lifted.instruction_bytes == 664 and len(lifted.insns) == 231
    assert built is not None
    assert build.round_trip(lifted,built) == []
    assert sum(4*len(t.targets) for t in lifted.tables.values()) == 120
    assert 0x94F150 not in lifted.tables and 0x94F2D8 not in lifted.tables
    # Logical base F248 is code; its negative indexes must use the relocated F238 table.
    instruction = next(lift._cs().disasm(built.image.read_upto(built.symbols['v_0094f144'],16),
        built.symbols['v_0094f144'],count=1))
    assert instruction.operands[0].mem.disp == built.symbols['v_0094f238'] + 16
    # A one-slot relocation error must still fail the round-trip checker.
    at = built.symbols['v_0094f13d']
    section = built.image.section_at(at)
    offset = at-section.va+3
    raw = section.raw
    wrong = (int.from_bytes(raw[offset:offset+4],'little')+4).to_bytes(4,'little')
    patched = replace(section,raw=raw[:offset]+wrong+raw[offset+4:])
    built.image.sections = [patched if s is section else s for s in built.image.sections]
    assert any(line.startswith('0x0094f13d:') for line in build.round_trip(lifted,built))
    # An unrelocated original address must not pass by textual identity.
    original_base = (0x94F150).to_bytes(4,'little')
    patched = replace(section,raw=raw[:offset]+original_base+raw[offset+4:])
    built.image.sections = [patched if s.va == section.va else s for s in built.image.sections]
    assert any(line.startswith('0x0094f13d:') for line in build.round_trip(lifted,built))
    # Synthetic alternative label layout: an ordinary code-label alias cannot
    # authorize a different displacement from this dispatch's physical anchor.
    dispatch = built.symbols['v_0094f144']
    wrong_address = built.symbols['v_0094f238'] + 12
    built.symbols['v_0094f248'] = wrong_address
    other_offset = dispatch-section.va+3
    patched = replace(section,raw=raw[:other_offset]+wrong_address.to_bytes(4,'little')+raw[other_offset+4:])
    built.image.sections = [patched if s.va == section.va else s for s in built.image.sections]
    assert any(line.startswith('0x0094f144:') for line in build.round_trip(lifted,built))


@pytest.mark.parametrize('change', ['bytes','table_sha256','unused','zero_index'])
def test_stale_or_invalid_table_declarations_block_build(table_hints,tmp_path,change):
    path,hints = table_hints
    hints = deepcopy(hints)
    row = hints['lift_jump_tables']['0x0094f13d']
    if change == 'unused':
        hints['lift_jump_tables']['0x0094f0f0'] = hints['lift_jump_tables'].pop('0x0094f13d')
    elif change == 'zero_index':
        row['index_range'][0] = 0
    else:
        row[change] = '00' * (7 if change == 'bytes' else 32)
    path.write_text(json.dumps(hints),encoding='utf-8')
    lifted,_,built = build.lift_and_build(VA,tmp_path/'rejected')
    assert built is None and any('declared jump table' in problem for problem in lifted.problems)


def test_workers_cannot_change_lift_bindings(table_hints):
    _,hints = table_hints
    difftest.validate_hints(hints)
    with pytest.raises(ValueError,match='worker.*lift'):
        difftest.validate_hints(hints,worker=True)


@pytest.mark.parametrize('bounds', [[True,3],[1,0],[-0x80000001,0],[0,0x80000000],[0]])
def test_malformed_table_domains_cannot_be_used(table_hints,bounds):
    _, hints = table_hints
    hints['lift_jump_tables']['0x0094f13d']['index_range'] = bounds
    with pytest.raises(ValueError,match='index_range'):
        difftest.validate_hints(hints)
