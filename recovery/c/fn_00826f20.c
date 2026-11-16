/* fn_00826f20 — preferred-image VA 0x00826f20 (RVA 0x00426f20), module setup.cpp.
   Pinned binary: Championship Manager 01/02 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Convention: ret 0 (no stack bytes popped) -> cdecl, two 32-bit arguments,
   32-bit EAX result (callers read no EDX). */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00b67a44 CM_GLOBAL(int *, 0x00b67a44)
#define DAT_00dd7118 CM_GLOBAL(s32, 0x00dd7118)

/* C callees (recovered tiers). */
extern u32 CM_CDECL cm_fn_0053b4f0(u32 nation);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* Original-code boundaries. Receivers come from the listing's ECX setup:
   0x00826f4f mov ecx, dword ptr [0x00ae2c74]   before call 0x005c64e0
   0x00826f77 / 0x00826fca / 0x0082705a mov ecx, dword ptr [0x00b67a44]
   The 69b790/69ebc0/5c64e0 sites pop their argument bytes in the callee
   (no `add esp` after the call), so they are thiscall with the receiver in ECX. */
extern void CM_THISCALL cm_fn_005c64e0(u8 *self, const volatile u32 *key_pointer);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern void CM_THISCALL cm_fn_0069b790(int *self, int *object, int mode, u32 arg4, int arg5);
extern void CM_THISCALL cm_fn_0069ebc0(int *self, int *arg1, int *arg2);

/* Original-code boundary; three pushes precede the call at 0x008270cb and the
   single `add esp, 0x24` at 0x008270f1 covers this call and the next three. */
extern char * CM_CDECL cm_fn_0090d130(void *arg1, void *arg2, u32 arg3);

