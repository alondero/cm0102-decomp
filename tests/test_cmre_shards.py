"""Shard reconciliation covers the whole pinned inventory, not only the C frontier."""
import json
import random
import zlib
from types import SimpleNamespace

import pytest

from cmre import cli, env, inventory, ledger, queue, shards, work


def _id(va):
    return f"fn_{va:08x}"


def _vas(shard, count, start=0x401000):
    """Deterministic preferred-image VAs owned by one shard."""
    out, va = [], start
    while len(out) < count:
        if shards.shard_of(va) == shard:
            out.append(va)
        va += 0x10
    return out


def _facts(functions, **states):
    facts = {"schema": shards.FACTS_SCHEMA, "input_sha256": "target",
             "inventory": {"sha256": {"functions.tsv": "pinned"},
                           "functions": {_id(va): size for va, size in functions.items()}},
             "source": [], "lifted": [], "lift_blocked": [], "held": {}, "generated": {},
             "accepted_holds": {}, "c_ready": [], "composed": [], "integrated": [], "outside": {}}
    for key, value in states.items():
        facts[key] = ({_id(va): v for va, v in value.items()} if isinstance(value, dict)
                      else [_id(va) for va in value])
    return facts


def _owned(section):
    return [f for ids in section["functions"].values() for f in ids]


def test_ownership_rule_is_the_published_crc_and_matches_the_c_selector():
    for va in (0x401000, 0x5D5A30, 0x95C8D0, 0xFFFFFFF0):
        expected = 1 + zlib.crc32(f"fn_{va:08x}".encode("ascii")) % 8
        assert shards.shard_of(va) == expected
        assert [k for k in range(1, 9) if queue.in_shard(va, (k, 8))] == [expected]
    assert shards.SHARDS == 8


def test_every_inventory_entry_is_owned_exactly_once_with_one_state():
    rng = random.Random(21)
    functions = {0x401000 + 0x10 * i: rng.randrange(1, 500) for i in range(400)}
    vas = sorted(functions)
    facts = _facts(functions, source=vas[:50], lifted=vas[:150], lift_blocked=vas[150:160],
                   held={va: "evidence-needed" for va in vas[100:120]},
                   generated={va: "import-thunk" for va in vas[40:60] + vas[200:230]})
    report = shards.reconcile(facts)
    owned = [f for s in report["shards"] for f in _owned(s)]
    assert sorted(owned) == sorted(_id(va) for va in vas)  # every entry once, none twice
    assert len(report["shards"]) == 8
    for section in report["shards"]:
        assert all(shards.shard_of(int(f[3:], 16)) == section["shard"] for f in _owned(section))
        assert section["owned"]["functions"] == sum(v["functions"] for v in section["states"].values())
        assert section["owned"]["body_bytes"] == sum(v["body_bytes"] for v in section["states"].values())
    assert report["totals"]["owned"] == {"functions": 400, "body_bytes": sum(functions.values())}
    total = report["totals"]["states"]
    assert total["source"]["functions"] == 50          # accepted source wins over generated
    assert total["generated"]["functions"] == 10 + 30  # 40..49 are source
    assert total["held"]["functions"] == 20
    assert total["lift-blocked"]["functions"] == 10
    assert total["c-queue"]["functions"] == 150 - 50 - 10 - 20
    assert total["unlifted"]["functions"] == 400 - 50 - 40 - 20 - 10 - 70


def test_denominator_is_stable_while_states_change():
    functions = {va: 10 + i for i, va in enumerate(_vas(3, 6) + _vas(5, 4))}
    vas = sorted(functions)
    before = shards.reconcile(_facts(functions, lifted=vas[:4]))
    after = shards.reconcile(_facts(functions, source=vas[:5], lifted=vas[:8],
                                    held={vas[6]: "needs-review"}))
    assert ([s["owned"] for s in before["shards"]] == [s["owned"] for s in after["shards"]])
    assert before["inventory"] == after["inventory"]
    assert after["totals"]["states"]["source"]["functions"] == 5


