#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0051c800(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_THISCALL cm_fn_00663a50(u32 self)
{
    struct { u32 prev, handler, state; } volatile frame;
    union { u32 align; u8 bytes[0x40c]; } scratch;
    u32 p, a, b, v, obj, table, result, line;
    u8 *s = scratch.bytes;
    void *x, *y;
    frame.state = 0xffffffff;
    frame.handler = 0x95c13e;
    frame.prev = CM_FS(u32,0);
    CM_FS(u32,0) = (u32)&frame;
    p = (u32)cm_fn_00944e46(8);
    if (!p) {
        x = s+0x20c; y = s+0xc; line = 0x245;
        cm_fn_00944cff(0x9eabc8,0,0,x,y);
    } else {
        a = CM_GLOBAL(volatile u32,self+12);
        a = CM_GLOBAL(volatile u32,a);
        a = CM_GLOBAL(volatile u32,a+0xb1);
        a = CM_GLOBAL(volatile u32,a);
        CM_GLOBAL(volatile u32,p) = a;
        a = CM_GLOBAL(volatile u32,self+12);
        a = CM_GLOBAL(volatile u32,a+4);
        a = CM_GLOBAL(volatile u32,a+0xb1);
        a = CM_GLOBAL(volatile u32,a);
        CM_GLOBAL(volatile u32,p+4) = a;
        table = CM_GLOBAL(volatile u32,self);
        /* VA 00663ab5 saves EBX, restored at 00663b6b; four argument
           pushes at 00663ab6/adf/ae0/ae1 feed the thiscall at 00663ae5.
           Evidence: pinned 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
        v = ((u32 (CM_THISCALL *)(u32,u32,void *,void *,u32))CM_GLOBAL(volatile u32,table+0x3c))(self,2,s,s+4,0);
        obj = (u32)cm_fn_00944cf1(0xb2);
        /* 00663af7 mov [esp+0x14],eax spills the allocation result at
           scratch+8 before 00663afd clears SEH state (VA proof: pinned
           3.9.68 bytes 89442414 / c78424200400000000000000). */
        CM_GLOBAL(volatile u32, (u32)(s + 8)) = obj;
        frame.state = 0;
        if (obj) {
            a = CM_GLOBAL(volatile u32,(u32)(s+4));
            b = CM_GLOBAL(volatile u16,self+0x40);
            table = CM_GLOBAL(volatile u32,self+4);
            result = cm_fn_00522e00(obj,self,2,p,CM_GLOBAL(volatile u32,(u32)s),table,v,(u16)b,2,1,(u16)a,0x14,0,0,0,0);
        } else result = 0;
        a = CM_GLOBAL(volatile u32,self+12);
        frame.state = 0xffffffff;
        CM_GLOBAL(volatile u32,a+8) = result;
        cm_fn_009452ca(p);
        cm_fn_009452ca(v);
        a = CM_GLOBAL(volatile u32,self+12);
        a = CM_GLOBAL(volatile u32,a+8);
        cm_fn_0051c800(a,0);
        a = CM_GLOBAL(volatile u32,self+12);
        a = CM_GLOBAL(volatile u32,a+8);
        if (a) {
            /* Preferred-image VAs 00663bf7/00663bfe/00663c07: capture the
               predecessor before the potentially aliasing object store.
               CM0102 3.9.68 SHA-256:
               0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
            u32 prev = frame.prev;
            CM_GLOBAL(volatile u32,self+0x30) = 2;
            CM_FS(u32,0) = prev;
            return;
        }
        x = s+0x30c; y = s+0x10c; line = 0x25c;
        cm_fn_00944cff(0x9eabc8,0,0,x,y);
    }
    a = (u32)cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,a,x,y,line);
    cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
    CM_GLOBAL(u32,0xb67a34) = 0;
    CM_FS(u32,0) = frame.prev;
}
