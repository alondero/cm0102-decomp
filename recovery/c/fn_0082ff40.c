#include "cmre.h"

/* WIP: O0/O2 events mismatch, case0/index16, original CRT consumes
   unwritten Find-output FILETIMEs at00946985. One failed invocation.
   Keep default600/VAseed/200000; require original-derived output proof. */
/* Preferred-image VA0082FF40/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL1/RET0/full EAX1. Every guarded teardown retains its original
   load, ECX receiver, original destructor, delete and global-clear order.
   Some empty callees ignore ECX; it remains explicit at those boundaries. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define DTOR(va) extern void CM_THISCALL cm_fn_##va(u32)
DTOR(007ff5f0); DTOR(007fc5e0); DTOR(006b4190); DTOR(0072b200);
DTOR(005af6a0); DTOR(0068e0e0); DTOR(00442e20); DTOR(004448a0);
DTOR(008961e0); DTOR(00555790); DTOR(008b1070); DTOR(00413410);
DTOR(008bbe20); DTOR(0062a8a0); DTOR(005a5120); DTOR(004185f0);
DTOR(005698d0); DTOR(00794ba0); DTOR(0085b7f0); DTOR(0076a710);
DTOR(0090d2e0); DTOR(00903570); DTOR(005a8f60); DTOR(008687c0);
DTOR(007293c0); DTOR(007c0d00); DTOR(0058e330); DTOR(005aa0d0);
DTOR(005fb730); DTOR(007f6aa0); DTOR(00785d00); DTOR(004c74f0);
DTOR(0059a5a0); DTOR(007a56f0); DTOR(0079b800); DTOR(0084be10);
DTOR(009227d0); DTOR(007da690);
extern void CM_CDECL cm_fn_006d5b50(void);
extern void CM_CDECL cm_fn_00669290(void);
extern void CM_CDECL cm_fn_00944c94(u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_0090cd50(u32);
extern void CM_CDECL cm_fn_00528080(void);
extern void CM_THISCALL cm_fn_0054a130(u32, u32, u32, u32, u32);

typedef void (CM_THISCALL *destructor)(u32);
typedef void (CM_THISCALL *deleting_destructor)(u32, u32);

static void destroy_object(u32 global, destructor destroy)
{
    u32 object = D(global);
    if (object != 0) {
        destroy(object);
        cm_fn_00944c94(object);
        D(global) = 0;
    }
}

static void release_array(u32 global)
{
    u32 array = D(global);
    if (array != 0) {
        cm_fn_009452ca(array);
        D(global) = 0;
    }
}

static void destroy_virtual(u32 global)
{
    u32 object = D(global), vtable, target;
    if (object != 0) {
        vtable = D(object);
        target = D(vtable);
        ((deleting_destructor)target)(object, 1);
        D(global) = 0;
    }
}

u32 CM_CDECL cm_fn_0082ff40(volatile u32 reset_window)
{
    u32 object;
    cm_fn_007ff5f0(0xb74450);
    if (reset_window != 0) cm_fn_007fc5e0(0xb74450);
    cm_fn_006d5b50();
    destroy_object(0xb67a50, cm_fn_006b4190);
    object = D(0xadadfc);
    if (object != 0) {
        cm_fn_009452ca(object);
        D(0xadadfc) = 0;
        D(0xadadf8) = 0;
    }
    cm_fn_00669290();
    destroy_object(0xb6b7c8, cm_fn_0072b200);
    destroy_object(0xae2a60, cm_fn_005af6a0);
    destroy_object(0xb67a44, cm_fn_0068e0e0);
    destroy_object(0xad9da8, cm_fn_00442e20);
    destroy_object(0xad9dac, cm_fn_004448a0);
    destroy_object(0xde1d08, cm_fn_008961e0);
    destroy_object(0xae28f0, cm_fn_00555790);
    destroy_object(0xde1f44, cm_fn_008b1070);
    destroy_object(0xad9c60, cm_fn_00413410);
    destroy_object(0xde1f54, cm_fn_008bbe20);
    destroy_object(0xb63d28, cm_fn_0062a8a0);
    destroy_object(0xae2a3c, cm_fn_005a5120);
    destroy_object(0xad9c6c, cm_fn_004185f0);
    destroy_object(0xae2a30, cm_fn_005698d0);
    destroy_object(0xb7433c, cm_fn_00794ba0);
    destroy_object(0xdd7ef4, cm_fn_0085b7f0);
    destroy_virtual(0xae2c74);
    destroy_object(0xb74318, cm_fn_0076a710);
    destroy_object(0xde2f70, cm_fn_0090d2e0);
    destroy_object(0xde1f58, cm_fn_00903570);
    destroy_object(0xae1000, cm_fn_005a8f60);
    destroy_object(0xdd7f00, cm_fn_008687c0);
    destroy_object(0xb6b7c0, cm_fn_007293c0);
    destroy_object(0xb74350, cm_fn_007c0d00);
    destroy_object(0xb5e134, cm_fn_0058e330);
    destroy_object(0xae2a58, cm_fn_005aa0d0);
    destroy_object(0xb63c98, cm_fn_005fb730);
    destroy_object(0xb74448, cm_fn_007f6aa0);
    destroy_object(0xb7431c, cm_fn_00785d00);
    object = D(0xdd7ef0);
    if (object != 0) {
        cm_fn_00944c94(object);
        D(0xdd7ef0) = 0;
    }
    destroy_virtual(0xb74434);
    destroy_object(0xae1050, cm_fn_004c74f0);
    destroy_object(0xae2a38, cm_fn_0059a5a0);
    release_array(0xae2d0c);
    release_array(0xae2d14);
    release_array(0xae2d1c);
    release_array(0xae2d20);
    release_array(0xae2d24);
    destroy_object(0xb7434c, cm_fn_007a56f0);
    destroy_object(0xb74340, cm_fn_0079b800);
    destroy_object(0xdd7ee4, cm_fn_0084be10);
    destroy_object(0xde2f74, cm_fn_009227d0);
    destroy_object(0xb74430, cm_fn_007da690);
    cm_fn_0090cd50(0xa813ac);
    release_array(0xae2338);
    cm_fn_00528080();
    cm_fn_0054a130(0xae2c90, 1, 0, 2001, 0xffffffffu);
    B(0xae2c88) = 0;
    D(0xdd711c) = 0;
    D(0xdd7130) = 0;
    return 1;
}
