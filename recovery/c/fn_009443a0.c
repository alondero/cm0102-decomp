#include "cmre.h"

/* Object fields by byte offset from the (possibly advanced) object pointer:
   0x04 counter, 0x08 limit (signed), 0x0c table pointer, 0x98 probe index,
   0x9c status word written with 0x5500aaff before each error report.
   0x00944e46 is _malloc (cdecl). 0x0094791c is _longjmp: it is noreturn, so the
   code after each error call is only reached if the callee returns. */
extern u32 CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_0094791c(int *buffer, int value);

u32 CM_CDECL cm_fn_009443a0(u32 arg1, u32 arg2)
{
    /* volatile: the object pointer is zero on one path; keeping it opaque stops
       the optimizer from proving a store through a null object is out of bounds. */
    volatile u32 esi;
    u32 ebx;
    u32 edi;
    u32 ecx;
    u32 edx;
    u32 ebp;
    u32 field4;

    esi = arg1;
    if (esi == 0) {
        *(volatile u32 *)(esi + 0x9cu) = 0x5500aaffu;
        cm_fn_0094791c((int *)(esi + 0x10u), 0x271b);
    }

    ebx = arg2;
    edi = cm_fn_00944e46(ebx + 0x12u);
    if (edi == 0) {
        *(volatile u32 *)(esi + 0x9cu) = 0x5500aaffu;
        esi = esi + 0x10u;
        cm_fn_0094791c((int *)esi, 0x2718);
    }

    *(volatile u32 *)edi = 0xaa53c5aau;
    *(volatile u32 *)(edi + 4u) = ebx;
    *(volatile u8 *)(edi + ebx + 0x10u) = 0x5au;
    *(volatile u8 *)(edi + ebx + 0x11u) = 0xf0u;

    ecx = *(volatile u32 *)(esi + 8u);
    field4 = *(volatile u32 *)(esi + 4u);
    if ((s32)field4 >= (s32)ecx) {
        *(volatile u32 *)(esi + 0x9cu) = 0x5500aaffu;
        esi = esi + 0x10u;
        cm_fn_0094791c((int *)esi, 0x271c);
    }

    edx = *(volatile u32 *)(esi + 0x98u);
    ebx = 0u;
    ebp = *(volatile u32 *)(esi + 0xcu);
    if ((s32)ecx > 0) {
        for (;;) {
            if (*(volatile u32 *)(ebp + edx * 4u) == 0) {
                *(volatile u32 *)(ebp + edx * 4u) = edi;
                *(volatile u32 *)(edi + 8u) = edx;
                *(volatile u32 *)(esi + 4u) = *(volatile u32 *)(esi + 4u) + 1u;
                break;
            }
            edx = (edx + 1u) % ecx;
            ebx = ebx + 1u;
            if ((s32)ebx >= (s32)ecx) {
                break;
            }
        }
    }

    *(volatile u32 *)(esi + 0x98u) = edx;
    if ((s32)ebx >= (s32)ecx) {
        *(volatile u32 *)(esi + 0x9cu) = 0x5500aaffu;
        esi = esi + 0x10u;
        cm_fn_0094791c((int *)esi, 0x2721);
    }

    return edi + 0x10u;
}
