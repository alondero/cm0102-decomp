#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00912db0(u32, u8);

/* Sole direct continuation calls 00912ad0, which calls 00912db0;
   its first EAX use is a full selector load. */
void CM_FASTCALL cm_fn_00912bd0(u32 self)
{
    u8 scratch[0x220];
    u32 local = (u32)scratch;
    u32 i, first, second, a, b, value, text, current, total;
    u32 selected, counter_address, source, destination, offset;
    u8 scan, count, index, pass;
    for (i = 0; i != 10; ++i)
        B(local + 0x10 + i) = 0;
    for (i = 0; i != 10; ++i)
        B(local + 4 + i) = (u8)i;
    B(local + 1) = 0;
    for (i = 20; i != 0; --i) {
        first = (u32)(s32)(s8)cm_fn_0090cfc0(10);
        second = (u32)(s32)(s8)cm_fn_0090cfc0(10);
        b = B(local + 4 + second);
        a = B(local + 4 + first);
        B(local + 4 + first) = (u8)b;
        B(local + 4 + second) = (u8)a;
    }
    value = (u32)cm_fn_00944e46(0xa0);
    D(self + 0xf7) = value;
    if (value == 0) {
        cm_fn_00944cff(0x00ad4680, 0, 0, local + 0x120, local + 0x20);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x120, local + 0x20, 0x340);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return;
    }
    B(local + 2) = 0;
    do {
        current = (u32)(s32)(s8)B(local + 1);
        if ((s32)current >= (s32)D(self + 0xf3))
            return;
        scan = 0;
        B(local + 3) = scan;
        do {
            current = (u32)(s32)(s8)B(local + 1);
            total = D(self + 0xf3);
            D(local + 0x1c) = current;
            if ((s32)current >= (s32)total)
                break;
            index = B(local + 4 + (u32)(s32)(s8)scan);
            if ((s8)index < 10) {
                selected = (u32)(s32)(s8)index;
                count = B(local + 0x10 + selected);
                counter_address = local + 0x10 + selected;
                value = cm_fn_00912db0(self, (u8)(index - 1));
                if ((s16)(s8)count < (s16)value) {
                    source = D(self + 0xef);
                    offset = (u32)(s32)(s8)count + selected * 4;
                    current = D(local + 0x1c);
                    destination = D(self + 0xf7);
                    value = D(source + current * 4);
                    D(destination + offset * 4) = value;
                    scan = B(local + 1);
                    scan = (u8)(scan + 1);
                    count = (u8)(count + 1);
                    B(local + 1) = scan;
                    B(counter_address) = count;
                }
            }
            scan = B(local + 3);
            scan = (u8)(scan + 1);
            B(local + 3) = scan;
        } while ((s8)scan < 10);
        pass = B(local + 2);
        pass = (u8)(pass + 1);
        B(local + 2) = pass;
    } while ((s8)pass < 4);
}
