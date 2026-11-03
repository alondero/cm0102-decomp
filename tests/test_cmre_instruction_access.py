"""Whole-access classification and bounded query work at the real _run seam."""
import importlib.util
from pathlib import Path
from types import SimpleNamespace

from cmre import composition as production_composition, env, lift

HERE = Path(__file__).resolve().parent
if (HERE / 'composition.py').is_file():
    spec = importlib.util.spec_from_file_location('cmre._composition_candidate', HERE / 'composition.py')
    composition = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(composition)
else:
    composition = production_composition


def classify(monkeypatch, body, events_by_case):
    """Only emulation is scaffolding; _run executes its actual classification."""
    reports = [SimpleNamespace(events=events, covered=set(body.insns), outcome='return')
               for events in events_by_case]
    machine = SimpleNamespace(uc=SimpleNamespace(hook_add=lambda *a, **k: None),
                              _escape=None, run=lambda entry, case, budget: reports[case])
    cases = iter(range(len(reports)))
    monkeypatch.setattr(composition.emu, 'Machine', lambda *a, **k: machine)
    monkeypatch.setattr(composition.difftest, 'arena_templates', lambda: ())
    monkeypatch.setattr(composition.difftest, 'make_case', lambda *a: next(cases))
    monkeypatch.setattr(composition.difftest, '_normalize', lambda *a: {})
    monkeypatch.setattr(composition.difftest, '_digest_view', lambda *a: 'trace')
    monkeypatch.setattr(composition.difftest, '_diff', lambda *a, fault_mapping=None, strict_data_fault=False: None)
    built = SimpleNamespace(symbols={}, image=SimpleNamespace(
        text=SimpleNamespace(va=env.LINK_BASE, end=env.LINK_BASE+4096)))
    opts = SimpleNamespace(seed=1, cases=len(reports), budget=1, discovery=0, min_coverage=1)
    return composition._run(body.va, {body.va: body}, built, {body.va: env.LINK_BASE}, opts)


def test_large_routine_data_access_does_not_scan_every_instruction(monkeypatch):
    body = lift.discover(0x6181d0)
    assert len(body.insns) == 24141
    class CountedAddress(int):
        comparisons = 0

        def __lt__(self, other):
            type(self).comparisons += 1
            return int(self) < int(other)

        def __gt__(self, other):
            type(self).comparisons += 1
            return int(self) > int(other)
    address = CountedAddress(0x03001000)
    report = classify(monkeypatch, body, [[('r', address, 4, 0)]])
    assert report['code_as_data_cases'] == 0
    assert CountedAddress.comparisons < 128, CountedAddress.comparisons


def test_whole_access_matches_strict_interval_rule_for_gaps_nested_overlaps_and_boundaries(monkeypatch):
    # Unsorted, duplicate-start, adjacent and nested ranges. These are classifier
    # inputs only, not a claim that this synthetic layout is recovered game code.
    spans = [(30, 32), (10, 25), (12, 13), (10, 11), (32, 36), (40, 42)]
    body = SimpleNamespace(va=10, region_end=42, insns={index: SimpleNamespace(va=start, size=end-start)
                           for index, (start, end) in enumerate(spans)})
    queries = [(address, size) for address in range(-2, 46) for size in range(-2, 24)]
    for address, size in queries:
        expected = any(address < end and address+size > start for start, end in spans)
        report = classify(monkeypatch, body, [[('r', address, size, 0)]])
        assert report['code_as_data_cases'] == int(expected), (address, size, expected)


def test_calls_are_ignored_and_writes_crossing_into_code_are_detected(monkeypatch):
    body = SimpleNamespace(va=100, region_end=104,
                           insns={100: SimpleNamespace(va=100, size=4)})
    report = classify(monkeypatch, body, [[('c', 'stub', 0, ())],
        [('w', 99, 1, 0)], [('w', 99, 2, 0)], [('r', 104, 1, 0)]])
    assert report['code_as_data_cases'] == 1
