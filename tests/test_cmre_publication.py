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


def test_short_listings_and_prose_addresses_are_not_findings(originals):
    seven = "".join(LISTING.splitlines(keepends=True)[:7])
    prose = "".join(f"0x{0x00401000 + i:08x} and 0x{0x00402000 + i:08x} call sites differ\n" for i in range(20))
    adapter = '__asm__ volatile("push ebp\\n" "mov ebp, esp\\n" "pop ebp\\n" "ret\\n");\n' * 4
    for text in (seven, prose, adapter):
        assert pub.scan_blob("recovery/c/fn_00401000.c", text.encode(), originals["index"]) == []


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


def test_receipt_records_a_passing_private_scan(originals, repo, tmp_path):
    head = _commit(repo, {"b.txt": b"clean\n"}, "clean")
    commit, findings = pub.scan_commit(repo, "fresh", originals["index"], private_refs=())
    path = pub.write_receipt(tmp_path / "receipts", commit, findings, originals["index"],
                             remote={"name": "origin", "url": "https://example.invalid/r.git",
                                     "visibility": "PRIVATE"})
    receipt = json.loads(path.read_text(encoding="utf-8"))
    assert path.name == f"{head}.json"
    assert (receipt["schema"], receipt["commit"], receipt["result"]) == (pub.RECEIPT_SCHEMA, head, "pass")
    assert receipt["threshold_bytes"] == 16 and receipt["index"]["sha256"] == originals["index"].identity()
    assert pub.receipt_allows_push(path, head, "origin")


@pytest.mark.parametrize("change", [
    {"result": "fail"}, {"commit": "0" * 40}, {"remote": {"name": "origin", "visibility": "PUBLIC"}},
    {"remote": {"name": "upstream", "visibility": "PRIVATE"}}, {"schema": "other/1"},
])
def test_receipts_that_do_not_prove_a_private_pass_block_push(tmp_path, change):
    head = "a" * 40
    receipt = {"schema": pub.RECEIPT_SCHEMA, "commit": head, "result": "pass",
               "remote": {"name": "origin", "visibility": "PRIVATE"}}
    receipt.update(change)
    path = tmp_path / f"{head}.json"
    path.write_text(json.dumps(receipt), encoding="utf-8")
    assert not pub.receipt_allows_push(path, head, "origin")
    assert not pub.receipt_allows_push(tmp_path / "missing.json", head, "origin")
