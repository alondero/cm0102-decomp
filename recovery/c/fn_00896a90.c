#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_008982b0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00896a90(u32 self, u32 source, u8 selector,
                               u32 target, volatile u32 key)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 name, extension, line, text, index, base, value, cursor, remaining;
    u8 count;
    if (source == 0) {
        name = local + 0x100;
        extension = local;
        line = 0x315;
        goto error;
    }
    if ((s8)selector < 0 || (s8)selector > 19) {
        name = local;
        extension = local + 0x100;
        line = 0x31d;
        goto error;
    }
    if (target == 0) {
        name = local;
        extension = local + 0x100;
        line = 0x325;
        goto error;
    }
    index = cm_fn_008982b0(self, source, key);
    if (index == 0xffffffffu)
        return 0;
    base = D(self + 0x601);
    base += (u32)(s32)(s8)selector * 4u;
    value = D(target);
    D(base + index * 145u + 2) = value;
    if (B(self + 0x5f0) != 0) {
        cursor = self + 0x38;
        remaining = 16;
        do {
            if (D(cursor) != 0 && D(cursor - 8) == source) {
                value = key;
                if (D(cursor - 4) == value) {
                    D(cursor) = 0;
                    count = B(self + 0x5f0);
                    B(self + 0x5f0) = (u8)(count - 1u);
                }
            }
            cursor += 0x5c;
        } while (--remaining != 0);
    }
    return 1;
error:
    cm_fn_00944cff(0x00a8b908, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
