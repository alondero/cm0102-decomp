#include "cmre.h"

/* fn_00549a30, preferred-image VA 0x00549a30, image base 0x00400000, binary
   3.9.68 (SHA-256 0c78e7da...f86).  cdecl entry: every exit is a plain `ret`
   with 0 stack bytes popped, and both arguments are read from their
   caller-owned homes: 0x549a36 (arg1, [esp+0xd0] before the pushes) and
   0x549acc / 0x549b4a (arg1 / arg2 at [esp+0xe0] / [esp+0xe4] after the four
   saved registers).

   Frame (L = ESP immediately after `sub esp, 0xcc`):
     L+0x00  s32 running sum, initialised to 0 at 0x549a45 (before the arg test)
     L+0x04  u32[50] selected record pointers, filled at 0x549a7d, 0x549a80
     L+0xcc  top of frame: the 50-entry table exactly fills the frame.
   One block keeps the array's adjacency to the sum (the array base is L+4). */

#define DAT_00ae2364 CM_GLOBAL(int, 0x00ae2364)
#define DAT_00ae2d14 CM_GLOBAL(u32, 0x00ae2d14)

/* Call-site map for 0x945333: four pushed arguments (base, count, width,
   compar) which the caller
   pops with `add esp, 0x10` at 0x549a9b.  The call site is 0x549a96:
     push 0x549c30 (compar) ; push 4 (width) ; lea eax,[esp+0x1c] (base)
     push esi (count) ; push eax (base) ; call
   EAX = L+4 and EDX = the probe pointer after the 50-iteration fill loop
   (arg1 + 0xd7 + 50*4 = arg1 + 0x19f) at the moment of the call. */
/* regparm(2) carries the two register inputs without shifting the four
   stack operands: 0x549a90 sets EAX=L+4, and the last 0x549a83 increment
   leaves EDX=arg1+0x19f. */
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
cm_fn_00945333(u32 eax_in, u32 edx_in, volatile u32 base_arg,
                                                  volatile u32 count_arg,
                                                  volatile u32 width_arg,
                                                  volatile u32 compare_arg);

/* Authored comparator from the instruction bytes at VA 0x00549c30 in
   the supplied listing. In particular, the second record is subtracted
   from the first in reverse argument order, and nulls are asymmetric.
   Binding this callback to C keeps qsort's indirect CALL in owned code;
   the case-0 escape address 5545008 is exactly original VA 0x00549c30. */
static s32 CM_CDECL cm_fn_00549c30(u32 left, u32 right)
{
  u32 a = *(volatile u32 *)left;
  u32 b = *(volatile u32 *)right;
  u32 da, db;
  s16 x, y;
  if (!a) return 1;
  da = *(volatile u32 *)(a + 0x61u);
  if (!da) return 1;
  if (!b) return -1;
  db = *(volatile u32 *)(b + 0x61u);
  if (!db) return -1;
  x = *(volatile s16 *)(db + 5u);
  y = *(volatile s16 *)(da + 5u);
  if (x != y) return (s32)x - (s32)y;
  x = *(volatile s16 *)(db + 11u);
  y = *(volatile s16 *)(da + 11u);
  return (s32)x - (s32)y;
}

