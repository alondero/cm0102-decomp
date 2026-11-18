/* fn_0041add0 (preferred-image VA 0x0041add0, image base 0x00400000),
   translated from the pinned CM0102 3.9.68 instructions
   (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).

   Frame: `push -1; push handler; push prev; mov fs:[0],esp` establishes the
   SEH record { prev, handler, state } at S-0x0c.  `sub esp,0xfc` then names the
   locals region S-0x108 .. S-0x0d, which is `L` below:

     L+0x00  record base, 0x0041ae4b `lea eax,[esp+0xc]` (=S-0x108), 17 bytes
     L+0x04  second record dword, 0x0041ae2a `mov [esp+0x10],ecx` (=S-0x104)
     L+0x08  object from 0x0054a110 / receiver of 0x00417c10 (S-0x100)
     L+0x14  0x00417c10 argument, 0x0041ae26 `lea edx,[esp+0x20]` (S-0x0f4)
     L+0x1c  object for 0x0077bbe0 / 0x00785790 / 0x007861e0 (S-0x0ec)

   The 17-byte record handed to 0x0068d0d0 at 0x0041ae4b is L0..L10.  L0 and L4
   are the two dwords this routine stores; L8..Lf are the eight bytes that
   0x0054a110 (0x0054a112 `mov word [eax],0x28`, 0x0054a117 `mov word
   [eax+2],0x7b4`, 0x0054a11d `mov dword [eax+4],1`) and 0x00417c10
   (0x00417c21/0x00417c28/0x00417c2e copies) leave there.  The trailing byte
   L10 (S-0x0f8) is never written by this routine, by those callees, or by
   0x00417c10's eight-byte copy to L+0x14; it stays uninitialized caller stack
   and therefore stays an uninitialized scratch byte here.  It is not zeroed,
   not padded and not removed from the 17-byte count. */

#include "cmre.h"

#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define DAT_00ae2a3c CM_GLOBAL(volatile u32, 0x00ae2a3c)
#define DAT_00b7431c CM_GLOBAL(volatile u32, 0x00b7431c)

/* 0x00417c10: ECX = 8-byte destination, arg1 = 8-byte result destination,
   arg2 = 8-byte source; `ret 8` at 0x00417c40. */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
/* 0x00544410: cdecl, arg1 = record, arg2 read as `movsx edx,byte [esp+8]`
   (0x00544414); returns through __ftol, only AL is used by our caller. */
extern long long CM_CDECL cm_fn_00544410(volatile u32 record, volatile u32 adjustment);
/* 0x0054a110: pointer in ECX only (0x0054a110 `mov eax,ecx`). */
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
/* 0x005a5980: ECX = receiver, 3 stack arguments, `ret 0xc`. */
extern void CM_THISCALL cm_fn_005a5980(int param_1, int *param_2, u32 param_3, char param_4);
/* 0x00600a50: cdecl, five stack arguments, caller cleans (`ret`). */
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
/* 0x0068d0d0: ECX = map object, arg1 = record, arg2 = count; `ret 8`
   (0x0068d0d0 only reads ECX and its two stack arguments). */
