#include "cmre.h"

/* Memory access helpers through the object pointers. Volatile keeps the
   listing's load/store order and widths (u8/u16/s16/u32) for every access. */
#define P8(a) (*(volatile u8 *)(u32)(a))
#define P16(a) (*(volatile u16 *)(u32)(a))
#define P32(a) (*(volatile u32 *)(u32)(a))
#define S16(a) ((s32)(s16)P16(a))

extern u16 CM_CDECL cm_fn_00933b20(u32 object, u32 index);
extern void CM_CDECL cm_fn_00934e00(u32 object);
extern void CM_CDECL cm_fn_00935140(u32 object);
extern void CM_CDECL cm_fn_00935cb0(u32 object);
extern void CM_CDECL cm_fn_00936060(u32 object, u32 value, u32 pointer);
extern u32 CM_CDECL cm_fn_009362b0(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6);
extern u32 CM_CDECL cm_fn_0093c680(u32 arg1, u32 arg2);
extern s32 CM_CDECL cm_fn_009446e0(s32 a, s32 b);

/* Cdecl, nine stack arguments (entry ESP + 4 .. + 0x24). frame[k] is the
   routine's stack slot at F + 4k, where F is the ESP after `sub esp, 8`.
   Slots 3..11 start as the incoming arguments and are reused as locals
   exactly as the listing writes them. */
