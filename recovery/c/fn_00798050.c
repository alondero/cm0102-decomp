#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
#define M32(a) CM_GLOBAL(volatile u32,(a))
#define M16(a) CM_GLOBAL(volatile u16,(a))
typedef u32 (CM_THISCALL *virtual_fn)(u32,u32,u32 *,u32 *,u32);
u32 CM_THISCALL cm_fn_00798050(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x408];
    u32 clubs, p, q, vt, schedule, obj, result, line, text, n, rounds;
    u32 *a = (u32 *)(scratch + 0);
    u32 *b = (u32 *)(scratch + 4);
    void *s1, *s2;
    frame.prev = CM_FS(u32,0);
    frame.handler = 0x95eb4e;
    frame.state = 0xffffffff;
    CM_FS(u32,0) = (u32)&frame;
    clubs = (u32)cm_fn_00944e46(16);
    if (!clubs) {
        s1 = scratch + 0x208; s2 = scratch + 8;
        cm_fn_00944cff(0xa6a068,0,0,s1,s2);
        line = 0x2cb;
    } else {
        p = M32(self+0xb1); n = M32(p); M32(clubs) = n;
        p = M32(self+12); p = M32(p); p = M32(p+0xb1); n = M32(p+0x3b); M32(clubs+4) = n;
        p = M32(self+0xb1); n = M32(p+0x3b); M32(clubs+8) = n;
        p = M32(self+12); p = M32(p); p = M32(p+0xb1); n = M32(p); M32(clubs+12) = n;
        vt = M32(self);
        /* 007980b8 saves EBX; 007980b9/f6/f7/f8 are four arguments, SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
        schedule = ((virtual_fn)M32(vt+0x3c))(self,1,a,b,0);
        obj = (u32)cm_fn_00944cf1(0xb2);
        frame.state = 0;
        if (obj) {
            rounds = *b;
            n = M16(self+0x40);
            p = M32(self+4);
            q = *a;
            result = cm_fn_00522e00(obj,self,4,clubs,q,p,schedule,n,1,2,rounds,0x14,0,0,0,0);
        } else result = 0;
        p = M32(self+12);
        frame.state = 0xffffffff;
        M32(p+4) = result;
        cm_fn_009452ca(schedule);
        cm_fn_009452ca(clubs);
        p = M32(self+12);
        p = M32(p+4);
        if (p) {
            result = cm_fn_0051c800(p,0);
            CM_FS(u32,0) = frame.prev;
            return result;
        }
        s1 = scratch + 0x308; s2 = scratch + 0x108;
        cm_fn_00944cff(0xa6a068,0,0,s1,s2);
        line = 0x2e3;
    }
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,text,s1,s2,line);
    result = cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
    M32(0xb67a34) = 0;
    CM_FS(u32,0) = frame.prev;
    return result;
}
