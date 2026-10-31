#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x00 tested for zero on entry, then passed to 0x00946464 as its second argument;
   0x04 passed to 0x00946464 as its third argument. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_CDECL cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4, void *param_5);

/* Two stack arguments (ret 8): arg1 is a pointer to a u32 value, arg2 is a
   plain u32 that is forwarded to 0x00417c10. The return value is 0 or 1
   (the original builds it with neg/sbb/inc from 0x00946464's result). */
u32 CM_THISCALL cm_fn_0085ddb0(u32 self, u32 arg1, u32 arg2)
{
    /* Scratch frame from the original's local area L (esp after sub esp, 0x14):
       frame[0] (L+0x00) is the pointer passed to 0x00417c10 (uninitialized here),
       frame[2] (L+0x08) holds the copy of *arg1,
       frame[3] (L+0x0c) is the object passed in ECX to 0x0054a110 and 0x00417c10. */
    u32 frame[5];
    u32 field4;
    u32 field0;
    u32 status;

    cm_fn_0054a110((u16 *)&frame[3]);
    if (OBJ_U32(0x00) == 0) {
        return 1u;
    }

    frame[2] = *(volatile u32 *)arg1;
    cm_fn_00417c10((u32 *)&frame[3], &frame[0], (u16 *)arg2);

    field4 = OBJ_U32(0x04);
    field0 = OBJ_U32(0x00);
    status = cm_fn_00946464((u32)&frame[2], field0, field4, 12, (void *)0x0085e380u);
    return (u32)(status == 0);
}
