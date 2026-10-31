/* Preferred-image VA 0x00859dc0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_004b0700(u32 value);
extern void CM_THISCALL cm_fn_00859e20(u32 self, u32 item, u32 first, u32 second);

u32 CM_THISCALL cm_fn_00859dc0(u32 self)
{
    u32 index = 0, count;
    W(self) = 0;
    count = D(0x00adadf8u);
    if ((s32)count > 0) {
        do {
            u32 table = D(0x00adadfcu);
            u32 item = D(table + index * 4u);
            if (item) {
                u32 record = D(item + 4u);
                u32 value = D(record);
                if (cm_fn_004b0700(value) && (B(item + 0x52u) & 2u))
                    cm_fn_00859e20(self, item, 1, 0);
            }
            count = D(0x00adadf8u);
            ++index;
        } while ((s32)index < (s32)count);
    }
    return count;
}
