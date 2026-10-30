"""Audit the isolated clone and source restoration before any guest boot."""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET
from prepare_runtime import ROOT, digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--preparation', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    prep, out = args.preparation.resolve(), args.output.resolve()
    area = ROOT / 'CM0102/Runtime'
    if not prep.is_relative_to(area) or out == area or not out.is_relative_to(area):
        parser.error('Local Runtime evidence required')
    out.mkdir(exist_ok=False)
    record = json.loads(prep.read_text(encoding='utf-8'))
    restore = prep.parent / 'source-config-restoration.json'
    restoration = json.loads(restore.read_text(encoding='utf-8'))
    assert restoration['original_preparation_sha256'] == digest(prep)
    assert digest(Path(record['source_config'])) == record['source_config_sha256']
    assert digest(Path(record['source_disk'])) == record['source_disk_sha256']
    assert digest(prep.parent / 'source-used/prepare_reference_vm.py') == record['script_sha256']
    tool = Path(record['virtualbox'])
    assert digest(tool) == record['virtualbox_sha256']
    command = [str(tool), 'showvminfo', record['clone_name'], '--machinereadable']
    info = subprocess.check_output(command, text=True)
    values = {}
    for line in info.splitlines():
        if '=' in line:
            key, value = line.split('=', 1)
            values[key.strip('"')] = value.strip('"').replace('\\\\', '\\')
    assert values['VMState'] == 'poweroff' and values['cpus'] == '1'
    assert all(values[f'nic{i}'] == 'none' for i in range(1, 9))
    assert values['clipboard'] == values['draganddrop'] == 'disabled' and values['vrde'] == 'off'
    assert values['audio'] == 'none' and values['ehci'] == 'off'
    config, disk = Path(values['CfgFile']), Path(values['SATA-0-0'])
    assert config.resolve().is_relative_to(prep.parent) and disk.resolve().is_relative_to(prep.parent)
    assert digest(disk) == record['clone_disk_initial_sha256']
    ns = {'v': 'http://www.virtualbox.org/'}
    tree = ET.parse(config)
    shares = tree.findall('.//v:SharedFolder', ns)
    assert len(shares) == 1 and shares[0].attrib['name'] == 'cmref'
    assert shares[0].attrib['writable'] == 'false'
    assert Path(shares[0].attrib['hostPath']).resolve() == ROOT / record['shared_runtime']
    assert values['SATA-1-0'] == str(ROOT / 'CM0102/InstallMedia/CM0102.iso')
    assert digest(Path(values['SATA-1-0'])) == record['iso_sha256']
    (out / 'clone-info.txt').write_text(info, encoding='utf-8')
    shutil.copyfile(config, out / 'clone-config-before-boot.vbox')
    # Source config checked again after the clone-only metadata query.
    assert digest(Path(record['source_config'])) == record['source_config_sha256']
    result = dict(status='pass_before_boot', command=sys.argv, python=sys.version,
        script_sha256=digest(Path(__file__)), support_sha256=digest(ROOT / 'scripts/prepare_runtime.py'),
        preparation_sha256=digest(prep), restoration_sha256=digest(restore),
        virtualbox_sha256=digest(tool), metadata_command=command, clone_uuid=values['UUID'],
        clone_name=record['clone_name'], clone_info_sha256=digest(out / 'clone-info.txt'),
        clone_config_sha256=digest(config), clone_disk_sha256=digest(disk),
        source_config_sha256=digest(Path(record['source_config'])), source_disk_sha256=record['source_disk_sha256'],
        checks=['Source primary configuration restored byte-for-byte; source disk freshly hash-checked',
            'Source-used preparation preserved and failed report unchanged',
            'Clone powered off, all NICs disabled, clipboard/drag/drop/VRDE disabled',
            'One read-only share of disposable runtime; supplied ISO attached; one vCPU'],
        scope='Isolated clone approved by checks for experiment; no guest/game runtime acceptance')
    (out / 'audit.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print('PASS isolated VM/source restoration audit before boot')


if __name__ == '__main__':
    main()
