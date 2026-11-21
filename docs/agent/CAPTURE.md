# Capture supply: open unlifted functions with real game inputs

Use this workflow when `cmre pipeline survey` shows most remaining bytes in the
`unlifted` band, or a function's lifted test is `low-coverage` with faults in
callees. Policy and limits are in
[ADR 0009](../adr/0009-captured-entry-inputs.md). Captures are inputs. They
never accept anything: the unchanged lifted gate and `cmre accept` do.

Terms: a **capture** is the registers, stack words and touched memory pages one
function received in the running original game. The **recorder** is the
debugger that takes captures. **Expansion** mutates stored captures offline to
reach more of a function.

## 1. Record (needs the game; one recorder at a time)

1. Mount `CM0102/InstallMedia/CM0102.iso`. Copy a prepared primary runtime
   (`CM0102/Runtime/primary-input-20261003-a`) to a fresh
   `CM0102/Runtime/host-capture-<date>/game` and start
   `cm0102.exe -windowed -seed 1` there. Never run or record the supplied
   `CM0102/Game` copy in place: the game writes into its folder.
2. Start the recorder before the activity you want covered. Database load and
   new-game creation run only once per process, so attach at the main menu:

   ```powershell
   py -3.13 -m cmre capture record --pid PID --seconds 1800 --status CM0102/Analysis/cmre/captures/SESSION.json
   ```

   With no function IDs it targets every inventory function without an accepted
   record. It refuses a process whose image is not the pinned executable at
   its preferred base, or whose `.text` differs from the file.
3. Play. Breadth matters more than length: start a new game, continue through
   several weeks, play and watch matches, make and answer transfer bids, change
   tactics and training, open every screen class once, save and load. A person
   playing for 30 minutes reaches far more code than scripted clicks.
   The recorder freezes the game for about 70 ms per hit; early stutter is
   expected and fades as functions reach their capture limit.
4. The recorder detaches at `--seconds`, or earlier once no new target function
   has been reached for `--plateau-minutes` (default 30; `0` disables), and
   leaves the game running. A plateau stop sets `stopped` in the summary: switch
   activity (a holiday saturates; an earlier round ran two seasons of holiday for
   little new code) and attach a new round with a fresh status path. Never kill
   the recorder: its entry breakpoints stay written into the game and the next
   hit crashes it. Read `SESSION.json`: `reached` and `reached_bytes` say what
   the session touched. On 21 Nov, 30 minutes of interactive play (a signing,
   a staff contract, a watched match) reached +37 functions and +231 KB, more than
   the preceding holiday hour.

Completion: a session summary with reached functions and bytes, and capture
files under `local/captures/`.

### Fullscreen and season-boundary capture

When a windowed client is black but its cursor/UI still responds, preserve the
process evidence and ask the owner for display intervention. On 2026-11-17,
the owner loaded a reference save and selected fullscreen in the unchanged
primary executable; rendering returned and holiday progression worked. This
is an observed workaround, not a diagnosis or a guarantee for other hosts.
Keep that working mode: take PID-scoped client screenshots without restoring,
repositioning or refocusing the window. Desktop driving requires the owner's
current permission and a bounded handover deadline.

For season-transition supply, load a suitable local reference save and let
holiday progression cross late July/early August without UI interruptions.
Keep the same game process across bounded, non-overlapping recording rounds,
with a fresh status/output path for each. Heavy transition processing or a
capture plateau is not by itself a hang; inspect the visible game and retained
progress before intervening. Entry captures do not establish every call or
replay native import outputs.

## 2. Expand and apply (no game needed; parallel)

During an active farm, name unaccepted targets explicitly and retain their
existing effective fixture/capture prefixes and execution policies:

```powershell
py -3.13 -m cmre capture expand fn_XXXXXXXX --jobs 4 --output CM0102/Analysis/cmre/captures/expand-N.json
py -3.13 -m cmre capture apply fn_XXXXXXXX --jobs 4 --output CM0102/Analysis/cmre/captures/apply-N.json
```

