/* WIP: ESI mismatch, cases 5/9/17 (eflags 0x602, DF set); report path calls 0x0090bde0 (string ops, DF gap) */
#include "cmre.h"

/* Plain cdecl routine with three stack arguments (the caller pops them):
   arg1 at entry esp+4, arg2 at entry esp+8, arg3 at entry esp+0xc.
   Returns 32-bit EAX: 0 or 1 on every path. The ECX object 0x00b74450 is an
   immediate address (not a memory read) passed as `this` or fastcall argument.
   Structure mirrors 0x0041c100 with 0x0041cbd0, 0x004afd80 and [arg1+0x5d]. */

extern u32 CM_FASTCALL cm_fn_007fee80(u32 p);
extern void CM_THISCALL cm_fn_007feeb0(u32 self, u32 p);
extern u32 CM_CDECL cm_fn_0041c9d0(u32 p1, u32 p2);
extern u32 CM_CDECL cm_fn_0090bde0(u32 p1, u32 p2);
extern u32 CM_CDECL cm_fn_0041cad0(u32 argument, u32 lane);
extern u32 CM_CDECL cm_fn_0041cbd0(u32 p1, u32 p2);
extern u32 CM_CDECL cm_fn_004afd80(u32 p1, s8 p2);
extern void CM_CDECL cm_fn_0041bfb0(void);

/* Listing 0x0041c455..0x0041c49a path tails and 0x0041c4bc..0x0041c4d8,
   0x0041c4f4..0x0041c50e, 0x0041c531..0x0041c54b: format through 0x0090bde0 with
   the string at 0x009899e0, then report through this = 0x00b74450 and return 1. */
static u32 report(u32 obj, u32 r)
{
    u32 p;

    p = cm_fn_0090bde0((u32)&CM_GLOBAL(u8, 0x009899e0), r);
    cm_fn_007feeb0(obj, p);
    return 1u;
}

u32 CM_CDECL cm_fn_0041c410(u32 arg1, u32 arg2, u32 arg3)
{
    u32 obj;
    u32 r;
    u32 e;

    obj = (u32)&CM_GLOBAL(u8, 0x00b74450);

    r = cm_fn_007fee80(obj);
    if (*(volatile u8 *)r != 0x5cu) {
        r = cm_fn_007fee80(obj);
        if (*(volatile u8 *)r != 0u) {
            return 0u;
        }
    }

    if (CM_GLOBAL(volatile u32, 0x00989858) == 0u) {
        cm_fn_007feeb0(obj, (u32)&CM_GLOBAL(u8, 0x00989adc));
        return 1u;
    }

    if (arg1 != 0u) {
        r = cm_fn_0041cbd0(arg1, arg2);
        if (r != 0u) {
            return report(obj, r);
        }
        if (arg3 != 0u) {
            return 0u;
        }

        r = cm_fn_004afd80(arg1, 0);
        if (r != 0u) {
            r = cm_fn_0041cad0(r, arg2);
            if (r != 0u) {
                return report(obj, r);
            }
        }

        e = *(volatile u32 *)(arg1 + 0x5du);
        if (e != 0u) {
            r = cm_fn_0041c9d0(e, arg2);
            if (r != 0u) {
                return report(obj, r);
            }
        }
    } else {
        if (arg3 != 0u) {
            return 0u;
        }
    }

    r = cm_fn_0041cbd0(0u, arg2);
    if (r != 0u) {
        return report(obj, r);
    }

    cm_fn_0041bfb0();
    return 1u;
}
