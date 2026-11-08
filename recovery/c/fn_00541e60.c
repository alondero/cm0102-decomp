/* Preferred-image VA 0x00541e60, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is the original uninitialized 0x198-byte allocation: direction L+4,
   array L+8, descending index L+2, saved extra count L+3. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_00541f90(u32 club, u32 output, u32 extended);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(
    u32 eax_in, u32 edx_in, u32 base, u32 count, u32 width, u32 compare);
extern u32 CM_CDECL cm_fn_0053c190(u32 club, u32 person, u32 date, u32 detach, u32 players_only);

u32 CM_CDECL cm_fn_00541e60(void)
{
    u8 scratch[0x198];
    u32 l = (u32)scratch, result = D(0x00ae235cu);
    u32 index = 0, club = D(0x00ae23b4u), item, role, cursor, remaining;
    u8 count, extra, descending;
    if ((s32)result <= 0)
        return result;
    do {
        result = cm_fn_00540a50(club, l + 4u, 0);
        if (result == 0)
            goto clear_flag;
        role = D(result + 0x53u);
        if (role == 0)
            goto clear_flag;
        B(club + 0x244u) = 1;
        result = D(l + 4u);
        if (result == 0)
            goto next_club;
        result = cm_fn_00541f90(club, l + 8u, 1);
        cm_fn_00945333(result, l + 8u, l + 8u, 100, 4, 0x005402f0u);
        count = 0;
        extra = 0;
        cursor = l + 8u;
        B(l + 3u) = extra;
        remaining = 100;
        do {
            item = D(cursor);
            if (item != 0) {
                role = D(item + 0x61u);
                count = (u8)(count + 1u);
                if (role != 0 && (s8)B(role + 0xfu) >= 15)
                    extra = (u8)(extra + 1u);
            }
            cursor += 4u;
            --remaining;
        } while (remaining != 0);
        B(l + 3u) = extra;
        if ((s8)count <= 48)
            goto next_club;
        B(l + 2u) = 99;
        do {
            if ((s8)count <= 48)
                break;
            descending = B(l + 2u);
            item = D(l + 8u + (u32)(s32)(s8)descending * 4u);
            if (item != 0) {
                role = D(item + 0x61u);
                if (role != 0 && (s8)B(role + 0xfu) >= 15) {
                    if ((s8)extra < 3)
                        goto previous_item;
                    extra = (u8)(extra - 1u);
                    B(l + 3u) = extra;
                }
                role = D(item + 0x39u);
                cm_fn_0053c190(role, item, 0, 1, 0);
                extra = B(l + 3u);
                count = (u8)(count - 1u);
            }
previous_item:
            descending = (u8)(B(l + 2u) - 1u);
            B(l + 2u) = descending;
        } while ((s8)descending >= 0);
        goto next_club;
clear_flag:
        B(club + 0x244u) = 0;
next_club:
        result = D(0x00ae235cu);
        ++index;
        club += 0x245u;
    } while ((s32)index < (s32)result);
    return result;
}
