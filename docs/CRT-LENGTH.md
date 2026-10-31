# Maintained bounded-length CRT support

Primary input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Addresses are preferred-image VAs, image base `0x00400000`.
Body `0x009519D3..0x009519FD`: **43 bytes / 20 instructions**, SHA-256
`b67fe6e7812efed1741593bed7ebbacf75b01ba12349fbdd095a0116a7792075`.

`recovery/crt/length-ledger.json` accepts one CRT/string entry independently of
application coverage. Identified, decompiled, compiled and finite-corpus validated
are true; integrated is false. Locale/morphology adoption remains false.
This is native assembly recovery; C optimization levels do not apply.

Maintained assembly is byte-identical to the reviewed
[candidate](CRT-LENGTH-DISCOVERY.md). It preserves an extra byte read at the
limit, including limit zero, and reads a loop-encountered NUL twice. It reloads
the original pointer from the caller slot when subtracting, preserves ESI and
retains original registers/flags and unsigned arithmetic. It requires no
original executable code, data, globals or boundary fixtures.

The historical candidate audit, finish report and standards/spec review remain
unchanged. A fresh read-only prerequisite audit rechecks their instruction,
source, artifact, tool and frozen-input chains. The first maintained build passes
48,176 cases. Spec review clarified the header: a NUL read only by the final
comparison is read once. Its inputs were frozen before that wording change.
Final `crt-length-maintained-test-b` again passes **48,176** cases, with four
inputs frozen in `crt-length-maintained-source-used`. The header also compiles
under 32-bit GCC with warnings treated as errors.

Comparisons cover all eight integer registers plus EFLAGS, complete synthetic
stack/arena state, ordered accesses, selected caller-slot aliases and first
unmapped-read CPU faults. An independent oracle checks ordinary mapped inputs.
Finite Unicorn evidence does not establish Windows SEH, universal pointer-wrap
equivalence or live game integration.

```powershell
# python currently resolves to an unrelated venv; use the tested interpreter.
py -3.13 scripts/verify_crt_length.py --output CM0102/Analysis/crt-length-recheck-audit
py -3.13 recovery/crt/test_cm_crt_length.py --output CM0102/Analysis/crt-length-recheck-test
py -3.13 scripts/map_crt_length_family.py --output CM0102/Analysis/crt-length-recheck-map
```

Use fresh directories. The current thirteen-ledger map selects **45 application
and six CRT entries**, checks **16,340 calls and three tails**, and pins eighteen
inputs. Fresh `crt-length-family-final-a/b` agree on nine artifacts. Every prior
map, accepted ledger and source remains unchanged. Separate locale candidates
link this helper in an emulator experiment; maintained adoption/integration is
unchanged. [Deeper locale recovery](CRT-LOCALE-DISCOVERY.md), M2 replay, M4 live
integration and M6 standalone game gates remain open.
