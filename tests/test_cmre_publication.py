"""Publication boundary: original bytes, listings, assets and credentials stay out of Git."""
import json
import random
import subprocess

import pytest

from cmre import publication as pub


def _random(seed, size):
    return random.Random(seed).randbytes(size)


@pytest.fixture
def originals(tmp_path):
    root = tmp_path / "supplied"
    (root / "Data").mkdir(parents=True)
    image = b"MZ" + _random(1, 64 * 1024)
    data = _random(2, 64 * 1024)
    (root / "game.exe").write_bytes(image)
    (root / "Data" / "table.dat").write_bytes(data)
    (root / "Data" / "small.cfg").write_bytes(b"supplied configuration text\n")
    return {"root": root, "image": image, "data": data,
            "index": pub.OriginalIndex.build([root])}


def kinds(findings):
    return sorted({f.kind for f in findings})


def test_sixteen_copied_image_bytes_are_found_but_fifteen_are_not(originals):
    image, index = originals["image"], originals["index"]
    assert "original-bytes" in kinds(pub.scan_blob("x.txt", b"head" + image[1001:1017] + b"tail", index))
    assert "original-bytes" not in kinds(pub.scan_blob("x.txt", b"head" + image[1001:1016] + b"tail", index))


@pytest.mark.parametrize("encode", [
    lambda b: b.hex(),
    lambda b: " ".join(f"{x:02x}" for x in b),
    lambda b: "".join(f"\\x{x:02x}" for x in b),
    lambda b: ", ".join(f"0x{x:02X}" for x in b),
    lambda b: '"' + b[:8].hex() + '"\n    "' + b[8:].hex() + '"',
    lambda b: ", ".join(f"0x{int.from_bytes(b[i:i + 4], 'little'):08x}" for i in range(0, len(b), 4)),
])
def test_hex_encoded_image_bytes_in_text_are_found(originals, encode):
    run = originals["image"][2048:2048 + 24]
    text = ("fixture = [" + encode(run) + "]\n").encode()
    assert kinds(pub.scan_blob("tests/fixture.py", text, originals["index"])) == ["original-bytes"]


def test_authored_hex_and_hashes_are_not_findings(originals):
    text = (b'mem_write(SCRATCH, bytes.fromhex("64a100000000c3"))  # mov eax, fs:[0]; ret\n'
            b'TARGET_SHA256 = "0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86"\n'
            b"x = 0x00de2fd4; y = 0x00401000;\n")
    assert pub.scan_blob("tests/test_x.py", text, originals["index"]) == []


def test_low_information_runs_are_not_findings(tmp_path):
    root = tmp_path / "supplied"
    root.mkdir()
    (root / "pad.exe").write_bytes(b"MZ" + bytes(64) + (b"\x01" + bytes(15)) * 4 + b"\xcc" * 64)
    index = pub.OriginalIndex.build([root])
    for run in (bytes(32), b"\xcc" * 32, (b"\x01" + bytes(15)) * 2):
        assert pub.scan_blob("x.txt", b"<" + run.hex().encode() + b">", index) == []


def test_data_file_excerpts_and_whole_supplied_files_are_found(originals):
    index, data = originals["index"], originals["data"]
    assert kinds(pub.scan_blob("x.txt", data[5000:5000 + 80].hex().encode(), index)) == ["original-bytes"]
    small = (originals["root"] / "Data" / "small.cfg").read_bytes()
    assert kinds(pub.scan_blob("notes/copy.cfg.txt", small, index)) == ["asset-copy"]


def test_generated_inspection_manifests_are_matched_only_as_whole_copies(tmp_path):
    supplied, inspection = tmp_path / "supplied", tmp_path / "inspection"
    supplied.mkdir()
    inspection.mkdir()
    manifest = b'{"files": [' + b", ".join(b'{"sha256": "%s"}' % _random(i, 32).hex().encode() for i in range(8)) + b"]}"
    (inspection / "manifest.json").write_bytes(manifest)
    index = pub.OriginalIndex.build([supplied], copy_only=[inspection])
    assert pub.scan_blob("docs/x.md", manifest[:200], index) == []
    assert kinds(pub.scan_blob("docs/x.json", manifest, index)) == ["asset-copy"]


