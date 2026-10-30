"""Clone a powered-off reference VM without changing its disk/config; isolate the clone."""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import shutil
import subprocess
import sys
from prepare_runtime import ROOT, SHA256, GDI_SHA256, digest

VBOX = Path('C:/Program Files/Oracle/VirtualBox/VBoxManage.exe')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', required=True, help='Existing powered-off VM UUID')
    parser.add_argument('--name', required=True, help='New clone name')
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out, runtime = args.output.resolve(), args.runtime.resolve()
    local = ROOT / 'CM0102/Runtime'
    if out == local or not out.is_relative_to(local) or runtime == local or not runtime.is_relative_to(local):
        parser.error('Output and shared runtime must remain beneath Runtime')
    if not args.name.isascii() or not args.name.replace('-', '').isalnum():
        parser.error('Clone name must be ASCII letters/digits/hyphens')
    if out.exists():
        parser.error('Refusing existing output')
    if digest(runtime / 'cm0102.exe') != SHA256 or digest(runtime / 'cm0102_GDI.exe') != GDI_SHA256:
        raise ValueError('Runtime executables must be preserved unpatched versions')
    info = subprocess.check_output([str(VBOX), 'showvminfo', args.source, '--machinereadable'], text=True)
    values = {}
    for line in info.splitlines():
        if '=' in line:
            key, value = line.split('=', 1)
            values[key.strip('"')] = value.strip('"').replace('\\\\', '\\')
    if values['VMState'] != 'poweroff':
        raise ValueError('Source VM must be powered off; do not clone changing disk state')
    config, disk = Path(values['CfgFile']), Path(values['SATA-0-0'])
    iso = ROOT / 'CM0102/InstallMedia/CM0102.iso'
    if not iso.is_file():
        raise ValueError('Supplied ISO missing')
    out.mkdir()
    (out / 'source-vm-info.txt').write_text(info, encoding='utf-8')
    shutil.copyfile(config, out / 'source-vm-config.vbox')
    record = dict(status='incomplete', command=sys.argv, python=sys.version,
        recorded_utc=datetime.now(timezone.utc).isoformat(), script_sha256=digest(Path(__file__)),
        support_sha256=digest(ROOT / 'scripts/prepare_runtime.py'), source_uuid=args.source,
        clone_name=args.name, source_config=str(config), source_config_sha256=digest(config),
        source_disk=str(disk), source_disk_sha256=digest(disk), iso_sha256=digest(iso),
        virtualbox=str(VBOX), virtualbox_sha256=digest(VBOX),
        virtualbox_version=subprocess.check_output([str(VBOX), '--version'], text=True).strip(),
        source_vm_info_sha256=digest(out / 'source-vm-info.txt'),
        shared_runtime=runtime.relative_to(ROOT).as_posix(),
        primary_sha256=SHA256, gdi_sha256=GDI_SHA256, commands=[])

    def command(*arguments):
        row = dict(command=[str(VBOX), *arguments], started_utc=datetime.now(timezone.utc).isoformat())
        record['commands'].append(row)
        result = subprocess.run(row['command'], capture_output=True, text=True)
        row.update(exit_code=result.returncode, stdout=result.stdout, stderr=result.stderr,
            finished_utc=datetime.now(timezone.utc).isoformat())
        (out / 'preparation.json').write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
        result.check_returncode()
        return result.stdout

    try:
        command('clonevm', args.source, '--name', args.name, '--basefolder', str(out), '--mode', 'machine', '--register')
        command('modifyvm', args.name, '--nic1', 'none', '--clipboard-mode', 'disabled',
            '--drag-and-drop', 'disabled', '--audio-enabled', 'off', '--cpus', '1', '--usb-ehci', 'off')
        command('sharedfolder', 'add', args.name, '--name', 'cmref', '--hostpath', str(runtime), '--readonly')
        command('storageattach', args.name, '--storagectl', 'SATA', '--port', '1', '--device', '0',
            '--type', 'dvddrive', '--medium', str(iso))
        clone_info = command('showvminfo', args.name, '--machinereadable')
        (out / 'clone-vm-info.txt').write_text(clone_info, encoding='utf-8')
        record['clone_vm_info_sha256'] = digest(out / 'clone-vm-info.txt')
        clone_values = {}
        for line in clone_info.splitlines():
            if '=' in line:
                key, value = line.split('=', 1)
                clone_values[key.strip('"')] = value.strip('"').replace('\\\\', '\\')
        clone_disk = Path(clone_values['SATA-0-0'])
        if not clone_disk.resolve().is_relative_to(out):
            raise ValueError('Clone disk escaped output directory')
        record['clone_disk_initial_path'] = str(clone_disk)
        record['clone_disk_initial_sha256'] = digest(clone_disk)
        record['status'] = 'pass'
        record['scope'] = 'Preparation only; clone registered but not started; original VM remains powered off'
    except BaseException as error:
        record['error'] = repr(error)
        raise
    finally:
        record['source_config_after_sha256'] = digest(config)
        record['source_disk_after_sha256'] = digest(disk)
        record['source_preservation'] = (
            record['source_config_after_sha256'] == record['source_config_sha256'] and
            record['source_disk_after_sha256'] == record['source_disk_sha256'])
        if not record['source_preservation']:
            record['status'] = 'source_preservation_failed'
        (out / 'preparation.json').write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    if not record['source_preservation']:
        raise ValueError('Source VM preservation check failed; do not boot before restoring/auditing source')
    print('PASS isolated reference VM preparation; source disk/config unchanged')


if __name__ == '__main__':
    main()
