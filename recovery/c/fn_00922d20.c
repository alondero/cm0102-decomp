#include "cmre.h"
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0090cba0(u32, u32);
extern u32 CM_CDECL cm_fn_00946fd8(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_STDCALL cm_fn_00922d20(u32 stream, u32 output)
{
    u8 scratch[0xae0];
    u32 local = (u32)scratch;
    u32 result, text, word, current, next, index;
    if (stream == 0) {
        cm_fn_00944cff(0x00ad4dec, 0, 0, local + 0x48, local + 0x148);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x48, local + 0x148, 0xea);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return result & 0xffffff00u;
    }
    result = cm_fn_0090cba0(stream, local + 0x310);
    if (result == 0)
        return result & 0xffffff00u;
    result = cm_fn_00946fd8(local + 0x310, 0x00ad4e2c,
        local + 0x248, local + 0x44,
        local, local + 4, local + 8, local + 0xc,
        local + 0x10, local + 0x14, local + 0x18, local + 0x1c,
        local + 0x20, local + 0x24, local + 0x28, local + 0x2c,
        local + 0x30, local + 0x34, local + 0x38, local + 0x3c, local + 0x40);
    if (result != 0x13)
        return result & 0xffffff00u;
    word = W(local + 0x44);
    current = B(local);
    W(output) = (u16)word;
    /* Each next byte is loaded before the preceding output store. */
    for (index = 1; index != 17; ++index) {
        next = B(local + index * 4u);
        B(output + index + 1) = (u8)current;
        current = next;
    }
    B(output + 0x12) = (u8)current;
    return (output & 0xffffff00u) | 1u;
}
