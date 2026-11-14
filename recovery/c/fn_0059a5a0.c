#include "cmre.h"

/* Original helpers kept as explicit boundaries. The build binds these
   symbols to their preferred-image addresses.
     0x009462bb: __stdcall element destructor loop, ret 0x10.
     0x00944c94: cdecl free wrapper (calls 0x009452ca). */
extern void CM_STDCALL cm_fn_009462bb(void *base, u32 elem_size, u32 count,
                                      void *dtor);
extern void CM_CDECL cm_fn_00944c94(void *p);

/* fn_0059a5a0: thiscall. ecx points to a pointer slot holding the array's
   first element (or null). When non-null, the element count is the DWORD in
   front of that element, the fixed element size is 0x167, the per-element
   destructor is 0x005a8f60, and the allocation base (element - 4) is freed. */
void CM_THISCALL cm_fn_0059a5a0(void **slot)
{
    u32 *p = (u32 *)*slot;

    if (p == 0)
        return;

    {
        u32 count = p[-1];

        cm_fn_009462bb(p, 0x167u, count, (void *)0x005a8f60);
        cm_fn_00944c94(p - 1);
    }
}
