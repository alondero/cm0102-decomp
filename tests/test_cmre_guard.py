"""PreToolUse guard: protected paths, generated records and interpreter choice."""
import json
import os
import re
import subprocess
import sys

import pytest

from cmre import env

GUARD = env.ROOT / "cmre" / "guard.py"


def decide(tool, **tool_input):
    p = subprocess.run([sys.executable, str(GUARD)], input=json.dumps({"tool_name": tool, "tool_input": tool_input}),
                       capture_output=True, text=True, cwd=env.ROOT)
    if p.returncode == 2:
        return "deny", p.stderr
    if p.stdout.strip():
        return json.loads(p.stdout)["hookSpecificOutput"]["permissionDecision"], p.stdout
    return "allow", ""


@pytest.mark.parametrize("path", [
    "CM0102/Game/cm0102.exe",
    (env.ROOT / "CM0102/Original-3.9.60/data/x").as_posix(),
    "CM0102/InstallMedia/CM0102.iso",
    "recovery/ledger/fn_00450a70.json",
    "recovery/lifted/fn_00450a70.S",
    "recovery/blocked/fn_00450a70.json",
    "recovery/date/ledger.json",
    "recovery/lang/cm_lang.c",
    "recovery/registry.json",
    "docs/STATUS.md",
])
def test_protected_paths_are_denied(path):
    assert decide("Write", file_path=path, content="x")[0] == "deny"
    assert decide("Edit", file_path=path, old_string="a", new_string="b")[0] == "deny"


@pytest.mark.skipif(sys.platform != "win32", reason="directory junctions")
def test_protected_paths_stay_protected_through_a_linked_asset_folder(tmp_path, monkeypatch):
    """A clone that links the local CM0102 folder must not resolve its way out of policy."""
    import _winapi
    from cmre import guard

    clone, assets = tmp_path / "clone", tmp_path / "assets"
    (assets / "Game").mkdir(parents=True)
    clone.mkdir()
    _winapi.CreateJunction(str(assets), str(clone / "CM0102"))
    monkeypatch.setattr(guard, "ROOT", clone)
    for path in (clone / "CM0102/Game/cm0102.exe", "CM0102/Game/cm0102.exe"):
        monkeypatch.chdir(clone)
        assert guard.decide("Write", {"file_path": str(path), "content": "x"})[0] == "deny"


@pytest.mark.parametrize("path", ["recovery/hints/fn_00450a70.json", "recovery/c/fn_00450a70.c", "docs/agent/notes.md"])
def test_agent_work_paths_are_allowed(path):
    assert decide("Write", file_path=path, content="x")[0] == "allow"


def test_harness_edits_need_approval():
    assert decide("Edit", file_path="cmre/difftest.py", old_string="a", new_string="b")[0] == "ask"


@pytest.mark.parametrize("command", [
    "python -m cmre status",
    "cd x && python3 scripts/verify_game.py",
    "rm -rf CM0102/Game/Data",
    "sed -i s/a/b/ recovery/ledger/fn_00450a70.json",
    "git push origin main",
])
def test_dangerous_commands_are_denied(command):
    assert decide("Bash", command=command)[0] == "deny"


@pytest.mark.parametrize("command", ["py -3.13 -m cmre next", "py -3.13 -m pytest", "git status", "cat recovery/ledger/fn_1.json"])
def test_normal_commands_are_allowed(command):
    assert decide("Bash", command=command)[0] == "allow"


def test_powershell_is_guarded_too():
    assert decide("PowerShell", command="Remove-Item -Recurse CM0102\\Game\\Data")[0] == "deny"


@pytest.mark.parametrize("command", ["taskkill /F /IM python.exe /T", "taskkill //F //IM py.exe", "Stop-Process -Name python"])
def test_image_name_kills_are_denied(command):
    assert decide("Bash", command=command)[0] == "deny"


@pytest.mark.parametrize("command", [
    # Observed false positives: stderr redirection beside a read of a protected path.
    "cat docs/STATUS.md docs/NEXT-SESSION-PROMPT.md && py -3.13 -m cmre status 2>&1 | head -60",
    "find . -path ./CM0102/Game -prune -o -type f -print 2>/dev/null",
    "ls CM0102/Game > $TEMP/listing.txt",
    "Get-Content docs/STATUS.md 2>$null",
])
def test_reads_with_redirection_elsewhere_are_allowed(command):
    assert decide("Bash", command=command)[0] == "allow"


