"""Explain maps a filtered comparison event back into each raw trace separately."""
from types import SimpleNamespace

import pytest

from cmre import difftest, emu, env


@pytest.mark.parametrize('profile, cursors', [('abi', (5, 7)), ('semantic', (5, 7)), ('exact', (0, 0))])
def test_explain_context_tracks_filtered_import_event_indices(monkeypatch, profile, cursors):
    a_events = [('w', emu.ARENA, 4, 1), ('r', emu.ARENA, 4, 1), ('c', 'first', 0, ()),
                ('w', emu.ARENA + 4, 4, 2), ('r', emu.ARENA + 4, 4, 2), ('c', 'left', 0, ())]
    b_events = [('w', emu.STACK_TOP - 4, 4, 0), ('w', emu.ARENA, 4, 1),
                ('r', emu.ARENA, 4, 1), ('c', 'first', 0, ()), ('w', emu.ARENA + 8, 4, 3),
                ('w', emu.ARENA + 4, 4, 2), ('r', emu.ARENA + 4, 4, 2), ('c', 'right', 0, ())]
    a = emu.Result('return', (), (0,) * 9, a_events, {}, set(), read_sites={4: 0x004671B0})
    b = emu.Result('return', (), (0,) * 9, b_events, {}, set(), read_sites={6: 0x004671C0})
    machines = iter([SimpleNamespace(uc=None, run=lambda *args: a),
                     SimpleNamespace(uc=None, run=lambda *args: b)])
    monkeypatch.setattr(emu, 'Machine', lambda *args, **kwargs: next(machines))
    monkeypatch.setattr(difftest, 'make_case', lambda *args: emu.Case({}, [], b''))
    monkeypatch.setattr(difftest, 'arena_templates', lambda: ())
    monkeypatch.setattr(difftest, 'semantic_fp', lambda *args: ())
    built = SimpleNamespace(entry=env.LINK_BASE + 0x1000, symbols={}, image=SimpleNamespace(
        text=SimpleNamespace(va=env.LINK_BASE + 0x1000, end=env.LINK_BASE + 0x2000), x87_bindings=()))
    options = difftest.Options(**{**difftest.DEFAULTS, 'cases': 1, 'discovery': 0}, seed=1, hints={})
    result = difftest.explain(0x004671B0, (0x004671B0, 0x004671C0), set(), built, {}, options,
                              profile, case_index=0, window=1)
    assert result['events_original_raw'] == [str(event) for event in a_events[max(0, cursors[0] - 1):cursors[0] + 1]]
    assert result['events_recovered_raw'] == [str(event) for event in b_events[max(0, cursors[1] - 1):cursors[1] + 1]]
    if profile != 'exact':
        assert result['read_sites_original'] == {'4': '0x004671b0'}
        assert result['read_sites_recovered'] == {'6': '0x004671c0'}

