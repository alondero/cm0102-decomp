"""End-to-end accept: records are byte-stable and revalidate cleanly."""
import json

import pytest

from cmre import env, ledger, recover

VA = 0x004671B0


@pytest.fixture
def sandbox(tmp_path, monkeypatch):
    for name in ("LEDGER_DIR", "LIFTED_DIR", "BLOCKED_DIR"):
        monkeypatch.setattr(env, name, tmp_path / name.lower())
    # Job receipts and the command lease stay here, apart from a live machine job.
    # Inventory paths are fixed at import, so they still read the real exports.
    monkeypatch.setattr(env, "ANALYSIS", tmp_path / "analysis")
    monkeypatch.setenv("CMRE_COORDINATION_PATH", str(tmp_path / "command.lock"))
    monkeypatch.delenv("CMRE_WRITER_TOKEN", raising=False)
    monkeypatch.delenv("CMRE_WRITER_PATH", raising=False)
    return tmp_path


def test_accepted_source_hash_matches_file_on_disk(sandbox):
    ok, result = recover.accept(VA)
    assert ok, result
    record = json.loads(ledger.record_path(VA).read_text(encoding="utf-8"))
    source = env.LIFTED_DIR / f"fn_{VA:08x}.S"
    assert b"\r\n" not in source.read_bytes()  # LF on every platform: hashes must not depend on the OS
    assert env.digest(source) == record["lifted"]["source_sha256"]
    assert recover.test_lifted(VA)["source_sha256"] == record["lifted"]["source_sha256"]


def test_records_are_lf_and_byte_stable(sandbox):
    recover.accept(VA)
    first = ledger.record_path(VA).read_bytes()
    ledger.record_path(VA).unlink()
    recover.accept(VA)
    assert ledger.record_path(VA).read_bytes() == first
    assert b"\r\n" not in first


def test_harness_is_deterministic_across_processes_and_hash_seeds():
    # Per-function acceptance re-runs in-process; cross-process determinism is a harness
    # property, checked here once per harness version.
    import os
    import subprocess
    import sys

    def digest(seed):
        out = subprocess.run([sys.executable, "-m", "cmre", "test", "0x00414c20", "--fresh", "--json", "--cases", "120"],
                             cwd=env.ROOT, capture_output=True, text=True, env=dict(os.environ, PYTHONHASHSEED=seed))
        return json.loads(out.stdout)["difftest"]["result_digest"]

    assert digest("1") == digest("12345")


def test_accept_does_not_spawn_a_second_harness_process(sandbox, monkeypatch):
    import subprocess
    import sys

    real = subprocess.run

    def guarded(args, *a, **k):
        assert sys.executable not in [str(x) for x in args], "accept must not re-run the harness in a new process"
        return real(args, *a, **k)  # toolchain (gcc/ld/nm) is fine

    monkeypatch.setattr(subprocess, "run", guarded)
    ok, result = recover.accept(VA)
    assert ok, result


def test_harness_digest_covers_only_behaviour_modules():
    # Editing the guard, CLI or queue must not change the recorded harness identity,
    # and the identity must be of the code loaded in this process (computed once).
    assert {"guard.py", "cli.py", "queue.py"}.isdisjoint(recover.HARNESS_MODULES)
    assert {"lift.py", "build.py", "emu.py", "difftest.py", "imports.py"} <= set(recover.HARNESS_MODULES)
    assert recover.harness_digest() is recover.harness_digest()


def test_refresh_rewrites_record_with_current_harness(sandbox):
    from cmre import cli

    recover.accept(VA)
    path = ledger.record_path(VA)
    record = json.loads(path.read_text(encoding="utf-8"))
    record["lifted"]["harness_sha256"] = "stale"
    path.write_text(json.dumps(record), encoding="utf-8")
    assert cli.main(["revalidate", "--refresh", "--jobs", "1"]) == 0
    refreshed = json.loads(path.read_text(encoding="utf-8"))
    assert refreshed["lifted"]["harness_sha256"] == recover.harness_digest()
    # The job receipt stays in the sandbox; a real one would become `cmre job status`'s latest.
    assert len(list((env.ANALYSIS / "cmre/jobs").glob("*.json"))) == 1
    assert env.ANALYSIS.is_relative_to(sandbox)


@pytest.mark.parametrize('source_drift', [False, True])
@pytest.mark.parametrize('stale_lifted', [False, True])
def test_inventory_refresh_validates_c_without_erasing_its_lifted_baseline(sandbox, monkeypatch, source_drift, stale_lifted):
    from cmre import cli
    monkeypatch.setattr(env, 'C_DIR', sandbox / 'c')
    env.C_DIR.mkdir()
    assert recover.accept(VA)[0]
    source = env.C_DIR / f'fn_{VA:08x}.c'
    source.write_text('#include "cmre.h"\nCM_THISCALL void *cm_fn_004671b0(void *self) { return (u8 *)self + 0x20; }\n')
    assert recover.accept(VA, 'c')[0]
    path = ledger.record_path(VA)
    record = json.loads(path.read_text())
    if stale_lifted:
        record['lifted']['harness_sha256'] = 'stale'
    record['c']['harness_sha256'] = 'stale'
    ledger.write_json(path, record)
    before = path.read_bytes()
    if source_drift:
        source.write_bytes(source.read_bytes()+b'\n')  # Same behavior, changed accepted source identity.
    _, result = cli._revalidate_one(VA, refresh=True)
    if source_drift:
        assert result == 'FAIL refresh c source-drift'
        assert path.read_bytes() == before
    else:
        assert result == 'ok'
        refreshed = json.loads(path.read_text())
        assert refreshed['c']['harness_sha256'] == recover.harness_digest()
        assert refreshed['c']['source_sha256'] == record['c']['source_sha256']
        assert refreshed['lifted'] == record['lifted']
        assert refreshed['status']['decompiled'] is True


def test_unblock_batch_keeps_agent_blocks(sandbox):
    from cmre import cli

    recover.block(0x00450A70, "low-coverage: 0.1 < 0.6", by="batch")
    recover.block(0x004671B0, "mismatch: agent diagnosis", by="agent")
    assert cli.main(["unblock", "--batch"]) == 0
    assert set(ledger.blocked()) == {0x004671B0}
