"""Asset-free application tests through the real CLI call seam."""
import importlib.util
import json
import os
from pathlib import Path
import subprocess

import pytest

MODULE = Path(__file__).resolve().parents[1] / "cmre_hint_apply.py"
spec = importlib.util.spec_from_file_location("cmre_hint_apply", MODULE)
apply = importlib.util.module_from_spec(spec)
spec.loader.exec_module(apply)
FN = "fn_00500000"
VA = 0x00500000


def fixture(seed=0):
    return {"regs": {n: 0 for n in apply.difftest.emu.REG_NAMES},
            "stack": [0] * 10, "arena_template": 0, "arena_writes": {},
            "globals": {}, "import_seed": seed}


@pytest.fixture
def store(tmp_path, monkeypatch):
    from cmre import env
    for key, suffix in {
        "ROOT": "", "ANALYSIS": "CM0102/Analysis", "HINTS_DIR": "recovery/hints",
        "LEDGER_DIR": "recovery/ledger", "REGISTRY": "recovery/registry.json",
        "LOCAL_CAPTURES": "local/captures",
    }.items():
        monkeypatch.setattr(env, key, tmp_path / suffix)
    monkeypatch.delenv("CMRE_WRITER_TOKEN", raising=False)
    monkeypatch.delenv("CMRE_WRITER_PATH", raising=False)
    monkeypatch.delenv("CMRE_COORDINATION_PATH", raising=False)
    subprocess.run(["git", "init", "-q", str(tmp_path)], check=True)
    (tmp_path / ".gitignore").write_text("CM0102/Analysis/\n")
    env.HINTS_DIR.mkdir(parents=True)
    env.REGISTRY.write_text('{"current": []}')
    hint = env.HINTS_DIR / f"{FN}.json"
    hint.write_bytes((json.dumps({"auto": True, "cases": 100,
                                 "fixtures": [fixture()]}) + "\r\n").encode())
    proposal = tmp_path / "proposal.json"
    evidence = env.ANALYSIS / "new-evidence"
    return hint, proposal, evidence


def invoke(store, capsys, proposal=None, expected=None, fn=FN):
    hint, path, evidence = store
    if proposal is not None:
        path.write_bytes(json.dumps(proposal).encode() + b"\n")
    code = apply.main([fn, "--proposal", str(path), "--expected-sha256",
                       expected or apply.env.digest(hint), "--evidence-dir", str(evidence)])
    return code, json.loads(capsys.readouterr().out)


def extended(store):
    old = json.loads(store[0].read_bytes())
    old.pop("auto", None)
    return {**old, "fixtures": old["fixtures"] + [fixture(1)]}


def test_exact_proposal_and_backup_before_mutation(store, capsys, monkeypatch):
    hint, proposal, evidence = store
    before = hint.read_bytes()
    real = apply.coordination.atomic_bytes

    def observe(path, data):
        assert hint.read_bytes() == before
        assert (evidence / "prior.json").read_bytes() == before
        assert (evidence / "proposal.json").read_bytes() == data == proposal.read_bytes()
        manifest = json.loads((evidence / "manifest.json").read_bytes())
        assert manifest["prior_sha256"] == apply.env.digest_bytes(before)
        assert manifest["proposal_sha256"] == apply.env.digest_bytes(data)
        return real(path, data)

    monkeypatch.setattr(apply.coordination, "atomic_bytes", observe)
    code, result = invoke(store, capsys, extended(store))
    assert code == 0 and result["status"] == "applied"
    assert result["fixtures_added"] == 1
    assert hint.read_bytes() == proposal.read_bytes()
    assert result["sha256"] == apply.env.digest(hint)
    assert result["manifest_sha256"] == apply.env.digest(evidence / "manifest.json")


@pytest.mark.parametrize("change", ["hash", "prefix", "capture", "policy", "remove",
                                  "malformed", "schema", "accepted", "legacy", "c_hints"])
