#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_004afd80(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0057a500(u32 self)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 allocation, value, id, base, target, year, name, extension, line, text;
    W(self + 0x3e) = 2;
    allocation = (u32)cm_fn_00944e46(12);
    D(self + 0xa7) = allocation;
    if (allocation == 0) {
        name = local + 0x100;
        extension = local;
        line = 0xc1;
        goto error;
    }
    id = D(0x009cf6e8);
    base = D(0x00ae23d0);
    value = cm_fn_004afd80(base + id * 107u, 0);
    target = D(self + 0xa7);
    D(target) = value;
    target = D(self + 0xa7);
    if (D(target) == 0) {
        name = local;
        extension = local + 0x100;
        line = 0xca;
        goto error;
    }
    year = W(self + 0x40);
    id = D(0x009cf6ec);
    if (year != 1999)
        id = D(0x009cf6f0);
    base = D(0x00ae23d0);
    value = cm_fn_004afd80(base + id * 107u, 0);
    target = D(self + 0xa7);
    D(target + 6) = value;
    target = D(self + 0xa7);
    if (D(target + 6) == 0) {
        name = local;
        extension = local + 0x100;
        line = 0xd5;
        goto error;
    }
    B(target + 4) = 0;
    target = D(self + 0xa7);
    B(target + 5) = 0;
    target = D(self + 0xa7);
    B(target + 10) = 0;
    target = D(self + 0xa7);
    B(target + 11) = 0;
    return 1;
error:
    cm_fn_00944cff(0x009c45f8, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return line == 0xc1 ? 0xffffffffu : 0;
}
