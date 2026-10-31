#include "cmre.h"
/* Preferred-image VA005dc3a0,base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX, scratch200 diagnosticsL100,L0.
   Original date helpers remain boundaries, including their scratch behavior.
   Re-read the signed year at every original site; parity tests compare the
   original signed remainder with zero, including negative odd years. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00521e60(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521eb0(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521f10(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_005dc3a0(u32 self,volatile u8 selector_arg,
    volatile u32 first_arg,volatile u32 second_arg,volatile u32 optional_arg)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,first,second,optional,result,year,kind,version;
    if (selector_arg!=0xff) return 0;
    optional=optional_arg;
    if (optional) D(optional)=0;
    first=first_arg;second=second_arg;
    W(first)=3;W(second)=0;
    result=cm_fn_00944e46(0x138);
    if (!result) {
        cm_fn_00944cff(0x009cd450,0,0,l+0x100,l);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x100,l,0x98);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(result,0,1,6,0,5,year);
    year=(u32)(s32)(s16)W(self+0x40);
    if (year&1) {
        cm_fn_00521eb0(result,0,9,6,0,4,2,year,8);kind=5;
    } else {
        cm_fn_00521eb0(result,0,24,6,0,0,2,year,8);kind=3;
    }
    cm_fn_00521f10(result,0,0,kind,2,3,0);
    cm_fn_00521f10(result,0,1,0xffffffffu,0xffffffffu,0xffffffffu,0);
    W(result+7)=0x78;W(result+9)=0;W(result+0xb)=0;W(result+0xd)=0x101;
    B(result+0x17)=6;W(result+0x18)=4;W(result+0x1a)=2;W(result+0x1c)=4;
    W(result+0xf)=0;W(result+0x1e)=0;
    B(result+0x20)=0;B(result+0x21)=0;B(result+0x22)=0;
    D(result+0x5c)=0;D(result+0x60)=0xffffffffu;D(result+0x64)=0;
    year=(u32)(s32)(s16)W(self+0x40);
    if (year&1) {
        cm_fn_00521e60(result,1,11,6,0,6,year);
        year=(u32)(s32)(s16)W(self+0x40);
        cm_fn_00521eb0(result,1,12,6,0,0,2,year,8);
        cm_fn_00521f10(result,1,0,1,2,3,0);
    } else {
        /* ECX=4 at5dc4e6 and is untouched through PUSH ECX at5dc562. */
        cm_fn_00521e60(result,1,28,6,0,4,year);
        year=(u32)(s32)(s16)W(self+0x40);
        cm_fn_00521eb0(result,1,29,6,0,5,2,year,8);
        cm_fn_00521f10(result,1,0,6,1,3,0);
    }
    cm_fn_00521f10(result,1,1,0xffffffffu,0xffffffffu,0xffffffffu,0);
    W(result+0x6f)=0x82;W(result+0x71)=1;W(result+0x73)=0;W(result+0x75)=0x501;
    B(result+0x7f)=6;W(result+0x80)=4;W(result+0x82)=2;W(result+0x84)=2;
    W(result+0x77)=0;W(result+0x86)=4;
    B(result+0x88)=0;B(result+0x89)=0;B(result+0x8a)=0;
    D(result+0xc4)=0;D(result+0xc8)=0xffffffffu;D(result+0xcc)=0;
    year=(u32)(s32)(s16)W(self+0x40);
    if (year&1) {
        cm_fn_00521e60(result,2,14,6,0,2,year);
        year=(u32)(s32)(s16)W(self+0x40);
        cm_fn_00521eb0(result,2,16,6,0,4,2,year,0x40);
    } else {
        cm_fn_00521e60(result,2,31,6,0,0,year);
        year=(u32)(s32)(s16)W(self+0x40);
        cm_fn_00521eb0(result,2,1,7,0,1,2,year,0x40);
    }
    W(result+0xd9)=2;W(result+0xe8)=2;W(result+0xdd)=1;W(result+0xea)=1;
    W(result+0xd7)=0x96;W(result+0xdb)=0;B(result+0xe7)=6;
    W(result+0xec)=0;W(result+0xdf)=0;W(result+0xee)=0;
    B(result+0xf0)=0;B(result+0xf1)=0;B(result+0xf2)=0;
    D(result+0x12c)=0;D(result+0x130)=0xffffffffu;D(result+0x134)=0;
    return result;
}
