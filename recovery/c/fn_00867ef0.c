/* Preferred-image VA 00867ef0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00867ef0(u32 self, u32 destination, s32 size)
{
    u8 work[0x200];
    u32 index, target, format, text;
    if (size < 20) {
        cm_fn_00944cff(0x00a8360cu, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x213fu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    index = (u32)B(self + 0x4fu) >> 4;
    format = 0x00a839c0u;
    if (index <= 6u) {
        target = D(0x00868000u + index * 4u);
        switch (target) {
        case 0x00867f81u: format = 0x00a83a04u; break;
        case 0x00867f90u: format = 0x00a161ccu; break;
        case 0x00867f97u: format = 0x00a839f8u; break;
        case 0x00867fa6u: format = 0x00a839ecu; break;
        case 0x00867fb5u: format = 0x00a839e4u; break;
        case 0x00867fbcu: format = 0x00a839d8u; break;
        case 0x00867fcbu: format = 0x00a839ccu; break;
        default: __builtin_unreachable();
        }
    }
    cm_fn_0066f4e0(destination, format);
    return 1;
}