def test_refusal_leaves_canonical_untouched(store, capsys, change):
    hint, proposal, evidence = store
    before = hint.read_bytes()
    new = extended(store)
    expected = None
    if change == "hash":
        expected = "0" * 64
    elif change == "prefix":
        new["fixtures"][0]["import_seed"] = 7
    elif change == "capture":
        new["captures"] = ["a" * 64]
    elif change == "policy":
        new["cases"] = 1
    elif change == "remove":
        del new["cases"]
    elif change == "c_hints":
        new["c_hints"] = {"fixtures": [fixture()]}
    elif change == "schema":
        new["fixtures"].append({})
    elif change == "accepted":
        apply.env.LEDGER_DIR.mkdir()
        (apply.env.LEDGER_DIR / f"{FN}.json").write_text("{}")
    elif change == "legacy":
        legacy = apply.env.ROOT / "legacy.json"
        legacy.write_text(json.dumps({"functions": [{"va": hex(VA), "size": 1,
                                                    "sha256": "a" * 64}]}))
        apply.env.REGISTRY.write_text('{"current": ["legacy.json"]}')
    if change == "malformed":
        proposal.write_bytes(b'{"fixtures": SECRET_PAYLOAD')
        new = None
    code, result = invoke(store, capsys, new, expected)
    assert code == 2 and result["status"] == "refused"
    assert "SECRET_PAYLOAD" not in json.dumps(result)
    assert hint.read_bytes() == before
    assert not evidence.exists()


def test_same_raw_bytes_and_auto_claim(store, capsys):
    hint, proposal, evidence = store
    claimed = json.loads(hint.read_bytes())
    del claimed["auto"]
    hint.write_bytes(json.dumps(claimed).encode() + b"\r\n")
    proposal.write_bytes(hint.read_bytes())
    code, result = invoke(store, capsys)
    assert code == 0 and result["sha256"] == apply.env.digest(proposal)
    assert result["fixtures_added"] == 0


def test_auto_true_may_be_removed(store, capsys):
    new = json.loads(store[0].read_bytes())
    del new["auto"]
    assert invoke(store, capsys, new)[0] == 0


def test_capture_append_and_prefix(store, capsys):
    regs = {n: 0 for n in apply.difftest.CAPTURE_REG_NAMES}
    first = apply.difftest.save_capture(VA, regs, 0x37F, [0] * 64, {}, 0, {})
    second = apply.difftest.save_capture(VA, regs, 0x37F, [1] * 64, {}, 0, {})
    old = json.loads(store[0].read_bytes())
    old["captures"] = [first]
    store[0].write_text(json.dumps(old))
    new = {**old, "captures": [second, first]}
    del new["auto"]
    assert invoke(store, capsys, new)[0] == 2
    new["captures"] = [first, second]
    assert invoke(store, capsys, new)[0] == 0


def test_final_identity_check_does_not_clobber(store, capsys, monkeypatch):
    original = apply.retain

    def drift(*args, **kwargs):
        original(*args, **kwargs)
        store[0].write_bytes(b'{"notes": "newer work"}\n')

    monkeypatch.setattr(apply, "retain", drift)
    assert invoke(store, capsys, extended(store))[0] == 2
    assert store[0].read_bytes() == b'{"notes": "newer work"}\n'
    assert (store[2] / "manifest.json").exists()


@pytest.mark.parametrize("fn", ["../fn_00500000", "fn_0050000A", "00500000"])
def test_invalid_function(store, capsys, fn):
    before = store[0].read_bytes()
    assert invoke(store, capsys, extended(store), fn=fn)[0] == 2
    assert store[0].read_bytes() == before


def test_busy_lease_refuses(store, capsys, monkeypatch):
    from contextlib import contextmanager

    @contextmanager
    def busy():
        raise apply.coordination.BusyError("busy")
        yield

    monkeypatch.setattr(apply.coordination, "writer", busy)
    before = store[0].read_bytes()
    assert invoke(store, capsys, extended(store))[0] == 2
    assert store[0].read_bytes() == before


@pytest.mark.parametrize("raw", [b'{"cases":100,"cases":1}', b'{"seed":NaN}'])
def test_ambiguous_json_refused(store, capsys, raw):
    before = store[0].read_bytes()
    store[1].write_bytes(raw)
    assert invoke(store, capsys)[0] == 2
    assert store[0].read_bytes() == before


def test_expected_hash_is_not_newline_normalized(store, capsys):
    before = store[0].read_bytes()
    normalized = apply.env.digest_bytes(before.replace(b"\r\n", b"\n"))
    assert invoke(store, capsys, extended(store), normalized)[0] == 2
    assert store[0].read_bytes() == before


def test_backup_failure_prevents_mutation(store, capsys, monkeypatch):
    def fail(*args):
        raise OSError("backup unavailable")

    monkeypatch.setattr(apply, "retain", fail)
    before = store[0].read_bytes()
    assert invoke(store, capsys, extended(store))[0] == 2
    assert store[0].read_bytes() == before


def test_existing_evidence_is_never_reused(store, capsys):
    store[2].mkdir(parents=True)
    sentinel = store[2] / "prior.json"
    sentinel.write_bytes(b"retained")
    before = store[0].read_bytes()
    assert invoke(store, capsys, extended(store))[0] == 2
    assert store[0].read_bytes() == before
    assert sentinel.read_bytes() == b"retained"


