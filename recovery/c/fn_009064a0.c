#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_00785830(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_009064a0(u32 person,u32 type)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x2e0];
    u32 eax, p, v;
    frame.state = 0xffffffff;
    frame.handler = 0x9652ab;
    frame.prev = CM_FS(u32,0);
    CM_FS(u32,0) = (u32)&frame;
    eax = cm_fn_0077bbe0(scratch,0,0);
    frame.state = 0;
    if (person) {
        eax = *(volatile u32 *)(person + 0x39);
        if (eax) {
            eax = cm_fn_00786b70(CM_GLOBAL(volatile u32,0xb7431c),person,type,(u32)scratch);
            if (!eax) {
                cm_fn_00944cff(0xaa1b90,0,0,scratch+0x1e0,scratch+0xe0);
                v = cm_fn_0090d130();
                cm_fn_00944c9f(0xae24d0,0x9870e8,v,scratch+0x1e0,scratch+0xe0,0x45);
                eax = cm_fn_005e8290(0x9870e0,0xae24d0,0);
                CM_GLOBAL(u32,0xb67a34) = 0;
            } else {
                eax = cm_fn_007fe600(0xb74450,0x906610,0x906830,1,0x5a8f60,0);
                if (eax) {
                    p = *(volatile u32 *)person;
                    cm_fn_007ff270(0xb74450,0,p,0);
                    v = cm_fn_00785830(scratch,1,0);
                    cm_fn_007ff270(0xb74450,1,v,0);
                    p = *(volatile u32 *)(person+0x39);
                    v = *(volatile u32 *)p;
                    cm_fn_007ff270(0xb74450,2,v,0);
                    eax = cm_fn_007ff270(0xb74450,3,type,0);
                }
            }
        }
    }
    CM_FS(u32,0) = frame.prev;
    return eax;
}
