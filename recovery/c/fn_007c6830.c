/* Preferred-image VA 007c6830, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 player, u32 club);
extern u32 CM_CDECL cm_fn_00946464(void *key, u32 base, u32 count, u32 size, u32 comparator);

u8 CM_THISCALL cm_fn_007c6830(u32 self, u32 player, u32 club, u8 nation, u32 flag)
{
    u8 scratch[8];
    u32 id, count, pointer, result, related, left, right, table;
    u8 comparison;
    if (player != 0) {
        id = D(player);
        count = D(0x00ae2364u);
        if ((s32)id >= (s32)count) {
            pointer = D(player + 0x1au);
            return B(pointer + 0x77u) == nation ? 10u : 0u;
        }
    }
    if (flag != 0) {
        table = D(0x00ae1050u);
        result = cm_fn_004cf5f0(table, player, club);
        if (result != 0) {
            related = D(player + 0x39u);
            if (related != 0) {
                left = D(club + 0x53u);
                right = D(related + 0x53u);
                if (left == right)
                    return B(result + 0x4fu) & 15u;
            }
        }
    }
    if (club != 0) {
        pointer = D(club + 0x53u);
        if (pointer != 0) {
            related = D(player + 0x1au);
            comparison = B(pointer + 0x77u);
            if (B(related + 0x77u) == comparison)
                return 10;
            related = D(player + 0x1eu);
            if (related != 0 && B(related + 0x77u) == comparison)
                return 10;
        }
    }
    related = D(player + 0x1au);
    if (B(related + 0x77u) == nation)
        return 10;
    related = D(player + 0x1eu);
    if (related != 0 && B(related + 0x77u) == nation)
        return 10;
    table = D(self + 0x3cu);
    if (table == 0)
        return 0;
    id = D(player);
    *(volatile u8 *)(scratch + 4) = nation;
    count = D(self + 0x44u);
    *(volatile u32 *)scratch = id;
    result = cm_fn_00946464(scratch, table, count, 6, 0x007c6d40u);
    if (result == 0)
        return 0;
    return B(result + 5u);
}
