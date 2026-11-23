/* Recovery of preferred-image VA 0x005d8e10 (image base 0x00400000) from the
   pinned CM0102 3.9.68 primary executable,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Destructor-shaped routine: 201 bytes, `ret` with no stack cleanup, ECX read
   before written (`005d8e25 push ecx`, `005d8e27 mov esi, ecx`), so `this`
   arrives in ECX and the body is thiscall.

   SEH: 005d8e10 push -1 (state), 005d8e12 push 0x95a558 (handler),
   005d8e17/1e link fs:[0] to the three-dword record, 005d8e3a writes state = 0,
   005d8ebb writes state = -1 before the base cleanup call, and 005d8ec8 restores
   fs:[0] from the saved predecessor before the 0x10-byte frame pop.

   Field reloads: the listing issues a fresh DWORD/HALF load for every property
   use (0x005d8e34 +0xa7, 0x005d8e62 +0xad, 0x005d8e74 +0xad, 0x005d8e8a +0x3c,
   0x005d8e93 +0xad, 0x005d8ea2 +8), including the reloads that follow the free
   calls, so each is written as a volatile access at its own listing site.

   Receivers at the two ECX calls are explicit: 0x005d8ea9 `mov ecx, edi` with
   0x005d8eab `call 0x49f450` passes the child pointer, and 0x005d8eb9
   `mov ecx, esi` with 0x005d8ec3 `call 0x518690` passes `this`. Those calls take
   no stack arguments (the `push edi` at 0x005d8eb0 belongs to 0x944c94).

   The pushed ECX slot at 0x005d8e25, written at 0x005d8e2a with `this`, is never
   read by the routine, so it carries no observable value. */
#include "cmre.h"

/* 0x005d8eab: `mov ecx, edi`; ECX carries the arg, nothing is pushed. */
extern void CM_FASTCALL cm_fn_0049f450(u32 self);
/* 0x005d8ec3: `mov ecx, esi`; thiscall base/cleanup call, no stack arguments. */
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
/* 0x005d8eb1: cdecl, one pointer pushed at 0x005d8eb0, cleaned by `add esp, 4`.
   EAX/EDX are never read afterwards. */
extern void CM_CDECL cm_fn_00944c94(void *arg4);
/* 0x005d8e47/5a/82/9a: cdecl, one pointer pushed, cleaned by `add esp, 4`.
   EAX/EDX are never read afterwards. */
extern void CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_005d8e10(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095a558u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    CM_GLOBAL(volatile u32, self) = 0x0096b044u;
    u32 value = CM_GLOBAL(volatile u32, self + 0xa7u);
    frame.state = 0;
    if (value != 0)
        cm_fn_009452ca(value);

    value = CM_GLOBAL(volatile u32, self + 0xa3u);
    if (value != 0)
        cm_fn_009452ca(value);

    u32 array = CM_GLOBAL(volatile u32, self + 0xadu);
    if (array != 0) {
        u32 index = 0;
        s16 initial_count = CM_GLOBAL(volatile s16, self + 0x3cu);
        if (initial_count > 0) {
            do {
                array = CM_GLOBAL(volatile u32, self + 0xadu);
                value = CM_GLOBAL(volatile u32, array + index * 4u);
                if (value != 0)
                    cm_fn_009452ca(value);
                s16 count = CM_GLOBAL(volatile s16, self + 0x3cu);
                index++;
                if ((s32)index >= count)
                    break;
            } while (1);
        }
        array = CM_GLOBAL(volatile u32, self + 0xadu);
        cm_fn_009452ca(array);
    }

    u32 child = CM_GLOBAL(volatile u32, self + 8u);
    if (child != 0) {
        cm_fn_0049f450(child);
        cm_fn_00944c94((void *)child);
    }

    frame.state = 0xffffffffu;
    cm_fn_00518690((void *)self);
    u32 restore_prev = frame.prev;
    CM_FS(u32, 0) = restore_prev;
}
