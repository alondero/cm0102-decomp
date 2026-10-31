#include "cmre.h"
/* Preferred-image VA008ab7e0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch200 diagnostics L0/L100. Copy0xf07 DWORDs, then WORD/BYTE;
   keep row-type reloads after copying and the late second formal read. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_008ac100(u32,u8,u8);
extern u32 CM_THISCALL cm_fn_005b2f30(u32,u8,u8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_008ab7e0(u32 self,volatile u32 first_arg,volatile u32 second_arg,volatile u32 mode_arg)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,first=first_arg,second,mode,base,left,right,column,source,destination,i,value,version;
    u8 left_type,right_type;
    if ((s8)first<0 || (s8)first>19) goto diagnostic;
    second=second_arg;
    if ((s8)second<0 || (s8)second>19) goto diagnostic;
    mode=mode_arg;
    if (mode) {
        base=D(self+0xde5);
        if (!base) return 0;
    }
    base=D(self+4);
    if (base) {
        if (mode) {
            column=(u32)(s32)(s8)B(self+0xdea)*20;
            left=base+(column+(u32)(s32)(s8)first)*446+0x4796;
            second=second_arg;
            right=base+(column+(u32)(s32)(s8)second)*446+0x4796;
        } else {
            left=self+(u32)(s32)(s8)first*446+0x2735;
            right=self+(u32)(s32)(s8)second*446+0x2735;
        }
        left_type=B(left+0x19);
        if (left_type==0xff) {
            right_type=B(right+0x19);
            if ((s8)right_type>=0 || right_type==0xfe) {
                second=second_arg; first=first_arg;
                cm_fn_008ac100(self,(u8)first,(u8)second);
                return 1;
            }
            first=first_arg;
        }
        if ((s8)left_type>=0 || left_type==0xfe) {
            if (B(right+0x19)==0xff) {
                second=second_arg;
                cm_fn_008ac100(self,(u8)second,(u8)first);
                return 1;
            }
        }
        if ((s8)left_type<0 && left_type!=0xfe) return 0;
        right_type=B(right+0x19);
        if ((s8)right_type<0 && right_type!=0xfe) return 0;
        source=self+0xe44; destination=self+0x4a63;
        for (i=0;i<0xf07;++i) {
            value=D(source); D(destination)=value;
            source+=4; destination+=4;
        }
        W(destination)=W(source); B(destination+2)=B(source+2);
        D(self+0xc4de)=1;
        right_type=B(right+0x19);
        if (right_type==0xfe) right_type=B(right+0x1a);
        left_type=B(left+0x19);
        if (left_type==0xfe) left_type=B(left+0x1a);
        cm_fn_005b2f30(self+0xe44,left_type,right_type);
    } else {
        if ((s8)first>=11 || (s8)second>=11) return 0;
        source=self+0xe44; destination=self+0x4a63;
        for (i=0;i<0xf07;++i) {
            value=D(source); D(destination)=value;
            source+=4; destination+=4;
        }
        second=second_arg;
        W(destination)=W(source); B(destination+2)=B(source+2);
        D(self+0xc4de)=1;
        cm_fn_005b2f30(self+0xe44,(u8)first,(u8)second);
    }
    D(self+0x4a5f)=1;
    return 1;
diagnostic:
    cm_fn_00944cff(0x00a8b94c,0,0,l+0x100,l);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x100,l,0x1e79);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}
