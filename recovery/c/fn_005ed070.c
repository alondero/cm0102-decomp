#include "cmre.h"
/*
 * Preferred-image VAs below refer to CM0102 3.9.68, SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * 005ed0a2..005ed0dc: eleven stack arguments, ECX receiver.
 * 005ed12c..005ed149: repeated name/key/key triples, zero name terminates.
 * 005ed175..005ed1ce: eight stack arguments, ECX local-object receiver.
 * The 004ae8a0 scratch-tail transport hold is NOT resolved by this source:
 * that callee remains an original boundary; no padding is initialized here.
 * Full caller/transport and pending UI contract validation remain required.
 */
extern u32 CM_THISCALL cm_fn_004ae590(u32, u32);
extern void CM_THISCALL cm_fn_004ae630(u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, const char *, u32, u32);
extern u32 CM_THISCALL cm_fn_0055fba0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern s32 cm_fn_005e5ad0(u32, const char *);
extern u32 *cm_fn_00946540(u32 *, char);
extern u32 CM_THISCALL cm_fn_004aeaf0(int *,u16,int,int,int *,u16,u16,u16,u8);

u32 cm_fn_005ed070(u32 p1,u32 p2,u32 p3,u32 p4,u32 p5,
                   volatile u32 p6,u32 p7,u32 p8,u32 p9,u32 p10,...)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 local[3];
    u32 obj = (u32)&local[1];
    u32 left, right, text, font, name, a, b, mode, last, result;
    s32 width;
    __builtin_va_list ap;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095ab98;
    frame.prev = CM_FS(u32,0);
    CM_FS(u32,0) = (u32)&frame;
    cm_fn_004ae590(obj,0);
    left = p1;
    right = p3;
    frame.state = 0;
    result = cm_fn_0055fba0(
        CM_GLOBAL(volatile u32,0x00b74c4c +
                  (u32)(s32)CM_GLOBAL(volatile s16,0x00b77c7a)*768u),
        left,p2,right,p4,1,0,p7,0,p5,p6,0xffffffffu);
    text = p10;
    p6 = result;
    if (!text) {
        frame.state = 0xffffffffu;
        cm_fn_004ae630(obj);
        CM_FS(u32,0) = frame.prev;
        return 1;
    }
    cm_fn_004ae660((void *)obj,text);
    __builtin_va_start(ap,p10);
    name = __builtin_va_arg(ap,u32);
    while (name) {
        a = __builtin_va_arg(ap,u32);
        b = __builtin_va_arg(ap,u32);
        cm_fn_004ae8a0(obj,(const char *)name,a,b);
        name = __builtin_va_arg(ap,u32);
    }
    __builtin_va_end(ap);
    font = p8;
    width = cm_fn_005e5ad0(font,(const char *)text);
    if (width <= (s32)(right-left-9u) &&
        !cm_fn_00946540((u32 *)text,10)) {
        local[0] = (u32)((s32)(p7-1u)/2);
        mode = 1;
        last = 0;
    } else {
        local[0] = 0;
        mode = 0;
        last = p7;
    }
    cm_fn_004aeaf0((int *)obj,(u16)p6,(int)(left+5u),
                   (int)(right-5u),(int *)&local[0],(u16)font,
                   (u16)p9,(u16)mode,(u8)last);
    frame.state = 0xffffffffu;
    cm_fn_004ae630(obj);
    CM_FS(u32,0) = frame.prev;
    return 1;
}
