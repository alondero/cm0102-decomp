#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cced0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cd030(u32, u32, u32);
/* Actual RET12/no local ECX input; retain ECX forwarded by the root. */
extern u32 CM_THISCALL cm_fn_006830b0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_THISCALL cm_fn_0069b790(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static __attribute__((always_inline)) inline void notify(u32 club, u32 destination)
{
    u32 map, id, receiver;
    map = D(0x00adadfc);
    if (map != 0 && destination != 0) {
        id = D(destination);
        if (D(map + id * 4U) != 0)
            return;
    }
    if (cm_fn_00600a50(club, 1, 1, 0, 0) != 0) {
        receiver = D(0x00b67a44);
        cm_fn_0069b790(receiver, club, 1, 7, 0);
    }
}

static __attribute__((always_inline)) inline u32 report(u32 first, u32 second, u32 line)
{
    u32 version, eax;
    cm_fn_00944cff(0x009f15f4U, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0U, 0x009870e8U, version, first, second, line);
    eax = cm_fn_005e8290(0x009870e0U, 0x00ae24d0U, 0);
    D(0x00b67a34U) = 0;
    return eax;
}

/* Preferred-image VA00689c80/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS6/RET24/full incidental EAX, including the two final MOV AL writes.
   Scratch210: countersL2/L3,indexL4,offsetL8,selfLc; diagsL10/L110. */
u32 CM_THISCALL cm_fn_00689c80(u32 self, volatile u32 source_arg,
                              volatile u32 target_arg, volatile u32 clear_arg,
                              volatile u32 pair_arg, volatile u8 expected3,
                              volatile u8 expected2)
{
    u32 scratch[0x210 / 4];
    u32 l = (u32)scratch, clear, target, source, table, row, receiver;
    u32 index, offset, limit, search, probe, target_row, club, destination, old;
    u32 eax, mode;
    u8 type, count2, count3, expected;

    clear = clear_arg;
    D(l + 0xc) = self;
    B(l + 2) = 0;
    B(l + 3) = 0;
    if (clear != 0) {
        target = target_arg;
        if ((s16)W(target + 0x3e) > 0) {
            index = 0;
            offset = 0;
            do {
                table = D(target + 0xb1);
                row = table + offset;
                type = B(table + offset + 0x37);
                if (type != 5 && type != 0) {
                    destination = D(target + 4);
                    club = D(row);
                    receiver = D(0x00ae1050);
                    cm_fn_004cced0(receiver, club, destination);
                }
                limit = (u32)(s32)(s16)W(target + 0x3e);
                ++index;
                offset += 59U;
            } while ((s32)index < (s32)limit);
        }
    } else {
        target = target_arg;
    }

    mode = pair_arg;
    eax = mode;
    D(l + 4) = 0;
    offset = 0;
    if (mode != 0) {
        source = source_arg;
        eax = source;
        if ((s16)W(source + 0x3e) > 0) {
            D(l + 8) = 0;
            do {
                table = D(source + 0xb1);
                row = table + offset;
                type = B(table + offset + 0x37);
                if (type == 3) {
                    limit = (u32)(s32)(s16)W(target + 0x3e);
                    search = 0;
                    if ((s32)limit > 0) {
                        table = D(target + 0xb1);
                        probe = table + 0x37;
                        do {
                            type = B(probe);
                            if (type == 0 || type == 5) {
                                target_row = table + search * 59U;
                                source = source_arg;
                                destination = D(source + 4);
                                club = D(target_row);
                                receiver = D(l + 0xc);
                                cm_fn_006830b0(receiver, club, destination, 0);
                                club = D(row);
                                target = target_arg;
                                old = D(club + 0x57);
                                destination = D(target + 4);
                                D(club + 0x5b) = old;
                                D(club + 0x57) = destination;
                                receiver = D(0x00ae1050);
                                cm_fn_004cd030(receiver, club, old);
                                notify(club, destination);
                                count2 = B(l + 2);
                                count3 = B(l + 3);
                                ++count2;
                                ++count3;
                                B(target_row + 0x37) = 0xff;
                                target = target_arg;
                                B(row + 0x37) = 0xff;
                                B(l + 2) = count2;
                                B(l + 3) = count3;
                                break;
                            }
                            ++search;
                            probe += 59U;
                        } while ((s32)search < (s32)limit);
                    }
                }
                source = source_arg;
                index = D(l + 4);
                offset = D(l + 8);
                ++index;
                limit = (u32)(s32)(s16)W(source + 0x3e);
                offset += 59U;
                D(l + 4) = index;
                D(l + 8) = offset;
                eax = source;
            } while ((s32)index < (s32)limit);
        }
    } else {
        source = source_arg;
        if ((s16)W(source + 0x3e) > 0) {
            do {
                table = D(source + 0xb1);
                row = table + offset;
                if (B(row + 0x37) == 3) {
                    club = D(row);
                    destination = D(target + 4);
                    old = D(club + 0x57);
                    D(club + 0x57) = destination;
                    D(club + 0x5b) = old;
                    receiver = D(0x00ae1050);
                    cm_fn_004cd030(receiver, club, old);
                    notify(club, destination);
                    count2 = B(l + 2);
                    target = target_arg;
                    source = source_arg;
                    ++count2;
                    B(row + 0x37) = 0xff;
                    B(l + 2) = count2;
                }
                index = D(l + 4);
                offset += 59U;
                limit = (u32)(s32)(s16)W(source + 0x3e);
                ++index;
                D(l + 4) = index;
                eax = index;
            } while ((s32)index < (s32)limit);
        }
        index = 0;
        if ((s16)W(target + 0x3e) > 0) {
            offset = 0;
            do {
                table = D(target + 0xb1);
                row = table + offset;
                type = B(table + offset + 0x37);
                if (type == 0 || type == 5) {
                    destination = D(source + 4);
                    club = D(row);
                    receiver = D(l + 0xc);
                    cm_fn_006830b0(receiver, club, destination, 0);
                    count3 = B(l + 3);
                    source = source_arg;
                    ++count3;
                    B(row + 0x37) = 0xff;
                    B(l + 3) = count3;
                }
                eax = (u32)(s32)(s16)W(target + 0x3e);
                ++index;
                offset += 59U;
            } while ((s32)index < (s32)eax);
        }
    }

    expected = expected3;
    eax = (eax & 0xffffff00U) | expected;
    if (expected != 0xff && B(l + 3) != expected)
        eax = report(l + 0x110, l + 0x10, 0x15dc);
    expected = expected2;
    eax = (eax & 0xffffff00U) | expected;
    if (expected != 0xff && B(l + 2) != expected)
        eax = report(l + 0x10, l + 0x110, 0x15e2);
    return eax;
}
