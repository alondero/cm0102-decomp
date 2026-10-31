#include "cmre.h"

extern u32 CM_CDECL cm_fn_00540a50(u32 self, u32 zero, u32 one);
extern void cm_fn_005e8290(const char *message, const char *buffer, int mode);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  void *buffer1, void *buffer2);

u8 CM_CDECL cm_fn_0053c9b0(u32 self)
{
    u8 scratch[0x200];
    u32 index;
    u32 global_value;
    u32 count = 0u;
    u32 entry;
    u32 result;

    if (self == 0u) {
        (void)cm_fn_00944cff(0x009b5ac8u, 0u, 0u,
                             scratch + 0x100, scratch);
        result = (u32)(unsigned long)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, result,
                             (u32)(unsigned long)(scratch + 0x100),
                             (u32)(unsigned long)scratch, 0x3914u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 0u;
    }

    global_value = CM_GLOBAL(volatile u32, 0x00ae2364);
    index = 0u;
    do {
        entry = *(volatile u32 *)(self + 0x1cfu + index * 4u);
        if (entry != 0u && (s32)*(volatile s32 *)entry < (s32)global_value)
            ++count;
        ++index;
    } while (index != 3u);

    result = cm_fn_00540a50(self, 0u, 1u);
    if (result != 0u) {
        global_value = CM_GLOBAL(volatile u32, 0x00ae2364);
        index = 0u;
        do {
            entry = *(volatile u32 *)(result + 0x1cfu + index * 4u);
            if (entry != 0u && (s32)*(volatile s32 *)entry < (s32)global_value)
                ++count;
            ++index;
        } while (index != 3u);
    }
    return (u8)count;
}
