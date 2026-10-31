/* Preferred-image VA 0x0094df1d, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
typedef u32 (CM_STDCALL *convert_fn)(u32, u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_009519fe(u32 value, u32 flag);

u32 CM_CDECL cm_fn_0094df1d(void)
{
    u32 cursor = D(0x00de2fbcu), item = D(cursor);
    convert_fn convert;
    if (!item) return 0;
    convert = (convert_fn)D(0x009670d8u);
    do {
        u32 length = convert(1, 0, item, 0xffffffffu, 0, 0, 0, 0);
        u32 output;
        if (!length) return 0xffffffffu;
        output = (u32)cm_fn_00944e46(length);
        if (!output) return 0xffffffffu;
        item = D(cursor);
        if (!convert(1, 0, item, 0xffffffffu, output, length, 0, 0))
            return 0xffffffffu;
        cm_fn_009519fe(output, 0);
        item = D(cursor + 4u);
        cursor += 4u;
    } while (item);
    return 0;
}
