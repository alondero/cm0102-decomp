/* fn_00942140 (VA 0x00942140), cdecl, ret 0, one stack argument (the object pointer).
   Calls 0x009445d0 (cdecl, two arguments, no return value) in listing order. Each call
   pushes ([object + field]) second and ([object + 0]) first, so the first argument
   is always the dword at offset 0 of the object. Callers are cdecl: the argument
   cleanup after groups of calls matches the listing (add esp, 0x30 / 8 / 0x20).
   Field reads are volatile and happen before each call, as in the listing. */
#include "cmre.h"

extern void CM_CDECL cm_fn_009445d0(u32 param_1, u32 param_2);

void cm_fn_00942140(u32 param_1)
{
    u32 value;
    u32 first;
    u32 slot;
    u32 k;

    if (param_1 == 0) {
        return;
    }

    /* Six calls: (object[0], object[field]) with no cleanup between them. */
    value = *(volatile u32 *)(param_1 + 0xa4);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    value = *(volatile u32 *)(param_1 + 0x18);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    value = *(volatile u32 *)(param_1 + 0x1c);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    value = *(volatile u32 *)(param_1 + 0x20);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    value = *(volatile u32 *)(param_1 + 0xa8);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    value = *(volatile u32 *)(param_1 + 0xac);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    /* Two calls over the table at object + 0xb4 (listing: lea edi, dec ebx, jne). */
    slot = param_1 + 0xb4;
    for (k = 0; k < 2; k++) {
        value = *(volatile u32 *)slot;
        first = *(volatile u32 *)(param_1 + 0);
        cm_fn_009445d0(first, value);
        slot += 4;
    }

    value = *(volatile u32 *)(param_1 + 0x14);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    value = *(volatile u32 *)(param_1 + 0xc);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    value = *(volatile u32 *)(param_1 + 0xf4);
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, value);

    /* Last call passes the object pointer itself as the second argument. */
    first = *(volatile u32 *)(param_1 + 0);
    cm_fn_009445d0(first, param_1);
}
