#include "cmre.h"
/* Preferred-image VA0088c6d0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS12/fullEAX returns self. Scratch208: index BYTE L0/saved selfL4,
   diagnosticsL8/L108. SignedBYTE club loop uses cached signedWORD count. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32,u8,u32,u8,u32);
extern u32 CM_THISCALL cm_fn_0088c9f0(u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0088c6d0(u32 self,volatile u16 count_arg,volatile u32 ids_arg,volatile u32 first_arg,volatile u32 second_arg,volatile u32 start_arg,volatile u32 cutoff_arg,volatile u32 filter_arg,volatile u8 flag_a_arg,volatile u8 flag_b_arg,volatile u32 bytes_arg,volatile u32 selector_arg,volatile u32 list_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x208];
    u32 l=(u32)scratch,eax,bytes,i,filter,base,value,ids,first,second,start,cutoff,selector,list,name,extension,line,version;
    s32 count;
    u8 flag_a,flag_b,index;
    frame.state=0xffffffff; frame.handler=0x00962ffb;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(l+4)=self;
    cm_fn_00682200(self);
    flag_a=flag_a_arg; flag_b=flag_b_arg;
    B(self+0xc2)=flag_a; B(self+0xc3)=flag_b;
    bytes=bytes_arg;
    frame.state=0;
    D(self)=0x0096ff90;
    for (i=0;i<4;++i) {flag_a=B(bytes+i); B(self+0xc5+i)=flag_a;}
    filter=filter_arg;
    D(self+0xbe)=0; D(self+0x2c)=0; D(self+0x30)=0xffffffff;
    B(self+0x44)=0xfe; W(self+0x3c)=0; D(self+0xc)=0; D(self+8)=0;
    D(self+0xdd)=1;
    if ((s32)filter>=0 && (s32)filter<(s32)D(0x00ae2378)) {
        base=D(0x00ae23d0);
        D(self+4)=base+filter*107;
    } else D(self+4)=0;
    value=count_arg;
    W(self+0x3e)=(u16)value;
    count=(s16)value;
    eax=cm_fn_00944e46((u32)count*59);
    D(self+0xb1)=eax;
    if (!eax) {
        name=l+0x108; extension=l+8; line=0x56;
        goto diagnostic;
    }
    count=(s16)W(self+0x3e);
    index=0; B(l)=0;
    if (count>0) {
        ids=ids_arg;
        do {
            value=D(ids+(u32)(s32)(s8)index*4);
            base=D(0x00ae23b4);
            cm_fn_00687430(self,B(l),base+value*0x245,0,0);
            ++index;
            B(l)=index;
        } while ((s32)(s8)index<count);
        filter=filter_arg;
    }
    list=list_arg;
    selector=selector_arg;
    cutoff=cutoff_arg;
    start=start_arg;
    second=second_arg;
    first=first_arg;
    eax=cm_fn_0088c9f0(self,first,second,start,cutoff,filter,selector,list);
    if (!eax) {
        name=l+8; extension=l+0x108; line=0x63;
        goto diagnostic;
    }
    eax=cm_fn_006827d0(self,0);
    if (eax) goto done;
    name=l+8; extension=l+0x108; line=0x68;
diagnostic:
    cm_fn_00944cff(0x00a8b464,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return self;
}
