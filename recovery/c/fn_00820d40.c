#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0054a130(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4d0(void);
extern u32 CM_THISCALL cm_fn_005fb810(u32, u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_00820d40(void)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 index, slot, person, count, pointer, selector, id, base, context, result, text;
    index = (u32)(s32)(s16)W(0x00b77c7a);
    slot = 0x00b74c26u + index * 768u;
    if (D(slot) == 0) {
        base = D(0x00ae23bc);
        if (base == 0) {
            cm_fn_00944cff(0x00a801dc, 0, 0, local + 0x100, local);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x39b3);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            person = 0;
        } else {
            count = D(0x00ae2364);
            person = base + count * 110u - 110u;
            index = 0;
            while (B(person + 0x5f) != 0) {
                ++index;
                person -= 110;
                if ((s32)index >= 16) {
                    person = 0;
                    break;
                }
            }
        }
        index = (u32)(s32)(s16)W(0x00b77c7a);
        D(0x00b74c26u + index * 768u) = person;
        pointer = D(person + 4);
        B(pointer) = 0;
        pointer = D(person + 8);
        B(pointer) = 0;
        pointer = D(person + 0xc);
        B(pointer) = 0;
        cm_fn_0054a130(person + 0x10, 1, 0, 0x7ae, 0xffffffffu);
        W(person + 0x18) = 35;
        selector = cm_fn_0066f4d0();
        if (selector <= 12u) {
            /* Retain the original guarded target-table read. Its pinned
               thirteen entries map to the cases below. */
            context = D(0x00820f18u + selector * 4u);
            (void)context;
        }
        switch (selector) {
        case 1: id = D(0x009cf4f8); break;
        case 2: id = D(0x009cf300); break;
        case 3: id = D(0x009cf314); break;
        case 4: id = D(0x009cf360); break;
        case 5: id = D(0x009cf488); break;
        case 6: id = D(0x009cf2c0); break;
        case 7: id = D(0x009cf338); break;
        case 8: id = D(0x009cf408); break;
        case 9: id = D(0x009cf434); break;
        case 10: id = D(0x009cf4a8); break;
        case 11: id = D(0x009cf36c); break;
        case 12: id = D(0x009cf484); break;
        default: id = D(0x009cf2e4); break;
        }
        base = D(0x00ae23a8);
        D(person + 0x1a) = base + id * 290u;
        D(person + 0x24) = 0;
        D(person + 0x39) = 0;
        B(person + 0x5f) = 5;
        context = D(0x00b63c98);
        cm_fn_005fb810(context, person);
    }
    result = cm_fn_007fe600(0x00b74450, 0x00820f50, 0x00821520, 1, 0, 0);
    if (result != 0) {
        cm_fn_007ff270(0x00b74450, 0, 0, 0);
        result = cm_fn_007ff270(0x00b74450, 1, 0, 0);
    }
    return result;
}
