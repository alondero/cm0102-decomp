#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0090bde0(u32, u32);
extern u32 CM_CDECL cm_fn_00945b00(u32, u32);
extern u32 CM_CDECL cm_fn_00945bfe(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945a09(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_005aee90(void)
{
    u32 table = D(0x00ae2a5c);
    u8 scratch[0x200];
    u32 name, extension, line, count, text, result, path, stream, bytes, address, i;
    if (table != 0) {
        name = (u32)scratch + 0x100;
        extension = (u32)scratch;
        line = 0xfe4;
        goto early_error;
    }
    count = D(0x00ae235c);
    table = (u32)cm_fn_00944e46(count * 2u);
    D(0x00ae2a5c) = table;
    if (table == 0) {
        name = (u32)scratch;
        extension = (u32)scratch + 0x100;
        line = 0xfec;
        goto early_error;
    }
    path = cm_fn_0090bde0(0x00987da0, 0x009cc248);
    stream = cm_fn_00945b00(path, 0x0098f928);
    if (stream != 0) {
        count = D(0x00ae235c);
        table = D(0x00ae2a5c);
        result = cm_fn_00945bfe(table, 2, count, stream);
        count = D(0x00ae235c);
        if (result != count) {
            cm_fn_00944cff(0x009cc208, 0, 0, (u32)scratch, (u32)scratch + 0x100);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch, (u32)scratch + 0x100, 0xff6);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
        }
        return cm_fn_00945a09(stream);
    }
    count = D(0x00ae235c);
    address = D(0x00ae2a5c);
    bytes = count * 2u;
    for (i = bytes >> 2; i != 0; --i) {
        D(address) = 0;
        address += 4;
    }
    for (i = bytes & 3u; i != 0; --i) {
        B(address) = 0;
        ++address;
    }
    return 0;

early_error:
    cm_fn_00944cff(0x009cc208, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}
