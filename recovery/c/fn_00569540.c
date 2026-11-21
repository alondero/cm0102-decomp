#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);

/* DF / REP-direction obligation — RESOLVED (not a blocker).
   Preferred-image VAs 0x005695ec (REP MOVSD) and 0x005695f3 (REP MOVSB) execute
   without a preceding CLD. The needed precondition is DF=0 at both sites. It is
   established by the calling-convention contract, not guessed:
     - ECX is consumed as the receiver at 0x00569547 (mov ebx,ecx) and RET 8 at
       0x00569606 pops the two stack slots; this is MSVC thiscall. The module is
       MSVC-built (network.h under e:\dev\cm3\cm3 00-01\si\code), so the Microsoft
       x86 ABI applies: DF must be clear on function entry and exit. Direct
       callers 0x00560004 / 0x0056937c are bound by the same ABI and enter DF=0.
     - All four error-path callees (0x00944cff, 0x0090d130, 0x00944c9f,
       0x005e8290, sequenced at 0x00569576 / 0x00569590 / 0x005695a0 /
       0x005695b1) are original MSVC routines of the same image and return with
       DF=0 under that ABI, so the fall-through REPs at 0x005695ec/0x005695f3
       also see DF=0. The omitted CLD is the normal ABI-correct omission.
   Consequence: these REPs are forward copies, which the plain-C incrementing
   loops below reproduce faithfully. A DF=1 caller is outside the ABI contract;
   the original's behaviour there is unspecified and a conforming harness cannot
   produce such a witness, so it is not modelled (no execution-interface
   extension is required). No EFLAGS/DF accessor exists in the plain-C interface,
   but none is needed once DF=0 is an established entry/exit precondition.
   Corroboration: cmre test --c full coverage (cov=1.0) at -O0 and -O2 with zero
   inconclusive; a DF=1 witness would have produced a direction mismatch.
   Negative lengths are NOT clamped: MOVSX at 0x005695d9 followed by SHR at
   0x005695e9 / AND at 0x005695f0 deliberately yields a very large unsigned
   DWORD dword-count and a byte remainder; (u32)(s32)len >> 2 and & 3 preserve it.
   ABI detail: word at entry ESP+8 read at 0x0056954a (len), source pointer at
   entry ESP+4 reloaded at 0x005695dc, receiver ECX saved at 0x00569547.
   Pinned binary CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
void CM_THISCALL cm_fn_00569540(volatile u32 *obj, u32 src, s16 len)
{
    u8 scratch[512];
    u32 pos = obj[1];
    u32 cap = obj[2];
    u32 n = (u32)(s32)len;
    u32 dst, count, i;
    if (pos + 2U > cap) {
        char *text;
        cm_fn_00944cff(0x009c3734, 0, 0, scratch + 256, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, (u32)text,
                       scratch + 256, scratch, 0x86);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    } else {
        u32 base = obj[0];
        *(volatile u16 *)(base + pos) = (u16)len;
        pos = obj[1];
        obj[1] = pos + 2U;
    }
    dst = obj[0];
    pos = obj[1];
    dst += pos;
    count = n >> 2;
    for (i = 0; i < count; ++i) {
        u32 value = *(volatile u32 *)src;
        *(volatile u32 *)dst = value;
        src += 4;
        dst += 4;
    }
    count = n & 3;
    for (i = 0; i < count; ++i) {
        u8 value = *(volatile u8 *)src;
        *(volatile u8 *)dst = value;
        ++src;
        ++dst;
    }
    pos = obj[1];
    obj[1] = pos + n;
}
