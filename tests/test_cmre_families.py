import capstone

from cmre import families, lift


def instructions(raw, base=0x401000):
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    result = []
    for i in decoder.disasm(bytes.fromhex(raw), base):
        branch = i.mnemonic.startswith("j")
        target = i.operands[0].imm if branch else None
        result.append(lift.Insn(i.address, bytes(i.bytes), i.mnemonic, i.op_str,
                                target=target, kind="jcc" if branch else "plain"))
    return result


def test_family_offsets_are_deltas_not_evidence_of_equivalence():
    a, lines_a = families.describe(instructions("8b 41 10 c3"))
    b, lines_b = families.describe(instructions("8b 41 20 c3", 0x402000))
    assert a == b
    assert lines_a != lines_b
    assert "0x10" in lines_a[0] and "0x20" in lines_b[0]


def test_family_retains_operand_width_register_and_branch_condition():
    base, _ = families.describe(instructions("8b 41 10 c3"))
    for raw in ("0f b6 41 10 c3", "8b 42 10 c3", "66 8b 41 10 c3"):
        assert families.describe(instructions(raw))[0] != base
    assert families.describe(instructions("74 00 c3"))[0] != families.describe(instructions("75 00 c3"))[0]


def test_internal_branch_topology_survives_address_normalization():
    left = families.describe(instructions("90 75 fd c3"))
    relocated = families.describe(instructions("90 75 fd c3", 0x402000))
    different_target = families.describe(instructions("90 75 00 c3"))
    assert left == relocated
    assert left[0] != different_target[0]
