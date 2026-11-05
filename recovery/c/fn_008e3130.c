#include "cmre.h"
/* Preferred-image VA008e3130, base00400000, target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Three stack words / RET12. Incoming ECX is forwarded unchanged to008d1130,
   whose008d1156 saves that physical receiver; keep it explicit at the boundary.
   L is ESP after SUB158. Reporter L+0; recipient words L+e0..157;
   original linked SEH record begins L+158. Scratch bytes are uninitialized. */
extern u32 CM_THISCALL cm_fn_008d1130(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))

u32 CM_THISCALL cm_fn_008e3130(u32 incoming_ecx,u32 record,u32 other,u32 input)
{
    u8 scratch[0x158];
    volatile struct { u32 prev,handler,state; } frame;
    u32 a,b,c,p,remaining,predecessor;
    u8 count;
    predecessor=CM_FS(u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x009647cbu;
    frame.prev=predecessor;
    CM_FS(u32,0)=(u32)&frame;
    a=cm_fn_008d1130(incoming_ecx,input,3,(u32)(scratch+0xe0),30);
    count=(u8)a;
    if(count) {
        a=(u32)cm_fn_0077bbe0(scratch,0xfba,0);
        frame.state=0;
        b=D(record);a=(u32)cm_fn_00785790(scratch,0,b);
        c=D(record+4);b=D(c+0x33);
        a=(u32)cm_fn_00785790(scratch,1,b);
        c=D(record+8);b=D(c+0x33);
        a=(u32)cm_fn_00785790(scratch,2,b);
        c=D(record+0xc);b=D(c+0x33);
        a=(u32)cm_fn_00785790(scratch,3,b);
        b=D(other);a=(u32)cm_fn_00785790(scratch,4,b);
        c=D(0x00ae1050);a=cm_fn_004cf5a0(c,record);
        D(0x00ae1080)=a;
        if(a && (B(a+0x35)&0x3f)!=5) {
            b=D(a+4);c=D(0x00ae23b4);b=c+b*581u;
            if(b) {
                c=D(0x00ae1050);a=cm_fn_004cf5a0(c,record);
                D(0x00ae1080)=a;
                if(a && (B(a+0x35)&0x3f)!=5) {
                    b=D(a+4);c=D(0x00ae23b4);
                    a=D(c+b*581u);
                } else {
                    /* Native008e325d XOR EAX,EAX;008e325f reads DWORD[0].
                       Volatile address materialization retains the data fault. */
                    volatile u32 null_address=0;
                    a=D(null_address);
                }
            } else a=0xffffffffu;
        } else a=0xffffffffu;
        a=(u32)cm_fn_00785790(scratch,5,a);
        if((s8)count>0) {
            p=(u32)(scratch+0xe0);remaining=(u32)(s32)(s8)count;
            do {
                b=D(p);c=D(0x00b7431c);
                a=(u32)cm_fn_007861e0((void *)c,(u32)scratch,b);
                p+=4;
                --remaining;
            } while(remaining);
        }
    }
    c=frame.prev;
    CM_FS(u32,0)=c;
    return a;
}
