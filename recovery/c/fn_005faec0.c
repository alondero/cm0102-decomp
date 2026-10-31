#include "cmre.h"
/* Preferred-image VA005faec0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD1/fullEAX; scratch1c0 messagesL0/Le0, eachdfbytes. Re-read record
   pointers per field; reload list formal only after the second message branch. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u8);
extern u32 CM_THISCALL cm_fn_00785790(u32,u8,u32);
extern u32 CM_THISCALL cm_fn_00786640(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786780(u32,u32);

u32 CM_STDCALL cm_fn_005faec0(volatile u32 list_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x1c0];
    u32 l=(u32)scratch,list,record,item,value,id,other,base,recipient,owner,offset,i;
    u8 kind;
    frame.prev=CM_FS(volatile u32,0);
    frame.handler=0x0095afa6; frame.state=0xffffffff;
    CM_FS(volatile u32,0)=(u32)&frame;
    list=list_arg;
    value=D(list+2); D(list+0xa)=value;
    item=cm_fn_0068d1b0(list);
    while (item) {
        record=D(item);
        kind=B(record+0x1e);
        if ((s8)kind>0 || kind==0xfd) {
            value=W(record+4);
            other=W(0x00ae2c92);
            value-=8;
            if (other==value) goto next;
        }
        id=D(record);
        other=D(0x009cf7a0);
        if (id==other) goto next;
        if (id==D(0x009cf79c) && W(record+4)==2006) {
            value=D(record+6);
            other=D(0x009cf314);
            if (value==other) goto next;
        }
        if (kind==0xff) {
            cm_fn_0077bbe0(l,4,0);
            record=D(item);
            frame.state=0;
            value=D(record); cm_fn_00785790(l,0,value);
            record=D(item); value=W(record+4); cm_fn_00785790(l,1,value);
            for (i=0;i<6;++i) {
                record=D(item); value=D(record+6+i*4);
                cm_fn_00785790(l,(u8)(i+2),value);
            }
            record=D(item); id=D(record);
            base=D(0x00ae23d0);
            recipient=D(base+id*107+0x59);
            if (recipient) {
                owner=D(0x00b7431c);
                cm_fn_00786640(owner,l,recipient);
            } else {
                owner=D(0x00b7431c);
                cm_fn_00786780(owner,l);
            }
            frame.state=0xffffffff;
        } else {
            cm_fn_0077bbe0(l+0xe0,5,0);
            record=D(item);
            frame.state=1;
            kind=B(record+0x1e);
            if (kind==0xfd) kind=0;
            value=D(record); cm_fn_00785790(l+0xe0,0,value);
            record=D(item); value=W(record+4); cm_fn_00785790(l+0xe0,1,value);
            record=D(item);
            offset=(u32)(s32)(s8)kind*8;
            value=D(record+offset+6); cm_fn_00785790(l+0xe0,2,value);
            record=D(item); value=D(record+offset+0xa); cm_fn_00785790(l+0xe0,3,value);
            record=D(item); base=D(0x00ae23d0); id=D(record);
            recipient=D(base+id*107+0x59);
            if (recipient) {
                owner=D(0x00b7431c);
                cm_fn_00786640(owner,l+0xe0,recipient);
            } else {
                owner=D(0x00b7431c);
                cm_fn_00786780(owner,l+0xe0);
            }
            list=list_arg;
            frame.state=0xffffffff;
        }
next:
        item=cm_fn_0068d1b0(list);
    }
    CM_FS(volatile u32,0)=frame.prev;
    return item;
}
