#include "cmre.h"
/* Preferred-image VA004c0f00, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/fullEAX; scratch200 diagnosticsL0/L100. Three104-byte rows.
   The third row has no00521f10 call; retain untouched allocation bytes. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00521e60(u32,s16,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521eb0(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521f10(u32,s16,s16,s16,u8,u8,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_004c0f00(u32 self,volatile u8 selector,volatile u32 first_out,volatile u32 second_out,volatile u32 optional_out)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,first,second,optional,buffer,year,version;
    if (selector!=0xff) return 0;
    optional=optional_out;
    if (optional) D(optional)=0;
    first=first_out; second=second_out;
    W(first)=3; W(second)=0;
    buffer=cm_fn_00944e46(0x138);
    if (!buffer) {
        cm_fn_00944cff(0x009a7d30,0,0,l+0x100,l);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x100,l,0x97);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(buffer,0,1,7,0,5,year);
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521eb0(buffer,0,28,8,0,3,2,year,0);
    cm_fn_00521f10(buffer,0,0,-1,0xff,0xff,0);
    W(buffer+7)=10; W(buffer+9)=0; W(buffer+0xb)=0; W(buffer+0xd)=3;
    B(buffer+0x17)=8; W(buffer+0x18)=8; W(buffer+0x1a)=4;
    W(buffer+0x1c)=8; W(buffer+0xf)=0; W(buffer+0x1e)=0;
    B(buffer+0x20)=0; B(buffer+0x21)=0; B(buffer+0x22)=0;
    D(buffer+0x5c)=0; D(buffer+0x60)=0xffffffff; D(buffer+0x64)=0;
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(buffer,1,29,8,0,4,year);
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521eb0(buffer,1,1,9,0,6,2,year,0);
    cm_fn_00521f10(buffer,1,0,-1,0xff,0xff,0);
    W(buffer+0x6f)=130; W(buffer+0x71)=1; W(buffer+0x73)=0;
    W(buffer+0x75)=3; B(buffer+0x7f)=8; W(buffer+0x80)=4;
    W(buffer+0x82)=2; W(buffer+0x84)=0; W(buffer+0x77)=0;
    W(buffer+0x86)=0; B(buffer+0x88)=0; B(buffer+0x89)=0; B(buffer+0x8a)=0;
    D(buffer+0xc4)=0; D(buffer+0xc8)=0xffffffff; D(buffer+0xcc)=0;
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(buffer,2,2,9,0,0,year);
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521eb0(buffer,2,4,9,0,2,2,year,0);
    W(buffer+0xd9)=2; W(buffer+0xe8)=2; W(buffer+0xd7)=150;
    W(buffer+0xdb)=0; W(buffer+0xdd)=3; B(buffer+0xe7)=8;
    W(buffer+0xea)=1; W(buffer+0xec)=0; W(buffer+0xdf)=0; W(buffer+0xee)=0;
    B(buffer+0xf0)=0; B(buffer+0xf1)=0; B(buffer+0xf2)=0;
    D(buffer+0x12c)=0; D(buffer+0x130)=0xffffffff; D(buffer+0x134)=0;
    return buffer;
}
