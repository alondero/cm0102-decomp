#include "cmre.h"

/* Thiscall with four stack arguments (ret 0x10): p is an entry pointer, q a probe
   address compared with the global base 0x00ae23b4 plus 581 * p[8], r is the
   third argument (its slot address is also passed on), s is an optional output
   word that receives 9 on the failure path. Object fields used: +4 pointer to
   three offsets (+0, +0xc, +0x18) probed through 0x008e8c40. */

extern u32 CM_THISCALL cm_fn_008e8e50(u32 self, u32 arg, u32 *slot);
extern u32 CM_THISCALL cm_fn_008e8c40(u32 self, u32 arg, u32 value);
extern u32 CM_THISCALL cm_fn_008e8800(u32 self, u32 p, u32 q, u32 r, u32 s);

u32 CM_THISCALL cm_fn_00661630(u32 self, u32 p, u32 q, u32 r, u32 s)
{
    u32 base;
    u32 scaled;
    u32 table;
    u32 rv;
    u32 *r_slot;
    u32 r_value;
    u32 probe;

    base = CM_GLOBAL(volatile u32, 0x00ae23b4u);
    scaled = *(volatile u32 *)(p + 8u);
    scaled = base + scaled * 581u;
    r_slot = &r;
    r_value = r;

    if (q == scaled) {
        rv = cm_fn_008e8e50(self, r_value, (u32 *)r_slot);
        if (rv == 0) {
            goto fail;
        }
        table = *(volatile u32 *)(self + 4u);
        probe = cm_fn_008e8c40(self, r_value, table);
        if (probe != 0) {
            goto call_main;
        }
        table = *(volatile u32 *)(self + 4u);
        probe = cm_fn_008e8c40(self, r_value, table + 0xcu);
        if (probe != 0) {
            goto call_main;
        }
        table = *(volatile u32 *)(self + 4u);
        probe = cm_fn_008e8c40(self, r_value, table + 0x18u);
        if (probe == 0) {
            goto fail;
        }
        if (*(volatile u8 *)(p + 0x2fu) == 0) {
            goto fail;
        }
        goto call_main;
    }

    table = *(volatile u32 *)(p + 4u);
    if (table == 0) {
        goto call_main;
    }
    rv = cm_fn_008e8e50(self, r_value, (u32 *)r_slot);
    if (rv == 0) {
        goto fail;
    }
    table = *(volatile u32 *)(self + 4u);
    probe = cm_fn_008e8c40(self, r_value, table);
    if (probe != 0) {
        goto call_main;
    }
    table = *(volatile u32 *)(self + 4u);
    probe = cm_fn_008e8c40(self, r_value, table + 0xcu);
    if (probe != 0) {
        goto call_main;
    }
    table = *(volatile u32 *)(self + 4u);
    probe = cm_fn_008e8c40(self, r_value, table + 0x18u);
    if (probe == 0) {
        goto call_main;
    }

fail:
    if (s != 0) {
        *(volatile u16 *)s = 9;
    }
    return 0u;

call_main:
    rv = cm_fn_008e8800(self, p, q, r_value, s);
    return rv;
}
