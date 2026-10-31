#include "cmre.h"

/* Cdecl, one stack argument (a pointer to a u32). Global 0x009b5984 selects an
   early return (arg + 4). Global 0x00ae2408 holds the lazily allocated object;
   its fields used here: 0x14 (compared with *arg), 0x28 and 0x38 (added to the
   scaled result), and 0x20..0x3c cleared right after allocation. */

extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_FASTCALL cm_fn_00544fc0(u32 self);

u32 CM_CDECL cm_fn_005455a0(u32 arg)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 g1;
    u32 g2;
    u32 obj;
    u32 a;
    u32 b;
    u32 d;
    u32 result;
    u32 i;

    seh.state = -1;
    seh.handler = 0x0095799bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    g1 = CM_GLOBAL(volatile u32, 0x009b5984u);
    if (g1 == 0u || g1 == 1u) {
        CM_FS(u32, 0) = seh.prev;
        return arg + 4u;
    }

    g2 = CM_GLOBAL(volatile u32, 0x00ae2408u);
    if (g2 == 0u) {
        obj = cm_fn_00944cf1(0x40u);
        seh.state = 0;
        if (obj != 0u) {
            for (i = 0u; i < 8u; i++) {
                *(volatile u32 *)(obj + 0x20u + i * 4u) = 0u;
            }
            cm_fn_00544fc0(obj);
            g2 = obj;
        } else {
            g2 = 0u;
        }
        CM_GLOBAL(volatile u32, 0x00ae2408u) = g2;
        if (g2 == 0u) {
            CM_FS(u32, 0) = seh.prev;
            return 0u;
        }
    }

    b = *(volatile u32 *)(g2 + 0x14u);
    a = *(volatile u32 *)arg;
    if ((s32)a < (s32)b) {
        result = a * 83u + *(volatile u32 *)(g2 + 0x38u);
    } else {
        d = a - b;
        result = d * 83u + *(volatile u32 *)(g2 + 0x28u);
    }

    CM_FS(u32, 0) = seh.prev;
    return result;
}
