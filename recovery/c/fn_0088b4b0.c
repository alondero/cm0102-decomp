#include "cmre.h"
/* Original-instruction authority: preferred-image VAs 0088b4b0-0088b87f,
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX=B74450 and one pushed argument at 0088b4b7-0088b4be,
   0088b51e-0088b525 and 0088b52a-0088b530 establish getter ABIs.
   Three pushes and ECX=B74450 at 0088b5e8-0088b5f4 (and
   0088b51c-0088b53d, including the earlier push 0) establish setter ABI.
   Entry reads stack word at 0088b4fd; exits use ret without cleanup. */
extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 key);
extern u32 CM_THISCALL cm_fn_00803320(u32 self, u32 index_arg);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 key, u32 value, u32 zero);

static u32 field(s16 index, s16 bank, u32 offset)
{
    u32 base = CM_GLOBAL(volatile u32,
        0x00b74c4cu + (u32)(s32)bank * 768u);
    return CM_GLOBAL(volatile u32,
        base + (u32)(s32)index * 396u + offset);
}

static u16 command(s16 index, s16 bank, u16 fallback)
{
    u32 value = index == -1 ? 0 : field(index, bank, 0xba966u);
    return (u16)value == 0 ? fallback : (u16)value;
}

u32 CM_CDECL cm_fn_0088b4b0(s16 index)
{
    u32 ptr, flags, old, page, limit, value, target, map;
    s16 bank;
    u16 fallback, cmd;
    s32 next;
    ptr = cm_fn_007ff010(0xb74450u, 0);
    flags = cm_fn_007ff010(0xb74450u, 5);
    old = cm_fn_007ff010(0xb74450u, 14);
    page = cm_fn_007ff010(0xb74450u, 15);
    limit = cm_fn_007ff010(0xb74450u, 16);
    if (!ptr || CM_GLOBAL(volatile s32, ptr) <
                CM_GLOBAL(volatile s32, 0xae2364u)) {
        if (index != -1) {
            value = cm_fn_007ff010(0xb74450u, 10);
            value = cm_fn_00803320(0xb74450u, value);
            cm_fn_007ff270(0xb74450u, 9, value, 0);
        }
    }
    bank = CM_GLOBAL(volatile s16, 0xb77c7au);
    value = index == -1 ? 0 : field(index, bank, 0xba966u);
    fallback = CM_GLOBAL(volatile u16, 0xdd6bdeu);
    cmd = (u16)value == 0 ? fallback : (u16)value;
    if (cmd == 0x14) {
        if (index != -1) {
            u32 base = CM_GLOBAL(volatile u32,
                0xb74c4cu + (u32)(s32)bank * 768u);
            fallback = CM_GLOBAL(volatile u16, 0xdd6bdeu);
            value = CM_GLOBAL(volatile u32,
                base + (u32)(s32)index * 396u + 0xba9a6u);
        } else {
            value = 0;
        }
        if ((flags & value) == 0) {
            if (value - 1u <= 15u) {
                map = CM_GLOBAL(volatile u8, 0x88b888u + value - 1u);
                target = CM_GLOBAL(volatile u32, 0x88b880u + map * 4u);
                if (target == 0x88b5e5u)
                    cm_fn_007ff270(0xb74450u, 5,
                                  (flags & 0xffffffe0u) | value, 0);
            }
            return 0xfffffff5u;
        }
    }
    if (command(index, bank, fallback) == 0x20) {
        value = index == -1 ? 0 : field(index, bank, 0xba9a6u);
        if (old != value)
            cm_fn_007ff270(0xb74450u, 14, value, 0);
        cm_fn_007ff270(0xb74450u, 15, 1, 0);
        return 0xfffffff5u;
    }
    if (command(index, bank, fallback) == 0x3e) {
        cm_fn_007ff270(0xb74450u, 21, 1, 0);
        return 0xfffffff5u;
    }
    if (command(index, bank, fallback) == 0x3f) {
        cm_fn_007ff270(0xb74450u, 21, 0, 0);
        return 0xfffffff5u;
    }
    if (command(index, bank, fallback) == 0x21) {
        next = (s16)page - 1;
        if (next < 1) next = 1;
        cm_fn_007ff270(0xb74450u, 15, (u32)next, 0);
        return 0xfffffff5u;
    }
    if (command(index, bank, fallback) == 0x22) {
        next = (s16)page + 1;
        if ((s16)limit < next) next = (s16)limit;
        cm_fn_007ff270(0xb74450u, 15, (u32)next, 0);
        return 0xfffffff5u;
    }
    if (command(index, bank, fallback) == 0x15) {
        value = index == -1 ? 0 : field(index, bank, 0xba9a6u);
        cm_fn_007ff270(0xb74450u, 8, value, 0);
        return 0xfffffff5u;
    }
    return 0;
}
