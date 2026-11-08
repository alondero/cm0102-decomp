/* Preferred-image VA008ccb80, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original numeric callees remain boundaries. S names original ESP after
   four saved registers; its allocation starts at S+16. All related scratch
   addresses retain their adjacency. The linked SEH state is separate. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define R(o) D(S+(o))
typedef struct { void *prev; void *handler; s32 state; } cm_8ccb80_seh;
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern void CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);
extern void CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_008c6100(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00945667(u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_008ccb80(u32 self,u32 club,u32 output)
{
    volatile cm_8ccb80_seh frame;
    u8 scratch[0x254];
    u32 S=(u32)scratch-16u;
    u32 owner,entry,index,limit,table,node,mapped,target,pointer,slot;
    u32 row,ordinal,capacity,bytes,value,count,allocation,source_date;
    u32 kind,target_pc,version;
    u8 type;

    frame.state=-1;frame.handler=(void *)0x00963ec6u;
    frame.prev=CM_FS(void *,0);CM_FS(void *,0)=(void *)&frame;
    R(0x38u)=self;
    cm_fn_0068d020(S+0x24u);
    frame.state=0;
    R(0x18u)=0;R(0x20u)=0;R(0x14u)=0;
    cm_fn_0054a110(S+0x3cu);
    limit=D(self+4u);target=D(club);frame.state=(frame.state&~255)|1;
    R(0x1cu)=0;
    if((s32)limit>0) {
        index=0;
        do {
            table=D(self);entry=D(table+index*4u);
            if(entry) {
                type=B(entry+0x2cu);
                if(type!=2 && type!=3 && type!=0x14 && type!=0x10 && type!=4 &&
                   (type!=0xf || B(entry+0x2eu)<3)) {
                    if(D(entry+8u)==target) {
                        cm_fn_0068d0d0(S+0x24u,S+0x1cu,S+0x1cu,4);
                        index=R(0x1cu);
                    }
                }
            }
            limit=D(self+4u);++index;R(0x1cu)=index;
        }while((s32)index<(s32)limit);
    }
    cm_fn_008c6100(self,club,S+0x24u);
    value=R(0x26u);R(0x2eu)=value;
    node=cm_fn_0068d1b0(S+0x24u);
    owner=self;
    if(!node)goto finish;
outer:
    index=D(node);table=D(owner);entry=D(table+index*4u);
    if(B(entry+0x1fu)==1)goto next_outer;
    index=D(entry+0xcu);table=D(0x00ae23bcu);
    mapped=cm_fn_004cf5a0(D(0x00ae1050u),table+index*110u);
    D(0x00ae1080u)=mapped;
    if(mapped && (B(mapped+0x35u)&0x3fu)!=5u) {
        index=D(mapped+4u);table=D(0x00ae23b4u);
        R(0x1cu)=table+index*581u;
    } else R(0x1cu)=0;
    mapped=R(0x1cu);
    kind=(u32)(s32)(s8)B(entry+0x2cu);
    if(kind<=0x13u) {
        kind=B(0x008cd134u+kind);
        target_pc=D(0x008cd114u+kind*4u);
        switch(target_pc) {
        case 0x008cccf6u: B(S+0x13u)=1;break;
        case 0x008cccfd: B(S+0x13u)=2;break;
        case 0x008ccd04: B(S+0x13u)=3;break;
        case 0x008ccd0b: B(S+0x13u)=4;break;
        case 0x008ccd12: B(S+0x13u)=5;break;
        case 0x008ccd19: B(S+0x13u)=6;break;
        case 0x008ccd20: B(S+0x13u)=0;break;
        default: __builtin_unreachable();
        }
    } else B(S+0x13u)=0;
    limit=D(owner+0x928u);index=0;
    if((s32)limit>0) {
        owner=R(0x38u);value=D(entry+0x20u);table=D(owner+0x920u);
        pointer=table+8u;
        do {
            if(D(pointer)==value) {
                source_date=table+index*12u;
                if(source_date) {
                    cm_fn_00417c10(S+0x3cu,S+0x4cu,source_date);
                    goto date_ready;
                }
                break;
            }
            ++index;pointer+=12u;
        }while((s32)index<(s32)limit);
    }
    cm_fn_00417c10(S+0x3cu,S+0x5cu,entry+0x12u);
date_ready:
    capacity=R(0x20u);ordinal=R(0x14u);
    if(capacity==ordinal) {
        capacity+=20u;R(0x20u)=capacity;
        allocation=cm_fn_00945667(R(0x18u),capacity*27u);R(0x18u)=allocation;
        if(!allocation)goto allocation_error1;
    }
    table=R(0x18u);type=B(S+0x13u);row=table+ordinal*27u;
    B(row+0x19u)=type;
    cm_fn_00417c10(row,S+0x54u,S+0x3cu);
    value=D(entry+0xcu);D(row+8u)=value;
    type=B(entry+0x2du);B(row+0x1au)=type;
    value=D(entry+8u);
    if(D(club)==value) {
        if(mapped)value=D(mapped);else value=0xffffffffu;
        D(row+0xcu)=value;D(row+0x10u)=0;
    } else {D(row+0xcu)=value;D(row+0x10u)=1;}
    value=D(entry+0x1bu);D(row+0x14u)=value;
    if(B(entry+0x2fu)!=0 && D(entry+0x1bu)==0 &&
       ((type=B(entry+0x30u)&15u)==3 || type==4)) B(row+0x18u)=4;
    else if(D(entry+4u))B(row+0x18u)=1;
    else B(row+0x18u)=0;
    pointer=D(entry+4u);++ordinal;R(0x14u)=ordinal;
    if(!pointer)goto next_outer;
    value=D(pointer+2u);D(pointer+10u)=value;
    node=cm_fn_0068d1b0(D(entry+4u));
    if(!node)goto next_outer;
    slot=R(0x14u)*27u;bytes=R(0x20u)*27u;R(0x34u)=bytes;
nested:
    capacity=R(0x20u);count=R(0x14u);
    if(capacity==count) {
        bytes+=0x21cu;R(0x34u)=bytes;
        R(0x20u)=capacity+20u;
        allocation=cm_fn_00945667(R(0x18u),bytes);R(0x18u)=allocation;
        if(!allocation)goto allocation_error2;
    } else allocation=R(0x18u);
    type=B(S+0x13u);row=allocation+slot;B(row+0x19u)=type;
    cm_fn_00417c10(row,S+0x44u,S+0x3cu);
    value=D(node);D(row+8u)=value;value=D(entry+8u);
    if(D(club)==value) {
        mapped=R(0x1cu);if(mapped)value=D(mapped);else value=0xffffffffu;
        D(row+0xcu)=value;D(row+0x10u)=1;
    } else {D(row+0xcu)=value;D(row+0x10u)=0;}
    count=R(0x14u);D(row+0x14u)=0;B(row+0x18u)=1;
    pointer=D(entry+4u);++count;slot+=27u;R(0x14u)=count;
    node=cm_fn_0068d1b0(pointer);
    if(node) {bytes=R(0x34u);goto nested;}
next_outer:
    node=cm_fn_0068d1b0(S+0x24u);
    if(node) {owner=R(0x38u);goto outer;}
finish:
    count=R(0x14u);allocation=R(0x18u);
    cm_fn_00945333(allocation,count,27u,0x008e55f0u);
    frame.state=-1;D(output)=allocation;cm_fn_0068d040(S+0x24u);
    CM_FS(void *,0)=frame.prev;return count;
allocation_error1:
    cm_fn_00944cff(0x00a8d514u,0,0,S+0x164u,S+0x64u);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009a8120u,version,S+0x164u,S+0x64u,0x2b2cu,0x00a8ecfcu);
    goto allocation_error_common;
allocation_error2:
    cm_fn_00944cff(0x00a8d514u,0,0,S+0x64u,S+0x164u);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009a8120u,version,S+0x64u,S+0x164u,0x2b63u,0x00a8ecfcu);
allocation_error_common:
    cm_fn_0066f4e0(0x00ae28bcu,0x009a8108u);
    cm_fn_0066f4e0(0x00ae26c4u,0x009a7ff8u,0x00ae24d0u);
    cm_fn_005e8290(0x00ae28bcu,0x00ae26c4u,0);
    frame.state=-1;D(output)=0;cm_fn_0068d040(S+0x24u);
    CM_FS(void *,0)=frame.prev;return 0;
}
