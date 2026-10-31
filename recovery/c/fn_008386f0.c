/* Preferred-image VA 0x008386f0, image base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   No arguments; cdecl (plain ret). The 0x200-byte frame holds two 0x100-byte
   buffers at [esp+0] and [esp+0x100]; both are passed to callees uninitialized. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern void CM_FASTCALL cm_fn_005ca030(int param_1);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

u32 cm_fn_008386f0(void)
{
    u8 scratch[0x200];
    u32 alloc_base;
    u32 rec;
    u32 ecx_value;
    u32 v0;
    s32 idx;
    s32 count;

    count = CM_GLOBAL(int, 0x00ae2378);
    CM_GLOBAL(int, 0x00adadf8) = count;
    alloc_base = (u32)cm_fn_00945501((u32)count << 2, 1u);
    CM_GLOBAL(int, 0x00adadfc) = (int)alloc_base;
    if (alloc_base != 0) {
        goto table_fill;
    }

    /* Zero path: 0x944cff fills both buffers; 0x90d130 takes no arguments, so
       the three pushes before its call are the trailing varargs of 0x944c9f:
       (0x3896, scratch, scratch+0x100) after the 0x90d130 result. */
    cm_fn_00944cff(0x00a801dcu, 0u, 0u, scratch + 0x100, scratch);
    v0 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, v0,
                   (u32)(scratch + 0x100), (u32)scratch, 0x3896u);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
    return 0;

table_fill:
    rec = 0x00b63d6cu;
    do {
        if (*(u32 *)(rec + 4) != 0) {
            count = *(s32 *)rec;
            if (count > 0) {
                idx = 0;
                do {
                    u32 entry = *(u32 *)(*(u32 *)(rec + 4) + (u32)idx * 4u);
                    if (entry != 0) {
                        u32 key_ptr = *(u32 *)(entry + 4);
                        u32 slot = *(u32 *)key_ptr;
                        *(u32 *)(alloc_base + slot * 4u) = entry;
                        alloc_base = (u32)CM_GLOBAL(int, 0x00adadfc);
                    }
                    idx = idx + 1;
                } while (idx < *(s32 *)rec);
            }
        }
        rec = rec + 0x48u;
    } while ((s32)rec < 0x00b64744);

    ecx_value = (u32)CM_GLOBAL(int, 0x00ae2c74);
    cm_fn_005ca030((int)ecx_value);
    return 1;
}
