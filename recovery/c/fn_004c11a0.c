#include "cmre.h"

/* Scratch frame: the original `sub esp, 0x9a0` block. Offsets below are from its
   base L (the esp value right after that instruction). Layout from the listing:
   0x00 u8 count for the 0x40 array (bVar3), 0x04 u8 count for the 0x360 array
   (bVar2), 0x08 u8 count for the 0x680 array (bVar4), 0x0c u32 value loaded from
   [0x00adadfc + 4 * [0x009cf728]], 0x10 12 u32 entries (indexed by bl),
   0x40 200 u32 entries, 0x360 200 u32 entries, 0x680 200 u32 entries.
   Each buffer address is what the original `lea` produces at its own push depth. */
#define FR_U8(off) (*(volatile u8 *)(frame + (off)))
#define FR_U32(off) (*(volatile u32 *)(frame + (off)))

/* The four helper calls push full 32-bit values; the original's second and third
   arguments are read as shorts, so they are declared as 32-bit slots here. */
extern void cm_fn_004afbd0(u32 param_1, u32 param_2, u32 param_3);
extern u32 cm_fn_004afd80(u32 param_1, char param_2);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);

u32 CM_THISCALL cm_fn_004c11a0(u32 self)
{
    u8 frame[0x9a0];
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 edi;
    u32 ebp;
    u8 bl;
    s32 count;

    bl = 0;
    ebp = 0;
    *(volatile u16 *)(self + 0x3e) = 8;
    eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    ecx = CM_GLOBAL(volatile u32, 0x009cf728);
    edi = CM_GLOBAL(volatile u32, 0x00ae23b4);
    FR_U8(0x04) = bl;
    edx = *(volatile u32 *)(eax + ecx * 4u);
    eax = CM_GLOBAL(volatile u32, 0x00ae235c);
    count = (s32)eax;
    FR_U8(0x00) = bl;
    FR_U8(0x08) = bl;
    FR_U32(0x0c) = edx;

    if (count > 0) {
        do {
            eax = cm_fn_0053b900((u32 *)edi);
            if (eax != 0) {
                break;
            }
            eax = *(volatile u32 *)(edi + 0x53);
            if (eax != 0) {
                u32 rec;

                edx = *(volatile u32 *)eax;
                ecx = CM_GLOBAL(volatile u32, 0x009cf3d0);
                if (edx == ecx) {
                    if (FR_U8(0x04) < 200) {
                        eax = FR_U8(0x04);
                        FR_U32(0x360 + eax * 4u) = edi;
                        FR_U8(0x04) = (u8)(FR_U8(0x04) + 1);
                    }
                } else {
                    rec = *(volatile u32 *)(edi + 0x57);
                    ecx = rec;
                    if (rec != 0) {
                        ecx = *(volatile u32 *)rec;
                    }
                    if (rec != 0 && ecx == CM_GLOBAL(volatile u32, 0x009cf590)) {
                        if ((s8)bl < 12) {
                            edx = (u32)(s32)(s8)bl;
                            bl = (u8)(bl + 1);
                            FR_U32(0x10 + edx * 4u) = edi;
                        }
                    } else {
                        if (edx == CM_GLOBAL(volatile u32, 0x009cf4f8)
                            || *(volatile u8 *)(eax + 0x75) != 7) {
                            if (*(volatile u8 *)(eax + 0x75) == 6) {
                                if (FR_U8(0x08) < 200) {
                                    ecx = FR_U8(0x08);
                                    FR_U8(0x08) = (u8)(FR_U8(0x08) + 1);
                                    FR_U32(0x680 + ecx * 4u) = edi;
                                }
                            }
                        } else {
                            if (FR_U8(0x00) < 200) {
                                eax = FR_U8(0x00);
                                FR_U32(0x40 + eax * 4u) = edi;
                                FR_U8(0x00) = (u8)(FR_U8(0x00) + 1);
                            }
                        }
                    }
                }
            }
            ebp = ebp + 1;
            edi = edi + 0x245;
            eax = CM_GLOBAL(volatile u32, 0x00ae235c);
        } while ((s32)ebp < (s32)eax);
    }

    /* 0x4c12be: the first helper call's second argument keeps the upper half of EDX. */
    edx = (edx & 0xffff0000u) | FR_U8(0x04);
    eax = (u32)(frame + 0x360);
    cm_fn_004afbd0(eax, edx, 2);

    ecx = (ecx & 0xffff0000u) | FR_U8(0x00);
    edx = (u32)(frame + 0x40);
    cm_fn_004afbd0(edx, ecx, 3);

    eax = (eax & 0xffff0000u) | FR_U8(0x08);
    ecx = (u32)(frame + 0x680);
    cm_fn_004afbd0(ecx, eax, 1);

    edx = (edx & 0xffff0000u) | (u16)(s16)(s8)bl;
    eax = (u32)(frame + 0x10);
    cm_fn_004afbd0(eax, edx, 1);

    ecx = *(volatile u32 *)(self + 0xa7);
    edx = FR_U32(0x360);
    edi = FR_U32(0x0c);

    *(volatile u32 *)(ecx + 0xc) = edx;
    eax = *(volatile u32 *)(self + 0xa7);
    ecx = FR_U32(0x364);
    *(volatile u32 *)(eax + 0x12) = ecx;
    edx = *(volatile u32 *)(self + 0xa7);
    eax = FR_U32(0x40);
    *(volatile u32 *)(edx + 0x18) = eax;
    ecx = *(volatile u32 *)(self + 0xa7);
    edx = FR_U32(0x44);
    *(volatile u32 *)(ecx + 0x1e) = edx;
    eax = *(volatile u32 *)(self + 0xa7);
    ecx = FR_U32(0x48);
    *(volatile u32 *)(eax + 0x24) = ecx;
    edx = *(volatile u32 *)(self + 0xa7);
    eax = FR_U32(0x680);
    *(volatile u32 *)(edx + 0x2a) = eax;

    if (edi == 0) {
        ecx = *(volatile u32 *)(self + 0xa7);
        edx = FR_U32(0x10);
        *(volatile u32 *)ecx = edx;
        eax = *(volatile u32 *)(self + 0xa7);
        ecx = FR_U32(0x14);
        *(volatile u32 *)(eax + 6) = ecx;
    } else {
        eax = CM_GLOBAL(volatile u32, 0x009cf590);
        ecx = eax + eax * 2u;
        edx = ecx + ecx * 8u;
        ecx = CM_GLOBAL(volatile u32, 0x00ae23d0);
        edx = (edx << 2) - eax;
        edx = edx + ecx;
        eax = cm_fn_004afd80(edx, 0);
        ecx = *(volatile u32 *)(self + 0xa7);
        *(volatile u32 *)ecx = eax;
        edx = *(volatile u32 *)(edi + 4);
        eax = cm_fn_004afd80(edx, 0);
        ecx = *(volatile u32 *)(self + 0xa7);
        *(volatile u32 *)(ecx + 6) = eax;
        eax = *(volatile u32 *)(self + 0xa7);
        edx = *(volatile u32 *)eax;
        ecx = *(volatile u32 *)(eax + 6);
        if (edx == ecx) {
            eax = CM_GLOBAL(volatile u32, 0x009cf590);
            edi = CM_GLOBAL(volatile u32, 0x00ae23d0);
            ecx = eax + eax * 2u;
            ecx = ecx + ecx * 8u;
            ecx = (ecx << 2) - eax;
            ecx = ecx + edi;
            eax = cm_fn_004afd80(ecx, 1);
            edx = *(volatile u32 *)(self + 0xa7);
            *(volatile u32 *)edx = eax;
        }
    }

    ecx = 0;
    if ((s16)*(volatile u16 *)(self + 0x3e) > 0) {
        eax = 0;
        do {
            ecx = ecx + 1;
            edx = *(volatile u32 *)(self + 0xa7);
            *(volatile u8 *)(edx + eax + 4) = 0;
            edx = *(volatile u32 *)(self + 0xa7);
            *(volatile u8 *)(edx + eax + 5) = 0;
            eax = eax + 6;
            edx = (u32)(s32)(s16)*(volatile u16 *)(self + 0x3e);
        } while ((s32)ecx < (s32)edx);
    }

    return 1;
}
