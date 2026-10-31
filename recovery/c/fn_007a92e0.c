#include "cmre.h"
/* Preferred-image VA007a92e0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full incidental EAX; SEH95f22b/scratch158 recordL0,
   pool30DWORDsLe0. Preserve late BYTE argument reads and actual EAX
   through partial AL writes, including the saved FS predecessor on null input.
   Original constructors, setters and delivery routines remain boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008d1da0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786780(u32,u32);
extern u32 CM_THISCALL cm_fn_007863f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007862d0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_CDECL cm_fn_0058a980(u32,u32,u32);

u32 CM_STDCALL cm_fn_007a92e0(volatile u32 source_arg,
    volatile u8 first_arg,volatile u8 second_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x158];
    u32 l=(u32)scratch,eax,source,context,value,entity,first,other,club,pointer;
    u32 id,base,remaining;
    u8 type;
    eax=CM_FS(volatile u32,0);frame.state=0xffffffffu;
    frame.handler=0x0095f22b;frame.prev=eax;CM_FS(volatile u32,0)=(u32)&frame;
    source=source_arg;
    if (!source) goto done;
    eax=D(source+0x61);
    if (eax) {
        eax=(eax&0xffffff00u)|B(source+0x3d);
        if ((u8)eax==8 || (u8)eax==5) goto done;
    }
    eax=D(source+0x39);
    if (!eax) goto done;
    context=D(0x00b7431c);(void)context;
    eax=cm_fn_007875e0(eax);
    if (!eax) goto done;
    cm_fn_0077bbe0(l,0xbba,0);
    value=D(source);frame.state=0;cm_fn_00785790(l,0,value);
    pointer=D(source+4);value=D(pointer+0x33);cm_fn_00785790(l,1,value);
    pointer=D(source+8);value=D(pointer+0x33);cm_fn_00785790(l,2,value);
    pointer=D(source+0xc);value=D(pointer+0x33);cm_fn_00785790(l,3,value);
    value=(u32)(s32)(s8)first_arg;cm_fn_00785790(l,4,value);
    value=(u32)(s32)(s8)second_arg;cm_fn_00785790(l,5,value);
    pointer=D(source+0x39);value=pointer?D(pointer):0xffffffffu;
    cm_fn_00785790(l,6,value);
    if (B(source+0x22)) {pointer=D(source+0x1a);value=D(pointer);}
    else value=0xffffffffu;
    cm_fn_00785790(l,7,value);
    type=B(source+0x3d);if(type==15) type=8;else if(type==13) type=6;
    cm_fn_00785790(l,8,(u32)(s32)(s8)type);
    value=B(source+0x22);cm_fn_00785790(l,9,value);
    value=(u32)(s32)(s8)B(source+0x28);cm_fn_00785790(l,10,value);
    pointer=D(source+0x61);value=0;
    if(pointer) {pointer=D(source+0x69);if(pointer) value=1;}
    cm_fn_00785790(l,11,value);
    value=(u32)(s32)(s8)B(source+0x18);cm_fn_00785790(l,12,value);
    value=D(source+0x69)!=0;cm_fn_00785790(l,13,value);
    value=D(source+0x61)!=0;eax=cm_fn_00785790(l,14,value);
    context=D(0x00de1f54);
    if (context) {
        eax=D(source+0x61);
        if (eax) {eax=cm_fn_008d1da0(context,source,10,l+0xe0,30);goto recipients;}
    }
    eax&=0xffffff00u;
recipients:
    entity=D(source+0x1a);
    if (entity && (s8)B(entity+0x85)>8) {
        first=D(source+0x61);
        if (first && (s16)W(first+0xd)>8500) goto all;
        other=D(source+0x69);
        if (other && (s16)W(other+0xc)>8750 && B(source+0x3d)==5) goto all;
    }
    first=D(source+0x61);
    if (first && (s16)W(first+0xd)>7500) goto entity_delivery;
    other=D(source+0x69);
    if (other && (s16)W(other+0xa)>7250 && B(source+0x3d)==5) goto entity_delivery;
    club=D(source+0x39);
    if (club) {
        value=D(club+0x57);
        if (value && B(source+0x3d)==5) {
            context=D(0x00b7431c);eax=cm_fn_007862d0(context,l,value);
            goto delivered;
        }
    }
    if (!first) {
        context=D(0x00b7431c);eax=cm_fn_007861e0(context,l,club);
        goto delivered;
    }
    if ((s8)(u8)eax>0) {
        remaining=(u32)(s32)(s8)(u8)eax;pointer=l+0xe0;
        do {
            value=D(pointer);context=D(0x00b7431c);
            eax=cm_fn_007861e0(context,l,value);
            pointer+=4;--remaining;
        } while (remaining);
    }
    goto delivered;
all:
    context=D(0x00b7431c);eax=cm_fn_00786780(context,l);
    goto delivered;
entity_delivery:
    context=D(0x00b7431c);eax=cm_fn_007863f0(context,l,entity);
    eax=D(source+0x39);
    if (!eax) goto delivered;
    eax=D(eax+0x53);entity=D(source+0x1a);
    if (eax!=entity) {context=D(0x00b7431c);eax=cm_fn_007863f0(context,l,eax);}
delivered:
    eax=(eax&0xffffff00u)|first_arg;
    if ((u8)eax!=8) goto done;
    eax=D(source+0x61);
    if (!eax || (s16)W(eax+0xb)<=4250) goto done;
    context=D(0x00ae1050);eax=cm_fn_004cf5a0(context,source);D(0x00ae1080)=eax;
    if (!eax) goto done;
    id=D(eax+4);base=D(0x00ae23b4);eax=base+id*581u;
    if (eax) {value=D(source+0x52);eax=cm_fn_0058a980(eax,source,value);}
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
