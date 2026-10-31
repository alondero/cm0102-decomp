/* Tier-2 C recovery for preferred-image VA 0x00949d5c (cdecl, ret 0).
   Arguments are named by their [ebp+N] stack offset. Each is one dword.
   Callee 0x00949df7 takes its sixth argument as a full dword ([ebp+0x24]
   pushed unchanged), so the boundary declares it u32. */
#include "cmre.h"

extern void cm_fn_0094a5e2(void);
extern void cm_fn_0094a0a3(u32, u32, u32, u32);
extern void cm_fn_00949df7(u32, u32, u32, u32, u32, u32, u32, u32);

u32
cm_fn_00949d5c(u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14,
               u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24)
{
  u32 obj;
  u32 fn;

  if (*(volatile u32 *)arg_18 != 0x19930520u) {
    cm_fn_0094a5e2();
  }
  if ((*(volatile u8 *)(arg_08 + 4) & 0x66) == 0) {
    /* 0x00949d9a */
    if (*(volatile u32 *)(arg_18 + 0xc) == 0) {
      return 1;
    }
    if (*(volatile u32 *)arg_08 == 0xe06d7363u &&
        *(volatile u32 *)(arg_08 + 0x14) > 0x19930520u) {
      obj = *(volatile u32 *)(arg_08 + 0x1c);
      fn = *(volatile u32 *)(obj + 8);
      if (fn != 0) {
        /* 0x00949dad..0x00949dcd: indirect call, cdecl, 8 dwords popped
           by the caller. Last argument is the zero-extended byte. */
        return ((u32 (*)(u32, u32, u32, u32, u32, u32, u32, u32))fn)(
            arg_08, arg_0c, arg_10, arg_14, arg_18, arg_1c, arg_20,
            arg_24 & 0xff);
      }
    }
    /* 0x00949dd4 */
    cm_fn_00949df7(arg_08, arg_0c, arg_10, arg_14, arg_18, arg_24,
                   arg_1c, arg_20);
    return 1;
  }
  /* 0x00949d7b */
  if (*(volatile u32 *)(arg_18 + 4) == 0) {
    return 1;
  }
  if (arg_1c != 0) {
    return 1;
  }
  cm_fn_0094a0a3(arg_0c, arg_14, arg_18, 0xffffffffu);
  return 1;
}
