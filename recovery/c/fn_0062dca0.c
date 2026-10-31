#include "cmre.h"
/* Preferred-image VA0062dca0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full incidental EAX. SEH95b13b, scratch15c:
   BYTEcounterL3/recordL4/pool30DWORDsLe4. Original constructors/setters
   initialize only their actual fields. Keep repeated lookups and full EAX
   across the final partial-AL counter writes. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u8 CM_THISCALL cm_fn_008d1da0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_CDECL cm_fn_007e4de0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786780(u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);

u32 CM_STDCALL cm_fn_0062dca0(volatile u32 source_arg,
    volatile u32 details_arg,volatile u32 value_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x15c];
    u32 l=(u32)scratch,previous,context,source,details,eax,value,club,id,base,index;
    u8 count,counter;
    previous=CM_FS(volatile u32,0);context=D(0x00de1f54);
    frame.state=0xffffffffu;frame.handler=0x0095b13b;frame.prev=previous;
    CM_FS(volatile u32,0)=(u32)&frame;
    source=source_arg;
    if (context) count=cm_fn_008d1da0(context,source,0x5a,l+0xe4,30);
    else count=0;
    value=D(source+0x24);
    if (value && (s8)count<30) {
        index=(u32)(s32)(s8)count;count=(u8)(count+1);
        D(l+0xe4+index*4u)=value;
    }
    cm_fn_0077bbe0(l+4,2000,0);
    value=D(source);frame.state=0;cm_fn_00785790(l+4,0,value);
    value=D(source+4);if(value) value=D(value+0x33);
    cm_fn_00785790(l+4,1,value);
    value=D(source+8);if(value) value=D(value+0x33);
    cm_fn_00785790(l+4,2,value);
    value=D(source+0xc);if(value) value=D(value+0x33);
    cm_fn_00785790(l+4,3,value);
    details=details_arg;value=(u32)(s32)(s8)B(details);
    cm_fn_00785790(l+4,4,value);
    value=(u32)(s32)(s16)W(details+0x20);cm_fn_00785790(l+4,5,value);
    value=W(details+0x22);cm_fn_00785790(l+4,6,value);
    value=(u32)(s32)(s16)W(details+0x28);cm_fn_00785790(l+4,7,value);
    value=(u32)(s32)(s16)W(source+0x10);cm_fn_00785790(l+4,8,value);
    value=W(source+0x12);cm_fn_00785790(l+4,9,value);
    if (B(details+0x1a)!=0xff) {
        value=D(details+0x12);cm_fn_00785790(l+4,10,value);
        value=D(details+0x16);cm_fn_00785790(l+4,11,value);
        value=D(details+9);
    } else {
        cm_fn_00785790(l+4,10,0xffffffffu);
        cm_fn_00785790(l+4,11,0xffffffffu);value=0xffffffffu;
    }
    cm_fn_00785790(l+4,12,value);
    value=value_arg;cm_fn_00785790(l+4,13,value);
    value=D(source+0x39);if(value) value=D(value);else value=0xffffffffu;
    cm_fn_00785790(l+4,14,value);
    cm_fn_00785790(l+4,15,0x420e9);
    B(l+3)=16;eax=cm_fn_007e4de0(details+1,l+4,l+3);
    if ((s16)W(details+0x28)>100) {
        value=D(source+0x61);
        if (value && (s16)W(value+0xd)>8000) {
            context=D(0x00b7431c);eax=cm_fn_00786780(context,l+4);
            goto done;
        }
        eax&=0xffffff00u;B(l+3)=(u8)eax;
        if ((s8)count<=0) goto done;
        do {
            index=(u32)(s32)(s8)(u8)eax;
            club=D(l+0xe4+index*4u);context=D(0x00b7431c);
            eax=cm_fn_007861e0(context,l+4,club);
            counter=B(l+3);counter=(u8)(counter+1);
            eax=(eax&0xffffff00u)|counter;B(l+3)=counter;
        } while ((s8)counter<(s8)count);
        goto done;
    }
    if (!source) goto done;
    eax=D(source+0x39);
    if (!eax) goto done;
    context=D(0x00b7431c);(void)context;
    eax=cm_fn_007875e0(eax);
    if (!eax) goto done;
    eax=D(source+0x39);context=D(0x00b7431c);
    eax=cm_fn_007861e0(context,l+4,eax);
    context=D(0x00ae1050);eax=cm_fn_004cf5a0(context,source);D(0x00ae1080)=eax;
    if (eax) {
        id=D(eax+4);base=D(0x00ae23b4);eax=base+id*581u;
        if (eax) {
            context=D(0x00ae1050);eax=cm_fn_004cf5a0(context,source);D(0x00ae1080)=eax;
            if (eax) {id=D(eax+4);base=D(0x00ae23b4);eax=base+id*581u;}
            club=D(source+0x39);
            if (eax!=club) {
                context=D(0x00ae1050);eax=cm_fn_004cf5a0(context,source);D(0x00ae1080)=eax;
                if (eax) {id=D(eax+4);base=D(0x00ae23b4);eax=base+id*581u;}
                context=D(0x00b7431c);eax=cm_fn_007861e0(context,l+4,eax);
            }
        }
    }
    club=D(source+0x24);
    if (club) {context=D(0x00b7431c);eax=cm_fn_007861e0(context,l+4,club);}
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
