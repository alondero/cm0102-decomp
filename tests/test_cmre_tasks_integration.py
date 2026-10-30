"""Real pinned-binary packet -> staged test -> acceptance in an isolated ledger."""
import json

from cmre import env, ledger, tasks


def test_real_packet_publication_preserves_source_and_dependency_identity(tmp_path, monkeypatch):
    va = 0x004671B0
    original_record = json.loads(ledger.record_path(va).read_text(encoding='utf-8'))
    # The fixture starts at tier 1 even after this function gains production C.
    original_record.pop('c', None)
    original_record['status']['decompiled'] = False
    original_lift = (env.LIFTED_DIR / f'fn_{va:08x}.S').read_bytes()
    contract = (env.C_DIR / 'contracts' / f'fn_{va:08x}.json').read_bytes()
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(tmp_path / 'writer.lock'))
    monkeypatch.delenv('CMRE_WRITER_TOKEN', raising=False)
    for name in ('LEDGER_DIR', 'LIFTED_DIR', 'BLOCKED_DIR', 'C_DIR', 'RUNS', 'ANALYSIS'):
        path = tmp_path / name.lower()
        path.mkdir()
        monkeypatch.setattr(env, name, path)
    ledger.record_path(va).write_text(json.dumps(original_record), encoding='utf-8')
    (env.LIFTED_DIR / f'fn_{va:08x}.S').write_bytes(original_lift)
    (env.C_DIR / 'contracts').mkdir()
    (env.C_DIR / 'contracts' / f'fn_{va:08x}.json').write_bytes(contract)
    packet = tasks.prepare(va)
    assert packet['abi']['analyst_contract']['claims']['calling_convention'] == 'thiscall'
    task_id = packet['task_id']
    claimed = tasks.claim(task_id, 'integration-test')
    source = env.ROOT / packet['candidate_path']
    source.write_text('#include "cmre.h"\n'
                      'CM_THISCALL void *cm_fn_004671b0(void *self) { return (void *)((u32)self + 0x20u); }\n',
                      encoding='utf-8', newline='\n')
    submitted = tasks.submit(task_id, 'integration-test', claimed['lease']['token'])
    assert submitted['status'] == 'submitted'
    assert not (env.C_DIR / f'fn_{va:08x}.c').exists()
    published = tasks.publish(task_id)
    assert published['status'] == 'published'
    record = json.loads(ledger.record_path(va).read_text(encoding='utf-8'))
    assert record['c']['source_sha256'] == env.digest(source)
    assert record['status']['decompiled'] is True
    assert record['status']['integrated'] is False
    for path, digest in record['c']['source_dependencies'].items():
        assert env.digest(env.ROOT / path) == digest
    assert tasks.publish(task_id)['status'] == 'published'
