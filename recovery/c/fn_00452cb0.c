#include "cmre.h"

/* Original callees. The listing shows cdecl cleanup by the caller after each of
   these calls (add esp after the call) and no receiver. 0x0090d130 takes no
   arguments: the pushes made before it are trailing varargs of the following
   0x00944c9f call (MSVC right-to-left pushes). */
extern unsigned long long CM_CDECL cm_fn_00945667(u32 arg8, u32 argC);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2, u32 buffer1, u32 buffer2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);

u32 CM_THISCALL cm_fn_00452cb0(u32 self, u32 arg1, u8 arg2)
{
    /* One scratch allocation: local_area + 0 is the 256-byte buffer at [esp+4],
       local_area + 0x100 is the buffer at [esp+0x104] in the listing. */
    u8 local_area[0x200];
    u32 cap;
    u32 count;
    u32 base;
    u32 index;
    u32 new_ptr;
    u32 ret;

    cap = *(volatile u32 *)(self + 0x28u);
    count = *(volatile u32 *)(self + 0x2cu);
    if (count == cap) {
        base = *(volatile u32 *)(self + 0x24u);
        cap = cap + 5u;
        *(volatile u32 *)(self + 0x28u) = cap;
        new_ptr = (u32)cm_fn_00945667(base, cap * 6u);
        *(volatile u32 *)(self + 0x24u) = new_ptr;
        if (new_ptr == 0u) {
            cm_fn_00944cff(0x0098b6a0u, 0u, 0u,
                           (u32)(unsigned long)(local_area + 0x100u),
                           (u32)(unsigned long)local_area);
            ret = (u32)(unsigned long)cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret,
                           (u32)(unsigned long)(local_area + 0x100u),
                           (u32)(unsigned long)local_area, 0x174fu);
            cm_fn_005e8290((const char *)0x009870e0u,
                           (const char *)0x00ae24d0u, 0);
            CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
            return 0u;
        }
    }

    index = *(volatile u32 *)(self + 0x2cu);
    base = *(volatile u32 *)(self + 0x24u);
    *(volatile u32 *)(base + index * 6u) = arg1;

    index = *(volatile u32 *)(self + 0x2cu);
    base = *(volatile u32 *)(self + 0x24u);
    *(volatile u8 *)(base + index * 6u + 4u) = arg2;

    index = *(volatile u32 *)(self + 0x2cu);
    *(volatile u32 *)(self + 0x2cu) = index + 1u;
    return 1u;
}
