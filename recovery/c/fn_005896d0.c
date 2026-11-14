/* fn_005896d0 - plain C reconstruction of the original routine at
   preferred-image VA 0x005896d0 (cm0102.exe 3.9.68).

   Original shape (listing in the job bundle):
     cdecl, four stack arguments at [entry_esp+4 .. entry_esp+16];
     installs the SEH frame {prev,handler,state} with `push -1 / push
     0x958c3b / push eax` and links it with `mov fs:[0], esp`, then
     reserves a 224 byte scratch object with `sub esp,0xe0`;
     guards on eight pointers, fills the object through cm_fn_00785790
     and hands it to cm_fn_007861e0.

   Stack layout of the original (entry esp = E):
     E-236 .. E-12  224 byte object  (L+0, `sub esp,0xe0`)
     E-12  .. E     SEH frame        (L+224: prev, handler, state)
   Both live in one scratch allocation here so their adjacency and the
   relative offsets survive: object at +0, frame at +224.

   Every `lea ecx,[esp+disp]` in the listing resolves to L+0:
     `lea ecx,[esp+0x14]` with two live pushed arguments (esp = E-256)
     and `lea ecx,[esp+0xc]` with none live (esp = E-248) both give
     E-236.  The object is therefore a single 224 byte buffer.

   Held obligation (resolved, verified against the gate):
     the earlier WIP kept the 224 byte object and the SEH frame in two
     separate allocations, so the `fs:[0]` link pointed into a stack page
     the original never writes; the gate reported
       mismatch: {"field":"memory","pages":["0x6e5d3000","0x7ffdf000"]}
     Reproduced here by splitting the frame out again, and cleared by the
     single allocation above (object at +0, frame at +224, matching the
     original's adjacency).
     The same WIP also read `param_1[1] + 0x33` as u32* arithmetic, i.e.
     byte offset 0xCC instead of 0x33; the gate catches that as a fault
     address mismatch (cases 1/9/12).  All six +0x33 reads below use an
     explicit (u8*) byte offset. */

#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

/* One scratch allocation: the 224 byte object the original reserves with
   `sub esp,0xe0`, followed by the SEH frame it pushed just before that. */
struct scratch_s {
  u8 obj[224];                                          /* original L+0   */
  volatile struct { u32 prev, handler, state; } frame;   /* original L+224 */
};