def test_accepted_during_backup_refused(store, capsys, monkeypatch):
    original = apply.retain

    def accepted(*args):
        original(*args)
        apply.env.LEDGER_DIR.mkdir()
        (apply.env.LEDGER_DIR / f"{FN}.json").write_bytes(b"{}")

    monkeypatch.setattr(apply, "retain", accepted)
    before = store[0].read_bytes()
    assert invoke(store, capsys, extended(store))[0] == 2
    assert store[0].read_bytes() == before


def test_corrupt_capture_refused_without_payload_output(store, capsys):
    name = "a" * 64
    path = apply.difftest.capture_path(VA, name)
    path.parent.mkdir(parents=True)
    path.write_bytes(b"SECRET_CAPTURE")
    new = extended(store)
    new["captures"] = [name]
    before = store[0].read_bytes()
    code, out = invoke(store, capsys, new)
    assert code == 2 and "SECRET_CAPTURE" not in json.dumps(out)
    assert store[0].read_bytes() == before


def test_evidence_must_be_ignored_analysis(store, capsys):
    hint, proposal, _ = store
    before = hint.read_bytes()
    outside = (hint, proposal, apply.env.ROOT / "outside")
    assert invoke(outside, capsys, extended(store))[0] == 2
    assert hint.read_bytes() == before


def test_type_sensitive_prefix_and_unchanged_unknown_policy(store, capsys):
    new = extended(store)
    new["fixtures"][0]["import_seed"] = False
    assert invoke(store, capsys, new)[0] == 2
    new = extended(store)
    new["notes"] = "no arbitrary metadata merge"
    assert invoke(store, capsys, new)[0] == 2


def test_fixture_provenance_is_the_only_mutable_metadata(store, capsys):
    new = json.loads(store[0].read_bytes())
    del new["auto"]
    new["fixture_provenance"] = {"review": "external", "finite": [1.25]}
    assert invoke(store, capsys, new)[0] == 0
    assert store[0].read_bytes() == store[1].read_bytes()


def test_real_cooperative_lease_blocks_application(store, capsys):
    before = store[0].read_bytes()
    lock = apply.env.ROOT / "CM0102/Analysis/cmre/command.lock"
    with apply.coordination.lock(lock):
        assert invoke(store, capsys, extended(store))[0] == 2
    assert store[0].read_bytes() == before


def test_noncanonical_hint_path_refused(store, capsys, monkeypatch):
    before = store[0].read_bytes()
    monkeypatch.setattr(apply.env, "HINTS_DIR", apply.env.ROOT / "not-hints")
    assert invoke(store, capsys, extended(store))[0] == 2
    assert store[0].read_bytes() == before


@pytest.mark.parametrize("argv", [[], ["--help"], ["--unknown"]])
def test_cli_errors_are_json_only(capsys, argv):
    assert apply.main(argv) == 2
    assert json.loads(capsys.readouterr().out)["status"] == "refused"


def test_validation_exit_is_structured(store, capsys, monkeypatch):
    def fail(*args):
        raise SystemExit("SECRET_IDENTITY_DETAIL")

    monkeypatch.setattr(apply.difftest, "validate_hints", fail)
    before = store[0].read_bytes()
    code, result = invoke(store, capsys, extended(store))
    assert code == 2 and "SECRET" not in json.dumps(result)
    assert store[0].read_bytes() == before


@pytest.mark.parametrize("edit", ["fixtures", "captures", "provenance", "unchanged"])
def test_auto_true_proposal_refused_without_rewriting(store, capsys, edit):
    before = store[0].read_bytes()
    new = json.loads(before)
    if edit == "fixtures":
        new["fixtures"].append(fixture(1))
    elif edit == "captures":
        regs = {n: 0 for n in apply.difftest.CAPTURE_REG_NAMES}
        name = apply.difftest.save_capture(VA, regs, 0x37F, [0] * 64, {}, 0, {})
        new["captures"] = [name]
    elif edit == "provenance":
        new["fixture_provenance"] = {"review": "external"}
    raw = json.dumps(new).encode() + b"\r\n"
    store[1].write_bytes(raw)
    code, result = invoke(store, capsys)
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before
    assert store[1].read_bytes() == raw
    assert not store[2].exists()


