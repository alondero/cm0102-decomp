"""Asset-free application tests through the real CLI call seam."""
import importlib.util
import json
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
