#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                   void *buffer1, void *buffer2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer, int mode);

u32 CM_THISCALL cm_fn_00766290(volatile u8 *self, const u32 *source)
{
    volatile u8 scratch[0x200];
    u32 allocation;

    *(volatile s16 *)(self + 0x3eu) = 4;
    allocation = cm_fn_00944e46(0x18u);
    *(volatile u32 *)(self + 0xa7u) = allocation;
    if (allocation == 0u) {
        cm_fn_00944cff(0x00a5ab00u, 0u, 0u,
                       (void *)(scratch + 0x100u), (void *)scratch);
        {
            u32 message = (u32)(unsigned long)cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                           (void *)(scratch + 0x100u), (void *)scratch, 0x105u);
        }
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 0u;
    }

    if (*(volatile s16 *)(self + 0x3eu) > 0) {
        const u32 *source_cursor = source;
        u32 offset = 0u;
        s32 index = 0;

        do {
            u32 output = *(volatile u32 *)(self + 0xa7u);
            u32 value = *(volatile u32 *)source_cursor;
            source_cursor = (const u32 *)((u32)source_cursor + 4u);
            ++index;
            *(volatile u32 *)(output + offset) = value;

            output = *(volatile u32 *)(self + 0xa7u);
            *(volatile u8 *)(output + offset + 5u) = 0u;
            output = *(volatile u32 *)(self + 0xa7u);
            *(volatile u8 *)(output + offset + 4u) = 0u;

            offset += 6u;
        } while (index < *(volatile s16 *)(self + 0x3eu));
    }

    return 1u;
}
