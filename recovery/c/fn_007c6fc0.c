#include "cmre.h"

/* Original frame: `sub esp, 0x200` scratch area. Scratch A is the base of the
   frame (listing `lea edx, [esp + 4]` after `push esi`), scratch B is A + 0x100
   (listing `lea eax, [esp + 0x104]`). Both live in one 0x200-byte array.

   `this` arrives in ECX (mov esi, ecx). Fields are named by their offset from
   `this`; their roles are not established beyond the stores shown. */

struct cm_obj_007c6fc0 {
    void *f00;   /* [this + 0x00] */
    u32 f04;     /* [this + 0x04] */
    u32 f08;     /* [this + 0x08] */
    u32 f0c;     /* [this + 0x0c] */
    u32 f10;     /* [this + 0x10] */
    u32 f14;     /* [this + 0x14] */
    u32 f18;     /* [this + 0x18] */
};

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

void *CM_THISCALL cm_fn_007c6fc0(void *self, u32 arg1, u32 arg2, u32 arg3,
                                 u32 arg4, s32 arg5)
{
    volatile struct cm_obj_007c6fc0 *obj = self;
    u8 scratch[0x200];
    u32 allocation;
    u32 ret_0090d130;

    /* Stores in listing order: arg1 -> +4, arg4 -> +0x10, arg2 -> +8,
       arg3 -> +0xc, arg5 -> +0x18, zero -> +0x14. */
    obj->f04 = arg1;
    obj->f10 = arg4;
    obj->f08 = arg2;
    obj->f0c = arg3;
    obj->f18 = (u32)arg5;
    obj->f14 = 0u;

    if (arg5 < 1) {
        /* jge not taken: arg5 is zero or negative (signed). */
        obj->f00 = (void *)0;
        return self;
    }

    /* eax*9, then shl 1: arg5 * 18 as a 32-bit value. */
    allocation = (u32)cm_fn_00944e46((u32)arg5 * 18u);
    obj->f00 = (void *)allocation;
    if (allocation != 0u) {
        return self;
    }

    /* Allocation failed. cm_fn_00944cff receives scratch B as arg4 and scratch
       A as arg5. */
    (void)cm_fn_00944cff(0x00a7853cu, 0u, 0u, scratch + 0x100, scratch);
    /* cm_fn_0090d130 takes no arguments: its three pushes (0x2f, scratch A,
       scratch B) stay on the stack as varargs of cm_fn_00944c9f. */
    ret_0090d130 = cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                         scratch + 0x100, scratch, 0x2fu);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return self;
}
