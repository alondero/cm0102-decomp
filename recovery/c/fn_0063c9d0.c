#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (this). */
#define OBJ_U16(off) (*(volatile u16 *)(self + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

/* Callee at 0x005e8290: cdecl, three stack arguments, no receiver. */
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* Callee at 0x0090d130: no stack arguments; returns EAX. The three values pushed
   before its call are varargs of the following cm_fn_00944c9f call. */
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_0063c9d0(u32 self)
{
    /* Scratch at the local base: bytes 0..0xff are one output buffer, bytes
       0x100..0x1ff the other. */
    u8 scratch[0x200];
    u32 allocated;
    u32 ret_0090d130;
    u32 record;

    OBJ_U16(0x3e) = 2;
    allocated = (u32)cm_fn_00944e46(0xcu);
    OBJ_U32(0xa7) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x009ea1c8u, 0u, 0u, scratch + 0x100, scratch);
        ret_0090d130 = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                       scratch + 0x100, scratch, 0xc2u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(u32, 0x00b67a34u) = 0;
        return 0xffffffffu;
    }

    *(volatile u32 *)allocated = 0u;
    record = OBJ_U32(0xa7);
    *(volatile u32 *)(record + 6u) = 0u;
    record = OBJ_U32(0xa7);
    *(volatile u8 *)(record + 4u) = 0u;
    record = OBJ_U32(0xa7);
    *(volatile u8 *)(record + 5u) = 0u;
    record = OBJ_U32(0xa7);
    *(volatile u8 *)(record + 0xau) = 0u;
    record = OBJ_U32(0xa7);
    *(volatile u8 *)(record + 0xbu) = 0u;
    return 1u;
}
