import pytest
from unicorn import UC_HOOK_MEM_WRITE
from unicorn.x86_const import UC_X86_REG_EIP


class Guest:
    def __init__(self):
        self.hooks = {}

    def hook_add(self, kind, record):
        assert kind == UC_HOOK_MEM_WRITE
        handle = object()
        self.hooks[handle] = record
        return handle

    def hook_del(self, handle):
        del self.hooks[handle]

    def reg_read(self, register):
        assert register == UC_X86_REG_EIP
        return self.ip

    def write(self, ip, address, size, value):
        self.ip = ip
        for observer in self.hooks.values():
            observer(self, None, address, size, value, None)


def test_page_trace_only_observes_chosen_original_case_and_final_recovered_attempt(monkeypatch):
    from cmre import emu, recover
    from scripts import cmre_page_writes

    class Machine:
        def __init__(self, recovered):
            self.recovered = recovered
            self.uc = Guest()

        def run(self, entry, case, budget):
            for params in case:
                self.uc.write(*params)
            return (entry, budget)

    before = Machine.run
    monkeypatch.setattr(emu, "Machine", Machine)
    va = 0x401000
    seen = []

    def explain(fn, case_index, overrides, *, source, profile, opt, window):
        assert (fn, case_index, source, opt) == (va, 1, None, "-O0")
        assert (overrides, profile, window) == ({"cases": 600, "budget": 300000}, "abi", 8)
        a, b = emu.Machine(None), emu.Machine(object())
        seen.extend((a, b))
        early = [(0x1000, 0x3000, 1, 0x11)]
        final = [(0x1011, 0x3004, 1, 0xff)]
        assert a.run(va, early, 10) == (va, 10)
        assert a.run(va, [(0x1007, 0x2ffe, 4, 0x11223344),
                          *final, (0x1018, 0x4000, 1, 0xaa)], 10) == (va, 10)
        assert b.run(0x6000, early, 10) == (0x6000, 10)
        assert b.run(0x6000, final, 200) == (0x6000, 200)
        return {"case": 1, "difference": {"field": "memory"},
                "original": {}, "recovered": {"budget_extended": True}}

    monkeypatch.setattr(recover, "explain_c", explain)
    result = cmre_page_writes.replay(va, 1, 0x3000, c=True, opt="-O0", limit=1,
                                     cases=600, budget=300000, profile="abi", window=8)
    assert result["difference"] == {"field": "memory"}
    trace = result["page_writes"]
    assert trace["page"] == "0x00003000"
    assert trace["original"] == {"observed": 2, "truncated": True, "writes": [
        {"ip": "0x00001007", "address": "0x00002ffe", "size": 4, "value": "0x11223344"},
    ]}
    assert trace["recovered"] == {"observed": 1, "truncated": False, "writes": [
        {"ip": "0x00001011", "address": "0x00003004", "size": 1, "value": "0xff"},
    ]}
    assert [len(machine.uc.hooks) for machine in seen] == [0, 0]
    assert Machine.run is before


def test_page_trace_restores_machine_method_and_removes_hook_on_error(monkeypatch):
    from cmre import emu, recover
    from scripts import cmre_page_writes

    uc = Guest()

    def run(*_args):
        raise RuntimeError("replay error")

    machine_type = type("Machine", (), {"run": run, "recovered": None, "uc": uc})
    monkeypatch.setattr(emu, "Machine", machine_type)

    def explain(*_args, **_kwargs):
        emu.Machine().run(0x401000, None, 10)

    monkeypatch.setattr(recover, "explain", explain)
    with pytest.raises(RuntimeError, match="replay error"):
        cmre_page_writes.replay(0x401000, 0, 0x3000)
    assert machine_type.run is run and not uc.hooks


def test_page_trace_enforces_explain_source_fence(monkeypatch, tmp_path):
    from contextlib import contextmanager

    from cmre import cli, recover
    from scripts import cmre_page_writes

    @contextmanager
    def refuse(va, source):
        assert va == 0x401000 and source == tmp_path
        raise ValueError("live claim")
        yield

    def no_build(*_args, **_kwargs):
        pytest.fail("build started despite refused explain source fence")

    monkeypatch.setattr(cli, "_diagnostic_source", refuse)
    monkeypatch.setattr(recover, "explain_c", no_build)
    with pytest.raises(ValueError, match="live claim"):
        cmre_page_writes.replay(0x401000, 0, 0x3000, c=True, source=tmp_path)
