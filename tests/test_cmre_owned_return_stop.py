"""Original-backed owned-return STOP regression tests."""

import struct, tempfile, unittest
from pathlib import Path
from unittest.mock import patch
import unicorn
from unicorn import x86_const as R
from cmre import build, composition, difftest, emu, env, image, lift, return_transport

ce = emu
VA = 0x524D50
# Instruction-derived adapter fixture; original callee remains a game boundary.
SOURCE_TEXT = r"""#include "cmre.h"
void CM_CDECL __attribute__((naked)) cm_fn_00524d50(void) {
 __asm__("v_00524d50:\n"
 "movl 8(%esp), %eax\n movl 4(%esp), %ecx\n pushl $0\n pushl %eax\n pushl %ecx\n"
 CM_ORIGINAL_CALL(00524d5c,00524d61,00528540)
 "addl $12, %esp\n v_00524d64:\n ret\n");
}
"""


def case(fault=False):
    arena = bytearray(emu.ARENA_SIZE)
    arena[0x100:0x105] = b"test\0"
    arena[0x200:0x205] = b"path\0"
    return emu.Case(
        {"eax": 0x55667788, "ecx": 0x12345678, "eflags": 0x202},
        [emu.ARENA + 0x100, emu.ARENA + 0x200] + [0] * 8,
        bytes(arena),
        {
            0xB77C4E: emu.ARENA + 0x1000,
            0xB77C52: 0,
            0xAE3138: 0 if fault else emu.ARENA + 0x2000,
        },
    )


