#include "cmre.h"

/* Preferred-image VA 0x0055e620, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch offsets use L = ESP immediately after SUB ESP,114h. The getter
   reads only the low byte of its stack argument (VA 00785830). */
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u8);
extern unsigned long long CM_CDECL cm_fn_007e52c0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_0055e620(u32 arg1, u32 arg2)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x114];
    u32 work = (u32)scratch, eax, receiver, i, index, entry, field;
    u32 fifth = 0, sixth = 0;
    static const u8 destinations[15] = {
        0x0d, 0x14, 0x0b, 0x10, 0, 0, 4, 5, 6, 7, 0x0f, 0x0e, 0x18, 0x0c, 3
    };

    frame.state = 0xffffffffu;
    frame.handler = 0x00958026;
    eax = CM_FS(u32, 0);
    frame.prev = eax;
    CM_FS(u32, 0) = (u32)&frame;
    B(work + 2) = 0;
    cm_fn_0077bbe0(work + 0x34, 0, 0);
    frame.state = 0;
    cm_fn_0054a110(work + 0x2c);
    B((u32)&frame.state) = 1;
    eax = cm_fn_007fe600(0x00b74450, 0x0055ea10, 0x0055ed00, 1, 0, 0);
    if (!eax) goto done;
    receiver = D(0x00b7431c);
    cm_fn_00786b70(receiver, arg1, arg2, work + 0x34);
    for (i = 0; i < 15; ++i) {
        index = B(work + 2);
        B(work + 2) = (u8)(index + 1);
        eax = (u32)cm_fn_00785830(work + 0x34, (u8)index);
        if (i == 4) fifth = eax;
        else if (i == 5) sixth = eax;
        else if (i == 1 || i == 3 || i == 12) D(work + destinations[i]) = eax;
        else B(work + destinations[i]) = (u8)eax;
    }
    index = B(work + 2);
    B(work + 2) = (u8)(index + 2);
    cm_fn_007e52c0(work + 0x34, work + 2, work + 0x1c);
    index = D(work + 0x28);
    entry = D(0x00ae23bc);
    entry += index * 110u;
    field = (u32)(s32)(s8)B(work + 0x0d);
    cm_fn_007ff270(0x00b74450, 0, field, 0);
    field = D(entry + 4);
    field = D(field + 0x33);
    cm_fn_007ff270(0x00b74450, 1, field, 0);
    field = D(entry + 8);
    field = D(field + 0x33);
    cm_fn_007ff270(0x00b74450, 2, field, 0);
    field = D(entry + 12);
    field = D(field + 0x33);
    cm_fn_007ff270(0x00b74450, 3, field, 0);
    field = D(entry);
    cm_fn_007ff270(0x00b74450, 4, field, 0);
    field = (u32)(s32)(s8)B(work + 0x0b);
    cm_fn_007ff270(0x00b74450, 5, field, 0);
    field = (u32)(s32)(s16)W(work + 0x10);
    cm_fn_007ff270(0x00b74450, 6, field, 0);
    cm_fn_007ff270(0x00b74450, 8, fifth & 0xffffu, 0);
    cm_fn_007ff270(0x00b74450, 9, sixth & 0xffffu, 0);
    for (i = 0; i < 6; ++i) {
        static const u8 offsets[6] = {4, 5, 6, 7, 0x0f, 0x0e};
        field = (u32)(s32)(s8)B(work + offsets[i]);
        cm_fn_007ff270(0x00b74450, i + 10, field, 0);
    }
    field = D(work + 0x18);
    cm_fn_007ff270(0x00b74450, 16, field, 0);
    cm_fn_007ff270(0x00b74450, 17, arg2, 0);
    field = (u32)(s32)(s8)B(work + 0x0c);
    cm_fn_007ff270(0x00b74450, 18, field, 0);
    field = (u32)(s32)(s8)B(work + 3);
    cm_fn_007ff270(0x00b74450, 19, field, 0);
    field = D(work + 0x14);
    eax = (u32)cm_fn_007ff270(0x00b74450, 20, field, 0);
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