COMMENTARY = [f"Commentary line {i:02d}: %s strikes from twenty yards out!".encode() for i in range(8)]
PHRASES = [f"phrase {i:02d} of English".encode() for i in range(15)]  # 19 bytes each, under TEXT_MINIMUM


@pytest.fixture
def game_text(tmp_path):
    root = tmp_path / "supplied"
    root.mkdir()
    strings = [b"GetModuleHandleA", b"%s has been sacked by the board",
               b"The chairman has decided to terminate your contract with immediate effect today."]
    (root / "game.exe").write_bytes(b"MZ\0\0" + b"\0\0".join(strings + COMMENTARY + PHRASES) + b"\0" + _random(3, 256))
    return pub.OriginalIndex.build([root])


def test_identifiers_and_short_quotations_of_game_text_are_permitted(game_text):
    assert pub.scan_blob("tests/test_imports.py", b'name = "KERNEL32.dll!GetModuleHandleA"\n', game_text) == []
    assert pub.scan_blob("docs/quote.md", b'Message: "%s has been sacked by the board".\n', game_text) == []
    quotes = b"\n".join(b'- "' + line + b'"' for line in COMMENTARY[:3])
    assert pub.scan_blob("docs/quotes.md", quotes, game_text) == []


def test_long_game_text_quotes_and_pasted_string_tables_are_blocked(game_text):
    sentence = b'"The chairman has decided to terminate your contract with immediate effect today."'
    assert kinds(pub.scan_blob("docs/quote.md", sentence, game_text)) == ["original-text"]
    table = b"static const char *lines[] = {\n" + b"".join(b'    "' + line + b'",\n' for line in COMMENTARY) + b"};\n"
    found = pub.scan_blob("recovery/c/fn_00500000.c", table, game_text)
    assert kinds(found) == ["original-text"] and "8 quotes" in found[0].detail


def test_coincidental_short_phrases_do_not_add_up(game_text):
    prose = b"\n".join(b"We wrote " + phrase + b" ourselves." for phrase in PHRASES)
    assert pub.scan_blob("docs/prose.md", prose, game_text) == []


LISTING = "".join(f'  "v_{0x006181d0 + 4 * i:08x}: mov eax, dword ptr [esp + {4 * i}]\\n"\n' for i in range(8))


def test_original_instruction_listings_are_found(originals):
    found = pub.scan_blob("recovery/c/fn_006181d0.c", LISTING.encode(), originals["index"])
    assert kinds(found) == ["instruction-listing"]
    objdump = "".join(f"  {0x401000 + 2 * i:x}:\t8b c1\tmov    %ecx,%eax\n" for i in range(8))
    assert kinds(pub.scan_blob("notes.txt", objdump.encode(), originals["index"])) == ["instruction-listing"]
    lifted = "".join(f"v_{0x00545a60 + 4 * i:08x}:\n    cmp ecx, {i}\n" for i in range(8))
    assert kinds(pub.scan_blob("tests/fixtures/x.S", lifted.encode(), originals["index"])) == ["instruction-listing"]


def test_candidate_listing_advice_honors_only_exact_source_review(tmp_path):
    data = LISTING.encode()
    source = "recovery/c/fn_006181d0.c"
    assert kinds(pub.candidate_listing_findings(tmp_path, 0x006181D0, data)) == ["instruction-listing"]
    reviews = tmp_path / "CM0102/Analysis/cmre/publication/source-reviews.json"
    reviews.parent.mkdir(parents=True)
    reviews.write_text(json.dumps({"schema": "cmre-source-reviews/1", "reviews": [
        {"path": source, "sha256": pub.env.digest_bytes(data), "reviewer": "analyst", "reason": "authored"}
    ]}), encoding="utf-8")
    assert pub.candidate_listing_findings(tmp_path, 0x006181D0, data) == []
    assert kinds(pub.candidate_listing_findings(tmp_path, 0x006181D0, data + b"\n")) == ["instruction-listing"]
    reviews.write_text("{", encoding="utf-8")
    assert kinds(pub.candidate_listing_findings(tmp_path, 0x006181D0, data)) == ["instruction-listing"]


