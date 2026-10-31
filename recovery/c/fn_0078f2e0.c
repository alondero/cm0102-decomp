#include "cmre.h"

/* Preferred-image VA0078F2E0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Each of seven date/store rows is independently checked. Original549F70
   masks its DWORD year to WORD before use and initializes all eight bytes. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0078f2e0(u32 self, u32 selector, u32 count_out,
                              u32 zero_out, u32 optional_out)
{
    static const struct {
        u32 first[4], last[5];
        u16 value7, valued, valuef, value18, value1a, value1c;
        u8 value17, value20, value21, value22;
    } rows[7] = {
        {{7,3,0,2}, {5,4,0,2,2}, 10,0x403,0,128,64,128,4,0,1,0},
        {{6,4,0,3}, {26,4,0,2,2}, 20,0x403,0,64,32,0,4,0,1,0},
        {{27,4,0,3},{2,5,0,2,2}, 30,3,0,32,16,0,4,0,1,0},
        {{3,5,0,3}, {30,5,0,2,2},40,2,3,16,8,0,4,1,0,0},
        {{1,6,0,3}, {4,7,0,2,2},120,2,3,8,4,0,6,1,1,0},
        {{5,7,0,3}, {25,8,0,5,1},130,2,3,4,2,0,6,1,1,3},
        {{29,8,0,2},{31,9,0,6,1},150,2,3,2,1,0,6,1,0,3}
    };
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x218];
    u32 L = (u32)scratch, p, result, year, i, r, day, first, second;
    u16 left, right;
    u8 mode;
    frame.prev = CM_FS(volatile u32, 0);
    frame.state = 0xffffffff;
    frame.handler = 0x95e53b;
    mode = (u8)selector;
    CM_FS(volatile u32, 0) = (u32)&frame;
    if (mode != 0xff) { result = 0; goto done; }
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 7;
    W(zero_out) = 0;
    p = cm_fn_00944e46(0x2d8);
    if (p == 0) {
        cm_fn_00944cff(0xa69708, 0, 0, (void *)(L + 0x118), (void *)(L + 0x18));
        result = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, result, L + 0x118, L + 0x18, 0xa5);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        result = 0;
        goto done;
    }
    for (i = 0; i != 7; ++i) {
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521e60(p, i, rows[i].first[0], rows[i].first[1],
                      rows[i].first[2], rows[i].first[3], year);
        day = rows[i].last[0];
        if (i == 6) {
            year = W(self + 0x40);
            r = cm_fn_00549f70(L + 0x10, 1, 10, year, 0xffffffff);
            first = D(r);
            second = D(r + 4);
            year = W(self + 0x40);
            D(L) = first;
            frame.state = 0;
            D(L + 4) = second;
            r = cm_fn_00549f70(L + 8, 31, 9, year, 6);
            right = W(r + 2);
            left = W(L + 2);
            if (left < right) result = 1;
            else if (left > right) result = 0;
            else {
                left = W(L);
                right = W(r);
                result = (s16)left < (s16)right;
            }
            frame.state = 0xffffffff;
            if (result != 0) day = 24;
        }
        year = (u32)(s32)(s16)W(self + 0x40);
        cm_fn_00521eb0(p, i, day, rows[i].last[1], rows[i].last[2],
                      rows[i].last[3], rows[i].last[4], year, i == 6 ? 4 : 0);
        r = p + i * 104u;
        W(r + 7) = rows[i].value7;
        W(r + 9) = (u16)i;
        W(r + 0xb) = 0;
        W(r + 0xd) = rows[i].valued;
        if (i != 0) W(r + 0xf) = rows[i].valuef;
        B(r + 0x17) = rows[i].value17;
        W(r + 0x18) = rows[i].value18;
        W(r + 0x1a) = rows[i].value1a;
        W(r + 0x1c) = rows[i].value1c;
        if (i == 0) W(r + 0xf) = 0;
        W(r + 0x1e) = 0;
        B(r + 0x20) = rows[i].value20;
        B(r + 0x21) = rows[i].value21;
        B(r + 0x22) = rows[i].value22;
        D(r + 0x5c) = 0;
        D(r + 0x60) = 0xffffffff;
        D(r + 0x64) = 0;
    }
    result = p;
done:
    first = frame.prev;
    CM_FS(volatile u32, 0) = first;
    return result;
}
