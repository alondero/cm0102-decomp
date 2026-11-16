/* fn_007da6e0 — preferred-image VA 0x007da6e0, image base 0x00400000,
   CM0102 3.9.68 (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86).

   ABI from the original instructions: `mov ebx, ecx` at 007da6f6 reads ECX
   before writing it (receiver), `mov eax, [esp+4]` at 007da6e0 and
   `mov esi, [esp+0x2e0]` at 007da6ed (== entry [esp+8]) read two dword stack
   arguments, and `ret 8` at 007da711/007daa47/007daa65 pops them. So this is a
   CM_THISCALL receiver plus two dword stack arguments; the result is set with
   `xor al,al` (007da708) / `mov al,1` (007da8d4, 007daa3e, 007daa5c), i.e. u8.

   Local layout, L = ESP after `sub esp,0x2cc` at 007da6e4 (all offsets are the
   original displacements recomputed against ESP at each instruction):
     L+0x000 byte flag   (007da7f3 = 1, 007da7de = 0, read 007daa09)
     L+0x001 mode byte   (007da7cd, re-read 007da9de)
     L+0x002 parse word  (written through the pointer pushed at 007da73a,
                          low byte re-read at 007da9e2)
     L+0x004 name A      (0x1a-byte comparisons; 007da7eb, 007da810, ...)
     L+0x068 100-byte text B (007da714, 007da779/81, 007da9de reads no)
     L+0x0cc name C      (007da922, 007da941)
     L+0x130 212-byte record R (007daa2e/007daa4c, count 0xd4)
     L+0x204 text D      (007da78a, 007da7be)
     L+0x268 100-byte text E (007da779, 007da9d0)
   The record bytes L+0x196..L+0x1f9 (R+0x66..R+0xca, 100 bytes) are never
   written by this routine: they are left uninitialized here, exactly as the
   original leaves its own stack bytes, and the complete 212-byte copy to the
   callee is preserved (no zeroing, no invented padding, no shortened copy). */

#include "cmre.h"

extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);
extern u8 CM_STDCALL cm_fn_007daa70(char *param_1);
extern u32 CM_STDCALL cm_fn_007dab30(u32 input, u32 output);
extern u32 CM_CDECL cm_fn_0090cba0(u32 *stream, u32 output_base);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);
extern s32 CM_CDECL cm_fn_00946fa0(u32 first, u32 second, u32 count);
extern s32 CM_CDECL cm_fn_00946fd8(char *param_1, u32 param_2, u32 param_3);

