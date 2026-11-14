"""Asset free tests for the scoped original-only provider broker.

Every test drives the broker through the injected fake backend, so no game
asset, engine, harness or network access is required.
"""

from __future__ import annotations

import gzip
import hashlib
import json
import sys
from pathlib import Path

import pytest

from cmre import provider_broker
from cmre.provider_broker import BrokerError, dispatch


# fakes


class FakeSection:
    def __init__(self, start: int, end: int, writable: bool, executable: bool) -> None:
        self.start = start
        self.end = end
        self.writable = writable
        self.executable = executable


class FakeImage:
    def __init__(self) -> None:
        self.sections = [
            FakeSection(0x1000, 0x2000, True, False),
            FakeSection(0x400000, 0x401000, False, True),
        ]

    def section_at(self, address: int):
        for section in self.sections:
            if section.start <= address < section.end:
                return section
        return None


class FakeBackend:
    def __init__(self) -> None:
        self.calls: list[tuple] = []
        self.image = FakeImage()
        self.rows = [
            {"va": "0x401000", "raw": "90", "mnemonic": "nop", "op_str": "", "kind": "insn"},
            {
                "va": "0x401004",
                "raw": "89c8",
                "mnemonic": "mov",
                "op_str": "eax, ecx",
                "kind": "insn",
            },
            {"va": "0x401008", "raw": "c3", "mnemonic": "ret", "op_str": "", "kind": "insn"},
            {"va": "0x40100c", "raw": "c3", "mnemonic": "ret", "op_str": "", "kind": "insn"},
        ]

    def evidence(self, function, start, limit):
        self.calls.append(("evidence", function, start, limit))
        # Live-shaped backend: always return the complete decode; the broker
        # owns scoping so it can retain the full artifact and report a cursor.
        return {
            "insns": list(self.rows),
            "total": len(self.rows),
            "body_bytes": 32,
            "callees": [0x401100],
            "target_sha256": "0" * 64,
        }

    def memory(self, address, size):
        self.calls.append(("memory", address, size))
        return bytes((address + index) & 0xFF for index in range(size))

    def probe(self, function, fixture, budget, shapes, output):
        self.calls.append(("probe", function, fixture, budget, shapes, output))
        outdir = Path(output)
        outdir.mkdir(parents=True, exist_ok=True)
        raw = {
            "result": {
                "events": [
                    {"pc": "0x401000", "kind": "entry"},
                    {"pc": "0x401008", "kind": "ret"},
                ],
                "arrivals": {"4198400": 1},
                "returns": {},
            }
        }
        (outdir / "raw.json.gz").write_bytes(gzip.compress(json.dumps(raw).encode()))
        (outdir / "input.json.gz").write_bytes(
            gzip.compress(json.dumps({"fixture": fixture}).encode())
        )
        (outdir / "execution.json").write_text(json.dumps({"outcome": "ok"}))
        return {
            "outcome": "ok",
            "detail": "synthetic",
            "regs": {"eax": "0x0"},
            "import_count": 0,
        }

    def build(self, candidate, output):
        self.calls.append(("build", candidate, output))
        outdir = Path(output)
        outdir.mkdir(parents=True, exist_ok=True)
        (outdir / "candidate.o").write_bytes(b"\x00")
        (outdir / "candidate.exe").write_bytes(b"\x01")
        return {"candidate": str(candidate), "files": ["candidate.o", "candidate.exe"]}

    def screen(self, function):
        self.calls.append(("screen", function))
        return {"outcome": "screen-ok", "function": function}


# helpers


def _sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _make_job(root: Path) -> dict:
    root.mkdir(parents=True, exist_ok=True)
    seed = root / "seed.json"
    seed.write_text(
        json.dumps(
            {"regs": {"eax": 0}, "stack": [1, 2, 3], "arena_writes": {}, "globals": {}}
        )
    )
    return {
        "function": "0x401000",
        "allowed_functions": ["0x401000", "0x401100"],
        "seed_fixture": {"path": str(seed), "sha256": _sha256(seed)},
        "directory": str(root),
    }


def _probe_request(patch=None) -> dict:
    request = {
        "function": "0x401000",
        "name": "probe-one",
        "shapes": {"0x401000": {"ecx": False, "stack_words": 0}},
    }
    if patch is not None:
        request["patch"] = patch
    return request


def _no_duplicate_pairs(pairs):
    result = {}
    for key, value in pairs:
        assert key not in result, f"duplicate JSON key {key!r}"
        result[key] = value
    return result


# evidence


