#include "cmre.h"

/* Preferred-image VA 0042f0a0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep every table/owner reload and its order. The final loop loads a DWORD
   from its BYTE flag slot; 4a2540 consumes only the low BYTE at 4a25d0.
   No initialization of the other three bytes is present in the original. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_004a2540(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0042f0a0(u32 self)
{
    u8 scratch[0x204];
    u32 local = (u32)scratch, owner, table, value, flag, result, version;
    u32 row, offset, remaining;
    u8 counter;
    owner = D(self + 8);
    if (owner == 0) {
        cm_fn_00944cff(0x0098aba4, 0, 0, local + 0x104, local + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      local + 0x104, local + 4, 0x4a2);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    table = D(self + 0xee);
    cm_fn_00945333(table, 0x38, 4, 0x004b07d0);
    for (row = 0; row < 16; ++row) {
        table = D(self + 0xee);
        if (row % 3 == 2) {
            value = D(table + row * 4);
            owner = D(self + 8);
        } else {
            owner = D(self + 8);
            value = D(table + row * 4);
        }
        if (row == 0) flag = 1;
        else if (row == 1) flag = 2;
        else if (row < 4) flag = 3;
        else if (row < 6) flag = 5;
        else if (row < 8) flag = 7;
        else flag = 9;
        cm_fn_004a2540(owner, value, flag);
    }
    counter = 0x10;
    offset = 0x40;
    B(local) = counter;
    remaining = 0x28;
    do {
        table = D(self + 0xee);
        flag = D(local);
        value = D(table + offset);
        owner = D(self + 8);
        cm_fn_004a2540(owner, value, flag);
        result = (u32)(s32)(s8)B(local);
        counter = (u8)(counter + 1);
        result += 8;
        if ((u32)(s32)(s8)counter == result) B(local) = counter;
        offset += 4;
        --remaining;
    } while (remaining != 0);
    return result;
}
