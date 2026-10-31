# Maintained compiler stack probe

Primary: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`;
hash-identical copy `CM0102/Analysis/baseline-c/cm0102.exe`.
Addresses are preferred-image VA, image base `0x00400000`.

`recovery/crt/cm_stack_probe.S` recovers `0x00945610..0x0094563E`:
47 original bytes / 17 instructions. Original SHA-256 is
`4be32df90e8e3db3f5eed0d31857bddd88255c7ed09ae57e0e20e4510124a255`.
`recovery/crt/stack-probe-ledger.json` accepts compiler support separately;
no application-count increase or live integration is claimed.

The register ABI supplies unsigned allocation in EAX, not a cdecl argument.
For incoming ESP S, cursor starts at S+4, reads a DWORD after each 4096-byte
decrement, then at the exact remainder, including zero. Normal return preserves
ECX/other general registers, sets ESP to S+4-allocation and EAX to the original
return address, and retains the final TEST flags. The return-address push needs
four bytes below the cursor even when its final read succeeds.

Source is byte-identical to the reviewed assembly candidate. The compiled
47-byte/17-instruction body uses equivalent alternate register encodings;
SHA-256 `21b0d1ae0fe61dfd80264ca22da64cab82b87e0967d52f1270ea4a35f2f59446`.
`stack-probe-maintained-final-a/report.json` passes 3,020 comparisons: 2,442
mapped-stack, 512 seeded random, 30 first unmapped-read and 36 first return-push
write faults. Registers, flags, ordered accesses, touched stack and first fault
are compared. Three inputs are frozen in `stack-probe-maintained-source-used`.
The maintained audit checks source/report/tool/artifact and instruction identities.

```powershell
python recovery/crt/test_cm_stack_probe.py --output CM0102/Analysis/stack-probe-recheck
python scripts/verify_stack_probe.py --output CM0102/Analysis/stack-probe-recheck-audit
```

Use new output folders. No O0/O2 C-lowering claim applies to the assembly helper.
Windows guard-page growth, SEH, arbitrary wrap and live integration remain open.
[Candidate evidence](STACK-PROBE-DISCOVERY.md) retains the prior fixture correction.
[Morphology relink](LANG-MORPH-RELINKED.md) is separately accepted after full
O0/O2,323,545 comparisons per build and terminal controller exit zero/PASS. The helper ledger's adoption-false field records its separate
acceptance point; it does not supply or preclude future relink evidence.
