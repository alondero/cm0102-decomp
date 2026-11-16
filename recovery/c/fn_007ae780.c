/* fn_007ae780, preferred-image VA 0x007ae780, image base 0x00400000.

   Shape (ECX is read before written, so it is `this`): the routine pushes
   selector 1 and capacity 0xc9, then calls the cdecl helper 0x0053dba0 with
   (person, &this->0x70, 0xc9, 1). A non-null stack argument is the person;
   a null one is replaced by the record at [0x00ae23bc] + this->0 * 110.
   Both paths clean four words and return 1 (ret 4).

   Both branches perform their own call and their own stack cleanup, so the
   two calls stay separate in source order. The null-person branch keeps its
   own read of this->0 and of the 0x00ae23bc table base; the offsets inside
   that record are 110 (0x6e) byte records, derived by the original's
   lea chain (index*0x6e). */

#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053dba0(u32 person, u32 output, u32 capacity, u32 selector);

u32 CM_THISCALL cm_fn_007ae780(u32 self, u32 person)
{
    u32 output = self + 0x70;

    if (person == 0) {
        u32 index = *(volatile u32 *)self;
        u32 base = CM_GLOBAL(volatile u32, 0x00ae23bc);

        cm_fn_0053dba0(base + index * 0x6e, output, 0xc9, 1);
        return 1;
    }
    cm_fn_0053dba0(person, output, 0xc9, 1);
    return 1;
}