@pytest.mark.parametrize("number", ["1e999", "-1e999"])
@pytest.mark.parametrize("source", ["proposal", "prior"])
def test_nonfinite_exponent_refused_in_nested_provenance(store, capsys, number, source):
    new = extended(store)
    new["fixture_provenance"] = {"nested": [{"value": "EXPONENT"}]}
    raw = json.dumps(new).replace('"EXPONENT"', number).encode()
    if source == "prior":
        store[0].write_bytes(raw)
        new["fixture_provenance"]["nested"][0]["value"] = 1.25
        store[1].write_bytes(json.dumps(new).encode())
    else:
        store[1].write_bytes(raw)
    before, proposal = store[0].read_bytes(), store[1].read_bytes()
    code, result = invoke(store, capsys)
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before
    assert store[1].read_bytes() == proposal
    assert not store[2].exists()


# --- accepted lifted / no-C C-only fixture lane -------------------------------

def lifted_record(fn=FN, *, c=None, validated=True, lifted=True):
    va = int(fn[3:], 16)
    record = {"schema": "cmre-ledger/1", "va": f"0x{va:08x}",
              "extent": {"start": f"0x{va:08x}", "end": f"0x{va + 1:08x}",
                         "bytes": 1, "sha256": "a" * 64},
              "status": {"identified": True, "decompiled": bool(c),
                         "compiled": bool(c), "validated": validated,
                         "integrated": False}}
    if lifted:
        record["lifted"] = {"source": "recovery/lifted/fn_00500000.S",
                            "source_sha256": "b" * 64,
                            "validation": {"profile": "exact", "cases": 600}}
    if c is not None:
        record["c"] = c
    return record


def install_ledger(record=None, fn=FN):
    apply.env.LEDGER_DIR.mkdir(parents=True, exist_ok=True)
    (apply.env.LEDGER_DIR / f"{fn}.json").write_text(json.dumps(record or lifted_record(fn)))


def c_proposal(store, *, extra=None, replace=None):
    old = json.loads(store[0].read_bytes())
    if old.get("auto") is True:
        del old["auto"]
    c_hints = dict(old.get("c_hints", {}))
    if extra:
        c_hints.update(extra)
    if replace is not None:
        c_hints = replace
    return {**old, "c_hints": c_hints}


def test_accepted_lifted_c_only_fixtures_append(store, capsys):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    shared = old["fixtures"]
    new = c_proposal(store, extra={
        "fixtures": shared + [fixture(1)],
        "fixture_provenance": {"0": "existing", "1": "new_c_only"},
    })
    code, result = invoke(store, capsys, new)
    assert code == 0 and result["status"] == "applied"
    assert result["c_fixtures_added"] == 1
    assert result["c_captures_added"] == 0
    manifest = json.loads((store[2] / "manifest.json").read_bytes())
    assert manifest["scope"] == "accepted-lifted-c-only"
    assert manifest["prior_absent"] is False
    assert manifest["ledger_sha256"] == apply.env.digest(apply.env.LEDGER_DIR / f"{FN}.json")


def test_accepted_lifted_c_only_captures_append_and_prefix(store, capsys):
    install_ledger()
    regs = {n: 0 for n in apply.difftest.CAPTURE_REG_NAMES}
    first = apply.difftest.save_capture(VA, regs, 0x37F, [0] * 64, {}, 0, {})
    second = apply.difftest.save_capture(VA, regs, 0x37F, [1] * 64, {}, 0, {})
    assert invoke(store, capsys, c_proposal(store, extra={"captures": [first]}))[0] == 0
    store = (store[0], store[1], apply.env.ANALYSIS / "capture-evidence-2")
    assert invoke(store, capsys, c_proposal(store, extra={"captures": [first, second]}))[0] == 0
    store = (store[0], store[1], apply.env.ANALYSIS / "capture-evidence-3")
    code, result = invoke(store, capsys, c_proposal(store, extra={"captures": [second, first]}))
    assert code == 2 and result["status"] == "refused"


