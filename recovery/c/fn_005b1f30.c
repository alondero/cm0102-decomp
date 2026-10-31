#include "cmre.h"

extern u32 *CM_CDECL cm_fn_0090bde0(const char *first, const char *second);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);
extern u32 CM_CDECL cm_fn_005b2170(void);

void CM_CDECL cm_fn_005b1f30(u32 requested_index)
{
    u32 initialized = CM_GLOBAL(volatile u32, 0x00ae2c68);

    if (initialized == 0) {
        u32 *path = cm_fn_0090bde0((const char *)0x009c3718, (const char *)0x009cc2c8);
        if (path == 0)
            path = cm_fn_0090bde0((const char *)0x009b5348, (const char *)0x009cc2c8);
        if (path == 0) {
            CM_GLOBAL(volatile u8, 0x00ae2a64) = 0;
        } else {
            cm_fn_009466d0(0x00ae2a64u, (u32)path, 0x104u);
            CM_GLOBAL(volatile u8, 0x00ae2b67) = 0;
            if (cm_fn_005b2170() != 0)
                CM_GLOBAL(volatile u32, 0x00ae2c68) = 1;
        }
    }

    {
        s32 argument = (s32)requested_index;
        u32 limit;

        if (argument != -2 && argument != -1) {
            limit = CM_GLOBAL(volatile u32, 0x00ae2c70) & 0xffffu;
            if (argument >= (s32)limit)
                return;
        }
        CM_GLOBAL(volatile s32, 0x009cc2c4) = argument;
    }
}