extern u32 CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
/* 0x0077bbe0: ECX = receiver, 2 stack arguments, `ret 8`. */
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
/* 0x00785790: ECX = receiver, arg1 = byte index, arg2 = value, `ret 8`. */
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
/* 0x007861e0: ECX = receiver, 2 stack arguments, `ret 8`. */
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
/* 0x007875e0: single stack argument, `ret 4`, returns EAX. */
extern u32 CM_STDCALL cm_fn_007875e0(u32 club);
/* 0x0090cfc0: single stack argument at 0x0090cfc2 `mov esi,[esp+0xc]`, plain
   `ret`; EAX/EDX are written before any read. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

void CM_THISCALL cm_fn_0041add0(u32 *self, u32 *param_1, u32 *param_2)

{
  volatile struct { u32 prev; u32 handler; u32 state; } frame;
  u8 L[0xfc];
  u32 v;
  u32 w;
  u32 *child;
  long long rv;

  frame.state = 0xffffffffu;                       /* 0x0041add0 push -1 */
  frame.handler = 0x00953f36u;                     /* 0x0041add2 push 0x953f36 */
  frame.prev = (u32)CM_FS(void *, 0);              /* 0x0041add7 push fs:[0] */
  CM_FS(void *, 0) = (void *)(u32)(&frame);        /* 0x0041adde mov fs:[0],esp */

  cm_fn_0054a110((u16 *)(L + 8));                  /* 0x0041adf0 lea ecx,[esp+0x14] */
  frame.state = 0;                                 /* 0x0041ae00 [esp+0x110] */
  if (param_1 == (u32 *)0) goto epilogue;          /* 0x0041ae0b */
  if (param_2 == (u32 *)0) goto epilogue;          /* 0x0041ae1a */

  v = *param_2;                                    /* 0x0041ae22 mov ecx,[esi] */
  w = *param_1;                                    /* 0x0041ae24 mov eax,[ebx] */
  *(u32 *)(L + 4) = v;                             /* 0x0041ae2a [esp+0x10] */
  *(u32 *)(L + 0) = w;                             /* 0x0041ae38 [esp+0x14] */
  cm_fn_00417c10((u32 *)(L + 8), (u32 *)(L + 0x14), (u16 *)(&DAT_00ae2c90));

  /* 0x0041ae41 mov ecx,[edi+8]; 0x0041ae44 lea eax,[esp+0xc];
     0x0041ae48 push 0x11; 0x0041ae4a push eax; 0x0041ae4b call 0x68d0d0 */
  cm_fn_0068d0d0((volatile u8 *)(u32)(*(u32 *)((u8 *)self + 8)),
                 (const u8 *)(L + 0), 17);

  if (cm_fn_007875e0((u32)(u32)param_2) != 0) {     /* 0x0041ae50-0x0041ae5e */
    for (;;) {
      v = cm_fn_0090cfc0(5);                       /* 0x0041ae64 */
      child = (u32 *)(u32)(*(u32 *)((u8 *)param_2 + v * 4 + 0x19f));
      v = cm_fn_0090cfc0(0x14);                    /* 0x0041ae72 */
      if (v == 0) break;                           /* 0x0041ae7e */
      if (child != (u32 *)0) break;                /* 0x0041ae82 */
    }

    cm_fn_0077bbe0((void *)(L + 0x1c), 0x138f, 0); /* 0x0041ae84-0x0041ae8f */

    v = *param_1;                                  /* 0x0041ae94 mov eax,[ebx] */
    *((volatile u8 *)&frame.state) = 1;            /* 0x0041ae9d byte [esp+0x118] */
    cm_fn_00785790((void *)(L + 0x1c), 0, v);      /* 0x0041ae96-0x0041aea5 */

    v = *param_2;                                  /* 0x0041aeaa mov ecx,[esi] */
    cm_fn_00785790((void *)(L + 0x1c), 1, v);      /* 0x0041aeaf-0x0041aeb3 */

    v = cm_fn_0090cfc0(2);                         /* 0x0041aeb8 */
    cm_fn_00785790((void *)(L + 0x1c), 2, v);      /* 0x0041aec2-0x0041aec9 */

    cm_fn_00785790((void *)(L + 0x1c), 3,
                   *(u32 *)((*(u32 *)((u8 *)param_1 + 4)) + 0x33u));
    cm_fn_00785790((void *)(L + 0x1c), 4,
                   *(u32 *)((*(u32 *)((u8 *)param_1 + 8)) + 0x33u));
    cm_fn_00785790((void *)(L + 0x1c), 5,
                   *(u32 *)((*(u32 *)((u8 *)param_1 + 0xc)) + 0x33u));

    if (child == (u32 *)0) {                       /* 0x0041af04 */
      cm_fn_00785790((void *)(L + 0x1c), 6, 0xffffffffu);
      cm_fn_00785790((void *)(L + 0x1c), 7, 0xffffffffu);
      cm_fn_00785790((void *)(L + 0x1c), 8, 0xffffffffu);
      v = 0xffffffffu;
    }
    else {
      cm_fn_00785790((void *)(L + 0x1c), 6, *(u32 *)child);
      cm_fn_00785790((void *)(L + 0x1c), 7,
                     *(u32 *)((*(u32 *)((u8 *)child + 4)) + 0x33u));
      cm_fn_00785790((void *)(L + 0x1c), 8,
                     *(u32 *)((*(u32 *)((u8 *)child + 8)) + 0x33u));
      v = *(u32 *)((*(u32 *)((u8 *)child + 0xc)) + 0x33u);
    }
    cm_fn_00785790((void *)(L + 0x1c), 9, v);      /* 0x0041af6c */

    /* 0x0041af77 mov ecx,[0xb7431c]; 0x0041af7d lea eax,[esp+0x28];
       0x0041af81 push esi; 0x0041af82 push eax; 0x0041af83 call 0x7861e0 */
    cm_fn_007861e0((void *)(u32)DAT_00b7431c, (u32)(u32)(L + 0x1c),
                   (u32)(u32)param_2);
  }
  /* 0x0041af88 push 0; push 0; push 1; push 0; push esi */
  v = cm_fn_00600a50((u32)(u32)param_2, 0, 1, 0, 0);
  if (v != 0) goto epilogue;                       /* 0x0041af9b */

  v = *(u32 *)((u8 *)param_2 + 0xcf);              /* 0x0041af9d */
  if (v == 0) goto epilogue;                       /* 0x0041afa5 */
  v = *(u32 *)(v + 0x69);                          /* 0x0041afa7 */
  if (v == 0) goto epilogue;                       /* 0x0041afac */

  w = cm_fn_0090cfc0((u32)(s32)(s8)(*(u8 *)(v + 0x14)));
  v = cm_fn_0090cfc0(10);                          /* 0x0041afbc */
  if ((s32)w <= (s32)v) goto epilogue;             /* 0x0041afc4 cmp edi,eax; 0x0041afc6 jle (signed) */

  w = *(u32 *)((u8 *)param_2 + 0xcf);              /* 0x0041afc8 reload */
  w = *(u32 *)(w + 0x69);
  rv = cm_fn_00544410((volatile u32)w,              /* 0x0041afd1 mov cl,[eax+0x19] */
                      (volatile u32)(*(u8 *)(w + 0x19)));
  w = cm_fn_0090cfc0((u32)(s32)(s8)(u8)rv);         /* 0x0041afdb movsx edx,al */
  v = cm_fn_0090cfc0(5);                           /* 0x0041afe8 */
  v = ((s32)w <= (s32)v) ? 2u : 1u;               /* 0x0041aff0 cmp esi,eax; 0x0041aff4 jle (signed) */
  cm_fn_005a5980((int)(u32)DAT_00ae2a3c, (int *)param_1, v, (char)1);

epilogue:
  CM_FS(void *, 0) = (void *)(u32)frame.prev;      /* 0x0041b008-0x0041b012 */
}
