#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u8 CM_THISCALL cm_fn_005fa730(u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);

/* Preferred-image VA005e1cd0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   FAST1 ECX/RET0/full Boolean EAX. Scratch210: saved result BYTE L3,
   scan index L4, initialized history outputs L8/Lc, diagnostics L10/L110.
   The 2001 list, 1999 exception, signed WORD counters, and duplicate scan
   against only the original history prefix follow the listing. Callees stay
   original boundaries; no output or diagnostic buffer is invented. */
u32 CM_FASTCALL cm_fn_005e1cd0(u32 self)
{
    static const u32 nation_ids[12] = {
        0x009cf29c, 0x009cf260, 0x009cf2a8, 0x009cf294,
        0x009cf3d0, 0x009cf33c, 0x009cf2d4, 0x009cf428,
        0x009cf4f4, 0x009cf504, 0x009cf420, 0x009cf254
    };
    u32 scratch[0x210 / 4];
    u32 l = (u32)scratch;
    u32 allocation, version, year, id, base, result, array, row;
    u32 filled = 0, history, nation, total, index, limit, pointer, selected;
    u32 duplicate, remaining, entry;

    D(self + 0xf2) = 12;
    allocation = cm_fn_00944e46(48);
    D(self + 0xee) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009cd640, 0, 0, l + 0x110, l + 0x10);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x110, l + 0x10, 0x37b);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    year = (allocation & 0xffff0000u) | W(self + 0x40);
    if ((u16)year == 2001) {
        for (row = 0; row != 12; ++row) {
            id = D(nation_ids[row]);
            base = D(0x00ae23a8);
            result = cm_fn_0053b3d0(base + id * 0x122u);
            array = D(self + 0xee);
            D(array + row * 4u) = result;
        }
        return 1;
    }
    D(l + 8) = 0xffffffffu;
    D(l + 0xc) = 0xffffffffu;
    pointer = D(0x00b5e134);
    id = D(0x009cf7a8);
    result = cm_fn_005fa730(pointer, id, year, l + 8, l + 0xc, 1);
    history = (u32)(s32)(s8)result;
    B(l + 3) = (u8)result;
    if ((s16)history > 0) {
        pointer = l + 8;
        do {
            id = D(pointer);
            base = D(0x00ae23a8);
            result = cm_fn_0053b3d0(base + id * 0x122u);
            array = D(self + 0xee);
            index = (u32)(s32)(s16)filled;
            ++filled;
            pointer += 4u;
            D(array + index * 4u) = result;
        } while ((s16)filled < (s16)history);
    }
    total = D(0x00ae2350);
    nation = D(0x00ae23a8);
    D(l + 4) = 0;
    if ((s32)total > 0) {
        do {
            limit = D(self + 0xf2);
            if ((s32)(s16)filled >= (s32)limit) break;
            pointer = D(nation + 0x71);
            selected = 0;
            if (pointer != 0) {
                id = D(pointer);
                base = D(0x009cfa1c);
                selected = id == base;
            }
            if (!selected) {
                year = W(self + 0x40);
                if ((u16)year == 1999) {
                    id = D(nation);
                    base = D(0x009cf36c);
                    selected = id == base;
                } else {
                    id = D(nation);
                    base = D(0x009cf280);
                    selected = id == base;
                }
                if (!selected) {
                    id = D(nation);
                    base = D(0x009cf3d0);
                    selected = id == base;
                }
            }
            if (selected) {
                remaining = (u32)(s32)(s8)B(l + 3);
                duplicate = 0;
                if ((s32)remaining > 0) {
                    array = D(self + 0xee);
                    do {
                        entry = D(array);
                        if (D(entry + 0x53) == nation) duplicate = 1;
                        array += 4u;
                        --remaining;
                    } while (remaining != 0);
                }
                if (!duplicate) {
                    result = cm_fn_0053b3d0(nation);
                    array = D(self + 0xee);
                    index = (u32)(s32)(s16)filled;
                    ++filled;
                    D(array + index * 4u) = result;
                }
            }
            index = D(l + 4);
            total = D(0x00ae2350);
            ++index;
            nation += 0x122u;
            D(l + 4) = index;
        } while ((s32)(s16)index < (s32)total);
    }
    limit = D(self + 0xf2);
    index = (u32)(s32)(s16)filled;
    if (index != limit) {
        cm_fn_00944cff(0x009cd640, 0, 0, l + 0x10, l + 0x110);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x10, l + 0x110, 0x3bc);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    array = D(self + 0xee);
    cm_fn_00945333(array, index, 4, 0x004b07d0);
    return 1;
}