u32 cm_fn_00826f20(u32 param_1, u32 param_2)
{
  /* One 0x204 scratch frame as after `sub esp, 0x204` at 0x00826f20.
     Saved esi/edi keep esp at L-8; `push ebx` at 0x00826f82 (success path only)
     puts it at L-12, so `[esp+0xc]` is L+0x000 on the success path and L+0x004
     on the null-argument path.
     L+0x000 : fild dword temp ([esp+0xc] with ebx pushed)
     L+0x004 : first char buffer ([esp+0xc] without ebx)
     L+0x104 : second char buffer ([esp+0x10c]) */
  u8 scratch[0x204];

  if (param_1 == 0) goto error;
  if (param_2 == 0) goto error;

  /* 0x00826f46: the +0xcf status dword of the argument object, else of the
     object cm_fn_00540a50 resolves for it. */
  if (*(u32 *)(void *)(param_2 + 0xcfu) != 0) {
    /* pushes 1, 0, 3, esi */
    cm_fn_0069b790(DAT_00b67a44, (int *)(void *)param_2, 3, 0, 1);
  } else {
    u32 resolved = cm_fn_00540a50(param_2, 0, 1);
    if (resolved != 0 && *(u32 *)(void *)(resolved + 0xcfu) != 0) {
      cm_fn_0069b790(DAT_00b67a44, (int *)(void *)resolved, 3, 0, 1);
    }
  }

  if (cm_fn_0053b900((u32 *)(void *)param_2) != 0) {
    if (cm_fn_005409b0((u32 *)(void *)param_2) == 0) {
      u32 owner = cm_fn_0053b4f0(*(u32 *)(void *)(param_2 + 0x53u));
      if (*(u32 *)(void *)(owner + 0xcfu) != 0) {
        /* pushes 0, 0, 1, 1, ebx */
        if (cm_fn_00600a50(owner, 1, 1, 0, 0) == 0) {
          /* push 1; push eax(==0); push 3; push ebx */
          cm_fn_0069b790(DAT_00b67a44, (int *)(void *)owner, 3, 0, 1);
        }
      }
    }
  }

  /* 0x00826fdb: signed 16-bit field, /10 by 0x66666667 magic then +3000, stored
     as a 16-bit word at +0xa. The listing performs the whole sequence twice
     with a fresh load of the field each time.
     Pointer DWORD reloads at preferred-image VAs 0x00826fe9, 0x0082700d
     and 0x00827043 must survive overlapping WORD stores at 0x00826ffb
     and 0x0082701f (binary SHA-256 as above). Volatile accesses retain
     these widths and their order even if a destination aliases +0x69. */
  {
    s16 v = *(volatile s16 *)(void *)(param_2 + 0x80u);
    s32 q = (s32)v / 10;
    u32 destination = *(volatile u32 *)(void *)(param_1 + 0x69u);
    *(volatile u16 *)(void *)(destination + 0xau) = (u16)(q + 3000);
  }
  {
    s16 v = *(volatile s16 *)(void *)(param_2 + 0x80u);
    s32 q = (s32)v / 10;
    u32 destination = *(volatile u32 *)(void *)(param_1 + 0x69u);
    *(volatile u16 *)(void *)(destination + 0x8u) = (u16)(q + 3000);
  }

  /* 0x00827023: movsx word [esi+0x80] -> dword temp [esp+0xc];
     0x0082702e fild dword [esp+0xc]; 0x00827032 fmul qword [0x00968e00];
     0x00827038 fadd qword [0x0096a5f0]; 0x0082703e call 0x00945640 (__ftol).
     The live ST0 is exactly the MSVC lowering of a long-double expression whose
     value is converted to 64-bit and then narrowed to AX at 0x00827047. */
  {
    s32 iv = (s32)*(volatile s16 *)(void *)(param_2 + 0x80u);
    long double t;
    long long converted;
    u32 destination;
    *(s32 *)(void *)(scratch + 0x000) = iv;
    t = (long double)*(s32 *)(void *)(scratch + 0x000) * CM_GLOBAL(double, 0x00968e00)
        + CM_GLOBAL(double, 0x0096a5f0);
    converted = (long long)t;
    destination = *(volatile u32 *)(void *)(param_1 + 0x69u);
    *(volatile u16 *)(void *)(destination + 0xcu) = (u16)converted;
  }

  *(u8 *)(void *)(param_1 + 0x59u) = 0x14;

  /* ecx = dword [0x00ae2c74]; one stacked argument (esi). */
  cm_fn_005c64e0((u8 *)(void *)(u32)CM_GLOBAL(u32, 0x00ae2c74),
                 (const volatile u32 *)(void *)param_2);
  /* ecx = dword [0x00b67a44]; stacked arguments esi then edi. */
  cm_fn_0069ebc0(DAT_00b67a44, (int *)(void *)param_2, (int *)(void *)param_1);

  if (DAT_00dd7118 == 0) {
    /* 0x00827071: esi = dword [esi]; eax = dword [0x00b67a44]; eax = dword [eax];
       index = esi*0x49 (lea [esi+esi*8] then lea [esi+ecx*8]); clear bit 0 of
       the byte at base + index*0x49 + 2. */
    u32 idx = *(u32 *)(void *)param_2;
    u32 base = *(u32 *)(void *)DAT_00b67a44;
    u8 *p = (u8 *)(base + idx * 0x49u + 2u);
    *p = (u8)(*p & 0xfeu);
  }

  return 1;

error:
  /* 0x0082709b: esp is L-8 here (esi, edi pushed; no ebx), so
     `lea ecx,[esp+0xc]` is L+0x004 and `lea edx,[esp+0x10c]` is L+0x104. */
  {
    void *buf_a = (void *)(scratch + 0x104);
    void *buf_b = (void *)(scratch + 0x004);
    char *written;
    /* `push ecx; push edx; push 0; push 0; push 0x00a801dc` -> arg1 is the
       path literal, arg4 is edx (L+0x104) and arg5 is ecx (L+0x004). */
    cm_fn_00944cff(0x00a801dcu, 0, 0, buf_a, buf_b);
    /* 0x008270b9: lea eax,[esp+0xc] = L+0x004, lea ecx,[esp+0x10c] = L+0x104;
       `push 0x1a4d; push eax; push ecx` -> (L+0x104, L+0x004, 0x1a4d). */
    written = cm_fn_0090d130(buf_a, buf_b, 0x1a4du);
    /* 0x008270d0..0x008270db: `push eax; push 0x009870e8; push 0x00ae24d0`.
       The single `add esp, 0x24` at 0x008270f1 is 9 dwords, covering the three
       0x0090d130 pushes, these three, and the three 0x005e8290 pushes. Because
       this routine performs no cleanup between the calls, the three
       0x0090d130 argument words are still live at [esp+0x10], [esp+0x14] and
       [esp+0x18] while 0x00944c9f runs, and its format reads them as trailing
       varargs. Omitting them makes the gate diverge on the sprintf output page
       (0x00ae2000), so they are passed explicitly to reproduce that stack
       content in listing order. This is original behaviour, not a guess. */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)(void *)written,
                   (u32)buf_a, (u32)buf_b, 0x1a4du);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)(&DAT_00ae24d0), 0);
  }
  DAT_00b67a34 = 0;
  return 0;
}