@pytest.mark.parametrize("command", [
    "echo x > docs/STATUS.md",
    "cat a 2>&1 > recovery/ledger/fn_00450a70.json",
    "echo x >> recovery/lifted/fn_00450a70.S",
    "py -3.13 -m cmre status 1>CM0102/Game/out.txt",
    "Get-Content x | Out-File recovery/ledger/fn_00450a70.json",
])
def test_redirection_into_protected_paths_is_denied(command):
    assert decide("Bash", command=command)[0] == "deny"


@pytest.mark.parametrize("command", ['grep -n "foo\\|python " scripts/verify_game.py', "echo 'python x'"])
def test_python_inside_quotes_is_not_a_command(command):
    assert decide("Bash", command=command)[0] == "allow"


def codex_hook(tool, tool_input, *, scopes=()):
    hook_env = dict(os.environ, CMRE_AUTHORIZED_PATHS=json.dumps(scopes))
    return subprocess.run(
        [sys.executable, str(GUARD), "--codex"],
        input=json.dumps({"hook_event_name": "PreToolUse", "tool_name": tool,
                          "tool_input": tool_input, "cwd": str(env.ROOT)}),
        capture_output=True, text=True, cwd=env.ROOT, env=hook_env,
    )


@pytest.mark.parametrize("path,expected", [
    ("recovery/c/fn_00450a70.c", 0),
    ("CM0102/Game/cm0102.exe", 2),
    ("cmre/guard.py", 2),
])
def test_codex_patch_command_payload(path, expected):
    patch = f"*** Begin Patch\n*** Update File: {path}\n@@\n-a\n+b\n*** End Patch"
    result = codex_hook("apply_patch", {"command": patch})
    assert result.returncode == expected, result.stderr
    assert not result.stdout.strip()  # Codex must never receive unsupported `ask`.


def test_codex_authorization_does_not_override_preservation():
    for path, expected in [("cmre/guard.py", 0), ("CM0102/Game/cm0102.exe", 2)]:
        patch = f"*** Begin Patch\n*** Update File: {path}\n@@\n-a\n+b\n*** End Patch"
        result = codex_hook("apply_patch", {"command": patch}, scopes=["cmre", "CM0102/Game"])
        assert result.returncode == expected, result.stderr


def test_codex_shell_payload():
    assert codex_hook("Bash", {"command": "py -3.13 -m cmre status"}).returncode == 0
    result = codex_hook("Bash", {"command": "git push origin main"})
    assert result.returncode == 2
    assert "cmre publication scan" in result.stderr


@pytest.mark.skipif(sys.platform != "win32", reason="Windows hook command")
@pytest.mark.parametrize("tool,tool_input,expected", [
    ("Bash", {"command": "py -3.13 -m cmre status"}, 0),
    ("Bash", {"command": "git push origin main"}, 2),
    ("apply_patch", {"command": "*** Begin Patch\n*** Update File: cmre/guard.py\n@@\n-a\n+b\n*** End Patch"}, 2),
])
def test_configured_codex_hook_from_subdirectory(tool, tool_input, expected):
    config = json.loads((env.ROOT / ".codex" / "hooks.json").read_text())
    groups = config["hooks"]["PreToolUse"]
    handlers = [handler for group in groups if re.search(group["matcher"], tool)
                for handler in group["hooks"]]
    assert len(handlers) == 1
    handler = handlers[0]
    assert handler["type"] == "command"
    result = subprocess.run(
        handler["commandWindows"], shell=True,
        input=json.dumps({"hook_event_name": "PreToolUse", "tool_name": tool,
                          "tool_input": tool_input}),
        capture_output=True, text=True, cwd=env.ROOT / "recovery",
        env=dict(os.environ, CMRE_AUTHORIZED_PATHS="[]"), timeout=handler["timeout"],
    )
    assert result.returncode == expected, result.stderr
    assert not result.stdout.strip()


def _git(repo, *args):
    return subprocess.run(["git", "-c", "user.name=t", "-c", "user.email=t@example.invalid",
                           "-c", "commit.gpgsign=false", *args],
                          cwd=repo, check=True, capture_output=True, text=True).stdout.strip()


@pytest.fixture
def publication(tmp_path, monkeypatch):
    """A repository and receipt folder standing in for the authoritative checkout."""
    from cmre import guard

    repo = tmp_path / "repo"
    repo.mkdir()
    _git(repo, "init", "-q", "-b", "github-main")
    (repo / "a.txt").write_text("clean\n")
    _git(repo, "add", "a.txt")
    _git(repo, "commit", "-q", "-m", "clean")
    receipts = tmp_path / "receipts"
    receipts.mkdir()
    monkeypatch.setattr(guard, "ROOT", repo)
    monkeypatch.setattr(guard, "PUBLICATION_RECEIPTS", receipts)
    return guard, repo, receipts, _git(repo, "rev-parse", "HEAD")


