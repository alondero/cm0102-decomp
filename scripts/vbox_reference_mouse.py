"""Record a mouse click on the audited, isolated reference clone."""
import argparse
from datetime import datetime, timezone
import importlib.metadata
import json
from pathlib import Path
import subprocess
import sys
import time

import win32com.client
import pythoncom
import pywintypes
from prepare_runtime import ROOT, digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--audit', type=Path, required=True)
    parser.add_argument('--record', type=Path, required=True)
    parser.add_argument('--x', type=int, required=True, help='Zero-based screenshot pixel')
    parser.add_argument('--y', type=int, required=True)
    args = parser.parse_args()
    audit_path, out = args.audit.resolve(), args.record.resolve()
    area = ROOT / 'CM0102/Runtime'
    if not audit_path.is_relative_to(area) or not out.is_relative_to(area) or out.exists():
        parser.error('Local audit and fresh Runtime record required')
    audit = json.loads(audit_path.read_text(encoding='utf-8'))
    assert audit['status'] == 'pass_before_boot'
    tool = Path('C:/Program Files/Oracle/VirtualBox/VBoxManage.exe')
    assert digest(tool) == audit['virtualbox_sha256']
    query = [str(tool), 'showvminfo', audit['clone_uuid'], '--machinereadable']
    info = subprocess.check_output(query, text=True)
    values = {}
    for line in info.splitlines():
        if '=' in line:
            key, value = line.split('=', 1)
            values[key.strip('"')] = value.strip('"').replace('\\\\', '\\')
    assert values['UUID'] == audit['clone_uuid'] and values['name'] == audit['clone_name']
    assert values['VMState'] == 'running' and Path(values['CfgFile']).resolve().is_relative_to(area)
    assert all(values[f'nic{i}'] == 'none' for i in range(1, 9))
    assert values['clipboard'] == values['draganddrop'] == 'disabled' and values['vrde'] == 'off'
    record = dict(status='incomplete', command=sys.argv, python=sys.version,
        python_executable_sha256=digest(Path(sys.executable)), pywin32_version=importlib.metadata.version('pywin32'),
        modules={module.__name__:dict(path=module.__file__, sha256=digest(Path(module.__file__)))
            for module in (win32com.client, pythoncom, pywintypes)},
        script_sha256=digest(Path(__file__)), support_sha256=digest(ROOT / 'scripts/prepare_runtime.py'),
        audit_sha256=digest(audit_path), virtualbox_sha256=digest(tool), clone_uuid=audit['clone_uuid'],
        metadata_command=query, started_utc=datetime.now(timezone.utc).isoformat(), events=[],
        coordinate_convention='Zero-based screenshot pixels converted to one-based VirtualBox pixels',
        scope='Guest mouse requests; no assertion that the game consumed them')
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    session = None
    locked = False
    pressed = False
    try:
        vbox = win32com.client.Dispatch('VirtualBox.VirtualBox')
        machine = vbox.FindMachine(audit['clone_uuid'])
        assert machine.Name == audit['clone_name']
        session = win32com.client.Dispatch('VirtualBox.Session')
        machine.LockMachine(session, 1)  # LockType_Shared; do not edit VM settings.
        locked = True
        width, height, depth, ox, oy, status = session.Console.Display.GetScreenResolution(0)
        record['display'] = dict(width=width, height=height, bits_per_pixel=depth, origin_x=ox, origin_y=oy, status=status)
        assert 0 <= args.x < width and 0 <= args.y < height
        assert session.Console.Mouse.AbsoluteSupported
        for buttons in (0, 1, 0):
            event = dict(x=args.x + 1, y=args.y + 1, buttons=buttons, status='requested',
                requested_utc=datetime.now(timezone.utc).isoformat())
            record['events'].append(event)
            if buttons == 1:
                pressed = True  # A failed/interrupting COM return may still have delivered the press.
            session.Console.Mouse.PutMouseEventAbsolute(args.x + 1, args.y + 1, 0, 0, buttons)
            pressed = buttons == 1
            event.update(status='returned', returned_utc=datetime.now(timezone.utc).isoformat())
            time.sleep(0.12)
        record['status'] = 'pass'
    except BaseException as error:
        record['error'] = repr(error)
        raise
    finally:
        if locked:
            try:
                if pressed:
                    event = dict(x=args.x + 1, y=args.y + 1, buttons=0, cleanup=True, status='requested',
                        requested_utc=datetime.now(timezone.utc).isoformat())
                    record['events'].append(event)
                    session.Console.Mouse.PutMouseEventAbsolute(args.x + 1, args.y + 1, 0, 0, 0)
                    event.update(status='returned', returned_utc=datetime.now(timezone.utc).isoformat())
            except BaseException as error:
                record['release_error'] = repr(error)
                record['status'] = 'incomplete'
            try:
                session.UnlockMachine()
            except BaseException as error:
                record['unlock_error'] = repr(error)
                record['status'] = 'incomplete'
        record['finished_utc'] = datetime.now(timezone.utc).isoformat()
        out.write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    print('Recorded reference VM click:', record['status'])
    if record['status'] != 'pass':
        raise SystemExit(1)


if __name__ == '__main__':
    main()
