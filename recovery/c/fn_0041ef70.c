#include "cmre.h"

/* fn_0041ef70: cdecl, no stack arguments (ret 0), no ECX read before write.
   Preferred-image VA 0x0041ef70, image base 0x00400000.
   Scratch: local_80 is a 32-entry array of record pointers at L (esp after sub esp, 0x80).
   Only the first `count` entries are stored by the scan; the rest are read uninitialized by
   the listing and are not initialized here. Entries are read through volatile accesses in
   listing order. */

extern void cm_fn_00945333(u32 *param_1, u32 param_2, u32 param_3, void *param_4);

void cm_fn_0041ef70(void)
{
    u32 local_80[32];
    volatile u32 *arr;
    u32 iVar1;
    u32 iVar6;
    u32 iVar7;
    u32 p;
    s16 sVar2;
    s8 cVar4;
    s32 idx;
    s32 i;

    cVar4 = 0;
    iVar6 = (u32)CM_GLOBAL(u32, 0x009cf5a4) * 0x6bu + CM_GLOBAL(u32, 0x00ae23d0);
    sVar2 = 0;
    iVar7 = (u32)CM_GLOBAL(u32, 0x009cf5a8) * 0x6bu + CM_GLOBAL(u32, 0x00ae23d0);
    iVar1 = CM_GLOBAL(u32, 0x00ae23b4);
    if ((s32)CM_GLOBAL(u32, 0x00ae235c) > 0) {
        do {
            p = *(volatile u32 *)(iVar1 + 0x57u);
            if (p != 0u && *(volatile u32 *)p == CM_GLOBAL(u32, 0x009cf7c4)) {
                idx = (s32)cVar4;
                cVar4 = (s8)(cVar4 + 1);
                local_80[idx] = iVar1;
                if (cVar4 == 0x20) {
                    break;
                }
            }
            sVar2 = (s16)(sVar2 + 1);
            iVar1 = iVar1 + 0x245u;
        } while ((s32)sVar2 < (s32)CM_GLOBAL(u32, 0x00ae235c));
    }

    cm_fn_00945333(local_80, (u32)(s32)cVar4, 4u, (void *)0x0053fe50u);

    arr = (volatile u32 *)local_80;
    for (i = 0; i < 0x20; i++) {
        p = arr[i];
        if ((s16)i < 0x10) {
            *(volatile u32 *)(p + 0x60u) = iVar6;
        } else {
            *(volatile u32 *)(p + 0x60u) = iVar7;
        }
    }
}
