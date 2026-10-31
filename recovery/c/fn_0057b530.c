/* Preferred-image VA 0x0057b530, base 0x00400000; primary executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Caller VA 0x0057c1ea tests full EAX. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message,
                                   s32 flags);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club,
                                     u8 flag, u32 base);
extern void CM_THISCALL cm_fn_00684230(u32 self);

u32 CM_THISCALL cm_fn_0057b530(u32 self, u32 entries)
{
    u8 scratch[0x200];
    u32 allocation, selected = 0, index = 0;

    CM_GLOBAL(volatile u16, self + 0x3eu) = 4;
    allocation = (u32)cm_fn_00944e46(0xecu);
    CM_GLOBAL(volatile u32, self + 0xb1u) = allocation;
    if (allocation == 0) {
        u32 text;
        cm_fn_00944cff(0x009c4638u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x100, scratch, 0x208u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }
    do {
        u32 row;
        if ((s16)selected >= 4)
            break;
        row = entries + (u32)(s32)(s16)index * 5u;
        if (CM_GLOBAL(volatile u8, row + 4u) == 1) {
            u32 club = CM_GLOBAL(volatile u32, row);
            u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4u);
            u8 slot = (u8)selected;
            ++selected;
            cm_fn_00687430(self, slot, base + club * 0x245u, 0, 0);
        }
        ++index;
    } while ((s16)index < 0x10);
    cm_fn_00684230(self);
    return 1;
}
