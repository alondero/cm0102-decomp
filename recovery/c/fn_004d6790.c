#include "cmre.h"
/*
 * Preferred-image VAs, CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Entry 004d69fc RET 10: four stack arguments, no incoming receiver.
 * Receiver/argument setup: 004d6820-682b, 004d685b-68ee,
 * 004d6912-691e, 004d692c-6930, 004d6935-6955,
 * 004d695e-6966, 004d69b5-69ca, 004d69d5-69e1.
 * 004d67e5-680c: the pushes surrounding 0090d130 belong to
 * 00944c9f; 004d6811 cleans both cdecl calls.
 */
extern u32 CM_CDECL cm_fn_00544670(u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_00842180(u32,u32,u32,u32,u32,u32);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
#define R(a) CM_GLOBAL(volatile u32,(a))

void CM_STDCALL cm_fn_004d6790(u32 arg1, s32 arg2, u32 arg3, u32 arg4)
{
    u32 prev = CM_FS(u32,0);
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x2f0];
    u32 i, base, edi, esi, p, v, flag, receiver;
    frame.state = 0xffffffffu;
    frame.handler = 0x956ed6;
    frame.prev = prev;
    CM_FS(u32,0) = (u32)&frame;
    if (!arg1) {
        cm_fn_00944cff(0x9a8190,0,0,scratch+0x1f0,scratch+0xf0);
        p = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0,0x9870e8,p,scratch+0x1f0,
                      scratch+0xf0,0x24f0);
        cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
        R(0xb67a34) = 0;
    } else {
        cm_fn_0077bbe0(scratch+0x10,0xbbd,0);
        i = R(arg1+4);
        base = R(0xae23b4);
        frame.state = 0;
        edi = base + i*0x245u;
        i = R(arg1);
        v = R(edi);
        base = R(0xae23bc);
        esi = base+i*0x6eu;
        cm_fn_00785790(scratch+0x10,0,v);
        v = R(esi);
        cm_fn_00785790(scratch+0x10,2,v);
        p = R(esi+4); v = R(p+0x33);
        cm_fn_00785790(scratch+0x10,3,v);
        p = R(esi+8); v = R(p+0x33);
        cm_fn_00785790(scratch+0x10,4,v);
        p = R(esi+0xc); v = R(p+0x33);
        cm_fn_00785790(scratch+0x10,5,v);
        cm_fn_00785790(scratch+0x10,6,(u32)(s32)(s8)arg2);
        cm_fn_00785790(scratch+0x10,7,arg3);
        v = R(esi+0x61);
        cm_fn_00785790(scratch+0x10,8,v!=0);
        p = R(edi+0x53);
        flag = arg4;
        if (p && !cm_fn_00544670(p)) flag = 0;
        receiver = R(0xb7431c);
        cm_fn_007861e0((void *)receiver,(u32)(scratch+0x10),edi);
        if (flag) {
            cm_fn_0068d020((u32)scratch);
            receiver = R(0xde1f54);
            receiver = R(receiver+0x856);
            *(volatile u8 *)&frame.state = 1;
            cm_fn_00842180(receiver,esi,(u32)scratch,1,1,0);
            v = *(u32 *)(scratch+2);
            *(u32 *)(scratch+0xa) = v;
            esi = cm_fn_0068d1b0((u32)scratch);
            while (esi) {
                i = R(esi+4);
                v = R(edi);
                if (i != v) {
                    base = R(0xae23b4);
                    p = R(base+i*0x245u+0x53);
                    if (p && cm_fn_00544670(p)) {
                        i = R(esi+4);
                        base = R(0xae23b4);
                        p = base+i*0x245u;
                        receiver = R(0xb7431c);
                        cm_fn_007861e0((void *)receiver,(u32)(scratch+0x10),p);
                    }
                }
                esi = cm_fn_0068d1b0((u32)scratch);
            }
            *(volatile u8 *)&frame.state = 0;
            cm_fn_0068d040((u32)scratch);
        }
    }
    prev = frame.prev;
    CM_FS(u32,0) = prev;
}
