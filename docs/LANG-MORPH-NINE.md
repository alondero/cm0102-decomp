# Morphology language nine: ordered qualifiers and direct-zero selector

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
`CM0102/Analysis/baseline-c/cm0102.exe` has the same hash. Addresses are
preferred-image VAs, image base `0x00400000`. Parent entry is `0x006701F0`.

`recovery/lang/cm_lang_morph_nine_body.inc`, included by the maintained partial
C source and entered by its assembly dispatch, adds **3,536 original instruction
bytes / 1,026 records**, `0x0067B506..0x0067C2D5`. The static preflight's
last fifteen bytes were already accepted shared code and are not counted again.
The parent now has **49,588 regional bytes in twenty-one nonoverlapping regions**,
including **10,359 manually decoded bytes / 2,342 records** absent from the
existing Ghidra export. Nine contributes 1,372 absent bytes / 263 records.
Helpers A/B remain separate full routines. The parent remains incomplete and
outside the unchanged 44-application plus one-CRT full-routine inventory.

## Recovered behavior

The seventeen prefix comparisons retain original order: `upper`, `lower`,
`no`, `No`, `o`, `a`, `O`, `do`, `di`, `Do`, `DO`, `ao`, `pelo`, `um`,
`Um`, `Uma`, `deste`. Case conversion receives signed word bytes, stores only
AL, and has no acronym exception. Lowercase conversion preserves the next-byte
read even when a callback returns zero. Suffix capture at `0x0067BE49` follows
these callbacks, preserving mutations to the incoming suffix slot/properties.

Thirteen property-prefix keys use eleven five-pointer tables. Signed low
property1 with unsigned `JA 4` admits 0..4; other bytes copy a space. `do`/`di`
instead use the seven-pointer table at `0x006811C0` and 101-byte selector at
`0x006811DC`. Values 0..4 select separate literal copies; 5..99 copy a space.
Value 100 directly clears the first local-prefix byte at `0x0067BDCA`, without
a string scan or grouped copy. Signed negative/out-of-range values use a space.
Unknown nonempty prefixes erase the first caller byte through the original
empty-literal grouped-copy primitive.

Suffix comparisons are `s`, `promovido`, `relegado`, `foi promovido`,
`despromovido`, `eliminado`, `foi despromovido`, `foi premiado` and original
byte string `est\xE1 qualificado`. `s` copies an empty local suffix. Other
recognized keys select five-pointer tables using signed low property2 with
unsigned 0..4 bounds and space defaults. Unknown nonempty suffixes erase their
caller byte. All nine paths reach shared composition; the body adds no helper
call or early-zero return. The original dynamic language reread and contraction
still occur. Prefix/suffix aliases and trailing buffer bytes remain observable.

The native worker maps 88 original code targets to literal-copy actions and
keeps the direct-zero action separate. Its fixed-address data is still original.
Arbitrary edits to code-pointer table targets are outside the accepted contract.

## Acceptance and provenance

`CM0102/Analysis/morph-nine-final-a-O0/report.json` and its O2 counterpart each
pass **224,181 comparisons**, with thirteen identical current/frozen inputs.
All 208,504 earlier inputs remain, reclassifying nine's neutral dispatch; nine
adds **15,677 cases**. These include 8,704 prefix-property and 4,608 suffix-property
cases across all 256 low bytes with two high-bit patterns, 512 first-byte cases,
765 combinations, selected aliases and callback mutations, return-bit patterns,
dynamic language/output replacement, lengths through 2,000 and 1,000 seeded cases.
GCC 15.2/binutils 2.45 compile both builds with warnings as errors.

The watch contract checks full EAX/ABI, ordered CRT boundary events, original
argument bytes, the full 16KB caller arena, shared output, language state,
frame `P+0x10..P+0x2813` and **225 original table regions**. The relocated probe
return-address cell is excluded. This does not establish a complete ordered
memory-access trace, all aliases, faults, overflow, concurrency or real CRT behavior.
The original 47-byte stack probe and explicit CRT fixtures remain dependencies.

Revision eight of `recovery/lang/morph-partial-ledger.json` pins both reports
and `morph-partial-source-used-nine`. `morph-nine-prior-state` archives revision
seven's ledger/verifier/docs. The verifier checks all seven earlier acceptance
stages against frozen inputs and keeps eight full-routine ledger hashes intact.
`morph-nine-acceptance-audit-a/audit.json` passes original byte/table/source,
artifact/tool/export, historical and adoption-provenance checks.

The isolated candidate's two 15,677-case reports, audit and sixteen inputs remain
frozen under `morph-nine-candidate-source-used`. `morph-nine-adoption.json`
records script, input/output hashes and candidate/previous-ledger identities.
The verifier checks token identity between its worker and maintained source.
[The historical discovery note](LANG-MORPH-NINE-DISCOVERY.md) records earlier
analysis assumptions and candidate-only evidence. Neither old test driver should
be rerun against changed maintained inputs; use its frozen snapshot for audit.

Initial independent reviews confirmed worker identity and accounting; Spec ran
sixty additional comparisons. Final review and preservation results are recorded
in [the partial handoff](LANG-MORPH-PARTIAL.md).

Next complete [language five](LANG-MORPH-FIVE-DISCOVERY.md) at `0x0067C2EE`.
Unknown initial languages still transfer into original code at `0x0067C2E5`.
Close five, the shared conditional guard at `0x0067FD31..0x0067FD3E` and default
composition before auditing the complete parent. No recovered code is integrated.
M2 replay/primary-GDI mapping, M4 live integration and M6 standalone remain open.
