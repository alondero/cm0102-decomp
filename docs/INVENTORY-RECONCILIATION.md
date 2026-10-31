# Pinned inventory reconciliation

Primary input: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`. Addresses are
preferred-image VAs (base `0x00400000`). Inventory: the pinned Ghidra exports in
`cmre/env.py` (9,241 functions, 4,309,884 instruction-body bytes).

```powershell
py -3.13 -m cmre shards              # each entry: one shard, one state (issue #21)
py -3.13 -m cmre boundaries --ids    # exceptions, dispositions, generated parents (~2 min)
py -3.13 -m cmre boundaries --shard K --json --write   # local report, VAs and counts only
```

`cmre boundaries` is read-only accounting. An exception stays `open` until
mechanical evidence from the pinned image or an accepted record disposes of it.
Reports hold VAs, IDs and counts, never instruction bytes. They go under the
ignored `CM0102/Analysis/cmre/boundaries/`.

## Compiler-generated entries require metadata

`queue.compiler_generated` no longer trusts instruction shape. `cmre/generated.py`
classifies an entry only when the image proves it:

| Kind | Proof | Parent | Regeneration |
| --- | --- | --- | --- |
| `unwind-funclet` | action in a C++ EH FuncInfo unwind map (magic `0x1993052x`) | entry owning the single `push stub` of its `mov eax, FuncInfo; jmp` handler stub | compile the parent's C++ |
| `seh-finally` | null-filter handler in an `_except_handler3` scope table | entry owning `push -1; push table; push handler; mov eax, fs:[0]` | compile the parent's SEH |
| `import-thunk` | single `jmp [slot]` through a named IAT slot | the import (`DLL!name`) | link the import library |

The parent is the inventory entry whose decoded instructions contain the
registration site; extent containment alone does not count. The shape heuristic
survives as `queue.generated_shape`, used only to report disagreements.

## Results (2026-10-30)

- 3,898 entries are proven compiler-generated: 3,873 unwind funclets, 2 SEH
  `__finally` blocks and 23 import thunks. 3,582 have a verified inventory parent or
  import. 1,550 handler stubs each name one FuncInfo and are pushed exactly once;
  1,464 of them are registered inside inventory entries.
- `fn_00946323` and `fn_00946419` were mislabelled unwind funclets: they are SEH
  `__finally` handlers (scope tables `0x009713d0`, `0x009713f0`).
- `fn_009452b2` matched only the shape, and is called directly from `0x0094526d`,
  `0x00947971` and `0x0094bfd4`. Funclets are never called directly, so it is
  ordinary code and now enters the C queue.
- The last entry, `fn_009660d0`, could not be lifted: the region fallback was
  `0x00966000`, but `.text` ends at `0x00967000`. Fixed in `inventory.py`,
  which requires a full revalidation.
- Accepted entries outside the inventory, `fn_00682060` and `fn_0066f4c0`, are
  legacy records with manual boundary evidence. They are referenced only by
  absolute pointers (`0x0066f479`, `0x0066fcc0`; `0x0066efdd`). They are disposed
  as inventory omissions and stay excluded from inventory byte counts.
- 176 of 177 extent/body differences are explained by jump tables, alignment
  fill, code-referenced data, SEH filter/except blocks named by scope tables,
  or legacy records that decoded their whole extent. For example, `fn_006701f0`
  decodes 64,465 bytes where the inventory counts 49,771.
- `fn_00945a60`: the inventory body (133) overlaps `fn_00945ae0` by 5 bytes, while
  the accepted extent (128) ends at the next entry.

## Open exceptions

| Kind | Open | Meaning |
| --- | ---: | --- |
| `extent-body` | 1 | `fn_009450ec`: 3 unreferenced bytes (`push 1; pop eax`) inside the extent |
| `generated-parent-outside-inventory` | 316 | funclets whose 86 registering prologues lie outside every entry |
| `unowned-text` | 1,033 | 1,181,347 `.text` bytes outside every entry; 47 intervals (241,336 B) hold the 86 EH prologues |

`.text` (5,660,672 B) = 4,415,172 B owned by entries + 29,307 B fill + 34,846 B
handler stubs/fill + 1,181,347 B unresolved. Per-shard counts come from
`cmre boundaries`. An unowned interval is shown in the shard of its start VA only
provisionally: an explicit inventory revision decides ownership, and
`cmre shards --since` lists the entries it adds or removes.

## Revalidation (2026-10-31)

The whole accepted inventory was revalidated under the harness that includes
the `inventory.py` fix: receipt
`CM0102/Analysis/cmre/jobs/20261030T204625-revalidate-131a86bb.json`. It was
interrupted once by a machine restart and resumed with the original options.
Result: 7,984 functions checked, 7,980 pass, 4 fail. The four are tracked in
[issue #23](https://github.com/alondero/cm0102-decomp/issues/23): `fn_005a90b0`,
`fn_005ab510`, `fn_00666720` and `fn_005e5df0`. Their hints changed after
acceptance or are shared by the lifted and C tiers. A failing item blocks the
batch unblock step, so `cmre unblock --batch` has not been run.

Closing this issue needs that inventory revision. Add the missed functions with
evidence (EH-registered prologues first, then referenced code), and classify the
remaining unowned bytes as code or data. Neighbouring lift regions shrink when
entries are added, so the revision requires a full revalidation.
