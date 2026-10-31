#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);
extern void CM_CDECL cm_fn_00945945(int _Code);
extern unsigned long long cm_fn_0094a640(u32 dst, u32 value, u32 count);
extern unsigned long long CM_CDECL cm_fn_0094b2a0(u32 arg);

/* Imports called through the global slots (stdcall, callee pops its arguments). */
typedef s32 (CM_STDCALL *create_process_fn)(u32 app, u32 cmd, u32 proc_attr, u32 thread_attr,
                                             s32 inherit, u32 flags, u32 env, u32 cwd,
                                             u32 startup, u32 process_info);
typedef u32 (CM_STDCALL *get_last_error_fn)(void);
typedef u32 (CM_STDCALL *wait_fn)(u32 handle, u32 timeout);
typedef u32 (CM_STDCALL *exit_code_fn)(u32 handle, u32 *code);
typedef u32 (CM_STDCALL *close_fn)(u32 handle);

/* Cdecl with four stack arguments (ret 0), returning a dword. The routine:
   - validates the mode in arg1 (signed 0..4; mode 4 also sets a flag);
   - rewrites the NUL-separated argument list at arg3 in place, turning internal NULs into
     spaces, and saves its start;
   - clears a 0x44-byte startup block, and builds an array of 5*count+4 bytes (count from
     the record table) of flags and values from the 2-level record table;
   - calls CreateProcessA through the global slot at 0x00967054 and closes or waits on the
     handles per mode. arg3 is an in/out dword, written with the result.
   One frame buffer (0x60 bytes) holds the startup block at 0x00, the 16-byte process info
   at 0x44, the scratch word at 0x32, the allocation pointer at 0x34, the saved command line
   at 0x54 and the flags dword at 0x58. The startup block and the scratch overlap as in the
   listing, so they share the one buffer. */
u32 CM_CDECL cm_fn_00951373(u32 mode, u32 app, u32 cmd_slot, u32 env)
{
    u8 frame[0x60];
    u32 slot;
    u32 p;
    u32 cur;
    u32 q;
    u32 n;
    u32 i;
    u32 idx;
    u32 rec;
    u32 blk;
    u16 size16;
    u32 flag_m4;
    u8 flag;
    s32 m;
    u32 saved;
    u32 hproc;
    u32 hthread;
    s32 created;
    u32 err;

    flag_m4 = 0;
    *(u32 *)(frame + 0x58) = 0;
    slot = cmd_slot;
    m = (s32)mode;
    if (m < 0 || m > 4) {
        CM_GLOBAL(volatile u32, 0x00de2f8c) = 0x16U;
        CM_GLOBAL(volatile u32, 0x00de2f90) = 0;
        return 0xffffffffU;
    }
    if (m == 4) {
        flag_m4 = 1;
    }

    p = slot;
    saved = p;
    *(u32 *)(frame + 0x54) = saved;
    cur = p;
    while (*(u8 *)cur != 0) {
        q = cur + 1;
        while (*(u8 *)q != 0) {
            q++;
        }
        if (*(u8 *)(q + 1) == 0) {
            cur = q;
        } else {
            *(u8 *)q = 0x20;
            cur = q + 1;
        }
    }

    cm_fn_0094a640((u32)frame, 0, 0x44U);
    *(u32 *)(frame + 0) = 0x44U;

    n = CM_GLOBAL(volatile u32, 0x00de43e0);
    if (n != 0) {
        for (;;) {
            idx = n - 1;
            rec = CM_GLOBAL(volatile u32, 0x00de42e0 + (idx >> 5) * 4) + (idx & 0x1fU) * 8;
            if (CM_GLOBAL(volatile u8, rec + 4) != 0) {
                break;
            }
            n--;
            if (n == 0) {
                break;
            }
        }
    }

    size16 = (u16)(5 * n + 4);
    *(u16 *)(frame + 0x32) = size16;
    blk = (u32)cm_fn_00945501((u32)size16, 1U);
    *(u32 *)(frame + 0x34) = blk;
    CM_GLOBAL(volatile u32, blk) = n;

    for (i = 0; i < n; i++) {
        idx = i;
        rec = CM_GLOBAL(volatile u32, 0x00de42e0 + (idx >> 5) * 4) + (idx & 0x1fU) * 8;
        flag = CM_GLOBAL(volatile u8, rec + 4);
        if ((flag & 0x10) != 0) {
            CM_GLOBAL(volatile u8, blk + 4 + i) = 0;
            CM_GLOBAL(volatile u32, blk + 4 + n + 4 * i) = 0xffffffffU;
        } else {
            CM_GLOBAL(volatile u8, blk + 4 + i) = flag;
            CM_GLOBAL(volatile u32, blk + 4 + n + 4 * i) = CM_GLOBAL(volatile u32, rec);
        }
    }

    if (flag_m4 != 0) {
        for (i = 0; i < ((n < 3) ? n : 3U); i++) {
            CM_GLOBAL(volatile u8, blk + 4 + i) = 0;
            CM_GLOBAL(volatile u32, blk + 4 + n + 4 * i) = 0xffffffffU;
        }
        *(u32 *)(frame + 0x58) = 8;
    }

    CM_GLOBAL(volatile u32, 0x00de2f8c) = 0;
    CM_GLOBAL(volatile u32, 0x00de2f90) = 0;
    created = ((create_process_fn)CM_GLOBAL(volatile u32, 0x00967054))(
        app, saved, 0, 0, 1, *(u32 *)(frame + 0x58), env, 0,
        (u32)frame, (u32)(frame + 0x44));
    err = ((get_last_error_fn)CM_GLOBAL(volatile u32, 0x009670e8))();
    cm_fn_009452ca(*(u32 *)(frame + 0x34));

    if (created == 0) {
        cm_fn_0094b2a0(err);
        return 0xffffffffU;
    }

    if (mode == 2) {
        cm_fn_00945945(0);
    }

    hproc = *(u32 *)(frame + 0x44);
    hthread = *(u32 *)(frame + 0x48);
    if (mode == 0) {
        ((wait_fn)CM_GLOBAL(volatile u32, 0x00967058))(hproc, 0xffffffffU);
        ((exit_code_fn)CM_GLOBAL(volatile u32, 0x0096705c))(hproc, &slot);
        ((close_fn)CM_GLOBAL(volatile u32, 0x009670ec))(hproc);
    } else if (mode == 4) {
        ((close_fn)CM_GLOBAL(volatile u32, 0x009670ec))(hproc);
        slot = 0;
    } else {
        slot = hproc;
    }
    ((close_fn)CM_GLOBAL(volatile u32, 0x009670ec))(hthread);
    return slot;
}
