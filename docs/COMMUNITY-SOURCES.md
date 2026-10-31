# Community research intake

Checked 2026-09-27. These are leads, not accepted labels for our binary.

The champman0102 forum thread "Offsets" (topic 1540) has now been captured in
full and verified mechanically. See **[OFFSETS-COMMUNITY.md](OFFSETS-COMMUNITY.md)**
for the results, the reproduction commands, and the mined documents under
`CM0102/Analysis/Community-Offsets/mined/`. Summary of what changed: the thread's
hex-edit addresses are raw file offsets (not VAs) and 97.3% of them resolve in
our clean 3.9.68 `cm0102.exe`; 1454 of 1779 quoted instruction lines are
byte-identical to our build; and 789 of 791 original developer source paths
listed in the thread are exact strings in our executable. The verdict vocabulary
below still governs every claim.

| Source | Use | Access and confidence |
| --- | --- | --- |
| [Offsets thread, all 31 pages](https://champman0102.co.uk/viewtopic.php?t=1540) | Primary community offset and disassembly source | Captured whole; raw HTML hashed in `CM0102/Analysis/Community-Offsets/posts.json`; see `docs/OFFSETS-COMMUNITY.md` |
| [Offsets thread, page 21](https://champman0102.co.uk/viewtopic.php?start=500&t=1540) | Candidate address and behavior claims | Superseded by the full capture; individual claims must still be checked against our bytes |
| [Offsets thread, page 5](https://champman0102.co.uk/viewtopic.php?p=21674) | Example of version-specific offset discussion | Search excerpt explicitly mentions Tapani/Saturn; cannot transfer to official baseline |
| [Nick's patcher source](https://github.com/nckstwrt/CM0102Patcher) | Locate patch byte patterns, save/data investigations, and resolution research | Repository listing and README reviewed; source routines have not yet been validated against our binary |
| [Patcher forum thread](https://champman0102.net/viewtopic.php?t=1943) | Context and original discussion for the patcher | Search indexed; follow individual claims to source and original bytes |
| [Introduction: Patches](https://champman0102.co.uk/viewtopic.php?f=35&t=1395) | The only off-thread link made inside topic 1540 | Fetched; a single sticky introduction post, not a second offset database |

The patcher repository has useful candidate entry points including `Patcher.cs`,
`OffsetCalculator.cs`, `YearChanger.cs`, `ResolutionChanger.cs`, `SaveChanger/`,
and `Scouter/`. Their presence is a navigation aid, not evidence that their names
describe recovered original routines. No community code or patches were installed
or applied to the analysis target.

Address-space warning, measured on the full topic 1540 capture: the community
quotes `cm0102.exe` two different ways in the same posts. Disassembly listings and
annotated instructions use **virtual addresses**; hex edits use **raw file
offsets**. Reading a hex edit as a VA puts 1458 of 2008 lines outside any section.
For `.text`, `.rdata` and `.data` of our 3.9.68 target, `VA = file offset +
0x00400000`; `.rsrc` does not follow that rule.

For each future adopted claim record:

1. Exact post URL or commit-pinned source path and line, author/date if available.
2. Claimed executable/version, patch prerequisites, address convention, original
   and replacement bytes. Unknown provenance stays unknown.
3. Our target path and SHA-256; file offset/RVA/VA conversion using its PE sections.
4. Observed original bytes, surrounding instructions, function boundaries, callers,
   data references, and runtime evidence where possible.
5. Verdict: mismatch, unresolved hypothesis, statically supported, or dynamically
   validated. Never infer equivalence from an offset alone.

A patch may reveal a constant, branch, or structure field without explaining the
complete algorithm. Recover all paths from the original instructions; do not use
the patcher's intended improvement as the original engine specification.
