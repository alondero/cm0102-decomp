/* Preferred-image VA007b7840, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3. Original helpers remain boundaries. Scratch offsets derive from
   ESP after four saves; no uninitialized upper bytes are manufactured. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
extern u32 CM_THISCALL cm_fn_0054a110(void *);
extern u32 CM_THISCALL cm_fn_00785830(void *,u32);
extern u32 CM_CDECL cm_fn_007e52c0(void *,void *,void *);
extern u32 CM_THISCALL cm_fn_004afa40(void *);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_006181d0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00616f20(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007f6cd0(u32,s8,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_007b7840(u32 source,u32 mode,u32 destination)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x3b4];
    u32 p=(u32)scratch, nation,competition,youth,offset,tag;
    u32 first,second,third,person,record,current,lookup,a,b,line,result;
    u32 count,scan,read,write,words;
    u8 counter;
    frame.state=0xffffffffu;
    frame.handler=0x0095f5fbu;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0054a110((void *)(p+0x18));
    frame.state=0;
    if(source==0 || destination==0) {
        a=p+0x1b4;b=p+0x2b4;line=0x12a5;
        goto diagnostic;
    }
    B(p+3)=1;
    B(p+4)=(u8)cm_fn_00785830((void *)source,0);
    cm_fn_007e52c0((void *)source,(void *)(p+3),(void *)(p+8));
    counter=B(p+3);B(p+3)=(u8)(counter+1);
    nation=cm_fn_00785830((void *)source,counter);
    counter=B(p+3);B(p+3)=(u8)(counter+1);
    B(p+0x20)=(u8)cm_fn_00785830((void *)source,counter);
    counter=B(p+3);B(p+3)=(u8)(counter+1);
    competition=cm_fn_00785830((void *)source,counter);
    counter=B(p+3);B(p+3)=(u8)(counter+1);
    youth=cm_fn_00785830((void *)source,counter);
    cm_fn_004afa40((void *)destination);
    if(mode) {
        person=D(p+0x14);third=D(p+0x10);second=D(p+0xc);first=D(p+8);
        cm_fn_0053e140(first,second,third,p+0xec,200,1,person);
    }
    if((s32)nation>-1) {
        record=D(0x00ae23a8);offset=nation*0x122u;
        cm_fn_006181d0(record+offset,p+0x24,200,1);
        record=D(0x00ae23a8);tag=(u32)(s32)(s8)B(record+offset+0x37);
        if(mode)cm_fn_0066f4e0(0x00de1f64,0x00a7048c,tag,tag,p+0xec,p+0x24);
        else cm_fn_0066f4e0(0x00de1f64,0x00a70304,tag,tag,p+0x24);
    } else if((s8)B(p+0x20)>-1) {
        tag=D(p+0x20);
        cm_fn_00616f20(tag,p+0x24,200,1);
        if(mode)cm_fn_0066f4e0(0x00de1f64,0x00a70420,100,100,p+0xec,p+0x24);
        else cm_fn_0066f4e0(0x00de1f64,0x00a702dc,100,100,p+0x24);
    } else if((s32)competition>-1) {
        record=D(0x00ae23d0)+competition*0x6bu;
        scan=record+4;count=0xffffffffu;
        do { if(!count)break; --count;tag=B(scan++); } while(tag!=0);
        count=~count;read=scan-count;write=p+0x24;words=count>>2;
        while(words--){tag=D(read);D(write)=tag;read+=4;write+=4;}
        count&=3;
        while(count--){tag=B(read);B(write)=(u8)tag;++read;++write;}
        tag=(u32)(s32)(s8)B(record+0x37);
        if(mode)cm_fn_0066f4e0(0x00de1f64,0x00a703b4,tag,tag,p+0xec,p+0x24);
        else cm_fn_0066f4e0(0x00de1f64,0x00a702b4,tag,tag,p+0x24);
    } else if(youth) {
        if(mode) {
            cm_fn_0066f4e0(p+0x24,0x00a70398);
            cm_fn_0066f4e0(0x00de1f64,0x00a7032c,p+0xec,p+0x24);
        } else {
            cm_fn_0066f4e0(0x00de1f64,0x00a7029c);
            cm_fn_004ae660((void *)destination,0x00de1f64);
            result=1;goto finish;
        }
    } else {
        if(mode){a=p+0x2b4;b=p+0x1b4;line=0x133e;}
        else {a=p+0x1b4;b=p+0x2b4;line=0x139f;}
        goto diagnostic;
    }
    cm_fn_004ae660((void *)destination,0x00de1f64);
    person=D(p+0x14);
    if((s32)person<=-1){result=1;goto finish;}
    third=D(p+0x10);second=D(p+0xc);first=D(p+8);
    record=D(0x00ae23bc)+person*0x6eu;
    if(!cm_fn_0053e990(record,first,second,third,p+0x18)){result=1;goto finish;}
    if(mode) {
        person=D(p+0x14);record=D(0x00ae23bc)+person*0x6eu;
        cm_fn_004ae8a0((void *)destination,p+0xec,0x7d7,record);
    }
    tag=D(p+4);lookup=D(0x00b74448);
    current=cm_fn_007f6cd0(lookup,(s8)tag,0);
    person=D(current+0x180);
    if((s32)person>-1) {
        third=D(current+0x17c);second=D(current+0x178);first=D(current+0x174);
        record=D(0x00ae23bc)+person*0x6eu;
        if(cm_fn_0053e990(record,first,second,third,current+0x184)) {
            person=D(current+0x180);record=D(0x00ae23bc)+person*0x6eu;
            goto compare;
        }
    }
    record=0;
compare:
    person=D(p+0x14);lookup=D(0x00ae23bc)+person*0x6eu;
    if(record==lookup) {
        tag=(u32)(s32)(s8)B(p+4);
        cm_fn_004ae8a0((void *)destination,p+0x24,0x7e4,tag);
    }
    result=1;goto finish;
diagnostic:
    cm_fn_00944cff(0x00a701b0,0,0,(void *)a,(void *)b);
    tag=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,tag,a,b,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    result=0;
finish:
    /* Original retains state zero until unlink; there is no final state reset. */
    lookup=frame.prev;
    CM_FS(volatile u32,0)=lookup;
    return result;
}
