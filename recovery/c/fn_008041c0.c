#include "cmre.h"

/* Object fields by byte offset from this (ECX): 0x1ae1 + 4*k table of u32 slots
   cleared in the loop, 0x1b09, 0x361a and 0x3634 cleared at the end, 0x361e
   sub-object cleared by 0x0068d020. Element stride 0x2b0 starting at this+1 and
   at this+0x1b0e. The stack argument's dword is reused as a local: its low byte
   takes the byte-sized counter, as in the original. */

extern void CM_STDCALL cm_fn_009463af(u32 base, u32 stride, s32 count,
                                      u32 constructor, u32 destructor);
extern void CM_FASTCALL cm_fn_0068d020(u32 self);
extern u8 CM_THISCALL cm_fn_007f6e30(volatile u8 *self,
                                     const volatile u32 *index_pointer);
extern s32 CM_THISCALL cm_fn_007f6cd0(u32 unused_ecx, u32 packed_value,
                                      u32 pointer);
extern void CM_THISCALL cm_fn_007b7690(u32 self, u32 arg);

u32 CM_THISCALL cm_fn_008041c0(u32 self, u32 arg_dword)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 arg_slot;
    u32 esi;
    u32 edi;
    u32 ebp_slot;
    u32 ecx_value;
    u8 al;
    u8 bl;
    s32 result;
    int error;

    seh.state = -1;
    seh.handler = 0x00961144u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    arg_slot = arg_dword;
    esi = self;
    edi = esi + 1u;

    cm_fn_009463af(edi, 0x2b0u, 0xa, 0x007ae390u, 0x007ae4f0u);

    seh.state = 0;
    cm_fn_009463af(esi + 0x1b0eu, 0x2b0u, 0xa, 0x007ae390u, 0x007ae4f0u);

    *(volatile u8 *)&seh.state = 1;
    cm_fn_0068d020(esi + 0x361eu);

    *(volatile u8 *)&seh.state = 2;
    al = (u8)arg_slot;
    error = ((s8)al < 0) || ((s8)al >= 10);
    if (error) {
        al = cm_fn_007f6e30((volatile u8 *)CM_GLOBAL(volatile u32, 0x00b74448u),
                            (const volatile u32 *)0);
    }
    *(volatile u8 *)esi = al;

    /* byte store of zero into the argument slot's low byte */
    arg_slot = arg_slot & 0xffffff00u;
    bl = 0;
    ebp_slot = esi + 0x1ae1u;
    do {
        ecx_value = arg_slot;
        result = cm_fn_007f6cd0(CM_GLOBAL(volatile u32, 0x00b74448u), ecx_value, 0u);
        cm_fn_007b7690(edi, (u32)result);
        *(volatile u32 *)ebp_slot = 0;
        bl = (u8)(bl + 1u);
        arg_slot = (arg_slot & 0xffffff00u) | bl;
        edi = edi + 0x2b0u;
        ebp_slot = ebp_slot + 4u;
    } while (bl < 10u);

    *(volatile u32 *)(esi + 0x1b09u) = 0;
    *(volatile u32 *)(esi + 0x361au) = 0;
    *(volatile u32 *)(esi + 0x3634u) = 0;
    CM_FS(u32, 0) = seh.prev;
    return esi;
}
