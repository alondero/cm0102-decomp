#include "cmre.h"

/* Object fields by byte offset from the object in ECX: 0x35 kind byte, whose low
   six bits select the branch. Kinds 3, 4 and 1 call 0x0066f4e0 with the argument
   and a fixed image address; any other kind calls 0x0054a650 with the object at
   offset 0x2d as ECX and the argument plus two immediates on the stack. */
#define OBJ_U8(self, off) (*(volatile u8 *)((self) + (off)))

/* Variadic formatter; kind 4 passes one extra argument (add esp, 0xc). */
extern u32 CM_CDECL cm_fn_0066f4e0(u32 param_1, u32 param_2, ...);
extern u32 CM_THISCALL cm_fn_0054a650(u32 param_1, u32 param_2, u32 param_3, u32 param_4);

u32 CM_THISCALL cm_fn_0085e8f0(u32 self, u32 arg)
{
    u8 kind;

    kind = OBJ_U8(self, 0x35) & 0x3fu;
    if (kind == 3u) {
        return cm_fn_0066f4e0(arg, 0x00a8370cu);
    }
    if (kind == 4u) {
        return cm_fn_0066f4e0(arg, 0x00a836e4u, 0x00ad9c64u);
    }
    if (kind == 1u) {
        return cm_fn_0066f4e0(arg, 0x00a836dcu);
    }
    return cm_fn_0054a650(self + 0x2du, arg, 0xc8u, 0xffu);
}
