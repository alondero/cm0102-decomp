#include "cmre.h"
/* Preferred-image VA 0x00556e90, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall: ECX carries the object, RET 0xc pops three stack words.
   The entry reads [ecx], [ecx+4], [ecx+0x14] and [ecx+0x18].
   The 0x10 local block is one allocation: the two-dword date copy at +0x0
   (0x00ae2c90, 0x00ae2c94) and the date-result buffer at +0x8 that
   0x0054c5b0 fills. Stack home [E0+8] is reused for the 0x007e82e0 result. */

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))

/* 0x0054c5b0: thiscall, RET 8. EAX is the out pointer: 0x0054c738 reloads
   the out argument, stores both result dwords through it and returns it.
   The packet's void declaration is not the physical return width. */
extern u16 * CM_THISCALL cm_fn_0054c5b0(u16 *self, u32 *out, u32 increment);
/* 0x00556d00: thiscall, RET 8; stack word 1 is a dword, stack word 2 is
   read with movsx word at 0x00556d06. Kept as the original boundary. */
extern u32 CM_THISCALL cm_fn_00556d00(u32 self, u32 arg1, s16 arg2);
/* 0x0058e340: thiscall; ECX is the object, three stack words, callee pops. */
extern void CM_THISCALL cm_fn_0058e340(u32 self, int *arg1, int *arg2, u32 arg3);
/* accepted C boundary: ECX receiver, key and create on the stack. */
extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, volatile u32 create);

u32 CM_THISCALL cm_fn_00556e90(u32 self, volatile u32 arg1,
                              volatile u32 arg2, volatile u32 arg3)
{
    u32 frame[4];
    volatile u32 *date = (volatile u32 *)frame;
    u32 objects, r, date_lo, rec, t, base, seven, callee_self;
    u32 eax, argument, record_base;
    u16 rec_hi, cur_hi, res_hi, index_word;
    s32 idx;
    s16 recidx;
    u8 tag, bumped;
    u16 *res;

    /* 0x00556e93: BYTE read from the second caller-owned stack home. */
    idx = (s32)*(volatile s8 *)&arg2;
    argument = arg1;                                       /* original cached EBX */
    objects = D(self);                                     /* 0x00556ea5 */
    /* 0x00556ea7: the receiver is objects[(s8)arg2]; pushes 0 then arg1. */
    r = cm_fn_007e82e0(*(volatile u32 *)(objects + (u32)(idx * 4)),
                       argument, 0u);                      /* 0x00556eaa */
    arg2 = r;                                              /* 0x00556eb1, DWORD home */
    eax = r;
    if (r == 0u)
        return eax;                                        /* 0x00556eb5 */
    index_word = W(r + 6u);
    eax = (eax & 0xffff0000u) | index_word;                  /* 0x00556ebb */
    if (index_word == 0xffffu)
        return eax;                                        /* 0x00556ec3 */

    recidx = (s16)index_word;                              /* 0x00556ec9 */
    date_lo = D(0x00ae2c90);                               /* 0x00556ecc */
    record_base = D(self + 4u);
    rec = record_base + (u32)recidx * 20u;                  /* 0x00556ed2..db */
    tag = *(volatile u8 *)rec;                             /* 0x00556ed8 */

    if (tag != 0xffu) {
        /* 0x00556ee7..0x00556f14 */
        t = D(0x00ae2c94);
        rec_hi = (u16)W(rec + 6u);
        eax = (record_base & 0xffff0000u) | rec_hi;          /* 0x00556eed */
        date[0] = date_lo;
        date[1] = t;
        cur_hi = W((u32)frame + 2u);
        if (cur_hi < rec_hi)
            return eax;
        if (cur_hi == rec_hi) {
            eax = (u32)((s16)date_lo < (s16)W(rec + 4u));
            if (eax != 0u)
                return eax;
        }
        eax = arg3;                                        /* 0x00556f1a, DWORD home */
        if (eax == 0u)
            return eax;
        bumped = (u8)(*(volatile u8 *)(rec + 1u) + 1u);    /* 0x00556f26 */
        *(volatile u8 *)(rec + 1u) = bumped;               /* 0x00556f2d */
        eax = (eax & 0xffffff00u) | bumped;                 /* 0x00556f2b */
        if (tag != bumped)                                 /* 0x00556f30 */
            return eax;
    } else {
        /* 0x00556f76..0x00556fb2 */
        /* EDX holds recidx*5 when only DX is overwritten by the word at
           rec+2, so the increment argument carries (recidx*5) bits 31..16. */
        u32 inc;
        u16 rec2;

        t = D(0x00ae2c94);
        rec2 = (u16)W(rec + 2u);
        date[1] = t;
        date[0] = date_lo;
        inc = (((u32)recidx * 5u) & 0xffff0000u) | (u32)rec2;
        res = cm_fn_0054c5b0((u16 *)(rec + 4u),
                              (u32 *)((u8 *)frame + 8u), inc); /* 0x00556f91 */
        eax = (u32)res;
        res_hi = W((u32)res + 2u);                         /* 0x00556f96, WORD */
        cur_hi = W((u32)frame + 2u);                       /* 0x00556f9a, WORD */
        if (cur_hi < res_hi)
            return eax;
        if (cur_hi == res_hi) {
            eax = (u32)((s16)date_lo < (s16)W((u32)res));
            if (eax != 0u)
                return eax;
        }
    }

    /* 0x00556f38 / 0x00556fb4: this[0x14] + 29*arg1 is the record slot. */
    base = D(self + 0x14u);
    seven = argument * 8u - argument;
    base = base + (argument + seven * 4u);
    callee_self = D(self + 0x18u);                         /* 0x00556f52 */
    cm_fn_0058e340(callee_self, (int *)(base + 0x14u),
                   (int *)(base + 0x18u), rec);            /* 0x00556f56 */
    /* 0x00556f61 / 0x00556fda: r's word at +6 is reloaded after the call. */
    r = arg2;                                              /* original home reload */
    return cm_fn_00556d00(self, argument, (s16)W(r + 6u)); /* 0x00556f67 / 00556fe2 */
}
