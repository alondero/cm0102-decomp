#include "cmre.h"

/* Preferred-image VA 0x00725a70, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_007ffe40(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_00725a70(volatile u32 kind, u32 match, u32 output,
                           s32 length, u32 style)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, format, index, value, name, ext, line, text;
    u32 add_flag = 0;
    u8 status;
    if (match == 0 || output == 0 || length < 100) {
        name = local; ext = local + 0x100; line = 0x1ce4;
        goto error;
    }
    status = (u8)kind;
    B(output) = 0;
    D(style) = 0x1021;
    if (status == 1) format = 0x00a16d8c;
    else if (status == 2) format = 0x00a16d7c;
    else if (status == 3) format = 0x00a16d6c;
    else if (status == 4) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        value = D(match + index * 4u + 0xc5);
        if (value != 0) format = 0x00a16d5c;
        else if (B(match + 0x108) != 0) {
            format = 0x00a16d54;
            add_flag = 1;
        } else format = 0x00a16d48;
    } else if (status == 5) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        value = D(0x00b74c26 + index * 768u);
        value = cm_fn_007ffe40(0x00b74450, value);
        if (value != 0) {
            format = 0x00a16d34;
            add_flag = 1;
        } else format = 0x00a16d24;
    } else {
        name = local + 0x100; ext = local; line = 0x1d14;
        goto error;
    }
    cm_fn_0066f4e0(output, format);
    if (add_flag != 0) {
        value = D(style);
        D(style) = value | 0x40;
    }
    return 1;
error:
    cm_fn_00944cff(0x00a161d4, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
