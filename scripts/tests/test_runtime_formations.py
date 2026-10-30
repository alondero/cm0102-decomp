"""Filesystem safety and both supported formation layouts; no game execution."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import repair_runtime_formations as repair


class FormationRepairTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / "CM0102/Game"
        self.runtime = self.root / "CM0102/Runtime/copy"
        self.runtime.mkdir(parents=True)
        (self.runtime / "Data").mkdir()
        (self.root / "CM0102/Inspection").mkdir()
        for name in ["cm0102.exe", "cm0102_GDI.exe"]:
            (self.runtime / name).write_bytes(b"test executable")
        sha = hashlib.sha256(b"test executable").hexdigest()
        self.patches = patch.multiple(repair, ROOT=self.root, SOURCE=self.source,
                                     SHA256=sha, GDI_SHA256=sha)
        self.patches.start()
        self.addCleanup(self.patches.stop)

    def layout(self, directory):
        rows = []
        (self.source / directory).mkdir(parents=True)
        for index in range(40):
            name = f"formation-{index:02}.pct"
            data = f"original formation {index}".encode()
            (self.source / directory / name).write_bytes(data)
            if directory == "Data":
                (self.runtime / "Data" / name).write_bytes(data)
            rows.append(dict(path=f"{directory}/{name}", sha256=hashlib.sha256(data).hexdigest()))
        self.manifest = self.root / "CM0102/Inspection/game-manifest.json"
        self.manifest.write_text(json.dumps(rows))

    def run_repair(self):
        with contextlib.redirect_stdout(io.StringIO()):
            repair.repair(self.runtime)

    def test_historical_layout_is_additive_and_idempotent(self):
        self.layout("Tactics")
        original_manifest = self.manifest.read_bytes()
        self.run_repair()
        evidence = (self.runtime / "formation-layout-repair.json").read_bytes()
        self.run_repair()
        self.assertEqual(evidence, (self.runtime / "formation-layout-repair.json").read_bytes())
        self.assertEqual(original_manifest, self.manifest.read_bytes())
        self.assertEqual(len(list((self.source / "Tactics").glob("*.pct"))), 40)
        self.assertEqual(len(list((self.runtime / "Data").glob("*.pct"))), 40)

    def test_corrected_layout_is_a_recorded_noop(self):
        self.layout("Data")
        self.run_repair()
        record = json.loads((self.runtime / "formation-layout-repair.json").read_text())
        self.assertTrue(all(row["already_present"] for row in record["files"]))

    def test_conflicting_destination_refuses_before_any_copy(self):
        self.layout("Tactics")
        conflict = self.runtime / "Data/formation-39.pct"
        conflict.write_bytes(b"different")
        with self.assertRaisesRegex(ValueError, "Refusing to overwrite"):
            self.run_repair()
        self.assertEqual(list((self.runtime / "Data").iterdir()), [conflict])
        self.assertEqual(conflict.read_bytes(), b"different")

    def test_bad_source_refuses_before_any_copy(self):
        self.layout("Tactics")
        (self.source / "Tactics/formation-39.pct").write_bytes(b"changed")
        with self.assertRaisesRegex(ValueError, "Preserved asset mismatch"):
            self.run_repair()
        self.assertEqual(list((self.runtime / "Data").iterdir()), [])

    def test_preserved_destination_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "beneath CM0102/Runtime"):
            repair.repair(self.source)


if __name__ == "__main__":
    unittest.main()
