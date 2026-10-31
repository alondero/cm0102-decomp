#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_THISCALL cm_fn_0068d020(u32);
extern void CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_004cca30(u32, u32);
extern u32 CM_THISCALL cm_fn_008c5e40(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008c5fc0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008cde70(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053d7f0(u32);
extern u32 CM_CDECL cm_fn_0053d980(u32, u32, u32);

/* Preferred-image VA008ce990/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full0or1. SEH963f78. Scratch10 is the list object;
   its original constructor initializes only the first14 bytes. */
u32 CM_THISCALL cm_fn_008ce990(u32 self, volatile u32 person_arg,
                              volatile u32 club_arg, volatile u32 output_arg,
                              volatile u32 mode_arg)
{
    volatile struct { u32 prev, handler, state; } seh;
    u32 scratch[4];
    u32 l = (u32)scratch, person, club, nation, pointer, id, value;
    u32 old_club, output, flags, slot, object, count, base, code, result;
    u8 type, byte;

    seh.state = 0xffffffffU;
    seh.handler = 0x00963f78U;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;
    cm_fn_0068d020(l);
    person = person_arg;
    club = club_arg;
    seh.state = 0;
    nation = D(person + 0x1a);
    if (nation != 0) {
        id = D(nation);
        value = D(0x009cf458);
        if (id == value) {
            old_club = D(person + 0x39);
            if (old_club == 0 || D(old_club + 0x53) == nation) {
                if (club != 0 && D(club + 0x53) != nation) {
                    code = 0x27;
                    goto failed;
                }
            }
        }
    }
    id = D(person);
    flags = D(self + 0x86e);
    if ((B(flags + id * 8U) & 0x40U) != 0) {
        pointer = D(0x00ae1050);
        if (cm_fn_004cca30(pointer, id) != 0) {
            code = 0x21;
            goto failed;
        }
        id = D(person);
        flags = D(self + 0x86e);
        pointer = flags + id * 8U;
        byte = B(pointer);
        B(pointer) = byte & 0xbfU;
    }
    value = mode_arg;
    if (value == 0) {
        value = cm_fn_008c5e40(self, person, 0);
        if (value == 0)
            value = cm_fn_008c5fc0(self, person, value);
        if (value != 0) {
            code = 7;
            goto failed;
        }
    }
    value = D(self + 0x92e);
    D(self + 0x936) = value;
    pointer = cm_fn_0068d1b0(self + 0x92c);
    while (pointer != 0) {
        id = D(person);
        value = D(pointer);
        if (id == value) {
            code = 1;
            goto failed;
        }
        pointer = cm_fn_0068d1b0(self + 0x92c);
    }
    /* The root tests the complete physical EAX returned by this boundary. */
    value = cm_fn_008cde70(self, person, 0);
    if (value != 0) {
        cm_fn_008c5cf0(self, person, l, 0);
        value = D(l + 2);
        D(l + 0xa) = value;
        pointer = cm_fn_0068d1b0(l);
        while (pointer != 0) {
            id = D(pointer);
            count = D(self + 4);
            if ((s32)id < (s32)count) {
                base = D(self);
                slot = base + id * 4U;
            } else {
                slot = D(self);
            }
            object = D(slot);
            type = B(object + 0x2c);
            if (type == 10 || type == 11 || type == 12 || type == 13 || type == 14) {
                output = output_arg;
                B(object + 0x2d) = 1;
                if (output != 0)
                    W(output) = 0x12;
                goto cleanup_failure;
            }
            if (type == 19) {
                code = 0x20;
                goto failed;
            }
            pointer = cm_fn_0068d1b0(l);
        }
    }
    if (club == 0 || person == 0)
        goto success;
    if (D(person + 0x61) == 0)
        goto success;
    if (cm_fn_0053d7f0(club) != 0) {
        id = D(0x009cf424);
        base = D(0x00ae23a8);
        nation = base + id * 290U;
        if (cm_fn_0053d980(person, nation, 1) != 0) {
            code = 0x19;
            goto failed;
        }
        goto success;
    }
    nation = D(club + 0x53);
    if (nation != 0) {
        id = D(nation);
        value = D(0x009cf4dc);
        if (id == value) {
            pointer = D(club + 0x57);
            if (pointer != 0) {
                id = D(pointer);
                value = D(0x009cf8d8);
                if (id == value)
                    goto fallback;
            }
            if (cm_fn_0053d980(person, nation, 1) != 0) {
                code = 0x28;
                goto failed;
            }
            goto success;
        }
    }
fallback:
    id = D(club);
    value = D(0x009d0128);
    if (id != value) {
        if (id != D(0x009d01a0))
            goto success;
    }
    pointer = D(person + 0x1a);
    if (pointer != 0) {
        id = D(pointer);
        value = D(0x009cf488);
        if (id != value)
            goto success;
    }
    pointer = D(person + 0x1e);
    if (pointer != 0) {
        id = D(pointer);
        value = D(0x009cf424);
        if (id == value)
            goto success;
    }
    code = 0x1a;
failed:
    output = output_arg;
    if (output != 0)
        W(output) = (u16)code;
cleanup_failure:
    seh.state = 0xffffffffU;
    cm_fn_0068d040(l);
    result = 0;
    goto restore;
success:
    output = output_arg;
    if (output != 0)
        W(output) = 0;
    seh.state = 0xffffffffU;
    cm_fn_0068d040(l);
    result = 1;
restore:
    CM_FS(u32, 0) = seh.prev;
    return result;
}