def test_short_listings_and_prose_addresses_are_not_findings(originals):
    seven = "".join(LISTING.splitlines(keepends=True)[:7])
    prose = "".join(f"0x{0x00401000 + i:08x} and 0x{0x00402000 + i:08x} call sites differ\n" for i in range(20))
    adapter = '__asm__ volatile("push ebp\\n" "mov ebp, esp\\n" "pop ebp\\n" "ret\\n");\n' * 4
    for text in (seven, prose, adapter):
        assert pub.scan_blob("recovery/c/fn_00401000.c", text.encode(), originals["index"]) == []


# Synthetic wrapper-shaped instructions at made-up addresses (no original bytes or listings).
HEAP_WRAPPER = [(f"{0x00401000 + 4 * n:08x}", insn) for n, insn in enumerate([
    "push esi", "mov esi, ecx", "call 0x401200", "test byte ptr [esp + 8], 1", "je 0x401028",
    "push esi", "call 0x401300", "add esp, 4", "mov eax, esi", "pop esi", "ret 4"])]
LISTING_SHAPES = {
    "bare-in-comment": lambda a, i: f" *   {a}  {i}\n",
    "0x-in-comment": lambda a, i: f" *   0x{a}  {i}\n",
    "colon-and-arrow-note": lambda a, i: f"{a}: {i} -> keeps the receiver\n",
    "pipe": lambda a, i: f"{a} | {i}\n",
    "arrow-separator": lambda a, i: f"0x{a} -> {i}   ; note\n",
    "trailing-semicolon-note": lambda a, i: f" *   {a}  {i}                      ; note\n",
}


@pytest.mark.parametrize("shape", sorted(LISTING_SHAPES))
def test_address_keyed_listing_shapes_are_found(originals, shape):
    text = "/* map\n" + "".join(LISTING_SHAPES[shape](a, i) for a, i in HEAP_WRAPPER) + " */\n"
    found = pub.scan_blob("recovery/c/fn_0056f230.c", text.encode(), originals["index"])
    assert kinds(found) == ["instruction-listing"]


def test_branch_and_ret_operands_count_toward_a_listing(originals):
    text = "".join(f" *   {a}  {i}\n" for a, i in HEAP_WRAPPER if i.split()[0] in {"call", "je", "ret"}) * 3
    assert kinds(pub.scan_blob("recovery/c/fn_0056f230.c", text.encode(), originals["index"])) == ["instruction-listing"]


def test_inline_tags_and_prose_with_addresses_are_not_listings(originals):
    inline = "".join(f"    x = {i}; /* 0x{0x0090b083 + i:08x} */\n" for i in range(12))
    prose = "".join(f" * 0x{0x00401000 + i:08x} and 0x{0x00402000 + i:08x} call 0x{0x00403000 + i:08x} sites differ\n"
                    for i in range(12))
    ranges = "".join(f" * {0x0090f4d0 + i:08x}..{0x0090f4e0 + i:08x} jump 5 times, then ret\n" for i in range(12))
    for text in (inline, prose, ranges):
        assert pub.scan_blob("recovery/c/fn_0090b080.c", text.encode(), originals["index"]) == []


@pytest.mark.parametrize("secret", [
    "token = 'ghp_" + "a1B2" * 9 + "'",
    "github_pat_" + "A1b2C3d4E5" * 5,
    "-----BEGIN OPENSSH " + "PRIVATE KEY-----",
    "aws = 'AKIA" + "ABCDEFGHIJKLMNOP" + "'",
    "key = 'sk-ant-api03-" + "x" * 40 + "'",
])
def test_credentials_are_found(originals, secret):
    assert kinds(pub.scan_blob("scripts/x.py", secret.encode(), originals["index"])) == ["credential"]


@pytest.mark.parametrize("path", ["CM0102/Game/readme.txt", "tools/unshield/README", "build/out.exe",
                                  "recovery/rng/cm_rng.dll", "data/eng.lng", "NUL", "bash.exe.stackdump"])
def test_supplied_and_build_paths_are_found(originals, path):
    assert "asset-path" in kinds(pub.scan_blob(path, b"text\n", originals["index"]))


def test_binary_content_needs_review(originals):
    assert kinds(pub.scan_blob("docs/image.dat.txt", b"\x89PNG\r\n\x1a\n\0\0\0", originals["index"])) == ["binary"]


