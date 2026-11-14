"""Stage accounting and planning arithmetic; no game content or provider calls."""
import pytest

from cmre import pipeline


def entries(*rows):
    return [tuple(row) for row in rows]


def test_family_of_uses_the_first_token():
    assert pipeline.family_of('missing-real-virtual-entry note') == 'missing-real-virtual-entry'
    assert pipeline.family_of('') == '(none)'
    assert pipeline.family_of('   ') == '(none)'


def test_build_classifies_the_three_bands():
    state = pipeline.build(
        entries(
            (0x1000, 400, False, True),   # lifted, no source
            (0x2000, 200, False, True),   # authored
            (0x3000, 900, True, True),    # accepted C: outside every band
            (0x4000, 100, False, False),  # unlifted ledger entry: ignored
        ),
        {'fn_00002000': {'state': 'evidence-needed', 'blocker': 'x-thing here'},
         'fn_00001000': {'state': 'harness-blocker', 'blocker': 'y-thing'}},
        {0x2000},
        {0x5000: 1000, 0x6000: 300},
    )
    assert state.bands['authored'].functions == 1
    assert state.bands['authored'].body_bytes == 200
    assert state.bands['authored'].blocker_families == {'x-thing': 1}
    assert state.bands['lifted'].functions == 1
    assert state.bands['lifted'].blocker_families == {'y-thing': 1}
    assert state.bands['unlifted'].functions == 2
    assert state.bands['unlifted'].body_bytes == 1300
    assert state.total('authored', 'lifted', 'unlifted') == 1900


def test_gate_seconds_is_monotonic_in_size():
    costs = [pipeline.gate_seconds(size) for size in (0, 100, 600, 1500, 4096)]
    assert costs == sorted(costs)
    assert costs[0] == pipeline.DEFAULT_GATE_SECONDS[0]


def test_select_is_largest_first_and_stops_at_the_target():
    state = pipeline.Survey(
        bands={name: pipeline.Band(name, 0, 0, 0) for name in
               ('authored', 'lifted', 'unlifted')},
        sizes={'authored': [100, 400, 200], 'lifted': [900], 'unlifted': []},
    )
    chosen = pipeline.select(state, 700)
    assert [(item['band'], item['body_bytes']) for item in chosen] == [
        ('authored', 400), ('authored', 200), ('authored', 100)]
    assert sum(item['body_bytes'] for item in chosen) == 700


def test_select_continues_into_the_next_band():
    state = pipeline.Survey(
        bands={name: pipeline.Band(name, 0, 0, 0) for name in
               ('authored', 'lifted', 'unlifted')},
        sizes={'authored': [100], 'lifted': [500], 'unlifted': [900]},
    )
    chosen = pipeline.select(state, 1100)
    assert [item['band'] for item in chosen] == ['authored', 'lifted', 'unlifted']


def test_plan_reports_a_shortfall_when_supply_is_insufficient():
    state = pipeline.Survey(
        bands={name: pipeline.Band(name, 0, 0, 0) for name in
               ('authored', 'lifted', 'unlifted')},
        sizes={'authored': [100], 'lifted': [], 'unlifted': []},
    )
    result = pipeline.plan(state, 1000, 60)
    assert result['selected_bytes'] == 100
    assert result['shortfall_bytes'] == 900
    assert result['selected_functions'] == 1


def test_plan_uses_the_measured_pass_rate():
    state = pipeline.Survey(
        bands={name: pipeline.Band(name, 0, 0, 0) for name in
               ('authored', 'lifted', 'unlifted')},
        sizes={'authored': [1000, 1000, 1000, 1000], 'lifted': [], 'unlifted': []},
    )
    optimistic = pipeline.plan(state, 2000, 60)
    measured = pipeline.plan(state, 2000, 60, pass_rate={'authored': 0.25})
    assert optimistic['selected_functions'] == 2
    # A 25% yield needs four attempts of 1000 bytes to expect 1000 bytes; the
    # plan selects until the expected yield reaches the target.
    assert measured['selected_functions'] == 4
    assert measured['assumption'].startswith('scaled')
    assert measured['expected_accepted_bytes'] == 1000
    assert measured['shortfall_bytes'] == 1000


def test_plan_sizes_author_slots_from_the_window():
    state = pipeline.Survey(
        bands={name: pipeline.Band(name, 0, 0, 0) for name in
               ('authored', 'lifted', 'unlifted')},
        sizes={'authored': [], 'lifted': [1000] * 10, 'unlifted': []},
    )
    result = pipeline.plan(state, 5000, 60, action_minutes=4.5)
    assert result['selected_functions'] == 5
    assert result['author_slots'] == 1
    tight = pipeline.plan(state, 10000, 30, action_minutes=4.5)
    assert tight['selected_functions'] == 10
    assert tight['author_slots'] == 2


def test_plan_rejects_nonpositive_inputs():
    state = pipeline.Survey(
        bands={name: pipeline.Band(name, 0, 0, 0) for name in
               ('authored', 'lifted', 'unlifted')},
        sizes={'authored': [], 'lifted': [], 'unlifted': []},
    )
    with pytest.raises(ValueError):
        pipeline.plan(state, 100, 0)
    with pytest.raises(ValueError):
        pipeline.select(state, 0)
