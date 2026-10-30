"""Read-only verification of the separately accepted partial morphology slice.

Writes a fresh local audit; never promotes partial regions into full routines.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import platform
import re
import sys
from pathlib import Path

import capstone
import dump_rng_slice as pe

ROOT = Path(__file__).resolve().parents[1]
LEDGER = ROOT / 'recovery/lang/morph-partial-ledger.json'
EXPECTED_CASES = {
    'known_languages': 400, 'all_first_bytes': 1536, 'lengths': 512,
    'long_word_lengths': 16, 'ignored_properties': 6, 'shared_output_alias': 2,
    'dynamic_language_postprocess': 10, 'nonempty_shared_composition': 2,
    'upper_return_bits': 4, 'suffix_slot_after_upper': 1,
    'explicit_original_residual': 9, 'ten_neutral_dispatch': 1, 'eight_neutral_dispatch': 1,
    'six_neutral_dispatch': 1, 'two_neutral_dispatch': 1, 'four_neutral_dispatch': 1, 'three_neutral_dispatch': 1,
    'random_known_languages': 1000, 'ten_dictionary_and_qualifiers': 2208,
    'ten_all_first_bytes': 512, 'ten_all_last_bytes_and_endings': 1536,
    'ten_short_and_stem_endings': 60, 'ten_synthetic_ending_lists': 24,
    'ten_synthetic_dictionary': 16, 'ten_character_return_bits': 16,
    'ten_dynamic_language': 20, 'ten_word_lengths': 32,
    'ten_ignored_properties': 12, 'ten_shared_output_alias': 4,
    'ten_suffix_slot_after_character': 4, 'ten_random': 1000,
    'eight_dictionary_and_qualifiers': 2990, 'eight_article_property_bytes': 3072,
    'eight_all_first_bytes': 1024, 'eight_all_last_bytes_and_endings': 1536,
    'eight_short_and_stem_endings': 60, 'eight_ens_suffix': 55, 'eight_ens_lengths': 36,
    'eight_synthetic_ending_lists': 24, 'eight_synthetic_dictionary': 24,
    'eight_character_return_bits': 20, 'eight_dynamic_language': 21,
    'eight_ens_format_language': 2, 'eight_ens_nonempty_output': 2,
    'eight_suffix_slot_after_character': 4, 'eight_shared_output_alias': 4,
    'eight_random': 1000,
    'six_words_and_qualifiers': 180, 'six_all_first_bytes': 512,
    'six_all_last_bytes_and_endings': 1536, 'six_short_and_stem_endings': 60,
    'six_synthetic_ending_lists': 24, 'six_character_return_bits': 8,
    'six_dynamic_language': 12, 'six_word_lengths': 32, 'six_ignored_properties': 9,
    'six_shared_output_alias': 3, 'six_suffix_slot_after_character': 2,
    'six_nonempty_shared_output': 1, 'six_random': 1000,
    'two_prefix_property_bytes': 11776, 'two_prefix_first_bytes': 5888,
    'two_suffix_property_bytes': 16384, 'two_independent_properties': 828,
    'two_prefix_suffix_combinations': 9384, 'two_caller_prefix_search': 117,
    'two_unknown_qualifier_erasure': 16, 'two_qualifier_aliases': 24,
    'two_character_return_bits': 12, 'two_suffix_slot_after_character': 3,
    'two_properties_after_character': 3, 'two_find_return_and_property_mutation': 6,
    'two_dynamic_language': 21, 'two_stadium_format_language': 3,
    'two_stadium_nonempty_output': 3, 'two_word_lengths': 32,
    'two_shared_output_alias': 5, 'two_random': 1000,
    'four_prefix_property_bytes': 6656, 'four_suffix_property_bytes': 8192,
    'four_ban_signed_property': 512, 'four_first_bytes': 512,
    'four_independent_properties': 1053, 'four_prefix_suffix_combinations': 5400,
    'four_unknown_qualifier_erasure': 16, 'four_qualifier_aliases': 24,
    'four_character_return_bits': 8, 'four_suffix_slot_after_character': 2,
    'four_properties_after_character': 2, 'four_dynamic_language': 21,
    'four_nonempty_shared_output': 1, 'four_word_lengths': 32,
    'four_shared_output_alias': 5, 'four_random': 1000,
    'three_prefix_property_bytes': 48128, 'three_suffix_property_bytes': 58880,
    'three_all_first_bytes': 1536, 'three_s_selector': 2048,
    'three_independent_properties': 2350, 'three_dictionary_and_qualifiers': 1380,
    'three_synthetic_dictionary': 24, 'three_prefix_suffix_combinations': 1680,
    'three_qualifier_aliases': 36, 'three_character_return_bits': 40,
    'three_suffix_slot_after_character': 10, 'three_properties_after_character': 10,
    'three_captured_property_after_print': 144, 'three_dynamic_language': 25,
    'three_nonempty_shared_output': 25, 'three_word_lengths': 40,
    'three_shared_output_alias': 5, 'three_random': 1000,
    'nine_neutral_dispatch': 1, 'nine_prefix_properties': 8704,
    'nine_suffix_properties': 4608, 'nine_first_bytes': 512,
    'nine_combinations': 765, 'nine_aliases': 12, 'nine_return_bits': 8,
    'nine_suffix_slot': 2, 'nine_properties_after_character': 2,
    'nine_language_change': 16, 'nine_output_replacement': 16,
    'nine_lengths': 32, 'nine_random': 1000,
}


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read(path: Path):
    return json.loads(path.read_text(encoding='utf-8'))


def checked_report(record, source_root: Path, source_count=7):
    """Check an accepted run against its current or historically frozen source."""
    path = ROOT / record['report']
    assert digest(path) == record['sha256']
    report = read(path)
    assert report['status'] == 'pass' and report['quick'] is False
    assert report['optimization'] == record['optimization']
    assert report['input_sha256'] == pe.SHA256
    assert sum(report['cases'].values()) == record['cases']
    assert report['sources'] == record['source_hashes'] and len(report['sources']) == source_count
    for name, expected in report['sources'].items():
        assert digest(source_root / name) == expected, name
    for name, expected in report['artifacts'].items():
        assert digest(path.parent / name) == expected, name
    toolchain = Path(report['build_commands'][0][0]).parent
    for name, tool in report['tools'].items():
        assert digest(toolchain / name) == tool['sha256'], name
    for name, expected in report['evidence']['exports'].items():
        folder = ROOT / name
        assert digest(folder / 'run.json') == expected
        export = read(folder / 'run.json')
        assert export['input_sha256'] == pe.SHA256 and export['exit_code'] == 0
        for artifact, expected_artifact in export['outputs'].items():
            assert digest(folder / artifact) == expected_artifact
    return path, report


def checked_history(ledger):
    """Follow pinned earlier ledgers and validate reports against frozen source."""
    history = ledger.get('previous_acceptance')
    if not history:
        return []
    previous_path = ROOT / history['ledger']
    assert digest(previous_path) == history['ledger_sha256']
    archive_path = ROOT / history['archive_manifest']
    assert digest(archive_path) == history['archive_manifest_sha256']
    archive = read(archive_path)
    for name, expected in archive['files'].items():
        assert digest(archive_path.parent / name) == expected
    previous = read(previous_path)
    provenance = previous['acceptance_provenance']
    assert digest(ROOT / provenance['script']) == provenance['script_sha256']
    previous_snapshot = ROOT / previous['source_snapshot']['path']
    assert digest(previous_snapshot / 'manifest.json') == previous['source_snapshot']['manifest_sha256']
    previous_manifest = read(previous_snapshot / 'manifest.json')
    assert previous_manifest['status'] == 'pass'
    assert previous_manifest['reports'] == previous['validation_runs']
    assert len(previous['validation_runs']) == 2
    revision = previous.get('revision', 1)
    expected_cases = {1: 3505, 2: 8949, 3: 18823, 4: 22202, 5: 67707, 6: 91143, 7: 208504}[revision]
    optimizations = set()
    for record in previous['validation_runs']:
        _, previous_report = checked_report(record, previous_snapshot, source_count={1:7,2:7,3:9,4:9,5:10,6:11,7:12}[revision])
        assert record['cases'] == expected_cases
        assert previous_report['sources'] == previous_manifest['sources']
        optimizations.add(record['optimization'])
    assert optimizations == {'O0', 'O2'}
    assert previous['unchanged_full_ledgers'] == ledger['unchanged_full_ledgers']
    assert all(previous['parent'][k] is False for k in ('decompiled', 'compiled', 'validated', 'integrated'))
    assert previous['counted_as_full_routine'] is False
    return [dict(history, cases_per_build=expected_cases)] + checked_history(previous)


def checked_nine_adoption(ledger, current_report):
    """Retain candidate provenance without rerunning it against changed inputs."""
    record = ledger['nine_adoption']
    path = ROOT / record['record']
    assert digest(path) == record['sha256']
    adoption = read(path)
    assert adoption['input_sha256'] == pe.SHA256
    assert digest(ROOT / adoption['command'][0]) == adoption['script_sha256']
    assert adoption['prior_ledger_sha256'] == ledger['previous_acceptance']['ledger_sha256']
    previous = read(ROOT / ledger['previous_acceptance']['ledger'])
    old_snapshot = ROOT / previous['source_snapshot']['path']
    for name, identity in adoption['changes'].items():
        assert digest(ROOT / name) == identity['output']
        assert current_report['sources'][name] == identity['output']
        if identity['input'] is not None:
            assert digest(old_snapshot / name) == identity['input']
    candidate = ROOT / 'CM0102/Analysis/morph-nine-native-candidate-a'
    assert digest(candidate / 'report.json') == adoption['candidate_report_sha256']
    assert digest(candidate / 'body.inc') == adoption['candidate_body_sha256']
    assert digest(ROOT / 'CM0102/Analysis/test-morph-nine-candidate.py') == adoption['candidate_driver_sha256']
    def tokens(path):
        return re.sub(r'\s+', '', re.sub(r'/\*.*?\*/|//[^\n]*', '', path.read_text(), flags=re.S))
    assert tokens(candidate / 'body.inc') == tokens(ROOT / 'recovery/lang/cm_lang_morph_nine_body.inc')
    snapshot = ROOT / record['candidate_snapshot']
    assert digest(snapshot / 'manifest.json') == record['candidate_manifest_sha256']
    manifest = read(snapshot / 'manifest.json')
    assert manifest['status'] == 'pass' and len(manifest['sources']) == 16
    for name, identity in manifest['sources'].items():
        assert digest(snapshot / name) == identity
    audit_path = ROOT / 'CM0102/Analysis/morph-nine-candidate-audit-a/audit.json'
    assert digest(audit_path) == manifest['candidate_audit_sha256']
    audit = read(audit_path)
    assert audit['status'] == 'pass' and audit['accepted_into_current_ledger'] is False
    assert audit['sources'] == manifest['sources']
    assert digest(ROOT / audit['command'][0]) == audit['script_sha256']
    tool_reference = ROOT / audit['tool_reference']['report']
    assert digest(tool_reference) == audit['tool_reference']['sha256']
    assert read(tool_reference)['tools'] == audit['tools'] == current_report['tools']
    optimizations = set()
    for accepted, frozen in zip(audit['reports'], manifest['reports']):
        assert accepted['report'] == frozen['report'] and accepted['sha256'] == frozen['sha256']
        report_path = ROOT / frozen['report']
        assert digest(report_path) == frozen['sha256']
        report = read(report_path)
        assert report['status'] == 'pass' and report['input_sha256'] == pe.SHA256
        assert sum(report['cases'].values()) == accepted['cases'] == 15677
        assert report['sources'] == manifest['sources']
        assert report['optimization'] == accepted['optimization']
        optimizations.add(report['optimization'])
        for name, identity in report['artifacts'].items():
            assert digest(report_path.parent / name) == identity
    assert optimizations == {'O0', 'O2'}
    return record


def main():
    if not __debug__:
        raise SystemExit('Assertions required')
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    analysis = (ROOT / 'CM0102/Analysis').resolve()
    if out == analysis or not out.is_relative_to(analysis):
        parser.error('fresh output beneath Analysis required')
    out.mkdir(parents=True, exist_ok=False)
    audit = dict(status='incomplete', command=sys.argv, python=sys.version,
                 platform=platform.platform(), capstone=capstone.__version__,
                 script_sha256=digest(Path(__file__)), ledger_sha256=digest(LEDGER))
    try:
        assert digest(pe.TARGET) == digest(pe.FALLBACK) == pe.SHA256
        data, sections = pe.load_pe(pe.TARGET)
        ledger = read(LEDGER)
        assert ledger['input_sha256'] == pe.SHA256 and ledger['revision'] == 8
        parent = ledger['parent']
        assert parent['va'] == '0x006701f0' and parent['identified']
        assert all(parent[k] is False for k in ('decompiled', 'compiled', 'validated', 'integrated'))
        assert ledger['counted_as_full_routine'] is False
        provenance = ledger['acceptance_provenance']
        assert digest(ROOT / provenance['script']) == provenance['script_sha256']
        runs = []
        for record in ledger['validation_runs']:
            path, report = checked_report(record, ROOT, source_count=13)
            assert report['cases'] == EXPECTED_CASES
            assert record['cases'] == sum(EXPECTED_CASES.values())
            runs.append((path, report))
        assert len(runs) == 2
        assert {r['optimization'] for _, r in runs} == {'O0', 'O2'}
        first = runs[0][1]
        assert all(r['sources'] == first['sources'] and r['evidence'] == first['evidence'] for _, r in runs)
        assert ledger['original_regions'] == first['evidence']['regions']
        assert ledger['original_dispatch_tables'] == first['evidence']['tables']
        assert ledger['static_exports'] == first['evidence']['exports']
        assert [{k: r[k] for k in ('va', 'size', 'sha256')} for r in ledger['regions']] == ledger['original_regions']
        assert sum(r['size'] for r in ledger['original_regions']) == 49588
        assert len(ledger['regions']) == 21
        extents = sorted((int(r['va'], 16), int(r['va'], 16) + r['size'])
                         for r in ledger['original_regions'])
        assert all(end <= following for (_, end), (following, _) in zip(extents, extents[1:]))
        assert ledger['byte_accounting']['regional_instruction_bytes'] == 49588
        assert ledger['byte_accounting']['new_language_ten_instruction_bytes'] == 2171
        assert ledger['byte_accounting']['new_language_eight_instruction_bytes'] == 2738
        assert ledger['byte_accounting']['new_language_six_instruction_bytes'] == 1133
        assert ledger['byte_accounting']['new_language_two_instruction_bytes'] == 6622
        assert ledger['byte_accounting']['new_language_two_manual_instruction_bytes'] == 1678
        assert ledger['byte_accounting']['new_language_four_instruction_bytes'] == 5170
        assert ledger['byte_accounting']['new_language_four_manual_instruction_bytes'] == 2305
        assert ledger['byte_accounting']['new_language_three_instruction_bytes'] == 26973
        assert ledger['byte_accounting']['new_language_three_manual_instruction_bytes'] == 4897
        assert ledger['byte_accounting']['new_language_nine_instruction_bytes'] == 3536
        assert ledger['byte_accounting']['new_language_nine_manual_instruction_bytes'] == 1372
        support = ledger['recovered_support']
        assert support == first['evidence']['recovered_support']
        assert support['va'] == '0x006820a0' and support['size'] == 171
        assert digest(ROOT / support['ledger']) == support['ledger_sha256']
        helper = next(r for r in read(ROOT / support['ledger'])['functions'] if r['va'] == support['va'])
        assert helper['sha256'] == support['sha256'] and helper['validated'] and not helper['integrated']
        assert support['source'] in first['sources']
        helper_blob = pe.read_va(data, sections, int(support['va'], 16), support['size'])
        assert hashlib.sha256(helper_blob).hexdigest() == support['sha256']
        support_b = ledger['recovered_support_b']
        assert support_b == first['evidence']['recovered_support_b']
        assert support_b['va'] == '0x00682150' and support_b['size'] == 171
        assert digest(ROOT / support_b['ledger']) == support_b['ledger_sha256']
        helper_b = next(r for r in read(ROOT / support_b['ledger'])['functions'] if r['va'] == support_b['va'])
        assert helper_b['sha256'] == support_b['sha256'] and helper_b['validated'] and not helper_b['integrated']
        assert support_b['source'] in first['sources']
        helper_b_blob = pe.read_va(data, sections, int(support_b['va'], 16), support_b['size'])
        assert hashlib.sha256(helper_b_blob).hexdigest() == support_b['sha256']
        for region in ledger['original_regions']:
            blob = pe.read_va(data, sections, int(region['va'], 16), region['size'])
            assert hashlib.sha256(blob).hexdigest() == region['sha256']
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        instructions = read(runs[0][0].parent / 'region-instructions.json')
        assert instructions == read(runs[1][0].parent / 'region-instructions.json')
        assert sum(len(bytes.fromhex(i['bytes'])) for i in instructions) == 49588
        assert len(instructions) == 15886
        assert len({i['va'] for i in instructions}) == len(instructions)
        for instruction in instructions:
            va, blob = int(instruction['va'], 16), bytes.fromhex(instruction['bytes'])
            assert pe.read_va(data, sections, va, len(blob)) == blob
            decoded = list(md.disasm(blob, va))
            assert len(decoded) == 1 and decoded[0].size == len(blob)
            assert decoded[0].mnemonic + ' ' + decoded[0].op_str == instruction['instruction']
        exported = set()
        exported_bytes = 0
        export_path = ROOT / 'CM0102/Analysis/lang-loader-static-b/selected-disassembly.tsv'
        with export_path.open() as stream:
            for row in csv.DictReader(stream, delimiter='\t'):
                if int(row['function_va'], 16) == 0x6701F0:
                    exported.add(int(row['instruction_va'], 16))
                    exported_bytes += len(bytes.fromhex(row['bytes']))
        assert len(exported) == parent['ghidra_instruction_records'] == 17394
        assert exported_bytes == parent['ghidra_instruction_bytes'] == 49771
        assert all(i['ghidra_exported'] == (int(i['va'], 16) in exported) for i in instructions)
        manual = [i for i in instructions if not i['ghidra_exported']]
        assert manual == first['evidence']['manual_instructions']
        assert sum(len(bytes.fromhex(i['bytes'])) for i in manual) == 10359
        assert first['evidence']['manual_bytes'] == 10359
        assert len(manual) == 2342
        assert sum(len(bytes.fromhex(i['bytes'])) for i in manual
                   if 0x673777 <= int(i['va'], 16) < 0x674BA9) == 2305
        assert sum(len(bytes.fromhex(i['bytes'])) for i in manual
                   if 0x674BA9 <= int(i['va'], 16) < 0x67B506) == 4897
        assert sum(len(bytes.fromhex(i['bytes'])) for i in manual
                   if 0x67B506 <= int(i['va'], 16) < 0x67C2D6) == 1372
        assert ledger['byte_accounting']['manually_decoded_bytes_absent_from_ghidra_body'] == 10359
        assert ledger['static_inspections'] == first['evidence']['inspections']
        for name, expected in first['evidence']['inspections'].items():
            folder = ROOT / name
            assert digest(folder / 'report.json') == expected
            inspection = read(folder / 'report.json')
            assert inspection['input_sha256'] == pe.SHA256
            assert digest(ROOT / inspection['command'][0]) == inspection['script_sha256']
            for artifact, identity in inspection['artifacts'].items():
                assert digest(folder / artifact) == identity
        generated = ROOT / 'CM0102/Analysis/morph-two-data-a/suffix-descriptors.inc'
        assert digest(generated) == first['sources']['recovery/lang/cm_lang_morph_two_data.inc']
        generated_four = ROOT / 'CM0102/Analysis/morph-four-data-a/dispatch-descriptors.inc'
        assert digest(generated_four) == first['sources']['recovery/lang/cm_lang_morph_four_data.inc']
        generated_three = ROOT / 'CM0102/Analysis/morph-three-generated-d'
        generation = read(generated_three / 'report.json')
        assert generation['status'] == 'generated'
        assert digest(generated_three / 'body.inc') == first['sources']['recovery/lang/cm_lang_morph_three_body.inc']
        assert generation['static_report_sha256'] == digest(ROOT / 'CM0102/Analysis/morph-three-static-a/report.json')
        for name, identity in generation['semantic_inputs'].items():
            assert digest(ROOT / name) == identity
        lowering = read(generated_three / 'lowering.json')
        assert lowering['blocks'] == 626 and lowering['unmerged_blocks'] == 2898
        assert len(lowering['merged_blocks']) == 2272
        assert lowering['covered_records'] == 8729
        assert [(i['va'], i['instruction']) for i in lowering['uncovered']] == [
            ('0x674ba9', 'cmp eax, 3'), ('0x674bac', 'jne 0x67b506')]
        assert len(lowering['tables']) == 100
        for va, table in lowering['tables'].items():
            assert first['evidence']['tables'][f'0x{int(va, 16):08x}'] == table
        assert len(first['evidence']['tables']) == 225
        nine_adoption = checked_nine_adoption(ledger, first)
        for va, table in first['evidence']['tables'].items():
            blob = pe.read_va(data, sections, int(va, 16), table['size'])
            assert blob.hex() == table['bytes']
            assert hashlib.sha256(blob).hexdigest() == table['sha256']
        snapshot = ROOT / ledger['source_snapshot']['path']
        assert digest(snapshot / 'manifest.json') == ledger['source_snapshot']['manifest_sha256']
        manifest = read(snapshot / 'manifest.json')
        assert manifest['status'] == 'pass'
        assert manifest['sources'] == first['sources'] and manifest['reports'] == ledger['validation_runs']
        for name, expected in manifest['sources'].items():
            assert digest(snapshot / name) == expected, name
        history = checked_history(ledger)
        assert len(history) == 7
        entries = []
        assert len(ledger['unchanged_full_ledgers']) == 8
        for name, expected in ledger['unchanged_full_ledgers'].items():
            assert digest(ROOT / name) == expected, name
            entries.extend(read(ROOT / name)['functions'])
        assert len(entries) == 45
        assert len({int(e['va'], 16) for e in entries}) == 45
        assert sum(e.get('category', 'application') == 'application' for e in entries) == 44
        assert sum(e.get('category', '').startswith('CRT') for e in entries) == 1
        assert all(e['validated'] and not e['integrated'] for e in entries)
        assert all(all(r[k] for k in ('identified', 'decompiled', 'compiled', 'validated'))
                   and not r['integrated'] for r in ledger['regions'])
        assert digest(pe.TARGET) == digest(pe.FALLBACK) == pe.SHA256
        audit.update(status='pass', input_sha256=pe.SHA256,
                     accepted_runs=ledger['validation_runs'], cases_per_build=224181,
                     frozen_sources=13, regional_instruction_bytes=49588,
                     manual_instruction_bytes=10359,
                     nine_adoption=nine_adoption,
                     static_inspections=first['evidence']['inspections'],
                     historical_acceptance=history,
                     recovered_support=support, recovered_support_b=support_b,
                     unchanged_full_ledgers=ledger['unchanged_full_ledgers'],
                     full_application_entries=44, separate_crt_entries=1,
                     parent_status=parent, integrated_entries=0)
    except Exception as exc:
        audit['error'] = repr(exc)
        raise
    finally:
        (out / 'audit.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
    print(f'PASS: {out}; partial regions only, full parent remains unvalidated')


if __name__ == '__main__':
    main()
