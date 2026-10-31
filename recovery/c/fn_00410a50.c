#include "cmre.h"

/* Preferred-image VA 0x00410a50, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_004afd80(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_00410a50(u32 self)
{
    u8 scratch[0x200];
    u8 *name, *extension;
    u32 value, id, base, table, other, line, result;
    char *version;

    W(self + 0x3e) = 2;
    value = cm_fn_00944e46(12);
    D(self + 0xa7) = value;
    if (value == 0) {
        name = scratch + 0x100; extension = scratch;
        line = 0xc0; result = 0xffffffffu; goto error;
    }
    id = D(0x009cf64c);
    base = D(0x00ae23d0);
    value = cm_fn_004afd80(base + id * 107u, 0);
    table = D(self + 0xa7);
    D(table) = value;
    table = D(self + 0xa7);
    if (D(table) == 0) {
        line = 0xc9; goto missing;
    }
    id = D(0x009cf650);
    base = D(0x00ae23d0);
    value = cm_fn_004afd80(base + id * 107u, 0);
    table = D(self + 0xa7);
    D(table + 6) = value;
    table = D(self + 0xa7);
    other = D(table + 6);
    if (other == 0) {
        line = 0xd1; goto missing;
    }
    if (D(table) == other) {
        id = D(0x009cf64c);
        base = D(0x00ae23d0);
        value = cm_fn_004afd80(base + id * 107u, 1);
        table = D(self + 0xa7);
        D(table) = value;
    }
    table = D(self + 0xa7); B(table + 4) = 0;
    table = D(self + 0xa7); B(table + 5) = 0;
    table = D(self + 0xa7); B(table + 0xa) = 0;
    table = D(self + 0xa7); B(table + 0xb) = 0;
    return 1;

missing:
    name = scratch; extension = scratch + 0x100; result = 0;
error:
    cm_fn_00944cff(0x00987be0, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, name, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}
