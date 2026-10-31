/* fn_00414340 (award_manager.cpp), preferred-image VA 0x00414340, 167 bytes.
   Convention: thiscall with one stack argument (ret 4). ECX is this; the listing
   reads ECX only on the nonzero-argument path. */
#include "cmre.h"

extern void cm_fn_005e8290(u32 arg1, u32 arg2, u32 arg3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_00414340(u32 self, u32 arg1)
{
    /* One scratch block from `sub esp, 0x200`: scratch[0x000] is the listing's
       buffer at L, scratch[0x100] the buffer at L+0x100. */
    u8 scratch[0x200];
    u32 table;
    u32 entry;
    u32 uVar2;
    u8 dl;

    if (arg1 == 0) {
        cm_fn_00944cff(0x00987e24, arg1, arg1, scratch + 0x100, scratch);
        /* The three pushes for this call are not cleared here: its (void)
           signature takes none, so they become varargs of the next call. */
        uVar2 = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, uVar2, (u32)(scratch + 0x100), (u32)scratch, 0x2b3);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, arg1);
        CM_GLOBAL(s32, 0x00b67a34) = (s32)arg1;
        return 0;
    }

    table = *(u32 *)(self + 8);
    dl = 0;
    do {
        entry = *(u32 *)(table + (u32)dl * 4);
        if (entry != 0 && *(u8 *)(entry + 0x12) == 1 && *(u32 *)(entry + 0xe) == arg1) {
            return entry;
        }
        dl = (u8)(dl + 1);
    } while (dl < 0x1e);
    return 0;
}
