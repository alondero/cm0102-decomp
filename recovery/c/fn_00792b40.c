#include "cmre.h"

/* Cdecl, no stack arguments (ret 0). Eight 107-byte-stride addresses are formed
   from globals 0x009cf6c4..0x009cf6e0 (every 107 * global, plus the base at
   0x00ae23d0). Entries of a 0x245-byte-stride table at the pointer in 0x00ae23b4
   (count in 0x00ae235c) whose dword at +0x57 matches one of the eight are collected
   into a pointer table (count held in a byte) at frame+0x1c. That table is sorted
   by 0x00945333 with comparator 0x0053fd40, and the eight addresses are then
   stored back into +0x57 of the entries in index bands of 12. The 0x19c-byte
   frame is the original's local area. */

extern void CM_CDECL cm_fn_00945333(u32 table, u32 count, u32 size, u32 compare);

u32 CM_CDECL cm_fn_00792b40(void)
{
    u8 frame[0x19c];
    u32 g8;
    u32 g4;
    u32 g12;
    u32 g16;
    u32 base;
    u32 g20;
    u32 g24;
    u32 g28;
    u32 g32;
    u32 entry_ptr;
    s32 n;
    u32 esi_v;
    u32 edi_v;
    u32 ebp_v;
    u32 ebx_v;
    u32 arr_a;
    u32 arr_b;
    u32 arr_c;
    u32 arr_d;
    u32 ptr;
    u32 k;
    u32 i;
    u32 v;
    u32 cnt;
    s32 count;

    g8 = CM_GLOBAL(volatile u32, 0x009cf6c8u);
    g4 = CM_GLOBAL(volatile u32, 0x009cf6c4u);
    g12 = CM_GLOBAL(volatile u32, 0x009cf6ccu);
    g16 = CM_GLOBAL(volatile u32, 0x009cf6d0u);
    base = CM_GLOBAL(volatile u32, 0x00ae23d0u);
    g20 = CM_GLOBAL(volatile u32, 0x009cf6d4u);
    g24 = CM_GLOBAL(volatile u32, 0x009cf6d8u);
    g28 = CM_GLOBAL(volatile u32, 0x009cf6dcu);
    g32 = CM_GLOBAL(volatile u32, 0x009cf6e0u);
    entry_ptr = CM_GLOBAL(volatile u32, 0x00ae23b4u);
    n = (s32)CM_GLOBAL(volatile u32, 0x00ae235cu);

    edi_v = g8 * 107u + base;
    esi_v = g4 * 107u + base;
    ebp_v = g12 * 107u + base;
    ebx_v = g16 * 107u + base;
    arr_a = g20 * 107u + base;
    arr_b = g24 * 107u + base;
    arr_c = g28 * 107u + base;
    arr_d = g32 * 107u + base;

    cnt = 0u;
    if (n > 0) {
        ptr = entry_ptr;
        k = 0u;
        for (;;) {
            if ((s8)(u8)cnt >= 0x60) {
                break;
            }
            v = *(volatile u32 *)(ptr + 0x57u);
            if (v == esi_v || v == edi_v || v == ebp_v || v == ebx_v ||
                v == arr_a || v == arr_b || v == arr_c || v == arr_d) {
                *(volatile u32 *)(frame + 0x1cu + (cnt & 0xffu) * 4u) = ptr;
                cnt = (u8)(cnt + 1u);
            }
            k = k + 1u;
            ptr = ptr + 0x245u;
            if (!((s32)(s16)(u16)k < (s32)CM_GLOBAL(volatile u32, 0x00ae235cu))) {
                break;
            }
        }
    }

    cm_fn_00945333((u32)(frame + 0x1cu), (u32)(s32)(s8)(u8)cnt, 4u, 0x0053fd40u);

    count = (s32)(s8)(u8)cnt;
    if ((s16)count <= 0) {
        return 0u;
    }

    i = 0u;
    ptr = (u32)(frame + 0x1cu);
    do {
        if ((s16)(u16)i < 0x0c) {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = esi_v;
        } else if ((s16)(u16)i < 0x18) {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = edi_v;
        } else if ((s16)(u16)i < 0x24) {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = ebp_v;
        } else if ((s16)(u16)i < 0x30) {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = ebx_v;
        } else if ((s16)(u16)i < 0x3c) {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = arr_a;
        } else if ((s16)(u16)i < 0x48) {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = arr_b;
        } else if ((s16)(u16)i < 0x54) {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = arr_c;
        } else {
            *(volatile u32 *)(*(volatile u32 *)ptr + 0x57u) = arr_d;
        }
        ptr = ptr + 4u;
        i = i + 1u;
    } while ((s16)(u16)i < (s16)count);

    return i;
}
