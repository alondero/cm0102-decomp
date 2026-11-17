/* fn_005a8090 - preferred-image VA 0x005a8090, image base 0x00400000.
   CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Prologue resolution (held concern, 0x005a8090/0x005a8095):
     mov eax,0x16c0 ; call 0x00945610  is MSVC's stack probe helper.  The helper
   itself lowers ESP by 0x16c0 before returning to the next instruction, so the
   body addresses its scratch frame from the shifted ESP: the formals are then
   read at [esp+0x16c8] (arg2, 0x005a809a), [esp+0x16d0] after ebx/ebp/esi are
   pushed (arg1, 0x005a80a4) and [esp+0x16e0]/[esp+0x16e8]/[esp+0x16dc]/[esp+0x16e8]
   after the argument pushes (arg3, 0x005a81dc / 0x005a8312 / 0x005a8363).  The
   frame is released by `add esp,0x16c0` and the three caller-owned formals by
   `ret 0xc` (0x005a8373 / 0x005a8379).  Plain C here is one 0x16c0-byte scratch
   block plus three stdcall formals: no ordinary call to the probe helper and no
   invented stack global, so the original frame size, formal homes and 12-byte
   callee cleanup are all reproduced.  Frame offsets below are the original
   `[esp+...]`-derived ones, not compiler layout: six dword locals at
   0x00/0x04/0x08/0x0c/0x10/0x14 and three 0x190-byte format buffers at 0x18,
   0x1a8 and 0x338. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output,
                                   u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third,
                                   u32 date);
/* 0x0066f4e0 is called with 4 arguments at 0x005a81cc / 0x005a830a and with 5
   arguments at 0x005a82f3 (add esp,0x10 versus add esp,0x14): a variadic,
   cdecl formatter. */
