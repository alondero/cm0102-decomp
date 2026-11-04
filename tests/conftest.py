import pytest


@pytest.fixture(autouse=True)
def hermetic_hints(tmp_path_factory, monkeypatch):
    """Tests never read the live recovery/hints folder (batches write auto hints there)."""
    from cmre import env

    monkeypatch.setattr(env, "HINTS_DIR", tmp_path_factory.mktemp("hints"))

pytest_plugins = ['cmre.workspace_audit']
