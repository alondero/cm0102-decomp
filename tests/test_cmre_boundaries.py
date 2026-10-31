"""Inventory boundary reconciliation and metadata-proven compiler-generated parents."""
from types import SimpleNamespace

import pytest

from cmre import boundaries, generated, image


@pytest.mark.parametrize("raw", ["90", "cc", "8bff", "8d4900", "8da42400000000", "8bff8d4900cccc"])
def test_alignment_fill_is_recognized(raw):
    assert boundaries.is_fill(bytes.fromhex(raw))


@pytest.mark.parametrize("raw", ["c3", "8bff c3", "8d49", "00", "ffff"])
def test_other_bytes_are_not_fill(raw):
    assert not boundaries.is_fill(bytes.fromhex(raw.replace(" ", "")))


def _lifted(insns, tables=(), data=()):
    return SimpleNamespace(
        insns={va: SimpleNamespace(va=va, size=size, substitutions=subs) for va, size, subs in insns},
        tables={va: SimpleNamespace(va=va, targets=[0] * n) for va, n in tables},
        data=[SimpleNamespace(va=va, raw=bytes.fromhex(raw)) for va, raw in data])


def test_extent_disposition_explains_tables_fill_and_referenced_data():
    entry = SimpleNamespace(va=0x100, size=0x20)
    fn = SimpleNamespace(body_bytes=0x14)
    lifted = _lifted([(0x100, 0x10, {0x118: "v"}), (0x11C, 4, {})], tables=[(0x110, 1)],
                     data=[(0x114, "8bff9090"), (0x118, "01020304")])
    disposition, open_, detail = boundaries._extent_disposition(None, entry, fn, lifted)
    assert not open_, detail
    assert disposition == "extent = body + tables + fill + referenced_data"
    assert (detail["tables"], detail["fill"], detail["referenced_data"]) == (4, 4, 4)


def test_unreferenced_non_fill_inside_extent_stays_open(fake_references):
    entry = SimpleNamespace(va=0x100, size=0x14)
    lifted = _lifted([(0x100, 0x10, {}), (0x113, 1, {})], data=[(0x110, "010203")])
    disposition, open_, detail = boundaries._extent_disposition(
        _image({}), entry, SimpleNamespace(body_bytes=0x11), lifted)
    assert open_ and detail["unexplained"] == 3


def _image(blob: dict[int, str], references: dict[int, list[int]] | None = None):
    data = {va: bytes.fromhex(raw) for va, raw in blob.items()}

    def read(va, size):
        base = max(a for a in data if a <= va)
        return data[base][va - base: va - base + size]

    return SimpleNamespace(read=read, references=references or {})


@pytest.fixture
def fake_references(monkeypatch):
    monkeypatch.setattr(boundaries, "_references", lambda img, va: img.references.get(va, []))


def test_trailing_fill_to_the_extent_end_is_explained(fake_references):
    entry = SimpleNamespace(va=0x100, size=0x14)
    lifted = _lifted([(0x100, 0x10, {})])
    disposition, open_, detail = boundaries._extent_disposition(
        _image({0x110: "90909090"}), entry, SimpleNamespace(body_bytes=0x10), lifted)
    assert not open_ and detail["fill"] == 4, detail


def test_trailing_non_fill_beyond_discovery_stays_open(fake_references):
    entry = SimpleNamespace(va=0x100, size=0x14)
    lifted = _lifted([(0x100, 0x10, {})])
    assert boundaries._extent_disposition(
        _image({0x110: "909090c3"}), entry, SimpleNamespace(body_bytes=0x10), lifted)[1]


def test_interior_block_referenced_from_outside_is_explained(fake_references):
    # SEH filter/except blocks are reached only through their parent's scope table.
    entry = SimpleNamespace(va=0x100, size=0x14)
    lifted = _lifted([(0x100, 0x10, {}), (0x113, 1, {})], data=[(0x110, "6a0158")])
    disposition, open_, detail = boundaries._extent_disposition(
        _image({}, {0x110: [0x9713E4]}), entry, SimpleNamespace(body_bytes=0x11), lifted)
    assert not open_ and detail["externally_referenced"] == 3, detail


def test_unwind_funclet_parent_is_the_entry_registering_its_handler_stub():
    proof = generated.proofs()[0x009660D0]  # last pinned entry: lea ecx, [ebp - N] ; jmp destructor
    assert proof.kind == "unwind-funclet"
    assert image.primary().dword(proof.metadata) in generated.FUNCINFO_MAGIC
    assert generated.owner(proof.site) is not None


