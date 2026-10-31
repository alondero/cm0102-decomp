#include "cmre.h"

/* Object (ECX) fields by byte offset: 0x28 dword gates the work and is the second
   argument of 0x00946464; 0x2c dword is its third argument. Stack arguments by
   offset from the entry stack pointer: arg_04, arg_08, arg_0c (ret 0xc). */
#define OBJ_U32(self, off) (*(volatile u32 *)((self) + (off)))
#define ARG_PTR_U32(p) (*(volatile u32 *)(p))

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_CDECL cm_fn_00946464(void *param_1, u32 param_2, u32 param_3, s32 param_4, void *param_5);

/* Returns EAX as the listing leaves it: the gate value (0 on the early return),
   or the 0x00946464 result on the call path. */
u32 CM_THISCALL cm_fn_004ca2a0(u32 self, u32 arg_04, u32 arg_08, u32 arg_0c)
{
    /* Local frame of 0x18 bytes: dwords at 0x00, 0x04, 0x08 are filled from the
       three pointer arguments; 0x0c is the scratch area passed in ECX to 0x0054a110. */
    u32 frame[6];
    u32 gate;

    cm_fn_0054a110((u16 *)&frame[3]);

    gate = OBJ_U32(self, 0x28);
    if (gate == 0u) {
        return gate;
    }

    frame[0] = ARG_PTR_U32(arg_04);
    frame[1] = ARG_PTR_U32(arg_0c);
    frame[2] = ARG_PTR_U32(arg_08);

    return cm_fn_00946464((void *)frame, gate, OBJ_U32(self, 0x2c), 0x15,
                          (void *)&CM_GLOBAL(u8, 0x004dc970u));
}
