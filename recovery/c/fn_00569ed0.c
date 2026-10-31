/* Preferred-image VA 0x00569ed0 (image base 0x00400000). Thiscall: ECX is
   pushed on entry and then copied to EBX, so the first entry read is `this`.
   Plain `ret`, so no stack argument. The listing's `push ecx` slot is reused
   as a loop counter ([esp+0x10]); the counter is kept as a local here, and
   the ECX restored by `pop ecx` at exit is not compared by the gate. */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern u32 CM_THISCALL cm_fn_00456770(short *param_1, u32 param_2, u32 param_3);
extern u32 cm_fn_00547810(int param_1,int param_2);
extern void cm_fn_00547c10(int param_1,int param_2);
extern u32 cm_fn_00547790(int param_1,int param_2);
extern void cm_fn_00547bd0(int param_1,int param_2);
extern void CM_CDECL cm_fn_00547af0(u32 object, u32 value, u32 selector);
extern void CM_CDECL cm_fn_00547940(u32 object, u32 value, u32 selector);
extern void CM_STDCALL cm_fn_0056a3b0(volatile u8 *object, volatile u8 *other, u32 third, u32 fourth, u32 fifth);
extern void CM_THISCALL cm_fn_008c16b0(u32 self, u32 *param_1, u8 param_2, u32 param_3);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);

void CM_THISCALL cm_fn_00569ed0(u32 self)
{
    s32 counter;
    u32 p;
    u32 receiver;
    u32 elem;
    u32 record;
    u32 slot;
    u32 base;
    u32 v;
    s32 idx;
    s32 g;
    u32 third;
    u8 flag;

    p = *(volatile u32 *)self;
    if (p == 0) {
        return;
    }
    v = *(volatile u32 *)(p + 2u);
    counter = 0;
    *(volatile u32 *)(p + 0xau) = v;

    if (*(volatile s16 *)(*(volatile u32 *)self) <= 0) {
        return;
    }

    for (;;) {
        receiver = cm_fn_0068d1b0(*(volatile u32 *)self);
        if (receiver == 0) {
            return;                       /* je 0x0056a021: leaves the loop and the routine */
        }

        {
            u32 edx_value = CM_GLOBAL(volatile u32, 0x00ae2c94u);
            u32 eax_value = CM_GLOBAL(volatile u32, 0x00ae2c90u);
            if (cm_fn_00456770((short *)(receiver + 8u), eax_value, edx_value) == 0) {
                goto next;
            }
        }

        idx = *(volatile s32 *)receiver;
        if (idx <= -1) {
            goto next;
        }
        g = CM_GLOBAL(volatile s32, 0x00ae2364u);
        if (idx >= (s32)((u32)g - 1u)) {
            goto next;
        }

        base = CM_GLOBAL(volatile u32, 0x00ae23bcu);
        elem = base + (u32)idx * 110u;
        record = *(volatile u32 *)(elem + 0x39u);
        if (record == 0) {
            goto after;
        }
        if (*(volatile u32 *)record != *(volatile u32 *)(receiver + 4u)) {
            goto after;
        }

        flag = *(volatile u8 *)(receiver + 0x10u);
        if (flag != 0) {
            third = 1u;
        } else {
            u32 receiver_object = CM_GLOBAL(volatile u32, 0x00de1f54u);
            cm_fn_008c16b0(receiver_object, (u32 *)elem, 0, 0xffffffffu);

            slot = *(volatile u32 *)(record + 0xcfu);
            if (slot != 0) {
                if (cm_fn_00547810((int)elem, (int)slot) != 0) {
                    slot = *(volatile u32 *)(record + 0xcfu);
                    cm_fn_00547c10((int)elem, (int)slot);
                }
                if (cm_fn_00547790((int)elem, (int)record) != 0) {
                    cm_fn_00547bd0((int)record, (int)elem);
                }
                slot = *(volatile u32 *)(record + 0xcfu);
                cm_fn_00547af0(elem, slot, 1u);
            }
            cm_fn_00547940(record, elem, 1u);
            third = 2u;
        }
        /* mov ecx, ebx: 0x0056a3b0 does not read ECX. */
        cm_fn_0056a3b0((volatile u8 *)elem, (volatile u8 *)record, third, 0u, 0u);

    after:
        {
            u32 field4 = *(volatile u32 *)(self + 4u);
            s32 idx_now = *(volatile s32 *)receiver;
            u32 this_now;
            *(volatile u8 *)(field4 + (u32)idx_now) = 0;
            this_now = *(volatile u32 *)self;
            (void)cm_fn_0068d1f0((u8 *)this_now, (const u8 *)receiver);
        }

    next:
        counter = counter + 1;
        if (counter < (s32)*(volatile s16 *)(*(volatile u32 *)self)) {
            continue;
        }
        break;
    }
}
