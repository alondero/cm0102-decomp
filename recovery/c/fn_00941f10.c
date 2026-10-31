/* WIP: regs (EAX) mismatch, case 144: when obj == 0 the original returns the entry EAX unchanged. Plain C cannot read it; the return of 0 here is a placeholder, not evidence. */
#include "cmre.h"

/* Callees with cdecl frames (the caller pops their arguments). */
extern s32 CM_CDECL cm_fn_00937090(s32 param_1, u32 param_2, u32 param_3, u32 param_4, s32 param_5);
extern s32 CM_CDECL cm_fn_009446e0(u32 param_1, u32 param_2);

#define RD_U8(addr) (*(volatile u8 *)(addr))
#define RD_S16(addr) (*(volatile s16 *)(addr))
#define RD_U16(addr) (*(volatile u16 *)(addr))
#define RD_U32(addr) (*(volatile u32 *)(addr))

/* Arguments (cdecl): obj is the object at [esp+4] on entry, ctx is the second
   argument at [esp+8]. Fields are named by offset only:
     obj+0x14 dword, obj+0x120..0x148 written here, obj+0x18 sub-object,
     obj+0x130/0x134 u16 arrays, obj+0xd0/0xd4 dword limits (0x10000 disables),
     ctx+0x34 u16, ctx+0x36 s16 index, ctx+0x38..0x44 dwords, ctx+0x4c and
     ctx+0x50 dword pointers to u32 arrays, ctx+0x60/0x64 dwords.
   The early returns leave EAX as the listing has it; the first one (obj == 0)
   returns the entry EAX in the original, which plain C cannot read. */
