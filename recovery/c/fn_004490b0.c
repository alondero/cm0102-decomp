#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA004490b0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12/full Boolean EAX. Preserve signedBYTE type minus1,
   unsigned<=31 guard, original32-DWORD dispatch table at44950c, and
   output byte0 write BEFORE rejecting an unknown type. All32 original
   target/format pairs independently checked. Error buffers share0x200-byte
   scratch: their filename/extension order swaps between the two errors. */
u32 CM_STDCALL cm_fn_004490b0(volatile u32 record_type, volatile u32 output,
                             volatile u32 size)
{
    u32 scratch[0x200 / 4];
    u32 l = (u32)scratch, destination = output, index, target, format;
    u32 filename, extension, line, version;
    if (destination == 0 || (s32)size < 50) {
        filename = l;
        extension = l + 0x100;
        line = 0x793;
    } else {
        index = (u32)(s32)(s8)record_type - 1u;
        B(destination) = 0;
        if (index <= 31) {
            target = D(0x0044950c + index * 4u);
            switch (target) {
            case 0x004490ec: format = 0x0098b9f4; break;
            case 0x00449108: format = 0x0098b9dc; break;
            case 0x00449124: format = 0x0098b9c8; break;
            case 0x00449140: format = 0x0098b9b0; break;
            case 0x0044915c: format = 0x0098b99c; break;
            case 0x00449178: format = 0x0098b988; break;
            case 0x00449194: format = 0x0098b97c; break;
            case 0x004491b0: format = 0x0098b968; break;
            case 0x004491cc: format = 0x0098b958; break;
            case 0x004491e8: format = 0x0098b940; break;
            case 0x00449204: format = 0x0098b924; break;
            case 0x00449220: format = 0x0098b90c; break;
            case 0x0044923c: format = 0x0098b8f4; break;
            case 0x00449258: format = 0x0098b8d8; break;
            case 0x00449274: format = 0x0098b8bc; break;
            case 0x00449290: format = 0x0098b8ac; break;
            case 0x004492ac: format = 0x0098b894; break;
            case 0x004492c8: format = 0x0098b880; break;
            case 0x004492e4: format = 0x0098b870; break;
            case 0x00449300: format = 0x0098b858; break;
            case 0x0044931c: format = 0x0098b844; break;
            case 0x00449338: format = 0x0098b830; break;
            case 0x00449354: format = 0x0098b814; break;
            case 0x00449370: format = 0x0098b7f8; break;
            case 0x0044938c: format = 0x0098b7e8; break;
            case 0x004493a8: format = 0x0098b7d8; break;
            case 0x004493c4: format = 0x0098b7c4; break;
            case 0x004493e0: format = 0x0098b7a8; break;
            case 0x004493fc: format = 0x0098b788; break;
            case 0x00449418: format = 0x0098b770; break;
            case 0x00449434: format = 0x0098b758; break;
            case 0x00449450: format = 0x0098b740; break;
            default:
                /* The pinned read-only table has exactly these32 targets. */
                __builtin_unreachable();
            }
            cm_fn_0066f4e0(destination, format);
            return 1;
        }
        filename = l + 0x100;
        extension = l;
        line = 0x87f;
    }
    cm_fn_00944cff(0x0098b6a0, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
