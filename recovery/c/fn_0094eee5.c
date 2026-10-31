#include "cmre.h"

/* Callees. 0x0094c322 is cdecl (the caller pops two dwords); it is passed the
   byte and a class mask. 0x009471cd is cdecl with one dword argument. */
extern u32 CM_CDECL cm_fn_0094c322(u32 c, u32 mask);
extern u32 CM_CDECL cm_fn_009471cd(u32 c);

#define RD_U8(addr) (*(volatile u8 *)(addr))
#define RD_U16(addr) (*(volatile u16 *)(addr))
#define RD_U32(addr) (*(volatile u32 *)(addr))

/* Arguments (cdecl): str at [esp+4], endptr at [esp+8], base at [esp+0xc],
   flags at [esp+0x10] (the original reassigns the base and flags slots, so
   they are copied to locals). Locals: value = [ebp-8], cursor = [ebp-4],
   cutoff = [ebp-0xc]. Table at global 0x00ad8fd0 holds 2-byte class entries;
   0x00ad91dc gates calls to 0x0094c322; 0x00de2f8c receives the range error. */
u32 CM_CDECL cm_fn_0094eee5(u32 str, u32 endptr, s32 base_arg, u32 flags_arg)
{
    u32 value;
    u32 cursor;
    u32 cutoff;
    u32 eax;
    u32 ecx;
    u32 esi;
    u32 edx;
    u32 flags;
    u32 rem;
    s32 base;
    u8 bl;
    u8 al;

    flags = flags_arg;
    base = base_arg;
    value = 0;
    bl = RD_U8(str);
    esi = str + 1u;
    cursor = esi;

    /* Skip leading class-8 characters. */
    for (;;) {
        if (CM_GLOBAL(volatile s32, 0x00ad91dc) > 1) {
            eax = cm_fn_0094c322((u32)bl, 8u);
        } else {
            ecx = CM_GLOBAL(volatile u32, 0x00ad8fd0);
            eax = (u32)bl;
            eax = (u32)RD_U8(ecx + eax * 2u) & 8u;
        }
        if (eax == 0u) break;
        bl = RD_U8(esi);
        esi = esi + 1u;
    }

    cursor = esi;
    if (bl == 0x2d) {
        flags = flags | 2u;
    }
    if (bl == 0x2d || bl == 0x2b) {
        bl = RD_U8(esi);
        esi = esi + 1u;
        cursor = esi;
    }

    if (base < 0 || base == 1 || base > 0x24) {
        if (endptr != 0u) RD_U32(endptr) = str;
        return 0;
    }

    if (base == 0) {
        if (bl != 0x30) {
            base = 10;
            goto L_efaa;
        }
        al = RD_U8(esi);
        if (al == 0x78 || al == 0x58) {
            base = 16;
        } else {
            base = 8;
            goto L_efaa;
        }
    }
    if (base == 16 && bl == 0x30) {
        al = RD_U8(esi);
        if (al == 0x78 || al == 0x58) {
            bl = RD_U8(esi + 1u);
            esi = esi + 2u;
            cursor = esi;
        }
    }

L_efaa:
    eax = 0xffffffffu;
    cutoff = eax / (u32)base;

    /* Digit loop. */
    for (;;) {
        esi = (u32)bl;
        if (CM_GLOBAL(volatile s32, 0x00ad91dc) > 1) {
            eax = cm_fn_0094c322(esi, 4u);
        } else {
            ecx = CM_GLOBAL(volatile u32, 0x00ad8fd0);
            eax = (u32)RD_U8(ecx + esi * 2u) & 4u;
        }
        if (eax != 0u) {
            ecx = (u32)((s32)(s8)bl - 0x30);
        } else {
            if (CM_GLOBAL(volatile s32, 0x00ad91dc) > 1) {
                eax = cm_fn_0094c322(esi, 0x103u);
            } else {
                ecx = CM_GLOBAL(volatile u32, 0x00ad8fd0);
                eax = (u32)RD_U16(ecx + esi * 2u) & 0x103u;
            }
            if (eax == 0u) goto L_end;
            eax = cm_fn_009471cd((u32)(s32)(s8)bl);
            ecx = eax - 0x37u;
        }

        if (ecx >= (u32)base) goto L_end;

        esi = value;
        flags = flags | 8u;
        if (esi < cutoff) goto L_accum;
        if (esi != cutoff) goto L_overflow;
        eax = 0xffffffffu;
        edx = 0u;
        rem = 0xffffffffu % (u32)base;
        edx = rem;
        if (ecx <= edx) goto L_accum;

L_overflow:
        flags = flags | 4u;
        goto L_next;

L_accum:
        esi = esi * (u32)base;
        esi = esi + ecx;
        value = esi;

L_next:
        eax = cursor;
        cursor = cursor + 1u;
        bl = RD_U8(eax);
    }

L_end:
    ecx = flags;
    cursor = cursor - 1u;
    edx = endptr;
    if ((flags & 8u) == 0u) {
        if (edx != 0u) cursor = str;
        value = 0u;
        goto L_exit;
    }

    eax = 0x7fffffffu;
    if ((flags & 4u) != 0u) goto L_clamp;
    if ((flags & 1u) != 0u) goto L_exit;
    ecx = flags & 2u;
    if (ecx != 0u) {
        if (value > 0x80000000u) goto L_clamp;
    }
    if (ecx != 0u) goto L_exit;
    if (value <= eax) goto L_exit;

L_clamp:
    CM_GLOBAL(volatile u32, 0x00de2f8c) = 0x22u;
    if ((flags & 1u) != 0u) {
        value = 0xffffffffu;
    } else {
        value = (flags & 2u) ? 0x80000000u : 0x7fffffffu;
    }

L_exit:
    edx = endptr;
    if (edx != 0u) RD_U32(edx) = cursor;
    if ((flags & 2u) != 0u) value = 0u - value;
    return value;
}