def test_inventory_change_is_reported_per_shard_instead_of_silently_shifting():
    kept, merged = _vas(2, 3), _vas(6, 1)
    split = _vas(4, 2, start=0x500000)
    old_functions = {va: 40 for va in kept + merged}
    new_functions = {va: 40 for va in kept} | {va: 20 for va in split}
    old = shards.reconcile(_facts(old_functions, source=kept[:1]))
    new_facts = _facts(new_functions, source=kept[:1])
    new_facts["inventory"]["sha256"] = {"functions.tsv": "revised"}
    delta = shards.compare(old, shards.reconcile(new_facts))
    assert delta["inventory_changed"] is True
    by_shard = {s["shard"]: s for s in delta["shards"]}
    assert by_shard[6]["removed"] == [_id(merged[0])]
    assert by_shard[6]["owned"] == {"before": {"functions": 1, "body_bytes": 40},
                                    "after": {"functions": 0, "body_bytes": 0}}
    assert by_shard[4]["added"] == [_id(va) for va in split]
    assert by_shard[2]["added"] == by_shard[2]["removed"] == []
    assert by_shard[2]["owned"]["before"] == by_shard[2]["owned"]["after"]


def test_compare_requires_the_same_ownership_rule_and_target():
    report = shards.reconcile(_facts({0x401000: 1}))
    with pytest.raises(ValueError, match="target"):
        shards.compare({**report, "input_sha256": "other"}, report)
    with pytest.raises(ValueError, match="ownership"):
        shards.compare({**report, "shard_count": 4}, report)
    with pytest.raises(ValueError, match="ownership"):
        shards.compare({**report, "rule": "1 + crc32('FN_%08X') % 8"}, report)


def test_accepted_source_with_a_current_hold_keeps_a_shard_open():
    vas = _vas(5, 2)
    facts = _facts({va: 4 for va in vas}, source=vas, accepted_holds={vas[1]: "translation-defect"})
    section = shards.reconcile(facts)["shards"][4]
    assert section["states"]["source"]["functions"] == 2  # still accepted; the partition is unchanged
    assert section["accepted_holds"] == {_id(vas[1]): "translation-defect"}
    assert section["open"] == {"accepted-hold": 1}
    assert section["complete"] is False


def test_outside_inventory_entries_are_exceptions_not_progress():
    inside = _vas(1, 2)
    outside = _vas(1, 1, start=0x990000)[0]
    facts = _facts({va: 30 for va in inside}, source=inside)
    facts["outside"] = {_id(outside): ["accepted-source"]}
    section = shards.reconcile(facts)["shards"][0]
    assert section["owned"] == {"functions": 2, "body_bytes": 60}
    assert section["states"]["source"] == {"functions": 2, "body_bytes": 60}
    assert section["outside_inventory"] == [{"function": _id(outside), "kinds": ["accepted-source"]}]
    assert section["complete"] is False
    assert section["open"] == {"outside-inventory": 1}


def test_held_and_generated_entries_keep_a_shard_open():
    vas = _vas(7, 4)
    facts = _facts({va: 8 for va in vas}, source=vas[:2], lifted=vas[:3],
                   held={vas[2]: "harness-blocker"}, generated={vas[0]: "unwind-funclet",
                                                                vas[3]: "unwind-funclet"})
    section = shards.reconcile(facts)["shards"][6]
    assert section["functions"]["source"] == [_id(vas[0]), _id(vas[1])]
    assert section["functions"]["held"] == [_id(vas[2])]
    assert section["functions"]["generated"] == [_id(vas[3])]
    assert section["held_by_state"] == {"harness-blocker": 1}
    assert section["generated_by_kind"] == {"unwind-funclet": 1}
    assert section["generated_disposition"] == "unverified"
    assert section["complete"] is False
    assert section["open"] == {"generated": 1, "held": 1}


def test_an_empty_c_queue_does_not_close_a_shard():
    vas = _vas(4, 3)
    facts = _facts({va: 5 for va in vas}, source=vas[:1], lift_blocked=vas[1:2])
    section = shards.reconcile(facts)["shards"][3]
    assert section["states"]["c-queue"]["functions"] == 0
    assert section["complete"] is False
    assert section["open"] == {"lift-blocked": 1, "unlifted": 1}
    closed = shards.reconcile(_facts({va: 5 for va in vas}, source=vas))["shards"][3]
    assert closed["complete"] is True and closed["open"] == {}


