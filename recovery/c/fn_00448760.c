/* fn_00448760 -- preferred-image VA 0x00448760 (CM0102 3.9.68,
 * binary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
 *
 * ABI from the original instructions:
 *   0x00448764  mov ebx, dword ptr [esp + 0x18]   -> one stack argument
 *   0x00448769  mov ebp, ecx                      -> ECX is the receiver
 *   0x004487a6  call 0x4cf5a0                     -> thiscall, callee pops 4
 *   0x00448ac7  ret 4                             -> callee pops the argument
 * So: thiscall with the receiver in ECX and one 4-byte stack argument.
 */
#include "cmre.h"

#define G_00ae1050 CM_GLOBAL(volatile u32, 0x00ae1050)
#define G_00ae1080 CM_GLOBAL(volatile u32, 0x00ae1080)
#define G_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_00ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_THISCALL cm_fn_00452710(u32 self, u32 *index, u32 create);
extern u32 CM_THISCALL cm_fn_004528d0(u32 self, u32 *index, u32 create);
extern u32 CM_THISCALL cm_fn_00452cb0(u32 self, u32 arg1, u8 arg2);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern void CM_CDECL cm_fn_00547a10(u32 object, u32 value, u32 selector);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);

#define RD32(p)  (*(volatile u32 *)(p))
#define RD16(p)  (*(volatile u16 *)(p))
#define RD8(p)   (*(volatile u8 *)(p))
#define WR32(p, v) (*(volatile u32 *)(p) = (u32)(v))
#define WR8(p, v)  (*(volatile u8 *)(p) = (u8)(v))

