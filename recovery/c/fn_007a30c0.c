/* Preferred-image VA 0x007a30c0, image base 0x00400000.
   Original binary: CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Stack trace (L = ESP just after `sub esp,0x3e8` at 007a30c0):
     push esi                       -> [L-4]  = saved esi
     push 0x9899fc / 0xa6b2e4 / 0x9b5348
                                    -> [L-8],[L-12],[L-16]
     call 0090bde0 (2 args)         -> arg1=0x9b5348, arg2=0xa6b2e4
                                       third pushed dword 0x9899fc is left
     add esp,8                      -> ESP = L-8 (0x9899fc still live)
     push eax                       -> [L-12] = path (return of 0090bde0)
     call 00945b00 (2 args)         -> arg1=path, arg2=0x9899fc (leftover)
     mov esi,eax; add esp,8         -> esi = stream
     test esi,esi; je end
     push ebx                       -> [L-8]
     mov ebx,[esp+0x3f4]            -> [L+0x3ec] = [S+4] = incoming param_1
     push edi                       -> [L-12]
     mov edi,[esp+0x3fc]            -> [L+0x3f0] = [S+8] = incoming param_2
     push edi / lea eax,[esp+0x10] / push ebx / push eax / push esi
       eax = L-16+0x10 = L = &local_3e8
     call 007a3a10 (4 args)         -> (stream, &local_3e8, param_1, param_2)
     add esp,0x10; test al,al; je done
   loop at 007a3111 repeats the same call (lea ecx,[esp+0x10] = L)
     call 007a3a10 while al != 0
   push esi; call 00945a09(1 arg); add esp,4; pop edi/ebx/esi; ret
*/
#include "cmre.h"

extern u32 cm_fn_007a3a10(u32 param_1, u32 param_2, u32 param_3, int param_4);
extern u32 cm_fn_0090bde0(u8 *arg1, u8 *arg2);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text);

void cm_fn_007a30c0(u32 param_1, u32 param_2)
{
    u8 local_3e8[1000];
    u32 path;
    volatile u32 *stream;
    u8 r;

    path = cm_fn_0090bde0((u8 *)0x009b5348, (u8 *)0x00a6b2e4);
    /* The third pushed dword 0x9899fc survives add esp,8 at 007a30db and is
       consumed as the second argument of the call at 007a30df. */
    stream = cm_fn_00945b00(path, (volatile s8 *)&CM_GLOBAL(u8, 0x009899fc));

    if (stream != (volatile u32 *)0) {
        r = (u8)cm_fn_007a3a10((u32)stream, (u32)local_3e8,
                               param_1, (int)param_2);
        while (r != 0) {
            r = (u8)cm_fn_007a3a10((u32)stream, (u32)local_3e8,
                                   param_1, (int)param_2);
        }
        cm_fn_00945a09((u32)stream);
    }
}
