/* fn_0072b5f0, preferred-image VA 0x0072b5f0 (image base 0x00400000), 535 bytes,
   e:\dev\cm3\cm3 00-01\cm3\code\media.cpp.
   Pinned binary: CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   ABI evidence (preferred-image VAs, pinned SHA above):
   - 0072b5f0 mov eax,[esp+4] reads the one stack argument; 0072b601 mov edi,ecx
     reads the incoming ECX; 0072b7e9 / 0072b804 ret 4 pops that stack argument
     -> thiscall entry with one int stack argument and a four-byte callee pop.
   - 0072b7de xor al,al / 0072b7f9 mov al,1 define only the low byte of EAX,
     so the declared result width is u8.
   - 0072b619/.../0072b7b0 cdecl pushes with deferred single add esp,0x24 at
     0072b7d5: the three pushes at 0072b62c/631/0072b7af are consumed by the
     variadic 0x944c9f call, not by the (void) 0x90d130 call (legacy signature). */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define S_00a17bec ((u32)0x00a17bec)
#define S_00a17c14 ((const char *)0x00a17c14)
#define S_009870e0 ((const char *)0x009870e0)
#define S_009870e8 ((u32)0x009870e8)
#define LOAD32(addr) (*(volatile u32 *)(addr))

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char * cm_fn_0090d130(void);
extern int * cm_fn_009326e0(u32 param_1, u8 *param_2, int param_3, int param_4, u32 param_5);
extern void cm_fn_00932b00(int *param_1);
extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u8 CM_THISCALL cm_fn_0072b5f0(u32 *param_1, int param_2)
{
  /* L = ESP after `sub esp,0x204` at 0072b5f4; saved ebx/esi/edi occupy L-0xc..L.
     [esp+0x0c] = L+0 local_204, [esp+0x10] = L+4 local_200,
     [esp+0x110] = L+0x104 local_100. */
  u32 local_204;
  u8 scratch[512];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  u32 tail_a;   /* second push before the 0x0090d130 call */
  u32 tail_b;   /* third (last) push before the 0x0090d130 call */
  u32 code;     /* first push before the 0x0090d130 call */
  u32 r;

  if (param_2 == 0) {
    /* 0072b605..0072b632: lea [esp+0x10]=local_200, lea [esp+0x110]=local_100 */
    cm_fn_00944cff(S_00a17bec, 0, 0, local_100, local_200);
    tail_a = (u32)local_200;
    tail_b = (u32)local_100;
    code = 0x112;
  } else {
    /* 0072b637..0072b648: push 4; push 0x16; push 1; push 0xa17c14; push eax */
    local_204 = (u32)cm_fn_009326e0((u32)param_2, (u8 *)S_00a17c14, 1, 0x16, 4);
    if (local_204 == 0) {
      /* 0072b653..0072b680 */
      cm_fn_00944cff(S_00a17bec, 0, 0, local_200, local_100);
      tail_a = (u32)local_100;
      tail_b = (u32)local_200;
      code = 0x11a;
    } else {
      /* 0072b687 lea esi,[edi+0x14]; 0072b68f call 0x932e10 */
      r = cm_fn_00932e10((u32)((char *)param_1 + 0x14), 4, 1, local_204);
      if (r != 1) {
        /* 0072b69c..0072b6ca */
        cm_fn_00944cff(S_00a17bec, 0, 0, local_200, local_100);
        tail_a = (u32)local_100;
        tail_b = (u32)local_200;
        code = 0x121;
      } else {
        /* 0072b6d3/0072b6d5 load [esi] and [edi+0xc]; 0072b6dd call;
           0072b6e2 reloads [esi] before the compare at 0072b6e7. */
        r = cm_fn_00932e10(LOAD32((char *)param_1 + 0x0c), 0x19,
                           LOAD32((char *)param_1 + 0x14), local_204);
        if (r != LOAD32((char *)param_1 + 0x14)) {
          /* 0072b6eb..0072b718 */
          cm_fn_00944cff(S_00a17bec, 0, 0, local_200, local_100);
          tail_a = (u32)local_100;
          tail_b = (u32)local_200;
          code = 0x129;
        } else {
          /* 0072b721 lea esi,[edi+8]; 0072b72a call 0x932e10 */
          r = cm_fn_00932e10((u32)((char *)param_1 + 0x08), 4, 1, local_204);
          if (r != 1) {
            /* 0072b737..0072b764 */
            cm_fn_00944cff(S_00a17bec, 0, 0, local_200, local_100);
            tail_a = (u32)local_100;
            tail_b = (u32)local_200;
            code = 0x130;
          } else {
            /* 0072b76b/0072b76d load [esi]=[edi+8] and [edi]; 0072b774 call;
               0072b779 reloads [esi] before the compare at 0072b77e. */
            r = cm_fn_00932e10(LOAD32(param_1), 0x17,
                               LOAD32((char *)param_1 + 0x08), local_204);
            if (r != LOAD32((char *)param_1 + 0x08)) {
              /* 0072b782..0072b7ae */
              cm_fn_00944cff(S_00a17bec, 0, 0, local_200, local_100);
              tail_a = (u32)local_100;
              tail_b = (u32)local_200;
              code = 0x138;
            } else {
              /* 0072b7ec..0072b804: lea ecx,[esp+0xc]; push ecx; call 0x932b00 */
              cm_fn_00932b00((int *)&local_204);
              return 1;
            }
          }
        }
      }
    }
  }

  /* Shared tail, listing order 0072b7af..0072b7e9.  The value returned by
     0x90d130 in EAX is the third variadic argument of 0x944c9f. */
  cm_fn_00944c9f((u32)&DAT_00ae24d0, S_009870e8,
                 (u32)cm_fn_0090d130(), tail_b, tail_a, code);
  cm_fn_005e8290(S_009870e0, (const char *)&DAT_00ae24d0, 0);
  DAT_00b67a34 = 0;   /* 0072b7d8 mov [0xb67a34],ebx with ebx == 0 */
  return 0;
}