u32 CM_CDECL cm_fn_00934b60(u32 a4, u32 a8, u32 ac, u32 a10, u32 a14,
                            u32 a18, u32 a1c, u32 a20, u32 a24)
{
    u32 frame[12];
    u32 esi;
    u32 ebx;
    u32 ebp;
    u32 edi;
    u32 eax;
    u32 ecx;
    u32 edx;
    u16 cx;
    u16 dx;
    u16 di;
    s32 r;
    u16 r16;

    frame[3] = a4;
    frame[4] = a8;
    frame[5] = ac;
    frame[6] = a10;
    frame[7] = a14;
    frame[8] = a18;
    frame[9] = a1c;
    frame[10] = a20;
    frame[11] = a24;

    cx = (u16)frame[5];
    ebx = frame[4];
    esi = frame[3];
    di = (u16)frame[7];

    P16(esi + 0x1a) = cx;
    dx = P16(ebx);
    P16(esi + 4) = dx;
    ecx = P32(ebx + 4);
    P32(esi + 6) = ecx;
    edx = P32(ebx + 8);
    P32(esi + 0xa) = edx;
    ecx = P32(ebx + 0x14);
    P32(esi + 0xe) = ecx;
    edx = P32(ebx + 0xc);
    P32(esi + 0x12) = edx;
    edx = P32(ebx + 0x10);
    eax = 0;
    frame[0] = eax;
    P32(esi + 0x16) = edx;
    if (di != 3) {
        goto else_path;
    }
    if (P32(esi + 0x346) != 0) {
        frame[0] = 0xffffff9cu;
        return frame[0];
    }

    /* di == 3 and [esi+0x346] == 0. */
    edi = (u32)S16(esi + 0x2da);
    eax = (u32)S16(ebx + 2);
    frame[4] = edi;
    ecx = P32(ebx + 0x1c);
    edi = (u32)S16(esi + 0x2dc);
    frame[5] = edi;
    eax += 4;
    edi = (u32)S16(esi + 0x2de);
    frame[3] = edi;
    edi = (u32)S16(esi + 0x2e0);
    frame[11] = edi;
    if ((s32)eax <= 0) {
        return frame[0];
    }
    ebx = ecx;
    ebp = edx;
    frame[7] = eax;

loop1:
    ecx = (u32)S16(ebp);
    if ((s32)ecx < (s32)frame[5]) {
        goto l1_lt_t1;
    }
    edx = frame[4];
    eax = 1;
    if ((s32)ecx < (s32)edx) {
        goto l1_end;
    }
    eax = 0;
    goto l1_end;
l1_lt_t1:
    edx = frame[3];
    eax = 4;
    if ((s32)ecx < (s32)edx) {
        goto l1_lt_t2;
    }
    eax = 2;
    goto l1_end;
l1_lt_t2:
    if ((s32)ecx <= (s32)frame[11]) {
        goto l1_end;
    }
    eax = 3;
l1_end:
    edi = eax * 4;
    eax = P32(esi + edi + 0x31e);
    r = cm_fn_009446e0((s32)eax, (s32)ecx);
    eax = (u32)r;
    ecx = P32(esi + edi + 0x30a);
    eax += ecx;
    ebp += 2;
    P32(ebx) = eax;
    eax = frame[7];
    ebx += 4;
    eax -= 1;
    frame[7] = eax;
    if (eax != 0) {
        goto loop1;
    }
    return frame[0];

else_path:
    ecx = P32(esi + 0x20);
    ebp = P32(esi + 0x1c);
    r = S16(esi + 4);           /* 0x934c94 reads [esi+4] before the store */
    P32(esi + 0x50) = eax;
    frame[3] = ecx;
    frame[1] = ebp;
    frame[5] = eax;
    if (r <= 0) {
        goto after_loop2;
    }

loop2:
    edx = P32(esi + 0xa);
    ecx = P32(esi + 6);
    edx = (u32)S16(edx + eax * 2);
    edi = (u32)S16(ecx + eax * 2);
    /* movsx edi also overwrites DI (the low word of EDI). */
    di = (u16)edi;
    if ((s32)edi >= (s32)edx) {
        goto l2_hi_ge_lo;
    }
    ebx = edx;
    eax = edi;
    ecx = edi + 1;
l3:
    ebp = frame[3];
    P16(ebp + eax * 2) = (u16)ebx;
    ebp = frame[1];
    ebx = eax;
    P16(ebp + eax * 2) = (u16)ecx;
    eax = ecx;
    ecx += 1;
    if ((s32)eax <= (s32)edx) {
        goto l3;
    }
    ebx = frame[4];
    eax = frame[5];
    P16(ebp + edx * 2) = di;
    goto l2_tail;
l2_hi_ge_lo:
    edx = frame[3];
    P16(edx + edi * 2) = di;
    P16(ebp + edi * 2) = di;
l2_tail:
    ecx = (u32)S16(esi + 4);
    eax += 1;
    frame[5] = eax;
    if ((s32)eax < (s32)ecx) {
        goto loop2;
    }
    di = (u16)frame[7];

after_loop2:
    cm_fn_00934e00(esi);
    cm_fn_00935140(esi);
    cm_fn_00935cb0(esi);
    edx = P32(esi + 0x28);
    eax = 0;
    frame[3] = edx;
    frame[5] = eax;
    if (S16(esi + 4) <= 0) {
        goto after_loop4;
    }

loop4:
    ecx = P32(esi + 0xa);
    edx = P32(esi + 6);
    ebp = (u32)S16(ecx + eax * 2);
    edi = (u32)S16(edx + eax * 2);
    if ((s32)edi >= (s32)ebp) {
        goto l4_next;
    }
    eax = frame[3];
    ebx = eax + edi * 2;
l4_inner:
    if ((P8(ebx) & 2) != 0) {
        r16 = cm_fn_00933b20(esi, edi);
        eax = r16;
        if ((s16)r16 >= 0) {
            P8(ebx) = (u8)(P8(ebx) | 0x40);
        }
    }
    edi += 1;
    ebx += 2;
    if ((s32)edi <= (s32)ebp) {
        goto l4_inner;
    }
    ebx = frame[4];
    eax = frame[5];
l4_next:
    ecx = (u32)S16(esi + 4);
    eax += 1;
    frame[5] = eax;
    if ((s32)eax < (s32)ecx) {
        goto loop4;
    }

after_loop4:
    di = (u16)frame[7];
    edx = P32(esi + 0x58);
    eax = esi + 0x50;
    cm_fn_00936060(esi, edx, eax);
    if (di != 0) {
        goto di_nonzero;
    }
    eax = cm_fn_0093c680(ebx, esi);
    frame[0] = eax;
    return eax;

di_nonzero:
    if (di != 2) {
        goto not_two;
    }
    eax = frame[11];
    ecx = frame[10];
    edx = frame[9];
    eax = frame[8];
    r = (s32)cm_fn_009362b0(esi, ebx, frame[8], frame[9], frame[10], frame[11]);
    frame[0] = (u32)r;
    return (u32)r;

not_two:
    if (di == 1) {
        return frame[0];
    }
    frame[0] = 0xffffffffu;
    return frame[0];
}
