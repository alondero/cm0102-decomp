# ADR 0014: Calls to the CRT exit routine do not return

Status: accepted (2026-11-24) under
[ADR 0011](0011-engineering-decision-rights.md).

## Context

Error paths across the game push a status and call `0x00945934`, the CRT
`exit` routine:

```text
push -1
call 0x00945934
<next instruction: usually the join point of another branch>
```

No `add esp, 4` follows the call. Read naively, the fall-through edge from the
call looks like a path on which `exit` returned and the code at the join point
then runs with ESP four bytes lower. That framing would need a shifted stack,
registers restored from the wrong slots and a `ret` through a local slot, none
of which plain C can reproduce.

Translators and triagers therefore held 23 functions (12 KB) under the
work-state blockers `exit-return-stack-continuation`, `returning-exit-stack`,
`returning-exit` and `exit-return-unbalanced-stack`. On 24 Nov 2026 the farm
also withheld at least six candidates whose workers reported passing gates at
both optimization levels (fn_0049ee70, fn_0051e220, fn_00523380, fn_007af870,
fn_007f9290, fn_007ff270), because "non-return is unproven".

## Evidence (pinned 3.9.68 bytes, SHA-256 `0c78e7da…f86`, preferred-image VAs)

- **`0x00945934` (exit):** `push 0; push 0; push [esp+0xc]; call 0x00945956; add esp, 0xc; ret`.
  It is `doexit(status, quick = 0, retcaller = 0)`.
- **`0x00945945` (`_exit`):** the same, with `quick = 1`. doexit has exactly these
  two callers; no `_cexit`-style caller passes `retcaller = 1`.
- **`0x00945956` (doexit):**
  - When `[0xde2fd4] == 1`, it calls `TerminateProcess(GetCurrentProcess(), status)`.
    `[0xde2fd4]` is set only at `0x9459e1`, just before `ExitProcess`, so this
    branch handles re-entry during `ExitProcess`. If it returned, control would
    rejoin the normal path.
  - It runs the `atexit` table and the two termination tables (`0x9459ef`).
  - Its only `ret` (`0x9459ee`) is reached only through `jne 0x9459ed` on
    `retcaller` (`ebx`, loaded once at `0x945979`). `retcaller` is zero for both
    callers, so control reaches `call [0x967164]`.
- **`[0x967164]`** is `KERNEL32.dll!ExitProcess`. The Windows API declares it
  `DECLSPEC_NORETURN`, and the process ends in it.
- **The original compiler treated the fall-through edge as dead.** In fn_0054d740,
  for example, the error block cleans up its earlier calls (`0x54d771:
  add esp, 0x14`, `0x54d7ac: add esp, 0x24`) and then pushes -1 and calls `exit`.
  The join point `0x54d7ca` must have the same ESP as the `je 0x54d7ca` path, so
  an arrival four bytes lower is impossible by construction.
- So `0x00945934` and `0x00945945` return only if `ExitProcess` returns. They
  never return in the game.

The gate models this. `ExitProcess` (with `TerminateProcess` and the
unwinding imports) is in `cmre/imports.py` `TERMINAL`, and `cmre/lift.py` lists
it in `NORETURN_IMPORTS`. The C build's `cm_fn_00945934` runs the original exit
code, so both sides reach the same trap, and the emulator ends the case there.
Under an import transcript, reaching it is an `error` outcome on both sides.
The fall-through edge after `call 0x00945934` is never executed on either side.
Work-state notes that say the helper "RETs after ExitProcess in harness" are
mistaken.

Before this ADR the emulator stopped before recording the terminal call, so the
exit status was not compared: `cm_fn_00945934(0)` written for `(-1)` passed. The
emulator now records the `ExitProcess` and `TerminateProcess` calls with their
arguments (`imports.PROCESS_EXIT`) before the case ends, and the semantic gate
compares them as it does other import calls.

## Decision

A call to `0x00945934` or `0x00945945` ends its path. Recovered C writes the
call (`cm_fn_00945934(status);`) as the last statement of that path, typically
the end of an `if` block. Only the fall-through edge from the call is dead. The
code at the next address is usually a join point that other branches reach, and
it is still translated for them. Recovered C does not model the dead edge: no
shifted stack, no register reloads from shifted slots, no `exit_returned`
state, and no invented return value.

This is not an invented safe default. It follows from the original bytes, the
original compiler's own stack accounting and the documented `ExitProcess`
contract that the harness already relies on.

The rule covers only the return to the call's fall-through. Everything the exit
routine does before `ExitProcess` (`atexit` handlers, termination tables,
`TerminateProcess`) still runs as original code across the boundary, and the
gate compares its memory effects and the exit status.

A handler that longjmps or throws leaves through a setjmp site or an exception
handler with its own stack, never through `exit`'s `ret`. That is outside this
rule. Under the gate, `RaiseException` and `RtlUnwind` are already terminal.

## Consequences

- The four work-state blockers above are cleared to `ready` with a note citing
  this ADR. The farm and hand translation can then take the 23 functions.
- Candidates that pass the gate and differ only in modelling the impossible
  return are reworked to end the path at the call, then gated and accepted
  normally.
- The emulator change alters the harness digest. Accepted sources that reach
  `ExitProcess` are revalidated, and one that passes a different status is a
  defect.
- `docs/agent/C.md` states the rule beside the other compiler-boundary
  guidance.
- A function that calls `ExitProcess` directly is covered by the same
  contract. Other routines called before `exit` (for example the shutdown
  helpers `0x5cc7d0` and `0x5a8f60`) stay ordinary returning boundaries, and so
  does an indirect call through the `_exit` pointer at `.data:0xad6c40`.
