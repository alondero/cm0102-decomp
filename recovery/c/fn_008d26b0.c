/* Preferred-image VA008d26b0; base00400000; 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   WIP: native input coverage pending; no acceptance claim.
   All numeric callees bind original code. Sole indexed caller8bbc64 kills
   EAX at8bbc69. EDI OR -1 overwrites the saved incoming EDI before use.
   Sort CDECL4 ignores incidental EDX:EAX on early exit; actual sort kills
   EAX at945361/367 and EDX at945379 before helper/callback use. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00583280(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0040a580(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00412c80(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00424b90(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0043f0e0(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00518500(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00555460(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00579b30(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005a9950(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005c1570(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005dff70(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005eceb0(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005f9690(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0063c210(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00661380(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00666bd0(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007944f0(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007936d0(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007cd830(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007d3d80(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007ec770(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007f6390(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00859400(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00893d80(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_009020a0(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0090b940(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0090f080(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0066e630(u32,u32,u32,u32,u32,u32);

void CM_THISCALL cm_fn_008d26b0(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x204];
    u32 p,value,global,slot,object,remaining,count,base,first,offset,index=0;
    s8 entry;
    frame.state=0xffffffffu;
    frame.handler=0x00964298u;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    p=cm_fn_00944cf1(0x19u);
    frame.state=0u;
    if(p) {
        global=D(0x009cfa10u);
        value=cm_fn_00583280(p,0u,0xffffffffu,global,2u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8acu)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=1u;
    if(p) {
        global=D(0x009cf218u);
        value=cm_fn_0040a580(p,1u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8b0u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=2u;
    if(p) {
        global=D(0x009cf224u);
        value=cm_fn_00412c80(p,2u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8b4u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=3u;
    if(p) {
        global=D(0x009cf244u);
        value=cm_fn_00424b90(p,3u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8b8u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=4u;
    if(p) {
        global=D(0x009cf260u);
        value=cm_fn_0043f0e0(p,4u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8bcu)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=5u;
    if(p) {
        global=D(0x009cf2acu);
        value=cm_fn_00518500(p,5u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8c0u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=6u;
    if(p) {
        global=D(0x009cf2c0u);
        value=cm_fn_00555460(p,6u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8c4u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=7u;
    if(p) {
        global=D(0x009cf2e4u);
        value=cm_fn_00579b30(p,7u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8c8u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=8u;
    if(p) {
        global=D(0x009cf2fcu);
        value=cm_fn_005a9950(p,8u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8ccu)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=9u;
    if(p) {
        global=D(0x009cf300u);
        value=cm_fn_005c1570(p,9u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8d0u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=10u;
    if(p) {
        global=D(0x009cf314u);
        value=cm_fn_005dff70(p,10u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8d4u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=11u;
    if(p) {
        global=D(0x009cf31cu);
        value=cm_fn_005eceb0(p,11u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8d8u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=12u;
    if(p) {
        global=D(0x009cf338u);
        value=cm_fn_005f9690(p,12u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8dcu)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=13u;
    if(p) {
        global=D(0x009cf2dcu);
        value=cm_fn_0063c210(p,13u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8e0u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=14u;
    if(p) {
        global=D(0x009cf360u);
        value=cm_fn_00661380(p,14u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8e4u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=15u;
    if(p) {
        global=D(0x009cf36cu);
        value=cm_fn_00666bd0(p,15u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8e8u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=16u;
    if(p) {
        global=D(0x009cf408u);
        value=cm_fn_007944f0(p,17u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8f0u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=17u;
    if(p) {
        global=D(0x009cf3e4u);
        value=cm_fn_007936d0(p,16u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8ecu)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=18u;
    if(p) {
        global=D(0x009cf430u);
        value=cm_fn_007cd830(p,18u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8f4u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=19u;
    if(p) {
        global=D(0x009cf434u);
        value=cm_fn_007d3d80(p,19u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8f8u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=20u;
    if(p) {
        global=D(0x009cf448u);
        value=cm_fn_007ec770(p,20u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x8fcu)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=21u;
    if(p) {
        global=D(0x009cf45cu);
        value=cm_fn_007f6390(p,21u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x900u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=22u;
    if(p) {
        global=D(0x009cf488u);
        value=cm_fn_00859400(p,22u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x904u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=23u;
    if(p) {
        global=D(0x009cf4a8u);
        value=cm_fn_00893d80(p,23u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x908u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=24u;
    if(p) {
        global=D(0x009cf4dcu);
        value=cm_fn_009020a0(p,24u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x90cu)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=25u;
    if(p) {
        global=D(0x009cf4f8u);
        value=cm_fn_0090b940(p,25u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x910u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=26u;
    if(p) {
        global=D(0x009cf510u);
        value=cm_fn_0090f080(p,26u,global,0xffffffffu,1u,0u);
    } else value=0;
    frame.state=0xffffffffu;
    D(self+0x914u)=value;
    p=cm_fn_00944cf1(0x19u);
    frame.state=27u;
    if(p) {
        global=D(0x009cf484u);
        value=cm_fn_0066e630(p,27u,global,0xffffffffu,1u,0u);
    } else value=0;
    D(self+0x918u)=value;
    frame.state=0xffffffffu;
    D(self+0x8a8u)=0;
    slot=self+0x8acu;remaining=28;
    do {
        object=D(slot);
        if(object) {
            count=(u32)(s32)B(object+8u);
            value=D(self+0x8a8u);
            D(self+0x8a8u)=value+(count<<1);
        }
        slot+=4;--remaining;
    } while(remaining);
    count=D(self+0x8a8u);
    p=cm_fn_00944e46(count<<2);
    D(self+0x8a4u)=p;
    if(!p) {
        cm_fn_00944cff(0x00a8d514u,0,0,(u32)scratch+0x104u,(u32)scratch+4u);
        value=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u,0x009a8120u,value,(u32)scratch+0x104u,(u32)scratch+4u,0x3bdfu,0x00a8f530u);
        cm_fn_0066f4e0(0x00ae28bcu,0x009a8108u);
        cm_fn_0066f4e0(0x00ae26c4u,0x009a7ff8u,0x00ae24d0u);
        cm_fn_005e8290(0x00ae28bcu,0x00ae26c4u,0);
    } else {
        slot=self+0x8acu;remaining=28;
        do {
            object=D(slot);
            if(object) {
                entry=0;
                if(B(object+8u)>0) do {
                    first=D(object+4u);
                    base=D(self+0x8a4u);
                    offset=(u32)(s32)entry*12u;
                    D(base+index*4u)=first+offset;
                    object=D(slot);
                    ++index;
                    first=D(object+4u);
                    ++index;
                    entry=(s8)((u8)entry+1u);
                    value=first+offset+6u;
                    base=D(self+0x8a4u);
                    D(base+index*4u-4u)=value;
                    object=D(slot);
                } while(entry<B(object+8u));
            }
            slot+=4;--remaining;
        } while(remaining);
        count=D(self+0x8a8u);
        base=D(self+0x8a4u);
        cm_fn_00945333(base,count,4u,0x008e5590u);
    }
    value=frame.prev;
    CM_FS(u32,0)=value;
}
