#include "cmre.h"

/* Preferred-image VA 005b47a0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is ESP after SUB 274: masks L8, minimum index L18, coordinates L1c,
   used flags L48, diagnostic buffers L74/L174. L4 is deliberately not
   initialized: its early reads are dead until a maximum supplies the index. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
typedef u16 alias_u16 __attribute__((may_alias));
extern u32 CM_THISCALL cm_fn_005b3c70(u32, s8, s8, u32, u32);
extern u32 CM_THISCALL cm_fn_005b3560(u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_0090ba90(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_005b47a0(u32 self, volatile u32 alternate,
                              volatile u32 grid)
{
    u8 scratch[0x274];
    u32 l = (u32)scratch, i, mask, maximum_index, minimum_index;
    u32 line, filename, extension, version, value, mode;
    s16 minimum, maximum, coordinate;
    s16 x, y;

    mode = alternate;
    W(l + 8) = 1; W(l + 10) = 2; W(l + 12) = 4; W(l + 14) = 8;
    W(l + 16) = 0x10; W(l + 18) = 0x20; W(l + 20) = 0x40;
    W(l + 22) = 0xffff;
    if ((s8)mode < 0 || (s8)mode > 1) {
        filename = l + 0x74; extension = l + 0x174; line = 0x4c9;
        goto error;
    }
    x = (s16)*(volatile alias_u16 *)&grid;
    if (x < 0 || x > 2) goto invalid_grid;
    y = (s16)*((volatile alias_u16 *)&grid + 1);
    if (y < 0 || y > 3) goto invalid_grid;
    for (i = 0; i < 11; ++i) {
        if (i != 0) mode = alternate;
        value = grid;
        cm_fn_005b3c70(self, (s8)i, (s8)mode, value, l + 0x1c + i * 4);
        W(l + 0x1c + i * 4) = (u16)(0x1a - W(l + 0x1c + i * 4));
    }
    maximum_index = D(l + 4);
    for (i = 0; i < 11; ++i) D(l + 0x48 + i * 4) = 0;
    D(l) = l + 8;
    for (;;) {
        maximum = -1;
        minimum = 0x1b;
        for (i = 0; i < 11; ++i) {
            if (D(l + 0x48 + i * 4) != 0) continue;
            mask = D(l);
            value = W(mask);
            if ((W(self + 0x12d + i * 2) & value) == 0) continue;
            coordinate = (s16)W(l + 0x1c + i * 4);
            if (coordinate < minimum) {
                maximum_index = D(l + 4);
                minimum = coordinate;
                D(l + 0x18) = i;
            }
            if (coordinate > maximum) {
                maximum_index = i;
                maximum = coordinate;
                D(l + 4) = maximum_index;
            }
        }
        if (minimum < 0x1b && maximum > -1) {
            minimum_index = D(l + 0x18);
            if (minimum_index != maximum_index) {
                cm_fn_0090ba90((void *)(l + 0x1c + minimum_index * 4),
                               (void *)(l + 0x1c + maximum_index * 4), 4);
                D(l + 0x48 + minimum_index * 4) = 1;
                D(l + 0x48 + maximum_index * 4) = 1;
                continue;
            }
        }
        value = D(l) + 2;
        D(l) = value;
        if (W(value) == 0xffff) break;
    }
    value = grid;
    *(volatile alias_u16 *)&grid = (u16)(2u - value);
    value = grid;
    for (i = 0; i < 11; ++i) {
        u32 packed = D(l + 0x1c + i * 4);
        mode = alternate;
        cm_fn_005b3560(self, i, mode, value, packed);
    }
    D(self + 0x187f) = 0;
    return 1;

invalid_grid:
    filename = l + 0x174; extension = l + 0x74; line = 0x4d2;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
