#include "cmre.h"
/* Preferred-image VA005acbe0, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS9/RET36/fullEAX with final MOV AX count. Scratch224: indexBYTEL3,
   kindcountL4/kindsL8..f/yearL10/matchesL14/dayL18/phaseBYTEL1c/selfL20,
   diagnosticsL24/L124. Time-slot upper bytes are not observed: original
   5abf10 masks its third stack argument with ff before use. Do not initialize
   those bytes. Iterator68d1b0 writes real next-node EDX at entry/return;
   capture EDX:EAX and forward that EDX to FAST4 68d0d0. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_0048dee0(u32);
extern u32 CM_CDECL cm_fn_005af5b0(u32);
extern u32 CM_CDECL cm_fn_0048dd70(u32, u32);
extern u32 CM_THISCALL cm_fn_005abf10(u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32, u32, u32, s16);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005acbe0(u32 self, volatile u32 first_day,
                              volatile u32 first_year, volatile u16 last_day,
                              volatile u16 last_year, volatile u32 club_arg,
                              volatile u32 output_arg, volatile u32 key_arg,
                              volatile u8 filter_arg, volatile u8 side_arg)
{
    u8 scratch[0x224];
    u32 l = (u32)scratch, eax, edx, club, output, base, offset, pointer;
    u32 kind_count, index, other, list, day, owner, key, version;
    u8 byte, current, side, filter;
    unsigned long long pair;
    eax = first_day; edx = first_year; club = club_arg;
    D(l + 0x20) = self; D(l + 4) = 0;
    D(l + 0x18) = eax; D(l + 0x10) = edx;
    B(l + 0x1c) = 0; D(l + 0x14) = 0;
    if (club != 0xffffffffu) {
        base = G(0x00ae23b4); offset = club * 581u;
        eax = cm_fn_0053b900(offset + base);
        if (eax != 0) {
            B(l + 8) = 0; B(l + 9) = 1; D(l + 4) = 2;
        } else {
            base = G(0x00ae23b4);
            B(l + 8) = 2; B(l + 9) = 3; B(l + 0xa) = 4; B(l + 0xb) = 5;
            B(l + 0xc) = 6; B(l + 0xd) = 7;
            eax = cm_fn_0048dee0(offset + base);
            base = G(0x00ae23b4); B(l + 0xe) = (u8)eax;
            eax = cm_fn_005af5b0(offset + base); B(l + 0xf) = (u8)eax;
            D(l + 4) = 8;
            for (index = 0; index < 7; ++index) {
                other = index + 1; eax = other;
                current = B(l + 8 + index);
                while (other < 8) {
                    if (current == B(l + 8 + other)) {
                        B(l + 8 + index) = 0xff; break;
                    }
                    ++other; eax = other;
                }
            }
        }
    } else {
        edx = key_arg;
        if ((s32)edx > -1) {
            base = G(0x00ae23d0);
            eax = cm_fn_0048dd70(base + edx * 107u, 0);
            B(l + 8) = (u8)eax; D(l + 4) = 1;
        }
    }
    kind_count = D(l + 4);
    side = side_arg; output = output_arg;
day_start:
    day = D(l + 0x18);
    if ((u16)day >= 366) goto next_year;
date_guard:
    edx = D(l + 0x10);
    eax = (eax & 0xffff0000u) | last_year;
    if ((u16)edx > (u16)eax) goto done;
    if ((u16)edx == (u16)eax && (u16)day > last_day) goto done;
phase_start:
    B(l + 3) = 0;
    if ((s32)kind_count > 0) {
        eax = 0;
kind_loop:
        eax = (eax & 0xffffff00u) | B(l + 8 + eax);
        if ((u8)eax != 0xff) {
            owner = D(l + 0x20);
            eax = cm_fn_005abf10(owner, edx, day, B(l + 0x1c), eax);
            list = eax;
            if (list != 0) {
                pointer = D(list + 2); D(list + 0xa) = pointer;
                pair = cm_fn_0068d1b0(list);
                eax = (u32)pair; edx = (u32)(pair >> 32);
                while (eax != 0) {
                    filter = filter_arg;
                    if (filter != 0xfe && filter != B(eax + 0x42)) goto next_record;
                    if (club != 0xffffffffu) {
                        pointer = D(eax + 0xc);
                        if (club != pointer && club != D(eax + 0x10)) goto next_record;
                        if (side != 0xff) {
                            if (club == pointer) {
                                if (B(eax + 0x40) != side) goto next_record;
                            } else if (B(eax + 0x41) != side) goto next_record;
                        }
                    }
                    key = key_arg;
                    if (key != 0xffffffffu && key != D(eax)) goto next_record;
                    if (output != 0) {
                        pair = cm_fn_0068d0d0(output, edx, eax, 0x4f);
                        eax = (u32)pair; edx = (u32)(pair >> 32);
                        if (eax == 0) goto append_error;
                    }
                    D(l + 0x14) = D(l + 0x14) + 1u;
next_record:
                    pair = cm_fn_0068d1b0(list);
                    eax = (u32)pair; edx = (u32)(pair >> 32);
                }
            }
            kind_count = D(l + 4); edx = D(l + 0x10); day = D(l + 0x18);
        }
        byte = B(l + 3); ++byte; B(l + 3) = byte;
        eax = (u32)(s32)(s8)byte;
        if ((s32)eax < (s32)kind_count) goto kind_loop;
    }
    byte = B(l + 0x1c); ++byte;
    eax = (eax & 0xffffff00u) | byte;
    B(l + 0x1c) = byte;
    if (byte < 3) goto phase_start;
    ++day; B(l + 0x1c) = 0; D(l + 0x18) = day;
    if ((u16)day < 366) goto date_guard;
next_year:
    eax = D(l + 0x10); D(l + 0x18) = 0;
    ++eax; D(l + 0x10) = eax;
    goto day_start;
append_error:
    cm_fn_00944cff(0x009cc208, 0, 0, l + 0x124, l + 0x24);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x124, l + 0x24, 0x93b);
    eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    G(0x00b67a34) = 0;
done:
    return (eax & 0xffff0000u) | W(l + 0x14);
}
