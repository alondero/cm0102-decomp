/* fn_00401ea0, reconstructed from the original instructions of CM0102 3.9.68
   at preferred-image VA 0x00401ea0 (preferred image base 0x00400000,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).

   ABI, from the pinned instructions:
   - 0x00402083 `ret 4` pops one stack argument; ECX is read before it is
     written (0x00401ebe `mov esi, ecx`), so this is a thiscall with one
     caller-supplied stack argument at [esp+4] on entry
     (0x00401f02 `mov edi, [esp+0x428]`, esp == L-0x10 there, i.e. the entry
     return address + 4).
   - 0x00401f3e `call dword ptr [edx + 0x3c]` is an indirect thiscall: ECX is
     set from ESI at 0x00401f3c, the four real arguments are pushed at
     0x00401f33 (`push 0`), 0x00401f39 (`push eax` = &L+0x004), 0x00401f3a
     (`push ecx` = &L+0x000) and 0x00401f3b (`push edi`), and no caller cleanup
     follows the call, so the shared target removes its own 0x10 bytes. The
     `push ebx` at 0x00401f09 is the saved EBX register (EBX is popped again at
     0x00401fed), not an argument.
   - 0x00401fb9 `call 0x0068b100`: ECX from EAX (0x00401fb7), 22 stack
     arguments pushed at 0x00401f6d..0x00401fb6, and no `add esp, 0x58`
     afterwards, so that callee also removes its own arguments.
   - The three calls at 0x00402026, 0x00402036 and 0x00402047 share one
     `add esp, 0x24` at 0x0040204c, so all three leave their pushes (cdecl).
   - SEH frame at L+0x40c: `push -1` (0x00401ea6), `push 0x9534fe`
     (0x00401ea8), `push eax` (0x00401ead), `mov fs:[0], esp` (0x00401eae);
     the state dword is rewritten to 0 at 0x00401f56 and to -1 at 0x00401fc6,
     and restored from the saved predecessor at 0x0040206c/0x00402076.
   - Locals: `sub esp, 0x40c` at 0x00401eb5 gives L. The 256-byte buffers are
     at L+0x00c/L+0x20c (0x00401ed0/0x00401ed4) and L+0x10c/L+0x30c
     (0x00401ff0/0x00401ff7); the two 4-byte outputs are at L+0x000
     (0x00401f35) and L+0x004 (0x00401f2f, reloaded at 0x00401f74), and the
     0xee-byte allocation handle is stored at L+0x008 (0x00401f50). */

#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define ExceptionList CM_FS(void *, 0)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_00684230(u32 self);
extern u32 * CM_THISCALL cm_fn_0068b100(u32 *param_1,int param_2,u16 param_3,int param_4,u16 param_5, u32 param_6,u32 *param_7,u16 param_8,u8 param_9, u8 param_10,u8 param_11,int param_12,int param_13,u16 param_14, u8 param_15,u16 param_16,u8 param_17,u8 param_18, u16 param_19,u16 param_20,u16 param_21,u32 param_22, u8 param_23);
/* 0x0090d130 reads no incoming stack argument (the recovery map records it
   as a legacy `char *fn(void)`), so the three dwords pushed at 0x0040201f,
   0x00402024 and 0x00402025 are evaluated before its call yet belong to the
   next call: the single `add esp, 0x24` at 0x0040204c covers 9 dwords =
   those 3 + the 3 at 0x0040202b..0x00402031 + the 3 at
   0x0040203b..0x00402042. 0x00944c9f therefore receives six arguments and
   its format at 0x009870e8 takes %s %s %s %d. */
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Original SEH registration record: prev, handler, state in increasing
   addresses (state is the dword at L+0x414). */
typedef struct cm_seh_frame {
  void *prev;
  void *handler;
  u32 state;
} cm_seh_frame;

/* The shared virtual target of slot 0x3c, called at 0x00401f3e as a thiscall
   with ECX = object and four stack dwords; it pops those 0x10 bytes itself. */
typedef u32 (CM_THISCALL *cm_fn_00401ea0_vt3c_t)(void *self, int arg2, void *arg3, void *arg4, u32 arg5);

u32 CM_THISCALL cm_fn_00401ea0(u8 *param_1, int param_2)

