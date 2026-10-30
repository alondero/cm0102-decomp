"""Generate one fresh guest copy/launch experiment; never reuse a changed game directory."""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import shutil
import sys
from prepare_runtime import ROOT, digest, SHA256, GDI_SHA256


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--name', required=True)
    parser.add_argument('--fullscreen', action='store_true')
    parser.add_argument('--fresh-settings', action='store_true', help='Preserve then remove copied game.cfg before launch')
    args = parser.parse_args()
    if not args.name.isascii() or not args.name.replace('-', '').isalnum() or len(args.name) > 48:
        parser.error('ASCII letters/digits/hyphens name of at most 48 characters required')
    area = ROOT / 'CM0102/Runtime'
    source = area / 'primary-input-20261003-a'
    out = area / ('vm-launch-preparation-' + args.name)
    results = area / 'primary-vm-guest-results-a' / args.name
    ps_path = source / (args.name + '.ps1')
    bat_path = source / (args.name + '.cmd')
    if any(p.exists() for p in (out, results, ps_path, bat_path)):
        parser.error('Fresh experiment name required')
    for filename, expected in [('cm0102.exe', SHA256), ('cm0102_GDI.exe', GDI_SHA256)]:
        assert digest(source / filename) == expected
    old_record = json.loads((area / 'primary-vm-guest-preparation-a/preparation.json').read_text(encoding='utf-8'))
    for row in old_record['inputs']:
        assert digest(source / row['path']) == row['sha256']
    template = source / 'verify-guest-b.ps1'
    body = template.read_text(encoding='utf-8')
    assert body.count('C:\\CM0102-reference-20261003-a') == 1 and body.count('verification-b') == 1
    guest_root = 'C:\\CM0102-' + args.name
    body = body.replace('C:\\CM0102-reference-20261003-a', guest_root).replace('verification-b', args.name)
    if args.fullscreen:
        body = body.replace('-windowed -seed 1', '-seed 1')
    # Retain the exact configuration presented to this launch before the original may change it.
    body = body.replace('  $process = Start-Process', "  Copy-Item (Join-Path $root 'game.cfg') (Join-Path $output 'game-before.cfg')\n  $process = Start-Process")
    if args.fresh_settings:
        body = body.replace('  $process = Start-Process', "  Remove-Item -LiteralPath (Join-Path $root 'game.cfg')\n  $process = Start-Process")
    ps_path.write_text(body, encoding='utf-8')
    bat = ('@echo off\nset "CMREF=' + guest_root + '"\n'
        'if exist "%CMREF%" exit /b 19\nmkdir "%CMREF%"\n'
        'xcopy "\\\\vboxsvr\\cmprimary\\*" "%CMREF%\\" /E /I /H /Y > "\\\\vboxsvr\\cmout\\' + args.name + '\\copy.txt" 2>&1\n'
        'if errorlevel 1 exit /b 20\n'
        'powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%CMREF%\\' + args.name + '.ps1" > "\\\\vboxsvr\\cmout\\' + args.name + '\\setup-output.txt" 2>&1\n'
        'exit /b %errorlevel%\n')
    bat_path.write_text(bat.replace('\n', '\r\n'), encoding='ascii')
    out.mkdir()
    results.mkdir()
    for p in (template, ps_path, bat_path, source / 'guest-inputs.tsv', Path(__file__)):
        shutil.copyfile(p, out / p.name)
    record = dict(status='prepared_only', command=sys.argv, python=sys.version,
        recorded_utc=datetime.now(timezone.utc).isoformat(), script_sha256=digest(Path(__file__)),
        support_sha256=digest(ROOT / 'scripts/prepare_runtime.py'), guest_root=guest_root,
        original_input_record_sha256=digest(area / 'primary-vm-guest-preparation-a/preparation.json'),
        source_used={p.name:digest(out / p.name) for p in (template, ps_path, bat_path, source / 'guest-inputs.tsv', Path(__file__))},
        command_to_request='Q:\\' + args.name + '.cmd', arguments='-seed 1' if args.fullscreen else '-windowed -seed 1',
        copied_configuration_removed_after_capture=args.fresh_settings,
        input_count=len(old_record['inputs']), scope='Prepared launch only; copied inputs must pass guest-side verification')
    (out / 'preparation.json').write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    print('Prepared fresh guest experiment:', record['command_to_request'])


if __name__ == '__main__':
    main()
