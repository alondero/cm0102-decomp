/* Preferred-image VA 008cf880, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both direct callers supply ECX from 00de1f54. It is forwarded unchanged
   to 008cf7e0, despite no direct incoming ECX read in this body's listing.
   RET 24: six stack arguments; the fifth is not read. */
#include "cmre.h"
extern void CM_THISCALL cm_fn_008cf7e0(u32 self, u32 record, u32 output);
extern u32 CM_THISCALL cm_fn_008968f0(u32 self, u32 club, u32 index, u32 third);
extern u32 CM_CDECL cm_fn_0053d980(u32 player, u32 country, u32 flag);
extern u32 CM_CDECL cm_fn_0053d740(u32 player, u32 country);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output, u32 text, ...);
#define B(a) CM_GLOBAL(volatile u8, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))

u32 CM_THISCALL cm_fn_008cf880(u32 self, u32 club, u32 record,
                              u32 text, u32 players, u32 unused, u32 output)
{
    volatile u8 counters[6];
    u32 third = 0, value, player, index_word, bound, end, format;
    u8 type, count, index, temporary;
    (void)unused;
    cm_fn_008cf7e0(self, record, (u32)counters);
    type = counters[0];
    if (type == 0)
        return 0;
    value = D(record + 0x1cu);
    if (club == value && B(record + 0x40u) == 0)
        third = 1;
    else if (club == D(record + 0x20u) && B(record + 0x41u) == 0)
        third = 1;
    bound = (u8)((B(record + 0x4du) & 15u) + 11u);
    index = 0;
    index_word = record & 0xffffff00u;
    count = counters[4];
    if ((s8)bound > 0) {
        do {
            if (players == 0) {
                player = cm_fn_008968f0(D(0x00de1d08), club, index_word, third);
                if (player == 0) {
                    count = counters[4];
                    type = counters[0];
                    goto next_player;
                }
                count = counters[4];
                type = counters[0];
            } else {
                player = D(players + (u32)(s32)(s8)index * 4u);
                if (player == 0)
                    goto next_player;
            }
            if (type == 2) {
                value = cm_fn_0053d980(player, D(club + 0x53u), 1);
                if (value == 0)
                    goto reload_count;
            } else if (type == 1) {
                value = cm_fn_0053d980(player, D(club + 0x53u), 1);
                if (value == 0)
                    goto reload_count;
                value = cm_fn_0053d740(player, D(club + 0x53u));
                if (value != 0)
                    goto reload_count;
            } else {
                if (type != 3)
                    goto next_player;
                temporary = B(player + 0x18u);
                if ((s8)temporary <= (s8)counters[1])
                    goto next_player;
                if ((s8)index < 11) {
                    temporary = counters[2];
                    temporary = (u8)(temporary + 1u);
                    count = (u8)(count + 1u);
                    counters[2] = temporary;
                    counters[4] = count;
                } else {
                    temporary = counters[3];
                    temporary = (u8)(temporary + 1u);
                    count = (u8)(count + 1u);
                    counters[3] = temporary;
                    counters[4] = count;
                }
                goto next_player;
            }
            if ((s8)index < 11) {
                temporary = counters[2];
                count = counters[4];
                temporary = (u8)(temporary + 1u);
                count = (u8)(count + 1u);
                counters[2] = temporary;
                counters[4] = count;
            } else {
                temporary = counters[3];
                count = counters[4];
                temporary = (u8)(temporary + 1u);
                count = (u8)(count + 1u);
                counters[3] = temporary;
                counters[4] = count;
            }
            goto reload_type;
reload_count:
            count = counters[4];
reload_type:
            type = counters[0];
next_player:
            temporary = B(record + 0x4du);
            index = (u8)(index + 1u);
            index_word = (index_word & 0xffffff00u) | index;
            bound = (u8)((temporary & 15u) + 11u);
        } while ((s8)index < (s8)bound);
    }
    if (output != 0) {
        value = *(volatile u32 *)counters;
        D(output) = value;
        *(volatile u16 *)(output + 4u) = *(volatile u16 *)(counters + 4);
    }
    if (type == 1 || type == 2) {
        temporary = counters[5];
        if ((s8)count <= (s8)temporary)
            return 0;
        if (text == 0)
            return 1;
        if ((s8)count > 1) {
            if ((s8)temporary > 1) {
                format = type == 1 ? 0x00a8f208u : 0x00a8effcu;
                cm_fn_0066f4e0(text, format, (s32)(s8)count, (s32)(s8)temporary);
            } else {
                if (temporary == 1)
                    format = type == 1 ? 0x00a8f19cu : 0x00a8ef90u;
                else
                    format = type == 1 ? 0x00a8f134u : 0x00a8ef28u;
                cm_fn_0066f4e0(text, format, (s32)(s8)count);
            }
        } else if (temporary == 1) {
            format = type == 1 ? 0x00a8f0ccu : 0x00a8eebcu;
            cm_fn_0066f4e0(text, format, (s32)(s8)count);
        } else if (temporary == 0) {
            format = type == 1 ? 0x00a8f07cu : 0x00a8ee68u;
            cm_fn_0066f4e0(text, format);
        }
        return 1;
    }
    if (type != 3 || (s8)count <= (s8)counters[5])
        return 0;
    if (text == 0)
        return 1;
    if ((s8)count > 1)
        cm_fn_0066f4e0(text, 0x00a8ee08u, (s32)(s8)count, (s32)(s8)counters[1]);
    else
        cm_fn_0066f4e0(text, 0x00a8edbcu, (s32)(s8)counters[1]);
    temporary = counters[5];
    /* Original REPNE SCASB starts ECX=-1, retaining its finite wrap bound. */
    end = text;
    value = 0xffffffffu;
    do {
        bound = B(end);
        end++;
        value--;
    } while (value != 0 && bound != 0);
    end = text + (~value - 1u);
    if ((s8)temporary > 1)
        cm_fn_0066f4e0(end, 0x00a8ed8cu, (s32)(s8)temporary);
    else {
        format = temporary == 1 ? 0x00a8ed70u : 0x00a8ed48u;
        cm_fn_0066f4e0(end, format);
    }
    return 1;
}