def test_a_shard_with_no_owned_entries_is_not_complete():
    report = shards.reconcile(_facts({va: 1 for va in _vas(1, 2)}, source=_vas(1, 2)))
    assert report["shards"][1]["owned"]["functions"] == 0
    assert report["shards"][1]["complete"] is False


def test_progress_measures_stay_separate():
    vas = _vas(8, 5)
    facts = _facts({va: 100 for va in vas}, source=vas[:2], lifted=vas[:4],
                   composed=vas[:1], integrated=[])
    section = shards.reconcile(facts)["shards"][7]
    assert section["states"]["source"] == {"functions": 2, "body_bytes": 200}
    assert section["lift_inclusive"] == {"functions": 4, "body_bytes": 400}
    assert section["composed_reported"] == {"functions": 1, "body_bytes": 100}
    assert section["integrated"] == {"functions": 0, "body_bytes": 0}
    assert section["source_percent"] == 40.0


def test_reports_are_reproducible_and_per_shard_reports_match_the_whole():
    functions = {0x401000 + 0x10 * i: i + 1 for i in range(64)}
    vas = sorted(functions)
    facts = _facts(functions, source=vas[::3], lifted=vas[::2], held={vas[1]: "needs-review"})
    shuffled = json.loads(json.dumps(facts))
    for key in ("source", "lifted"):
        random.Random(key).shuffle(shuffled[key])
    shuffled["inventory"]["functions"] = dict(reversed(list(facts["inventory"]["functions"].items())))
    whole = shards.reconcile(facts)
    assert shards.canonical(whole) == shards.canonical(shards.reconcile(shuffled))
    assert shards.digest(whole) == shards.digest(shards.reconcile(shuffled))
    for k in range(1, 9):
        single = shards.reconcile(facts, shard=k)
        assert single["shards"] == [whole["shards"][k - 1]]
        assert single["facts_sha256"] == whole["facts_sha256"]


@pytest.fixture
def workspace(tmp_path, monkeypatch):
    monkeypatch.setenv("CMRE_COORDINATION_PATH", str(tmp_path / "command.lock"))
    for name in ("HINTS_DIR", "C_DIR", "ANALYSIS"):
        directory = tmp_path / name
        directory.mkdir()
        monkeypatch.setattr(env, name, directory)
    vas = _vas(2, 8)
    functions = {va: SimpleNamespace(name="test", body_bytes=10 * (i + 1)) for i, va in enumerate(vas)}
    outside = 0x00990000
    callees = {vas[4]: [vas[0]], vas[5]: [vas[7]]}
    monkeypatch.setattr(inventory, "load",
                        lambda: SimpleNamespace(functions=functions, callees=callees, modules={}))

    def entry(va, record):
        return SimpleNamespace(va=va, size=10, legacy=False, record={"lifted": {"source": "x"}, **record})

    cmre = [entry(vas[1], {"c": {"source": "c", "source_sha256": None}, "status": {"integrated": True}}),
            entry(vas[2], {}), entry(vas[3], {}), entry(vas[4], {}), entry(vas[5], {})]
    legacy = [SimpleNamespace(va=vas[0], size=10, legacy=True, record={}),
              SimpleNamespace(va=outside, size=10, legacy=True, record={})]
    blocked = {vas[6]: {"reason": "lift-blocked: x"}}
    generated = {}
    monkeypatch.setattr(ledger, "cmre_entries", lambda: cmre)
    monkeypatch.setattr(ledger, "legacy_entries", lambda: legacy)
    monkeypatch.setattr(ledger, "blocked", lambda: blocked)
    monkeypatch.setattr(queue, "compiler_generated", generated.get)
    monkeypatch.setattr(env, "INVENTORY_SHA256", {tmp_path / "functions.tsv": "pinned"})
    work.update(vas[2], "needs-review", "pass")
    (env.C_DIR / f"{_id(vas[3])}.c").write_text("/* unaccepted */")
    return SimpleNamespace(vas=vas, outside=outside, cmre=cmre, entry=entry, blocked=blocked,
                           generated=generated)


