#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (read only on the
   table path): 0x0c + 4 * index for a signed byte index in 0..15.
   Globals: 0x00b77c7a signed word selects a 0x300-byte row of dword table at
   0x00b74c26; 0x00ae2364 byte subtracted from the first byte of the argument
   pointer; 0x00b67a34 dword cleared on the report path. */

extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern char *cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, s32 param_3);

u32 CM_THISCALL cm_fn_007f6be0(u32 self, u32 arg)
{
    /* Two 0x100-byte buffers: the first at offset 0, the second at offset 0x100. */
    u8 scratch[0x200];
    u32 p;
    u32 table_value;
    s32 g;
    u8 first;
    u8 second;
    u8 idx8;
    s32 index;
    u32 path;

    path = 0;
    p = arg;
    if (p == 0) {
        g = (s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
        table_value = *(volatile u32 *)(0x00b74c26u + (((u32)g * 3u) << 8));
        p = table_value;
        if (table_value == 0) {
            path = 2;
        }
    }

    if (path == 0) {
        first = *(volatile u8 *)p;
        second = CM_GLOBAL(volatile u8, 0x00ae2364);
        idx8 = (u8)((u8)(first - second) + 0x10u);
        index = (s8)idx8;
        if (index >= 0 && index <= 15) {
            return *(volatile u32 *)(self + (u32)index * 4u + 0xcu);
        }
        path = 1;
    }

    if (path == 2) {
        /* 0x007f6c05: 0x944cff(0xa7b094, 0, 0, L+0x100, L), then the report tail
           with 0x0de as the last vararg and L+0x100 pushed before L. */
        cm_fn_00944cff(0x00a7b094u, 0u, 0u, scratch + 0x100, scratch);
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                             (u32)(scratch + 0x100), (u32)scratch, 0xdeu);
    } else {
        /* 0x007f6c57: 0x944cff(0xa7b094, 0, 0, L, L+0x100), then the report tail
           with 0x0e8 as the last vararg. */
        cm_fn_00944cff(0x00a7b094u, 0u, 0u, scratch, scratch + 0x100);
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                             (u32)scratch, (u32)(scratch + 0x100), 0xe8u);
    }

    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
    return 0u;
}