@pytest.mark.parametrize("change", [
    "top-level-fixtures", "top-level-cases", "top-level-auto", "c-arg-count",
    "c-budget", "c-seed", "c-min-coverage", "c-args", "c-globals", "c-arena",
    "c-fixture-prefix", "c-capture-prefix", "c-provenance-prefix", "c-remove-fixtures",
    "c-add-nonmutable", "c-fixture-evidence",
])
def test_accepted_lifted_shared_or_c_policy_changes_are_refused(store, capsys, change):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    new = c_proposal(store, extra={"fixtures": old["fixtures"] + [fixture(1)]})
    if change == "top-level-fixtures":
        new["fixtures"] = old["fixtures"] + [fixture(2)]
        new["c_hints"] = {}
    elif change == "top-level-cases":
        new["cases"] = 99
    elif change == "top-level-auto":
        new["auto"] = False
    elif change == "c-arg-count":
        new["c_hints"]["arg_count"] = 11
    elif change == "c-budget":
        new["c_hints"]["budget"] = 1000
    elif change == "c-seed":
        new["c_hints"]["seed"] = 1
    elif change == "c-min-coverage":
        new["c_hints"]["min_coverage"] = 0.5
    elif change == "c-args":
        new["c_hints"]["args"] = ["ptr"]
    elif change == "c-globals":
        new["c_hints"]["globals"] = {"0x00500000": [1]}
    elif change == "c-arena":
        new["c_hints"]["arena_size"] = 4096
    elif change == "c-fixture-prefix":
        new["c_hints"]["fixtures"][0]["import_seed"] = 7
    elif change == "c-capture-prefix":
        regs = {n: 0 for n in apply.difftest.CAPTURE_REG_NAMES}
        one = apply.difftest.save_capture(VA, regs, 0x37F, [0] * 64, {}, 0, {})
        two = apply.difftest.save_capture(VA, regs, 0x37F, [1] * 64, {}, 0, {})
        store[0].write_bytes(json.dumps(c_proposal(store, extra={"captures": [one]})).encode())
        new = c_proposal(store, extra={"captures": [two, one]})
    elif change == "c-provenance-prefix":
        old["c_hints"] = {"fixture_provenance": {"0": "existing"}}
        store[0].write_bytes(json.dumps(old).encode())
        new = c_proposal(store, extra={"fixtures": old["fixtures"] + [fixture(1)],
                                       "fixture_provenance": {"0": "changed"}})
    elif change == "c-remove-fixtures":
        old["c_hints"] = {"fixtures": old["fixtures"] + [fixture(1)]}
        store[0].write_bytes(json.dumps(old).encode())
        new = c_proposal(store, extra={})
        new["c_hints"].pop("fixtures")
    elif change == "c-add-nonmutable":
        new["c_hints"]["struct_size"] = 16
    elif change == "c-fixture-evidence":
        old["c_hints"] = {"fixture_evidence": "existing"}
        store[0].write_bytes(json.dumps(old).encode())
        new["c_hints"]["fixture_evidence"] = "changed"
    before = store[0].read_bytes()
    code, result = invoke(store, capsys, new)
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before
    assert not store[2].exists()


@pytest.mark.parametrize("state", ["accepted-c", "legacy", "unvalidated", "no-lifted"])
def test_accepted_c_and_ineligible_ledgers_are_refused(store, capsys, state):
    if state == "accepted-c":
        install_ledger(lifted_record(c={"source": "recovery/c/fn_00500000.c"}))
    elif state == "legacy":
        install_ledger()
        legacy = apply.env.ROOT / "legacy.json"
        legacy.write_text(json.dumps({"functions": [{"va": hex(VA), "size": 1,
                                                    "sha256": "a" * 64}]}))
        apply.env.REGISTRY.write_text('{"current": ["legacy.json"]}')
    elif state == "unvalidated":
        install_ledger(lifted_record(validated=False))
    else:
        install_ledger(lifted_record(lifted=False))
    before = store[0].read_bytes()
    assert invoke(store, capsys, c_proposal(store, extra={"fixtures": [fixture(1)]}))[0] == 2
    assert store[0].read_bytes() == before


def test_absent_canonical_hint_requires_explicit_token(store, capsys):
    hint, proposal, evidence = store
    hint.unlink()
    install_ledger()
    new = {"c_hints": {"fixtures": [fixture(1)]}}
    assert invoke(store, capsys, new, "0" * 64)[0] == 2
    assert not hint.exists()
    code, result = invoke(store, capsys, new, "absent")
    assert code == 0 and result["prior_sha256"] == "absent"
    assert result["prior_absent"] is True
    assert json.loads(hint.read_bytes())["c_hints"]["fixtures"] == [fixture(1)]
    manifest = json.loads((evidence / "manifest.json").read_bytes())
    assert manifest["prior_absent"] is True and manifest["prior_sha256"] == "absent"
    assert (evidence / "prior.json").read_bytes() == b""
    assert manifest["scope"] == "accepted-lifted-c-only"


def test_absent_token_refused_without_lifted_ledger_or_with_existing_hint(store, capsys):
    hint, proposal, evidence = store
    assert invoke(store, capsys, {"c_hints": {"fixtures": [fixture(1)]}}, "absent")[0] == 2
    install_ledger()
    assert invoke(store, capsys, c_proposal(store, extra={"fixtures": [fixture(1)]}), "absent")[0] == 2
    assert not evidence.exists()


