/* WIP: -O2 build blocked (GCC -Warray-bounds on the constant null-page store
   CM_GLOBAL(volatile u32, 0x9c) in the arg1==0 branch, under -Werror); -O0 passes.
   Preferred-image VA 0x009445d0, image base 0x00400000.
   Cdecl, two stack inputs (arg1 loaded into ECX, arg2 reloaded into EAX).
   Sentinel dword at arg2-0x10, byte checks at [[header+4]+header+0x10] and +0x11,
   then a store of 0 into the table at [ECX+0xc] indexed by [header+8], decrement
   of [ECX+4], and a call to cm_fn_009452ca. Error paths store 0x5500aaff at
   [ECX+0x9c], advance ECX by 0x10, call cm_fn_0094791c and fall through to the
   listing's continuation. Return is EAX: zero when arg2 is zero, otherwise the
   EAX left by cm_fn_009452ca. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void CM_CDECL cm_fn_0094791c(int *_Buf, int _Value);

u32 CM_CDECL cm_fn_009445d0(u32 arg1, u32 arg2)
{
    u32 ecx = arg1;
    u32 eax;
    u32 edx;
    u32 esi;

    if (ecx == 0) {
        CM_GLOBAL(volatile u32, 0x0000009c) = 0x5500aaffu;
        cm_fn_0094791c((int *)0x10, 0x271b);
    }

    eax = arg2;
    if (eax == 0) {
        return eax;
    }

    edx = *(u32 *)(eax - 0x10u);
    eax = eax - 0x10u;
    if (edx != 0xaa53c5aau) {
        *(u32 *)(ecx + 0x9cu) = 0x5500aaffu;
        ecx = ecx + 0x10u;
        cm_fn_0094791c((int *)ecx, 0x2719);
    }

    edx = *(u32 *)(eax + 4u);
    if (*(u8 *)(edx + eax + 0x10u) != 0x5au) {
        *(u32 *)(ecx + 0x9cu) = 0x5500aaffu;
        ecx = ecx + 0x10u;
        cm_fn_0094791c((int *)ecx, 0x2719);
    }

    if (*(u8 *)(edx + eax + 0x11u) != 0xf0u) {
        *(u32 *)(ecx + 0x9cu) = 0x5500aaffu;
        ecx = ecx + 0x10u;
        cm_fn_0094791c((int *)ecx, 0x2719);
    }

    edx = *(u32 *)(ecx + 0xcu);
    esi = *(u32 *)(eax + 8u);
    if (*(u32 *)(edx + esi * 4u) != eax) {
        *(u32 *)(ecx + 0x9cu) = 0x5500aaffu;
        ecx = ecx + 0x10u;
        cm_fn_0094791c((int *)ecx, 0x271f);
    }

    *(u32 *)(edx + esi * 4u) = 0u;
    edx = *(u32 *)(ecx + 4u);
    edx = edx - 1u;
    *(u32 *)(ecx + 4u) = edx;
    return (u32)cm_fn_009452ca(eax);
}
