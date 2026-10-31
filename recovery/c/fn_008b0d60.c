#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x7d9 signed byte index n; the callee's object is self + 155 * n + 0xfa;
   0x80a is an inner address (self + 0x80a) that is passed to the callee and,
   when the out pointer is non-null, also stored through it;
   0x818 receives the byte result of 0x008b5000. */
#define OBJ_U8(off) (*(volatile u8 *)(self + (off)))

extern u8 CM_THISCALL cm_fn_008b5000(u8 *self, u32 arg1, u32 arg2, u32 arg3);

/* Two stack arguments (ret 8): out is a pointer to a u32 slot (may be null);
   arg2 is a u32 value forwarded to the callee when out is non-null. When out is
   null the callee receives 0 for its first and third arguments. */
void CM_THISCALL cm_fn_008b0d60(u32 self, u32 out, u32 arg2)
{
    s32 index;
    u32 target;
    u8 result;

    if (out == 0) {
        index = *(volatile s8 *)(self + 0x7d9);
        target = self + (u32)index * 155u + 0xfau;
        result = cm_fn_008b5000((u8 *)target, 0u, self, 0u);
    } else {
        *(volatile u32 *)out = self + 0x80au;
        index = *(volatile s8 *)(self + 0x7d9);
        target = self + (u32)index * 155u + 0xfau;
        result = cm_fn_008b5000((u8 *)target, self + 0x80au, self, arg2);
    }
    OBJ_U8(0x818) = result;
}
