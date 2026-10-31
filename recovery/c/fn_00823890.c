#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u8 *CM_THISCALL cm_fn_005d7ad0(u8 *record, u8 *source);
extern u32 CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Cdecl with one stack argument (arg_04 = [entry+4]), `ret 0`.
   ECX on entry is only pushed into a scratch slot and overwritten before any
   read (the allocation pointer lands there), so the routine does not take
   ECX as a receiver; the stack argument is the only input.
   The SEH frame is one volatile { prev, handler, state } record linked at
   entry and unlinked from its saved prev at exit. state is -1 at entry,
   0 after the allocation, and -1 again before the two 0x7ff270 calls.
   The first 0x7fe600 call, a thiscall with ECX = 0x00b74450 and five stack
   arguments, gates the rest. The allocation (0x31b bytes) feeds 0x5d7ad0,
   whose source is the global at 0x00ae3138. Both 0x7ff270 calls use ECX =
   0x00b74450 and are called with their listing arguments: (0, result, 0) and
   (1, arg_04, 0). Error path returns through the saved prev only. */
u32 CM_CDECL cm_fn_00823890(u32 arg_04)
{
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 alloc;
    u32 result;
    u32 eax_result = 0;

    seh.state = -1;
    seh.handler = (void *)0x0096136b;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    if (cm_fn_007fe600(0x00b74450, 0x00823950, 0x008259a0, 1, 0, 0) != 0) {
        alloc = (u32)cm_fn_00944cf1(0x31b);
        seh.state = 0;
        if (alloc != 0) {
            result = (u32)cm_fn_005d7ad0((u8 *)alloc, (u8 *)CM_GLOBAL(u32, 0x00ae3138));
        } else {
            result = 0;
        }
        seh.state = -1;
        cm_fn_007ff270(0x00b74450, 0, (char *)result, 0);
        eax_result = cm_fn_007ff270(0x00b74450, 1, (char *)arg_04, 0);
    }

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
    return eax_result;
}
