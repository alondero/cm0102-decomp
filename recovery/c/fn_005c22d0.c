/* Preferred-image VA005c22d0,
   base00400000; targetSHA2560c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0/full incidental EAX. Original calls remain explicit. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 value);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);
extern u32 CM_STDCALL cm_fn_009462bb(u32 start,u32 stride,u32 count,u32 destructor);
extern unsigned long long CM_THISCALL cm_fn_0048c650(u32 self);

u32 CM_THISCALL cm_fn_005c22d0(u32 self)
{
    volatile struct { u32 prev,handler,state; } frame;
    u32 row,remaining,pointer,table,target,index,limit,result;
    frame.state=0xffffffffu;
    frame.handler=0x00959daeu;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    D(self)=0x0096ae7cu;
    frame.state=2;
    row=self+0x116u;
    remaining=127;
    do {
        pointer=D(row-0x65u);
        if (pointer!=0) {
            pointer=D(row);
            if (pointer!=0) {
                table=D(pointer);
                target=D(table);
                (void)((u32 (CM_THISCALL *)(u32,u32))target)(pointer,1);
            }
        }
        row+=105u;
        --remaining;
    } while (remaining!=0);
    pointer=D(self+0x34d8u);
    if (pointer!=0) (void)cm_fn_009452ca(pointer);
    pointer=D(self+0x34c8u);
    if (pointer!=0) { (void)cm_fn_0068d040(pointer);(void)cm_fn_00944c94(pointer); }
    pointer=D(self+0x34ccu);
    if (pointer!=0) { (void)cm_fn_0068d040(pointer);(void)cm_fn_00944c94(pointer); }
    pointer=D(self+0x34d0u);
    if (pointer!=0) { (void)cm_fn_0068d040(pointer);(void)cm_fn_00944c94(pointer); }
    pointer=D(self+0x34d4u);
    if (pointer!=0) { (void)cm_fn_0068d040(pointer);(void)cm_fn_00944c94(pointer); }
    pointer=D(self+0x34ecu);
    if (pointer!=0) (void)cm_fn_009452ca(pointer);
    pointer=D(self+0x34fau);
    if (pointer!=0) (void)cm_fn_009452ca(pointer);
    pointer=D(self+0x34f6u);
    if (pointer!=0) {
        limit=D(0x00ae2350u);
        index=0;
        if ((s32)limit>0) {
            do {
                table=D(self+0x34f6u);
                pointer=D(table+index*4u);
                if (pointer!=0) { (void)cm_fn_0068d040(pointer);(void)cm_fn_00944c94(pointer); }
                limit=D(0x00ae2350u);
                ++index;
            } while ((s32)index<(s32)limit);
        }
        pointer=D(self+0x34f6u);
        (void)cm_fn_009452ca(pointer);
    }
    pointer=D(self+0x3502u);
    if (pointer!=0) (void)cm_fn_009452ca(pointer);
    *(volatile u8 *)&frame.state=1;
    (void)cm_fn_009462bb(self+0xb1u,105,127,0x005a53c0u);
    frame.state=0xffffffffu;
    result=(u32)cm_fn_0048c650(self);
    CM_FS(u32,0)=frame.prev;
    return result;
}