def test_local_fixtures_are_content_addressed_and_verified(tmp_path, monkeypatch):
    from cmre import env

    monkeypatch.setattr(env, "LOCAL_FIXTURES", tmp_path)
    data = b"retained original-derived fixture\n"
    sha = env.digest_bytes(data)
    with pytest.raises(SystemExit, match="local-only fixture"):
        env.local_fixture(sha)
    (tmp_path / sha).write_bytes(data)
    assert env.local_fixture(sha) == data
    (tmp_path / sha).write_bytes(data + b"changed")
    with pytest.raises(SystemExit, match="hash mismatch"):
        env.local_fixture(sha)


def _git(repo, *args):
    return subprocess.run(["git", "-c", "user.name=t", "-c", "user.email=t@example.invalid",
                           "-c", "commit.gpgsign=false", *args],
                          cwd=repo, check=True, capture_output=True, text=True).stdout.strip()


@pytest.fixture
def repo(tmp_path):
    path = tmp_path / "repo"
    path.mkdir()
    _git(path, "init", "-q", "-b", "fresh")
    return path


def _commit(repo, files, message):
    for name, content in files.items():
        target = repo / name
        if content is None:
            _git(repo, "rm", "-q", name)
            continue
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(content)
        _git(repo, "add", name)
    _git(repo, "commit", "-q", "-m", message)
    return _git(repo, "rev-parse", "HEAD")


def test_commit_scan_covers_removed_files_in_reachable_history(originals, repo):
    _commit(repo, {"a.txt": originals["image"][100:140].hex().encode()}, "copy")
    head = _commit(repo, {"a.txt": None, "b.txt": b"clean\n"}, "remove")
    commit, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=())
    assert commit == head
    assert [(f.path, f.kind) for f in findings] == [("a.txt", "original-bytes")]


def test_commit_scan_marks_removed_content_already_in_local_tracking_history(originals, repo):
    tracked = _commit(repo, {"a.txt": originals["image"][100:140].hex().encode()}, "copy")
    tracking = "refs/remotes/origin/fresh"
    _git(repo, "update-ref", tracking, tracked)
    _commit(repo, {"a.txt": None, "b.txt": b"clean\n"}, "remove")

    _, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=(), tracking_ref=tracking)
    finding, = findings
    assert finding.kind == "original-bytes"
    assert "history" in finding.detail and tracking in finding.detail


def _published_history(originals, repo):
    """main publishes a dirty blob and later removes it; a PR branch builds on main."""
    _commit(repo, {"a.txt": originals["image"][100:140].hex().encode()}, "copy")
    published = _commit(repo, {"a.txt": None, "b.txt": b"clean\n"}, "remove")
    _git(repo, "update-ref", "refs/remotes/origin/main", published)
    return published


def test_history_only_content_already_on_published_main_is_set_aside(originals, repo):
    _published_history(originals, repo)
    _commit(repo, {"c.txt": b"new clean work\n"}, "pr work")
    public = []
    _, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=(),
                                  tracking_ref="refs/remotes/origin/pr", published_ref="refs/remotes/origin/main",
                                  public_history=public)
    assert findings == []
    finding, = public
    assert (finding.path, finding.kind) == ("a.txt", "original-bytes")
    assert "refs/remotes/origin/main" in finding.detail


def test_published_main_does_not_excuse_new_or_tip_content(originals, repo):
    _published_history(originals, repo)
    # The same public blob restored at the tip, and a new dirty blob only in branch history.
    _commit(repo, {"a.txt": originals["image"][100:140].hex().encode(),
                   "d.txt": originals["image"][300:340].hex().encode()}, "restore and add")
    _commit(repo, {"d.txt": None}, "drop new dirt")
    public = []
    _, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=(),
                                  tracking_ref="refs/remotes/origin/pr", published_ref="refs/remotes/origin/main",
                                  public_history=public)
    assert sorted((f.path, f.kind) for f in findings) == [("a.txt", "original-bytes"), ("d.txt", "original-bytes")]
    assert public == []


def _scan_with_public(originals, repo, **extra):
    public = []
    _, findings = pub.scan_commit(repo, "fresh", originals["index"], tracking_ref="refs/remotes/origin/pr",
                                  published_ref="refs/remotes/origin/main", public_history=public,
                                  **{"private_refs": (), **extra})
    return findings, public


