"""Record one supported action against the audited disposable reference VM only."""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import subprocess
import sys
from prepare_runtime import ROOT, digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=('start', 'capture', 'text', 'scan', 'state', 'display'))
    parser.add_argument('--audit', type=Path, required=True)
    parser.add_argument('--record', type=Path, required=True)
    parser.add_argument('--image', type=Path)
    parser.add_argument('--text')
    parser.add_argument('--scancodes', nargs='+')
    parser.add_argument('--width', type=int)
    parser.add_argument('--height', type=int)
    parser.add_argument('--depth', type=int, choices=(16, 32))
    args = parser.parse_args()
    audit_path, record_path = args.audit.resolve(), args.record.resolve()
    area = ROOT / 'CM0102/Runtime'
    if not audit_path.is_relative_to(area) or not record_path.is_relative_to(area) or record_path.exists():
        parser.error('Local audit and fresh Runtime record required')
    audit = json.loads(audit_path.read_text(encoding='utf-8'))
    assert audit['status'] == 'pass_before_boot'
    tool = Path('C:/Program Files/Oracle/VirtualBox/VBoxManage.exe')
    assert digest(tool) == audit['virtualbox_sha256']
    vm = audit['clone_uuid']
    query = [str(tool), 'showvminfo', vm, '--machinereadable']
    info = subprocess.check_output(query, text=True)
    values = {}
    for line in info.splitlines():
        if '=' in line:
            key, value = line.split('=', 1)
            values[key.strip('"')] = value.strip('"').replace('\\\\', '\\')
    assert values['name'] == audit['clone_name'] and values['UUID'] == vm
    assert Path(values['CfgFile']).resolve().is_relative_to(area)
    assert all(values[f'nic{i}'] == 'none' for i in range(1, 9))
    assert values['clipboard'] == values['draganddrop'] == 'disabled' and values['vrde'] == 'off'
    if args.action == 'start':
        assert values['VMState'] == 'poweroff'
        command = [str(tool), 'startvm', vm, '--type', 'headless']
    elif args.action == 'state':
        command = query
    else:
        assert values['VMState'] == 'running'
        command = [str(tool), 'controlvm', vm]
        if args.action == 'capture':
            if args.image is None:
                parser.error('capture needs --image')
            image = args.image.resolve()
            if not image.is_relative_to(area) or image.exists():
                parser.error('Fresh Runtime image required')
            image.parent.mkdir(parents=True, exist_ok=True)
            command += ['screenshotpng', str(image)]
        elif args.action == 'text':
            if args.text is None or not args.text.isascii() or not 1 <= len(args.text) <= 1024:
                parser.error('ASCII text length 1..1024 required; do not use this logged action for passwords')
            command += ['keyboardputstring', args.text]
        elif args.action == 'scan':
            if not args.scancodes or len(args.scancodes) > 80:
                parser.error('scan needs 1..80 hex bytes')
            if any(len(v) != 2 or not 0 <= int(v, 16) <= 255 for v in args.scancodes):
                parser.error('Scan codes must be two-digit hex bytes')
            command += ['keyboardputscancode', *args.scancodes]
        else:
            if args.width is None or args.height is None or args.depth is None:
                parser.error('display needs --width, --height and --depth')
            if not 800 <= args.width <= 4096 or not 600 <= args.height <= 2160:
                parser.error('Display dimensions outside bounded experiment range')
            command += ['setvideomodehint', str(args.width), str(args.height), str(args.depth)]
    record_path.parent.mkdir(parents=True, exist_ok=True)
    record = dict(status='incomplete', command=sys.argv, python=sys.version,
        script_sha256=digest(Path(__file__)), support_sha256=digest(ROOT / 'scripts/prepare_runtime.py'),
        audit_sha256=digest(audit_path), clone_uuid=vm, virtualbox_sha256=digest(tool),
        metadata_command=query, pre_action_state=values['VMState'], action_command=command,
        started_utc=datetime.now(timezone.utc).isoformat())
    record_path.write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    try:
        result = subprocess.run(command, capture_output=True, text=True)
        record.update(exit_code=result.returncode, stdout=result.stdout, stderr=result.stderr,
            finished_utc=datetime.now(timezone.utc).isoformat())
        result.check_returncode()
        if args.action == 'capture':
            record['image_sha256'] = digest(image)
        record['status'] = 'pass'
    except BaseException as error:
        record['error'] = repr(error)
        raise
    finally:
        record_path.write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    print('Recorded VM action:', args.action, record['status'])


if __name__ == '__main__':
    main()
