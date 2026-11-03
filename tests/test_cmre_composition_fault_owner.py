"""Isolated composition-only strict fault identity tests."""

import copy, json, random, tempfile, unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
import unicorn
from cmre import build, composition, difftest, emu, env, image, lift

ce = emu
cd = difftest
cc = composition


class OwnerStrict(unittest.TestCase):
    def result(self, pc, access=20, address=99):
        return emu.Result(
            "fault",
            ((access, address, 1), pc),
            (0, 0, 0, 0, 0x203F000, 0, 0, 0, 0x202),
            [],
            {},
            set(),
        )

    def metadata(self, labels):
        ins = SimpleNamespace(kind="plain", raw=b"\x90", size=1)
        bodies = {
            0x43F460: SimpleNamespace(insns={0x43F4AD: ins}),
            0x441C10: SimpleNamespace(insns={0x441C3C: ins}),
        }
        built = SimpleNamespace(
            symbols=labels,
            image=SimpleNamespace(
                text=SimpleNamespace(va=0x6E5D1000, end=0x6E5D2000),
                read=lambda address, size: b"\x90",
            ),
        )
        entries = {0x43F460: 0x6E5D1000, 0x441C10: 0x6E5D1080}
        return bodies, built, entries

    def test_owner_exact_and_unknown(self):
        a, b, e = self.metadata({"v_0043f4ad": 0x6E5D1066, "v_00441c3c": 0x6E5D114A})
        verified = cc._verified_fault_locations(a, b, e)
        m = ce.AddressMap(
            b.symbols, {}, 0x6E5D1000, 0x6E5D2000, verified_fault_locations=verified
        )
        self.assertEqual(m.fault_location(0x6E5D114A), 0x441C3C)
        self.assertEqual(m.fault_location(0x6E5D114B), 0x6E5D114B)

    def test_wrong_owner_duplicate_unknown_and_overlap_rejected(self):
        for labels in (
            {"v_00441c3c": 0x6E5D1066},
            {"v_0043f4ad": 0x6E5D1066, "v_00441c3c": 0x6E5D1066},
            {"v_00499999": 0x6E5D114A},
        ):
            a, b, e = self.metadata(labels)
            with self.assertRaises(ValueError):
                cc._verified_fault_locations(a, b, e)
        a, b, e = self.metadata({"v_0043f4ad": 0x6E5D1066})
        a[0x441C10].insns[0x43F4AD] = object()
        with self.assertRaises(ValueError):
            cc._verified_fault_locations(a, b, e)

    def test_instruction_span_crossing_owner_or_text_is_rejected(self):
        for label, address, owner, size in (
            ("v_0043f4ad", 0x6E5D107F, 0x43F460, 2),
            ("v_00441c3c", 0x6E5D1FFF, 0x441C10, 2),
        ):
            a, b, e = self.metadata({label: address})
            ins = a[owner].insns[int(label[2:], 16)]
            ins.size = size
            ins.raw = b"\x90" * size
            b.image.read = lambda address, size: b"\x90" * size
            with (
                self.subTest(label=label),
                self.assertRaisesRegex(ValueError, "span crosses"),
            ):
                cc._verified_fault_locations(a, b, e)

    def test_instruction_span_exactly_at_owner_end_is_allowed(self):
        a, b, e = self.metadata({"v_0043f4ad": 0x6E5D107E})
        ins = a[0x43F460].insns[0x43F4AD]
        ins.size = 2
        ins.raw = b"\x90" * 2
        b.image.read = lambda address, size: b"\x90" * size
        self.assertEqual(cc._verified_fault_locations(a, b, e), {0x6E5D107E: 0x43F4AD})

    def test_owner_label_needs_matching_ordinary_instruction_bytes(self):
        a, b, e = self.metadata({"v_00441c3c": 0x6E5D114A})
        b.image.read = lambda address, size: b"\xcc"
        self.assertEqual(cc._verified_fault_locations(a, b, e), {})
        b.image.read = lambda address, size: b"\x90"
        a[0x441C10].insns[0x441C3C].kind = "call"
        self.assertEqual(cc._verified_fault_locations(a, b, e), {})

    def test_strict_data_fault_does_not_accept_nearest_label_false_parity(self):
        m = ce.AddressMap(
            {"v_0043f4ad": 0x6E5D1066},
            {},
            0x6E5D1000,
            0x6E5D2000,
            verified_fault_locations={0x6E5D1066: 0x43F4AD},
        )
        a = cd._normalize(self.result(0x441C3C, 20, 0x43F591), None, "abi")
        b = cd._normalize(self.result(0x441C3C, 20, 0x6E5D114A), m, "abi")
        self.assertEqual(cd._diff(a, b, m), {})
        self.assertEqual(cd._diff(a, b, m, strict_data_fault=True)["field"], "detail")

    def test_same_data_address_and_near_misses(self):
        m = ce.AddressMap(
            {"v_0043f4ad": 0x6E5D1066},
            {},
            0x6E5D1000,
            0x6E5D2000,
            verified_fault_locations={0x6E5D1066: 0x43F4AD},
        )
        a = cd._normalize(self.result(0x441C3C, 20, 0x63), None, "abi")
        same = cd._normalize(self.result(0x441C3C, 20, 0x63), m, "abi")
        self.assertEqual(cd._diff(a, same, m, strict_data_fault=True), {})
        for other in (0x62, 0x64):
            b = cd._normalize(self.result(0x441C3C, 20, other), m, "abi")
            self.assertEqual(
                cd._diff(a, b, m, strict_data_fault=True)["field"], "detail"
            )

    def test_unknown_execute_address_and_pc_unchanged(self):
        m = ce.AddressMap(
            {"v_0043f4ad": 0x6E5D1066},
            {},
            0x6E5D1000,
            0x6E5D2000,
            verified_fault_locations={0x6E5D1066: 0x43F4AD},
        )
        b = cd._normalize(self.result(0x6E5D114A, 21, 0x6E5D1130), m, "abi")
        self.assertEqual(b[1], ((21, 0x6E5D1130, 1), 0x6E5D114A))

    def test_legacy_noncomposition_and_semantic_unchanged(self):
        m = ce.AddressMap({"v_0043f4ad": 0x6E5D1066}, {}, 0x6E5D1000, 0x6E5D2000)
        self.assertEqual(m.fault_location(0x6E5D114A), 0x43F591)
        self.assertEqual(
            cd._normalize(self.result(0x6E5D114A), m, "semantic")[1], (20, 99, 1)
        )

    def test_real_mapping_proof_and_address_size_near_misses(self):
        va = 0x768290
        with tempfile.TemporaryDirectory() as d:
            body, text, b = build.lift_and_build(va, Path(d))
            self.assertIsNotNone(b)
            entries = {va: b.entry}
            labels = cc._verified_fault_locations({va: body}, b, entries)
            m = ce.AddressMap(
                b.symbols,
                {},
                b.image.text.va,
                b.image.text.end,
                verified_fault_locations=labels,
            )

            def case(pointer):
                return emu.Case(
                    {"ecx": pointer, "eflags": 0x202}, [0] * 10, bytes(emu.ARENA_SIZE)
                )

            a = emu.Machine(image.primary()).run(va, case(b.entry), 20)
            z = emu.Machine(image.primary(), b.image).run(b.entry, case(b.entry), 20)
            self.assertEqual(a.detail[0], (unicorn.UC_MEM_WRITE_UNMAPPED, b.entry, 4))
            self.assertEqual(z.detail[0], (unicorn.UC_MEM_WRITE_PROT, b.entry, 4))
            self.assertIsNotNone(z.fault_mapping)
            an = cd._normalize(a, None, "abi")
            zn = cd._normalize(z, m, "abi")
            self.assertEqual(
                cd._diff(
                    an, zn, m, fault_mapping=z.fault_mapping, strict_data_fault=True
                ),
                {},
            )
            for field in ("address", "size"):
                bad = copy.deepcopy(z.fault_mapping)
                bad[field] += 1
                with self.subTest(field=field):
                    self.assertEqual(
                        cd._diff(an, zn, m, fault_mapping=bad, strict_data_fault=True)[
                            "field"
                        ],
                        "detail",
                    )
            self.assertEqual(
                cd._diff(an, zn, m, strict_data_fault=True)["field"], "detail"
            )
            # A true relocated code pointer has different raw data targets; this strict
            # composition policy rejects it rather than inferring pointer equivalence.
            native = emu.Machine(image.primary()).run(va, case(va), 20)
            x = cd._normalize(native, None, "abi")
            self.assertEqual(
                cd._diff(
                    x, zn, m, fault_mapping=z.fault_mapping, strict_data_fault=True
                )["field"],
                "detail",
            )

    def test_actual_composition_replay_records_unknown_pc_mismatch(self):
        vas = [0x43F460, 0x441C10, 0x450AA0]
        # Own the hint directory even under unittest, which does not run the
        # pytest autouse fixture that isolates the live recovery hints.
        hint_dir = tempfile.TemporaryDirectory()
        self.addCleanup(hint_dir.cleanup)
        hint_patch = patch.object(env, "HINTS_DIR", Path(hint_dir.name))
        hint_patch.start()
        self.addCleanup(hint_patch.stop)
        hint = {
            "original_x87": {
                "0x0043f460": {"bytes": "db01", "evidence": "pinned FILD"},
                "0x0043f464": {"bytes": "dc0db89cad00", "evidence": "pinned FMUL"},
            }
        }
        (env.HINTS_DIR / "fn_0043f460.json").write_text(json.dumps(hint), encoding="utf-8")
        sources = [env.C_DIR / f"fn_{v:08x}.c" for v in vas]
        expected = {
            "fn_0043f460.c": "cbd1e11a16836378319816fe12c34d7dc4e9ddc7e3a357cc921022b947534f18",
            "fn_00441c10.c": "4001480c297d6ca5d6a8eb6c6545364a57ef7b13e2b215b14d5523e3c2e13967",
            "fn_00450aa0.c": "0dfe282405cbae69448dd77a2ddc70af21ac1cbf6975ad8bdb21081d0bb4bce8",
        }
        self.assertEqual({p.name: env.digest(p) for p in sources}, expected)
        self.assertEqual(env.digest(env.TARGET), env.TARGET_SHA256)
        bodies = {v: lift.discover(v) for v in vas}
        opts = composition.composition_c_options(0x450AA0)
        rng = random.Random(opts.seed)
        templates = difftest.arena_templates()
        inputs = set()
        generator = emu.Machine(
            image.primary(), watch=(0x450AA0, bodies[0x450AA0].region_end)
        )
        for index in range(11):
            case = difftest.make_case(rng, opts, sorted(inputs), templates)
            own = difftest._discovery(
                generator, opts, index, (0x450AA0, bodies[0x450AA0].region_end)
            )
            result = generator.run(0x450AA0, case, opts.budget)
            if index < opts.discovery:
                inputs |= difftest._global_reads(result, image.primary(), own)
        self.assertEqual(result.outcome, "fault")
        self.assertEqual(result.detail, ((20, 99, 1), 0x441C3C))
        opts.cases = 1
        opts.discovery = 0
        with tempfile.TemporaryDirectory() as d:
            b = composition._build_separate(
                sources, lift.symbol(0x450AA0), Path(d), "-O0"
            )
            entries = composition._symbols(b, vas)
            checked = cc._verified_fault_locations(bodies, b, entries)
            self.assertEqual(checked[b.symbols["v_00450aa0"]], 0x450AA0)
            with patch.object(cd, "make_case", return_value=case):
                r = cc._run(0x450AA0, bodies, b, entries, opts)
            self.assertEqual(r["status"], "fail")
            self.assertEqual(r["mismatches"][0]["field"], "detail")
            self.assertIn("4463676", r["mismatches"][0]["original"])
            self.assertNotIn("4453777", r["mismatches"][0]["recovered"])


if __name__ == "__main__":
    unittest.main()
