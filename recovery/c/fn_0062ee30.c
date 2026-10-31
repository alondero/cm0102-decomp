/* Preferred-image VA 0062ee30, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All three direct callers discard EAX. Four stack arguments, ret 10h. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0062c680(u32 self, u32 player);
extern u32 CM_STDCALL cm_fn_0062ef30(u32 injury, u32 destination, s32 size, u32 mode);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_0062ee30(u32 self, u32 player, u32 destination, s32 size, u32 mode)
{
    u8 scratch[0x200];
    u32 id, count, base, injury, text, argument;
    if (player == 0) {
        cm_fn_00944cff(0x009e7f28u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, scratch + 0x100, scratch, 0xe63u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return;
    }
    id = D(player);
    count = D(0x00ae2364u);
    if ((s32)id >= (s32)count)
        return;
    cm_fn_0062c680(self, player);
    id = D(player);
    base = D(self);
    injury = D(base + id * 31u + 0xeu);
    if (injury == 0xffffffffu) {
        cm_fn_0066f4e0(destination, 0x009e7fd4u, 0x00ad9c64u);
        return;
    }
    base = D(self + 4u);
    /* Native MOV DL replaces only the destination argument's low byte. */
    argument = (destination & 0xffffff00u) | B(base + injury * 48u);
    cm_fn_0062ef30(argument, destination, size, mode);
}
