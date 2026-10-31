#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (this). */
#define OBJ_U8(off) (*(volatile u8 *)(self + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

/* Callee at 0x005e8290: cdecl, three stack arguments, no receiver. */
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* Callee at 0x0090d130: no stack arguments; returns EAX. The three values pushed
   before its call are varargs of the following cm_fn_00944c9f call. */
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_00804f00(u32 self)
{
    /* SEH registration record; handler at 0x0096115b. */
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    /* Scratch at the local base: byte 0 holds the saved object pointer, bytes
       4..0x103 are one output buffer, bytes 0x104..0x203 the other. */
    u8 scratch[0x204];
    u32 count;
    u32 allocated;
    u32 prev_frame;
    u32 ret_0090d130;

    seh.state = -1;
    seh.handler = 0x0096115bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    CM_GLOBAL(u8, 0x00ae24d0u) = 0;
    *(volatile u32 *)scratch = self;
    OBJ_U8(0x33) = 2;
    count = CM_GLOBAL(u32, 0x00ae2364u);
    seh.state = 0;
    allocated = (u32)cm_fn_00944e46(count * 4u);
    OBJ_U32(0x34) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x00a7de94u, 0u, 0u, scratch + 0x104, scratch + 4);
        ret_0090d130 = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                       scratch + 0x104, scratch + 4, 0x33u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(u32, 0x00b67a34u) = 0;
    }
    prev_frame = seh.prev;
    OBJ_U32(0x38) = 0;
    CM_FS(u32, 0) = prev_frame;
    return self;
}
