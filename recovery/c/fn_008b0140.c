/* Preferred-image VA 008b0140, image base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees remain explicit. The sole indexed direct caller discards
   EAX. Local scratch bytes are uninitialized except for original stores. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

void CM_THISCALL cm_fn_008b0140(void *self_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xf0];
    u32 self = (u32)self_arg, local = (u32)scratch;
    u32 previous, field, person, scaled, value, receiver, eax;
    s32 outer = 0, bound;
    u8 matches, cursor, slot;

    frame.state = 0xffffffffu;
    frame.handler = 0x0096380bu;
    previous = CM_FS(u32, 0);
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0((void *)(local + 16u), 0x1bu, 0);
    bound = (s32)(s8)B(self + 0x253u) + 11;
    frame.state = 0;
    B(local + 2u) = 0;
    if (bound > 0) {
        do {
            scaled = (u32)outer * 5u;
            matches = 0;
            B(local + 3u) = matches;
            field = self + scaled * 5u + 0x5eu;
            person = D(field);
            if ((s32)person > -1) {
                value = CM_GLOBAL(volatile u32, 0x00ae23b4u);
                value = cm_fn_00600a50(value + person * 581u, 0, 1, 0, 0);
                if (value) {
                    bound = (s32)(s8)B(self + 0x253u) + 11;
                    person = D(field);
                    cursor = B(local + 2u);
                    D(local + 12u) = person;
                    if (outer < bound) {
                        eax = (u32)outer;
                        do {
                            scaled = eax * 5u;
                            field = self + scaled * 5u + 0x5eu;
                            if (D(field) == person) {
                                if ((s8)matches < 3)
                                    B(local + 8u + (u32)(s32)(s8)matches) = cursor;
                                matches = (u8)(matches + 1u);
                                D(field) = 0xffffffffu;
                            }
                            bound = (s32)(s8)B(self + 0x253u) + 11;
                            cursor = (u8)(cursor + 1u);
                            eax = (u32)(s32)(s8)cursor;
                        } while ((s32)eax < bound);
                        B(local + 3u) = matches;
                    }
                    value = D(self + 0xdu);
                    cm_fn_00785790((void *)(local + 16u), 0, value);
                    value = D(self + 0x11u);
                    cm_fn_00785790((void *)(local + 16u), 1, value);
                    value = W(self + 0x244u);
                    cm_fn_00785790((void *)(local + 16u), 2, value);
                    value = W(self + 0x24cu);
                    cm_fn_00785790((void *)(local + 16u), 3, value);
                    outer = (s32)(s8)matches;
                    cm_fn_00785790((void *)(local + 16u), 4, (u32)outer);
                    cm_fn_00785790((void *)(local + 16u), 5, person);
                    if ((s8)matches <= 3) {
                        value = B(local + 3u);
                        slot = 6;
                        if ((s8)value > 0) {
                            u32 indices = local + 8u;
                            D(local + 4u) = (u32)outer;
                            do {
                                eax = (u32)(s32)(s8)B(indices);
                                eax *= 5u;
                                field = self + eax * 5u + 0x4eu;
                                value = D(field);
                                eax = (eax & 0xffffff00u) | slot;
                                slot = (u8)(slot + 1u);
                                eax = (u32)cm_fn_00785790((void *)(local + 16u), eax, value);
                                value = D(field + 4u);
                                eax = (eax & 0xffffff00u) | slot;
                                slot = (u8)(slot + 1u);
                                eax = (u32)cm_fn_00785790((void *)(local + 16u), eax, value);
                                value = D(field + 8u);
                                eax = (eax & 0xffffff00u) | slot;
                                slot = (u8)(slot + 1u);
                                eax = (u32)cm_fn_00785790((void *)(local + 16u), eax, value);
                                value = D(field + 12u);
                                eax = (eax & 0xffffff00u) | slot;
                                slot = (u8)(slot + 1u);
                                cm_fn_00785790((void *)(local + 16u), eax, value);
                                value = D(local + 4u);
                                ++indices;
                                value -= 1u;
                                D(local + 4u) = value;
                            } while (value);
                            person = D(local + 12u);
                        }
                    }
                    value = CM_GLOBAL(volatile u32, 0x00ae23b4u);
                    value += person * 581u;
                    receiver = CM_GLOBAL(volatile u32, 0x00b7431cu);
                    cm_fn_007861e0((void *)receiver, local + 16u, value);
                }
            }
            cursor = B(local + 2u);
            cursor = (u8)(cursor + 1u);
            B(local + 2u) = cursor;
            outer = (s32)(s8)cursor;
            bound = (s32)(s8)B(self + 0x253u) + 11;
        } while (outer < bound);
    }
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
}
