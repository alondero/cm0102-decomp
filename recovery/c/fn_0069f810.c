/* fn_0069f810 at preferred-image VA 0x0069f810, image base 0x00400000.
   The original writes only AL on both outcomes (mov al,1 / xor al,al), so the
   reconstructed return width is u8. Callees stay explicit boundaries. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
/* Both call sites (0x0069f90c, 0x0069f9d2, 0x0069fad4) push two stack
   arguments after the ECX receiver 0x00ae2c90. */
extern int CM_THISCALL cm_fn_0054cdb0(short *param_1, u32 param_2, u32 param_3);
extern u8 CM_THISCALL cm_fn_007ae350(u32 self, u32 key_ptr);
extern long double CM_THISCALL cm_fn_00844780(u32 self, u32 p, u32 d, u32 flag,
                                             u32 mode, u32 output);
/* 0x00945640 is __ftol; reached through the (long long) casts below. */
extern long long cm_fn_00945640(void);

u8 CM_STDCALL cm_fn_0069f810(u32 param_1, u32 param_2, double param_3)
{
  u32 edx;
  u32 eax;
  u32 ecx;
  u32 edi;
  u32 v;
  long double t;
  long long ll;

  /* 0x0069f817 */
  edx = *(u32 *)(param_1 + 0x69);
  if (edx == 0) goto ret0;
  if (param_2 == 0) goto ret0;

  /* 0x0069f82e */
  if ((s32)(s16)*(u16 *)(edx + 0x0c) > 0x109a) {
    ecx = *(u32 *)(param_1 + 0x1a);
    if (ecx != 0) {
      eax = *(u32 *)(param_2 + 0x53);
      if (eax != 0) {
        if (ecx != eax) {
          if ((s32)*(s8 *)(eax + 0x85) < 0x0a) goto ret0;
        }
      }
    }
  }

  /* 0x0069f855 */
  if ((s32)(s16)*(u16 *)(edx + 0x08) > 0x186a) {
    eax = *(u32 *)(param_1 + 0x1a);
    ecx = *(u32 *)(param_2 + 0x53);
    if (eax != ecx) {
      if ((s32)*(s8 *)(param_1 + 0x3d) == 5) {
        if ((s32)(s16)*(u16 *)(param_2 + 0x80) < 0x1676) goto ret0;
      }
    }
  }

  /* 0x0069f87c */
  if (cm_fn_0053b900((u32 *)param_2) != 0) {
    eax = *(u32 *)(param_1 + 0x65);
    if (eax != 0) {
      if (*(u32 *)(eax + 0x10) == param_2) goto ret0;
      if (*(u32 *)(eax + 0x14) == param_2) goto ret0;
      if (*(u32 *)(eax + 0x18) == param_2) goto ret0;
    }
  }

  /* 0x0069f8ab */
  if ((s32)*(s8 *)(param_1 + 0x3d) == 0x0b) {
    eax = *(u32 *)(param_1 + 0x61);
    if (eax != 0) {
      if ((s32)(s16)*(u16 *)(eax + 0x0b) > 0x128e) {
        if ((s32)(s16)*(u16 *)(param_2 + 0x80) < 0x1a5e) {
          edi = cm_fn_004cf5a0(CM_GLOBAL(u32, 0xae1050), param_1);
          if (edi != 0) {
            /* 0x0069f8ed: mov al,[edi+0x4f]; shr al,4; test al,al; cmp al,3 */
            eax = (u32)(*(u8 *)(edi + 0x4f) >> 4);
            if ((s32)(s8)eax != 0) {
              if ((s32)(s8)eax <= 3) goto ret0;
            }
            /* 0x0069f8ff: push [edi+0x29]; push [edi+0x25]; ecx=0xae2c90 */
            ecx = *(u32 *)(edi + 0x29);
            edx = *(u32 *)(edi + 0x25);
            v = (u32)cm_fn_0054cdb0((short *)0xae2c90, edx, ecx);
            if ((s32)v < 0xb4) goto ret0;
            if ((s32)*(s8 *)(edi + 0x3f) > 1) goto ret0;
            if ((s32)*(s8 *)(edi + 0x3e) == 0) goto ret0;
            if ((s32)*(s8 *)(param_1 + 0x18) < 0x23) {
              if ((s32)*(s8 *)(edi + 0x3d) > 0) goto ret0;
            }
            /* 0x0069f942 */
            eax = *(u32 *)(param_1 + 0x1a);
            ecx = *(u32 *)(param_2 + 0x53);
            if (eax != ecx) {
              if ((s32)(s16)*(u16 *)(param_2 + 0x80) < 0x1c52) {
                if (*(u32 *)(param_1 + 0x39) != param_2) goto ret0;
              }
            }
          }
        }
      }
    }
  }

  /* 0x0069f960 */
  if ((s32)*(s8 *)(param_1 + 0x3d) == 5) {
    edx = *(u32 *)(param_2 + 0x57);
    if (edx != 0) {
      eax = *(u32 *)(param_1 + 0x39);
      if (eax != 0) {
        ecx = *(u32 *)(eax + 0x57);
        if (ecx != 0) {
          if ((s32)(s16)*(u16 *)(edx + 0x69) <
              (s32)(s16)*(u16 *)(ecx + 0x69)) goto ret0;
          if (edx == ecx) {
            ecx = (u32)((s32)(s16)*(u16 *)(param_2 + 0x80) - 0x4e2);
            edx = (u32)(s32)(s16)*(u16 *)(eax + 0x80);
            if ((s32)ecx < (s32)edx) goto ret0;
          }
          /* 0x0069f9b1 */
          if ((s32)(s16)*(u16 *)(eax + 0x80) >
              (s32)(s16)*(u16 *)(param_2 + 0x80)) goto ret0;
          /* 0x0069f9c5 */
          ecx = *(u32 *)(param_1 + 0x42);
          edx = *(u32 *)(param_1 + 0x3e);
          v = (u32)cm_fn_0054cdb0((short *)0xae2c90, edx, ecx);
          ecx = (u32)(((s32)*(s8 *)(param_1 + 0x59) + 0x15) * 10);
          if ((s32)v < (s32)ecx) {
            edx = *(u32 *)(param_1 + 0x39);
            if ((s32)(s16)*(u16 *)(edx + 0x80) > 0x1482) goto ret0;
          }
        }
      }
    }
  }

  /* 0x0069f9f9 */
  eax = *(u32 *)(param_1 + 0x24);
  if (eax != 0) {
    if ((s32)*(s8 *)(param_1 + 0x28) == 5) goto ret0;
  }

  /* 0x0069fa0a */
  eax = *(u32 *)(param_1 + 0x69);
  if (eax != 0) {
    if ((s32)(s16)*(u16 *)(eax + 0x0a) <= 0x128e) goto L_69fa28;
  }
  /* 0x0069fa19 */
  if ((s32)(s16)*(u16 *)(param_2 + 0x80) < 0x8ca) goto ret0;

L_69fa28:
  /* 0x0069fa28: cmp byte [esi+0x22],0x4b / jbe */
  if (*(u8 *)(param_1 + 0x22) > 0x4b) {
    if ((s32)(s16)*(u16 *)(param_2 + 0x80) < 0xcb2) goto ret0;
  }

  /* 0x0069fa3d */
  eax = *(u32 *)(param_1 + 0x61);
  if (eax == 0) {
    if (cm_fn_007ae350(CM_GLOBAL(u32, 0xb7434c), param_1) != 0) goto ret0;
  }

  /* 0x0069fa58 */
  if ((s32)(s16)*(u16 *)(param_2 + 0x80) < 0x9c4) {
    eax = *(u32 *)(param_1 + 0x1a);
    ecx = *(u32 *)(param_2 + 0x53);
    if (eax == ecx) {
      ecx = *(u32 *)(param_1 + 0x69);
      if ((s32)(s16)*(u16 *)(ecx + 0x08) < 0x5dc) {
        if (*(u32 *)(param_1 + 0x39) == 0) {
          if (*(u32 *)(param_1 + 0x24) == 0) {
            return 1;
          }
        }
      }
    }
  }

  /* 0x0069fa8e */
  if (cm_fn_0053b900((u32 *)param_2) != 0) {
    edx = *(u32 *)(param_1 + 0x1a);
    eax = *(u32 *)(param_2 + 0x53);
    if (edx == eax) {
      eax = *(u32 *)(param_1 + 0x69);
      if ((s32)(s16)*(u16 *)(eax + 0x0a) < 0x1a5e) return 1;
      eax = *(u32 *)(param_1 + 0x39);
      if (eax == 0) return 1;
      /* 0x0069fac7 */
      ecx = *(u32 *)(param_1 + 0x42);
      edx = *(u32 *)(param_1 + 0x3e);
      v = (u32)cm_fn_0054cdb0((short *)0xae2c90, edx, ecx);
      ecx = (u32)(((s32)*(s8 *)(param_1 + 0x59) + 0x15) * 10);
      if ((s32)v > (s32)ecx) {
        edx = *(u32 *)(param_1 + 0x39);
        if ((s32)(s16)*(u16 *)(edx + 0x80) < 0x1e46) {
          ecx = (u32)(s32)(s16)*(u16 *)(param_2 + 0x80);
          eax = (u32)(s32)(s16)*(u16 *)(edx + 0x80) + 0x2ee;
          if ((s32)eax < (s32)ecx) return 1;
        }
      }
    }
  }

  /* 0x0069fb14 */
  v = (u32)*(u16 *)(param_2 + 0x80);
  if ((s32)(s16)v > 0x1e46) {
    u32 own;

    own = *(u32 *)(param_2 + 0x53);
    if (own != 0) {
      s32 m = (s32)*(s8 *)(own + 0x85) * (s32)(s16)v;
      t = (long double)m * CM_GLOBAL(double, 0x967888);
    }
    else {
      t = (long double)(s32)(s16)v * CM_GLOBAL(double, 0x96a5d0);
    }
    ll = (long long)t;                 /* 0x0069fb5a: call 0x00945640 */
    edi = (u32)ll;
    if ((s32)(s16)(u16)edi > 0x203a) {
      s32 d = (s32)(s16)(u16)edi - 0x203a;
      t = (long double)d * CM_GLOBAL(double, 0x96d278);
      ll = (long long)t;               /* 0x0069fb7f: call 0x00945640 */
      edi = 0x203a - (u32)ll;
    }

    /* 0x0069fb8b */
    eax = *(u32 *)(param_1 + 0x39);
    if (eax != 0) {
      u32 other = *(u32 *)(eax + 0x53);
      u32 res;

      if (other != 0) {
        s32 m = (s32)*(s8 *)(other + 0x85) *
                (s32)(s16)*(u16 *)(eax + 0x80);
        t = (long double)m * CM_GLOBAL(double, 0x967888);
      }
      else {
        t = (long double)(s32)(s16)*(u16 *)(eax + 0x80) *
            CM_GLOBAL(double, 0x96a5d0);
      }
      ll = (long long)t;               /* 0x0069fbd3: call 0x00945640 */
      res = (u32)ll;
      if ((s32)(s16)(u16)res > 0x203a) {
        s32 d = (s32)(s16)(u16)res - 0x203a;
        t = (long double)d * CM_GLOBAL(double, 0x96d278);
        ll = (long long)t;             /* 0x0069fbf5: call 0x00945640 */
        res = 0x203a - (u32)ll;
      }
      /* 0x0069fc03 */
      ecx = (u32)(s32)(s8)*(s8 *)(param_1 + 0x59);
      ecx = (u32)(s32)(s16)(u16)(ecx * 25);
      ecx = ecx + (u32)(s32)(s16)(u16)res + 0xfa;
      if ((s32)(s16)(u16)edi > (s32)ecx) return 1;
    }
  }

  /* 0x0069fc2a */
  eax = CM_GLOBAL(u32, 0xde1f54);
  ecx = *(u32 *)(eax + 0x856);
  t = cm_fn_00844780(ecx, param_1, param_2, 0, 5, 0);
  if (!(t >= param_3)) goto ret0;
  return 1;

ret0:
  return 0;
}