def test_evidence_scopes_and_limits(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    response = dispatch(
        job,
        {"kind": "evidence", "function": "0x401000", "start": "0x401004", "limit": 2},
        out,
        backend=backend,
    )

    result = response["result"]
    assert result["function"] == "0x401000"
    assert [row["va"] for row in result["instructions"]] == ["0x401004", "0x401008"]
    assert result["total"] == 4
    assert result["remaining"] == 1
    assert result["callees"] == [0x401100]
    assert result["provenance"]["target"] == "original"
    assert result["provenance"]["target_sha256"] == "0" * 64
    assert backend.calls == [("evidence", 0x401000, 0x401004, 2)]


def test_evidence_keeps_the_complete_raw_artifact(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    response = dispatch(job, {"kind": "evidence", "limit": 1}, out, backend=backend)

    assert len(response["result"]["instructions"]) == 1
    artifact = response["artifacts"]["artifacts/instructions.json"]
    assert artifact["kind"] == "instructions"
    raw = json.loads(Path(artifact["path"]).read_text())
    assert len(raw["instructions"]) == 4


def test_evidence_rejects_a_function_outside_the_allowed_scope(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "evidence", "function": "0x402000"},
            out,
            backend=backend,
        )
    assert backend.calls == []
    assert not (out / "receipt.json").exists()


@pytest.mark.parametrize("limit", [0, 121, "80", True])
def test_evidence_rejects_out_of_range_limits(tmp_path, limit):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    with pytest.raises(BrokerError):
        dispatch(job, {"kind": "evidence", "limit": limit}, out, backend=backend)
    assert backend.calls == []


def test_memory_reads_are_bounded(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    response = dispatch(
        job, {"kind": "memory", "address": "0x401000", "size": 4}, out, backend=backend
    )
    assert response["result"]["size"] == 4
    assert response["result"]["data"] == "00010203"

    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "memory", "address": "0x401000", "size": 129},
            tmp_path / "job" / "dispatch-2",
            backend=backend,
        )


# probe


def test_probe_rejects_data_writes_before_the_backend_runs(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    with pytest.raises((BrokerError, ValueError)):
        dispatch(
            job,
            {
                "kind": "probe",
                "request": _probe_request({"globals": {"0x5000": 1}}),
            },
            out,
            backend=backend,
        )
    assert [call[0] for call in backend.calls] == []
    assert not (out / "receipt.json").exists()


def test_probe_rejects_writes_into_a_read_only_section(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    with pytest.raises((BrokerError, ValueError)):
        dispatch(
            job,
            {
                "kind": "probe",
                "request": _probe_request({"globals": {"0x400100": 1}}),
            },
            out,
            backend=backend,
        )
    assert backend.calls == []


def test_probe_runs_the_original_and_retains_the_raw_artifacts(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    response = dispatch(
        job,
        {"kind": "probe", "request": _probe_request({"regs": {"eax": 7}})},
        out,
        backend=backend,
    )

    assert backend.calls[0][0] == "probe"
    fixture = backend.calls[0][2]
    assert fixture["regs"]["eax"] == 7
    kinds = {name: value["kind"] for name, value in response["artifacts"].items()}
    assert kinds["artifacts/raw.json.gz"] == "raw"
    assert kinds["artifacts/fixture.json"] == "fixture"
    assert kinds["artifacts/execution.json"] == "execution"
    assert response["result"]["summary"]["outcome"] == "ok"


def test_probe_refuses_a_tampered_seed_fixture(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    Path(job["seed_fixture"]["path"]).write_text(json.dumps({"regs": {}}))

    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "probe", "request": _probe_request()},
            out,
            backend=backend,
        )
    assert backend.calls == []


def test_probe_base_must_be_a_verified_prior_fixture(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    first = dispatch(
        job,
        {"kind": "probe", "request": _probe_request()},
        out,
        backend=backend,
    )
    artifact_id = "artifacts/fixture.json"
    prior = {artifact_id: first}

    second = tmp_path / "job" / "dispatch-2"
    response = dispatch(
        job,
        {"kind": "probe", "request": _probe_request(), "base": artifact_id},
        second,
        prior=prior,
        backend=backend,
    )
    assert response["result"]["base"] == artifact_id

    Path(first["artifacts"][artifact_id]["path"]).write_text(json.dumps({"regs": {}}))
    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "probe", "request": _probe_request(), "base": artifact_id},
            tmp_path / "job" / "dispatch-3",
            prior=prior,
            backend=backend,
        )


def test_probe_base_must_be_a_fixture_artifact(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    first = dispatch(
        job, {"kind": "probe", "request": _probe_request()}, out, backend=backend
    )
    artifact_id = "artifacts/raw.json.gz"
    prior = {artifact_id: first}
    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "probe", "request": _probe_request(), "base": artifact_id},
            tmp_path / "job" / "dispatch-2",
            prior=prior,
            backend=backend,
        )
    assert [call[0] for call in backend.calls] == ["probe"]


