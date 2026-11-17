#include "cmre.h"
/* Original-instruction authority: CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VAs 00732c80..00732fb1: ECX receiver, RET 16.
   Calls 00732ce0/00732df8 use global receivers and two/one pushes.
   00732cfe and all 00785790 calls use L+4, two pushes.
   00732f94 uses global receiver, scratch and ESI arguments.
   RNG calls (00732dcc,00732e3f,00732e4b,00732e57,00732e70,
   00732e81) push one bound and clean four bytes; only EAX is consumed. */
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0062ed70(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define SET(i,v) do { r=cm_fn_00785790(scratch.object,(i),(v)); a=(u32)r; d=(u32)(r>>32); } while(0)
#define RNG(n) do { a=cm_fn_0090cfc0(n); } while(0)

void CM_THISCALL cm_fn_00732c80(u32 self,u32 person,u32 club,s16 mode,s8 flag)
{
    volatile struct { u32 prev,handler,state; } frame;
    struct { u32 saved; u8 object[224]; } scratch;
    u32 a,d,p,q,count,id,idx,target,value;
    s32 selected;
    unsigned long long r;
    a=CM_FS(u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x0095d6bb;
    frame.prev=a;
    CM_FS(u32,0)=(u32)&frame;
    if (!person) goto done;
    a=D(person+0x61);
    if (!a) goto done;
    if (!club) goto done;
    a=D(club+0xcf);
    if (!a) goto done;
    a=cm_fn_004cf5f0(D(0xae1050),person,club);
    scratch.saved=a;
    if (!a) goto done;
    r=cm_fn_0077bbe0(scratch.object,0xbda,0);
    a=D(person);
    frame.state=0;
    SET(0,a);
    p=D(person+4); d=D(p+0x33);
    SET(1,d);
    a=D(person+8); p=D(a+0x33);
    SET(2,p);
    a=D(person+12);
    if(a) { d=D(a+0x33); value=d; } else value=0xffffffffu;
    SET(3,value);
    a=D(club);
    SET(4,a);
    count=D(self+8); d=0;
    if ((s32)count>0) {
        a=D(self); id=D(person); a+=0x10;
        do {
            if(D(a-8)==id && B(a)==2) goto special;
            ++d; a+=0x17;
        } while ((s32)d<(s32)count);
    }
    a=D(scratch.saved+0x45);
    if (!(a&0x2000) && !(a&8)) goto ordinary;
special:
    selected=mode;
    if (mode==14 || mode==0 || mode==2 || mode==9 ||
        mode==35 || mode==4 || mode==7) {
        RNG(10);
        p=(u32)(s32)(s8)B(person+0x5d);
        d=((s32)p >= (s32)a) ? 0x12 : 0x21;
        selected=(s32)d;
    }
    goto mode_ready;
ordinary:
    selected=mode;
mode_ready:
    a=cm_fn_0062ed70(D(0xb63d28),person);
    if(a) selected=12;
    selected=(s16)selected;
    SET(5,(u32)selected);
    a=(u32)(s32)flag;
    SET(6,a);
    if((u32)selected>0x25) goto zero;
    idx=B(0x732fcc+(u32)selected);
    target=D(0x732fb4+idx*4);
    switch(target) {
    case 0x732e3d: RNG(4); break;
    case 0x732e49: RNG(2); break;
    case 0x732e55: RNG(3); break;
    case 0x732e61:
        a=D(person+0x61);
        if(a && (s8)B(a+0x44)<10) {
            RNG(3);
            a=(u32)(s32)(s8)(u8)((u8)a+2);
        } else {
            RNG(5);
            if(a==0 || a==1) {
                person=D(person+0x61);
                if(person) {
                    p=B(person+0x44);
                    if((s8)p<20) B(person+0x44)=(u8)(p+1);
                }
            }
        }
        break;
    default:
zero:
        a=0;
        break;
    }
    SET(7,a);
    d=D(club+0xcf); a=D(d);
    SET(8,a);
    p=D(club+0xcf); d=D(p+4); a=D(d+0x33);
    SET(9,a);
    p=D(club+0xcf); d=D(p+8); a=D(d+0x33);
    SET(10,a);
    p=D(club+0xcf); d=D(p+12); a=D(d+0x33);
    SET(11,a);
    SET(12,0x420e9);
    a=D(club+0xd3);
    if(a) {
        p=D(a);
        SET(13,p);
        d=D(club+0xd3); a=D(d+4); p=D(a+0x33);
        SET(14,p);
        d=D(club+0xd3); a=D(d+8); p=D(a+0x33);
        SET(15,p);
        d=D(club+0xd3); a=D(d+12); p=D(a+0x33);
        SET(16,p);
    } else {
        SET(13,0xffffffffu);
    }
    q=D(0xb7431c);
    cm_fn_007861e0((void *)q,(u32)scratch.object,club);
done:
    CM_FS(u32,0)=frame.prev;
}
