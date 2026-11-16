/* fn_0043a900 — preferred-image VA 0x0043a900, image base 0x00400000.
   Pinned binary: CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Module: comp\leagues\bra_reg_rio.cpp.

   Held-concern note: the only virtual dispatch in the listing is
   `0043aa03 mov edx,[esi] ... 0043aa14 call dword ptr [edx+0x3c]`
   (pinned slot 0x00968644 -> 0x00439d40). The source below does not name that
   slot target and needs no inventory entry for it: it reads the vtable slot at
   run time from the caller-supplied object and calls the loaded pointer with
   the original argument order and thiscall receiver. Whatever object the
   fixture supplies is dispatched exactly as the original instructions do. */

#include "cmre.h"

typedef volatile u32 cm_vu32;
typedef volatile u16 cm_vu16;
typedef volatile u8 cm_vu8;
typedef volatile s16 cm_vs16;

#define CM_LD32(a) (*(cm_vu32 *)(u32)(a))
#define CM_ST32(a, v) (*(cm_vu32 *)(u32)(a) = (u32)(v))
#define CM_LD16(a) (*(cm_vu16 *)(u32)(a))
#define CM_LD16S(a) (*(cm_vs16 *)(u32)(a))
#define CM_ST8(a, v) (*(cm_vu8 *)(u32)(a) = (u8)(v))

/* 0x0043a900 body: 539 bytes, plain `ret` (ret 0); ECX holds the object.
   EDX is not read before being written anywhere in the body. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
/* Three cdecl arguments: the `add esp, 0x24` at 0x0043aadb accounts for
   3 x 3 dwords, and all three pushes at 0x0043aaaf/0x0043aab4/0x0043aab5
   immediately precede this call (legacy `(void)` record was a placeholder). */
extern char *cm_fn_0090d130(void *arg1, void *arg2, u32 arg3);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0051c800(u32 self, volatile u32 round_arg);
extern u32 CM_THISCALL cm_fn_00522e00(
    u32 self, u32 owner, s16 count, u32 clubs, s16 rounds, u32 competition,
    u32 schedule, u16 year, u8 flag_a, u8 flag_b, u16 round_flag, u8 flag_c,
    u32 builder, u32 numbered, u32 value_38, u32 club_flags);

/* SEH registration record: `push -1; push 0x954a5e; push fs:[0];
   mov fs:[0],esp` at 0x0043a900..0x0043a90e, undone at 0x0043ab02/0x0043ab0d. */
typedef struct {
  void *prev;
  void *handler;
  u32 state;
} cm_seh_frame;

