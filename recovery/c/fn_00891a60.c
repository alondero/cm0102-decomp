#include "cmre.h"
/* Preferred-image VA00891A60/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Signed phases below 5 include negatives. Native
   68A160->549F70 consumes the year's low WORD; 68A160 subtracts the full
   year but stores only the low WORD, so pushed upper year bits are unobserved.
   All 22 own date tuples and all three allocation/error branches are retained. */
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_CDECL cm_fn_0068a160(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0068a1c0(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521e60(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521eb0(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00891a60(u32 self,u32 phase_argument,u32 count_output,
                              u32 code_output,u32 optional_output)
{
    volatile u8 scratch[0x200];
    u32 l=(u32)scratch,output,index,target,year,name,extension,line,version;
    s8 phase=(s8)phase_argument;
    static const u8 dates[22][3]={
        {6,3,6},{13,3,6},{20,3,6},{27,3,6},
        {4,4,6},{11,4,6},{18,4,6},{7,5,6},
        {19,6,6},{2,7,6},{9,7,6},{16,7,6},
        {23,7,6},{30,7,6},{6,8,6},{13,8,6},
        {20,8,6},{4,9,6},{18,9,6},{25,9,6},{31,9,5},{8,10,6}
    };
    if(phase<5) {
        if(optional_output!=0) D(optional_output)=1;
        index=(u32)(s32)phase+1u;
        if(index<=5) {
            target=D(0x00892198u+index*4u);
            switch(target) {
            case 0x00891a9au: W(code_output)=0x46c; break;
            case 0x00891aa8u: W(code_output)=0x46d; break;
            case 0x00891ab6u: W(code_output)=0x46e; break;
            case 0x00891ac4u: W(code_output)=0x46f; break;
            case 0x00891ad2u: W(code_output)=0x470; break;
            case 0x00891ae0u: W(code_output)=0x471; break;
            }
        }
        W(count_output)=22;
        output=cm_fn_00944e46(0x596);
        if(output==0) {
            name=l+0x100u; extension=l; line=0xb9;
            goto error;
        }
        for(index=0;index<22;++index) {
            year=W(self+0x40u);
            cm_fn_0068a160(output,index,dates[index][0],dates[index][1],0,
                           dates[index][2],1,year,0);
            cm_fn_0068a1c0(output,index,0,0xffffffffu,0xffffffffu,0xffffffffu,0);
        }
        return output;
    }
    if(phase==5) {
        if(optional_output!=0) D(optional_output)=0;
        W(count_output)=1;
        W(code_output)=0x438;
        output=cm_fn_00944e46(0x68);
        if(output==0) {
            name=l; extension=l+0x100u; line=0x12b;
            goto error;
        }
        year=(u32)(s32)(s16)W(self+0x40u);
        cm_fn_00521e60(output,0,9,10,0,0,year);
        year=(u32)(s32)(s16)W(self+0x40u);
        cm_fn_00521eb0(output,0,15,10,0,6,1,year,0);
        W(output+0x18u)=6;
        W(output+0x1au)=3;
        W(output+0x1cu)=6;
        W(output+0xfu)=3;
        W(output+7u)=0xa0;
        W(output+9u)=0;
        W(output+0xbu)=0;
        W(output+0xdu)=0;
        B(output+0x17u)=5;
        W(output+0x1eu)=0;
        B(output+0x20u)=0;
        B(output+0x21u)=2;
        B(output+0x22u)=7;
        D(output+0x5cu)=0;
        D(output+0x60u)=0;
        D(output+0x64u)=0;
        return output;
    }
    if(phase!=6) return 0;
    if(optional_output!=0) D(optional_output)=0;
    W(count_output)=2;
    W(code_output)=0x452;
    output=cm_fn_00944e46(0xd0);
    if(output==0) {
        name=l; extension=l+0x100u; line=0x153;
        goto error;
    }
    year=(u32)(s32)(s16)W(self+0x40u);
    cm_fn_00521e60(output,0,9,10,0,0,year);
    year=(u32)(s32)(s16)W(self+0x40u);
    cm_fn_00521eb0(output,0,15,10,0,6,1,year,0);
    W(output+7u)=10;
    W(output+9u)=0;
    W(output+0xbu)=0;
    W(output+0xdu)=0;
    B(output+0x17u)=5;
    W(output+0x18u)=12;
    W(output+0x1au)=6;
    W(output+0x1cu)=12;
    W(output+0xfu)=3;
    W(output+0x1eu)=0;
    B(output+0x20u)=0;
    B(output+0x21u)=2;
    B(output+0x22u)=7;
    D(output+0x5cu)=0;
    D(output+0x60u)=0;
    D(output+0x64u)=0;
    year=(u32)(s32)(s16)W(self+0x40u);
    cm_fn_00521e60(output,1,23,10,0,0,year);
    year=(u32)(s32)(s16)W(self+0x40u);
    cm_fn_00521eb0(output,1,29,10,0,6,1,year,0);
    W(output+0x82u)=6;
    W(output+0x84u)=6;
    W(output+0x6fu)=20;
    W(output+0x71u)=1;
    W(output+0x73u)=0;
    W(output+0x75u)=0x400;
    B(output+0x7fu)=5;
    W(output+0x80u)=12;
    W(output+0x77u)=3;
    W(output+0x86u)=12;
    B(output+0x88u)=0;
    B(output+0x89u)=2;
    B(output+0x8au)=7;
    D(output+0xc4u)=0;
    D(output+0xc8u)=0;
    D(output+0xccu)=0;
    return output;
error:
    cm_fn_00944cff(0x00a8b5b0u,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,version,name,extension,line);
    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
    D(0x00b67a34u)=0;
    return 0;
}