u32 CM_THISCALL cm_fn_00448760(u32 self, u32 *volatile param_1)
{
  /* L+8 scratch buffer handed to cm_fn_00417c10 (8 bytes in the original). */
  union { u32 w[2]; u8 b[8]; unsigned long long a; } scratch;
  u8 *arg;
  u8 *selfp;
  u32 a, b, c, g1, i6, idx, r, x;
  u32 obj1, obj2, y, count, id;
  u32 *ptr;
  u8 *arr;
  s32 i;
  int found;

  arg = (u8 *)param_1;                       /* 0x00448764: ebx */
  selfp = (u8 *)self;

  a  = RD32(arg + 0x0c);                     /* 0x0044876b */
  g1 = G_00ae23b4;                           /* 0x0044876f: esi */
  b  = G_00ae23bc;                           /* 0x0044877f */
  c  = RD32(arg + 0x08);                     /* 0x00448787 */
  /* 0x00448776 lea ecx,[eax+eax*4]; 0x00448779 lea eax,[eax+ecx*2];
   * 0x0044877c lea edx,[eax+eax*4]; 0x00448784 lea ecx,[eax+edx*2]  -> b + 110*a */
  i6 = b + 110u * a;
  g1 = g1 + c;                               /* 0x0044878a: add esi, eax */
  /* 0x00448790 lea edx,[eax+eax*8]; 0x0044879a shl edx,4;
   * 0x0044879d add edx,eax; 0x0044879f lea edi,[esi+edx*4] -> esi + 580*c */
  idx = g1 + 580u * c;

  r = cm_fn_004cf5a0(G_00ae1050, i6);        /* 0x004487a6 */
  G_00ae1080 = r;                            /* 0x004487ad: dword store */
  if (r != 0) {
    x = RD32((u8 *)r + 4);                   /* 0x004487b4 */
    g1 = G_00ae23b4;                         /* 0x004487b7: re-read */
    param_1 = (u32 *)(g1 + 581u * x);        /* 0x004487ca: caller-owned home */
  } else {
    param_1 = (u32 *)0;                      /* 0x004487d0: caller-owned home */
  }

  obj1 = cm_fn_004528d0(self, (u32 *)idx, 1); /* 0x004487dd */
  obj2 = cm_fn_00452710(self, (u32 *)idx, 1); /* 0x004487e9 */

  ptr = param_1;                             /* 0x004487f0: reload home */
  if (ptr == 0) goto done;                   /* 0x004487f6 */

  y = RD32(arg + 0x1b);                      /* 0x004487fc */
  if (y == 0) goto skip_first;               /* 0x00448801 */
  if ((s32)y <= (s32)RD32((u8 *)obj1 + 0x313)) goto skip_first; /* 0x0044880d */

  WR32((u8 *)obj1 + 0x313, y);               /* 0x0044881f */
  cm_fn_007e4920(i6, (u32)((u8 *)obj1 + 0x317)); /* 0x00448825 */

  ptr = param_1;                             /* 0x0044882a: reload home */
  WR32((u8 *)obj1 + 0x32f, RD32((u8 *)ptr)); /* 0x00448835, 0x0044883c */
  cm_fn_00417c10((u32 *)((u8 *)obj1 + 0x333), (u32 *)scratch.b,
                 (u16 *)&G_00ae2c90);       /* 0x00448849 */

  WR8((u8 *)obj1 + 0x33b, (RD32(arg + 0x04) != 0) ? 1 : 0); /* 0x00448856 */
  if ((s32)RD32(arg + 0x1b) <= (s32)RD32((u8 *)obj2 + 0x313))
    goto skip_first;                         /* 0x00448865 */

  WR32((u8 *)obj2 + 0x313, RD32((u8 *)obj1 + 0x313)); /* 0x00448871 */
  WR32((u8 *)obj2 + 0x317, RD32((u8 *)obj1 + 0x317)); /* 0x0044887d */
  WR32((u8 *)obj2 + 0x31b, RD32((u8 *)obj1 + 0x31b)); /* 0x00448889 */
  WR32((u8 *)obj2 + 0x31f, RD32((u8 *)obj1 + 0x31f)); /* 0x00448895 */
  WR32((u8 *)obj2 + 0x323, RD32((u8 *)obj1 + 0x323)); /* 0x0044889b, 0x004488b3 */
  cm_fn_00417c10((u32 *)((u8 *)obj2 + 0x327), (u32 *)scratch.b,
                 (u16 *)((u8 *)obj1 + 0x327)); /* 0x004488b9 */
  WR32((u8 *)obj2 + 0x32f, RD32((u8 *)obj1 + 0x32f)); /* 0x004488be, 0x004488d6 */
  cm_fn_00417c10((u32 *)((u8 *)obj2 + 0x333), (u32 *)scratch.b,
                 (u16 *)((u8 *)obj1 + 0x333)); /* 0x004488dc */
  WR8((u8 *)obj2 + 0x33b, RD8((u8 *)obj1 + 0x33b)); /* 0x004488e1, 0x004488eb */

  {
    s32 v = (s32)(s16)RD16((u8 *)idx + 0x80) + 300; /* 0x004488f1 movsx, 0x004488f8 */
    u32 thr = (u32)v * 1000u;                 /* 0x004488fd..0x0044890c */
    if ((s32)RD32((u8 *)obj2 + 0x313) < (s32)thr)
      goto skip_first;                        /* 0x0044890f */
  }

  count = RD32(selfp + 0x2c);                /* 0x00448913 */
  i = 0;
  found = 0;
  if ((s32)count > 0) {
    id = RD32((u8 *)idx);                    /* 0x0044891c: guarded by 0x0044891a */
    arr = (u8 *)RD32(selfp + 0x24);          /* 0x0044891e */
    for (;;) {
      if (RD32(arr) == id) {
        if (RD8(arr + 4) == 0x1c) { found = 1; break; } /* 0x00448929 */
      }
      i++;
      arr += 6;                               /* 0x0044892c */
      if (!((s32)i < (s32)count)) break;      /* 0x0044892f */
    }
  }
  if (!found) cm_fn_00452cb0(self, RD32((u8 *)idx), 0x1c); /* 0x00448933, 0x0044893a */

  cm_fn_00547a10(RD32((u8 *)idx + 0xbf), i6, 1); /* 0x0044894d */

skip_first:                                    /* 0x00448955 */
  ptr = param_1;                             /* 0x00448955: reload home */
  obj1 = cm_fn_004528d0(self, (u32 *)ptr, 1); /* 0x0044895e */
  ptr = param_1;                             /* 0x00448963: reload home */
  obj2 = cm_fn_00452710(self, (u32 *)ptr, 1); /* 0x0044896e */

  y = RD32(arg + 0x1b);                      /* 0x00448975 */
  if (y == 0) goto done;                     /* 0x0044897a */
  if ((s32)y <= (s32)RD32((u8 *)obj1 + 0x33c)) goto done; /* 0x00448986 */
  WR32((u8 *)obj1 + 0x33c, y);               /* 0x0044898c */
  cm_fn_007e4920(i6, (u32)((u8 *)obj1 + 0x340)); /* 0x0044899e */

  WR32((u8 *)obj1 + 0x358, RD32((u8 *)idx)); /* 0x004489ae, 0x004489bc */
  cm_fn_00417c10((u32 *)((u8 *)obj1 + 0x35c), (u32 *)scratch.b,
                 (u16 *)&G_00ae2c90);        /* 0x004489c2 */

  WR8((u8 *)obj1 + 0x364, (RD32(arg + 0x04) != 0) ? 1 : 0); /* 0x004489cf */
  if ((s32)RD32(arg + 0x1b) <= (s32)RD32((u8 *)obj2 + 0x33c))
    goto done;                               /* 0x004489de */

  WR32((u8 *)obj2 + 0x33c, RD32((u8 *)obj1 + 0x33c)); /* 0x004489ea */
  WR32((u8 *)obj2 + 0x340, RD32((u8 *)obj1 + 0x340)); /* 0x004489f6 */
  WR32((u8 *)obj2 + 0x344, RD32((u8 *)obj1 + 0x344)); /* 0x00448a02 */
  WR32((u8 *)obj2 + 0x348, RD32((u8 *)obj1 + 0x348)); /* 0x00448a0e */
  WR32((u8 *)obj2 + 0x34c, RD32((u8 *)obj1 + 0x34c)); /* 0x00448a24 */
  cm_fn_00417c10((u32 *)((u8 *)obj2 + 0x350), (u32 *)scratch.b,
                 (u16 *)((u8 *)obj1 + 0x350)); /* 0x00448a32 */
  WR32((u8 *)obj2 + 0x358, RD32((u8 *)obj1 + 0x358)); /* 0x00448a37, 0x00448a47 */
  cm_fn_00417c10((u32 *)((u8 *)obj2 + 0x35c), (u32 *)scratch.b,
                 (u16 *)((u8 *)obj1 + 0x35c)); /* 0x00448a55 */
  /* 0x00448a5a read, 0x00448a60 home reload, 0x00448a64 store,
     0x00448a6a obj2 field read, 0x00448a70 word read: keep that order. */
  {
    u8 c364 = RD8((u8 *)obj1 + 0x364);        /* 0x00448a5a */
    ptr = param_1;                             /* 0x00448a60: reload home */
    WR8((u8 *)obj2 + 0x364, c364);            /* 0x00448a64: after the reload */
    {
      u32 o33c = RD32((u8 *)obj2 + 0x33c);     /* 0x00448a6a */
      s32 v = (s32)(s16)RD16((u8 *)ptr + 0x80) + 300; /* 0x00448a70 movsx */
      u32 thr = (u32)v * 1000u;                /* 0x00448a7c..0x00448a85 */
      if ((s32)o33c < (s32)thr)
        goto done;                             /* 0x00448a88 */
    }
  }

  count = RD32(selfp + 0x2c);                /* 0x00448a8c */
  i = 0;
  found = 0;
  if ((s32)count > 0) {
    id = RD32((u8 *)ptr);                    /* 0x00448a9a */
    arr = (u8 *)RD32(selfp + 0x24);          /* 0x00448a9c */
    for (;;) {
      if (RD32(arr) == id) {
        if (RD8(arr + 4) == 0x1d) { found = 1; break; } /* 0x00448aa6 */
      }
      i++;
      arr += 6;                               /* 0x00448aa9 */
      if (!((s32)i < (s32)count)) break;      /* 0x00448aac */
    }
  }
  if (!found) cm_fn_00452cb0(self, RD32((u8 *)ptr), 0x1d); /* 0x00448ab6 */

done:                                          /* 0x004487bb */
  return 1;                                   /* 0x00448abe */
}
