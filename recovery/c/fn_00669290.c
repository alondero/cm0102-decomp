#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
typedef u32 (CM_THISCALL *slot_fn)(u32, u32);

u32 CM_CDECL cm_fn_00669290(void)
{
    u32 row, eax = 0;
    for (row = 0x00b63d6c; row < 0x00b64744; row += 0x48) {
        u32 p = *(volatile u32 *)(row + 4);
        eax = p;
        if (p) {
            s32 count = *(volatile s32 *)row;
            u32 i = 0;
            while ((s32)i < count) {
                u32 table = *(volatile u32 *)(row + 4);
                u32 item = *(volatile u32 *)(table + i * 4);
                if (item) {
                    u32 vtable = *(volatile u32 *)item;
                    ((slot_fn)*(volatile u32 *)vtable)(item, 1);
                    table = *(volatile u32 *)(row + 4);
                    *(volatile u32 *)(table + i * 4) = 0;
                }
                count = *(volatile s32 *)row;
                ++i;
            }
            p = *(volatile u32 *)(row + 4);
            eax = (u32)cm_fn_009452ca(p);
            *(volatile u32 *)(row + 4) = 0;
            *(volatile u32 *)row = 0;
        }
    }
    return eax;
}
