#include "cmre.h"

/* WIP: __setjmp3 (0x00947998) captures live x86 context unavailable to plain C. */

extern u32 CM_CDECL cm_fn_00947998(u32 *buffer, u32 count);
extern void CM_CDECL cm_fn_00944350(u32 pointer);
extern void CM_CDECL cm_fn_009445d0(u32 first, u32 second);

void CM_CDECL cm_fn_00941760(u8 *object, u32 *output)
{
    u32 node;
    u32 result;
    u32 owner;
    u32 first;
    u32 mode;

    if (object == 0)
        return;
    if (output != 0) {
        node = *(volatile u32 *)(object + 0x11);
        result = cm_fn_00947998((u32 *)(node + 0x10u), 0);
        *(volatile u32 *)output = result;
        if (result != 0) {
            node = *(volatile u32 *)(object + 0x11);
            cm_fn_00944350(node);
            return;
        }
    }

    mode = *(volatile u8 *)(object + 0x10);
    if (mode == 1) {
        first = *(volatile u32 *)object;
        node = *(volatile u32 *)(object + 0x11);
        cm_fn_009445d0(node, first);
        node = *(volatile u32 *)(object + 0x11);
        cm_fn_009445d0(node, (u32)object);
        return;
    }

    node = *(volatile u32 *)(object + 0x11);
    owner = *(volatile u32 *)(node + 0x58);
    if ((u32)object == owner) {
        *(volatile u32 *)(node + 0x88) = 1;
        return;
    }
    cm_fn_009445d0(node, (u32)object);
}
