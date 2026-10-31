/* Preferred-image VA 0x0093A020, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl, ret 0 (the caller pops). One stack argument, the object
   (esi = arg1). The object holds a dword stack cursor at +0x20, a count at +4,
   and pointers at +0x08, +0x0C and +0x28.
   Work: pop two dwords (A = top, B = the one below), compute two operands from
   tables reached through the object, call through the object's function
   pointers, store one result dword at the B slot, and set the cursor to
   that slot plus 4.
   Parity of (count - 1): odd takes the signed-word table path with two
   calls. The second call is through [W + 0x98], where W = [obj + 0x28] was
   read before the pops. The listing spills W into the argument slot and
   reloads it after the first call; the reload is kept as the local W.
   Even takes the dword table path with one call through [obj + 0x46].
   Indirect calls use cdecl (the listing pops 12 bytes after the three-argument
   call on the even path, and 20 bytes after both calls on the odd path).
   Object and table accesses are volatile to keep the listing's order. */
#include "cmre.h"

typedef u32 (CM_CDECL *cdecl_fn_3)(u32, u32, u32);
typedef u32 (CM_CDECL *cdecl_fn_2)(u32, u32);

void CM_CDECL cm_fn_0093a020(u32 arg1)
{
    u32 obj;
    u32 *cursor;
    u32 w_val;
    u32 x_ptr;
    u32 y_ptr;
    u32 count;
    u32 a_idx;
    u32 b_idx;
    u32 target;
    u32 v1;
    u32 v2;
    u32 r1;
    u32 r2;

    obj = arg1;
    cursor = (u32 *)*(volatile u32 *)(obj + 0x20);
    w_val = *(volatile u32 *)(obj + 0x28);
    count = *(volatile u32 *)(obj + 0x04);
    x_ptr = *(volatile u32 *)(obj + 0x0c);

    cursor = cursor - 1;
    a_idx = *(volatile u32 *)cursor;
    b_idx = *(volatile u32 *)(cursor - 1);
    cursor = cursor - 1;

    y_ptr = *(volatile u32 *)(obj + 0x08);

    if (((count - 1) & 1) != 0) {
        u32 p1;
        u32 p2;
        u32 p3;
        u32 p4;
        s32 s_p1;
        s32 s_p2;
        s32 s_p3;
        s32 s_p4;

        p1 = *(volatile u32 *)(x_ptr + 0x14);
        p2 = *(volatile u32 *)(y_ptr + 0x14);
        p3 = *(volatile u32 *)(y_ptr + 0x10);
        s_p1 = *(volatile s16 *)(p1 + 2u * a_idx);
        s_p2 = *(volatile s16 *)(p2 + 2u * b_idx);
        s_p3 = *(volatile s16 *)(p3 + 2u * b_idx);
        v1 = (u32)(s_p2 - s_p1);

        p4 = *(volatile u32 *)(*(volatile u32 *)(obj + 0x0c) + 0x10);
        s_p4 = *(volatile s16 *)(p4 + 2u * a_idx);
        v2 = (u32)(s_p3 - s_p4);

        target = *(volatile u32 *)(obj + 0x4a);
        r1 = ((cdecl_fn_3)target)(obj, v2, v1);

        target = *(volatile u32 *)(w_val + 0x98);
        r2 = ((cdecl_fn_2)target)(w_val, r1);

        *(volatile u32 *)cursor = r2;
    } else {
        u32 q1;
        u32 q2;
        u32 q3;
        u32 q4;
        u32 q1b;
        u32 q3b;

        q1 = *(volatile u32 *)(y_ptr + 0x04);
        q2 = *(volatile u32 *)(x_ptr + 0x04);
        q1b = *(volatile u32 *)(q1 + 4u * b_idx);
        v1 = q1b - *(volatile u32 *)(q2 + 4u * a_idx);

        q3 = *(volatile u32 *)(y_ptr + 0x00);
        q3b = *(volatile u32 *)(q3 + 4u * b_idx);
        q4 = *(volatile u32 *)(x_ptr + 0x00);
        v2 = q3b - *(volatile u32 *)(q4 + 4u * a_idx);

        target = *(volatile u32 *)(obj + 0x46);
        r1 = ((cdecl_fn_3)target)(obj, v2, v1);

        *(volatile u32 *)cursor = r1;
    }

    cursor = cursor + 1;
    *(volatile u32 *)(obj + 0x20) = (u32)cursor;
}