void cm_fn_005896d0(u32 *param_1, u32 *param_2, u32 *param_3, u32 param_4)
{
  struct scratch_s sc;
  u32 prev;
  u32 *p;
  u32 *q;
  u32 v;

  prev = CM_FS(u32, 0);            /* 0x005896d0 mov eax, fs:[0]        */
  sc.frame.state = 0xffffffffu;    /* 0x005896d6 push -1                */
  sc.frame.handler = 0x00958c3b;   /* 0x005896d8 push 0x958c3b          */
  sc.frame.prev = prev;            /* 0x005896dd push eax               */
  CM_FS(u32, 0) = (u32)(u32 *)&sc.frame; /* 0x005896de mov fs:[0], esp */

  /* ---- guards, in listing order ---- */
  if (param_1 == 0) goto done;                                   /* 0x005896f5 */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_1 + 0x61);        /* 0x005896fd */
  if (p == 0) goto done;                                         /* 0x00589702 */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_1 + 0x1a);        /* 0x00589708 */
  if (p == 0) goto done;                                         /* 0x0058970d */
  if (param_2 == 0) goto done;                                   /* 0x0058971a */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_2 + 0x53);        /* 0x00589722 */
  if (p == 0) goto done;                                         /* 0x00589727 */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_2 + 0x69);        /* 0x0058972d */
  if (p == 0) goto done;                                         /* 0x00589732 */
  q = (u32 *)(u8 *)*(volatile u32 *)((u8 *)p + 0x38);             /* 0x00589738 */
  if (q == 0) goto done;                                         /* 0x0058973d */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_2 + 0xcf);        /* 0x00589743 */
  if (p == 0) goto done;                                         /* 0x0058974b */
  if (param_3 == 0) goto done;                                   /* 0x00589758 */

  /* ---- body ---- */
  /* 0x00589760 push 0 / 0x00589762 push 0xbe1 / 0x00589767 lea ecx,[esp+0x14] */
  cm_fn_0077bbe0((void *)&sc.obj[0], 0xbe1u, 0u);

  /* 0x00589770 mov eax,[esi] .. 0x00589779 mov dword ptr [esp+0xfc],0 */
  v = *(volatile u32 *)param_1;
  sc.frame.state = 0;
  cm_fn_00785790((void *)&sc.obj[0], 0u, v);                      /* 0x00589784 */

  /* 0x00589789 mov ecx,[esi+4] / 0x0058978c mov edx,[ecx+0x33] */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_1 + 4);
  v = *(volatile u32 *)((u8 *)p + 0x33);
  cm_fn_00785790((void *)&sc.obj[0], 1u, v);                      /* 0x00589796 */

  /* 0x0058979b mov eax,[esi+8] / 0x0058979e mov ecx,[eax+0x33] */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_1 + 8);
  v = *(volatile u32 *)((u8 *)p + 0x33);
  cm_fn_00785790((void *)&sc.obj[0], 2u, v);                      /* 0x005897a8 */

  /* 0x005897ad mov edx,[esi+0xc] / 0x005897b4 mov eax,[edx+0x33] */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_1 + 0xc);
  v = *(volatile u32 *)((u8 *)p + 0x33);
  cm_fn_00785790((void *)&sc.obj[0], 3u, v);                      /* 0x005897ba */

  /* 0x005897bf mov ecx,[edi] */
  v = *(volatile u32 *)param_2;
  cm_fn_00785790((void *)&sc.obj[0], 4u, v);                      /* 0x005897c8 */

  /* 0x005897cd mov edx,[esi+0x61] / 0x005897d6 cmp word ptr [edx+0xb],0x1c52
     0x005897dc setg al / 0x005897df inc eax */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_1 + 0x61);
  v = ((s16)*(volatile u16 *)((u8 *)p + 0xb) > (s16)0x1c52) ? 2u : 1u;
  cm_fn_00785790((void *)&sc.obj[0], 5u, v);                      /* 0x005897e3 */

  /* 0x005897e8 mov esi,[edi+0xcf] */
  p = (u32 *)(u8 *)*(volatile u32 *)((u8 *)param_2 + 0xcf);
  v = *(volatile u32 *)p;                                        /* 0x005897f2 */
  cm_fn_00785790((void *)&sc.obj[0], 6u, v);                      /* 0x005897f7 */

  /* 0x005897fc mov ecx,[esi+4] / 0x005897ff mov edx,[ecx+0x33] */
  q = (u32 *)(u8 *)*(volatile u32 *)((u8 *)p + 4);
  v = *(volatile u32 *)((u8 *)q + 0x33);
  cm_fn_00785790((void *)&sc.obj[0], 7u, v);                      /* 0x00589809 */

  /* 0x0058980e mov eax,[esi+8] / 0x00589811 mov ecx,[eax+0x33] */
  q = (u32 *)(u8 *)*(volatile u32 *)((u8 *)p + 8);
  v = *(volatile u32 *)((u8 *)q + 0x33);
  cm_fn_00785790((void *)&sc.obj[0], 8u, v);                      /* 0x0058981b */

  /* 0x00589820 mov edx,[esi+0xc] / 0x00589827 mov eax,[edx+0x33] */
  q = (u32 *)(u8 *)*(volatile u32 *)((u8 *)p + 0xc);
  v = *(volatile u32 *)((u8 *)q + 0x33);
  cm_fn_00785790((void *)&sc.obj[0], 9u, v);                      /* 0x0058982d */

  /* 0x00589832 mov ecx,[ebx] */
  v = *(volatile u32 *)param_3;
  cm_fn_00785790((void *)&sc.obj[0], 10u, v);                     /* 0x0058983b */

  /* 0x00589840 mov edx,[esp+0x108] (fourth stack argument) */
  cm_fn_00785790((void *)&sc.obj[0], 11u, param_4);               /* 0x0058984e */

  /* 0x00589853 mov ecx,[0xb7431c] / 0x00589859 lea eax,[esp+0xc]
     0x0058985d push edi / 0x0058985e push eax */
  cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                 (u32)(u32 *)&sc.obj[0], (u32)param_2);           /* 0x0058985f */

done:
  /* 0x00589864 mov ecx,[esp+0xec] .. 0x0058986e mov fs:[0],ecx */
  CM_FS(u32, 0) = sc.frame.prev;
  return;
}
