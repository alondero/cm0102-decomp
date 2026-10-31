/* WIP: budget-asymmetry, case 9 (original copy fault, recovered budget).
   Keep the 2000-case input policy and forward copy; the current shared
   instruction budget does not establish equivalence. */
#include "cmre.h"

/* Preferred-image VA 0x0062e9e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   EBX alternates between a full removal count and a partial BYTE injury
   quota. Keep the saved count, cached date high DWORD and forward copies. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_0062d250(u32, u32, u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0062e9e0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x2c];
    u32 local = (u32)scratch, previous, removed = 0, value, row, base, offset;
    u32 low, high, total, index, club, player, player_id, payload, raw_flag;
    u32 attempts, source, dest, bytes, words, tail, result;
    u16 today_year, row_year, cutoff_year, cutoff_day;
    u8 flag, byte;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b188;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 8) = 0;
    cm_fn_0054c8e0(0x00ae2c90, local + 0x1c, 5);
    total = D(self + 0x24);
    frame.state = 0;
    D(local + 12) = 0;
    if ((s32)total <= 0) goto reload_count;
    high = D(0x00ae2c94);
    D(local) = 0;
    do {
        base = D(self + 0x1c);
        offset = D(local);
        low = D(0x00ae2c90);
        row = base + offset;
        D(local + 0x14) = low;
        today_year = W(local + 0x16);
        row_year = W(row + 2);
        D(local + 0x18) = high;
        if (today_year < row_year) break;
        if (today_year == row_year && (s16)low < (s16)W(row)) break;
        row_year = W(row + 2);
        cutoff_year = W(local + 0x1e);
        if (cutoff_year > row_year) goto expired;
        if (cutoff_year == row_year) {
            cutoff_day = W(local + 0x1c);
            if ((s16)cutoff_day > (s16)W(row)) goto expired;
        }
        if (B(row + 8) == 3) {
            low = D(0x00ae2c90);
            D(local + 0x28) = high;
            D(local + 0x24) = low;
            today_year = W(local + 0x26);
            if (today_year != W(row + 2) || (u16)low != W(row)) goto next_record;
            removed = cm_fn_0090cfc0(4);
            removed = (removed & 0xffffff00u) | (u8)((u8)removed + 1u);
        } else {
            value = cm_fn_0090cfc0(3);
            removed = (removed & 0xffffff00u) | (u8)value;
        }
        base = D(self + 0x1c);
        offset = D(local);
        index = D(base + offset + 9);
        base = D(0x00ae23b4);
        club = base + index * 581u;
        D(local + 0x10) = club;
        value = cm_fn_0053b900(club);
        if (value != 0) goto quota_done;
        D(local + 4) = value;
        for (;;) {
            if ((s8)removed <= 0) break;
            value = cm_fn_0090cfc0(50);
            index = (u32)(s32)(s8)value;
            player = D(club + index * 4u + 0xd7);
            if (player != 0) {
                player_id = D(player);
                total = D(0x00ae2364);
                if ((s32)player_id < (s32)total) {
                    base = D(self + 0x1c);
                    offset = D(local);
                    row = base + offset;
                    flag = B(row + 8);
                    base = D(self);
                    if (B(base + player_id * 31u + 0x1d) != flag) {
                        payload = D(row + 13);
                        raw_flag = (offset & 0xffffff00u) | flag;
                        cm_fn_0062d250(self, raw_flag, 0, player, 0, 0, payload, 0, 0);
                        removed = (removed & 0xffffff00u) | (u8)((u8)removed - 1u);
                    }
                }
            }
            attempts = D(local + 4) + 1u;
            D(local + 4) = attempts;
            if ((s32)attempts >= 20) break;
            club = D(local + 0x10);
        }
quota_done:
        high = D(0x00ae2c94);
        removed = D(local + 8);
        goto next_record;
expired:
        ++removed;
        D(local + 8) = removed;
next_record:
        value = D(local + 12);
        offset = D(local);
        total = D(self + 0x24);
        ++value;
        offset += 17;
        D(local + 12) = value;
        D(local) = offset;
    } while ((s32)value < (s32)total);
    total = D(self + 0x24);
    if (removed == 0) goto finish;
    if (removed == total) goto reload_count;
    dest = D(self + 0x1c);
    bytes = (total - removed) * 17u;
    source = dest + removed * 17u;
    words = bytes >> 2;
    while (words != 0) {
        value = D(source); D(dest) = value;
        source += 4; dest += 4; --words;
    }
    tail = bytes & 3u;
    while (tail != 0) {
        byte = B(source); B(dest) = byte;
        ++source; ++dest; --tail;
    }
reload_count:
    total = D(self + 0x24);
finish:
    previous = frame.prev;
    result = total - removed;
    D(self + 0x24) = result;
    CM_FS(u32, 0) = previous;
    return result;
}