`expand` adds derived inputs only where the original executes new instructions
of the function. `apply` selects a covering set, writes it to the function's
hints as `captures` (removing `"auto": true`, which claims the hint), and runs
the unchanged lifted gate. Its summary lists `passing` functions and their
bytes. Both commands default to every function that has stored captures; name
`fn_XXXXXXXX` IDs to bound them. `expand` takes no lease. `apply` holds the
writer lease only while it writes each hint, waiting out a running session
check or accept: hints are declared test inputs, and a write during a check
makes its evidence inconclusive.

If greedy selection would replace an existing prefix, prepare a private
append-only proposal for review and supported hint application instead.
Accepted lifts need C-only additions; accepted C needs explicit parent input
maintenance and fresh identity-bound review. Additional stored captures alone
authorize neither changing an accepted corpus nor dropping a failing case.

`expand` first runs **comparison-guided** mutation (input-to-state): it traces
the operands of every `cmp`/`test` the original executes, finds the input
position that supplied each operand (the exact address for a memory operand;
registers, stack words and earlier input reads with the same value otherwise),
and writes the value that flips the comparison (the other operand, ±1, every
case of a small `switch` bound, or the tested bits set/cleared). Random edits
use the remaining iterations. Derived captures from this phase carry
`"strategy": "compare"`. Code, constants, harness stack/arena memory and import
slots are never edited. `scripts/cmre_lift_lane.py` keys each function on
`EXPANDER_VERSION` plus its capture set, so an expander upgrade re-runs every
captured, unrecovered function once.

By default `expand` changes only captured registers, stack words and memory
bytes. `--mutate-import-seed` opts into also mutating the u32
`import_seed`, which selects the seeded import-stub results for a case. This is
input-coverage exploration of import scaffolding, **not** native or runtime
evidence and not an observation of a real Windows/API result, captured memory or
game behaviour. The default is off and reproduces the previous mutation and RNG
sequence exactly. An enabled run propagates the chosen seed into the original
run, the saved derived capture and later mutations of that capture. A
**seed-mutated** derived capture keeps `"kind": "mutated"`, names its parent,
records the old and new seed in `edits`, and carries the explicit `scaffolding`
label. Derived captures from ordinary register, stack or memory edits keep their
existing metadata unchanged. The flag applies only to `expand` and changes no
capture format, difftest, emulator, import or gate behaviour.

Completion: `apply` output with a status for every function. `low-coverage`
after expansion means the session did not exercise enough of that function:
record a different activity. `fail` is a lift mismatch on a real input: keep
the report and triage it through [TRIAGE.md](TRIAGE.md); do not drop the
capture to make it pass.

## 3. Accept the lifts (parent, serial)

Accept each passing function with the normal command. This takes the writer
lease, so the parent runs it between recovery batches:

```powershell
py -3.13 -m cmre accept fn_XXXXXXXX
```

Accepted lifts enter `cmre next --c`; the running [farm](FARM.md) picks them up.
Their C tests replay the same captures.

Completion: accepted lifted records; `cmre pipeline survey` shows the bytes
moved from `unlifted` to `lifted`.

## Rules

- Captures hold original game memory. They stay in `local/captures/`, outside
  commits, packets sent to providers, and published evidence. A hint lists only
  their hashes.
- Keep `local/captures/` with the retained local evidence. A tier whose hints
  list a capture cannot be re-run without that file; a missing capture is an
  unavailable test, never a pass.
- Do not edit a capture, hand-write one, or delete one to change a result.
  Record or expand a new one.
- `cmre/capture.py`, `cmre/emu.py` and `cmre/difftest.py` are harness modules:
  changes follow [HARNESS.md](HARNESS.md). The recorder and expander
  (`capture_record.py`, `capture_expand.py`) are tooling.
- A call that reaches an import continues with seeded stub results. Functions
  that depend on real `HeapAlloc`, file or message results may stay
  low-coverage; hold them under the existing import-capacity family.
