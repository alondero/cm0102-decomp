/* fn_005c29b0 - preferred-image VA 0x005c29b0, image base 0x00400000.
   Translation of the 301-byte original routine.  Every statement below is
   checked against the listing in the bundle brief (bundle/fn_005c29b0.md).

   Frame layout of the original (entry ESP = X):
     X-4  : SEH state   (push -1)
     X-8  : SEH handler (push 0x959de8)
     X-12 : SEH prev    (push eax)          <- fs:[0] is linked to X-12
     X-36 : L  (24-byte local area, "sub esp,0x18")
     X-40 : saved ebx
     X-44 : saved esi
     X-48 : saved edi
   Local area L+0x00/L+0x04 : the 8-byte pair copied from the second argument
   L+0x08 : 8-byte buffer handed to cm_fn_0054c8e0
   L+0x10 : 8-byte buffer handed to cm_fn_0054c5b0
   (the two buffers are 8 bytes apart and the pair sits 8 bytes below the
   first one, so the whole 24 bytes is kept in one scratch allocation).
*/
#include "cmre.h"

extern short *CM_THISCALL cm_fn_0054c8e0(short *param_1, u32 *param_2, int param_3);
extern short *CM_THISCALL cm_fn_0054c5b0(short *param_1, u32 *param_2, int param_3);

int CM_THISCALL cm_fn_005c29b0(volatile u8 *self, u32 arg1, volatile u32 *arg2)
{
    struct seh_frame { u32 prev, handler, state; } volatile frame;
    volatile u32 scratch[6];

    /* 005c29b0 mov eax,fs:[0] / push -1 / push 0x959de8 / push eax */
    {
        u32 prev = CM_FS(u32, 0);
        frame.state = 0xffffffffu;
        frame.handler = 0x00959de8u;
        frame.prev = prev;
    }
    /* 005c29c2 mov fs:[0],esp */
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    /* 005c29c9 mov edx,[ecx+4] / 005c29cf cmp eax,edx / 005c29d4 je */
    if (arg1 != *(volatile u32 *)(self + 4)) {
        /* 005c29d6 mov edx,[0xae2378] / 005c29dc mov esi,[eax] */
        s32 limit = (s32)(*(volatile u32 *)0x00ae2378u - 0x7fu);
        s32 value = *(volatile s32 *)arg1;
        /* 005c29e1 cmp esi,edx / 005c29e3 jge -> 005c29fe */
        if (value < limit) {
            /* 005c29e5 .. 005c29fb: restore fs:[0], return 1 */
            CM_FS(u32, 0) = frame.prev;
            return 1;
        }
    }

    /* 005c29fe mov al,[eax] / 005c2a00 mov bl,[ecx+0xaf] / 005c2a06 sub al,bl
       005c2a08 movsx edx,al  -> index * 21 * 4 + index * 21 == index * 105 */
    {
        u8 arg_byte = *(volatile u8 *)arg1;
        u8 self_byte = *(volatile u8 *)(self + 0xaf);
        s32 index = (s32)(s8)(arg_byte - self_byte);
        volatile u8 *record = self + index * 105;

        /* 005c2a1c mov ecx,[eax+0xb1] / 005c2a22 test ecx,ecx / 005c2a24 je */
        if (*(volatile u32 *)(record + 0xb1) == 0) {
            /* 005c2ac7 .. 005c2ada: restore fs:[0], return 0 */
            CM_FS(u32, 0) = frame.prev;
            return 0;
        }

        {
            u32 lo, hi;
            short *got;
            u16 asked_hi, got_hi;
            volatile u8 *out1 = (volatile u8 *)scratch + 8;   /* L+0x08 */
            volatile u8 *out2 = (volatile u8 *)scratch + 16;  /* L+0x10 */

            /* 005c2a3a mov ebx,[esi] / 005c2a3c mov edx,[esi+4]
               005c2a42 mov [esp+0x14],ebx / 005c2a46 mov [esp+0x18],edx */
            lo = arg2[0];
            hi = arg2[1];
            scratch[0] = lo;
            scratch[1] = hi;

            /* 005c2a4a call 0x54c8e0 (ECX = edi = record+0xb5) */
            got = cm_fn_0054c8e0((short *)(record + 0xb5), (u32 *)out1, 7);

            /* 005c2a4f mov cx,[eax+2] / 005c2a53 mov dx,[esp+0xe]
               005c2a5b mov dword ptr [esp+0x2c],0  (SEH state) */
            got_hi = *(volatile u16 *)((volatile u8 *)got + 2);
            asked_hi = *(volatile u16 *)((volatile u8 *)scratch + 2);
            frame.state = 0;

            /* 005c2a58 cmp dx,cx / 005c2a63 ja / 005c2a65 jb
               005c2a69 cmp bx,[eax] / 005c2a6c setg cl */
            if (asked_hi == got_hi) {
                if (!((s16)(u16)lo > *(volatile s16 *)got)) {
                    /* 005c2a73 je 005c2ac7 -> return 0 */
                    CM_FS(u32, 0) = frame.prev;
                    return 0;
                }
            } else if (asked_hi < got_hi) {
                /* 005c2a65 jb 005c2ac7 -> return 0 */
                CM_FS(u32, 0) = frame.prev;
                return 0;
            }
            /* 005c2a63 ja 005c2a75 -> fall through to the second call */

            /* 005c2a75 mov ebx,[esi] / 005c2a77 mov edx,[esi+4]
               005c2a83 mov [esp+0x14],ebx / 005c2a87 mov [esp+0x18],edx */
            lo = arg2[0];
            hi = arg2[1];
            scratch[0] = lo;
            scratch[1] = hi;

            /* 005c2a8b call 0x54c5b0 (ECX = edi = record+0xb5) */
            got = cm_fn_0054c5b0((short *)(record + 0xb5), (u32 *)out2, 7);

            /* 005c2a90 mov cx,[eax+2] / 005c2a94 mov dx,[esp+0xe] */
            got_hi = *(volatile u16 *)((volatile u8 *)got + 2);
            asked_hi = *(volatile u16 *)((volatile u8 *)scratch + 2);

            /* 005c2a99 cmp dx,cx / 005c2a9c jb / 005c2a9e ja
               005c2aa2 cmp bx,[eax] / 005c2aa5 setl cl */
            if (asked_hi == got_hi) {
                if (!((s16)(u16)lo < *(volatile s16 *)got)) {
                    /* 005c2aac je 005c2ac7 -> return 0 */
                    CM_FS(u32, 0) = frame.prev;
                    return 0;
                }
            } else if (asked_hi > got_hi) {
                /* 005c2a9e ja 005c2ac7 -> return 0 */
                CM_FS(u32, 0) = frame.prev;
                return 0;
            }
            /* 005c2a9c jb 005c2aae -> return 1 */

            CM_FS(u32, 0) = frame.prev;
            return 1;
        }
    }
}
