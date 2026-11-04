/* CM0102 3.9.68, preferred-image VA 00729ff0, base 00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3 / RET12, full EAX. Native callees remain explicit boundaries. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0094627a(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern void CM_THISCALL cm_fn_00923010(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0059af40(u32,u32,u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_00945333(u32,u32,u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_0085add0(u32,u32);
extern u8 CM_THISCALL cm_fn_00897590(u32,u32,u32,u32);
extern u16 CM_THISCALL cm_fn_005b2ae0(u32,u8,u32);
extern u8 CM_STDCALL cm_fn_0085a760(u32,u32);
extern void CM_THISCALL cm_fn_0085a470(u32,u32,u32);
extern void CM_THISCALL cm_fn_00417c10(u32,u32,u32);

u32 CM_STDCALL cm_fn_00729ff0(u32 input,u32 output,u32 mode)
{
    /* L is ESP immediately after the native SUB ESP,218. */
    u8 scratch[0x218];
    u32 l=(u32)scratch, a,b,c,p,q,count=0,index,side,src,dst,team;
    u32 date,receiver,record,loop,kind,target,at,field;
    u8 flag,v,sidebyte;
    long double t;
    v=B(0x00b6b7c4);
    if (!(v&1u)) {
        B(0x00b6b7c4)=v|1u;
        (void)cm_fn_00549ef0(0x00b6b7b8,0x29,0x7b4);
        (void)cm_fn_0094627a(0x0072aa70);
    }
    if (!input || !output) {
        if (!input) {
            (void)cm_fn_00944cff(0x00a17ba0,0,0,(void *)(l+0x118),(void *)(l+0x18));
            a=l+0x118; b=l+0x18; c=0x2bd;
        } else {
            (void)cm_fn_00944cff(0x00a17ba0,0,0,(void *)(l+0x18),(void *)(l+0x118));
            a=l+0x18; b=l+0x118; c=0x2c5;
        }
        p=cm_fn_0090d130();
        /* The three pushes preceding 90d130 remain variadic words for 944c9f. */
        (void)cm_fn_00944c9f(0x00ae24d0,0x009870e8,p,a,b,c);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    p=D(input+0x4792); a=D(p+0xc); D(output)=a;
    p=D(input+0x4792); a=D(p+0x10); D(output+4)=a;
    p=D(input+0x4792); a=W(p+0x2a); W(output+8)=(u16)a;
    p=D(input+0x4792); a=W(p+0x28); W(output+0xa)=(u16)a;
    a=D(input+0xf634); D(output+0x34)=a;
    p=input+0x200; q=output+0x16;
    for (loop=5;loop;--loop) {
        a=W(p-0xa); p+=2; W(q-0xa)=(u16)a;
        a=W(p-2); W(q)=(u16)a;
        a=W(p+8); W(q+0xa)=(u16)a;
        a=W(p+0x12); W(q+0x14)=(u16)a; q+=2;
    }
    D(output+0x38)=0;
    a=D(input+0x477a);
    if ((s32)a<=-1) D(output+0x3c)=0xffffffffu;
    else {
        b=D(0x00ae23bc); p=D(b+a*110u+0x61);
        if (p) { a=D(p); D(output+0x3c)=a; }
        else D(output+0x3c)=0xffffffffu;
    }
    p=D(input+0xf5b6);
    if (!p || (s32)D(p)<=-1) D(output+0x40)=0xffffffffu;
    else { a=D(p); D(output+0x40)=a; }
    a=(u32)(s32)(s16)W(input+0x8ed0);
    W(output+0x4d)=(u16)((s32)a/11);
    a=W(input+0x8ed4); W(output+0x4f)=(u16)a;
    a=W(input+0x8ed2); W(output+0x51)=(u16)a;
    v=B(input); B(output+0x4c)=v;
    v=B(input+0x8eb3); D(output+0x53)=(v==1 || v==4);
    p=D(input+0x4792); receiver=D(0x00de2f74);
    cm_fn_00923010(receiver,output+0xd76,p);
    if (!mode) {
        a=D(output+0x34); p=D(input+0x4792); receiver=D(0x00ae2a38);
        a=cm_fn_0059af40(receiver,p,a); D(output+0x38)=a;
    }
    record=D(input+0x8f7a);
    if (record) {
        a=W(record+6); index=0;
        if ((s16)a>0) {
            q=output+0x57;
            do {
                if ((s16)index<0 || (s16)index>=(s16)a) p=0;
                else p=record+0x720+(u32)(s32)(s16)index*14u;
                a=(u32)(s32)(s16)W(p); kind=0;
                if ((s32)a<=0x1ff8) {
                    if ((s32)a>=0x1ff5) kind=5;
                    else {
                        a+=0xffffe0aau;
                        if (a<=0x79u) {
                            b=B(0x0072a9f4+a);
                            target=D(0x0072a9dc+b*4u);
                            switch (target) {
                            case 0x0072a346: kind=6; break;
                            case 0x0072a439: kind=1; break;
                            case 0x0072a31d: kind=2; break;
                            case 0x0072a2cb: kind=4; break;
                            case 0x0072a2f4: kind=3; break;
                            default: break;
                            }
                        }
                    }
                } else if ((s32)a<=0x201b) {
                    if ((s32)a>=0x2013) kind=1;
                    else if (a==0x2000) kind=7;
                    else if (a==0x2008) kind=8;
                } else if (a==0x2114) kind=1;
                if (kind && count!=50) {
                    if (kind==7 || kind==8) D(output+0x53)=1;
                    v=B(p+6); B(q)=(u8)((v!=0)|((u8)(count+1u)<<1));
                    v=B(p+5); B(q+1)=v; B(q+2)=(u8)kind;
                    if (kind==7 || kind==8) B(q+3)=(u8)count;
                    else { v=B(p+4); B(q+3)=v; }
                    ++count; q+=4;
                }
                ++index; a=W(record+6); b=(u32)(s32)(s16)a;
            } while ((s32)index<(s32)b);
            if ((s32)count>0) {
                (void)cm_fn_00945333(output,b,output+0x57,count,4,0x0072b160);
                p=output+0x57;
                for (loop=count;loop;--loop) { v=B(p); p+=4; B(p-4)=v&1u; }
            }
        }
    }
    if ((s32)count<50) B(output+0x57+count*4u)=0xff;
    for (side=0;side<2;++side) {
        dst=output+side*0x625u+0x11f;
        src=input+side*0x36u+0xf5bc;
        a=D(output+side*4u); team=D(0x00ae23b4)+a*0x245u;
        p=D(input+0x4792); v=B(p+0x40+side);
        flag=0;
        if (v) flag=cm_fn_0085add0(team,0)!=0;
        v=B(src+1); B(dst)=v; v=B(src); B(dst+1)=v;
        v=B(src+4); B(dst+3)=v; v=B(src+5); B(dst+4)=v;
        v=B(src+6); B(dst+2)=v;
        for (loop=7;loop<=9;++loop) { v=B(src+loop); B(dst+loop-2)=v; }
        v=B(src+2); B(dst+8)=v; v=B(src+3); B(dst+9)=v;
        v=B(src+0xb); B(dst+0xa)=v; v=B(src+0xa); B(dst+0xb)=v;
        v=B(src+0xc); B(dst+0xc)=v; v=B(src+0xd); B(dst+0xd)=v;
        v=B(src+0xe); B(dst+0xe)=v; v=B(src+0x10); B(dst+0xf)=v;
        v=B(src+0xf); B(dst+0x10)=v;
        for (loop=0;loop<6;++loop) { a=W(src+0x26+loop*2u); W(dst+0x11+loop*2u)=(u16)a; }
        a=W(src+0x34); W(dst+0x1d)=(u16)a;
        p=src+0x21; q=dst+0x24;
        for (loop=5;loop;--loop) {
            v=B(p-0xf); ++p; B(q-5)=v;
            v=B(p-1); B(q)=v; v=B(p-6); B(q+5)=v;
            v=B(p-0xb); B(q+0xa)=v; ++q;
        }
        v=B(input+0x8d4f+side); B(dst+0x33)=v;
        if (mode) {
            p=D(input+0x4792); v=B(p+0x40+side); a=D(p+0x1c+side*4u);
            receiver=D(0x00de1d08);
            v=cm_fn_00897590(receiver,a,v==0,0);
            at=input+((u32)(s32)(s8)v+side*20u)*0x1beu+0x4796;
            v=B(at); B(dst+0x34)=v;
        } else { v=B(src+0x11); B(dst+0x34)=v; }
        receiver=input+side*0x18e3u+0xc3a8;
        for (loop=0;loop<11;++loop) {
            a=cm_fn_005b2ae0(receiver,(u8)loop,0); W(dst+0x35+loop*2u)=(u16)a;
            a=cm_fn_005b2ae0(receiver,(u8)loop,1); W(dst+0x4b+loop*2u)=(u16)a;
        }
        a=D(input+side*0x18e3u+0xc92c); D(dst+0x61)=a;
        p=input+side*0xd0u; q=D(p+0x904d);
        if (q) {
            a=D(q); D(dst+0x65)=a;
            a=W(p+0x9032); W(dst+0x69)=(u16)a;
            a=W(p+0x9034); W(dst+0x6b)=(u16)a;
            a=W(p+0x9036);
        } else { a=0xffffffffu; D(dst+0x65)=a; W(dst+0x69)=(u16)a; W(dst+0x6b)=(u16)a; }
        W(dst+0x6d)=(u16)a;
        dst+=0x8d;
        a=W(input+side*2u+0x1d6); W(dst-0x1e)=(u16)a;
        src=input+side*0x22d8u+0x47ff;
        for (loop=0;loop<20;++loop,src+=0x1be,dst+=0x49) {
            if (flag) {
                a=D(src+4);
                if (!a) B(dst)=0;
                else {
                    a=D(src); v=cm_fn_0085a760(a,team);
                    sidebyte=(u8)((s8)v<0 ? -(s32)(s8)v : (s32)(s8)v);
                    B(dst)=sidebyte;
                    if (!sidebyte) {
                        a=D(src); receiver=D(0x00dd7ef0);
                        cm_fn_0085a470(receiver,team,a);
                        a=D(src); v=cm_fn_0085a760(a,team);
                        B(dst)=(u8)((s8)v<0 ? -(s32)(s8)v : (s32)(s8)v);
                    }
                }
            } else { a=D(src); B(dst)=a ? (u8)(loop+1u) : 0; }
            p=D(src); a=p ? D(p) : 0xffffffffu; D(dst-0x1c)=a;
            v=B(src-0x50); B(dst+1)=v; v=B(src-0x4f); B(dst+2)=v;
            a=D(dst-0x1c);
            if ((s32)a<=-1) continue;
            p=D(src+4); a=p ? D(p) : 0xffffffffu; D(dst-0x18)=a;
            a=D(src-0xc); D(dst-0x14)=a;
            a=D(src-8); D(dst-0x10)=a;
            a=D(src-4); D(dst-0xc)=a;
            date=D(src); date=date ? date+0x10 : 0x00b6b7b8;
            cm_fn_00417c10(dst-8,l+0x10,date);
            v=B(src-0x4d); B(dst+3)=v;
            t=(long double)CM_GLOBAL(volatile float,src-0x1c);
            t=t*(long double)CM_GLOBAL(volatile double,0x00967898);
            /* Physical native __ftol call 0072A8C0. */
            B(dst+4)=(u8)(long long)t;
            v=B(src-0x5d); B(dst+5)=v; v=B(src-0x5a); B(dst+6)=v;
            v=B(src-0x5c); B(dst+7)=v; v=B(src-0x5b); B(dst+8)=v;
            v=B(src-0x5f); B(dst+9)=v; v=B(src-0x5e); B(dst+0xa)=v;
            v=B(src-0x59); B(dst+0xb)=v; v=B(src-0x58); B(dst+0xc)=v;
            v=B(src-0x55); B(dst+0xd)=v; v=B(src-0x54); B(dst+0xe)=v;
            v=B(src-0x56); B(dst+0xf)=v; v=B(src-0x45); B(dst+0x10)=v;
            v=B(src-0x46); B(dst+0x11)=v; v=B(src-0x44); B(dst+0x12)=v;
            v=B(src-0x43); B(dst+0x13)=v; v=B(src-0x4a); B(dst+0x16)=v;
            v=B(src-0x47); B(dst+0x17)=v; v=B(src-0x49); B(dst+0x14)=v;
            t=(long double)CM_GLOBAL(volatile float,src-0x18);
            /* Physical native __ftol call 0072A937. */
            W(dst+0x1a)=(u16)(long long)t;
            v=B(src-0x52); B(dst+0x15)=v; v=B(src-0x48); B(dst+0x18)=v;
            a=W(src-0x22); W(dst+0x1c)=(u16)a;
            a=W(src-0x20); W(dst+0x1e)=(u16)a;
            a=W(src-0x1e); W(dst+0x20)=(u16)a;
            v=B(src-0x4e); B(dst+0x19)=v;
            for (field=0;field<9;++field) { v=B(src-0x68+field); B(dst+0x22+field)=v; }
            v=B(src-0x57); B(dst+0x2b)=v;
        }
    }
    return 1;
}
