#include "cmre.h"

#define DAT_AE24D0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_B67A34 CM_GLOBAL(volatile u32, 0x00b67a34)

typedef void (CM_THISCALL *cm_76a380_slot)(void *entry, u32 argument);
extern int CM_CDECL cm_fn_00932e10(u32 *flag, u32 size, u32 one, u32 argument);
extern void CM_CDECL cm_fn_00944cff(u32 source, u32 zero1, u32 zero2, void *buffer1, void *buffer2);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, const char *format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message, u32 zero);
extern void CM_THISCALL cm_fn_00767d60(void *self, u32 argument);

void CM_THISCALL cm_fn_0076a380(volatile u8 *self, u32 argument)
{
    volatile u8 scratch[0x204];
    volatile u32 *flag = (volatile u32 *)(scratch + 0u);
    u32 *buffer1 = (u32 *)(scratch + 4u);
    u32 *buffer2 = (u32 *)(scratch + 0x104u);
    u32 initial = *(volatile u32 *)(self + 0x0au);
    u16 index = 0u;

    *flag = initial != 0u;
    if (*(volatile s16 *)(self + 8u) > 0) {
        do {
            u32 array = *(volatile u32 *)(self + 4u);
            void *entry = *(void *volatile *)(array + (u32)(s32)(s16)index * 4u);
            void *vtable = *(void *volatile *)entry;
            (*(cm_76a380_slot volatile *)((u8 *)vtable + 4u))(entry, argument);
            ++index;
        } while ((s16)index < *(volatile s16 *)(self + 8u));
    }

    if (cm_fn_00932e10((u32 *)flag, 4u, 1u, argument) != 1) {
        cm_fn_00944cff(0x00a5b26cu, 0u, 0u, (void *)buffer2, (void *)buffer1);
        {
            u32 formatted = (u32)cm_fn_0090d130();
            cm_fn_00944c9f((u32)&DAT_AE24D0, (const char *)0x009870e8u,
                           formatted, (void *)buffer2, (void *)buffer1, 0x82bu);
        }
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)&DAT_AE24D0, 0u);
        DAT_B67A34 = 0u;
        return;
    }

    initial = *(volatile u32 *)(self + 0x0au);
    if (initial != 0u)
        cm_fn_00767d60((void *)initial, argument);
}
