/* fn_0085add0: stdcall (ret 8) with two stack arguments: param_1 at entry [esp+4]
   (read as the object pointer; 0 takes the message branch) and param_2 at entry
   [esp+8] (optional pointer to a u32 index; 0 skips its check).
   Preferred-image VA 0x0085add0, image base 0x00400000.
   Scratch frame: `sub esp, 0x200`; bytes [0x000,0x100) are the buffer passed as
   the fifth argument of 0x00944cff, bytes [0x100,0x200) the fourth. */
#include "cmre.h"

extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
/* Takes no arguments. The pushes made before its call are trailing varargs of
   the following 0x00944c9f call (see the caller's listing). */
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_STDCALL cm_fn_0085add0(u32 param_1, u32 *param_2)
{
  u8 scratch[0x200];
  u8 *self;
  u32 *ptr;
  u32 table;
  u32 index;
  u32 obj;
  u32 result;

  if (param_1 == 0u) {
    cm_fn_00944cff(0x00a82b88u, 0u, 0u, scratch + 0x100, scratch);
    result = (u32)cm_fn_0090d130();
    /* 0x00944c9f receives 0x4b0, the frame base, the 0x100 offset and the
       result of 0x0090d130 as its variadic tail, pushed in that order. */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, result, (u32)(scratch + 0x100),
                   (u32)scratch, 0x4b0u);
    cm_fn_005e8290((const char *)&CM_GLOBAL(u8, 0x009870e0),
                   (const char *)&CM_GLOBAL(u8, 0x00ae24d0), 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0u;
    return 0u;
  }

  self = (u8 *)param_1;
  ptr = *(u32 **)(self + 0x57u);
  if (ptr == 0) {
    return 0u;
  }

  table = CM_GLOBAL(u32, 0x00adadfc);
  index = *ptr;
  obj = *(u32 *)(table + index * 4u);
  if (obj == 0u) {
    return 0u;
  }
  if ((*(u8 *)(obj + 0x52u) & 2u) == 0u) {
    return 0u;
  }

  if (param_2 != 0) {
    index = *param_2;
    obj = *(u32 *)(table + index * 4u);
    if ((*(u8 *)(obj + 0x52u) & 2u) == 0u) {
      return 0u;
    }
  }
  return 1u;
}
