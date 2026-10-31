#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_005409b0(u32);
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00896c60(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007757d0(u32, u32, u32, u32, u32, u32);
extern u8 CM_THISCALL cm_fn_0062c400(u32, u32);
extern u32 CM_THISCALL cm_fn_0055d270(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0090e020(u32, u32);

u32 CM_THISCALL cm_fn_0090d8f0(u32 self, u32 club,
    u32 unused __attribute__((unused)), u32 selection, u32 output,
    u32 active_output, u32 seen_output, u32 flagged_output)
{
    u8 scratch[0x10];
    u32 local = (u32)scratch;
    u32 value, mode, cursor, remaining, person, id, limit, context;
    u32 lookup, pointer, index, flags, threshold;
    D(local + 0xc) = self;
    B(local + 2) = 0;
    B(local + 3) = 0;
    B(local + 1) = 0;
    value = cm_fn_0053b900(club);
    if (value != 0)
        value = cm_fn_005409b0(club) == 0;
    mode = value != 0;
    D(local + 8) = mode;
    pointer = cm_fn_00945501(8, 1);
    D(local + 4) = pointer;
    cursor = club + 0xd7;
    remaining = 0x32;
    do {
        person = D(cursor);
        if (person != 0) {
            id = D(person);
            limit = D(0x00ae2364);
            if ((s32)id >= (s32)limit) {
                context = D(0x00de1d08);
                cm_fn_00896c60(context, club, person, 1, 1);
                person = D(cursor);
                context = D(0x00de1d08);
                cm_fn_00896c60(context, club, person, 0, 1);
                D(cursor) = 0;
            } else {
                value = B(local + 3);
                B(local + 3) = (u8)(value + 1);
                context = D(0x00b74318);
                if (cm_fn_007757d0(context, person, 0x00ae2c90, club, 0, 0) == 0) {
                    value = cm_fn_0053b900(club);
                    if (value == 0) {
                        person = D(cursor);
                        value = D(person + 0x39) == club;
                    }
                    if (value != 0) {
                        value = D(local + 8);
                        person = D(cursor);
                        flags = B(person + 0x5e);
                        if (flags & (value != 0 ? 4 : 1)) {
                            value = B(local + 1);
                            B(local + 1) = (u8)(value + 1);
                        }
                        context = D(0x00b63d28);
                        if (cm_fn_0062c400(context, person) != 2) {
                            value = 0;
                            if (selection != 0 && D(selection) != 0xfffffffeu) {
                                lookup = D(selection + 0x14);
                                person = D(cursor);
                                context = D(0x00ae28f0);
                                value = cm_fn_0055d270(context, person, lookup, 1, 0);
                            }
                            if (value == 0) {
                                person = D(cursor);
                                context = D(local + 0xc);
                                value = cm_fn_0090e020(context, person);
                                if ((u8)value != 0xff) {
                                    pointer = D(local + 4);
                                    index = (u32)(s32)(s8)value;
                                    flags = B(pointer + index);
                                    B(pointer + index) = (u8)(flags + 1);
                                }
                                value = B(local + 2);
                                B(local + 2) = (u8)(value + 1);
                            }
                        }
                    }
                }
            }
        }
        cursor += 4;
        --remaining;
    } while (remaining != 0);
    pointer = D(local + 4);
    if (output != 0)
        D(output) = pointer;
    if (active_output != 0)
        B(active_output) = B(local + 2);
    if (seen_output != 0)
        B(seen_output) = B(local + 3);
    if (flagged_output != 0)
        B(flagged_output) = B(local + 1);
    if ((s8)B(local + 2) >= 0x1e) {
        context = D(local + 0xc);
        flags = B(pointer);
        threshold = B(context + (u32)(s32)(s8)mode * 8);
        /* The comparison's two successors both test the same output pointer. */
        (void)flags;
        (void)threshold;
    }
    if (output == 0)
        cm_fn_009452ca(pointer);
    return 1;
}
