/* WIP: the registration record stores the original machine ESP and EBP at
   offsets +0x1c and +0x20. Plain C does not expose those register values, so
   this source leaves the fields at their earlier zero stores. */

#include "cmre.h"

typedef u32 (CM_CDECL *cm_callback_t)(u32 first, void *locals);
struct cm_registration {
    void *prev;
    u32 handler;
    u32 arg5;
    u32 arg2;
    u32 arg6;
    u32 arg7;
    u32 scope_handler;
    u32 saved_esp;
    u32 saved_ebp;
    u32 state;
};
struct cm_callback_locals { u32 arg1; u32 arg3; };

u32 CM_CDECL cm_fn_00945038(u32 arg1, u32 arg2, u32 arg3, u32 unused_arg4,
                            u32 arg5, u32 arg6, u32 arg7)
{
    volatile struct cm_registration frame;
    volatile struct cm_callback_locals locals;
    volatile u32 result;
    u32 first;
    cm_callback_t callback;

    frame.prev = (void *)0;
    frame.handler = 0x009450ecu;
    frame.arg5 = arg5;
    frame.arg2 = arg2;
    frame.arg6 = arg6;
    frame.arg7 = arg7;
    frame.scope_handler = 0;
    frame.saved_esp = 0;
    frame.saved_ebp = 0;
    frame.state = 0;
    frame.scope_handler = 0x009450beu;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    result = 1;
    locals.arg1 = arg1;
    locals.arg3 = arg3;
    first = *(volatile u32 *)(u32)arg1;
    callback = (cm_callback_t)CM_GLOBAL(volatile u32, 0x00de301c);
    (void)callback(first, (void *)&locals);
    result = 0;

    if (frame.state != 0) {
        void *head = CM_FS(void *, 0);
        void *head_prev = *(void * volatile *)head;
        void *prev = frame.prev;
        *(void * volatile *)prev = head_prev;
        CM_FS(void *, 0) = prev;
    } else {
        CM_FS(void *, 0) = frame.prev;
    }
    (void)unused_arg4;
    return result;
}
