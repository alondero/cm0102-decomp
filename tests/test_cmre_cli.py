"""Plain `cmre` report output: enough to act on without a `--json` rerun."""
from cmre import cli
import pytest


@pytest.mark.parametrize("claim", [False, True])
@pytest.mark.parametrize("include_unlifted", [False, True])
@pytest.mark.parametrize("no_static_callees", [False, True])
def test_next_routes_unlifted_opt_in(monkeypatch, claim, include_unlifted, no_static_callees):
    from cmre import queue

    calls = []
    monkeypatch.setattr(queue, "claim_next", lambda *a, **kw: calls.append((a, kw)) or [])
    monkeypatch.setattr(queue, "c_frontier", lambda *a, **kw: calls.append((a, kw)) or [])
    args = ["next", "--c", "-n", "2", "--min-bytes", "129", "--max-bytes", "600",
            "--module", "util", "--shard", "1/2"]
    if claim:
        args += ["--claim", "coordinator"]
    if include_unlifted:
        args += ["--include-unlifted"]
    if no_static_callees:
        args += ["--no-static-callees"]
    assert cli.main(args) == 0
    assert len(calls) == 1
    positional, filters = calls[0]
    assert positional == ((2, "coordinator", None) if claim else (2,))
    assert filters["include_unlifted"] is include_unlifted
    assert filters["no_static_callees"] is no_static_callees
    assert (filters["min_bytes"], filters["max_bytes"], filters["module"], filters["shard"]) == (
        129, 600, "util", (1, 2))


@pytest.mark.parametrize("extra", [[], ["--blocked"]])
def test_next_rejects_include_unlifted_without_c(monkeypatch, capsys, extra):
    from cmre import ledger, queue

    monkeypatch.setattr(queue, "frontier", lambda *a, **kw: pytest.fail("selection before validation"))
    monkeypatch.setattr(ledger, "blocked", lambda: pytest.fail("blocked read before validation"))
    assert cli.main(["next", "--include-unlifted", *extra]) == 2
    assert "--include-unlifted requires --c" in capsys.readouterr().err


@pytest.mark.parametrize("extra", [[], ["--blocked"], ["--claim", "coordinator"], ["--shard", "1/2"]])
def test_next_rejects_no_static_callees_without_c_before_reads(monkeypatch, capsys, extra):
    from cmre import inventory, image, ledger, queue, work

    def no_read(*args, **kwargs):
        pytest.fail("file access or selection before validation")

    for module, names in ((inventory, ("load",)), (image, ("primary",)),
                          (ledger, ("blocked", "cmre_entries", "legacy_entries")),
                          (work, ("read", "claims")), (queue, ("frontier", "c_frontier", "claim_next"))):
        for name in names:
            monkeypatch.setattr(module, name, no_read)
    assert cli.main(["next", "--no-static-callees", *extra]) == 2
    assert "--no-static-callees requires --c" in capsys.readouterr().err


def test_next_blocked_retains_precedence_over_c_filters(monkeypatch, capsys):
    from cmre import ledger, queue

    monkeypatch.setattr(ledger, "blocked", lambda: {0x401000: {"reason": "held"}})
    monkeypatch.setattr(queue, "c_frontier", lambda *a, **kw: pytest.fail("blocked must take precedence"))
    monkeypatch.setattr(queue, "claim_next", lambda *a, **kw: pytest.fail("blocked must take precedence"))
    assert cli.main(["next", "--c", "--blocked", "--no-static-callees", "--claim", "coordinator"]) == 0
    assert "fn_00401000  held" in capsys.readouterr().out


def test_build_failure_prints_compiler_errors_once(capsys):
    stderr = ("fn.c: In function 'cm_fn_007862d0':\n"
              "fn.c:46:17: error: too many arguments to function 'cm_fn_0090d130'; expected 0, have 3\n"
              "   46 |     line = (u32)cm_fn_0090d130(a, b, line);\n")
    builds = {opt: {"status": "build-failed", "detail": "compile failed: fn.c", "stderr": stderr}
              for opt in ("-O0", "-O2")}
    cli._print_report({"va": "0x007862d0", "status": "fail", "builds": builds}, False)
    out = capsys.readouterr().out
    assert out.count("error: too many arguments") == 1
    assert "46 |" in out


def test_per_build_mismatches_are_printed_once(capsys):
    difftest = {"coverage": 0.9, "inconclusive": 0, "mismatches": [{"case": 20, "field": "memory", "pages": ["0xae2000"]}]}
    builds = {opt: {"status": "fail", "difftest": difftest} for opt in ("-O0", "-O2")}
    cli._print_report({"va": "0x007862d0", "status": "fail", "builds": builds}, False)
    out = capsys.readouterr().out
    assert out.count('"case": 20') == 1 and "explain" in out
    assert "--case 20 --c --opt=-O0" in out


def test_disasm_exposes_unowned_call_site_without_inventory(monkeypatch, capsys):
    from cmre import env, inventory

    def no_inventory():
        raise AssertionError("raw instruction windows must not require a function owner")

    monkeypatch.setattr(inventory, "load", no_inventory)
    assert cli.main(["disasm", "0x0071a6f4", "--count", "3"]) == 0
    out = capsys.readouterr().out
    assert env.TARGET_SHA256 in out and "base 0x00400000" in out
    assert "0071a6f4" in out and "call 0x800d30" in out
    assert "0071a6f9" in out


def test_disasm_stops_at_executable_section_end(monkeypatch, capsys):
    from cmre import image

    img = object.__new__(image.Image)
    img.base = 0x00400000
    img.sections = [image.Section(".text", 0x00401000, 0x1000, b"\x90\xc3", 0x60000020)]
    monkeypatch.setattr(image, "primary", lambda: img)
    assert cli.main(["disasm", "0x00401000", "--count", "8"]) == 0
    out = capsys.readouterr().out
    assert "00401000  90  nop" in out
    assert "00401001  c3  ret" in out
    assert "00401002" not in out


@pytest.mark.parametrize("count", ["0", "257"])
def test_disasm_rejects_unbounded_instruction_counts(count, capsys):
    assert cli.main(["disasm", "0x0071a6f4", "--count", count]) == 2
    assert "1..256" in capsys.readouterr().err


def test_disasm_follows_original_vtable_slot_without_function_inventory(monkeypatch, capsys):
    from cmre import image

    img = object.__new__(image.Image)
    img.base = 0x00400000
    img.sections = [image.Section(".text", 0x00401000, 1, b"\xc3", 0x60000020),
                    image.Section(".rdata", 0x00402000, 4, b"\x00\x10\x40\x00", 0x40000040)]
    monkeypatch.setattr(image, "primary", lambda: img)
    assert cli.main(["disasm", "0x00402000", "--indirect", "--count", "1"]) == 0
    out = capsys.readouterr().out
    assert "0x00402000 -> 0x00401000" in out
    assert "00401000  c3  ret" in out


@pytest.mark.parametrize("address", ["00402000", "00403000"])
def test_disasm_rejects_non_code_addresses(monkeypatch, capsys, address):
    from cmre import image

    img = object.__new__(image.Image)
    img.base = 0x00400000
    img.sections = [image.Section(".data", 0x00402000, 4, b"\0" * 4, 0xc0000040)]
    monkeypatch.setattr(image, "primary", lambda: img)
    assert cli.main(["disasm", address]) == 2
    assert "not in an executable section" in capsys.readouterr().err
