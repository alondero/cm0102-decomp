#include "cmre.h"

/* Original instruction evidence: preferred-image VAs 00556830-00556938,
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receivers and stack arguments are set at 0055685f-62,
   00556883-8c, 005568a1-a5, 005568f1-f6 and 0055690a-17.
   Entry consumes ECX and RET 4; final EAX is FFFFFFFF (00556925). */
extern void CM_THISCALL cm_fn_0055e1d0(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, u32 create);
extern u32 CM_THISCALL cm_fn_00556d00(u32 self, u32 key, s16 arg);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);

u32 CM_THISCALL cm_fn_00556830(u32 self, u32 key)
{
    u32 offset = key * 29u;
    u32 base = CM_GLOBAL(volatile u32, self + 0x14) + offset;
    u32 cursor;
    u32 i;
    u32 node;
    cm_fn_0055e1d0(self,
        CM_GLOBAL(volatile u32, 0x00ae23bc) + key * 110u);
    cursor = base;
    for (i = 0; i < 4; ++i) {
        s8 tag = CM_GLOBAL(volatile s8, cursor);
        if (tag != -1) {
            u32 table = CM_GLOBAL(volatile u32, self);
            u32 receiver = CM_GLOBAL(volatile u32, table + (u32)(s32)tag * 4u);
            node = cm_fn_007e82e0(receiver, key, 0);
            if (node != 0) {
                s16 value = CM_GLOBAL(volatile s16, node + 6);
                if (value > -1)
                    cm_fn_00556d00(self, key, value);
                {
                    u32 p = CM_GLOBAL(volatile u32, self + 0x14);
                    CM_GLOBAL(volatile u8, p + offset) = 0xff;
                    p = CM_GLOBAL(volatile u32, self + 0x14);
                    CM_GLOBAL(volatile u32, p + offset + 1) = 0xffffffff;
                }
                CM_GLOBAL(volatile u32, node) = 0xffffffff;
            }
        }
        cursor += 5;
        offset += 5;
    }
    {
        u32 p = CM_GLOBAL(volatile u32, self + 0x1e);
        self += 0x1c;
        CM_GLOBAL(volatile u32, self + 0xa) = p;
    }
    node = cm_fn_0068d1b0(self);
    while (node != 0) {
        if (CM_GLOBAL(volatile s8, base + 0x1c) <= 0)
            break;
        if (CM_GLOBAL(volatile u32, node) == key) {
            cm_fn_0068d1f0((u8 *)self, (const u8 *)node);
            CM_GLOBAL(volatile u8, base + 0x1c) =
                CM_GLOBAL(volatile u8, base + 0x1c) - 1;
        }
        node = cm_fn_0068d1b0(self);
    }
    for (i = 0; i < 7; ++i)
        CM_GLOBAL(volatile u32, base + 4 * i) = 0xffffffff;
    CM_GLOBAL(volatile u8, base + 28) = 0xff;
    CM_GLOBAL(volatile u8, base + 0x1c) = 0;
    return 0xffffffff;
}
