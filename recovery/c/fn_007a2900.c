/* Preferred-image VA 007a2900, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_007a2900(u32 first, volatile u32 second)
{
    u8 work[0x200];
    u32 staff, person, player, offset, left, right, sum, line, version;
    s16 ability, potential, rating;
    void *name, *extension;
    if (first == 0)
        goto first_error;
    staff = D(first + 0x69u);
    if (staff == 0)
        goto first_error;
    person = second;
    if (person == 0)
        goto second_error;
    player = D(person + 0x61u);
    if (player == 0)
        goto second_error;
    ability = (s16)W(player + 5u);
    if (ability > 160)
        return 0;
    potential = (s16)W(player + 7u);
    if (potential > 160 && ability > 100)
        return 0;
    rating = (s16)W(staff + 4u);
    if (rating > 140)
        offset = 25;
    else {
        offset = rating <= 80 ? 1u : 0u;
        offset = ((offset - 1u) & 0xfffffff1u) + 55u;
    }
    left = D(first);
    right = D(person);
    sum = left + right;
    return (u32)((s32)sum % 35) + offset + 30u;
first_error:
    name = work;
    extension = work + 0x100;
    line = 0x355u;
    goto error;
second_error:
    name = work + 0x100;
    extension = work;
    line = 0x35au;
error:
    cm_fn_00944cff(0x00a6b1e0u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 180;
}
