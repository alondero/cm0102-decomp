#include "cmre.h"

extern u32 CM_CDECL cm_fn_005444a0(u32 argument, u32 flag);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self, u32 argument, u32 flag);
extern u32 CM_CDECL cm_fn_00547d60(u32 argument, u32 nested);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 divisor);
extern void CM_CDECL cm_fn_00547af0(u32 argument, u32 nested, u32 flag);
extern void CM_THISCALL cm_fn_00842c00(u32 self, u32 argument, u32 flag);
extern void CM_THISCALL cm_fn_004d71b0(u32 self, u32 argument, u32 nested);

void CM_THISCALL cm_fn_00842320(u32 self, u32 argument, u32 optional)
{
    if (cm_fn_005444a0(argument, 0) != 0)
        return;

    u32 manager = CM_GLOBAL(volatile u32, 0x00ae1050u);
    u32 item = cm_fn_004cf5a0(manager, argument);
    if (item == 0)
        return;

    u8 high = *(volatile u8 *)(item + 0x4fu);
    u8 low = *(volatile u8 *)(item + 0x4eu);
    *(volatile u8 *)(item + 0x4eu) = (u8)(low | 1u);
    *(volatile u8 *)(item + 0x4fu) = (u8)((high & 0x0fu) | 0x70u);

    u32 service = CM_GLOBAL(volatile u32, 0x00de1f54u);
    if (cm_fn_008cca10(service, argument, 0) != 0)
        return;

    u32 nested = *(volatile u32 *)(argument + 0x39u);
    u32 has_peer = cm_fn_00547d60(argument, nested);
    if (has_peer == 0) {
        s8 adjustment = *(volatile s8 *)(item + 0x44u);
        s32 first = cm_fn_0090cfc0(100);
        s32 second = cm_fn_0090cfc0(100);
        if ((s32)adjustment > first - second) {
            u32 nested_for_update = *(volatile u32 *)(argument + 0x39u);
            u32 peer = *(volatile u32 *)(nested_for_update + 0xcfu);
            cm_fn_00547af0(argument, peer, 1);
        }
    } else {
        u32 nested_for_update = *(volatile u32 *)(argument + 0x39u);
        u32 peer = *(volatile u32 *)(nested_for_update + 0xcfu);
        cm_fn_00547af0(argument, peer, 1);
    }

    if (optional != 0) {
        cm_fn_00842c00(self, argument, 1);

        u32 nested_after = *(volatile u32 *)(argument + 0x39u);
        u32 manager_after = CM_GLOBAL(volatile u32, 0x00ae1050u);
        cm_fn_004d71b0(manager_after, argument, nested_after);
    }
}