class Guard(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if env.digest(env.TARGET) != env.TARGET_SHA256:
            raise ValueError("original target identity differs")
        cls.tmp = tempfile.TemporaryDirectory()
        root = Path(cls.tmp.name)
        cls.built = {}
        source = root / "fn_00524d50.c"
        source.write_text(SOURCE_TEXT, encoding="utf8")
        for opt in ("-O0", "-O2"):
            d = root / opt[1:]
            d.mkdir()
            cls.built[opt] = build.build(source, lift.symbol(VA), d, opt)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def machine(self, b):
        return ce.Machine(image.primary(), b.image)

    def test_owned_stop_escape_at_both_optimizations(self):
        for opt, b in self.built.items():
            m = self.machine(b)
            hits = []

            def mutate(uc, pc, size, user):
                sp = uc.reg_read(R.UC_X86_REG_ESP)
                uc.mem_write(sp, struct.pack("<I", emu.STOP))
                hits.append(sp)

            m.uc.hook_add(unicorn.UC_HOOK_CODE, mutate, begin=0x528819, end=0x528819)
            r = m.run(b.entry, case(), 300000)
            self.assertEqual((r.outcome, r.detail), ("escape", (emu.STOP,)), opt)
            self.assertTrue(hits)
            self.assertEqual(len(m.return_runtime.frames), 1)

    def test_valid_nested_return_still_returns(self):
        for opt, b in self.built.items():
            m = self.machine(b)
            r = m.run(b.entry, case(), 300000)
            self.assertEqual(r.outcome, "return", opt)
            self.assertFalse(m.return_runtime.frames)
            self.assertIsNone(m.return_runtime.pending)

    def test_fault_with_active_frame_remains_fault(self):
        for opt, b in self.built.items():
            m = self.machine(b)
            r = m.run(b.entry, case(True), 300000)
            self.assertEqual(r.outcome, "fault", opt)
            self.assertEqual(r.detail[1], 0x5286E0)
            self.assertTrue(m.return_runtime.frames)

    def test_real_pending_transfer_to_stop_is_escape(self):
        for opt, b in self.built.items():
            m = self.machine(b)
            seen = []
            pc = b.image.return_bindings[0].compiled_push + 5

            def redirect(uc, address, size, user):
                pending = m.return_runtime.pending
                seen.append(
                    (pending.phase, pending.target, len(m.return_runtime.frames))
                )
                uc.reg_write(R.UC_X86_REG_EIP, emu.STOP)

            m.uc.hook_add(unicorn.UC_HOOK_CODE, redirect, begin=pc, end=pc)
            r = m.run(b.entry, case(), 300000)
            self.assertEqual(r.outcome, "escape", opt)
            self.assertEqual(r.detail, (emu.STOP,))
            self.assertEqual(seen, [("callee", 0x528540, 0)])
            self.assertFalse(m.return_runtime.frames)

    def test_real_pending_transfer_fault_stays_fault(self):
        for opt, b in self.built.items():
            m = self.machine(b)
            seen = []
            pc = b.image.return_bindings[0].compiled_push + 5

            def redirect(uc, address, size, user):
                pending = m.return_runtime.pending
                seen.append((pending.phase, pending.target))
                uc.reg_write(R.UC_X86_REG_EIP, 0x1000)

            m.uc.hook_add(unicorn.UC_HOOK_CODE, redirect, begin=pc, end=pc)
            r = m.run(b.entry, case(), 300000)
            self.assertEqual(r.outcome, "fault", opt)
            self.assertEqual(seen, [("callee", 0x528540)])
            self.assertIsNotNone(m.return_runtime.pending)

    def test_escape_propagates_as_differential_mismatch(self):
        for opt, b in self.built.items():
            orig = ce.Machine(image.primary())
            compiled = self.machine(b)

            def mutate(uc, pc, size, user):
                uc.mem_write(uc.reg_read(R.UC_X86_REG_ESP), struct.pack("<I", emu.STOP))

            orig.uc.hook_add(unicorn.UC_HOOK_CODE, mutate, begin=0x528819, end=0x528819)
            compiled.uc.hook_add(
                unicorn.UC_HOOK_CODE, mutate, begin=0x528819, end=0x528819
            )
            a = orig.run(VA, case(), 300000)
            z = compiled.run(b.entry, case(), 300000)
            amap = emu.AddressMap(b.symbols, {}, b.image.text.va, b.image.text.end)
            verdict, diff = difftest._classify(
                difftest._normalize(a, None, "semantic"),
                difftest._normalize(z, amap, "semantic"),
                a,
                amap,
                (VA, VA + 21),
                [],
            )
            self.assertEqual(
                (a.outcome, z.outcome, verdict), ("return", "escape", "mismatch"), opt
            )
            self.assertTrue(diff)

    def test_escape_is_recorded_in_full_differential_report(self):
        body = lift.discover(VA)
        Machine = emu.Machine
        for opt, b in self.built.items():

            def factory(*args, **kwargs):
                m = Machine(*args, **kwargs)

                def mutate(uc, pc, size, user):
                    uc.mem_write(
                        uc.reg_read(R.UC_X86_REG_ESP), struct.pack("<I", emu.STOP)
                    )

                m.uc.hook_add(
                    unicorn.UC_HOOK_CODE, mutate, begin=0x528819, end=0x528819
                )
                return m

            opts = difftest.Options.for_function(
                VA, {"cases": 1, "budget": 300000}, tier="c"
            )
            opts.discovery = 0
            with (
                patch.object(difftest.emu, "Machine", side_effect=factory),
                patch.object(difftest, "make_case", return_value=case()),
            ):
                report = difftest.run(
                    VA,
                    (body.va, body.region_end),
                    set(body.insns),
                    b,
                    {},
                    opts,
                    "semantic",
                )
            self.assertEqual(report["outcomes"].get("return"), 1, opt)
            self.assertEqual(report["inconclusive"], 0)
            self.assertEqual(len(report["mismatches"]), 1)
            self.assertEqual(report["mismatches"][0]["case"], 0)

    def test_owned_stop_reaches_composition_as_mismatch(self):
        body = lift.discover(VA)
        Machine = emu.Machine
        for opt, b in self.built.items():
            opts = composition.composition_c_options(VA, {"cases": 1, "budget": 300000})
            opts.discovery = 0

            def factory(*args, **kwargs):
                m = Machine(*args, **kwargs)

                def mutate(uc, pc, size, user):
                    uc.mem_write(
                        uc.reg_read(R.UC_X86_REG_ESP), struct.pack("<I", emu.STOP)
                    )

                m.uc.hook_add(
                    unicorn.UC_HOOK_CODE, mutate, begin=0x528819, end=0x528819
                )
                return m

            with (
                patch.object(composition.emu, "Machine", side_effect=factory),
                patch.object(difftest, "make_case", return_value=case()),
            ):
                report = composition._run(VA, {VA: body}, b, {VA: b.entry}, opts)
            self.assertEqual(report["status"], "fail", opt)
            self.assertEqual(report["mismatches"][0]["field"], "outcome")
            self.assertEqual(report["mismatches"][0]["recovered"], "escape")

    def test_valid_lifted_direct_transport_still_returns(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            source = root / "lifted.S"
            source.write_text(
                """ .intel_syntax noprefix
.text
.globl _cm_fn_005230c0
_cm_fn_005230c0:
v_005230c0:
 push esi
 mov esi, ecx
v_005230c3:
 call _cm_fn_00523300
v_005230c8:
 mov eax, dword ptr [esp-4]
 pop esi
 ret 4
.globl _cm_fn_00523300
_cm_fn_00523300:
v_00523300:
 ret
""",
                encoding="utf8",
            )
            b = build.build(source, lift.symbol(0x5230C0), root)
            entries = {v: b.symbols[lift.symbol(v)] for v in (0x5230C0, 0x523300)}
            b.image.return_bindings = return_transport.bind_lifted(
                b.image, b.symbols, entries, (0x5230C3,)
            )
            m = ce.Machine(image.primary(), b.image)
            r = m.run(
                b.entry,
                emu.Case(
                    {"ecx": 0x3010E58, "eflags": 0x602}, [0] * 10, bytes(emu.ARENA_SIZE)
                ),
                1000,
            )
            self.assertEqual(r.outcome, "return")
            self.assertEqual(m.return_runtime.dispatches, 1)
            self.assertFalse(m.return_runtime.frames)
            self.assertIsNone(m.return_runtime.pending)

    def test_valid_indirect_transport_still_returns(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            source = root / "macro.c"
            source.write_text(
                '#include "cmre.h"\nvoid __attribute__((naked)) cm_fn_007e6150(void) { __asm__("movl $0x03000400, %ecx\\n movl $0x0203eff8, %eax\\n pushl $1\\n" CM_ORIGINAL_INDIRECT_CALL(007e6189,007e618b) "ret\\n"); }\n',
                encoding="utf8",
            )
            b = build.build(source, "cm_fn_007e6150", root)
            arena = bytearray(emu.ARENA_SIZE)
            struct.pack_into("<I", arena, 0x409, 0x12345678)
            m = ce.Machine(image.primary(), b.image)
            r = m.run(
                b.entry,
                emu.Case({}, [], bytes(arena), {emu.STACK_TOP - 8: 0x785830}),
                1000,
            )
            self.assertEqual(r.outcome, "return")
            self.assertEqual(r.regs[0], 0x12345678)
            self.assertEqual(m.return_runtime.dispatches, 1)
            self.assertFalse(m.return_runtime.frames)
            self.assertIsNone(m.return_runtime.pending)

    def test_untransported_original_still_returns(self):
        m = ce.Machine(image.primary())
        r = m.run(VA, case(), 300000)
        self.assertEqual(r.outcome, "return")
        self.assertIsNone(m.return_runtime)


if __name__ == "__main__":
    unittest.main()
