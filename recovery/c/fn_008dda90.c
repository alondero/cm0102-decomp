#include "cmre.h"
/* Full independent source/native review complete; actual canonical gate pending. */

/* Preferred-image VA 008dda90, base 00400000; 466 original body bytes.
   CM0102 3.9.68 SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native boundaries, not replacement implementations. The count helper's
   return is deliberately full EAX: its final MOV AL does not clear high bits.
   No compilation, execution, acceptance or unwind claim accompanies this file. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_008d1130(u32 self, u32 record,
                                    u32 flags, void *output, u32 limit);
extern u32 CM_THISCALL cm_fn_0077bbe0(void *self, u32 kind, u32 flag);
extern u32 CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern u32 CM_THISCALL cm_fn_007861e0(u32 self, void *message, u32 recipient);

u32 CM_THISCALL cm_fn_008dda90(u32 self, u32 record, u32 argument)
{
    volatile struct { u32 prev, handler, state; } seh;
    /* L is ESP immediately after SUB ESP,15Ch. Saved registers put the
       steady ESP at L-10h. Keep all locals in this one uninitialized frame:
       L+0 saved optional pointer; L+4 message; L+E4 recipient DWORDs. */
    union { u32 alignment; u8 bytes[0x15c]; } scratch;
    u32 index, receiver, entry, base, optional, subject, result, value;
    u32 cursor, remaining;
    u8 selector, count;

    seh.prev = CM_FS(u32, 0);
    seh.state = 0xffffffffu;
    seh.handler = 0x009645bbu;
    CM_FS(u32, 0) = (u32)&seh;

    index = *(volatile u32 *)(record + 0x0cu);
    /* LEAs form 11*index, then 55*index, then base+110*index. */
    receiver = CM_GLOBAL(volatile u32, 0x00ae1050);
    entry = CM_GLOBAL(volatile u32, 0x00ae23bc) + index * 110u;
    result = cm_fn_004cf5a0(receiver, entry);
    base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    CM_GLOBAL(volatile u32, 0x00ae1080) = result;
    optional = 0;
    if (result != 0) {
        selector = *(volatile u8 *)(result + 0x35u);
        if ((selector & 0x3fu) != 5u) {
            index = *(volatile u32 *)(result + 4u);
            optional = base + index * 581u;
        }
    }
    *(volatile u32 *)(scratch.bytes + 0) = optional;

    index = *(volatile u32 *)(record + 8u);
    selector = *(volatile u8 *)(record + 0x2cu);
    subject = base + index * 581u;
    result = cm_fn_008d1130(self, record, selector == 0x11u ? 1u : 2u,
                           scratch.bytes + 0xe4, 0x1eu);
    count = (u8)result;
    if (count != 0) {
        (void)cm_fn_0077bbe0(scratch.bytes + 4, 0xfabu, 0);
        optional = *(volatile u32 *)(scratch.bytes + 0);
        seh.state = 0;
        value = optional != 0 ? *(volatile u32 *)optional : 0xffffffffu;
        result = cm_fn_00785790(scratch.bytes + 4, 0, value);
        value = *(volatile u32 *)subject;
        result = cm_fn_00785790(scratch.bytes + 4, 1, value);
        value = *(volatile u32 *)entry;
        result = cm_fn_00785790(scratch.bytes + 4, 2, value);
        value = *(volatile u32 *)(entry + 4u);
        value = *(volatile u32 *)(value + 0x33u);
        result = cm_fn_00785790(scratch.bytes + 4, 3, value);
        value = *(volatile u32 *)(entry + 8u);
        value = *(volatile u32 *)(value + 0x33u);
        result = cm_fn_00785790(scratch.bytes + 4, 4, value);
        value = *(volatile u32 *)(entry + 0x0cu);
        value = *(volatile u32 *)(value + 0x33u);
        result = cm_fn_00785790(scratch.bytes + 4, 5, value);
        value = (u32)(s32)*(volatile s8 *)(record + 0x2cu);
        result = cm_fn_00785790(scratch.bytes + 4, 6, value);
        result = cm_fn_00785790(scratch.bytes + 4, 7, 0);
        result = cm_fn_00785790(scratch.bytes + 4, 8, 0);
        value = *(volatile u32 *)(record + 0x20u);
        result = cm_fn_00785790(scratch.bytes + 4, 9, value);
        value = (u32)(s32)(s16)argument;
        result = cm_fn_00785790(scratch.bytes + 4, 10, value);
        if ((s8)count > 0) {
            cursor = (u32)(scratch.bytes + 0xe4);
            remaining = (u32)(s32)(s8)count;
            do {
                value = *(volatile u32 *)cursor;
                receiver = CM_GLOBAL(volatile u32, 0x00b7431c);
                result = cm_fn_007861e0(receiver, scratch.bytes + 4, value);
                cursor += 4u;
                --remaining;
            } while (remaining != 0);
        }
    }
    receiver = seh.prev;
    CM_FS(u32, 0) = receiver;
    return result;
}
