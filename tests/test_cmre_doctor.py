from importlib import metadata

import capstone
import pytest

from cmre import cli, env, image, inventory, ledger


@pytest.fixture
def doctor_inputs(monkeypatch, tmp_path):
    target = tmp_path / "game.exe"
    target.write_bytes(b"target")
    monkeypatch.setattr(env, "TARGET", target)
    monkeypatch.setattr(env, "TARGET_SHA256", env.digest(target))
    monkeypatch.setattr(env, "INVENTORY_SHA256", {})
    monkeypatch.setattr(env, "rel", str)
    monkeypatch.setattr(env, "toolchain_identity", lambda: {"gcc": {"version": "test compiler"}})
    monkeypatch.setattr(image, "primary", lambda: None)
    monkeypatch.setattr(inventory, "load", lambda: None)
    monkeypatch.setattr(ledger, "verify", list)


def test_doctor_labels_installed_distribution_and_different_binding_version(doctor_inputs, monkeypatch, capsys):
    versions = {"capstone": "5.0.9", "unicorn": "2.1.4", "pefile": "2024.8.26"}
    monkeypatch.setattr(metadata, "version", versions.__getitem__)
    monkeypatch.setattr(capstone, "__version__", "5.0.7")

    assert cli.cmd_doctor(None) == 0
    output = capsys.readouterr().out
    assert "capstone 5.0.9 (module declares 5.0.7)" in output
    assert "pefile 2024.8.26" in output


def test_doctor_fails_if_a_loaded_dependency_has_no_distribution_identity(doctor_inputs, monkeypatch, capsys):
    def version(name):
        if name == "capstone":
            raise metadata.PackageNotFoundError(name)
        return "1.2.3"

    monkeypatch.setattr(metadata, "version", version)
    assert cli.cmd_doctor(None) == 1
    assert "BAD packages:" in capsys.readouterr().out