u8 CM_THISCALL cm_fn_007da6e0(u8 *self, u32 arg1, volatile u32 arg2)
{
  u8 L[0x2cc];
  u32 esi;
  s32 ebp;
  u8 mode;
  s32 t;

  /* 007da6e0-007da711: cm_fn_0090cba0(arg1, arg2); a zero result returns 0 */
  if (cm_fn_0090cba0((u32 *)arg1, (u32)arg2) == 0) {
    return 0;
  }

  /* 007da714-007da71c: cm_fn_007dab30(arg2, &L[0x68]) */
  esi = cm_fn_007dab30((u32)arg2, (u32)(L + 0x068));
  /* 007da721-007da737: pointer is never null (constant-false original test) */
  if ((u32)(L + 0x068) == 0u) {
    return 0;
  }

  /* 007da73a-007da761: 3 pushed arguments, `add esp,0xc`; zero returns 0 */
  if (cm_fn_00946fd8((char *)(L + 0x068), 0xa6bc70u, (u32)(L + 0x002)) == 0) {
    return 0;
  }

  /* 007da764-007da771 */
  esi = cm_fn_007dab30(esi, (u32)(L + 0x068));

  /* 007da773-007da787: cm_fn_009466d0(&L[0x268], &L[0x68], 100) */
  cm_fn_009466d0((u32)(L + 0x268), (u32)(L + 0x068), 0x64u);
  /* 007da793 */
  *(u8 *)(L + 0x2cb) = 0;

  /* 007da78a-007da79d */
  esi = cm_fn_007dab30(esi, (u32)(L + 0x204));
  /* 007da7a4-007da7bb */
  if ((u32)(L + 0x204) == 0u) {
    return 0;
  }

  /* 007da7be-007da7c8 */
  mode = cm_fn_007daa70((char *)(L + 0x204));
  /* 007da7cd */
  *(u8 *)(L + 0x001) = mode;

  /* 007da7d1-007da7e6: movsx eax,al; sub eax,2; je; sub eax,2; je */
  t = (s32)(s8)mode;
  if ((t - 2) == 0 || (t - 4) == 0) {
    /* 007da7f1-007da7f8: both argument pushes happen before the flag store */
    *(u8 *)(L + 0x000) = 1;
    esi = cm_fn_007dab30(esi, (u32)(L + 0x004));
    /* 007da7fd: dword store into the incoming argument home, then reloaded
       at 007da91b */
    arg2 = esi;
    /* 007da804-007da80a */
    if ((u32)(L + 0x004) == 0u) {
      return 1;
    }
    /* 007da810-007da816 */
    if (*(u8 *)(L + 0x004) == 0) {
      return 1;
    }
    {
      s32 i = 0;
      s32 n;
      u32 base;
      u32 p;

      /* 007da81c / 007da822 / 007da827 */
      n = CM_GLOBAL(s32, 0x00ae2354);
      base = CM_GLOBAL(u32, 0x00ae23ac);
      if (n > 0) {
        p = base + 4u;
        for (;;) {
          s32 hit = 0;
          /* 007da830-007da84b */
          if (p != 0u) {
            if (*(u8 *)p != 0) {
              if (cm_fn_00946fa0(p, (u32)(L + 0x004), 0x1au) == 0) {
                hit = 1;
              }
            }
          }
          if (hit) {
            /* 007da85c-007da861 */
            ebp = i;
            if (i != -1) {
              goto lab_91b;
            }
            goto lab_867;
          }
          /* 007da84d-007da858 */
          n = CM_GLOBAL(s32, 0x00ae2354);
          i = i + 1;
          p = p + 0x38u;
          if (!(i < n)) {
            goto lab_867;
          }
        }
      }
    }
  }
  else {
    /* 007da7de-007da7e6 */
    *(u8 *)(L + 0x000) = 0;
    ebp = -1;
    goto lab_922;
  }

lab_867:
  /* 007da867: 0x245-stride group, two name slots per element */
  {
    s32 i = 0;
    s32 n = CM_GLOBAL(s32, 0x00ae235c);
    u32 base = CM_GLOBAL(u32, 0x00ae23b4);
    u32 p;

    /* 007da874 */
    if (n <= 0) {
      return 1;
    }
    /* 007da87c */
    p = base + 0x38u;
    for (;;) {
      s32 hit = 0;
      u32 q = p - 0x34u;

      /* 007da87f-007da8a2 */
      if (q != 0u) {
        if (*(u8 *)q != 0) {
          if (cm_fn_00946fa0(q, (u32)(L + 0x004), 0x1au) == 0) {
            hit = 1;
          }
        }
      }
      /* 007da8a4-007da8bf */
      if (!hit) {
        if (p != 0u) {
          if (*(u8 *)p != 0) {
            if (cm_fn_00946fa0(p, (u32)(L + 0x004), 0x1au) == 0) {
              hit = 1;
            }
          }
        }
      }
      if (hit) {
        break;
      }
      /* 007da8c1-007da8cf */
      n = CM_GLOBAL(s32, 0x00ae235c);
      i = i + 1;
      p = p + 0x245u;
      if (!(i < n)) {
        return 1;
      }
    }
    /* 007da8e0 */
    if (i == -1) {
      return 1;
    }
    /* 007da8e9-007da915: lea eax,[edi+edi*8]; shl eax,4; add eax,edi;
       add ecx,edi; mov eax,[ecx+eax*4+0x69] */
    {
      u32 tv = (u32)(i + i * 8);
      u32 v;
      u32 w;

      tv = tv << 4;
      tv = tv + (u32)i;
      v = *(u32 *)(CM_GLOBAL(u32, 0x00ae23b4) + (u32)i + tv * 4u + 0x69u);
      if (v == 0u) {
        return 1;
      }
      /* 007da905 */
      w = *(u32 *)(v + 0x38u);
      if (w == 0u) {
        return 1;
      }
      /* 007da910-007da915 */
      ebp = (s32)*(u32 *)w;
      if (ebp == -1) {
        return 1;
      }
    }
    goto lab_91b;
  }

lab_91b:
  /* 007da91b */
  esi = (u32)arg2;

lab_922:
  /* 007da922-007da92d */
  (void)cm_fn_007dab30(esi, (u32)(L + 0x0cc));
  /* 007da932-007da93b */
  if ((u32)(L + 0x0cc) == 0u) {
    goto lab_9c1;
  }
  /* 007da941-007da94a */
  if (*(u8 *)(L + 0x0cc) == 0) {
    goto lab_9c1;
  }
  {
    s32 i = 0;
    s32 n;
    u32 base;
    u32 p;

    /* 007da94c / 007da952 / 007da957 */
    n = CM_GLOBAL(s32, 0x00ae2350);
    base = CM_GLOBAL(u32, 0x00ae23a8);
    /* 007da959 */
    if (n <= 0) {
      goto lab_9c1;
    }
    /* 007da95d */
    p = base + 0x38u;
    for (;;) {
      s32 hit = 0;
      u32 q = p - 0x34u;

      /* 007da960-007da986 */
      if (q != 0u) {
        if (*(u8 *)q != 0) {
          if (cm_fn_00946fa0(q, (u32)(L + 0x0cc), 0x1au) == 0) {
            hit = 1;
          }
        }
      }
      /* 007da988-007da9a6 */
      if (!hit) {
        if (p != 0u) {
          if (*(u8 *)p != 0) {
            if (cm_fn_00946fa0(p, (u32)(L + 0x0cc), 0x1au) == 0) {
              hit = 1;
            }
          }
        }
      }
      if (hit) {
        break;
      }
      /* 007da9a8-007da9b6 */
      n = CM_GLOBAL(s32, 0x00ae2350);
      i = i + 1;
      p = p + 0x122u;
      if (!(i < n)) {
        goto lab_9c1;
      }
    }
    /* 007da9ba-007da9bf */
    if (i == -1) {
      goto lab_9c1;
    }
    esi = (u32)i;
    goto lab_9c7;
  }

lab_9c1:
  /* 007da9c1 */
  esi = CM_GLOBAL(u32, 0x009cf2e4);

lab_9c7:
  {
    u8 m;
    u8 pb;
    u8 flag;
    u16 w;
    u32 obj;

    /* 007da9c7-007da9d9: cm_fn_009466d0(&L[0x132], &L[0x268], 100) */
    cm_fn_009466d0((u32)(L + 0x132), (u32)(L + 0x268), 0x64u);
    /* 007da9de: mov al,[esp+0x1d] with ESP=L-0x1c */
    m = *(u8 *)(L + 0x001);
    /* 007da9e2: mov cl,[esp+0x1e] */
    pb = *(u8 *)(L + 0x002);
    /* 007da9e6: mov byte [esp+0x216],al  (L+0x1fa) */
    *(u8 *)(L + 0x1fa) = m;
    /* 007da9ed */
    w = *(u16 *)(self + 8);
    /* 007da9f4: word store of the pre-increment value */
    *(u16 *)(L + 0x130) = w;
    /* 007da9fc-007da9fd: inc eax, then terminate the copied text */
    *(u8 *)(L + 0x195) = 0;
    /* 007daa05: word store of ax = low word of the incremented value */
    *(u16 *)(self + 8) = (u16)(w + 1u);
    /* 007daa09 */
    flag = *(u8 *)(L + 0x000);
    /* 007daa0f */
    *(u32 *)(L + 0x1fb) = (u32)ebp;
    /* 007daa16 */
    *(u32 *)(L + 0x1ff) = esi;
    /* 007daa1d */
    *(u8 *)(L + 0x203) = pb;
    if (flag == 0) {
      /* 007daa4a-007daa54: ECX = [self], src = L+0x130, count = 0xd4 */
      obj = *(u32 *)(self + 0);
      (void)cm_fn_0068d0d0((volatile u8 *)(u32)obj, (u32)(L + 0x130),
                           (const u8 *)(u32)(L + 0x130), (s16)0xd4);
    }
    else {
      /* 007daa2b-007daa36: ECX = [self+4], same record and count */
      obj = *(u32 *)(self + 4);
      (void)cm_fn_0068d0d0((volatile u8 *)(u32)obj, (u32)(L + 0x130),
                           (const u8 *)(u32)(L + 0x130), (s16)0xd4);
    }
    return 1;
  }
}
