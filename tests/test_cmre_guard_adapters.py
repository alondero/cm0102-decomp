"""The policy evaluator understands both clients; scope authorization is explicit."""
import json

from cmre import guard


def test_codex_shell_payload_gets_same_preservation_rules():
    assert guard.decide('exec_command', {'cmd': 'git push origin main'})[0] == 'deny'
    assert guard.decide('exec_command', {'cmd': 'py -3.13 -m cmre status'})[0] == 'allow'


def test_patch_checks_every_destination():
    patch = ('*** Begin Patch\n*** Update File: recovery/c/fn_004671b0.c\n'
             '@@\n-a\n+b\n*** Add File: recovery/ledger/fn_004671b0.json\n+{}\n*** End Patch')
    assert guard.decide('apply_patch', {'patch': patch})[0] == 'deny'


def test_move_destination_cannot_escape_policy():
    patch = ('*** Begin Patch\n*** Update File: recovery/c/fn_004671b0.c\n'
             '*** Move to: CM0102/Game/overwrite.c\n@@\n-a\n+b\n*** End Patch')
    assert guard.decide('apply_patch', {'input': patch})[0] == 'deny'


def test_explicit_scope_avoids_repeat_approval_without_weakening_preservation(monkeypatch):
    monkeypatch.setenv('CMRE_AUTHORIZED_PATHS', json.dumps(['cmre/', 'tests/']))
    assert guard.decide('Edit', {'file_path': 'cmre/difftest.py'})[0] == 'allow'
    assert guard.decide('Edit', {'file_path': 'pytest.ini'})[0] == 'ask'
    monkeypatch.setenv('CMRE_AUTHORIZED_PATHS', json.dumps(['recovery/ledger/']))
    assert guard.decide('Edit', {'file_path': 'recovery/ledger/fn_x.json'})[0] == 'deny'


def test_git_mutations_do_not_exempt_protected_paths_or_compound_commands():
    assert guard.decide('exec_command', {'cmd': 'git rm recovery/ledger/fn_x.json'})[0] == 'deny'
    assert guard.decide('exec_command', {'cmd': 'git status; Remove-Item CM0102/Game/x'})[0] == 'deny'
    assert guard.decide('exec_command', {'cmd': 'git restore cmre/queue.py; Remove-Item CM0102/Game/x'})[0] == 'deny'
    assert guard.decide('exec_command', {'cmd': 'git diff -- recovery/ledger/'})[0] == 'allow'