def test_published_blob_restored_at_another_tip_path_still_blocks(originals, repo):
    _published_history(originals, repo)
    _commit(repo, {"moved.txt": originals["image"][100:140].hex().encode()}, "restore elsewhere")
    findings, public = _scan_with_public(originals, repo)
    assert sorted({(f.path, f.kind) for f in findings}) == [("a.txt", "original-bytes"), ("moved.txt", "original-bytes")]
    assert public == []


def test_published_ref_that_is_not_an_ancestor_sets_nothing_aside(originals, repo):
    _published_history(originals, repo)
    # A tracking ref moved to an unrelated commit must not vouch for this branch's history.
    _git(repo, "checkout", "-q", "--orphan", "other")
    _commit(repo, {"a.txt": originals["image"][100:140].hex().encode()}, "copy")
    _git(repo, "update-ref", "refs/remotes/origin/main", _commit(repo, {"a.txt": None, "e.txt": b"x\n"}, "remove"))
    _git(repo, "checkout", "-q", "fresh")
    findings, public = _scan_with_public(originals, repo)
    assert [(f.path, f.kind) for f in findings] == [("a.txt", "original-bytes")]
    assert public == []


def test_scan_without_public_history_list_blocks_published_history(originals, repo):
    _published_history(originals, repo)
    _, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=(),
                                  tracking_ref="refs/remotes/origin/pr", published_ref="refs/remotes/origin/main")
    assert [(f.path, f.kind) for f in findings] == [("a.txt", "original-bytes")]


def test_private_ancestry_still_blocks_when_all_history_is_published(originals, repo):
    published = _published_history(originals, repo)
    _git(repo, "update-ref", "refs/private/archive", published)
    findings, public = _scan_with_public(originals, repo, private_refs=("refs/private/archive",))
    assert [f.kind for f in findings] == ["private-history"]
    assert [(f.path, f.kind) for f in public] == [("a.txt", "original-bytes")]


def test_missing_published_ref_sets_nothing_aside(originals, repo):
    _commit(repo, {"a.txt": originals["image"][100:140].hex().encode()}, "copy")
    _commit(repo, {"a.txt": None, "b.txt": b"clean\n"}, "remove")
    public = []
    _, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=(),
                                  tracking_ref="refs/remotes/origin/pr", published_ref="refs/remotes/origin/main",
                                  public_history=public)
    assert [(f.path, f.kind) for f in findings] == [("a.txt", "original-bytes")]
    assert public == []


def test_receipt_passes_with_public_history_and_records_it(originals, tmp_path):
    remote = {"name": "origin", "url": "https://example.invalid/source.git", "visibility": "PUBLIC"}
    public = [pub.Finding("a.txt", "original-bytes", "detail")]
    path = pub.write_receipt(tmp_path, "a" * 40, [], originals["index"], remote,
                             public_history=public, published={"ref": "refs/remotes/origin/main", "commit": "b" * 40})
    receipt = json.loads(path.read_text(encoding="utf-8"))
    assert receipt["result"] == "pass" and receipt["findings"] == []
    assert receipt["public_history"] == [{"path": "a.txt", "kind": "original-bytes", "detail": "detail"}]
    assert receipt["published"] == {"ref": "refs/remotes/origin/main", "commit": "b" * 40}


def test_commit_scan_rejects_history_shared_with_private_refs(originals, repo):
    _commit(repo, {"a.txt": b"old local history\n"}, "old")
    _git(repo, "branch", "private")
    _commit(repo, {"b.txt": b"new\n"}, "new")
    _, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=("refs/heads/private",))
    assert kinds(findings) == ["private-history"]
    _git(repo, "checkout", "-q", "--orphan", "clean")
    head = _commit(repo, {"b.txt": b"new\n"}, "fresh root")
    commit, findings = pub.scan_commit(repo, "clean", originals["index"], private_refs=("refs/heads/private",))
    assert (commit, findings) == (head, [])


def test_default_private_history_survives_main_migration(originals, repo):
    old = _commit(repo, {"a.txt": b"old local history\n"}, "old")
    _git(repo, "update-ref", "refs/private/cm0102-pre-publication", old)
    _git(repo, "checkout", "-q", "--orphan", "main")
    clean = _commit(repo, {"a.txt": b"published source\n"}, "new root")
    assert pub.scan_commit(repo, "main", originals["index"]) == (clean, [])
    assert kinds(pub.scan_commit(repo, "fresh", originals["index"])[1]) == ["private-history"]
    _git(repo, "update-ref", "-d", "refs/private/cm0102-pre-publication")
    with pytest.raises(SystemExit, match="cannot compare"):
        pub.scan_commit(repo, "main", originals["index"])