def test_probe_base_cannot_escape_the_job_root(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    outside = tmp_path / "outside.json"
    outside.write_text(json.dumps({"regs": {}}))

    prior = {
        "fixture": {
            "artifacts": {
                "fixture": {
                    "path": str(outside),
                    "sha256": _sha256(outside),
                    "kind": "fixture",
                }
            }
        }
    }
    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "probe", "request": _probe_request(), "base": "fixture"},
            tmp_path / "job" / "dispatch-1",
            prior=prior,
            backend=backend,
        )
    assert backend.calls == []


# inspect


def test_inspect_slices_a_hash_verified_prior_record(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    first = dispatch(
        job,
        {"kind": "probe", "request": _probe_request()},
        out,
        backend=backend,
    )
    artifact_id = "artifacts/raw.json.gz"
    prior = {artifact_id: first}

    response = dispatch(
        job,
        {
            "kind": "inspect",
            "artifact": artifact_id,
            "section": "events",
            "start": 0,
            "limit": 1,
        },
        tmp_path / "job" / "dispatch-2",
        prior=prior,
        backend=backend,
    )
    result = response["result"]
    assert result["total"] == 2
    assert result["remaining"] == 1
    assert len(result["items"]) == 1
    assert result["items"][0]["kind"] == "entry"

    Path(first["artifacts"][artifact_id]["path"]).write_bytes(gzip.compress(b"{}"))
    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "inspect", "artifact": artifact_id, "section": "events"},
            tmp_path / "job" / "dispatch-3",
            prior=prior,
            backend=backend,
        )


def test_inspect_rejects_an_unknown_section(tmp_path):
    job = _make_job(tmp_path / "job")
    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "inspect", "artifact": "x", "section": "secrets"},
            tmp_path / "job" / "dispatch-1",
            backend=FakeBackend(),
        )


# receipts, replay and refusals


