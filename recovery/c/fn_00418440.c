#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (this):
   0x00 pointer to a 14-byte record (cleared after release), 0x04 and 0x08
   record pointers, 0x0c pointer released through 0x009452ca. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern u32 CM_THISCALL cm_fn_00418670(u32 *param_1, int param_2);
/* 0x0068d020 returns its ECX argument in EAX (mov eax, ecx), so the caller
   keeps the pointer it passed; the printed void prototype is kept. */
extern void CM_FASTCALL cm_fn_0068d020(u16 *param_1);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_00418440(u32 self, u32 arg1)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 value;
    u32 global;

    seh.state = -1;
    seh.handler = 0x00953e42u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    OBJ_U32(0x0c) = 0;
    OBJ_U32(0x00) = 0;
    OBJ_U32(0x04) = 0;
    OBJ_U32(0x08) = 0;

    if (arg1 == 0) {
        goto zero_arg;
    }
    if ((u8)cm_fn_00418670((u32 *)self, (int)arg1) != 0) {
        goto exit;
    }

    value = OBJ_U32(0x0c);
    if (value != 0) {
        cm_fn_009452ca(value);
        OBJ_U32(0x0c) = 0;
    }
    value = OBJ_U32(0x00);
    if (value != 0) {
        cm_fn_0068d040((int)value);
        cm_fn_00944c94((void *)value);
        OBJ_U32(0x00) = 0;
    }

    global = CM_GLOBAL(volatile u32, 0x00ae2364);
    value = (u32)cm_fn_00945501(global, 1u);
    OBJ_U32(0x0c) = value;
    value = (u32)cm_fn_00944cf1(0xeu);
    seh.state = 0;
    if (value != 0) {
        cm_fn_0068d020((u16 *)value);
    }

    seh.state = -1;
    OBJ_U32(0x00) = value;
    value = (u32)cm_fn_00944cf1(0xeu);
    seh.state = 1;
    if (value != 0) {
        cm_fn_0068d020((u16 *)value);
    }

    seh.state = -1;
    OBJ_U32(0x04) = value;
    value = (u32)cm_fn_00944cf1(0xeu);
    seh.state = 2;
    if (value != 0) {
        cm_fn_0068d020((u16 *)value);
    }
    OBJ_U32(0x08) = value;
    goto exit;

zero_arg:
    global = CM_GLOBAL(volatile u32, 0x00ae2364);
    value = (u32)cm_fn_00945501(global, 1u);
    OBJ_U32(0x0c) = value;
    value = (u32)cm_fn_00944cf1(0xeu);
    seh.state = 3;
    if (value != 0) {
        cm_fn_0068d020((u16 *)value);
    }

    seh.state = -1;
    OBJ_U32(0x00) = value;
    value = (u32)cm_fn_00944cf1(0xeu);
    seh.state = 4;
    if (value != 0) {
        cm_fn_0068d020((u16 *)value);
    }

    seh.state = -1;
    OBJ_U32(0x04) = value;
    value = (u32)cm_fn_00944cf1(0xeu);
    seh.state = 5;
    if (value != 0) {
        cm_fn_0068d020((u16 *)value);
    }
    OBJ_U32(0x08) = value;

exit:
    CM_FS(u32, 0) = seh.prev;
    return self;
}
