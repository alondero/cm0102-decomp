/* Preferred-image VA 005219b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005219b0(u32 self, volatile s32 first_arg, volatile s32 second_arg,
                              volatile u32 output_arg, volatile u32 count_arg)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 output, count, first, second, offset, table, row, n, line, version;
    s32 next_first, next_second;
    u16 item;
    output = output_arg;
    count = count_arg;
    if (output == 0 || count == 0) {
        line = 0x158cu;
        name = work;
        extension = work + 0x100;
        goto diagnostic;
    }
    first = (u32)first_arg;
    if ((s32)first < 0) goto invalid;
    second = (u32)second_arg;
    if (second == 0xffffffffu) goto invalid;
    offset = second * 53u;
    table = D(self + 0xadu);
    row = D(table + first * 4u) + offset;
    item = W(row);
    if (item == 0xffffu) {
        next_second = (s16)W(row + 0xau);
        next_first = (s16)W(row + 0xeu);
        cm_fn_005219b0(self, next_first, next_second, output, count);
        output = output_arg;
    } else {
        n = D(count);
        W(output + n * 2u) = item;
        n = D(count);
        D(count) = n + 1u;
    }
    table = D(self + 0xadu);
    row = D(table + first * 4u) + offset;
    item = W(row + 2u);
    if (item == 0xffffu) {
        next_second = (s16)W(row + 0xcu);
        next_first = (s16)W(row + 0x10u);
        cm_fn_005219b0(self, next_first, next_second, output, count);
        return D(count);
    }
    n = D(count);
    W(output + n * 2u) = item;
    n = D(count) + 1u;
    D(count) = n;
    return n;
invalid:
    line = 0x1592u;
    name = work + 0x100;
    extension = work;
diagnostic:
    cm_fn_00944cff(0x009b55acu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return D(count);
}