u8 CM_CDECL cm_fn_00549a30(u32 arg1, u32 arg2)
{
  u32 frame[51];
  u32 count;
  u32 probe;
  u32 entry;
  u32 detail;
  u32 record;
  u32 table;
  u32 idx;
  s32 limit;
  s32 slots;
  s32 bound;
  s32 iterations;
  s32 i;
  s32 sum;
  s32 quotient;
  s16 rating;
  s16 other;
  s16 delta;
  u8 level;

  /* 0x549a45: mov dword ptr [esp + 8], edi (edi = 0) -> sum home = 0 */
  frame[0] = 0u;
  /* 0x549a43 / 0x549a49: cmp edx, edi ; jne 0x549a56 */
  if (arg1 == 0u) {
    /* 0x549a4b..0x549a55: pop edi ; mov al,3 ; pop esi ; add esp,0xcc ; ret */
    return 3u;
  }

  /* 0x549a58: mov ebp, dword ptr [0xae2364] (limit, hoisted out of the loop) */
  limit = DAT_00ae2364;
  /* 0x549a62: add edx, 0xd7 */
  probe = arg1 + 0xd7u;
  /* 0x549a68: mov ebx, 0x32 (50 slots) */
  slots = 0x32;
  /* 0x549a41: xor esi, esi */
  count = 0u;
  do {
    /* 0x549a6d: mov eax, dword ptr [edx] */
    entry = *(volatile u32 *)probe;
    /* 0x549a6f: test eax, eax ; je 0x549a83 */
    if (entry != 0u) {
      /* 0x549a73: cmp dword ptr [eax + 0x61], 0 ; je 0x549a83 */
      if (*(volatile u32 *)(entry + 0x61u) != 0u) {
        /* 0x549a79: cmp dword ptr [eax], ebp ; jge 0x549a83 (signed) */
        if ((s32)*(volatile u32 *)entry < limit) {
          /* 0x549a7d: mov dword ptr [ecx], eax ; 0x549a7f inc esi ;
             0x549a80 add ecx, 4 */
          frame[1u + count] = entry;
          count = count + 1u;
        }
      }
    }
    /* 0x549a83: add edx, 4 ; 0x549a86 dec ebx ; 0x549a87 jne 0x549a6d */
    probe = probe + 4u;
    slots = slots - 1;
  } while (slots != 0);

  /* 0x549a89..0x549a9b: the four pushed arguments plus EAX/EDX inputs, then
     `add esp, 0x10`.  The call is kept: it orders the selected pointers in
     place through the comparator at 0x00549c30. All four stack inputs
     are retained at this explicit original-code boundary. */
   cm_fn_00945333((u32)(&frame[1]), probe, (u32)(&frame[1]), count, 4u,
                  (u32)cm_fn_00549c30);

  /* 0x549a9e: xor eax, eax (i = 0) */
  i = 0;
  /* 0x549aa0: lea edx, [esi - 1] (count - 1, computed before esi is reused) */
  bound = (s32)(count - 1u);
  /* 0x549abd: edi counts the iterations actually taken. */
  iterations = 0;
  for (;;) {
    /* 0x549aa7: cmp eax, edx ; jge 0x549ac8 */
    if (i >= bound) {
      break;
    }
    /* 0x549aab: mov esi, dword ptr [ecx] (ecx walks L+4 in steps of 4) */
    record = frame[1u + (u32)i];
    /* 0x549aad: mov ebx, dword ptr [esp + 0x10] - the sum is reloaded from
       its home on every iteration */
    sum = (s32)frame[0];
    /* 0x549ab4: mov esi, dword ptr [esi + 0x61] */
    detail = *(volatile u32 *)(record + 0x61u);
    /* 0x549ab7: movsx esi, word ptr [esi + 5] */
    rating = *(volatile s16 *)(detail + 5u);
    /* 0x549abb: add ebx, esi */
    sum = sum + (s32)rating;
    /* 0x549abd: inc edi */
    iterations = iterations + 1;
    /* 0x549abe: inc eax */
    i = i + 1;
    /* 0x549abf: mov dword ptr [esp + 0x10], ebx */
    frame[0] = (u32)sum;
    /* 0x549ac3: cmp eax, 0x10 ; jl 0x549aa7 (signed, at most 16 iterations) */
    if (i < 0x10) {
      continue;
    }
    break;
  }

  /* 0x549ac8: test edi, edi ; jne 0x549b0c */
  if (iterations != 0) {
    /* 0x549b0c: mov eax, dword ptr [esp + 0x10] ; cdq ; idiv edi */
    quotient = sum / iterations;
    /* 0x549b13: cmp eax, 0x9b ; jle ... (signed ladder) */
    if (quotient > 0x9b) {
      level = 6u;
    }
    else if (quotient > 0x87) {
      level = 5u;
    }
    else if (quotient > 0x6e) {
      level = 4u;
    }
    else if (quotient > 0x55) {
      level = 3u;
    }
    else if (quotient > 0x3c) {
      level = 2u;
    }
    else {
       /* 0x549b44: cmp eax, 0x28 ; setg al (no increment on this path). */
       level = (u8)(quotient > 0x28);
    }
  }
  else {
    /* 0x549acc: mov ecx, dword ptr [esp + 0xe0] (arg1 home)
       0x549ad3: mov ax, word ptr [ecx + 0x80] */
    other = *(volatile s16 *)(arg1 + 0x80u);
    /* 0x549ada: cmp ax, 0x1c52 ; jle ... (signed word ladder) */
    if (other > 0x1c52) {
      level = 6u;
    }
    else if (other > 0x1676) {
      level = 5u;
    }
    else if (other > 0x128e) {
      level = 4u;
    }
    else if (other > 0xcb2) {
      level = 3u;
    }
    else {
      /* 0x549b02: cmp ax, 0x8ca ; setg al ; inc eax */
      level = (u8)((other > 0x8ca) + 1);
    }
  }

  /* 0x549b4a: mov edi, dword ptr [esp + 0xe4] (arg2 home) ; test ; je */
  if (arg2 != 0u) {
    /* 0x549b59: mov cx, word ptr [edi + 0x80] */
    rating = *(volatile s16 *)(arg2 + 0x80u);
    /* 0x549b60: cmp cx, 0x1c52 ; jle 0x549b91 */
    if (rating > 0x1c52) {
      /* 0x549b67: mov ebx, dword ptr [esp + 0xe0]
         0x549b6e: mov dx, word ptr [ebx + 0x80] */
      delta = *(volatile s16 *)(arg1 + 0x80u);
      /* 0x549b75: cmp dx, 0x109a ; jge 0x549b82 */
      if (!(delta >= 0x109a)) {
        /* 0x549b7c: cmp al, 2 ; jle 0x549b82 */
        if (!((s8)level <= 2)) {
          /* 0x549b80: add al, 0xfe */
          level = (u8)(level + 0xfeu);
        }
      }
      /* 0x549b82: cmp dx, 0x1c52 ; jge 0x549ba5 */
      if (!(delta >= 0x1c52)) {
        /* 0x549b89: test al, al ; jle 0x549ba5 */
        if ((s8)level > 0) {
          /* 0x549b8d: dec al */
          level = (u8)(level - 1u);
        }
      }
    }
    else {
      /* 0x549b91: cmp cx, 0x109a ; jge 0x549b9e */
      if (!(rating >= 0x109a)) {
        /* 0x549b96: cmp al, 5 ; jge 0x549b9e */
        if ((s8)level < 5) {
          /* 0x549b9c: inc al */
          level = (u8)(level + 1u);
        }
      }
    }

    /* 0x549ba5: mov edx, dword ptr [edi + 0x57] ;
       0x549ba8: mov esi, dword ptr [ebx + 0x57] ; cmp ; jne 0x549c0a */
    if (*(volatile u32 *)(arg2 + 0x57u) == *(volatile u32 *)(arg1 + 0x57u)) {
      /* 0x549baf: sub cx, word ptr [ebx + 0x80] (16-bit, then signed cmp) */
      delta = (s16)(rating - *(volatile s16 *)(arg1 + 0x80u));
      /* 0x549bb6: mov edx, dword ptr [0xae2d14] */
      table = DAT_00ae2d14;
      /* 0x549bbc: cmp cx, 0x4e2 ; jle 0x549be3 */
      if (delta > 0x4e2) {
        /* 0x549bc3: test al, al ; jle 0x549be3 */
        if ((s8)level > 0) {
          /* 0x549bc7: mov esi, [edi] ; lea esi,[esi+esi*2] ; [edx+esi*2+4] */
          idx = (*(volatile u32 *)arg2) * 6u;
          /* 0x549bcc: cmp byte ptr [edx + esi*2 + 4], 0 ; jl 0x549be3 */
          if (!(*(volatile s8 *)(table + idx + 4u) < 0)) {
            /* 0x549bd3: mov esi, [ebx] ; lea esi,[esi+esi*2] */
            idx = (*(volatile u32 *)arg1) * 6u;
            /* 0x549bd8: cmp byte ptr [edx + esi*2 + 4], 0 ; jg 0x549be3 */
            if (!(*(volatile s8 *)(table + idx + 4u) > 0)) {
              /* 0x549bdf: dec al ; jmp 0x549c0a */
              level = (u8)(level - 1u);
              goto finished;
            }
          }
        }
      }
      /* 0x549be3: cmp cx, 0x4e2 ; jge 0x549c0a */
      if (!(delta >= 0x4e2)) {
        /* 0x549bea: cmp al, 5 ; jge 0x549c0a */
        if ((s8)level < 5) {
          /* 0x549bee: mov ecx, [ebx] ; lea ecx,[ecx+ecx*2]
             0x549bf3: mov bl, byte ptr [edx + ecx*2 + 4] ; test bl,bl ; jl */
          idx = (*(volatile u32 *)arg1) * 6u;
          if (!(*(volatile s8 *)(table + idx + 4u) < 0)) {
            /* 0x549bfb: mov ecx, [edi] ; lea ecx,[ecx+ecx*2]
               0x549c00: mov bl, byte ptr [edx + ecx*2 + 4] ; test bl,bl ; jg */
            idx = (*(volatile u32 *)arg2) * 6u;
            if (!(*(volatile s8 *)(table + idx + 4u) > 0)) {
              /* 0x549c08: inc al */
              level = (u8)(level + 1u);
            }
          }
        }
      }
    }
  }

finished:
  /* 0x549c0a: pop ebp ; 0x549c0b: pop ebx
     0x549c0c: test al, al ; jge 0x549c1b */
  if ((s8)level < 0) {
    /* 0x549c10..0x549c1a: pop edi ; xor al,al ; pop esi ; add esp,0xcc ; ret */
    return 0u;
  }
  /* 0x549c1b: cmp al, 6 ; jle 0x549c21 ; 0x549c1f: mov al, 6 */
  if ((s8)level > 6) {
    level = 6u;
  }
  /* 0x549c21..0x549c29: pop edi ; pop esi ; add esp,0xcc ; ret (AL) */
  return level;
}
