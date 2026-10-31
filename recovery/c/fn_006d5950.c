#include "cmre.h"

/* Callee at 0x006ce310: thiscall (object in ECX) with seven stack arguments, popped
   by the callee. The listing pushes dword values for param_2 and param_8, so they
   are declared as dwords; the pointers write result bytes into this routine's
   frame and its argument slot. */
extern u32 CM_THISCALL cm_fn_006ce310(u8 *self, u32 param_2, u8 *param_3, u8 *param_4,
                                      u8 *param_5, u8 *param_6, u8 *param_7, u32 param_8);

/* Returns AL only (the listing's byte results); the upper EAX bits of the original
   return come from the callee's return value and are not part of this declared width.
   Stack arguments by offset from the entry stack pointer (thiscall, ret 4):
   arg_04 dword passed by value and also written by the callee through its address. */
u8 CM_THISCALL cm_fn_006d5950(u32 self, u32 arg_04)
{
    /* The pushed ECX dword sits at entry-4; its four bytes are the callee's
       result bytes: byte 0 (entry-4), byte 1 (entry-3), byte 2 (entry-2), byte 3 (entry-1). */
    u32 frame = self;
    u8 *bytes = (u8 *)&frame;
    u8 al;
    u8 cl;

    cm_fn_006ce310((u8 *)self, arg_04, (u8 *)&arg_04, bytes + 0, bytes + 3,
                   bytes + 1, bytes + 2, 0u);

    al = *(volatile u8 *)&arg_04;
    cl = *(volatile u8 *)bytes;

    if ((s8)al < (s8)cl) {
        return 1u;
    }
    if (bytes[1] == 0u) {
        return 0u;
    }
    if (bytes[2] != 0u) {
        return 0u;
    }
    if (bytes[3] != 0u) {
        return 0u;
    }
    if ((s8)al <= (s8)cl) {
        return 1u;
    }
    return 0u;
}
