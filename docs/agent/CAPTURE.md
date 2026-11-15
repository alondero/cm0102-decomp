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
4. The recorder detaches at `--seconds` and leaves the game running. Read
   `SESSION.json`: `reached` and `reached_bytes` say what the session touched.

Completion: a session summary with reached functions and bytes, and capture
files under `local/captures/`.

## 2. Expand and apply (no game needed; parallel)

```powershell
py -3.13 -m cmre capture expand --jobs 16 --output CM0102/Analysis/cmre/captures/expand-N.json
py -3.13 -m cmre capture apply --jobs 16 --output CM0102/Analysis/cmre/captures/apply-N.json
```

`expand` adds derived inputs only where the original executes new instructions
of the function. `apply` selects a covering set, writes it to the function's
hints as `captures` (removing `"auto": true`, which claims the hint), and runs
the unchanged lifted gate. Its summary lists `passing` functions and their
bytes. Both commands default to every function that has stored captures; name
`fn_XXXXXXXX` IDs to bound them. Neither takes the writer lease.

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

Accepted lifts enter `cmre next --c`. Translate them through [C.md](C.md);
their C tests replay the same captures.

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
