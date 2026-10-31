#include "cmre.h"

/* Preferred-image VA005dc700, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX. Scratch218: six initialized DWORD slots L0..17,
   diagnostics L18/L118. First scan caches the global count; fallback reloads
   it every iteration. Duplicate slots still increment the original counter. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_004afd80(u32, u8);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005dc700(u32 self)
{
    u8 scratch[0x218];
    u32 l = (u32)scratch, country, base, target, count, pointer, index;
    u32 found = 0, slot, value, nation, wanted, object, offset;
    u32 name, extension, line, continuation;
    u8 rank;
    W(self + 0x3e) = 6;
    country = D(0x009cf54c);
    base = D(0x00ae23d0);
    target = country * 0x6bu;
    for (index = 0; index < 6; ++index) D(l + index * 4) = 0;
    count = D(0x00ae235c);
    pointer = D(0x00ae23b4);
    target += base;
    index = 0;
    if ((s32)count <= 0) goto first_diagnostic;
    do {
        if ((s16)(u16)found >= 6) goto scan_finished;
        if (D(pointer + 0x5b) == target) {
            rank = B(pointer + 0x5f);
            if ((s8)rank < 7 && (s8)rank > 0) {
                ++found;
                D(l + (u32)(s32)(s8)rank * 4u - 4u) = pointer;
            }
        }
        ++index;
        pointer += 0x245;
    } while ((s32)index < (s32)count);
scan_finished:
    if ((u16)found == 6) goto allocate;
first_diagnostic:
    name = l + 0x118; extension = l + 0x18; line = 0x11d;
    continuation = 1;
    goto diagnostic;
fallback:
    index = 0;
    count = D(0x00ae235c);
    pointer = D(0x00ae23b4);
    D(0x00b67a34) = 0;
    if ((s32)count <= 0) goto allocate;
    do {
        if ((s16)(u16)found >= 6) goto allocate;
        nation = D(pointer + 0x53);
        if (nation != 0) {
            wanted = D(0x009cf314);
            if (D(nation) == wanted && D(pointer + 0x5b) == target &&
                (s8)B(pointer + 0x5f) > 6) {
                for (slot = 0; slot < 6; ++slot) {
                    if (D(l + slot * 4) == 0) {
                        D(l + slot * 4) = pointer;
                        break;
                    }
                }
                ++found;
            }
        }
        count = D(0x00ae235c);
        ++index;
        pointer += 0x245;
    } while ((s32)index < (s32)count);
allocate:
    count = (u32)(s32)(s16)W(self + 0x3e);
    object = cm_fn_00945501(count, 6);
    D(self + 0xa7) = object;
    if (object == 0) {
        name = l + 0x18; extension = l + 0x118; line = 0x13d;
        continuation = 2;
        goto diagnostic;
    }
    country = D(0x009cf714);
    base = D(0x00ae23d0);
    value = cm_fn_004afd80(base + country * 0x6bu, 0);
    object = D(self + 0xa7);
    D(object + 0x1e) = value;
    object = D(self + 0xa7);
    if (D(object + 0x1e) == 0) {
        name = l + 0x18; extension = l + 0x118; line = 0x146;
        continuation = 3;
        goto diagnostic;
    }
    offset = 0x18;
    do {
        for (slot = 0; slot < 6; ++slot) {
            value = D(l + slot * 4);
            if (value == 0) continue;
            object = D(self + 0xa7);
            if (value == D(object + 0x1e)) continue;
            object = D(self + 0xa7);
            value = D(l + slot * 4);
            D(object + offset) = value;
            D(l + slot * 4) = 0;
            break;
        }
        offset -= 6;
    } while ((s32)offset >= 0);
    offset = 0;
    do {
        object = D(self + 0xa7);
        B(object + offset + 4) = 0;
        object = D(self + 0xa7);
        B(object + offset + 5) = 0;
        if ((s32)offset >= 0x18) {
            object = D(self + 0xa7);
            B(object + offset + 4) = 1;
        }
        offset += 6;
    } while ((s32)offset < 0x24);
    return 1;
diagnostic:
    cm_fn_00944cff(0x009cd450, 0, 0, name, extension);
    value = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    if (continuation == 1) goto fallback;
    D(0x00b67a34) = 0;
    return continuation == 2 ? 0xffffffffu : 0;
}
