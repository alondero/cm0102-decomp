/* Preferred-image VA 0x00948976, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_STDCALL *module_handle_948976)(u32 name);
u32 CM_CDECL cm_fn_00948976(u32 output)
{
    u32 image, offset, header, low;
    low = *(volatile u32 *)output;
    *(volatile u32 *)output = low & 0u;
    image = ((module_handle_948976)CM_GLOBAL(volatile u32, 0x00967138u))(0);
    if (*(volatile u16 *)image != 0x5a4du) return image;
    offset = *(volatile u32 *)(image + 0x3cu);
    if (!offset) return image;
    header = image + offset;
    low = *(volatile u8 *)(header + 0x1au);
    *(volatile u8 *)output = (u8)low;
    low = *(volatile u8 *)(header + 0x1bu);
    *(volatile u8 *)(output + 1u) = (u8)low;
    return (header & 0xffffff00u) | low;
}