def test_staged_scan_checks_inherited_accepted_c(originals, repo, tmp_path, monkeypatch):
    path = "recovery/c/fn_00401000.c"
    _commit(repo, {path: b"int before(void) { return 1; }\n"}, "published snapshot")
    ledger = tmp_path / "accepted-ledger"
    ledger.mkdir()
    monkeypatch.setattr(pub.env, "LEDGER_DIR", ledger)
    accepted = b"int after(void) { return 2; }\n"
    (ledger / "fn_00401000.json").write_text(
        json.dumps({"c": {"source_sha256": pub.env.digest_bytes(accepted)}}), encoding="utf-8"
    )
    (repo / "notes.md").write_text("update notes\n", encoding="utf-8")
    _git(repo, "add", "--", "notes.md")

    findings = pub.scan_staged(repo, originals["index"])
    assert [(finding.path, finding.kind) for finding in findings] == [(path, "accepted-mismatch")]

    _git(repo, "rm", "--", path)
    assert pub.scan_staged(repo, originals["index"]) == []


def test_commit_checks_accepted_identity_for_each_path_sharing_a_blob(originals, repo, tmp_path, monkeypatch):
    stale = "recovery/c/fn_00401000.c"
    equal = "recovery/c/fn_00402000.c"
    original = b"int f(void) { return 1; }\n"
    _commit(repo, {stale: original, equal: original}, "shared blob")
    ledger = tmp_path / "accepted-ledger"
    ledger.mkdir()
    monkeypatch.setattr(pub.env, "LEDGER_DIR", ledger)
    for path, digest in ((stale, "1" * 64), (equal, pub.env.digest_bytes(original))):
        record = ledger / (path.rsplit("/", 1)[1].removesuffix(".c") + ".json")
        record.write_text(json.dumps({"c": {"source_sha256": digest}}), encoding="utf-8")

    _, findings = pub.scan_commit(repo, "HEAD", originals["index"], private_refs=())
    assert [(finding.path, finding.kind) for finding in findings] == [(stale, "accepted-mismatch")]


def test_published_c_must_be_the_accepted_bytes(originals, repo, tmp_path, monkeypatch):
    from cmre import env

    ledger = tmp_path / "ledger"
    ledger.mkdir()
    monkeypatch.setattr(env, "LEDGER_DIR", ledger)
    accepted = b"int x;\r\n"  # Git's eol=lf normalisation would publish different bytes
    (ledger / "fn_00401000.json").write_text(json.dumps({"c": {"source_sha256": env.digest_bytes(accepted)}}))
    (repo / ".gitattributes").write_text("*.c text eol=lf\n")
    _commit(repo, {"recovery/c/fn_00401000.c": accepted, ".gitattributes": b"*.c text eol=lf\n"}, "normalised")
    _, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=())
    assert [(f.path, f.kind) for f in findings] == [("recovery/c/fn_00401000.c", "accepted-mismatch")]
    _commit(repo, {"recovery/c/fn_00401000.c": None}, "withdraw until re-accepted")
    assert pub.scan_commit(repo, "fresh", originals["index"], private_refs=())[1] == []


@pytest.mark.parametrize("visibility", ["PRIVATE", "PUBLIC"])
def test_receipt_records_a_passing_source_scan(originals, repo, tmp_path, visibility):
    head = _commit(repo, {"b.txt": b"clean\n"}, "clean")
    commit, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=())
    path = pub.write_receipt(tmp_path / "receipts", commit, findings, originals["index"],
                             remote={"name": "origin", "url": "https://example.invalid/r.git",
                                     "visibility": visibility})
    receipt = json.loads(path.read_text(encoding="utf-8"))
    assert path.name == f"{head}.json"
    assert (receipt["schema"], receipt["commit"], receipt["result"]) == (pub.RECEIPT_SCHEMA, head, "pass")
    assert receipt["threshold_bytes"] == 16 and receipt["index"]["sha256"] == originals["index"].identity()
    assert pub.receipt_allows_push(path, head, "origin", remote_url="https://example.invalid/r.git",
                                   visibility=visibility)


