# Proposal: an unattended capture lane in an isolated VM

Status: proposal (2026-11-19). Decision rights: [ADR 0011](../0011-engineering-decision-rights.md).
Builds on [ADR 0009](../0009-captured-entry-inputs.md) and [RUNTIME-VM.md](../../RUNTIME-VM.md).

## Problem

Captures feed the largest band of unrecovered code: 952 unlifted functions,
1.83 MB. Today a capture needs the real game running on Adam's desktop. The
DirectDraw build reads the real cursor and screen, so an agent can drive it
only while he is away. Captures are also untargeted ("play broadly"): nothing
maps a function to the game activity that reaches it.

On 19 Nov, capture apply over 421 captured-but-unlifted functions gave 8
lifts. Expansion of 145 more brought 9 over the coverage threshold. More
mutation gives less and less; the missing inputs are game activity that no
one has recorded.

## Proposal

1. **Capture VM.** Clone the audited reference VM `CM0102-reference-20261003-a`
   to a separate capture VM, leaving the audited clone unchanged. Enable
   VirtualBox 7.2's guest debugger (`--guest-debug-provider=gdb`,
   `--guest-debug-io-provider=tcp`, localhost only). The game runs in the
   guest from a private copy, as in RUNTIME-VM.md. Input goes through the
   existing VirtualBox mouse tools (`scripts/vbox_reference_mouse.py`), not
   the host cursor.
2. **Host-side recorder over GDB remote protocol.** It replaces the Win32
   debug-API transport in `capture_record.py`. The steps are:
   - break in only while the game process's address space is current;
   - set entry breakpoints;
   - at each hit, read registers, stack words and the touched pages;
   - save them through the existing `capture.save`.

   The capture format, store, expansion and lifted gate do not change, and no
   code runs in the guest. The first spike is one breakpoint at one function
   entry in one known scenario. It must save a capture byte-compatible with a
   host-recorded capture of the same entry.
3. **Scenario catalogue.** A scenario is a starting save plus a scripted action
   sequence with screenshot checkpoints, for example:
   - new game to the first week;
   - holiday across the season rollover;
   - a transfer bid;
   - a full match day.

   Every recorded run already reports the functions it reached. Stored per
   scenario, these reports build a map from each function to the scenarios
   that reach it.
4. **Targeting.** A `capture` lane on `cmre_farm.py lanes` groups unlifted and
   low-coverage functions by scenario, ranked by bytes. A function no scenario
   reaches is attributed to the nearest reached caller in the static call
   graph. That caller names the screen or activity a new scenario must
   extend.
5. **Trigger.** When the capture lane has demand and the VM is idle, the farm
   launches a capture agent, the same way it launches the lift lane. The agent
   runs the scenarios with the most bytes waiting, or writes a new one from
   screenshots for an uncovered activity, and records them. The lift lane
   consumes the captures within the hour. One recorder runs at a time, and
   captures stay local.

## Constraints

- The fidelity and preservation rules hold. The supplied game folders and the
  audited reference VM are never modified. Captures never leave `local/`.
- Running the VM uses CPU on Adam's machine but not his desktop. Desktop
  driving on the host remains his decision
  ([CAPTURE.md](../../agent/CAPTURE.md)).
- Functions that depend on real heap or file results stay low-coverage until
  import-result recording lands ([FARM.md](../../agent/FARM.md), supply item 1).
  That is a separate release.

## First steps

1. Spike item 2 against the capture VM clone and compare it with a host capture.
2. Record the reached-function reports of the existing capture sessions
   (`CM0102/Analysis/cmre/captures/*.json`) as the first scenario-map entries.
3. Add the `capture` lane to `lanes` once the map exists.
