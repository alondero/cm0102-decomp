"""Post one recorded client-coordinate click to an explicit disposable GDI PID."""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import sys
import time
from gdi_interact import client_size, user32
from prepare_runtime import ROOT, digest
from replay_runtime_guard import VerifiedGame, tool_pins

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--x', type=int, required=True)
parser.add_argument('--y', type=int, required=True)
parser.add_argument('--record', type=Path, required=True)
parser.add_argument('--pid', type=int, required=True)
parser.add_argument('--runtime', type=Path, required=True)
args = parser.parse_args()
record = args.record.resolve()
if not record.is_relative_to(ROOT / 'CM0102/Runtime') or record.exists():
    parser.error('record must be a new file beneath CM0102/Runtime')
record.parent.mkdir(parents=True, exist_ok=True)
with VerifiedGame(args.pid, args.runtime) as game:
    width, height = client_size(game.hwnd)
    if not 0 <= args.x < width or not 0 <= args.y < height:
        parser.error('click outside game client')
    position = args.x | (args.y << 16)
    data = dict(recorded_utc=datetime.now(timezone.utc).isoformat(), command=sys.argv,
        python=sys.version, script_sha256=digest(Path(__file__)), tools=tool_pins(),
        process=game.identity, client=[width, height], position=[args.x, args.y], events=[],
        mechanism='Posted messages only; verify resulting screen separately', status='incomplete')
    try:
        for message, wparam in ((0x200, 0), (0x201, 1), (0x202, 0)):
            game.check()
            succeeded = bool(user32.PostMessageW(game.hwnd, message, wparam, position))
            data['events'].append(dict(recorded_utc=datetime.now(timezone.utc).isoformat(),
                message=f'0x{message:04x}', wparam=wparam, lparam=position, posted=succeeded))
            if not succeeded:
                raise RuntimeError('Mouse message rejected')
            time.sleep(0.1)
        game.check()
        data['status'] = 'pass'
    except BaseException as error:
        data['error'] = repr(error)
        raise
    finally:
        record.write_text(json.dumps(data, indent=2) + '\n', encoding='utf-8')
print('Posted one verified client click', args.x, args.y)
