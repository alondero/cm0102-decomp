#include "cmre.h"

/* Preferred-image VA 00795b90, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD4/full EAX. Keep signed capacity, signed selector with unsigned range
   guard, all three target-table reads and optional formatter argument. */
#define D(a) CM_GLOBAL(volatile u32,(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_STDCALL cm_fn_00795b90(u32 destination,volatile u32 capacity,
                             volatile u32 selector_arg,volatile u32 mode_arg)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,name,extension,line,version,mode,selector,target,format;
    if (destination==0) {
        name=local+0x100;
        extension=local;
        line=0x302;
        goto diagnostic;
    }
    if ((s32)capacity<50) {
        name=local;
        extension=local+0x100;
        line=0x308;
        goto diagnostic;
    }
    mode=(u8)mode_arg;
    selector=(u32)(s32)(s8)selector_arg-1;
    if (mode==2) {
        if (selector>3) {
            format=0x00a69c38;
            goto with_extra;
        }
        target=D(0x00795de4+selector*4);
        (void)target;
        switch (selector) {
        case 0: format=0x00a69c98; break;
        case 1: format=0x00a69c84; break;
        case 2: format=0x00a69c70; break;
        default: format=0x00a69cac; break;
        }
    } else if (mode==0) {
        if (selector>3) format=0x00a69b9c;
        else {
            target=D(0x00795df4+selector*4);
            (void)target;
            switch (selector) {
            case 0: format=0x00a69bf8; break;
            case 1: format=0x00a69bd4; break;
            case 2: format=0x00a69bb4; break;
            default: format=0x00a69c1c; break;
            }
        }
    } else {
        if (selector>3) format=0x00a69ab0;
        else {
            target=D(0x00795e04+selector*4);
            (void)target;
            switch (selector) {
            case 0: format=0x00a69b44; break;
            case 1: format=0x00a69b10; break;
            case 2: format=0x00a69adc; break;
            default: format=0x00a69b74; break;
            }
        }
        goto with_extra;
    }
    cm_fn_0066f4e0(destination,format);
    return destination;
with_extra:
    cm_fn_0066f4e0(destination,format,0x00ad9c64);
    return destination;
diagnostic:
    cm_fn_00944cff(0x00a69a18,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}
