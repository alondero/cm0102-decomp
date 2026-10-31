/* Preferred-image VA 005af180, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_005af180(volatile u32 argument)
{
    u8 work[0x200];
    u32 table, record, index, address, pass, version, result;
    u16 old, masked, year;
    u8 flag;
    table = D(0x00ae2a5cu);
    if (table == 0) {
        cm_fn_00944cff(0x009cc208u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1032u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    record = argument;
    index = D(record + 0xcu);
    address = table + index * 2u;
    for (pass = 0; ; ++pass) {
        old = W(address);
        masked = old & 0xfffeu;
        if (masked == 0) {
            if (W(record + 0x28u) == 2001)
                W(address) = masked;
            else
                W(address) = old | 1u;
            flag = B(address);
            year = W(record + 0x28u);
            W(address) = (u16)((u16)(year << 1) | (flag & 1u));
        } else {
            if ((old & 1u) != 0 && W(record + 0x28u) == 2001)
                W(address) = masked;
            old = W(address);
            year = W(record + 0x28u);
            if (year > (u16)(old >> 1))
                W(address) = (u16)((u32)year * 2u | (old & 1u));
        }
        if (pass != 0)
            break;
        index = D(record + 0x10u);
        table = D(0x00ae2a5cu);
        address = table + index * 2u;
    }
    return address;
}