@pytest.mark.parametrize("change", [
    {"result": "fail"}, {"commit": "0" * 40}, {"remote": {"name": "origin", "visibility": "UNKNOWN"}},
    {"remote": {"name": "upstream", "visibility": "PRIVATE"}}, {"schema": "other/1"},
])
def test_receipts_that_do_not_prove_a_source_scan_block_push(tmp_path, change):
    head = "a" * 40
    receipt = {"schema": pub.RECEIPT_SCHEMA, "commit": head, "result": "pass",
               "remote": {"name": "origin", "visibility": "PRIVATE"}}
    receipt.update(change)
    path = tmp_path / f"{head}.json"
    path.write_text(json.dumps(receipt), encoding="utf-8")
    assert not pub.receipt_allows_push(path, head, "origin")
    assert not pub.receipt_allows_push(tmp_path / "missing.json", head, "origin")


def test_listing_review_is_bound_to_exact_path_and_content(originals, tmp_path):
    data = LISTING.encode()
    path = "recovery/c/fn_006181d0.c"
    review = tmp_path / "review.json"
    review.write_text(json.dumps({"schema": "cmre-source-reviews/1", "reviews": [{
        "path": path, "sha256": pub.env.digest_bytes(data), "reviewer": "analyst",
        "reason": "Authored executable adapter reviewed against retained emission evidence."
    }]}))
    reviews = pub.load_source_reviews(review)
    assert pub.scan_blob(path, data, originals["index"], reviews) == []
    assert kinds(pub.scan_blob(path, data + b"\n", originals["index"], reviews)) == ["instruction-listing"]
    assert kinds(pub.scan_blob("other.c", data, originals["index"], reviews)) == ["instruction-listing"]


@pytest.mark.parametrize("extra,expected", [
    (b"\n" + b"ghp_" + b"a1B2" * 9, "credential"),
    (None, "original-bytes"),
])
def test_reviewed_assembly_still_checks_copied_bytes_and_credentials(originals, extra, expected):
    data = LISTING.encode() + (extra if extra is not None else originals["image"][1001:1025].hex().encode())
    path = "recovery/c/fn_006181d0.c"
    reviews = {(path, pub.env.digest_bytes(data)): {"reviewer": "analyst", "reason": "reviewed"}}
    assert expected in kinds(pub.scan_blob(path, data, originals["index"], reviews))


def test_reviewed_assembly_in_removed_history_is_checked_by_blob(originals, repo):
    path = "src/adapter.S"
    data = LISTING.encode()
    _commit(repo, {path: data}, "authored adapter")
    _commit(repo, {path: None, "a.txt": b"clean\n"}, "remove")
    reviews = {(path, pub.env.digest_bytes(data)): {"reviewer": "analyst", "reason": "reviewed"}}
    assert pub.scan_commit(repo, "fresh", originals["index"], private_refs=(), source_reviews=reviews)[1] == []
    assert kinds(pub.scan_commit(repo, "fresh", originals["index"], private_refs=())[1]) == ["instruction-listing"]


def test_source_review_does_not_cover_an_identical_blob_at_another_historical_path(originals, repo):
    data = LISTING.encode()
    _commit(repo, {"src/a.S": data, "src/b.S": data}, "same bytes at two paths")
    _commit(repo, {"src/b.S": None}, "remove unreviewed copy")
    reviews = {("src/a.S", pub.env.digest_bytes(data)): {"reviewer": "analyst", "reason": "reviewed"}}
    _, found = pub.scan_commit(repo, "fresh", originals["index"], private_refs=(), source_reviews=reviews)
    assert [(f.path, f.kind) for f in found] == [("src/b.S", "instruction-listing")]


def test_merge_only_local_path_is_checked_even_with_clean_contents(originals, repo):
    _commit(repo, {"base.txt": b"base\n"}, "base")
    _git(repo, "checkout", "-q", "-b", "side")
    _commit(repo, {"side.txt": b"side\n"}, "side")
    _git(repo, "checkout", "-q", "fresh")
    _commit(repo, {"main.txt": b"main\n"}, "main")
    _git(repo, "merge", "--no-commit", "--no-ff", "side")
    _commit(repo, {"local/fixtures/merge.c": b"int merge_only;\n"}, "merge with local-only file")
    _, found = pub.scan_commit(repo, "fresh", originals["index"], private_refs=())
    assert [(f.path, f.kind) for f in found] == [("local/fixtures/merge.c", "asset-path")]


