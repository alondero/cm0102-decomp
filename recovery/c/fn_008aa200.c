#include "cmre.h"
/* Preferred-image VA008aa200, base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS5/RET20/full boolean; scratch200, diagnostics L100 then L0.
   BL selects rows, while the complete cached index is forwarded to helpers. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_STDCALL cm_fn_0085add0(u32,u32);
extern u8 CM_STDCALL cm_fn_0085a760(u32,u32);
extern u32 CM_THISCALL cm_fn_008968f0(u32,u32,u32,u32);
extern u16 CM_THISCALL cm_fn_005b2ae0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_005b7810(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_008aa200(u32 self,volatile u32 index_arg,
    volatile u32 output_arg,volatile u32 capacity_arg,
    volatile u32 option_arg,volatile u32 mode_arg)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,index,output,mode,row,context,base,person,club;
    u32 number,third,input,capacity,flags,version;
    s32 value;
    index=index_arg;
    if ((s8)index<0 || (s8)index>19) goto diagnostic;
    output=output_arg;
    if (!output) goto diagnostic;
    if ((s32)capacity_arg<10) goto diagnostic;
    mode=mode_arg;
    B(output)=0;
    if (mode && !D(self+0xde5)) return 0;
    if ((s32)D(self+0x10)>-1) {
        context=(u32)(s32)(s8)B(self+(mode?0xdea:0xde9));
        row=self+context*1573u+(u32)(s32)(s8)index*73u;
        number=(u32)(s32)(s8)index+1;
        if (B(row+0x1c5)!=0xfb) {
            value=(s8)B(row+0x1c4);
            if (value>0) number=(u32)value;
        }
        goto numeric;
    }
    if (D(self+8)) goto ordinal;
    club=D(self+(mode?0xde5:0));
    context=D(0x00dd7ef0);
    (void)context;
    if (!cm_fn_0085add0(club,0)) goto ordinal;
    base=D(self+4);
    if (base) {
        if (mode) {
            context=(u32)(s32)(s8)B(self+0xdea);
            row=base+0x4796+((u32)(s32)(s8)index+context*20u)*446u;
        } else row=self+0x2735+(u32)(s32)(s8)index*446u;
        if (B(row+0x19)==0xfb) return 1;
        person=D(row+0x69);
        if (!person) return 1;
        club=D(self+(mode?0xde5:0));
        context=D(0x00dd7ef0);
        if (!cm_fn_0085a760(person,club)) return 1;
        club=D(self+(mode?0xde5:0));
        context=D(0x00dd7ef0);
        value=(s8)cm_fn_0085a760(person,club);
        number=(u32)(value<0?-value:value);
        output=output_arg;
        goto numeric;
    }
    if (mode) person=D(self+(u32)(s32)(s8)index*4u+0xdf0);
    else {
        third=D(self+8);
        input=D(self);
        context=D(0x00de1d08);
        person=cm_fn_008968f0(context,input,index,third);
    }
    if (person) {
        club=D(self+(mode?0xde5:0));
        context=D(0x00dd7ef0);
        if (cm_fn_0085a760(person,club)) {
            club=D(self+(mode?0xde5:0));
            context=D(0x00dd7ef0);
            value=(s8)cm_fn_0085a760(person,club);
            output=output_arg;
            number=(u32)(value<0?-value:value);
            cm_fn_00944c9f(output,0x0098806c,number);
            goto fallback;
        }
    }
    output=output_arg;
fallback:
    if (B(output)) return 1;
    if (option_arg) return 1;
    if ((s8)index<11) {
        capacity=capacity_arg;
        /* At 8aa45b EAX=capacity. The arg2=0 path at 5b2af2..5b2b22
           writes AL then AX only: its physical upper EAX is still capacity.
           Keep the full push at 8aa474 without inventing register residue. */
        flags=cm_fn_005b2ae0(self+0xe44,index,0);
        flags|=capacity&0xffff0000u;
        cm_fn_005b7810(flags,output,capacity,1);
    } else cm_fn_0066f4e0(output,0x0098ea6c,(u32)(s32)(s8)index-10u);
    return 1;
ordinal:
    number=(u32)(s32)(s8)index+1;
numeric:
    cm_fn_00944c9f(output,0x0098806c,number);
    return 1;
diagnostic:
    cm_fn_00944cff(0x00a8b94c,0,0,l+0x100,l);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x100,l,0x1a74);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}
