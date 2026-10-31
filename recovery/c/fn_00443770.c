#include "cmre.h"
/* Preferred-image VA00443770, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD1/full EAX; scratch408 allocationL0/listAllocationL4,
   diagnosticsL8/L108/L208/L308. Retain the late input-list reload per node,
   second node-ID read, lazy output-list allocation and failure cleanup.
   68d0d0 has ECX receiver and two stack words/RET8. Its incidental EDX
   return transport is not consumed here; no register default is supplied. */
#define D(a) CM_GLOBAL(volatile u32,(a))
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d0d0(u32,u32,s16);
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
#define DIAG(name,extension,line) do { \
    cm_fn_00944cff(0x0098b644,0,0,(name),(extension)); \
    value=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,(name),(extension),(line)); \
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0); \
} while (0)

u32 CM_STDCALL cm_fn_00443770(volatile u32 input_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x408];
    u32 l=(u32)scratch,input,first,flags,limit,index,cursor,item,id,count,value,list,allocation;
    frame.prev=CM_FS(volatile u32,0);
    frame.state=0xffffffff; frame.handler=0x00954f8e;
    CM_FS(volatile u32,0)=(u32)&frame;
    input=input_arg;
    value=0;
    if (input==0) goto done;
    limit=D(0x00ae2378);
    first=cm_fn_00944cf1(limit*4);
    D(l)=first;
    if (first==0) {
        DIAG(l+0x108,l+8,0x238);
        D(0x00b67a34)=0;
        value=0;
        goto done;
    }
    limit=D(0x00ae2378);
    flags=cm_fn_00944cf1(limit*4);
    if (flags==0) {
        DIAG(l+8,l+0x108,0x243);
        D(0x00b67a34)=0;
        cm_fn_00944c94(first);
        value=0;
        goto done;
    }
    limit=D(0x00ae2378);
    index=0;
    if ((s32)limit>0) {
        cursor=flags;
        do {
            D(cursor)=0;
            limit=D(0x00ae2378);
            ++index; cursor+=4;
        } while ((s32)index<(s32)limit);
    }
    value=D(input+2);
    D(input+0xa)=value;
    item=cm_fn_0068d1b0(input);
    cursor=first;
    count=0;
    while (item!=0) {
        id=D(item);
        if (D(flags+id*4)==0) {
            D(cursor)=id;
            id=D(item);
            ++count; cursor+=4;
            D(flags+id*4)=1;
        }
        input=input_arg;
        item=cm_fn_0068d1b0(input);
    }
    cm_fn_00944c94(flags);
    cm_fn_00945333(first,count,4,0x00444500);
    list=0;
    index=0;
    if ((s32)count>0) {
        do {
            if (list==0) {
                allocation=cm_fn_00944cf1(14);
                D(l+4)=allocation;
                frame.state=0;
                if (allocation!=0) allocation=cm_fn_0068d020(allocation);
                else allocation=0;
                frame.state=0xffffffff;
                list=allocation;
                if (list==0) {
                    DIAG(l+0x308,l+0x208,0x26e);
                    value=D(l);
                    D(0x00b67a34)=0;
                    cm_fn_00944c94(value);
                    value=0;
                    goto done;
                }
            }
            cm_fn_0068d0d0(list,first,4);
            ++index; first+=4;
        } while ((s32)index<(s32)count);
        first=D(l);
    }
    cm_fn_00944c94(first);
    value=list;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return value;
}