void CM_FASTCALL cm_fn_0043a900(u32 self)
{
  /* One scratch block for the bytes below L, keeping the original addresses:
       L+0x000  3-byte initialiser record {u8 0, u8 1, u16 0}
       L+0x004  virtual-call out slot  (lea [esp+0x14] at 0x0043aa05)
       L+0x008  virtual-call out slot  (lea [esp+0x1c] at 0x0043aa0a)
       L+0x00c  allocation of 0xb2      (mov [esp+0x1c],eax at 0x0043aa26)
       L+0x010/0x110/0x210/0x310  four 256-byte buffers
     A 4-byte struct keeps the byte/byte/word stores of the listing. */
  struct {
    u8 f0;
    u8 f1;
    u16 f2;
    u32 slot4;
    u32 slot8;
    u32 alloc_b2;
    u8 buf1[0x100];
    u8 buf2[0x100];
    u8 buf3[0x100];
    u8 buf4[0x100];
  } S;
  volatile cm_seh_frame frame;
  volatile u32 *p8;
  u32 obj3;
  u32 vt_result;
  u32 second;
  u32 created;
  u32 first;
  u32 a1;
  u32 a2;
  u32 line;
  s16 i;

  frame.state = 0xffffffffu;
  frame.handler = (void *)0x00954a5eu;
  frame.prev = CM_FS(void *, 0);
  CM_FS(void *, 0) = (void *)(u32)&frame;

  S.f0 = 0;
  S.f1 = 1;
  S.f2 = 0;

  /* 0x0043a923: push 8; call 0x944e46 */
  p8 = (volatile u32 *)(u32)cm_fn_00944e46(8);
  if (p8 == (volatile u32 *)0) {
    /* 0x0043a941 */
    cm_fn_00944cff(0x0098b014u, 0, 0, S.buf1, S.buf3);
    a1 = (u32)S.buf1;
    a2 = (u32)S.buf3;
    line = 0x226;
    goto report;
  }

  /* 0x0043a973: p8[0] = *(*((u32*)*(u32*)(self+0xc)) + 0xb1) */
  obj3 = CM_LD32(self + 0x0c);
  {
    u32 vt = CM_LD32(obj3);
    u32 pa = CM_LD32(vt + 0xb1);
    p8[0] = CM_LD32(pa);
  }
  /* 0x0043a982 */
  {
    u32 o = CM_LD32(self + 0x0c);
    u32 q = CM_LD32(o + 4);
    u32 pb = CM_LD32(q + 0xb1);
    second = CM_LD32(pb);
  }
  /* 0x0043a990 reads [edi] before 0x0043a994 stores [edi+4];
     the branch uses that saved value (cmp at 0x0043a992). */
  first = p8[0];
  p8[1] = second;
  if (first == second) {
    /* 0x0043a999 */
    u32 o;
    u32 vb;
    u8 five2 = 5;

    o = CM_LD32(self + 0x0c);
    CM_ST32(self + 0x30, 1);
    CM_ST32(self + 0x2c, 2);
    vb = CM_LD32(o);
    CM_ST8(CM_LD32(vb + 0xb1) + 0x37, five2);
    i = 0;
    CM_ST8(CM_LD32(CM_LD32(CM_LD32(self + 0x0c) + 4) + 0xb1) + 0x37, five2);
    if (CM_LD16S(self + 0x3e) <= 0) {
      goto done;
    }
    do {
      /* 0x0043a9d2 .. 0x0043a9fc: stride 59 = 0x3b, base *(u32*)(self+0xb1) */
      first = p8[0];
      {
        u32 base = CM_LD32(self + 0xb1);
        u32 elem = base + (u32)((s32)i * 59);
        if (CM_LD32(elem) == first) {
          CM_ST8(elem + 0x37, five2);
        }
      }
      i = (s16)(i + 1);
    } while (i < CM_LD16S(self + 0x3e));
    goto done;
  }

  /* 0x0043aa03..0x0043aa14: thiscall through vtable slot 0x3c.
     Pushes: 0, &slot4, &slot8, 2 with ECX = esi (the object). */
  vt_result =
      ((u32(CM_THISCALL *)(u32, u32, void *, void *, u32))CM_LD32(
           CM_LD32(self) + 0x3c))(self, 2, (void *)&S.slot8, (void *)&S.slot4,
                                  0);

  /* 0x0043aa17: push 0xb2; call 0x944cf1 */
  created = (u32)cm_fn_00944cf1(0xb2);
  S.alloc_b2 = created;
  frame.state = 0;
  if (created != 0) {
    u32 comp;
    u32 yw;

    /* 0x0043aa49 `mov ecx,[esi+4]`, 0x0043aa3e `mov dx,word [esi+0x40]`:
       the year argument is the partial-register artefact of the original
       (upper half still the address in EDX, lower half the word), kept as is. */
    comp = CM_LD32(self + 4);
    yw = ((u32)&S & 0xffff0000u) | (u32)CM_LD16(self + 0x40);
    /* receiver ECX = the 0xb2 allocation (0x0043aa5c); 15 pushed dwords. */
    created = cm_fn_00522e00(created, self, 2, (u32)p8, (s16)S.slot8, comp,
                             vt_result, (u16)yw, 2, 1, (u16)S.slot4, 0x14, 0, 1,
                             0, (u32)&S);
  }

  /* 0x0043aa67 */
  obj3 = CM_LD32(self + 0x0c);
  frame.state = 0xffffffffu;
  CM_ST32(obj3 + 8, created);
  {
    u32 o = CM_LD32(self + 0x0c);
    u32 c = CM_LD32(o + 8);

    if (c != 0) {
      /* 0x0043aae6: ECX = the stored value (0x0043aa7b), stack arg 0 */
      cm_fn_0051c800(c, 0);
      cm_fn_009452ca(vt_result);
      cm_fn_009452ca((u32)p8);
      CM_ST32(self + 0x30, 2);
    } else {
      /* 0x0043aa82 */
      cm_fn_00944cff(0x0098b014u, 0, 0, S.buf4, S.buf2);
      a1 = (u32)S.buf4;
      a2 = (u32)S.buf2;
      line = 0x250;
      goto report;
    }
  }
  goto done;

report:
  /* 0x0043aab6..0x0043aac6. The format at 0x009870e8 is `v%s %s%s%d` and only
     one vararg is pushed; the other three conversions read the argument slots
     left behind by the call above, which at this point still hold that call's
     three pushed arguments (L-0x28 buf-a, L-0x24 buf-b, L-0x20 line). They are
     passed explicitly so the formatting call receives the same values. */
  {
    u32 s = (u32)cm_fn_0090d130((void *)a1, (void *)a2, line);

    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, s, a1, a2, line);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
  }

done:
  /* 0x0043ab02: restore the saved fs:[0] */
  CM_FS(void *, 0) = frame.prev;
}
