/* WIP: events mismatch, case 2: transitive FindFirstFileA output remains
   unwritten in the frozen import scaffolding; differing original/C stack
   contents change 004c0aa0's attribute filter and the DeleteFileA call count. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff010(u32, u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
#define SET(index, value) cm_fn_007ff270(0x00b74450U, (index), (value), 0)

/* Preferred-image VA 008702a0/base00400000, pinned 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL5. regparm(1) transports incoming EAX separately from the five original
   stack arguments: early exits retain its high 24 bits after MOV AL,[self+3d].
   Scratch 200: saved 60 DWORDs at L0; diagnostic buffers L100/L0. */
u32 CM_CDECL __attribute__((regparm(1))) cm_fn_008702a0(
    u32 eax_in, volatile u32 self_arg, volatile s8 mode_arg,
    volatile s8 other_arg, volatile u32 restore_arg, volatile s8 final_arg)
{
    u32 scratch[0x200 / 4];
    u32 l = (u32)scratch, self, limit, id, eax, saved, i, value, secondary;
    u8 type;
    s8 mode, final;

    self = self_arg;
    if (self == 0) {
        cm_fn_00944cff(0x00a84aac, 0, 0, l + 0x100, l);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax, l + 0x100, l, 0x149);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return eax;
    }
    type = B(self + 0x3d);
    eax = (eax_in & 0xffffff00U) | type;
    limit = D(0x00ae2364);
    if (type != 2) {
        id = D(self);
        if ((s32)id < (s32)(limit - 16U)) goto update;
        if ((s32)id >= (s32)limit) goto update;
    }
    if (D(self + 0x61) != 0) {
        if ((s32)D(self) < (s32)limit) goto update;
    }
    if (type == 1) return eax;
    if (B(self + 0x28) == 5) {
        if (D(self + 0x39) == 0) goto second_checks;
        if (type != 11) goto second_checks;
    }
    if (type != 2) {
        id = D(self);
        if ((s32)id < (s32)(limit - 16U)) {
            if ((s32)id < (s32)limit) goto update;
        }
    }
second_checks:
    if (type == 1) return eax;
    if (type != 2) {
        id = D(self);
        if ((s32)id < (s32)(limit - 16U)) {
            if ((s32)id < (s32)limit) goto update;
        }
    }
    if (type == 1 || type == 2) return eax;
    if ((s32)D(self) >= (s32)limit) return eax;

update:
    saved = restore_arg;
    if (saved != 0) {
        for (i = 0; i != 60; ++i) {
            value = cm_fn_007ff010(0x00b74450, i);
            D(l + i * 4U) = value;
        }
        limit = D(0x00ae2364);
    }
    id = D(self);
    if ((s32)id >= (s32)(limit - 16U) && (s32)id < (s32)limit)
        mode = 5;
    else
        mode = mode_arg;
    eax = cm_fn_007fe600(0x00b74450, 0x00870700, 0x00888260, 0, 0x0088b8a0, 0);
    if (eax == 0) return eax;
    SET(0, self);
    SET(1, (u32)(s32)mode);
    value = (u32)(s32)other_arg;
    SET(2, value);
    SET(3, 0);
    id = D(self);
    limit = D(0x00ae2364);
    final = final_arg;
    if ((s32)id >= (s32)limit) goto ordinary;
    if (D(self + 0x69) == 0) goto ordinary;
    if (D(self + 0x61) != 0) {
        type = B(self + 0x3d);
        if (type == 11 || type == 15 || type == 13 || type == 12) {
            if (final != 1) goto ordinary;
        }
    }
    SET(4, 1);
    secondary = 1;
    goto remaining;
ordinary:
    SET(4, (u32)(s32)final);
    secondary = 2;
remaining:
    SET(5, secondary);
    SET(6, 0);
    SET(7, 0);
    SET(8, 0);
    SET(9, 0);
    SET(10, 0);
    SET(11, 0);
    SET(12, 0);
    SET(13, 0);
    SET(14, 0);
    SET(15, 1);
    SET(16, 1);
    value = W(0x00aebdf0);
    SET(17, value);
    value = W(0x00ae3184);
    SET(18, value);
    SET(19, 0);
    SET(20, 1);
    SET(21, 1);
    eax = restore_arg;
    if (eax == 0) return eax;
    value = D(l + 4);
    SET(1, value);
    value = D(l + 8);
    SET(2, value);
    value = D(l + 0x14);
    SET(5, value);
    value = D(l + 0x4c);
    eax = SET(19, value);
    if (D(l + 0x10) == 2)
        eax = SET(4, 2);
    return eax;
}
