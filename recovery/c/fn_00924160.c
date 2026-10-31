#include "cmre.h"

/* Stack arguments (stdcall, ret 0xc):
   arg1 at entry+4: object passed as ECX to the 0x00785830 and 0x004ae660/0x004ae8a0
     calls. In the non-zero arg2 path its stack slot is reused as scratch: the
     signed byte computed from the 0x0ae23d0 row is stored there and reloaded.
   arg2 at entry+8: selects the path (zero: table A, non-zero: table B).
   arg3 at entry+0xc: object passed as ECX to the final calls (reloaded after the
     11-argument varargs call). */

/* Read a signed byte at a computed address; the value is sign-extended to 32 bits. */
static u32 load_s8(u32 addr)
{
    return (u32)(s32)*(volatile s8 *)(addr);
}

extern unsigned long long CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 arg1, u32 fmt, ...);

void CM_STDCALL cm_fn_00924160(u32 arg1, u32 arg2, u32 arg3)
{
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 g_b4;
    u32 g_d0;
    u32 a;
    u32 x0;
    u32 x1;
    u32 y;
    u32 s0;
    u32 s1;
    u32 v_s;
    u32 v_b;
    u32 v_d;
    u32 v_e;
    u32 v_c;
    u32 fmt;

    if (arg2 == 0u) {
        r0 = (u32)cm_fn_00785830(arg1, 0u);
        r1 = (u32)cm_fn_00785830(arg1, 1u);
        r2 = (u32)cm_fn_00785830(arg1, 2u);
        g_d0 = CM_GLOBAL(volatile u32, 0x00ae23d0);
        g_b4 = CM_GLOBAL(volatile u32, 0x00ae23b4);

        a = r2 * 107u + g_d0 + 4u;
        x1 = r1 * 581u + g_b4;
        x0 = r0 * 581u + g_b4;

        v_s = load_s8(a + 0x33u);
        v_b = load_s8(x0 + 0x52u);
        v_d = load_s8(x1 + 0x52u);

        cm_fn_0066f4e0(0xde1f64u, 0xad55f4u,
                       v_b, v_b, v_d, v_d, v_s, v_s,
                       x0 + 0x38u, x1 + 0x38u, a);

        cm_fn_004ae660(arg3, 0xde1f64u);
        return;
    }

    r0 = (u32)cm_fn_00785830(arg1, 0u);
    r1 = (u32)cm_fn_00785830(arg1, 1u);
    r2 = (u32)cm_fn_00785830(arg1, 2u);
    r3 = (u32)cm_fn_00785830(arg1, 3u);

    /* Select the format row from the low byte of the fourth result. Each row uses
       the same register arithmetic; only the format string differs. */
    if ((r3 & 0x10u) != 0u) {
        fmt = 0x00ad5520u;
    } else if ((r3 & 0x04u) != 0u) {
        fmt = 0x00ad544cu;
    } else if ((r3 & 0x08u) != 0u) {
        fmt = 0x00ad5378u;
    } else {
        fmt = 0x00ad52b0u;
    }

    g_b4 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    g_d0 = CM_GLOBAL(volatile u32, 0x00ae23d0);

    s0 = r0 * 581u;
    s1 = r1 * 581u;
    x1 = s1 + g_b4;
    x0 = s0 + g_b4;
    y = g_d0 + r2 * 107u;

    v_e = load_s8(x1 + 0x37u);
    v_c = load_s8(y + 0x37u);
    v_b = load_s8(x0 + 0x37u);

    /* The original stores v_c into the first argument slot and reloads it. */
    arg1 = v_c;
    v_c = arg1;

    cm_fn_0066f4e0(0xde1f64u, fmt,
                   v_c, v_c, v_b, v_b, v_e, v_e,
                   y + 4u, x0 + 4u, x1 + 4u);

    cm_fn_004ae660(arg3, 0xde1f64u);

    g_b4 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    x0 = s0 + g_b4;
    cm_fn_004ae8a0(arg3, x0 + 4u, 0x7d5u, x0);

    g_b4 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    x1 = s1 + g_b4;
    cm_fn_004ae8a0(arg3, x1 + 4u, 0x7d5u, x1);
}
