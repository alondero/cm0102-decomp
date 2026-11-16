/* fn_004ab0c0  VA 0x004ab0c0 (image base 0x00400000), 1263 bytes
   module comp/comp_stats.cpp.  Plain C reconstruction from the original
   instructions only: every width, order and boundary follows the listing.

   ret 4 + ECX read before written => thiscall with one stack argument
   (the record-count home at [esp+0x20c] on entry = ebx).

   Local scratch block: after `sub esp,0x204` the frame spans L-0x204..L-1.
     [L-0x204] (scratch+0x000) : dword home for the allocation result
                                 (`mov [esp+8],eax` with esp = L-0x20c)
     [L-0x200] (scratch+0x004) : first 256-byte buffer (lea [esp+0xc])
     [L-0x100] (scratch+0x104) : second 256-byte buffer (lea [esp+0x10c])
   Kept in one allocation with explicit offsets because the original addresses
   are adjacent; later `mov edx,[esp+0x10]` reloads the same dword home.

   FP contract of 0x004ab343/0x004ab348: cm_fn_004a2ec0 returns its result in
   ST0 and the immediately following `call 0x00945640` (__ftol) consumes that
   live ST0 and returns the 64-bit integer in EDX:EAX.  It is MSVC's lowering
   of a floating-point to integer cast (ADR 0006), so the C form is a
   `(long long)` cast of the callee's long double result: the converted value
   is exactly the callee's ST0 contents on the original constants and the
   original truncating FP environment, and only EAX (the low dword) is used,
   as `mov dword ptr [ebp+0x2fa], eax` does. */

#include "cmre.h"

