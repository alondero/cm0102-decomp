#include "cmre.h"
/* Preferred-image VAs below refer to CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receivers and stack arguments follow 58b2-58ba, 5a8b-5a9d,
   5ac8-5ae7, 5992-599c and 5b76-5bb3 in this routine (0x008c prefix).
   0054c5b0's EAX is consumed as a pointer at 008c5a99 and 008c5ae3.
   Indirect slot at 008c5b2d receives ECX and one pushed date. */
#define M8(a) CM_GLOBAL(volatile u8,(u32)(a))
#define M16(a) CM_GLOBAL(volatile u16,(u32)(a))
#define M32(a) CM_GLOBAL(volatile u32,(u32)(a))
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053d740(u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_FASTCALL cm_fn_0054cfb0(u32);
extern u32 CM_THISCALL cm_fn_0068d080(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_008c18c0(u32);
extern u32 CM_THISCALL cm_fn_008c3eb0(u32);
extern u32 CM_THISCALL cm_fn_008c6220(u32,u32,s8,u8);
extern u32 CM_THISCALL cm_fn_008c63a0(u32,u32);
extern void CM_THISCALL cm_fn_008c6bc0(u32,u32);
extern u32 CM_THISCALL cm_fn_008c7330(u32);
extern u32 CM_FASTCALL cm_fn_008caee0(u32);
extern u32 CM_FASTCALL cm_fn_008cbbe0(u32);

void CM_THISCALL cm_fn_008c5880(u32 self,u32 date,volatile u8 tick)
{
    u32 prev=CM_FS(u32,0);
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x28];
    u32 a,d,c,p,q,i,slot,idx,iter;
    u8 bl,old;
    s32 day;
    frame.state=0xffffffff;
    frame.handler=0x963ce0;
    frame.prev=prev;
    CM_FS(u32,0)=(u32)&frame;
    if(M16(self+0x84f)==0) {
        cm_fn_00417c10(self+0x84d,(u32)(scratch+0x10),date);
        M8(self+0x855)=tick;
    }
    a=M32(date); d=M32(date+4);
    M32(scratch)=a; c=M16(scratch+2); M32(scratch+4)=d;
    if(c!=M16(self+0x84f)) goto done;
    if((u16)a!=M16(self+0x84d)) goto done;
    bl=tick; old=M8(self+0x855);
    if(old!=bl) goto done;
    if(bl==0) cm_fn_008c6bc0(self,date);
    day=(s16)M16(date);
    if(day%7==0) {
        i=0; a=M32(self+4);
        while((s32)i<(s32)a) {
            p=M32(self); a=M32(p+i*4);
            if(a) {
                d=M32(date); c=M32(date+4);
                M32(scratch)=d; q=M16(scratch+2); M32(scratch+4)=c;
                c=M16(a+0x14);
                if(q>c || (q==c && (s16)d>(s16)M16(a+0x12))) {
                    old=M8(a+0x2c);
                    if(old!=0x13 && old!=7 && old!=8 && old!=10)
                        cm_fn_008c6220(self,a,1,0);
                }
            }
            a=M32(self+4); i++;
        }
    }
    old=M8(self+0x855); M8(self+0x855)=(u8)(old+1);
    cm_fn_008c18c0(self);
    bl=tick; idx=(u32)(s32)(s8)bl;
    i=idx+3*(u32)(s32)(s8)M8(self+0x84c);
    d=M32(self+i*14+0x1a); M32(self+i*14+0x22)=d;
    i=idx+3*(u32)(s32)(s8)M8(self+0x84c);
    a=cm_fn_0068d1b0(self+i*14+0x18);
    if(a) {
        do {
            a=M32(a); c=M32(self+4);
            if((s32)a<(s32)c && (s32)a>=0) {
                c=M32(self); slot=c+a*4; a=M32(slot);
                if(a) {
                    d=M32(date); q=M32(date+4);
                    M32(scratch)=d; M32(scratch+4)=q;
                    q=M16(scratch+2);
                    if(q==M16(a+0x14) && (u16)d==M16(a+0x12)) {
                        old=tick; bl=M8(a+0x1a);
                        if(bl==old) cm_fn_008c63a0(self,slot);
                    }
                }
            }
            i=idx+3*(u32)(s32)(s8)M8(self+0x84c);
            a=cm_fn_0068d1b0(self+i*14+0x18);
        } while(a);
        bl=tick;
    }
    i=idx+3*(u32)(s32)(s8)M8(self+0x84c);
    a=cm_fn_0068d080(self+i*14+0x18);
    if(bl==2) {
        a=cm_fn_0054c5b0(self+0x84d,(u32)(scratch+0x18),1);
        cm_fn_00417c10(self+0x84d,(u32)(scratch+0x20),a);
        old=M8(self+0x84c); M8(self+0x855)=0;
        old=(u8)(old+1); M8(self+0x84c)=old;
        if((s8)old>=50) M8(self+0x84c)=0;
        p=cm_fn_0054c5b0(date,(u32)(scratch+0x20),1);
        frame.state=0;
        bl=(u8)cm_fn_0054cfb0(date);
        a=cm_fn_0054cfb0(p);
        frame.state=0xffffffff;
        a=(a&0xffffff00)|(bl!=(u8)a);
        if((u8)a) a=cm_fn_008caee0(self);
        if(M16(date)==0) {
            cm_fn_008c3eb0(self);
            a=cm_fn_008c7330(self);
        }
        slot=self+0x8ac;
        for(i=28;i;i--,slot+=4) {
            c=M32(slot);
            if(c) {
                a=M32(c);
                a=((u32 (CM_THISCALL *)(u32,u32))M32(a))(c,date);
            }
        }
        day=(s16)M16(date);
        a=(u32)(day/7);
        if(day%7==0) a=cm_fn_008c7330(self);
    } else if(bl==0) {
        a=cm_fn_008cbbe0(self);
    }
    a=(a&0xffff0000)|M16(date);
    if((u16)a!=100 || tick!=2) goto done;
    a=(a&0xffff0000)|M16(date+2);
    a=(u32)cm_fn_00549ef0((u32)(scratch+8),100,a-5);
    a=(a&0xffff0000)|M16(date+2);
    frame.state=1;
    a=(u32)cm_fn_00549ef0((u32)scratch,100,a-2);
    p=(date&0xffff0000)|M16(date+2);
    p-=10;
    M8((u32)&frame.state)=2;
    cm_fn_00549ef0((u32)(scratch+0x10),100,p);
    a=M32(0xae2364); p=M32(0xae23bc); iter=0;
    M8((u32)&frame.state)=3;
    if((s32)a<=0) goto done;
    do {
        a=M16(p+0x40); c=M16(scratch+2);
        if(c<a) goto next;
        if(c==a && (s16)M16(scratch)<=(s16)M16(p+0x3e)) goto next;
        a=M32(p+0x39); if(!a) goto next;
        a=M32(a+0x53); if(!a) goto next;
        c=M32(a+0x71); if(!c) goto next;
        c=M32(c); d=M32(0x9cfa10); if(c!=d) goto next;
        if(cm_fn_0053d740(p,a)) goto next;
        d=M32(p+0x39); a=M32(d+0x53); d=M32(a);
        a=M32(0x9cf488);
        if(d==a || d==M32(0x9cf434)) goto store;
        a=M16(p+0x40); c=M16(scratch+0xa);
        if(c>a || (c==a && (s16)M16(scratch+8)>(s16)M16(p+0x3e))) {
            if(d!=M32(0x9cf360) && d!=M32(0x9cf2fc) && d!=M32(0x9cf448)) goto store;
        }
        c=M16(scratch+0x12);
        if(c<a) goto next;
        if(c==a && (s16)M16(scratch+0x10)<=(s16)M16(p+0x3e)) goto next;
store:
        d=M32(p+0x39); a=M32(d+0x53); M32(p+0x1e)=a;
next:
        a=M32(0xae2364); iter++; p+=0x6e;
    } while((s32)iter<(s32)a);
done:
    prev=frame.prev;
    CM_FS(u32,0)=prev;
}
