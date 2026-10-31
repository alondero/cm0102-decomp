#include "cmre.h"

/* Preferred-image VA 00443520, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Shared 214-byte scratch: packed 11-byte record at L0, saved self Lc,
   allocation slot L10 and diagnostics L14/L114. Keep original boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_443520;
typedef u32 (CM_THISCALL *virtual_443520)(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32,u32);
extern void CM_THISCALL cm_fn_0048cc70(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32,u32,s16);
extern u32 CM_THISCALL cm_fn_0058f120(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1d0(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_CDECL cm_fn_00944c94(u32);

u32 CM_THISCALL cm_fn_00443520(u32 self,volatile u32 search)
{
    volatile frame_443520 frame;
    u8 scratch[0x214];
    u32 local=(u32)scratch,owner=self,result=0,count,index=0,table,item;
    u32 target,value,record,allocation,found,node,first,second,version;
    frame.state=0xffffffffu;
    frame.handler=0x00954f6c;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    value=search;
    D(local+0xc)=self;
    if (value==0) {
        cm_fn_00944cff(0x0098b644,0,0,local+0x114,local+0x14);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,
                       local+0x114,local+0x14,0x1d0);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    count=D(0x00adadf8);
    if ((s32)count>0) {
        do {
            table=D(0x00adadfc);
            item=D(table+index*4);
            if (item==0) goto next;
            table=D(item);
            target=D(table+0xc);
            value=((virtual_443520)target)(item);
            if ((u8)value!=0) goto next;
            record=D(item+4);
            value=D(record);
            if (cm_fn_004b0700(value)!=0) goto next;
            record=D(item+4);
            owner=D(0x00ae2c74);
            if ((u32)cm_fn_005c6710(owner,record)!=0) goto next;
            record=D(item+4);
            value=D(record);
            D(local)=value;
            value=W(item+0x40);
            W(local+4)=(u16)value;
            value=search;
            B(local+6)=0;
            W(local+7)=0xffff;
            W(local+9)=0xffff;
            cm_fn_0048cc70(item,value,local+7,local+9,local+6);
            if ((s16)W(local+7)<0) goto next;
            if (result==0) {
                allocation=cm_fn_00944cf1(0xe);
                D(local+0x10)=allocation;
                frame.state=result;
                if (allocation!=0) allocation=cm_fn_0068d020(allocation);
                frame.state=0xffffffffu;
                result=allocation;
            }
            cm_fn_0068d0d0(result,local,0xb);
next:
            count=D(0x00adadf8);
            ++index;
        } while ((s32)index<(s32)count);
        owner=D(local+0xc);
    }
    value=search;
    table=D(owner+8);
    owner=D(owner+4);
    record=D(value);
    record=table+record*16;
    second=D(record+0xc);
    first=D(record+8);
    found=cm_fn_0058f120(owner,first,second);
    if (found!=0) {
        value=D(found+6);
        D(found+0xa)=value;
        node=cm_fn_0068d1d0(found);
        if (node!=0) {
            do {
                if (result==0) {
                    allocation=cm_fn_00944cf1(0xe);
                    D(local+0x10)=allocation;
                    frame.state=1;
                    if (allocation!=0) allocation=cm_fn_0068d020(allocation);
                    frame.state=0xffffffffu;
                    result=allocation;
                }
                cm_fn_0068d0d0(result,node,0xb);
                node=cm_fn_0068d1d0(found);
            } while (node!=0);
        }
        cm_fn_0068d040(found);
        cm_fn_00944c94(found);
    }
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