{
  volatile cm_seh_frame frame;
  u8 scratch[0x40c];
  u32 block10;
  u32 blockee;
  u32 callret;
  u32 result;
  u32 vtbl;
  u32 idxbase;
  u32 count;
  u32 table;
  u32 *dst;
  u32 *arr1;
  u32 *arr2;
  volatile u32 *selected_slot;
  u32 slotval;
  u16 a_p20;
  u16 a_p16;
  u16 a_p14;
  u16 a_p21;
  u16 a_p19;
  u8 a_p17;
  u8 a_p11;
  u8 a_p10;
  u8 a_p9;
  u8 a_p15;
  u32 a_p8;
  void *err_hi;
  void *err_lo;
  u32 err_line;

  frame.state = 0xffffffffu;
  frame.handler = (void *)0x009534feu;
  frame.prev = ExceptionList;
  ExceptionList = (void *)(&frame);

  block10 = (u32)cm_fn_00944e46(0x10);

  if (block10 == 0u) {
    /* 0x00401ed0..0x00401ee4 */
    cm_fn_00944cff(0x009870f8u, 0u, 0u, (void *)(scratch + 0x20c), (void *)(scratch + 0x0c));
    err_hi = (void *)(scratch + 0x20c);
    err_lo = (void *)(scratch + 0x0c);
    err_line = 0x2dcu;
  }
  else {
    /* 0x00401f0a..0x00401f2b: copy the four dwords of row (param_2 + 1);
       the table base at [esi+0xee] is reloaded on every iteration. */
    idxbase = ((u32)param_2 + 1u) * 16u;
    count = 4u;
    dst = (u32 *)block10;
    do {
      table = *(volatile u32 *)(param_1 + 0xee);
      idxbase = idxbase + 4u;
      dst = dst + 1;
      count = count - 1u;
      *(dst - 1) = *(u32 *)(table + idxbase - 4u);
    } while (count != 0u);

    /* 0x00401f2d..0x00401f3e */
    vtbl = *(volatile u32 *)(param_1);
    callret = ((cm_fn_00401ea0_vt3c_t)(void *)(*(volatile u32 *)(vtbl + 0x3cu)))
                (param_1, param_2, (void *)(scratch + 0x000), (void *)(scratch + 0x004), 0u);

    /* 0x00401f41..0x00401f61 */
    blockee = (u32)cm_fn_00944cf1(0xee);
    *(volatile u32 *)(scratch + 0x008) = blockee;
    frame.state = 0u;
    if (blockee == 0u) {
      result = 0u;
    }
    else {
      /* Argument values read in listing order. */
      a_p20 = *(u16 *)(param_1 + 0xd9);                /* 0x00401f63 */
      a_p17 = *(u8 *)(param_1 + 0x51);                 /* 0x00401f6a */
      a_p16 = *(u16 *)(scratch + 0x004);               /* 0x00401f74 */
      a_p14 = *(u16 *)(param_1 + 0x40);                /* 0x00401f7d */
      a_p11 = *(u8 *)(param_1 + 0xc4);                 /* 0x00401f91 */
      a_p10 = *(u8 *)(param_1 + 0xc3);                 /* 0x00401f98 */
      a_p9 = *(u8 *)(param_1 + 0xc2);                  /* 0x00401f9f */
      a_p8 = *(u32 *)(scratch + 0x000);                /* 0x00401fa6 */
      a_p19 = 0u;
      a_p21 = (u16)0xffffu;
      a_p15 = (u8)param_2;

      result = (u32)cm_fn_0068b100(                    /* 0x00401fb9 */
                 (u32 *)blockee,
                 (int)(u32)param_1,                    /* 0x00401fb6 */
                 4,                                    /* 0x00401fb4 */
                 (int)block10,                          /* 0x00401fb3 */
                 1u,                                   /* 0x00401fb1 */
                 *(volatile u32 *)(param_1 + 4),        /* 0x00401fab */
                 (u32 *)callret,                        /* 0x00401faf */
                 a_p8,                                  /* 0x00401fae */
                 a_p9,                                  /* 0x00401faa */
                 a_p10,                                 /* 0x00401fa5 */
                 a_p11,                                 /* 0x00401f9e */
                 (int)(u32)(param_1 + 0xc5),            /* 0x00401f97 */
                 (int)(u32)(param_1 + 0xbe),            /* 0x00401f90 */
                 a_p14,                                 /* 0x00401f83 */
                 a_p15,                                 /* 0x00401f82 */
                 a_p16,                                 /* 0x00401f81 */
                 a_p17,                                 /* 0x00401f7c */
                 4u,                                    /* 0x00401f7a */
                 a_p19,                                 /* 0x00401f78 */
                 a_p20,                                 /* 0x00401f73 */
                 a_p21,                                 /* 0x00401f71 */
                 0u,                                    /* 0x00401f6f */
                 2u);                                   /* 0x00401f6d */
    }

    /* 0x00401fc2..0x00401fe8 */
    arr1 = (u32 *)(*(volatile u32 *)(param_1 + 0x0c));
    frame.state = 0xffffffffu;
    arr1[param_2] = result;
    cm_fn_009452ca(block10);
    cm_fn_009452ca(callret);
    arr2 = (u32 *)(*(volatile u32 *)(param_1 + 0x0c));
    selected_slot = (volatile u32 *)((u32)arr2 + (u32)param_2 * 4u);
    slotval = *selected_slot;
    if (slotval != 0u) {
      /* 0x00401fe8 retains the slot address in EAX. 0x00402060 reloads
         that slot after the potentially aliasing store at 0x0040205d. */
      *(volatile u32 *)(param_1 + 0x30) = (u32)param_2;
      cm_fn_00684230(*selected_slot);
      ExceptionList = frame.prev;
      return 1u;
    }

    /* 0x00401ff0..0x0040200e */
    cm_fn_00944cff(0x009870f8u, 0u, 0u, (void *)(scratch + 0x30c), (void *)(scratch + 0x10c));
    err_hi = (void *)(scratch + 0x30c);
    err_lo = (void *)(scratch + 0x10c);
    err_line = 0x2f2u;
  }

  /* 0x00402025..0x0040205b: shared failure tail. The three pushes made
     before `call 0x0090d130` (0x0040201f/0x00402024/0x00402025) are the
     leading varargs of 0x00944c9f whose call follows at 0x00402036. */
  {
    char *text;

    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                   (u32)err_hi, (u32)err_lo, err_line);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)(&DAT_00ae24d0), 0);
    DAT_00b67a34 = 0u;
  }
  ExceptionList = frame.prev;
  return 0u;
}