def test_absent_identity_is_rechecked_after_evidence(store, capsys, monkeypatch):
    hint, proposal, evidence = store
    hint.unlink()
    install_ledger()
    original = apply.retain

    def create_during_retain(*args, **kwargs):
        original(*args, **kwargs)
        hint.write_bytes(b'{"newer": true}\n')

    monkeypatch.setattr(apply, "retain", create_during_retain)
    assert invoke(store, capsys, {"c_hints": {"fixtures": [fixture(1)]}}, "absent")[0] == 2
    assert hint.read_bytes() == b'{"newer": true}\n'
    assert (evidence / "prior.json").read_bytes() == b""


def test_absent_proposal_cannot_add_shared_top_level_inputs(store, capsys):
    hint, proposal, evidence = store
    hint.unlink()
    install_ledger()
    new = {"fixtures": [fixture(1)], "c_hints": {"fixtures": [fixture(1)]}}
    assert invoke(store, capsys, new, "absent")[0] == 2
    assert not hint.exists()
    assert not evidence.exists()


@pytest.mark.parametrize("target", ["proposal", "hint", "ledger"])
def test_proposal_hint_and_ledger_symlinks_are_refused(store, capsys, target):
    hint, proposal, evidence = store
    install_ledger()
    real = apply.env.ROOT / f"real-{target}.json"
    if target == "proposal":
        real.write_text(json.dumps(c_proposal(store, extra={"fixtures": [fixture(1)]})))
        link = proposal
    elif target == "hint":
        real.write_bytes(hint.read_bytes())
        link = hint
    else:
        real.write_text(json.dumps(lifted_record()))
        link = apply.env.LEDGER_DIR / f"{FN}.json"
    try:
        link.unlink(missing_ok=True)
        os.symlink(real, link)
    except (OSError, NotImplementedError):
        pytest.skip("symlinks unavailable")
    hint_before = hint.read_bytes()
    real_before = real.read_bytes()
    assert invoke(store, capsys, c_proposal(store, extra={"fixtures": [fixture(1)]}))[0] == 2
    assert hint.read_bytes() == hint_before
    if target != "proposal":       # invoke writes the proposal path before the tool checks it
        assert real.read_bytes() == real_before

@pytest.mark.parametrize("shared_count,added", [(3, 3), (8, 3)])
def test_c_only_fixture_append_preserves_inherited_shared_prefix(store, capsys, shared_count, added):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    shared = [fixture(seed) for seed in range(shared_count)]
    old["fixtures"] = shared
    store[0].write_bytes(json.dumps(old).encode())
    wanted = shared + [fixture(shared_count + offset) for offset in range(added)]
    new = c_proposal(store, extra={"fixtures": wanted})
    code, result = invoke(store, capsys, new)
    assert code == 0 and result["status"] == "applied"
    assert result["c_fixtures_added"] == added
    accepted = json.loads(store[0].read_bytes())
    assert accepted["fixtures"] == shared
    assert accepted["c_hints"]["fixtures"] == wanted


@pytest.mark.parametrize("shared_count,added", [(3, 3), (8, 3)])
def test_c_only_fixture_append_rejects_dropping_an_inherited_shared_fixture(
        store, capsys, shared_count, added):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    shared = [fixture(seed) for seed in range(shared_count)]
    old["fixtures"] = shared
    store[0].write_bytes(json.dumps(old).encode())
    dropped = shared[1:] + [fixture(shared_count + offset) for offset in range(added)]
    before = store[0].read_bytes()
    code, result = invoke(store, capsys, c_proposal(store, extra={"fixtures": dropped}))
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before
    assert not store[2].exists()


def test_c_only_capture_append_preserves_inherited_shared_prefix(store, capsys):
    install_ledger()
    regs = {n: 0 for n in apply.difftest.CAPTURE_REG_NAMES}
    first = apply.difftest.save_capture(VA, regs, 0x37F, [0] * 64, {}, 0, {})
    second = apply.difftest.save_capture(VA, regs, 0x37F, [1] * 64, {}, 0, {})
    old = json.loads(store[0].read_bytes())
    old["captures"] = [first]
    store[0].write_bytes(json.dumps(old).encode())
    assert invoke(store, capsys, c_proposal(store, extra={"captures": [first, second]}))[0] == 0
    store = (store[0], store[1], apply.env.ANALYSIS / "inherited-capture-reject")
    before = store[0].read_bytes()
    code, result = invoke(store, capsys, c_proposal(store, extra={"captures": [second]}))
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before