@pytest.mark.parametrize("change", [
    {"path": "../adapter.S"}, {"path": "/adapter.S"}, {"path": "C:/adapter.S"},
    {"path": "src\\adapter.S"}, {"path": "raw.txt"}, {"sha256": "0" * 63},
    {"reviewer": " "}, {"reason": ""},
])
def test_invalid_source_review_is_rejected(tmp_path, change):
    entry = {"path": "src/a.S", "sha256": "0" * 64, "reviewer": "analyst", "reason": "authored adapter"}
    path = tmp_path / "reviews.json"
    path.write_text(json.dumps({"schema": "cmre-source-reviews/1", "reviews": [{**entry, **change}]}))
    with pytest.raises(SystemExit, match="source review"):
        pub.load_source_reviews(path)


@pytest.mark.parametrize("path", ["recovery/hints/x.json", "recovery/c/tasks/x/candidate.c", "local/fixtures/x.c"])
def test_local_records_remain_local_even_with_a_source_review(originals, path):
    data = LISTING.encode()
    reviews = {(path, pub.env.digest_bytes(data)): {"reviewer": "analyst", "reason": "reviewed"}}
    assert "asset-path" in kinds(pub.scan_blob(path, data, originals["index"], reviews))


@pytest.mark.parametrize("contents", [None, b"wrong executable"])
def test_publication_is_unavailable_without_pinned_original(tmp_path, monkeypatch, contents):
    target = tmp_path / "cm0102.exe"
    if contents is not None:
        target.write_bytes(contents)
    monkeypatch.setattr(pub.env, "TARGET", target)
    with pytest.raises(SystemExit, match="publication check unavailable"):
        pub.supplied_index()


@pytest.mark.parametrize("visibility", ["PUBLIC", "PRIVATE"])
def test_verify_remote_requires_the_scanned_destination(originals, tmp_path, monkeypatch, visibility):
    from types import SimpleNamespace

    commit = "a" * 40
    target = {"name": "origin", "url": "https://example.invalid/source.git", "visibility": visibility}
    monkeypatch.setattr(pub, "RECEIPTS", tmp_path)
    monkeypatch.setattr(pub, "remote_state", lambda repo, name: dict(target))
    monkeypatch.setattr(pub, "_git", lambda repo, *args: (commit + ("\trefs/heads/main" if args[0] == "ls-remote" else "") + "\n").encode())
    args = SimpleNamespace(ref="source", remote="origin")
    assert pub.verify_remote(args) == 1
    pub.write_receipt(tmp_path, commit, [], originals["index"], dict(target))
    assert pub.verify_remote(args) == 0
    target["url"] = "https://example.invalid/changed.git"
    assert pub.verify_remote(args) == 1


def _instruction_map():
    """A synthetic instruction map in the shape workers paste, built at runtime so this
    test file never carries a listing (or original bytes) of its own."""
    rows = [("push esi", "keep the register"), ("mov esi, ecx", "receiver"),
            ("call 0x401100", "callee"), ("test byte [esp+8],1", "flag"), (None, "grouped calls"),
            ("mov eax, esi", "result"), ("pop esi", "restore"), ("ret 4", "cleanup")]
    lines = ["/* Instruction map (listing in the brief):"]
    for i, (insn, note) in enumerate(rows):
        address = 0x00401000 + 3 * i
        if insn is None:
            lead = f"{address:08x}/{(address + 1) & 0xfff:03x}/{(address + 6) & 0xfff:03x}"
            lines.append(f" *   {lead:<29} -> {note}")
        else:
            lines.append(f" *   {address:08x} {insn:<19} -> {note}")
    return "\n".join(lines + [" */", ""])


INSTRUCTION_MAP = _instruction_map()


def test_instruction_map_with_compact_address_group_is_found(originals):
    found = pub.scan_blob("recovery/c/fn_0090a310.c", INSTRUCTION_MAP.encode(), originals["index"])
    assert kinds(found) == ["instruction-listing"]