def _selector_agrees(section, extra=()):
    selector = {_id(i.va) for i in queue.c_frontier(100, shard=(section["shard"], 8))}
    assert selector == set(section["functions"]["c-queue"]) | set(extra)


def test_gather_classifies_live_records_and_agrees_with_the_c_selector(workspace):
    vas = workspace.vas
    facts = shards.gather()
    section = shards.reconcile(facts, shard=2)["shards"][0]
    assert section["functions"] == {
        "source": [_id(vas[0]), _id(vas[1])], "generated": [], "lift-blocked": [_id(vas[6])],
        "held": [_id(vas[2]), _id(vas[3])], "c-queue": [_id(vas[4]), _id(vas[5])],
        "unlifted": [_id(vas[7])]}
    assert section["held_by_state"] == {"needs-review": 1, "unaccepted-source": 1}
    assert section["c_queue_ready"] == {"functions": 1, "body_bytes": 50}
    assert section["integrated"] == {"functions": 1, "body_bytes": 20}
    assert facts["outside"] == {_id(workspace.outside): ["accepted-source"]}
    _selector_agrees(section)


def test_gather_edge_cases_follow_the_c_selector(workspace):
    vas = workspace.vas
    workspace.generated[vas[2]] = "unwind-funclet"          # generated outranks a held record
    workspace.blocked[vas[4]] = {"reason": "manual"}        # a block record does not hold lifted work
    work.update(vas[5], "ready", "retried")                 # a ready record releases an on-disk source
    (env.C_DIR / f"{_id(vas[5])}.c").write_text("/* retry */")
    lifted_outside, source_outside = _vas(2, 2, start=0x00991000)
    workspace.cmre.append(workspace.entry(lifted_outside, {}))
    (env.C_DIR / f"{_id(source_outside)}.c").write_text("/* outside */")
    facts = shards.gather()
    section = shards.reconcile(facts, shard=2)["shards"][0]
    assert section["functions"]["generated"] == [_id(vas[2])]
    assert section["functions"]["c-queue"] == [_id(vas[4]), _id(vas[5])]
    assert section["functions"]["lift-blocked"] == [_id(vas[6])]
    assert facts["outside"] == {_id(workspace.outside): ["accepted-source"],
                                _id(lifted_outside): ["lifted"], _id(source_outside): ["held"]}
    assert {e["function"] for e in section["outside_inventory"]} == {_id(lifted_outside), _id(source_outside)}
    _selector_agrees(section, extra=[_id(lifted_outside)])  # the selector also offers outside lifts


def test_holds_on_accepted_source_count_only_for_the_accepted_bytes(workspace):
    vas = workspace.vas
    source = env.C_DIR / f"{_id(vas[1])}.c"
    source.write_text("/* draft */")
    work.update(vas[1], "evidence-needed", "draft obligation", "establish contract")
    work.update(vas[0], "translation-defect", "legacy regression", "fix")
    source.write_text("/* accepted */")
    workspace.cmre[0].record["c"]["source_sha256"] = env.digest(source)
    section = shards.reconcile(shards.gather(), shard=2)["shards"][0]
    assert section["accepted_holds"] == {_id(vas[0]): "translation-defect"}  # legacy has no source identity
    work.retry(vas[1], "draft obligation resolved by accepted source", changed=True)
    work.update(vas[1], "translation-defect", "accepted source regressed", "fix")
    work.update(vas[0], "needs-review", "superseded by acceptance")
    section = shards.reconcile(shards.gather(), shard=2)["shards"][0]
    assert section["accepted_holds"] == {_id(vas[1]): "translation-defect"}
    assert section["open"]["accepted-hold"] == 1


