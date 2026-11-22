#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define LAB_0058f7c0 CM_GLOBAL(u8, 0x0058f7c0)
#define LAB_0058ff80 CM_GLOBAL(u8, 0x0058ff80)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flag);
extern char * cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

void cm_fn_0058f500(u32 param_1, u32 param_2, u32 param_3, u32 param_4,
                    u32 param_5, u32 param_6, u32 param_7, short param_8,
                    short param_9, u32 param_10, u32 param_11, u32 param_12,
                    u32 param_13, u32 param_14)
{
  u8 scratch[0x200];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  u32 alloc1, alloc2, alloc3;
  u32 msg;

  if ((param_1 == 0) || (param_2 == 0) || (param_4 == 0)) {
    cm_fn_00944cff((u32)0x009c7a48, 0, 0, local_100, local_200);
    msg = (u32)cm_fn_0090d130((u32)local_100, (u32)local_200, 0x83);
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8, msg,
                   (u32)local_100, (u32)local_200, 0x83);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    return;
  }

  if (cm_fn_007fe600(0x00b74450, (u32)&LAB_0058f7c0, (u32)&LAB_0058ff80, 1, 0, 0) == 0)
    return;

  alloc1 = (u32)cm_fn_00944cf1(0x104);
  cm_fn_009466d0(alloc1, param_4, 0x104);
  *(u8 *)(alloc1 + 0x103) = 0;

  alloc2 = (u32)cm_fn_00944cf1(0x100);
  if ((param_3 == 1) && (param_6 != 0)) {
    cm_fn_009466d0(alloc2, param_6, 0x100);
    *(u8 *)(alloc2 + 0xff) = 0;
  } else {
    *(u8 *)alloc2 = 0;
  }

  alloc3 = (u32)cm_fn_00944cf1(0x104);
  *(u8 *)alloc3 = 0;

  cm_fn_007ff270(0x00b74450, 0, param_1, 1);
  cm_fn_007ff270(0x00b74450, 1, param_2, 1);
  cm_fn_007ff270(0x00b74450, 2, param_3, 0);
  cm_fn_007ff270(0x00b74450, 3, param_4, 0);
  cm_fn_007ff270(0x00b74450, 4, param_5, (u32)(param_5 != 0));
  cm_fn_007ff270(0x00b74450, 5, param_7, 0);
  cm_fn_007ff270(0x00b74450, 6, alloc1, 0);
  cm_fn_007ff270(0x00b74450, 7, alloc2, 0);
  cm_fn_007ff270(0x00b74450, 8, alloc3, 0);
  cm_fn_007ff270(0x00b74450, 9, 0, 0);
  cm_fn_007ff270(0x00b74450, 0xa, (u32)(s32)param_8, 0);
  cm_fn_007ff270(0x00b74450, 0xb, (u32)(s32)param_9, 0);
  cm_fn_007ff270(0x00b74450, 0xc, param_10, 0);
  cm_fn_007ff270(0x00b74450, 0xd, param_11, 0);
  cm_fn_007ff270(0x00b74450, 0xe, 0, 0);
  cm_fn_007ff270(0x00b74450, 0xf, 0, 0);
  cm_fn_007ff270(0x00b74450, 0x10, param_12, 0);
  cm_fn_007ff270(0x00b74450, 0x11, param_13, 0);
  cm_fn_007ff270(0x00b74450, 0x12, param_14, 0);
}