s32 CM_CDECL cm_fn_00941f10(u32 obj, u32 ctx)
{
    s32 esi;
    s32 k;
    s32 r;
    s32 cnt;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 edi;
    u32 L14;
    u32 p4;

    if (obj == 0) return 0;
    if (RD_U32(ctx + 0x60) == 0) return 0;
    eax = RD_U32(ctx + 0x64);
    if ((s32)eax <= 0) return (s32)eax;

    ecx = RD_U32(ctx + 0x4c);
    eax = RD_U32(obj + 0x14);
    esi = (s32)RD_S16(ctx + 0x36) + 4;
    RD_U32(obj + 0x120) = ecx;
    edx = RD_U32(ctx + 0x50);
    RD_U32(obj + 0x124) = edx;
    RD_U32(obj + 0x128) = eax;
    edx = (u32)esi * 4u;
    eax = eax + edx;
    RD_U32(obj + 0x12c) = eax;
    ecx = RD_U32(ctx + 0x40);
    RD_U32(obj + 0x130) = ecx;
    ecx = RD_U32(ctx + 0x44);
    RD_U32(obj + 0x134) = ecx;
    ecx = RD_U32(ctx + 0x48);
    RD_U32(obj + 0x138) = ecx;
    RD_U16(obj + 0x13c) = RD_U16(ctx + 0x34);
    RD_U16(obj + 0x13e) = 0;
    ecx = RD_U32(ctx + 0x38);
    edx = edx + eax;
    RD_U32(obj + 0x140) = ecx;
    ecx = RD_U32(ctx + 0x3c);
    RD_U32(obj + 0x144) = ecx;
    ecx = RD_U32(obj + 0x128);
    eax = 0;
    RD_U32(obj + 0x148) = edx;
    edi = RD_U32(ctx + 0x4c);
    L14 = ecx;
    if (esi > 0) {
        edi = edi - ecx;
        L14 = edi;
        for (k = 0; k < esi; k++) {
            edi = L14;
            edi = RD_U32(edi + ecx);
            RD_U32(ecx) = edi;
            RD_U8(edx + (u32)k) = 0;
            ecx = ecx + 4u;
        }
    }

    eax = RD_U32(obj + 0x12c);
    ecx = RD_U32(ctx + 0x50);
    if (esi > 0) {
        ecx = ecx - eax;
        edx = (u32)esi;
        do {
            edi = RD_U32(eax + ecx);
            RD_U32(eax) = edi;
            eax = eax + 4u;
            edx = edx - 1u;
        } while (edx != 0);
    }

    eax = RD_U32(obj + 0xd4);
    L14 = eax;
    if (eax != 0x10000u) {
        edi = RD_U32(obj + 0x134);
        if (esi > 0) {
            cnt = esi;
            do {
                eax = L14;
                edx = (u32)(s32)RD_S16(edi);
                r = cm_fn_009446e0(edx, eax);
                RD_U16(edi) = (u16)r;
                edi = edi + 2u;
                cnt = cnt - 1;
            } while (cnt != 0);
        }
    }

    eax = RD_U32(obj + 0xd0);
    L14 = eax;
    if (eax != 0x10000u) {
        edi = RD_U32(obj + 0x130);
        if (esi > 0) {
            do {
                eax = L14;
                r = cm_fn_009446e0((u32)(s32)RD_S16(edi), eax);
                RD_U16(edi) = (u16)r;
                edi = edi + 2u;
                esi = esi - 1;
            } while (esi != 0);
        }
    }

    edx = (u32)(s32)RD_S16(ctx + 0x36);
    eax = RD_U32(ctx + 0x4c);
    esi = RD_U32(eax + edx * 4u);
    ecx = RD_U32(eax + edx * 4u + 4u);
    ecx = ecx - esi;
    esi = esi + 0x20u;
    RD_U32(eax + edx * 4u) = esi;
    ecx = ecx + 0x20u;
    edx = (u32)(s32)RD_S16(ctx + 0x36);
    esi = RD_U32(eax + edx * 4u);
    ecx = ecx & 0xffffffc0u;
    edx = eax + edx * 4u;
    esi = esi & 0xffffffc0u;
    RD_U32(edx) = esi;
    edx = (u32)(s32)RD_S16(ctx + 0x36);
    esi = RD_U32(eax + edx * 4u);
    esi = esi + ecx;
    RD_U32(eax + edx * 4u + 4u) = esi;

    eax = RD_U32(ctx + 0x50);
    edx = (u32)(s32)RD_S16(ctx + 0x36);
    esi = RD_U32(eax + edx * 4u + 8u);
    ecx = RD_U32(eax + edx * 4u + 0xcu);
    ecx = ecx - esi;
    esi = esi + 0x20u;
    RD_U32(eax + edx * 4u + 8u) = esi;
    ecx = ecx + 0x20u;
    edx = (u32)(s32)RD_S16(ctx + 0x36);
    esi = RD_U32(eax + edx * 4u + 8u);
    ecx = ecx & 0xffffffc0u;
    edx = eax + edx * 4u + 8u;
    esi = esi & 0xffffffc0u;
    RD_U32(edx) = esi;
    edx = (u32)(s32)RD_S16(ctx + 0x36);
    esi = RD_U32(eax + edx * 4u + 8u);
    esi = esi + ecx;
    ecx = 0xdu;
    RD_U32(eax + edx * 4u + 0xcu) = esi;

    /* Copy 13 dwords from obj+0x3c to obj+0x70, then clear two bytes. */
    p4 = obj + 0x18u;
    eax = p4;
    obj = obj + 0xf4u;
    for (k = 0; k < 13; k++) {
        RD_U32(eax + 0x58u + (u32)k * 4u) = RD_U32(eax + 0x24u + (u32)k * 4u);
    }
    RD_U8(eax + 0xc9u) = 0;
    RD_U8(eax + 0xc8u) = 0;

    ecx = RD_U32(ctx + 0x60);
    edi = RD_U32(ctx + 0x64);
    eax = ecx + edi;
    r = cm_fn_00937090((s32)obj, ecx, eax, p4, 0);
    return r;
}