def _receipt(receipts, commit, **change):
    receipt = {"schema": "cmre-publication-scan/1", "commit": commit, "result": "pass",
               "remote": {"name": "origin", "visibility": "PRIVATE"}}
    receipt.update(change)
    (receipts / f"{commit}.json").write_text(json.dumps(receipt), encoding="utf-8")


PUSH = "git " + "push"  # keeps this file's own edit commands clear of the push guard


@pytest.mark.parametrize("command", [f"{PUSH} origin github-main:main", f"rtk {PUSH} origin github-main:refs/heads/main"])
def test_push_of_a_scanned_commit_is_allowed(publication, command):
    guard, _, receipts, head = publication
    _receipt(receipts, head)
    assert guard.decide("Bash", {"command": command})[0] == "allow"


def test_push_without_a_passing_receipt_for_that_commit_is_denied(publication):
    guard, repo, receipts, head = publication
    verdict, reason = guard.decide("Bash", {"command": f"{PUSH} origin github-main:main"})
    assert verdict == "deny" and "cmre publication scan" in reason
    _receipt(receipts, head, result="fail")
    assert guard.decide("Bash", {"command": f"{PUSH} origin github-main:main"})[0] == "deny"
    _receipt(receipts, head)
    (repo / "b.txt").write_text("later\n")
    _git(repo, "add", "b.txt")
    _git(repo, "commit", "-q", "-m", "unscanned")
    assert guard.decide("Bash", {"command": f"{PUSH} origin github-main:main"})[0] == "deny"


@pytest.mark.parametrize('destination', ['fix/recovery-preflight', 'refs/heads/fix/recovery-preflight'])
def test_same_name_pr_branch_requires_exact_passing_private_receipt(publication, destination):
    guard, repo, receipts, head = publication
    _git(repo, 'branch', 'fix/recovery-preflight')
    command = f'{PUSH} origin fix/recovery-preflight:{destination}'
    assert guard.decide('Bash', {'command': command})[0] == 'deny'
    _receipt(receipts, head, result='fail')
    assert guard.decide('Bash', {'command': command})[0] == 'deny'
    _receipt(receipts, head, remote={'name': 'origin', 'visibility': 'PUBLIC'})
    assert guard.decide('Bash', {'command': command})[0] == 'deny'
    _receipt(receipts, head)
    assert guard.decide('Bash', {'command': command})[0] == 'allow'
    _git(repo, 'checkout', '-q', 'fix/recovery-preflight')
    (repo / 'later.txt').write_text('unscanned branch update\n')
    _git(repo, 'add', 'later.txt')
    _git(repo, 'commit', '-qm', 'unscanned update')
    assert guard.decide('Bash', {'command': command})[0] == 'deny'


@pytest.mark.parametrize('ref', ['reviewed-tag', 'refs/tags/reviewed-tag'])
def test_same_name_tag_is_not_a_pr_branch_even_with_passing_receipt(publication, ref):
    guard, repo, receipts, head = publication
    _git(repo, 'tag', 'reviewed-tag')
    _receipt(receipts, head)
    assert guard.decide('Bash', {'command': f'{PUSH} origin {ref}:{ref}'})[0] == 'deny'


@pytest.mark.parametrize("command", [
    PUSH, f"{PUSH} origin main", f"{PUSH} --force origin github-main:main",
    f"{PUSH} origin +github-main:main", f"{PUSH} --all origin", f"{PUSH} --mirror origin",
    f"{PUSH} origin github-main:main --tags", f"{PUSH} upstream github-main:main",
    f"{PUSH} origin github-main:other", f"{PUSH} origin github-main:main && echo done",
    f"{PUSH} origin github-main:refs/tags/github-main", f"{PUSH} origin :github-main",
    f"{PUSH} --force origin github-main:github-main", f"{PUSH} origin +github-main:github-main",
    f"{PUSH} origin $(git rev-parse HEAD):main", f"cd /tmp && {PUSH} origin github-main:main",
])
def test_other_push_forms_are_denied_even_with_a_receipt(publication, command):
    guard, _, receipts, head = publication
    _receipt(receipts, head)
    assert guard.decide("Bash", {"command": command})[0] == "deny"
