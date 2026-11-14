/* fn_0055f7c0 - preferred-image VA 0x0055f7c0 (image base 0x00400000)
   Translated from the original listing; thiscall, one stack argument (ret 4). */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae28fc CM_GLOBAL(u32, 0x00ae28fc)
#define DAT_00ae2908 CM_GLOBAL(u8 *, 0x00ae2908)
#define DAT_00ae290c CM_GLOBAL(s32, 0x00ae290c)
#define DAT_00ae2910 CM_GLOBAL(s32, 0x00ae2910)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

#define s_009870e0 ((const char *)0x009870e0)
#define s_009870e8 ((const char *)0x009870e8)
#define s_009c3734 ((const char *)0x009c3734)

#define OBJ_U8(o)  (*(volatile u8 *)(p + (o)))
#define OBJ_U16(o) (*(volatile u16 *)(p + (o)))
#define OBJ_U32(o) (*(volatile u32 *)(p + (o)))

extern u32 CM_THISCALL cm_fn_004031a0(u32 self);
extern void CM_CDECL cm_fn_005e42b0(u32 object);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_005eef00(u32 self);
extern char *CM_CDECL cm_fn_0090d130(void *p1, void *p2, int p3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_0055f7c0(void *self, int param_2)
{
  u8 *p = (u8 *)self;
  u8 buf[0x200];
  u32 v;
  u32 mode;
  s32 i;
  s16 count;

  v = OBJ_U32(0x12f555);
  OBJ_U16(0x12f4ff) = 0xffff;
  OBJ_U8(0x12f4fe) = 0;

  if (v == 0 && param_2 != 0) {
    count = (s16)OBJ_U16(0x12e99e);
    for (i = 0; i < count; i++) {
      u32 self2 = (u32)p + 4u + (u32)((s32)i * 3057);
      cm_fn_004031a0(self2);
    }
    count = (s16)OBJ_U16(0x12e9a0);
    for (i = 0; i < count; i++) {
      u32 self2 = (u32)p + 0xba95eu + (u32)((s32)i * 396);
      cm_fn_005eef00(self2);
    }
  }

  mode = (u32)DAT_00ae2910;
  DAT_00ae28fc = 0;
  DAT_00ae290c = 0;

  if (mode >= 1u) {
    u8 *ptr = DAT_00ae2908;
    s32 counter;
    *ptr = 2;
    counter = DAT_00ae290c;
    counter = counter + 1;
    DAT_00ae290c = counter;
  }
  else {
    u32 r;
    u32 a = (u32)buf;
    u32 b = (u32)(buf + 0x100);
    cm_fn_00944cff((u32)s_009c3734, 0u, 0u, (void *)b, (void *)a);
    r = (u32)cm_fn_0090d130((void *)b, (void *)a, 0x85);
    /* The original leaves every pushed word on the stack until the single
       "add esp, 0x24" after the last call, so the variadic callee reads the
       still-pushed words of the earlier calls (b, a, 0x85, in stack order)
       as its trailing arguments. */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)s_009870e8, r, b, a, 0x85u);
    cm_fn_005e8290(s_009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
  }

  v = OBJ_U32(0x12f521);
  *(volatile u32 *)p = 0;
  OBJ_U16(0x12f4f6) = 0;
  OBJ_U16(0x12f4f8) = 0;
  OBJ_U16(0x12e9a0) = 0;
  OBJ_U16(0x12e99e) = 0;
  OBJ_U16(0x12f52d) = 0xffff;
  OBJ_U16(0x12f52f) = 0xffff;
  OBJ_U16(0x12f531) = 0xffff;
  OBJ_U32(0x12f545) = 0;
  OBJ_U32(0x12f509) = 0;
  OBJ_U32(0x12f50d) = 0;
  OBJ_U32(0x12f511) = 0;
  OBJ_U32(0x12f515) = 0;
  OBJ_U32(0x12f519) = 0;
  OBJ_U32(0x12f51d) = 0;
  OBJ_U32(0x12f549) = 0;
  OBJ_U32(0x12f54d) = 0;
  OBJ_U16(0x12f533) = 0xffff;
  OBJ_U32(0x12f551) = 0;
  OBJ_U32(0x12f555) = 0;
  OBJ_U32(0x12f4fa) = 0;

  if (v != 0) {
    cm_fn_005e42b0(v);
    OBJ_U32(0x12f521) = 0;
  }

  OBJ_U32(0x12f545) = 0;
}
