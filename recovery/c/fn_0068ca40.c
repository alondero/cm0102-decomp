#include "cmre.h"

/* Preferred-image VA 0068CA40, base 00400000; pinned 3.9.68 image.
   L is the original 218-byte scratch area. Callees remain original boundaries. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_CDECL cm_fn_00945ce6(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

void CM_THISCALL cm_fn_0068ca40(u32 self, u32 stream)
{
    u8 scratch[0x218];
    u32 L = (u32)scratch, p, q, value, count, index, offset, inner;
    u32 rows = self + 0x3e, field;
    value = D(self + 0xba);
    D(L + 0x14) = 0xffffffff;
    D(L + 0x0c) = 0;
    D(L + 0x08) = 0;
    if (value == 0 || D(self + 0xb1) == 0) {
        cm_fn_00944cff(0x9f1630, 0, 0, (void *)(L + 0x18), (void *)(L + 0x118));
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, value, L + 0x18, L + 0x118, 0x259);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        return;
    }
    cm_fn_00945ce6(self + 0xe1, 1, 1, stream);
    cm_fn_00945ce6(self + 0x28, 4, 1, stream);
    cm_fn_00945ce6(self + 0xdd, 4, 1, stream);
    cm_fn_00945ce6(self + 0xa7, 2, 1, stream);
    cm_fn_00945ce6(self + 0x51, 1, 1, stream);
    cm_fn_00945ce6(self + 0x49, 1, 1, stream);
    cm_fn_00945ce6(self + 0x4a, 1, 1, stream);
    cm_fn_00945ce6(self + 0x3c, 2, 1, stream);
    cm_fn_00945ce6(rows, 2, 1, stream);
    cm_fn_00945ce6(self + 0x40, 2, 1, stream);
    cm_fn_00945ce6(self + 0x43, 1, 1, stream);
    cm_fn_00945ce6(self + 0x42, 1, 1, stream);
    cm_fn_00945ce6(self + 0x3a, 2, 1, stream);
    p = D(self + 4);
    if (p != 0) {
        cm_fn_00945ce6(p, 4, 1, stream);
    } else {
        cm_fn_00944cff(0x9f1630, 0, 0, (void *)(L + 0x118), (void *)(L + 0x18));
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, value, L + 0x118, L + 0x18, 0x276);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        cm_fn_00945ce6(L + 0x14, 4, 1, stream);
    }
    cm_fn_00945ce6(self + 0xa9, 2, 1, stream);
    cm_fn_00945ce6(self + 0xc2, 1, 1, stream);
    cm_fn_00945ce6(self + 0xc3, 1, 1, stream);
    cm_fn_00945ce6(self + 0xc4, 1, 1, stream);
    cm_fn_00945ce6(self + 0xc5, 4, 1, stream);
    cm_fn_00945ce6(self + 0xbe, 4, 1, stream);
    cm_fn_00945ce6(self + 0xd9, 2, 1, stream);
    cm_fn_00945ce6(self + 0xdb, 2, 1, stream);
    cm_fn_00945ce6(self + 0xab, 2, 1, stream);
    cm_fn_00945ce6(self + 0x1c, 4, 1, stream);
    cm_fn_00945ce6(self + 0x20, 4, 1, stream);
    cm_fn_00945ce6(self + 0x44, 1, 1, stream);
    count = (u32)(int)(short)W(self + 0xa9);
    p = D(self + 0xba);
    cm_fn_00945ce6(p, 0x41, count, stream);
    cm_fn_00945ce6(self + 0x18, 4, 1, stream);
    cm_fn_00945ce6(self + 0x34, 2, 1, stream);
    p = D(self + 0x18);
    if (p != 0) {
        count = (u32)(int)(short)W(self + 0x34);
        cm_fn_00945ce6(p, 0x13, count, stream);
    }
    if ((short)W(rows) <= 0) return;
    D(L + 4) = 0;
    D(L) = 0;
    offset = 0;
    do {
        p = D(self + 0xb1);
        field = offset + p;
        value = D(field);
        if (value != 0) value = D(value);
        else value = 0xffffffff;
        D(field) = value;
        p = D(self + 0xb1);
        value = D(p + 5);
        if (value != 0) {
            count = (u32)((int)(short)W(rows) - 1);
            index = 0;
            D(L + 0x0c) = 1;
            if ((int)count > 0) {
                inner = 0;
                do {
                    p = D(self + 0xb1);
                    offset = D(L);
                    q = D(offset + p + 5);
                    field = q + inner;
                    value = D(field);
                    if (value != 0) value = D(value);
                    else value = 0xffffffff;
                    D(field) = value;
                    count = (u32)(int)(short)W(rows);
                    ++index;
                    inner += 0xe;
                } while ((int)index < (int)count - 1);
            }
        }
        index = D(L + 4);
        offset = D(L);
        count = (u32)(int)(short)W(rows);
        ++index;
        offset += 0x3b;
        D(L + 4) = index;
        D(L) = offset;
    } while ((int)index < (int)count);
    count = (u32)(int)(short)W(rows);
    p = D(self + 0xb1);
    cm_fn_00945ce6(p, 0x3b, count, stream);
    cm_fn_00945ce6(L + 0x0c, 4, 1, stream);
    value = D(L + 0x0c);
    if (value != 0) {
        count = (u32)(int)(short)W(rows);
        index = 0;
        if ((int)count > 0) {
            D(L) = 0;
            do {
                offset = D(L);
                --count;
                p = D(self + 0xb1);
                q = D(offset + p + 5);
                cm_fn_00945ce6(q, 0xe, count, stream);
                count = (u32)(int)(short)W(rows);
                offset = D(L);
                ++index;
                offset += 0x3b;
                D(L) = offset;
            } while ((int)index < (int)count);
        }
    }
    p = D(self + 0xb1);
    value = D(p + 9);
    if (value != 0) {
        if (W(self + 0xa7) == 0xffff) {
            u16 product = (u16)(W(rows) - 1u);
            product = (u16)((u32)product * W(self + 0x3c));
            W(self + 0xa7) = product;
        }
        D(L + 8) = 1;
        cm_fn_00945ce6(L + 8, 4, 1, stream);
        index = 0;
        if ((short)W(rows) > 0) {
            D(L) = 0;
            do {
                count = (u32)(int)(short)W(self + 0xa7);
                p = D(self + 0xb1);
                offset = D(L);
                q = D(offset + p + 9);
                cm_fn_00945ce6(q, count, 1, stream);
                offset = D(L);
                offset += 0x3b;
                ++index;
                D(L) = offset;
                count = (u32)(int)(short)W(rows);
            } while ((int)index < (int)count);
        }
    } else {
        cm_fn_00945ce6(L + 8, 4, 1, stream);
    }
    offset = 0;
    count = W(rows);
    D(L + 4) = 0;
    if ((short)count <= 0) return;
    D(L + 0x10) = 0;
    do {
        p = D(self + 0xb1);
        field = offset + p;
        value = D(field);
        if (value != 0xffffffff) {
            p = D(0xae23b4);
            value = p + value * 0x245u;
        } else value = 0;
        D(field) = value;
        p = D(self + 0xb1);
        inner = 0;
        if (D(p + 5) != 0) {
            count = (u32)((int)(short)W(rows) - 1);
            D(L) = 0;
            if ((int)count > 0) {
                do {
                    p = D(self + 0xb1);
                    q = D(offset + p + 5);
                    field = q + inner;
                    value = D(field);
                    if (value != 0xffffffff) {
                        p = D(0xae23b4);
                        value = p + value * 0x245u;
                        offset = D(L + 0x10);
                    } else value = 0;
                    D(field) = value;
                    count = (u32)(int)(short)W(rows);
                    index = D(L);
                    inner += 0xe;
                    ++index;
                    D(L) = index;
                } while ((int)index < (int)count - 1);
            }
        }
        index = D(L + 4);
        offset += 0x3b;
        count = (u32)(int)(short)W(rows);
        ++index;
        D(L + 0x10) = offset;
        D(L + 4) = index;
    } while ((int)index < (int)count);
}
