"""Hash-locked, fresh Ghidra analysis; never writes to the preserved input."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shutil
import struct
import subprocess
import sys
import zipfile

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / 'CM0102/Game/cm0102.exe'
SHA256 = '0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86'
ARCHIVE_SHA256 = 'ddac49f903da9d5bac833e5cc79395098b9c33cfd3279be5f31bd00387d2d4db'

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True, help='New directory; existing paths are refused')
    parser.add_argument('--ghidra', type=Path, default=ROOT / 'tools/ghidra/ghidra_12.1.4_PUBLIC')
    parser.add_argument('--java-home', type=Path, default=Path('C:/Program Files/Java/jdk-21.0.10'))
    args = parser.parse_args()
    if digest(TARGET) != SHA256:
        raise SystemExit('Input hash mismatch; refusing analysis')
    ghidra = args.ghidra.resolve()
    if ghidra.name != 'ghidra_12.1.4_PUBLIC':
        raise SystemExit('Ghidra directory must retain the pinned archive root name: ghidra_12.1.4_PUBLIC')
    archive = ghidra.parent / 'ghidra_12.1.4_PUBLIC_20260921.zip'
    if not archive.is_file() or digest(archive) != ARCHIVE_SHA256:
        raise SystemExit('Pinned Ghidra archive missing or hash mismatch')
    # Verify the distribution actually executed, not just the retained archive.
    installation = hashlib.sha256()
    with zipfile.ZipFile(archive) as bundle:
        for member in sorted(bundle.infolist(), key=lambda item: item.filename):
            if member.is_dir():
                continue
            installed = ghidra.parent / member.filename
            expected = hashlib.sha256(bundle.read(member)).hexdigest()
            if not installed.is_file() or digest(installed) != expected:
                raise SystemExit(f'Ghidra installation mismatch: {member.filename}')
            installation.update((member.filename + '\0' + expected + '\n').encode())
    properties = (ghidra / 'Ghidra/application.properties').read_text()
    if 'application.version=12.1.4' not in properties:
        raise SystemExit('Expected Ghidra 12.1.4')
    launcher = ghidra / 'support/analyzeHeadless.bat'
    java = args.java_home.resolve() / 'bin/java.exe'
    if not launcher.is_file() or not java.is_file():
        raise SystemExit('Ghidra extraction incomplete or Java executable missing')
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    copied = out / 'cm0102.exe'
    shutil.copyfile(TARGET, copied)
    if digest(copied) != SHA256:
        raise SystemExit('Copy verification failed')
    exports = out / 'exports'
    exports.mkdir()
    data = copied.read_bytes()
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    sections_count = struct.unpack_from('<H', data, pe + 6)[0]
    optional_size = struct.unpack_from('<H', data, pe + 20)[0]
    optional = pe + 24
    image_base = struct.unpack_from('<I', data, optional + 28)[0]
    entry = struct.unpack_from('<I', data, optional + 16)[0]
    sections = []
    for index in range(sections_count):
        pos = optional + optional_size + index * 40
        name, vsize, rva, size, offset = struct.unpack_from('<8sIIII', data, pos)
        sections.append(dict(name=name.rstrip(b'\0').decode('ascii'), rva=rva, va=image_base+rva,
                             virtual_size=vsize, file_offset=offset, raw_size=size))
    (exports / 'pe.json').write_text(json.dumps(dict(sha256=SHA256, image_base=image_base,
        entry_rva=entry, entry_va=image_base+entry, sections=sections), indent=2)+'\n')
    def address(offset):
        for s in sections:
            if s['file_offset'] <= offset < s['file_offset'] + s['raw_size']:
                rva = s['rva'] + offset - s['file_offset']
                return f'0x{rva:08x}\t0x{image_base+rva:08x}'
        return '\t'
    with (exports / 'raw-strings.tsv').open('w', encoding='utf-8', newline='\n') as stream:
        stream.write('file_offset\trva\tva\tencoding\tvalue_json\n')
        for encoding, pattern in [('ascii', rb'[\x20-\x7e]{5,}'), ('utf-16le', rb'(?:[\x20-\x7e]\x00){5,}')]:
            for match in re.finditer(pattern, data):
                stream.write(f'0x{match.start():08x}\t{address(match.start())}\t{encoding}\t{json.dumps(match.group().decode(encoding))}\n')
    env = os.environ.copy()
    env['JAVA_HOME'] = str(args.java_home.resolve())
    command = [str(launcher), str(out), 'cm0102',
               '-import', str(copied), '-processor', 'x86:LE:32:default', '-cspec', 'windows',
               '-scriptPath', str(ROOT / 'scripts/ghidra'), '-postScript', 'ExportBaseline.java', str(exports)]
    record = dict(input=str(TARGET.relative_to(ROOT)), input_sha256=SHA256, copy_sha256=digest(copied),
                  ghidra_version='12.1.4', archive_sha256=ARCHIVE_SHA256,
                  distribution_manifest_sha256=installation.hexdigest(),
                  python=sys.version, platform=platform.platform(), command=command,
                  java=subprocess.run([str(java), '-version'], capture_output=True, text=True).stderr,
                  scripts={str(p.relative_to(ROOT)):digest(p) for p in [Path(__file__).resolve(), ROOT/'scripts/ghidra/ExportBaseline.java']})
    with (out / 'headless.log').open('w', encoding='utf-8') as log:
        result = subprocess.run(command, env=env, stdout=log, stderr=subprocess.STDOUT)
    record['exit_code'] = result.returncode
    record['preserved_input_sha256_after'] = digest(TARGET)
    record['scripts_unchanged'] = all(digest(ROOT / name) == value for name, value in record['scripts'].items())
    record['outputs'] = {p.name:digest(p) for p in sorted(exports.iterdir())}
    (out / 'run.json').write_text(json.dumps(record, indent=2)+'\n')
    required = ['analysis-options.tsv', 'pe.json', 'raw-strings.tsv', 'program.txt', 'memory.tsv', 'defined-strings.tsv',
                'functions.tsv', 'imports.tsv', 'startup-disassembly.tsv', 'startup-references.tsv'] + [f'startup-{v}.c' for v in ['947a13','68d370','5cc6d0','5cc6b0','68d520','5cc7d0']]
    if result.returncode or not record['scripts_unchanged'] or digest(TARGET) != SHA256 or any(not (exports/p).is_file() for p in required):
        raise SystemExit(f'Analysis failed; inspect {out / "headless.log"}')
    print(f'Analysis complete: {out}')

if __name__ == '__main__':
    main()
