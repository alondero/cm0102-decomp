"""Post recorded ASCII text/backspaces to an explicit disposable GDI PID."""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import sys
import time
from gdi_interact import user32
from prepare_runtime import ROOT, digest
from replay_runtime_guard import VerifiedGame, tool_pins

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--text', required=True)
parser.add_argument('--backspaces', type=int, default=0)
parser.add_argument('--delay', type=float, default=0.12)
parser.add_argument('--record', type=Path, required=True)
parser.add_argument('--pid', type=int, required=True)
parser.add_argument('--runtime', type=Path, required=True)
args = parser.parse_args()
record = args.record.resolve()
if not record.is_relative_to(ROOT / 'CM0102/Runtime') or record.exists():
    parser.error('record must be a new file beneath CM0102/Runtime')
if not args.text.isascii() or not 0 <= args.backspaces <= 512 or not 0.03 <= args.delay <= 1:
    parser.error('ASCII text, 0..512 backspaces and delay 0.03..1 required')
record.parent.mkdir(parents=True, exist_ok=True)
characters = [ord(c) for c in args.text]
with VerifiedGame(args.pid, args.runtime) as game:
    data = dict(recorded_utc=datetime.now(timezone.utc).isoformat(), command=sys.argv, python=sys.version,
        script_sha256=digest(Path(__file__)), tools=tool_pins(), process=game.identity, events=[],
        characters=characters, backspace_keys=args.backspaces, delay=args.delay, status='incomplete',
        mechanism='VK_BACK keydown/up then WM_CHAR text; verify resulting field/screen separately')

    def post(message, wparam, lparam, delay):
        game.check()
        succeeded = bool(user32.PostMessageW(game.hwnd, message, wparam, lparam))
        data['events'].append(dict(recorded_utc=datetime.now(timezone.utc).isoformat(),
            message=hex(message), wparam=wparam, lparam=lparam, posted=succeeded))
        if not succeeded:
            raise RuntimeError('Text/key message rejected')
        time.sleep(delay)

    try:
        for _ in range(args.backspaces):
            post(0x100, 8, 0x000e0001, args.delay / 2)
            post(0x101, 8, 0xc00e0001, args.delay / 2)
        for character in characters:
            post(0x102, character, 1, args.delay)
        game.check()
        data['status'] = 'pass'
    except BaseException as error:
        data['error'] = repr(error)
        raise
    finally:
        record.write_text(json.dumps(data, indent=2) + '\n', encoding='utf-8')
print('Posted', args.backspaces, 'backspace keys and', len(characters), 'characters to verified game')
