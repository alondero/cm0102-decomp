#include "cmre.h"
/* Preferred-image VA007e6f70/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD2/RET8/fullEAX1; second formal unused. SEH960821/scratch108:
   counterBYTEL0/directionDWORDL4/recordL8/messageL28. Counter upper bytes
   remain unknown; original785790 reads only BYTE index at785790/7857a9.
   Original540a50 writes direction DWORD before a nonzero return (5412a7,
   54187c); no direction or counter-upper initialization is invented. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007875e0(u32,u32);
extern u32 CM_THISCALL cm_fn_005fd480(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_CDECL cm_fn_007e4750(u32,u32,u32);
extern unsigned long long CM_CDECL cm_fn_007e4de0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32,u32,u32);

u32 CM_STDCALL cm_fn_007e6f70(volatile u32 record_arg,u32 unused_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x108];
    u32 l=(u32)scratch,record,eax,club,manager,person,receiver,index,value;
    u8 byte;
    (void)unused_arg;
    frame.state=0xffffffffu; frame.handler=0x00960821;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0054a110(l+8);
    record=record_arg;
    frame.state=0;
    eax=cm_fn_0053b900(D(record+0x1c));
    if(eax!=0) goto done;
    byte=B(record+0x40);
    if(byte!=0){
        club=D(record+0x1c);
        eax=cm_fn_00540a50(club,l+4,1);
        if(eax==0) goto second;
        if(D(l+4)!=0) goto second;
    }
    club=D(record+0x1c); manager=D(0x00b7431c);
    eax=cm_fn_007875e0(manager,club);
    if(eax==0){
        club=D(record+0x1c);
        club=cm_fn_00540a50(club,0,1);
        if(club==0) goto second;
        manager=D(0x00b7431c);
        eax=cm_fn_007875e0(manager,club);
        if(eax==0) goto second;
        person=D(club+0xcf);
    }else{
        club=D(record+0x1c); person=D(club+0xcf);
    }
    if(B(record+0x40)!=0){
        manager=D(0x00b63c98);
        eax=cm_fn_005fd480(manager,person);
        if(eax!=0) goto second;
    }
    cm_fn_0077bbe0(l+0x28,0x25,0);
    club=D(record+0x1c);
    B((u32)&frame.state)=1;
    cm_fn_007e4750(record,club,l+8);
    B(l)=0;
    cm_fn_007e4de0(l+8,l+0x28,l);
    index=B(l); B(l)=(u8)(index+1u);
    byte=B(record+0x40); value=(byte==0);
    cm_fn_00785790(l+0x28,index,value);
    index=B(l); byte=B(record+0x41); B(l)=(u8)(index+1u);
    cm_fn_00785790(l+0x28,index,byte==0);
    eax=D(record+0x18);
    if(eax!=0) eax=D(eax); else eax=0xffffffffu;
    index=B(l);
    cm_fn_00785790(l+0x28,index,eax);
    receiver=D(person+0x39); manager=D(0x00b7431c);
    cm_fn_007861e0(manager,l+0x28,receiver);
    B((u32)&frame.state)=0;
second:
    byte=B(record+0x41);
    if(byte!=0){
        club=D(record+0x20);
        eax=cm_fn_00540a50(club,l+4,1);
        if(eax==0) goto done;
        if(D(l+4)!=0) goto done;
    }
    club=D(record+0x20); manager=D(0x00b7431c);
    eax=cm_fn_007875e0(manager,club);
    if(eax==0){
        club=D(record+0x20);
        club=cm_fn_00540a50(club,0,1);
        if(club==0) goto done;
        manager=D(0x00b7431c);
        eax=cm_fn_007875e0(manager,club);
        if(eax==0) goto done;
        person=D(club+0xcf);
    }else{
        club=D(record+0x20); person=D(club+0xcf);
    }
    if(B(record+0x41)!=0){
        manager=D(0x00b63c98);
        eax=cm_fn_005fd480(manager,person);
        if(eax!=0) goto done;
    }
    cm_fn_0077bbe0(l+0x28,0x25,0);
    club=D(record+0x20);
    B((u32)&frame.state)=2;
    cm_fn_007e4750(record,club,l+8);
    B(l)=0;
    cm_fn_007e4de0(l+8,l+0x28,l);
    index=B(l); B(l)=(u8)(index+1u);
    byte=B(record+0x41); value=(byte==0);
    cm_fn_00785790(l+0x28,index,value);
    index=B(l); byte=B(record+0x40); B(l)=(u8)(index+1u);
    cm_fn_00785790(l+0x28,index,byte==0);
    eax=D(record+0x18);
    if(eax!=0) eax=D(eax); else eax=0xffffffffu;
    index=B(l);
    cm_fn_00785790(l+0x28,index,eax);
    receiver=D(person+0x39); manager=D(0x00b7431c);
    cm_fn_007861e0(manager,l+0x28,receiver);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return 1;
}