def test_a_finished_output_is_replayed_without_calling_the_backend(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"
    action = {"kind": "evidence", "limit": 2}

    first = dispatch(job, action, out, backend=backend)
    calls = list(backend.calls)
    second = dispatch(job, action, out, backend=backend)

    assert second == first
    assert backend.calls == calls

    receipt = json.loads((out / "receipt.json").read_text())
    assert receipt["state"] == "finished"
    assert receipt["action_fingerprint"] == first["action_fingerprint"]
    assert receipt["job_fingerprint"] == first["job_fingerprint"]


def test_a_running_receipt_refuses_execution(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"
    out.mkdir(parents=True)
    (out / "receipt.json").write_text(json.dumps({"state": "running"}))

    with pytest.raises(BrokerError):
        dispatch(job, {"kind": "evidence"}, out, backend=backend)
    assert backend.calls == []


def test_a_corrupt_receipt_refuses_execution(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"
    out.mkdir(parents=True)
    (out / "receipt.json").write_text("{not json")

    with pytest.raises(BrokerError):
        dispatch(job, {"kind": "evidence"}, out, backend=backend)
    assert backend.calls == []


def test_a_tampered_retained_artifact_refuses_the_replay(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"
    action = {"kind": "evidence"}

    response = dispatch(job, action, out, backend=backend)
    artifact = response["artifacts"]["artifacts/instructions.json"]
    Path(artifact["path"]).write_text("[]")

    with pytest.raises(BrokerError):
        dispatch(job, action, out, backend=backend)


def test_the_output_must_stay_under_the_job_directory(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()

    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "evidence"},
            tmp_path / "elsewhere" / "dispatch-1",
            backend=backend,
        )
    assert backend.calls == []


def test_a_failed_run_is_retained_as_a_failed_receipt(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    def boom(*_args, **_kwargs):
        raise RuntimeError("backend exploded")

    backend.evidence = boom
    with pytest.raises(RuntimeError):
        dispatch(job, {"kind": "evidence"}, out, backend=backend)

    receipt = json.loads((out / "receipt.json").read_text())
    assert receipt["state"] == "failed"
    assert receipt["error"]["type"] == "RuntimeError"


# build and screen


def test_build_only_accepts_the_trusted_candidate(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    with pytest.raises(BrokerError):
        dispatch(job, {"kind": "build"}, out, backend=backend)
    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "build", "candidate": "C:\\tmp\\candidate.c"},
            tmp_path / "job" / "dispatch-2",
            backend=backend,
        )
    assert backend.calls == []

    candidate = tmp_path / "candidate.c"
    candidate.write_text("int main(void){return 0;}\n")
    response = dispatch(
        job,
        {"kind": "build"},
        tmp_path / "job" / "dispatch-3",
        backend=backend,
        candidate=str(candidate),
    )
    assert backend.calls[0][0] == "build"
    assert backend.calls[0][1] == str(candidate)
    assert response["result"]["build"]["candidate"] == str(candidate)


def test_screen_uses_the_job_function_only(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    response = dispatch(job, {"kind": "screen"}, out, backend=backend)
    assert backend.calls == [("screen", 0x401000)]
    assert response["result"]["function"] == "0x401000"

    with pytest.raises(BrokerError):
        dispatch(
            job,
            {"kind": "screen", "function": "0x401100"},
            tmp_path / "job" / "dispatch-2",
            backend=backend,
        )


# module level guarantees


def test_the_module_import_needs_no_assets():
    # Other tests legitimately execute backends and load these modules. Check
    # import isolation in a fresh process with an unavailable original image.
    import subprocess
    result = subprocess.run(['py', '-3.13', '-c',
                             'from pathlib import Path; from cmre import env; '
                             'env.TARGET=Path("missing-game-for-import-check.exe"); '
                             'import cmre.provider_broker'], capture_output=True, text=True)
    assert result.returncode == 0, result.stderr


def test_default_backend_exposes_its_protected_image_lazily(monkeypatch):
    from types import SimpleNamespace
    backend = provider_broker.LocalBackend()
    marker = object()
    checked = []
    monkeypatch.setattr(backend, '_verify_target', lambda: checked.append(True))
    monkeypatch.setattr(backend, '_module', lambda name: SimpleNamespace(primary=lambda: marker))
    assert backend.image is marker
    assert checked == [True]


def test_fp_observation_preserves_physical_80_bits_and_does_not_write_cpu():
    import unicorn
    from unicorn import x86_const as registers
    cpu = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
    cpu.reg_write(registers.UC_X86_REG_FPCW, 0x37f)
    cpu.reg_write(registers.UC_X86_REG_FPSW, 3 << 11)
    cpu.reg_write(registers.UC_X86_REG_FP3, (0x8123456789abcdef, 0x4002))
    before = [cpu.reg_read(r) for r in (registers.UC_X86_REG_FPCW,
              registers.UC_X86_REG_FPSW, registers.UC_X86_REG_FPTAG, registers.UC_X86_REG_FP3)]
    observed = provider_broker._fp_snapshot(cpu)
    assert observed['control_word'] == 0x37f and observed['top'] == 3
    assert observed['st0_raw80'] == '40028123456789abcdef'
    after = [cpu.reg_read(r) for r in (registers.UC_X86_REG_FPCW,
             registers.UC_X86_REG_FPSW, registers.UC_X86_REG_FPTAG, registers.UC_X86_REG_FP3)]
    assert after == before


def test_compiler_failure_retains_source_and_stderr_for_provider_repair(tmp_path, monkeypatch):
    from types import SimpleNamespace
    from cmre.build import BuildError
    source = tmp_path / 'fn_00401000.c'
    source.write_text('invalid C source', encoding='utf-8')
    def compile_failure(*args):
        raise BuildError('compile failed', 'syntax error at line 1')
    modules = {'build': SimpleNamespace(build=compile_failure, BuildError=BuildError),
               'lift': SimpleNamespace(symbol=lambda va: '_cm_fn_00401000'),
               'env': SimpleNamespace()}
    backend = provider_broker.LocalBackend(function=0x401000)
    monkeypatch.setattr(backend, '_verify_target', lambda: 'pinned')
    monkeypatch.setattr(backend, '_module', lambda name: modules[name])
    output = tmp_path / 'build'
    with pytest.raises(BrokerError, match='syntax error at line 1'):
        backend.build(source, output)
    assert (output / 'O0' / source.name).read_bytes() == source.read_bytes()
    assert json.loads((output / 'O0/error.json').read_text())['stderr'] == 'syntax error at line 1'


def test_the_existing_probe_helpers_are_reused():
    provider = provider_broker._provider_probe()
    for name in ("validate_request", "apply_patch", "validate_global_writes"):
        assert hasattr(provider, name)


def test_the_response_is_strict_json(tmp_path):
    job = _make_job(tmp_path / "job")
    backend = FakeBackend()
    out = tmp_path / "job" / "dispatch-1"

    dispatch(job, {"kind": "evidence"}, out, backend=backend)
    text = (out / "response.json").read_text()
    assert "NaN" not in text and "Infinity" not in text
    json.loads(text, object_pairs_hook=_no_duplicate_pairs, parse_constant=_reject)


def _reject(name):
    raise AssertionError(f"non finite JSON constant {name!r}")
