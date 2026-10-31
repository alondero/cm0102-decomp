# Language nine: historical isolated native candidate

This records the isolated candidate before promotion. Maintained source has
since passed full O0/O2 acceptance; see [LANG-MORPH-NINE.md](LANG-MORPH-NINE.md).
Candidate-only statuses and next-gate wording below describe that earlier stage.
Its sixteen inputs are now frozen under `morph-nine-candidate-source-used`.


Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
`CM0102/Analysis/baseline-c/cm0102.exe` has the same hash. Addresses are
preferred-image **VAs**, image base `0x00400000`.

Comparison `0x0067B506` tests language nine; body starts `0x0067B50F`.
JNE reaches language five at `0x0067C2E5`, whose body starts `0x0067C2EE`.
`morph-nine-static-a` records **3,551 bytes / 1,029 instructions** through
exclusive end `0x0067C2E5`, region SHA-256
`5eb4265c8802a62b87ca7574239f1ea0b04fab8b63cd54c36e99bf079fcd6593`.
The last fifteen bytes `0x0067C2D6..0x0067C2E4` are already accepted shared
unknown-suffix handling. Future parent coverage must add only **3,536 bytes**.

An isolated native candidate lives under
`CM0102/Analysis/morph-nine-native-candidate-a/`. Its generated worker contains
88 verified code-target-to-literal mappings, fifteen property-prefix rules and
eight property-suffix rules. A private C translation unit includes the existing
partial recovery, and a private assembly copy adds the new dispatch. **No current
recovery source or accepted ledger is modified by this candidate.** Its O0/O2
reports each pass **15,677 original-versus-native comparisons**, with source,
artifact, original-region/table and generation pins. This is useful finite
candidate evidence, not acceptance into the current combined corpus, whole-parent
validation, live integration or a standalone game build.

## Original rules and selector

The prefix argument is captured before comparisons. `upper` converts the signed
first local-word byte through `0x009471CD`, stores AL and continues to suffix
processing. `lower` converts through `0x009465FC`, has no acronym exceptions,
and preserves the next-byte read after a callback returns zero. Suffix capture
at `0x0067BE49` occurs after those calls.

The remaining prefix keys, in original order, are `no`, `No`, `o`, `a`, `O`,
`do`, `di`, `Do`, `DO`, `ao`, `pelo`, `um`, `Um`, `Uma`, `deste`. Thirteen
keys select eleven five-pointer tables; aliases preserve distinct original
comparison positions. Signed low property1 is followed by unsigned `JA 4`, so
0..4 select original targets and other bytes copy a single space. High property
bits are ignored. Unknown nonempty prefixes erase the first caller byte.

`do` and `di` share a separate selector at `0x0067BD5E`. Signed property1 is
checked with unsigned `JA 100`, then a 101-byte selector at `0x006811DC` chooses
among seven DWORD targets at `0x006811C0`. Properties 0..4 each select their own
literal-copy path. **Property 100 directly clears the first local-prefix byte**
at `0x0067BDCA` and jumps to suffix handling; it performs no string scan or
grouped copy. Properties 5..99 and unsigned out-of-range values take the original
single-space default. The candidate preserves that distinct direct store.

Suffix `s` copies an empty local suffix. The eight property suffixes are
`promovido`, `relegado`, `foi promovido`, `despromovido`, `eliminado`,
`foi despromovido`, `foi premiado`, and original byte string
`est\xE1 qualificado`. Each reads signed low property2 and its five-pointer table,
with unsigned 0..4 bounds and a default space. Unknown nonempty suffixes erase
the caller's first byte. All nine-language paths reach shared composition and
its dynamic language reread; this body adds no early zero return or helper call.

`morph-nine-summary-a` records 26 comparison occurrences: seventeen prefix
and nine suffix keys. `morph-nine-dependencies-a` records nineteen EAX-indexed
five-pointer tables and 82 distinct targets. `morph-nine-selector-a` records the
two additional selector regions and exact target instructions. Those six extra
literal-copy targets make the candidate's total 88 mappings; the direct-zero
target is a separate action. The candidate watches all **21 additional table
regions** alongside the current partial harness contract.

## Evidence and next gate

`prepare-morph-nine-prototype.py` verifies all four static report/artifact chains,
table bytes and copy-target shapes before emitting the candidate. Its first
attempt rejected the default-space target because that target falls directly
into its grouped-copy primitive instead of ending in JMP. The corrected shape
check permits that exact original fallthrough. The failure occurred before
output creation; it was an analysis assertion, not an application mismatch.

`test-morph-nine-candidate.py` compiles both isolated builds with warnings as
errors and rejects original parent/helper instructions on recovered paths.
Per build, 8,704 prefix-property and 4,608 suffix-property cases cover all 256
low bytes with two high-bit patterns. The remaining cases cover every first
byte, prefix/suffix combinations, selected aliases, character-return bits,
suffix-slot/property mutation, output replacement, dynamic language changes,
word lengths through 2,000 and 1,000 seeded inputs. State includes original
frame/output/language/table/caller bytes and ordered CRT boundary events.

`morph-nine-candidate-audit-a/audit.json` independently closes both report,
generation, source, artifact, inspection and original-table identities. It pins
the GCC 15.2/binutils 2.45 tools and their version strings through the identical
toolchain used by the full language-three O2 run. The original stack probe,
fixed-address data and print/upper/lower/find fixtures remain explicit. Valid
terminated strings, bounded mapped buffers, DF clear and normal returns are the
contract; general faults, concurrency, arbitrary aliases and overflow are excluded.

Next copy the candidate into maintained recovery source, expand and freeze the
combined source inputs, add the 3,536 new parent bytes and 21 table regions, and
run the full combined O0/O2 corpus before promoting another partial ledger.
Keep earlier accepted sources/reports and all eight full-routine ledgers intact.
After nine, recover five and remaining shared gaps. `morph-five-static-a`
already records **14,863 bytes / 4,767 instructions**, SHA-256
`981bc35bb9b0fc7ce37e9842210c64718ce0149fece033604e8cdf5aa53755ec`;
that preflight remains static discovery only. M2/M4/M6 stay open.
