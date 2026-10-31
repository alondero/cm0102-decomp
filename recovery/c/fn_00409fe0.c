#include "cmre.h"

/* Preferred-image VA 00409fe0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Signed WORD row loop, stride 59, fixed 25-entry allocation/shuffle and
   setter order. Diagnostic buffers share the original 200-byte scratch. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_004c08f0(u32,u32);
extern void CM_THISCALL cm_fn_004a2540(u32,u32,u8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00409fe0(u32 self)
{
    u8 scratch[0x200];
    static const u8 positions[25]={1,2,2,2,4,6,6,8,8,8,11,11,11,
                                  14,14,14,17,17,17,20,20,20,23,23,23};
    u32 local=(u32)scratch,allocation,source,value,owner,result,index,line,name,extension;
    if (D(self+8)==0) {
        cm_fn_00944cff(0x009874ec,0,0,local+0x100,local);
        name=local+0x100;
        extension=local;
        line=0x79e;
    } else {
        allocation=(u32)cm_fn_00944e46(0x64);
        if (allocation!=0) {
            index=0;
            if ((s16)W(self+0x3e)>0) {
                do {
                    u32 offset=(u32)(s32)(s16)index;
                    source=D(self+0xb1);
                    index+=1;
                    value=D(source+offset*59);
                    D(allocation+offset*4)=value;
                } while ((s16)index<(s16)W(self+0x3e));
            }
            cm_fn_004c08f0(allocation,25);
            for (index=0;index<25;++index) {
                value=D(allocation+index*4);
                owner=D(self+8);
                cm_fn_004a2540(owner,value,positions[index]);
            }
            return (u32)cm_fn_009452ca(allocation);
        }
        cm_fn_00944cff(0x009874ec,0,0,local,local+0x100);
        name=local;
        extension=local+0x100;
        line=0x7a6;
    }
    value=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,name,extension,line);
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return result;
}
