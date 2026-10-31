#include "cmre.h"

/* The original forwards the low byte of the stack argument to 00951d5b,
   with zero and four as its second and third arguments. */
extern u32 CM_CDECL cm_fn_00951d5b(u8, u32, u8);

u32 CM_CDECL cm_fn_00951d4a(u32 arg)
{
    return cm_fn_00951d5b((u8)arg, 0, 4);
}