def test_handler_stub_registration_resolves_to_its_prologue_owner():
    stub, site = next((stub, site) for stub, site in generated._cxx().values() if stub == 0x00953499)
    assert site == 0x00401002 and generated.owner(site) == 0x00401000


def test_seh_finally_parent_registers_the_scope_table():
    proof = generated.proofs()[0x00946323]
    assert (proof.kind, proof.metadata, proof.site) == ("seh-finally", 0x009713D0, 0x009462BE)
    assert generated.owner(proof.site) is not None


def test_import_thunk_names_its_import():
    proof = generated.proofs()[0x00944C10]
    assert proof.kind == "import-thunk" and "!" in proof.import_name


def test_registration_in_code_outside_every_entry_has_no_owner():
    # 0x00454900 has an EH prologue but no inventory entry; its preceding entry ends earlier.
    assert generated.owner(0x00454908) is None


@pytest.mark.parametrize("va, sites", [(0x00682060, [0x0066F479, 0x0066FCC0]), (0x0066F4C0, [0x0066EFDD])])
def test_outside_inventory_entries_are_referenced_by_absolute_pointers(va, sites):
    refs = boundaries._references(image.primary(), va)
    assert all(s in refs for s in sites)


def test_legacy_record_that_decoded_its_extent_explains_the_body():
    fn = SimpleNamespace(body_bytes=49771)
    whole = SimpleNamespace(legacy=True, size=64465, source="ledger", record={"instruction_bytes": 64465})
    disposition, open_, detail = boundaries._legacy_disposition(whole, fn)
    assert not open_ and "undercounts" in disposition and detail["instruction_bytes"] == 64465
    # A partial instruction count leaves the extent to discovery; no count does too.
    assert boundaries._legacy_disposition(
        SimpleNamespace(legacy=True, size=560, source="ledger", record={"instruction_bytes": 462}), fn) is None
    assert boundaries._legacy_disposition(SimpleNamespace(legacy=True, size=160, record={}), fn) is None


def test_trailing_data_referenced_by_the_function_is_explained(fake_references):
    entry = SimpleNamespace(va=0x100, size=0x18)
    lifted = _lifted([(0x100, 0x10, {0x110: "v"})], data=[(0x110, "0001020304050607")])
    disposition, open_, detail = boundaries._extent_disposition(
        _image({0x110: "0001020304050607"}), entry, SimpleNamespace(body_bytes=0x10), lifted)
    assert not open_ and detail["referenced_data"] == 8, detail


def test_called_shape_match_is_ordinary_code():
    disposition, open_ = boundaries._unproven_disposition([0x0094526D])
    assert not open_ and "called" in disposition


def test_gather_reports_every_exception_kind_over_a_small_inventory(monkeypatch):
    from cmre import inventory, ledger

    inv = inventory.load()
    keep = {0x006701F0, 0x0066E790, 0x009660D0, 0x00945A60, 0x00945AE0}
    small = SimpleNamespace(functions={va: inv.functions[va] for va in keep}, callees={}, modules={},
                            entries_sorted=sorted(keep), next_entry_after=inv.next_entry_after)
    recovered = {va: e for va, e in ledger.recovered().items() if va in keep | {0x00682060}}
    monkeypatch.setattr(inventory, "load", lambda: small)
    monkeypatch.setattr(ledger, "recovered", lambda: recovered)
    monkeypatch.setattr(generated, "proofs", lambda: {0x009660D0: generated.Proof(
        0x009660D0, "unwind-funclet", 0x00971A98, 0x00401002)})
    monkeypatch.setattr(generated, "parents", lambda: {0x00401002: 0x00401000})
    report = boundaries.gather()
    rows = {(e["kind"], e["function"]): e for e in report["exceptions"]}
    assert not rows["outside-inventory", "fn_00682060"]["open"]
    assert not rows["extent-body", "fn_006701f0"]["open"]
    assert not rows["extent-body", "fn_0066e790"]["open"]
    assert not rows["inventory-body-overlap", "fn_00945a60"]["open"]
    assert report["generated"]["fn_009660d0"]["parent"] == "fn_00401000"
    text = report["text"]
    assert text["owned_by_entries"] + sum(v["bytes"] for v in text["unowned"].values()) == text["bytes"]
    assert boundaries.summary(report)["exceptions"] == len(report["exceptions"])
