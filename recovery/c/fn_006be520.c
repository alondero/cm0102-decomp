#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern unsigned long long CM_THISCALL cm_fn_005fd210(u32, u32);
extern u32 CM_THISCALL cm_fn_005fd480(u32, u32);
extern u32 CM_THISCALL cm_fn_006ceb90(u32, u32, u32, u32);
extern u8 CM_THISCALL cm_fn_00898040(u32, u32, s8, u32);

u32 CM_FASTCALL cm_fn_006be520(u32 self)
{
    u8 scratch[0xc];
    u32 local = (u32)scratch;
    u32 i, side, column, record, person, limit, id, context, object;
    u32 mode, argument, pointer, value, use_existing;
    u8 slot, selected, flag;
    for (i = 0; i != 10; ++i)
        D(self + 0x8eda + i * 4) = 0;
    B(local) = 0;
    D(self + 0x1af) = 0xffffffffu;
    D(self + 0x1b3) = 0xffffffffu;
    D(self + 0x1b7) = 0xffffffffu;
    D(self + 0x1bb) = 0xffffffffu;
    D(self + 0x1bf) = 0xffffffffu;
    W(self + 0x1c3) = 0xffff;
    do {
        side = (u32)(s32)(s8)B(local);
        slot = 0;
        B(local + 8) = 0;
        D(local + 4) = self + side * 0xd0;
        do {
            record = D(local + 4);
            person = D(record + 0x904d);
            use_existing = 0;
            if (person != 0) {
                limit = D(0x00ae2364);
                id = D(person);
                if ((s32)id >= (s32)(limit - 0x10) && (s32)id < (s32)limit) {
                    flag = B(person + 0x5f);
                    if (flag != 0) {
                        context = D(0x00b63c98);
                        if ((u32)cm_fn_005fd210(context, person) == 0) {
                            record = D(local + 4);
                            use_existing = 1;
                            if (B(record + 0x904b) != 0) {
                                mode = B(local);
                                context = D(self + 0x4792);
                                object = D(context + (mode == 0 ? 0x1c : 0x20));
                                if (cm_fn_0053b900(object) == 0) {
                                    person = D(record + 0x904d);
                                    context = D(0x00b63c98);
                                    if (cm_fn_005fd480(context, person) == 0)
                                        use_existing = 0;
                                }
                            }
                        }
                    }
                }
            }
            if (use_existing != 0) {
                mode = B(local);
                context = D(self + 0x4792);
                flag = B(context + (mode != 0 ? 0x41 : 0x40));
                object = D(context + (mode != 0 ? 0x20 : 0x1c));
                argument = D(local + 8);
                context = D(0x00de1d08);
                selected = cm_fn_00898040(context, object, (s8)argument, flag == 0);
                column = (u32)(s32)(s8)slot;
                B(self + side * 11 + column + 0x1af) = selected;
                value = (u32)(s32)(s8)selected;
                B(self + value + side * 20 + 0x8eda) = 1;
            } else {
                argument = D(local);
                value = cm_fn_006ceb90(self, argument, 5, self + 0x8eda);
                /* Its first mode read006ceb9b is BYTE;898040's index is BYTE.
                   The uninitialized upper three bytes of each slot stay unknown. */
                if (value == 0) {
                    slot = (u8)(slot - 1);
                    value = self + side * 20 + 0x8eda;
                    D(value) = 0;
                    D(value + 4) = 0;
                    D(value + 8) = 0;
                    D(value + 0xc) = 0;
                    D(value + 0x10) = 0;
                } else {
                    selected = B(value);
                    column = (u32)(s32)(s8)slot;
                    B(self + side * 11 + column + 0x1af) = selected;
                    pointer = (u32)(s32)(s8)selected;
                    value = self + pointer;
                    B(value + side * 20 + 0x8eda) = 1;
                }
            }
            slot = (u8)(slot + 1);
            B(local + 8) = slot;
        } while ((s8)slot < 0xb);
        mode = B(local);
        mode = (u8)(mode + 1);
        value = (value & 0xffffff00u) | mode;
        B(local) = (u8)mode;
    } while ((s8)mode < 2);
    return value;
}