def test_totals_reconcile_with_status(workspace):
    report = shards.reconcile(shards.gather())
    status, totals = queue.coverage_summary(), report["totals"]
    outside = report["shards"][0]["outside_inventory"] + [e for s in report["shards"][1:]
                                                          for e in s["outside_inventory"]]
    accepted_outside = sum("accepted-source" in e["kinds"] for e in outside)
    assert totals["owned"] == {"functions": status["inventory_functions"],
                               "body_bytes": status["inventory_body_bytes"]}
    assert totals["states"]["source"]["functions"] + accepted_outside == status["source_functions"]
    assert totals["states"]["source"]["body_bytes"] == status["source_body_bytes"]
    assert totals["lift_inclusive"]["functions"] + accepted_outside == status["recovered_functions"]
    assert totals["states"]["lift-blocked"]["functions"] == status["blocked_functions"]
    assert totals["integrated"]["functions"] == status["integrated_functions"]


def test_composed_measure_requires_a_passing_report_with_current_inputs(workspace, monkeypatch):
    from cmre import recover

    vas = workspace.vas
    monkeypatch.setattr(recover, "execution_identity", lambda: {"harness": "h", "toolchain": "t"})
    for va in vas[:2]:
        (env.C_DIR / f"{_id(va)}.c").write_text(f"/* {va} */")
    included = env.C_DIR / "shared.h"
    included.write_text("/* shared */")
    current = {f"{_id(va)}.c": env.digest(env.C_DIR / f"{_id(va)}.c") for va in vas[:2]}
    header = env.digest(env.ROOT / "recovery/include/cmre.h")
    dependencies = {name: {"files": {included.as_posix(): env.digest(included)}} for name in current}
    reports = env.ANALYSIS / "cmre/compositions"
    reports.mkdir(parents=True)

    def write(name, status="pass", observed=True, sources=current, **changes):
        (reports / name).write_text(json.dumps({
            "schema": "cmre-composition-report/1", "input_sha256": env.TARGET_SHA256, "status": status,
            "observed_composed_calls_at_both_optimizations": observed,
            "campaign": {"functions": [_id(va) for va in vas[:2]]}, "source_sha256": sources,
            "header_sha256": header, "source_dependencies": dependencies,
            "execution_identity": {"harness": "h", "toolchain": "t"}, **changes}))

    write("a.json", status="fail")
    write("b.json", observed=False)
    write("c.json", sources={**current, f"{_id(vas[1])}.c": "stale"})
    write("d.json", header_sha256="old header")
    write("e.json", execution_identity={"harness": "old", "toolchain": "t"})
    assert shards.gather()["composed"] == []
    write("f.json")
    assert shards.gather()["composed"] == [_id(vas[0]), _id(vas[1])]
    included.write_text("/* changed include */")
    assert shards.gather()["composed"] == []


def test_cli_writes_a_content_addressed_local_report(workspace, capsys):
    assert cli.main(["shards", "--write"]) == 0
    out = capsys.readouterr().out
    assert "shard 2/8" in out and "INCOMPLETE" in out
    saved = sorted((env.ANALYSIS / "cmre/shards").glob("*.json"))
    assert len(saved) == 1
    assert saved[0].stem == shards.digest(json.loads(saved[0].read_text(encoding="utf-8")))
    assert cli.main(["shards", "--write"]) == 0
    assert sorted((env.ANALYSIS / "cmre/shards").glob("*.json")) == saved
    assert cli.main(["shards", "--shard", "2", "--ids", "--since", str(saved[0])]) == 0
    out = capsys.readouterr().out
    assert f"held {_id(workspace.vas[2])} {_id(workspace.vas[3])}" in out
    assert "inventory unchanged" in out
    assert cli.main(["shards", "--json", "--since", str(saved[0]), "--write"]) == 0
    combined = json.loads(capsys.readouterr().out)  # one JSON document, nothing appended
    assert combined["comparison"]["inventory_changed"] is False
    assert combined["saved"] == env.rel(saved[0])
    assert shards.digest({k: v for k, v in combined.items() if k not in ("comparison", "saved")}) == saved[0].stem
    assert cli.main(["shards", "--shard", "9"]) == 2


def test_live_inventory_partition_is_exact():
    inv = inventory.load()
    counts = [0] * 8
    for va in inv.functions:
        counts[shards.shard_of(va) - 1] += 1
    assert sum(counts) == len(inv.functions) and all(counts)
