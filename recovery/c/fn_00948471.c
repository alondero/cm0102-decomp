/* WIP: original 00945610 stack-probe data address differs, case 2,
   at the same original instruction VA 00945627 under O0 and O2. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_CDECL cm_fn_0094860a(u32);
extern u32 CM_CDECL cm_fn_00948654(u32);
extern void CM_CDECL cm_fn_00948687(void);
extern u32 CM_CDECL cm_fn_009486b0(void);

u32 CM_CDECL cm_fn_00948471(u32 requested)
{
    u8 scratch[0x18];
    u32 cp, entry, index, i, group, offset, row, cursor, first, last, mask, result;
    cp = cm_fn_0094860a(requested);
    if (cp == D(0x00de4408))
        return 0;
    if (cp == 0) {
        cm_fn_00948687();
        goto finish;
    }
    index = 0;
    for (entry = 0x00ad6c58; entry < 0x00ad6d48; entry += 0x30, ++index) {
        if (D(entry) == cp)
            goto known;
    }
    result = ((u32 (CM_STDCALL *)(u32, u32))D(0x00967128))(cp, (u32)scratch);
    if (result != 1) {
        if (D(0x00de3004) == 0)
            return 0xffffffffu;
        cm_fn_00948687();
        goto finish;
    }
    result = D((u32)scratch);
    D(0x00de4408) = cp;
    for (i = 0; i != 64; ++i)
        D(0x00de4520 + i * 4u) = 0;
    B(0x00de4620) = 0;
    D(0x00de4624) = 0;
    if (result > 1) {
        if (B((u32)scratch + 6) != 0) {
            cursor = (u32)scratch + 7;
            do {
                last = B(cursor);
                if (last == 0)
                    break;
                first = B(cursor - 1);
                while (first <= last) {
                    B(0x00de4521 + first) |= 4;
                    ++first;
                }
                cursor += 2;
            } while (B(cursor - 1) != 0);
        }
        for (i = 1; i < 0xff; ++i)
            B(0x00de4521 + i) |= 8;
        result = cm_fn_00948654(cp);
        D(0x00de4624) = result;
        D(0x00de441c) = 1;
    } else {
        D(0x00de441c) = 0;
    }
    D(0x00de4410) = 0;
    D(0x00de4414) = 0;
    D(0x00de4418) = 0;
    goto finish;

known:
    for (i = 0; i != 64; ++i)
        D(0x00de4520 + i * 4u) = 0;
    offset = index * 0x30u;
    B(0x00de4620) = 0;
    row = offset + 0x00ad6c68;
    for (group = 0; group != 4; ++group, row += 8) {
        if (B(row) != 0) {
            cursor = row;
            do {
                last = B(cursor + 1);
                if (last == 0)
                    break;
                first = B(cursor);
                if (first <= last) {
                    mask = B(0x00ad6c50 + group);
                    do {
                        B(0x00de4521 + first) |= (u8)mask;
                        ++first;
                    } while (first <= last);
                }
                cursor += 2;
            } while (B(cursor) != 0);
        }
    }
    D(0x00de441c) = 1;
    D(0x00de4408) = cp;
    result = cm_fn_00948654(cp);
    row = offset + 0x00ad6c5c;
    first = D(row);
    D(0x00de4410) = first;
    first = D(row + 4);
    D(0x00de4414) = first;
    D(0x00de4624) = result;
    first = D(row + 8);
    D(0x00de4418) = first;
finish:
    cm_fn_009486b0();
    return 0;
}
