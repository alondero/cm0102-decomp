#include "cmre.h"

/* Preferred-image VA 00630c50, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep signed capacity/type guards, physical thirteen-way target-table read,
   diagnostic buffer orientation, and full EAX with AL-only error clearing. */
#define D(a) CM_GLOBAL(volatile u32,(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_0066f4d0(void);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_STDCALL cm_fn_00630c50(volatile u32 type_arg,volatile u32 destination_arg,u32 capacity)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,type,destination,target,format,name,extension,line,version,result;
    if ((s32)capacity<20) {
        name=local+0x100;
        extension=local;
        line=0x10d0;
        cm_fn_00944cff(0x009e7f28,0,0,name,extension);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return result&0xffffff00u;
    }
    type=(u32)(s32)(s8)type_arg;
    if (type<=12) {
        target=D(0x00630eac+type*4);
        (void)target;
        switch (type) {
        case 0: format=0x009e89ac; break;
        case 1: format=0x009e89a0; break;
        case 2: format=0x009e8994; break;
        case 3: format=0x009e8988; break;
        case 4: format=0x009e8978; break;
        case 5: format=0x009e8968; break;
        case 6: format=0x009e8954; break;
        case 7: format=0x009e8948; break;
        case 8: format=0x009e893c; break;
        case 9: format=0x009e892c; break;
        case 10: format=0x009e8920; break;
        case 11: format=0x009e8914; break;
        default: format=0x009e8904; break;
        }
        destination=destination_arg;
        cm_fn_0066f4e0(destination,format);
    } else {
        cm_fn_00944cff(0x009e7f28,0,0,local,local+0x100);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local,local+0x100,0x10e7);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
    }
    result=cm_fn_0066f4d0();
    result=(result&0xffffff00u)|(result!=3);
    return (result-1)&2;
}