extern char *CM_CDECL cm_fn_0066f4e0(char *buffer, char *format, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

/* Return transport in the pinned image named above: preferred-image VAs
   0x005a820e/0x005a8210 and 0x005a834c/0x005a834e send zero EAX to
   0x005a836f; otherwise 0x005a836a supplies EAX. The epilogue at
   0x005a836f-0x005a8379 leaves EAX unchanged. */
u32 CM_STDCALL cm_fn_005a8090(u32 arg1, u32 arg2, u32 arg3)
{
    u32 frame[0x16c0 / 4];
    u8 *const base = (u8 *)frame;

    if (arg2 != 0) {
        u32 v0;
        u32 v1;
        u32 v2;
        u32 v9;
        u32 g;
        u32 r;

        v0 = (u32)cm_fn_00785830((void *)arg1, 0);
        v1 = (u32)cm_fn_00785830((void *)arg1, 1);
        v2 = (u32)cm_fn_00785830((void *)arg1, 2);
        frame[0] = (u32)cm_fn_00785830((void *)arg1, 3);
        frame[5] = (u32)cm_fn_00785830((void *)arg1, 4);
        frame[1] = (u32)cm_fn_00785830((void *)arg1, 5);
        frame[3] = (u32)cm_fn_00785830((void *)arg1, 6);
        frame[4] = (u32)cm_fn_00785830((void *)arg1, 7);
        frame[2] = (u32)cm_fn_00785830((void *)arg1, 8);
        v9 = (u32)cm_fn_00785830((void *)arg1, 9);

        (void)cm_fn_0053e140(v1, v2, frame[0], (u32)(base + 0x18), 0x190, 1, v0);
        (void)cm_fn_0053e140(frame[1], frame[3], frame[4],
                             (u32)(base + 0x1a8), 0x190, 1, frame[5]);

        if (frame[2] == 1) {
            char *fmt;

            if (v9 == 0) {
                fmt = (char *)0x009cbd54;
            } else if (v9 - 1 == 0) {
                fmt = (char *)0x009cbdf8;
            } else {
                fmt = (char *)0x009cbedc;
            }
            (void)cm_fn_0066f4e0((char *)(base + 0x338), fmt,
                                 (char *)(base + 0x18), (char *)(base + 0x1a8));
        } else {
            (void)cm_fn_0066f4e0((char *)(base + 0x338), (char *)0x009cbc9c,
                                 (char *)(base + 0x18), (char *)(base + 0x1a8));
        }

        /* The 64-bit result is dead: EDX is overwritten at 0x005a81e8 and EAX
           at 0x005a81eb. */
        (void)cm_fn_004ae660((void *)arg3, (u32)(base + 0x338));

        g = CM_GLOBAL(volatile u32, 0x00ae23bc);
        r = cm_fn_0053e990(110u * v0 + g, v1, v2, frame[0], 0);
        if (r != 0) {
            u32 g2;

            g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
            return cm_fn_004ae8a0((int)arg3, (u8 *)(base + 0x18), 0x7da,
                                 (int)(110u * v0 + g2));
        }
    } else {
        u32 v0;
        u32 v1;
        u32 v2;
        u32 v9;
        u32 g;
        u32 r;

        v0 = (u32)cm_fn_00785830((void *)arg1, 0);
        v1 = (u32)cm_fn_00785830((void *)arg1, 1);
        v2 = (u32)cm_fn_00785830((void *)arg1, 2);
        frame[0] = (u32)cm_fn_00785830((void *)arg1, 3);
        /* Result of index 10 (0x005a825b) is not stored: 0x005a8264 clobbers EAX. */
        (void)cm_fn_00785830((void *)arg1, 10);
        frame[3] = (u32)cm_fn_00785830((void *)arg1, 11);
        frame[1] = (u32)cm_fn_00785830((void *)arg1, 12);
        frame[2] = (u32)cm_fn_00785830((void *)arg1, 13);
        v9 = (u32)cm_fn_00785830((void *)arg1, 14);

        (void)cm_fn_0053e140(v1, v2, frame[0], (u32)(base + 0x18), 0x190, 5, v0);
        (void)cm_fn_0053e140(frame[3], frame[1], frame[2],
                             (u32)(base + 0x1a8), 0x190, 5, v0);

        /* Formatter argument order in the v9 == 1 branch is fixed by the
           original push sequence at preferred-image VAs (pinned binary
           SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86):
           0x005a82d2 lea edx,[esp+0x1b8] (second buffer, base+0x1a8) and
           0x005a82d9 lea eax,[esp+0x28] (first buffer, base+0x18); then
           0x005a82dd cmp esi,1 / 0x005a82e0 push edx / 0x005a82e1 push eax /
           0x005a82e2 jne.  On equality: 0x005a82e4 push 0x64,
           0x005a82e6 lea ecx,[esp+0x354] (destination, base+0x338),
           0x005a82ed push 0x9cbc44 (format), 0x005a82f2 push ecx,
           0x005a82f3 call 0x66f4e0, 0x005a82f8 add esp,0x14 (five arguments).
           cdecl pushes in reverse, so the call is
           destination, format, 0x64, first buffer, second buffer: 0x64 is the
           first variadic argument, not the last. */
        if (v9 == 1) {
            (void)cm_fn_0066f4e0((char *)(base + 0x338), (char *)0x009cbc44,
                                 0x64, (char *)(base + 0x18),
                                 (char *)(base + 0x1a8));
        } else {
            (void)cm_fn_0066f4e0((char *)(base + 0x338), (char *)0x009cbbe4,
                                 (char *)(base + 0x18), (char *)(base + 0x1a8));
        }

        (void)cm_fn_004ae660((void *)arg3, (u32)(base + 0x338));

        g = CM_GLOBAL(volatile u32, 0x00ae23bc);
        r = cm_fn_0053e990(110u * v0 + g, v1, v2, frame[0], 0);
        if (r != 0) {
            u32 g2;

            g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
            return cm_fn_004ae8a0((int)arg3, (u8 *)(base + 0x18), 0x7da,
                                 (int)(110u * v0 + g2));
        }
    }
    return 0;
}