/* Original callees: explicit boundaries, bound to original code. */
extern u32 CM_THISCALL cm_fn_00448110(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_00451570(volatile u8 *self, volatile u8 *source);
extern u32 CM_THISCALL cm_fn_00497910(u32 self, u32 flags);
extern u32 CM_THISCALL cm_fn_004a1f40(volatile u8 *self, volatile u8 *source);
extern u32 CM_THISCALL cm_fn_004a1f70(volatile u8 *self, volatile u8 *source);
extern u8 *CM_THISCALL cm_fn_004a1fb0(u8 *object, u8 *source);
extern u32 CM_THISCALL cm_fn_004a2050(volatile u8 *self, volatile u8 *source);
extern long double CM_STDCALL cm_fn_004a2ec0(u32 stats,volatile s8 selector);
extern u32 CM_THISCALL cm_fn_004ab5b0(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_004ab5d0(volatile u8 *self, volatile u8 *source);
extern u32 CM_THISCALL cm_fn_004ab5f0(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_004ad7f0(u32 self);
extern int CM_THISCALL cm_fn_0058f120(u32 *param_1,int param_2,int param_3);
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern u32 CM_THISCALL cm_fn_0068d1d0(u32 self);
/* 0x004ab115..0x004ab11c: three values are pushed for this call, and the
   call's result is the only argument the following `0x00944c9f` consumes
   (single `add esp,0x24` after three calls). 0x1715 stays with this call,
   which builds the file/line text. */
extern char * cm_fn_0090d130(void *arg1, void *arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
/* CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VAs (base 0x00400000): 0x004ab324 loads EDX from
   [EBP+0x14], 0x004ab327 loads EAX from [EBP+0xc]; 0x004ab32a,
   0x004ab32f, 0x004ab331, 0x004ab332 push comparator, width, count,
   base. CALL 0x004ab333 is followed by ADD ESP,0x10 at 0x004ab338.
   regparm(2) transports the first two formals in EAX/EDX while cdecl
   leaves exactly the four remaining DWORD operands caller-cleaned.
   This describes this original boundary, not callee composition. */
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
cm_fn_00945333(u32 eax_in, u32 edx_in, volatile u32 base_arg,
              volatile u32 count_arg, volatile u32 width_arg,
              volatile u32 compare_arg);
extern /* Library Function - Single Match cm_fn_00945640 Library: Visual Studio */ long long cm_fn_00945640(void);

/* [ebx]: the caller-owned record count.  Reloaded at each original site
   (`mov edx,[ebx]` appears repeatedly); volatile keeps those reloads. */
#define CNT (*(volatile u32 *)param_2)

void * CM_THISCALL cm_fn_004ab0c0(u32 self, u32 *param_2)
{
  volatile u8 *s = (volatile u8 *)self;
  u8 scratch[0x204];
  u8 *pv;
  u8 *d;
  u8 *rec;
  u32 idx;
  u32 v;
  u32 p;
  u32 q;
  long double t;
  int *list;
  char *smsg;

  CNT = 0;                                      /* mov dword ptr [ebx],0 */
  pv = (u8 *)(u32)cm_fn_00944e46(0x4b8fc);      /* call 0x00944e46 */
  *(u32 *)scratch = (u32)pv;                    /* mov dword ptr [esp+8],eax */
  if (pv == (u8 *)0) {
    cm_fn_00944cff(0x99ca94u, 0, 0, (void *)&scratch[0x104], (void *)&scratch[4]);
    smsg = cm_fn_0090d130((void *)&scratch[0x104], (void *)&scratch[4], 0x1715u);
    /* The single `add esp,0x24` after the last three calls means the three
       pushes below this call are still live on the original stack when
       0x00944c9f runs: its vararg stream is (result, B, A, 0x1715). */
    cm_fn_00944c9f(0xae24d0u, 0x9870e8u, (u32)smsg,
                   (u32)(void *)&scratch[0x104], (u32)(void *)&scratch[4],
                   0x1715u);
    cm_fn_005e8290((const char *)0x9870e0u, (const char *)0xae24d0u, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
    return (void *)0;
  }

  idx = CNT;
  *(u16 *)(pv + idx * 0x26bu) = 0;              /* mov word ptr [ecx+eax],0 */
  d = pv + idx * 0x26bu + 2;
  *(u32 *)(d + 0)    = *(volatile u32 *)(s + 0x26);
  *(u8  *)(d + 4)    = *(volatile u8  *)(s + 0x2a);
  *(u32 *)(d + 5)    = *(volatile u32 *)(s + 0x2b);
  *(u32 *)(d + 9)    = *(volatile u32 *)(s + 0x2f);
  *(u32 *)(d + 0xd)  = *(volatile u32 *)(s + 0x33);
  *(u32 *)(d + 0x11) = *(volatile u32 *)(s + 0x37);
  *(u32 *)(d + 0x15) = *(volatile u32 *)(s + 0x3b);
  *(u32 *)(d + 0x19) = *(volatile u32 *)(s + 0x3f);
  *(u32 *)(d + 0x1d) = *(volatile u32 *)(s + 0x43);
  *(u32 *)(d + 0x21) = *(volatile u32 *)(s + 0x47);
  *(u32 *)(d + 0x25) = *(volatile u32 *)(s + 0x4b);
  cm_fn_00448110((u32)(d + 0x29), self + 0x4f);
  *(u32 *)(d + 0x48) = *(volatile u32 *)(s + 0x6e);
  cm_fn_00448110((u32)(d + 0x4c), self + 0x72);
  *(u32 *)(d + 0x6b) = *(volatile u32 *)(s + 0x91);
  *(u32 *)(d + 0x6f) = *(volatile u32 *)(s + 0x95);
  *(u16 *)(d + 0x73) = *(volatile u16 *)(s + 0x99);
  cm_fn_00448110((u32)(d + 0x75), self + 0x9b);
  cm_fn_00448110((u32)(d + 0x94), self + 0xba);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xb3), s + 0xd9);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xcb), s + 0xf1);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xe3), s + 0x109);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xfb), s + 0x121);
  cm_fn_004a1f40((volatile u8 *)(d + 0x113), s + 0x139);
  cm_fn_00451570((volatile u8 *)(d + 0x132), s + 0x158);
  cm_fn_004a1f40((volatile u8 *)(d + 0x16a), s + 0x190);
  cm_fn_004a2050((volatile u8 *)(d + 0x189), s + 0x1af);
  cm_fn_004a1f40((volatile u8 *)(d + 0x1ac), s + 0x1d2);
  cm_fn_004a1f70((volatile u8 *)(d + 0x1cb), s + 0x1f1);
  cm_fn_004a1fb0((u8 *)(d + 0x1eb), (u8 *)(s + 0x211));
  cm_fn_004a1fb0((u8 *)(d + 0x22a), (u8 *)(s + 0x250));
  CNT = CNT + 1u;                               /* inc/mov [ebx] */

  if (*(volatile u32 *)(s + 0x2fa) == 0xffffffffu) {
    if (*(volatile u32 *)(s + 0xc) != 0) {
      if (*(volatile u8 *)(s + 0x18) != 2) {
        *(volatile u8 *)(s + 0x18) = 2;
        CM_GLOBAL(u32, 0x00adae10) = self;
        q = *(volatile u32 *)(s + 0x14);
        p = *(volatile u32 *)(s + 0xc);
        cm_fn_00945333(p, q, p, q, 0x76, 0x004ad1a0u);
      }
      p = *(volatile u32 *)(s + 0xc);
      t = cm_fn_004a2ec0(p, 2);                 /* call 0x004a2ec0, result in ST0 */
      v = (u32)(long long)t;                    /* call 0x00945640 (__ftol), EAX */
      *(volatile u32 *)(s + 0x2fa) = v;
      *(volatile u32 *)(s + 0x2fe) = *(volatile u32 *)p;
      *(volatile u16 *)(s + 0x302) = 0;
    }
  }

  {
    /* fld dword [ebp+0x418]; fcomp qword [0x00968948]; fnstsw ax;
       test ah,0x40; je -> the call happens when C3 is set, i.e. when the
       operands are equal or unordered (NaN). */
    float f = *(volatile float *)(s + 0x418);
    double c = CM_GLOBAL(double, 0x00968948);
    if (!((double)f < c) && !((double)f > c)) {
      cm_fn_004ad7f0(self);
    }
  }

  idx = CNT;
  p = *(volatile u32 *)(s + 0x502);
  *(u16 *)(pv + idx * 0x26bu) = *(volatile u16 *)(p + 0x40);
  d = (u8 *)(*(u32 *)scratch) + idx * 0x26bu + 2;
  *(u32 *)(d + 0)    = *(volatile u32 *)(s + 0x28f);
  *(u8  *)(d + 4)    = *(volatile u8  *)(s + 0x293);
  *(u32 *)(d + 5)    = *(volatile u32 *)(s + 0x294);
  *(u32 *)(d + 9)    = *(volatile u32 *)(s + 0x298);
  *(u32 *)(d + 0xd)  = *(volatile u32 *)(s + 0x29c);
  *(u32 *)(d + 0x11) = *(volatile u32 *)(s + 0x2a0);
  *(u32 *)(d + 0x15) = *(volatile u32 *)(s + 0x2a4);
  *(u32 *)(d + 0x19) = *(volatile u32 *)(s + 0x2a8);
  *(u32 *)(d + 0x1d) = *(volatile u32 *)(s + 0x2ac);
  *(u32 *)(d + 0x21) = *(volatile u32 *)(s + 0x2b0);
  cm_fn_004ab5b0((u32)(d + 0x25), self + 0x2b4);
  cm_fn_004ab5b0((u32)(d + 0x48), self + 0x2d7);
  *(u32 *)(d + 0x6b) = *(volatile u32 *)(s + 0x2fa);
  *(u32 *)(d + 0x6f) = *(volatile u32 *)(s + 0x2fe);
  *(u16 *)(d + 0x73) = *(volatile u16 *)(s + 0x302);
  cm_fn_00448110((u32)(d + 0x75), self + 0x304);
  cm_fn_00448110((u32)(d + 0x94), self + 0x323);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xb3), s + 0x342);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xcb), s + 0x35a);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xe3), s + 0x372);
  cm_fn_004ab5d0((volatile u8 *)(d + 0xfb), s + 0x38a);
  cm_fn_004a1f40((volatile u8 *)(d + 0x113), s + 0x3a2);
  cm_fn_00451570((volatile u8 *)(d + 0x132), s + 0x3c1);
  cm_fn_004a1f40((volatile u8 *)(d + 0x16a), s + 0x3f9);
  cm_fn_004a2050((volatile u8 *)(d + 0x189), s + 0x418);
  cm_fn_004a1f40((volatile u8 *)(d + 0x1ac), s + 0x43b);
  cm_fn_004a1f70((volatile u8 *)(d + 0x1cb), s + 0x45a);
  cm_fn_004a1fb0((u8 *)(d + 0x1eb), (u8 *)(s + 0x47a));
  cm_fn_004a1fb0((u8 *)(d + 0x22a), (u8 *)(s + 0x4b9));
  CNT = CNT + 1u;

  list = (int *)cm_fn_0058f120((u32 *)CM_GLOBAL(u32, 0x00adae14),
                               *(volatile u32 *)(s + 0x4f8),
                               *(volatile u32 *)(s + 0x4fc));
  if (list != (int *)0) {
    rec = (u8 *)list;
    *(u32 *)(rec + 0xa) = *(u32 *)(rec + 6);
    p = cm_fn_0068d1d0((u32)rec);
    while (p != 0) {
      idx = CNT;
      if (idx == 0x1f4u) break;
      v = p + 2;
      d = (u8 *)(*(u32 *)scratch) + idx * 0x26bu;
      CNT = idx + 1u;
      *(u16 *)d = *(u16 *)(v - 2);
      cm_fn_004ab5f0((u32)(d + 2), v);
      p = cm_fn_0068d1d0((u32)rec);
    }
    cm_fn_00497910((u32)rec, 1);
  }
  return (void *)(*(u32 *)scratch);
}
