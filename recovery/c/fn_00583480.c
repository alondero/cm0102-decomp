#include "cmre.h"

/* Preferred-image VA 0x00583480, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Failed individual allocations do not advance the ordinal. The last
   constructor's SEH state remains 3 through the final table store. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32, u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00583480(u32 self, u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 prev = CM_FS(u32, 0), local = (u32)scratch;
    u32 owner, table, allocated, result, counter = 0, ordinal, value, index, text;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958a28;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 4;
    table = cm_fn_00944e46(16);
    D(self + 4) = table;
    if (table == 0) {
        cm_fn_00944cff(0x009c48c8, 0, 0, local + 0x104, local + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x104, local + 4, 0x60);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        index = 0;
        do {
            allocated = cm_fn_00944cf1(0x2b);
            D(local) = allocated;
            frame.state = index;
            result = 0;
            if (allocated != 0) {
                if (index == 0) counter = 1;
                value = D(0x009cfd6c + index * 4u);
                ordinal = index == 0 ? 0 : counter;
                if (index == 1 || index == 2) ++counter;
                result = cm_fn_0092fe80(allocated, owner, ordinal, value, 8000, 7000,
                                       0xffffffffu, 1, 0, 0, argument, 1);
            }
            table = D(self + 4);
            if (index != 3) frame.state = 0xffffffffu;
            D(table + index * 4u) = result;
            ++index;
        } while (index < 4);
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
