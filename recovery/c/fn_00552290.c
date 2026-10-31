/* Preferred-image VA 0x00552290, base 0x00400000; primary executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both direct callers test full EAX. The selected byte and scanned WORD are
   sign-extended at their original comparison widths. */
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
extern u32 CM_THISCALL cm_fn_00682f70(u32 self, u32 value);

u32 CM_THISCALL cm_fn_00552290(u32 self)
{
    u8 scratch[0x200];
    u8 selected = 0;
    u32 allocation, club, candidate, group, scanned = 0;
    s32 total;

    CM_GLOBAL(volatile u16, self + 0x3eu) = 0x10;
    allocation = (u32)cm_fn_00944e46(0x3b0u);
    CM_GLOBAL(volatile u32, self + 0xb1u) = allocation;
    if (allocation == 0) {
        u32 text;
        cm_fn_00944cff(0x009b8e30u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x100, scratch, 0x142u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    total = CM_GLOBAL(volatile s32, 0x00ae235cu);
    club = CM_GLOBAL(volatile u32, 0x00ae23b4u);
    if (total > 0) {
        do {
            candidate = CM_GLOBAL(volatile u32, club + 0x57u);
            if (candidate != 0) {
                candidate = CM_GLOBAL(volatile u32, candidate);
                group = CM_GLOBAL(volatile u32, 0x009cf5b0u);
                if (candidate == group) {
                    u8 slot = selected;
                    selected = (u8)(selected + 1u);
                    /* Original PUSH EAX replaces only AL; 00687430 observes
                       only this byte of its first stack argument. */
                    cm_fn_00687430(self, slot, club, 0, 0);
                    if ((s16)(s8)selected ==
                        CM_GLOBAL(volatile s16, self + 0x3eu))
                        break;
                }
            }
            total = CM_GLOBAL(volatile s32, 0x00ae235cu);
            ++scanned;
            club += 0x245u;
        } while ((s32)(s16)scanned < total);
    }
    if ((s16)(s8)selected != CM_GLOBAL(volatile s16, self + 0x3eu))
        return 0;
    allocation = cm_fn_00682f70(self, 0x2710u);
    CM_GLOBAL(volatile u32, self + 0x28u) = allocation;
    CM_GLOBAL(volatile u8, self + 0xe1u) = 1;
    return 1;
}
