/* Preferred-image VA 0053cc30, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_0053cc30(u32 club, u32 output)
{
    u8 work[0x200];
    u32 related, cursor, remaining, value, index, count = 0, version;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x3997u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    related = cm_fn_00540a50(club, 0, 1);
    cursor = club + 0x19fu;
    if (related == 0) {
        for (index = 0; index < 5; ++index) {
            value = D(cursor + index * 4u);
            D(output + index * 4u) = value;
        }
        return 1;
    }
    remaining = 5;
    do {
        value = D(cursor);
        if (value != 0) {
            D(output + count * 4u) = value;
            ++count;
        }
        cursor += 4;
    } while (--remaining != 0);
    for (index = 0; index < 5 && count < 5; ++index) {
        value = D(related + index * 4u + 0x19fu);
        if (value != 0) {
            D(output + count * 4u) = value;
            ++count;
        }
    }
    while (count < 5) {
        D(output + count * 4u) = 0;
        ++count;
    }
    return 1;
}
