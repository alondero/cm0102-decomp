#include "cmre.h"
/* Original-instruction evidence: preferred-image VAs in CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry 008bfcb9 saves ECX; 008c0070 pops three arguments.
   Calls below retain the ECX receivers and reverse-order pushes in
   008bfcd1..008c0058. In particular 008bfd50 forwards EBP in ECX
   to 008c6db0, alongside the pushed ESI. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define S(a) CM_GLOBAL(volatile s8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf4a0(u32,u32);
extern u32 CM_THISCALL cm_fn_008cbf70(u32,u32);
extern u32 CM_THISCALL cm_fn_008c6db0(u32,u32);
extern u32 CM_THISCALL cm_fn_0084aeb0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_008c6220(u32,u32,s8,u8);
extern void CM_THISCALL cm_fn_008c1290(u32,u32);
extern u32 CM_THISCALL cm_fn_00849ce0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00843e10(u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_008d5170(u32,u32,s16);
extern u32 CM_THISCALL cm_fn_004cca70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004ccb30(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_THISCALL cm_fn_00842080(u32,u32,u32);

void CM_THISCALL cm_fn_008bfcb0(u32 self, volatile u32 argument,
                               volatile u32 flag, volatile u32 reason)
{
    u32 holder=argument, rec=D(holder), a, base, oldclub, savedclub;
    u32 club=0, person, node, list, x, y, selector, r;
    u8 status;
    a=D(rec+12);
    person=D(0xae23bc)+a*110u;
    a=cm_fn_004cf5a0(D(0xae1050),person);
    base=D(0xae23b4);
    D(0xae1080)=a;
    if(a) { x=D(a+4); argument=base+x*581u; }
    else argument=0;
    oldclub=argument;
    x=D(rec+8);
    savedclub=base+x*581u;
    if(flag) { a=D(holder); B(a+0x2d)=1; }
    a=D(holder);
    if(B(a+0x2c)==0x13) cm_fn_008cbf70(self,a);
    if(S(rec+0x2c)>=11) {
        x=D(rec+12);
        cm_fn_004cf4a0(D(0xae1050),x);
    }
    B(rec+0x2c)=3;
    cm_fn_008c6db0(self,rec);
    a=D(rec+12);
    x=D(0xae1050);
    person=D(0xae23bc)+a*110u;
    a=cm_fn_004cf5a0(x,person);
    base=D(0xae23b4);
    D(0xae1080)=a;
    if(a) { x=D(a+4); club=base+x*581u; }
    x=D(rec+8);
    cm_fn_0084aeb0(D(self+0x856),base+x*581u,person);
    list=D(rec+4);
    if(list) {
        x=D(list+2); D(list+10)=x;
        node=cm_fn_0068d1b0(D(rec+4));
        while(node) {
            x=D(node);
            person=D(0xae23bc)+x*110u;
            cm_fn_0084aeb0(D(self+0x856),club,person);
            node=cm_fn_0068d1b0(D(rec+4));
        }
    }
    B(rec+0x10)=4;
    cm_fn_008c6220(self,rec,10,0);
    status=B(rec+0x2c);
    if(status!=2 && status!=3 && status!=20 && status!=15 &&
       status!=16 && status!=4) goto after_clear;
    if(status==15 && B(rec+0x2e)<3) goto after_clear;
    list=D(rec);
    if(list) {
        x=D(list+2); D(list+10)=x;
        node=cm_fn_0068d1b0(D(rec));
        while(node) {
            x=D(node); a=D(self); a=D(a+x*4u);
            status=B(a+0x2c);
            if(status!=2 && status!=3 && status!=20 && status!=15 &&
               status!=16 && status!=4) goto after_clear;
            if(status==15 && B(a+0x2e)<3) goto after_clear;
            node=cm_fn_0068d1b0(D(rec));
        }
    }
    x=D(rec+12); a=D(self+0x86e); a+=x*8u;
    status=B(a);
    if(status&1) B(a)=status&0xfe;
after_clear:
    r=reason;
    if((u16)r==2) cm_fn_008c1290(self,rec);
    selector=B(rec+0x11)!=11;
    x=D(rec+12); person=D(0xae23bc)+x*110u;
    x=D(rec+8); base=D(0xae23b4); y=base+x*581u;
    a=cm_fn_00849ce0(D(self+0x856),y,person,selector,0);
    if(a) {
        status=B(a);
        base=D(0xae23b4);
        /* 008bff01 pushes EAX after SETNE AL, retaining its high bits. */
        selector=(a&0xffffff00u)|(status!=11);
        x=D(rec+8); y=base+x*581u;
        x=D(rec+12); person=D(0xae23bc)+x*110u;
        cm_fn_00843e10(D(self+0x856),person,y,1,selector);
    }
    cm_fn_008d5170(D(0xde1f54),rec,(s16)r);
    x=D(rec+8); base=D(0xae23b4); y=base+x*581u;
    x=D(rec+12); person=D(0xae23bc)+x*110u;
    cm_fn_004cca70(D(0xae1050),person,y);
    list=D(rec+4);
    B(rec+0x10)=0;
    if(list) {
        x=D(list+2); D(list+10)=x;
        node=cm_fn_0068d1b0(D(rec+4));
        while(node) {
            y=D(rec+0x20); x=D(node);
            person=D(0xae23bc)+x*110u;
            cm_fn_004ccb30(D(0xae1050),person,y);
            B(node+4)=0;
            node=cm_fn_0068d1b0(D(rec+4));
        }
    }
    list=D(rec+4);
    if(list && oldclub && cm_fn_00600a50(oldclub,0,1,0,0)==0) {
        list=D(rec+4); x=D(list+2); D(list+10)=x;
        node=cm_fn_0068d1b0(D(rec+4));
        while(node) {
            x=D(node); person=D(0xae23bc)+x*110u;
            a=cm_fn_00842080(D(self+0x856),person,savedclub);
            if((u8)a) {
                x=D(node); base=D(0xae23bc); person=base+x*110u;
                cm_fn_00843e10(D(self+0x856),person,savedclub,1,0);
            }
            node=cm_fn_0068d1b0(D(rec+4));
        }
    }
}
