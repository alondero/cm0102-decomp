#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (thiscall, ret 0).
   0x00 vtable pointer (written by this routine), 0x3c signed 16-bit count,
   0xa3 pointer released through cm_fn_009452ca, 0xa7 pointer released through
   cm_fn_009452ca, 0xad pointer to a dword array (its elements are released
   through cm_fn_009452ca; the pointer itself is released after the loop). */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_U16(off) (*(volatile u16 *)(self + (off)))

extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_00523300(u32 self)
{
    u32 value;
    u32 element;
    s32 index;

    value = OBJ_U32(0xa7);
    OBJ_U32(0x00) = 0x00969468u;
    if (value != 0) {
        cm_fn_009452ca(value);
    }

    value = OBJ_U32(0xa3);
    if (value != 0) {
        cm_fn_009452ca(value);
    }

    value = OBJ_U32(0xad);
    if (value != 0) {
        index = 0;
        /* Guard: the signed count must be positive before the first element. */
        if ((s16)OBJ_U16(0x3c) > 0) {
            do {
                element = *(volatile u32 *)(OBJ_U32(0xad) + (u32)index * 4u);
                if (element != 0) {
                    cm_fn_009452ca(element);
                }
                index = index + 1;
            } while (index < (s32)(s16)OBJ_U16(0x3c));
        }

        value = OBJ_U32(0xad);
        cm_fn_009452ca(value);
    }

    cm_fn_00518690((void *)self);
}
