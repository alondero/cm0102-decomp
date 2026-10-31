#include "cmre.h"

/* Preferred-image VA 007651e0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch1ac: group BYTE L0, text pointer L4, counter L8, object Lc,
   formatted rows L1c/Le4. No initialization of the other scratch bytes.
   55f990 forwards colours to 5ee150: WORD loads at 5ee3bb/3e8/400.
   55fba0 colour is read as WORD by 560290 and 402ac0; its EAX and
   55f990's EAX are retained in full, including their partial AX returns. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0066a0c0(u32);
extern u32 CM_THISCALL cm_fn_0066a280(u32,u32,u32,u32);
extern u8 CM_THISCALL cm_fn_0066a350(u32);
extern u8 CM_THISCALL cm_fn_0066a430(u32,u32);
/* 66a360 consumes only the low BYTE of its final stack argument. */
extern u32 CM_THISCALL cm_fn_0066a360(u32,u32,s32,u8);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_THISCALL cm_fn_0055f990(u32,
    u32,u32,u32,u32,u32,u32,u32,u32,u16,u16,u32,u32,u16,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0055fba0(u32,
    u32,u32,u32,u32,u32,u32,u32,u32,u32,u16,u32);

u32 CM_CDECL cm_fn_007651e0(volatile u32 arg1,volatile u32 arg2,volatile u32 arg3)
{
    u8 scratch[0x1ac];
    u32 l=(u32)scratch,text,index,person,row,first,second,ui,previous,flags,value;
    u16 colour13,colour10,colour9;
    u8 group;
    B(l)=0xff;
    text=cm_fn_0066a0c0(0);
    D(l+4)=text;
    cm_fn_0066a280(l+0xc,0,text,0);
    second=arg2;
    first=arg1;
    previous=D(l+4);
    index=0;
    D(l+8)=0;
    for (;;) {
        person=D(0x00b63d60+(u32)(s32)(s8)B(text+index)*72);
        if (person!=0) {
            cm_fn_00944c9f(l+0xe4,0x00991944,person+4);
            if (B(person+0x11c)!=0) {
                group=cm_fn_0066a350(l+0xc);
                value=0;
                if ((s8)group>1) {
                    group=cm_fn_0066a430(l+0xc,person);
                    value=(group!=B(l));
                }
                if (value!=0) {
                    group=cm_fn_0066a430(l+0xc,person);
                    B(l)=group;
                    cm_fn_0066a360(l+0xc,0x00de1f64,2000,B(l));
                    colour13=W(0x00aebdea);
                    colour10=W(0x00aebdec);
                    colour9=W(0x00ae31aa);
                    index=(u32)(s32)(s16)W(0x00b77c7a);
                    ui=D(0x00b74c4c+index*768);
                    value=cm_fn_0055f990(ui,2,0,0,0,0,0,0,0x10,
                        colour9,colour10,1,1,colour13,0x00de1f64,0,0,0,first);
                    colour9=W(0x00ae31aa);
                    index=(u32)(s32)(s16)W(0x00b77c7a);
                    ui=D(0x00b74c4c+index*768);
                    previous=cm_fn_0055fba0(ui,0,0,0,0,0,0,0,0,0x30,colour9,value);
                } else {
                    group=cm_fn_0066a350(l+0xc);
                    if (group==1) previous=first;
                }
                if (arg3!=0) {
                    cm_fn_00944c9f(l+0x1c,0x0098ed14,l+0xe4);
                    value=arg3;
                    flags=(person==value)?0x2002:2;
                    row=l+0x1c;
                } else {
                    flags=2;
                    row=l+0xe4;
                }
                colour13=W(0x00aebdea);
                colour10=W(0x00aebdec);
                colour9=W(0x00ae31aa);
                index=(u32)(s32)(s16)W(0x00b77c7a);
                ui=D(0x00b74c4c+index*768);
                cm_fn_0055f990(ui,flags,0,0,0,0,0,0,0x10,
                    colour9,colour10,1,1,colour13,row,0,second,person,previous);
            }
        }
        index=D(l+8)+1;
        D(l+8)=index;
        if ((s32)index>=35) return index;
        text=D(l+4);
    }
}
