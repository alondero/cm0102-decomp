/* CM0102 C recovery: fn_0085e590 (module staff_contracts.cpp),
   preferred-image VA 0x0085e590, image base 0x00400000.
   Translated from the pinned listing; widths, signedness, call order and
   store order follow the original instructions. */
#include "cmre.h"

#define DAT_00ae1050 CM_GLOBAL(u32, 0x00ae1050)
#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00b74350 CM_GLOBAL(u32, 0x00b74350)

#define STR_00a8360c ((u32)0x00a8360c)
#define STR_009870e0 ((u32)0x009870e0)
#define STR_009870e8 ((u32)0x009870e8)

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern void CM_CDECL cm_fn_00549c90(u32 input);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u8 CM_THISCALL cm_fn_007c6830(u32 self, u32 player, u32 club, u8 nation, u32 flag);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* The destination object is reached through ECX; its layout is only known
   from the field accesses of this routine. */
#define OBJ_B(off) (*(volatile u8 *)(obj + (off)))
#define OBJ_W(off) (*(volatile u16 *)(obj + (off)))
#define OBJ_D(off) (*(volatile u32 *)(obj + (off)))

void CM_THISCALL
cm_fn_0085e590(int *param_1, int *param_2, int *param_3, u32 param_4,
               char param_5, u8 param_6, u8 param_7)
{
  /* One scratch allocation for the original frame: [0,8) is the copy target,
     [8,0x108) and [0x108,0x208) are the two text buffers. */
  u8 frame[0x208];
  u8 *obj = (u8 *)param_1;
  u32 res, a1, a2, a3, v0, v_c;

  res = cm_fn_004cf5a0(DAT_00ae1050, (u32)param_2);
  if (res == 0) {
    cm_fn_00944cff(STR_00a8360c, 0, 0, (void *)(frame + 0x108),
                   (void *)(frame + 8));
    /* 0085e5db push 0xf5; 0085e5e0 push edx ([esp+0x14]);
       0085e5e1 push eax ([esp+0x114]); 0085e5e2 call 0x90d130 (no args).
       Those three pushes are the later varargs of 0x944c9f, which also
       receives 0x90d130's EAX at 0085e5e7. */
    cm_fn_00944c9f((u32)(void *)&DAT_00ae24d0, STR_009870e8,
                   (char *)cm_fn_0090d130(),
                   (char *)(frame + 0x108),
                   (char *)(frame + 8),
                   0xf5);
    cm_fn_005e8290((const char *)STR_009870e0,
                   (const char *)(void *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    return;
  }

  a1 = (u32)param_2;
  a2 = (u32)param_3;
  a3 = param_4;

  OBJ_D(0x00) = *(volatile u32 *)a1;
  OBJ_D(0x08) = 0;
  OBJ_D(0x04) = *(volatile u32 *)a2;

  v_c = *(volatile u32 *)(res + 0xc);
  v_c = v_c * (u32)(s32)(s8)param_5;
  {
    /* imul eax,ecx / sar edx,5 / shr eax,31 / add edx,eax */
    unsigned long long wide = (unsigned long long)v_c * (unsigned long long)0x51eb851fu;
    s32 hi = (s32)(u32)(wide >> 32);
    hi = hi >> 5;
    hi = hi + (s32)((u32)hi >> 31);
    v_c = (u32)hi;
  }
  OBJ_D(0x0c) = v_c;

  /* cm_fn_00417c10 copies eight bytes from its source argument into the
     receiver and from the receiver into its destination argument. */
  cm_fn_00417c10((u32 *)(obj + 0x25), (u32 *)(frame + 0),
                 (u16 *)0x00ae2c90);
  cm_fn_00417c10((u32 *)(obj + 0x2d), (u32 *)(frame + 0), (u16 *)a3);

  {
    u8 c35 = OBJ_B(0x35);
    u32 c0c, t, t2, t3, base;

    v0 = OBJ_D(0x00);
    c0c = OBJ_D(0x0c);
    OBJ_D(0x36) = c0c;
    OBJ_B(0x35) = (u8)((c35 & 0xc6) | 6);
    t = v0 * 5u;
    OBJ_B(0x3a) = 0;
    OBJ_B(0x49) = 0;
    t2 = v0 + t * 2u;
    OBJ_D(0x4a) = 0xffffffffu;
    OBJ_B(0x44) = 0;
    OBJ_B(0x4e) = 0;
    OBJ_D(0x45) = 0;
    t3 = t2 * 5u;
    base = (u32)DAT_00ae23bc;
    cm_fn_00549c90(base + t3 * 2u);
  }

  OBJ_D(0x40) = 0;
  OBJ_B(0x4e) = 4;
  OBJ_B(0x3f) = 0;
  OBJ_B(0x3e) = 0;

  if (a2 != 0) {
    u32 p53 = *(volatile u32 *)(a2 + 0x53);
    if (p53 != 0) {
      u8 nation = *(volatile u8 *)(p53 + 0x77);
      u8 r = cm_fn_007c6830(DAT_00b74350, a1, a2, nation, 0);
      s32 sv = (s8)r;
      u8 cl = (sv > 10) ? (u8)10 : (u8)sv;
      u8 a4f = OBJ_B(0x4f);
      u8 d = (u8)(a4f ^ cl);
      d = (u8)(d & 0xf);
      d = (u8)(d ^ a4f);
      OBJ_B(0x4f) = d;
    }
  }

  OBJ_W(0x3b) = 0;
  OBJ_B(0x1c) = 0;
  {
    u8 a5 = param_6;
    u8 a6 = param_7;

    OBJ_B(0x1d) = 0;
    OBJ_B(0x1e) = 0;
    OBJ_B(0x1f) = 0;
    OBJ_B(0x20) = 0;
    OBJ_D(0x10) = 0xffffffffu;
    OBJ_D(0x14) = 0xffffffffu;
    OBJ_D(0x18) = 0xffffffffu;
    OBJ_D(0x21) = 0;
    /* 0085e729 mov al,[esp+0x22c] (param_6) -> [esi+0x1c];
       0085e70a mov cl,[esp+0x230] (param_7) -> [esi+0x1d]. */
    OBJ_B(0x1c) = a5;
    OBJ_B(0x1d) = a6;
  }
}
