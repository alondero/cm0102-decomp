#include "cmre.h"
/* Preferred-image VA00636760/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full allocation-or-zero EAX; scratch200 diagnosticsL100/L0.
   Four independent records: retain each ordered field store and signed-WORD
   year reload before every CDECL7 521e60/CDECL9 521eb0 boundary. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00521e60(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521eb0(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00636760(u32 self,volatile u8 selector,
                              volatile u32 first_arg,volatile u32 second_arg,
                              volatile u32 numbered_arg)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,numbered,first,second,r,year,version;
    if(selector!=0xff) return 0;
    numbered=numbered_arg;
    if(numbered!=0) D(numbered)=0;
    first=first_arg;second=second_arg;W(first)=4;W(second)=0;
    r=(u32)cm_fn_00944e46(0x1a0);
    if(r==0){
        cm_fn_00944cff(0x009e9f0c,0,0,l+0x100,l);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x100,l,0x9d);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
        return 0;
    }
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(r,0,1,7,0,1,year);
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521eb0(r,0,0x17,7,0,2,2,year,0);
    W(r+7)=0xa;W(r+9)=0;W(r+0xb)=0;W(r+0xd)=3;
    B(r+0x17)=4;W(r+0x18)=0x10;W(r+0x1a)=8;W(r+0x1c)=0x10;
    W(r+0xf)=0;W(r+0x1e)=0;B(r+0x20)=0;B(r+0x21)=1;B(r+0x22)=0;
    D(r+0x5c)=0;D(r+0x60)=0xffffffffu;D(r+0x64)=0;
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(r,1,0x19,7,0,4,year);
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521eb0(r,1,0x12,9,0,2,2,year,0);
    W(r+0x6f)=0x78;W(r+0x71)=1;W(r+0x73)=0;W(r+0x75)=3;
    B(r+0x7f)=4;W(r+0x80)=8;W(r+0x82)=4;W(r+0x84)=0;
    W(r+0x77)=0;W(r+0x86)=0;B(r+0x88)=0;B(r+0x89)=1;B(r+0x8a)=0;
    D(r+0xc4)=0;D(r+0xc8)=0xffffffffu;D(r+0xcc)=0;
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(r,2,0x14,9,0,4,year);
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521eb0(r,2,0x11,0,1,2,2,year,0);
    W(r+0xd7)=0x82;W(r+0xd9)=2;W(r+0xdb)=0;W(r+0xdd)=3;W(r+0xdf)=0;
    B(r+0xe7)=4;W(r+0xe8)=4;W(r+0xea)=2;W(r+0xec)=0;W(r+0xee)=0;
    B(r+0xf0)=0;B(r+0xf1)=1;B(r+0xf2)=0;D(r+0x12c)=0;
    D(r+0x130)=0xffffffffu;D(r+0x134)=0;
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521e60(r,3,0x13,0,1,2,year);
    year=(u32)(s32)(s16)W(self+0x40);
    cm_fn_00521eb0(r,3,0xe,2,1,2,2,year,0);
    W(r+0x141)=3;W(r+0x145)=3;W(r+0x13f)=0x96;W(r+0x143)=0;
    W(r+0x147)=0;B(r+0x14f)=4;W(r+0x150)=2;W(r+0x152)=1;
    W(r+0x154)=0;W(r+0x156)=0;B(r+0x158)=0;B(r+0x159)=1;
    B(r+0x15a)=0;D(r+0x194)=0;D(r+0x198)=0xffffffffu;D(r+0x19c)=0;
    return r;
}