def test_inherited_prefix_applies_when_c_hints_exists_without_fixtures_or_captures(store, capsys):
    install_ledger()
    regs = {n: 0 for n in apply.difftest.CAPTURE_REG_NAMES}
    first = apply.difftest.save_capture(VA, regs, 0x37F, [0] * 64, {}, 0, {})
    second = apply.difftest.save_capture(VA, regs, 0x37F, [1] * 64, {}, 0, {})
    old = json.loads(store[0].read_bytes())
    shared = [fixture(seed) for seed in range(3)]
    old["fixtures"] = shared
    old["captures"] = [first]
    old["c_hints"] = {"fixture_provenance": {"0": "shared"}}
    store[0].write_bytes(json.dumps(old).encode())
    wanted = shared + [fixture(3), fixture(4)]
    new = c_proposal(store, extra={
        "fixtures": wanted,
        "captures": [first, second],
        "fixture_provenance": {"0": "shared", "1": "c-only"},
    })
    code, result = invoke(store, capsys, new)
    assert code == 0 and result["c_fixtures_added"] == 2
    assert result["c_captures_added"] == 1
    store = (store[0], store[1], apply.env.ANALYSIS / "c-hints-existing-reject")
    before = store[0].read_bytes()
    dropped = c_proposal(store, extra={
        "fixtures": shared[1:] + [fixture(3), fixture(4)],
        "captures": [second],
        "fixture_provenance": {"0": "shared", "1": "c-only"},
    })
    assert invoke(store, capsys, dropped)[0] == 2
    assert store[0].read_bytes() == before


@pytest.mark.parametrize("c_hints", [{}, {"fixture_provenance": {"review": "retained"}}])
def test_c_only_rejects_invalid_inherited_capture(store, capsys, c_hints):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    old["captures"] = ["a" * 64]  # Valid name, but no valid capture payload.
    old["c_hints"] = c_hints
    store[0].write_bytes(json.dumps(old).encode())
    before = store[0].read_bytes()
    new = c_proposal(store, extra={"fixtures": old["fixtures"] + [fixture(1)]})
    code, result = invoke(store, capsys, new)
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before
    assert not store[2].exists()


def test_duplicate_ledger_key_cannot_hide_accepted_c(store, capsys):
    install_ledger()
    record = json.dumps(lifted_record())
    raw = record[:-1] + ', "c": {"source": "accepted.c"}, "c": null}'
    (apply.env.LEDGER_DIR / f"{FN}.json").write_text(raw)
    before = store[0].read_bytes()
    code, result = invoke(store, capsys, c_proposal(
        store, extra={"fixtures": json.loads(before)["fixtures"] + [fixture(1)]}))
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before
    assert not store[2].exists()


@pytest.mark.parametrize("change", ["accepted-c", "lifted-ledger", "canonical-hint"])
def test_c_only_rechecks_ledger_and_hint_after_retaining_receipts(
        store, capsys, monkeypatch, change):
    install_ledger()
    before = store[0].read_bytes()
    original = apply.retain

    def drift(*args):
        original(*args)
        if change == "accepted-c":
            install_ledger(lifted_record(c={"source": "accepted.c"}))
        elif change == "lifted-ledger":
            record = lifted_record()
            record["lifted"]["validation"]["cases"] += 1
            install_ledger(record)
        else:
            store[0].write_bytes(b'{"notes": "newer work"}\n')

    monkeypatch.setattr(apply, "retain", drift)
    new = c_proposal(store, extra={"fixtures": json.loads(before)["fixtures"] + [fixture(1)]})
    code, result = invoke(store, capsys, new)
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == (b'{"notes": "newer work"}\n'
                                    if change == "canonical-hint" else before)
    assert (store[2] / "prior.json").read_bytes() == before
    assert (store[2] / "proposal.json").read_bytes() == store[1].read_bytes()


@pytest.mark.parametrize("absent", [False, True])
def test_c_only_checks_identity_inside_writer_lease(store, capsys, monkeypatch, absent):
    from contextlib import contextmanager

    install_ledger()
    new = c_proposal(store, extra={"fixtures": [fixture(), fixture(1)]})
    expected = apply.env.digest(store[0])
    if absent:
        store[0].unlink()
        new = {"c_hints": {"fixtures": [fixture(1)]}}
        expected = "absent"

    @contextmanager
    def changed_at_acquisition():
        store[0].write_bytes(b'{"notes": "lease-time work"}\n')
        yield

    monkeypatch.setattr(apply.coordination, "writer", changed_at_acquisition)
    code, result = invoke(store, capsys, new, expected)
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == b'{"notes": "lease-time work"}\n'
    assert not store[2].exists()


