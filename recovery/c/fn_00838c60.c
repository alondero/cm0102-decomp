/* fn_00838c60 (setup.cpp), preferred-image VA 0x00838c60, 169 bytes.
   Convention: cdecl with no stack arguments (ret). Returns the matched entry
   pointer in EAX, or 0. */
#include "cmre.h"

extern void cm_fn_005e8290(u32 arg1, u32 arg2, u32 arg3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_CDECL cm_fn_00838c60(void)
{
    /* One scratch block from `sub esp, 0x200`: scratch[0x000] is the listing's
       buffer at L, scratch[0x100] the buffer at L+0x100. */
    u8 scratch[0x200];
    u32 g;
    u32 a;
    u32 s;
    u32 t;
    u32 p;
    u32 i;
    u32 uVar2;

    g = CM_GLOBAL(u32, 0x00ae23bc);
    if (g == 0) {
        cm_fn_00944cff(0x00a801dc, 0, 0, scratch + 0x100, scratch);
        /* The pushes for this call are not cleared here: its (void)
           signature takes none, so they become varargs of the next call. */
        uVar2 = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, uVar2, (u32)(scratch + 0x100), (u32)scratch, 0x39b3);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        CM_GLOBAL(u32, 0x00b67a34) = 0;
        return 0;
    }

    /* p = g + 110 * a - 110 with a = CM_GLOBAL(u32, 0x00ae2364), computed with the
       listing's lea sequence: s = a + 4a; t = a + 2s; t = t + 4t; p = g + 2t - 0x6e. */
    a = CM_GLOBAL(u32, 0x00ae2364);
    s = a + a * 4;
    t = a + s * 2;
    t = t + t * 4;
    p = g + t * 2 - 0x6e;
    for (i = 0; i < 0x10; i++) {
        if (CM_GLOBAL(u8, p + 0x5f) == 0) {
            return p;
        }
        p = p - 0x6e;
    }
    return 0;
}
