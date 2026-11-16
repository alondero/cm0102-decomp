#include "cmre.h"

extern u32 CM_CDECL cm_fn_00452fe0(u32);
extern void CM_CDECL cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern void CM_CDECL cm_fn_00945934(u32);

u32 CM_THISCALL cm_fn_00452710(u32 self, u32 *index, u32 create)
{
    u8 scratch[512];
    u32 entry, value, capacity, base, text;
    if (!index) {
        cm_fn_00944cff(0x98b6a0, 0, 0, scratch + 256, scratch);
        text = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, text, scratch + 256, scratch, 0x1680);
        cm_fn_005e8290((char *)0x9870e0, (char *)0xae24d0, 0);
        CM_GLOBAL(volatile u32, 0xb67a34) = 0;
        return 0;
    }
    value = *(volatile u32 *)index;
    base = *(volatile u32 *)(self + 0x1c);
    entry = base + value * 106u;
    value = *(volatile u32 *)entry;
    if (value == 0xffffffffu && create) {
        capacity = *(volatile u32 *)(self + 8);
        value = *(volatile u32 *)(self + 12);
        if (value == capacity) {
            capacity += 50;
            *(volatile u32 *)(self + 8) = capacity;
            base = *(volatile u32 *)(self + 4);
            value = (u32)cm_fn_00945667(base, capacity * 907u);
            *(volatile u32 *)(self + 4) = value;
            if (!value) {
                cm_fn_00944cff(0x98b6a0, 0, 0, scratch, scratch + 256);
                text = (u32)cm_fn_0090d130();
                cm_fn_00944c9f(0xae24d0, 0x9870e8, text, scratch, scratch + 256, 0x1694);
                cm_fn_005e8290((char *)0x9870e0, (char *)0xae24d0, 0);
                CM_GLOBAL(volatile u32, 0xb67a34) = 0;
                cm_fn_005cc7d0();
                cm_fn_005a8f60();
                /* CONFIRMED UNRESOLVED ABI BOUNDARY.
                   CM0102 3.9.68 SHA-256
                   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86;
                   all addresses below are preferred-image VAs.
                   00452866 pushes -1; 00452868 calls 00945934;
                   0045286d continues without caller cleanup. If that call
                   returns without popping its argument, 00452898 balances
                   only the argument pushed at 00452892 for 00452893.
                   Entry 00452714 allocates 0x200 bytes, then 0045271c/1d
                   save ESI/EDI. Both epilogues (00452785..0045278f and
                   004528a9..004528c5) consequently pop -1 into EDI and
                   saved EDI into ESI, then RET 8 through saved ESI.
                   The listing does not establish the callee's return policy.
                   This ordinary C call does NOT preserve that conditional
                   stack displacement. Fixing it requires an authorized ABI
                   boundary, not a stdcall/noreturn guess or frame-layout
                   trick. Gate success is not evidence for this path. */
                cm_fn_00945934(0xffffffffu);
            }
        }
        value = *(volatile u32 *)(self + 12);
        *(volatile u32 *)entry = value;
        value = *(volatile u32 *)(self + 12);
        *(volatile u32 *)(self + 12) = value + 1;
        value = *(volatile u32 *)entry;
        base = *(volatile u32 *)(self + 4);
        cm_fn_00452fe0(base + value * 907u);
    }
    value = *(volatile u32 *)entry;
    if ((s32)value <= -1)
        return 0;
    base = *(volatile u32 *)(self + 4);
    return base + value * 907u;
}