def test_c_only_preserves_existing_generators_and_effective_provenance(store, capsys):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    old["fixture_provenance"] = {"0": {"source": "shared", "case": 0}}
    old["c_hints"] = {"arg_count": 10, "args": ["small"], "ecx": "ptr"}
    store[0].write_bytes(json.dumps(old).encode())
    new = c_proposal(store, extra={
        "fixtures": old["fixtures"] + [fixture(1)],
        "fixture_provenance": {**old["fixture_provenance"], "1": {"source": "C-only"}},
    })
    code, result = invoke(store, capsys, new)
    assert code == 0 and result["c_fixture_provenance_added"] == 1
    assert json.loads(store[0].read_bytes()) == new


@pytest.mark.parametrize("mode", ["drop", "replace", "type-change"])
def test_c_only_preserves_inherited_provenance(store, capsys, mode):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    old["fixture_provenance"] = {"0": {"source": "shared", "case": 1}}
    old["c_hints"] = {"args": ["small"]}
    store[0].write_bytes(json.dumps(old).encode())
    provenance = {"1": {"source": "C-only"}}
    if mode != "drop":
        provenance["0"] = {"source": "changed" if mode == "replace" else "shared",
                           "case": True if mode == "type-change" else 1}
    before = store[0].read_bytes()
    new = c_proposal(store, extra={"fixture_provenance": provenance})
    assert invoke(store, capsys, new)[0] == 2
    assert store[0].read_bytes() == before
    assert not store[2].exists()


@pytest.mark.parametrize("key", ["fixture_evidence", "notes", "input_probe", "input_probe_sha256"])
def test_c_only_new_provenance_metadata_allowed(store, capsys, key):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    old.pop("auto")
    store[0].write_bytes(json.dumps(old).encode())
    new = c_proposal(store, extra={key: "new provenance"})
    code, result = invoke(store, capsys, new)
    assert code == 0 and result["status"] == "applied"
    assert json.loads(store[0].read_bytes())["c_hints"][key] == "new provenance"


@pytest.mark.parametrize("key", ["fixture_evidence", "notes", "input_probe", "input_probe_sha256"])
@pytest.mark.parametrize("location", ["shared", "c"])
@pytest.mark.parametrize("change", ["replace", "remove"])
def test_c_only_existing_provenance_metadata_frozen(store, capsys, key, location, change):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    old.pop("auto")
    old["c_hints"] = {}
    target = old if location == "shared" else old["c_hints"]
    target[key] = "retained provenance"
    store[0].write_bytes(json.dumps(old).encode())
    new = c_proposal(store)
    target = new if location == "shared" else new["c_hints"]
    if change == "replace":
        target[key] = "replacement"
    else:
        del target[key]
    before = store[0].read_bytes()
    code, result = invoke(store, capsys, new)
    assert code == 2 and result["status"] == "refused"
    assert store[0].read_bytes() == before
    assert not store[2].exists()


@pytest.mark.parametrize("change", ["remove", "retain", "false", "shared-policy"])
def test_c_only_auto_claim_contract(store, capsys, change):
    install_ledger()
    new = c_proposal(store, extra={"fixtures": [fixture(), fixture(1)]})
    new["auto"] = True
    if change in {"remove", "shared-policy"}:
        del new["auto"]
    elif change == "false":
        new["auto"] = False
    if change == "shared-policy":
        new["cases"] += 1
    before = store[0].read_bytes()
    code, result = invoke(store, capsys, new)
    assert code == (0 if change == "remove" else 2)
    if change == "remove":
        assert "auto" not in json.loads(store[0].read_bytes())
    else:
        assert result["status"] == "refused"
        assert store[0].read_bytes() == before
        assert not store[2].exists()


@pytest.mark.parametrize("key", ["fixture_evidence", "notes", "input_probe", "input_probe_sha256"])
def test_c_only_cannot_shadow_existing_shared_provenance_metadata(store, capsys, key):
    install_ledger()
    old = json.loads(store[0].read_bytes())
    old.pop("auto")
    old[key] = "inherited provenance"
    store[0].write_bytes(json.dumps(old).encode())
    before = store[0].read_bytes()
    new = c_proposal(store, extra={key: "replacement"})
    assert invoke(store, capsys, new)[0] == 2
    assert store[0].read_bytes() == before
    assert not store[2].exists()
