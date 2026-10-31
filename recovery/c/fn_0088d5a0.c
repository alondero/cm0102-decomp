/* fn_0088d5a0: thiscall (ECX = object, read before written) with no stack arguments
   (ret 0). Preferred-image VA 0x0088d5a0, image base 0x00400000.
   Object fields are named by offset: +0x3e (s16 count), +0x a7 (u32 pointer
   assigned from 0x00944e46 and used as a byte buffer), +0xb2 (u32 pointer to a
   u32 table of 6-byte records). Scratch frame: `sub esp, 0x200`; bytes
   [0x000,0x100) and [0x100,0x200) are the buffers passed to 0x00944cff. */
#include "cmre.h"

extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
/* Takes no arguments. The pushes made before its call are trailing varargs of
   the following 0x00944c9f call (see the listing). */
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_0088d5a0(u8 *self)
{
  u8 scratch[0x200];
  u32 eax;
  u32 result;
  u32 idx;
  u32 ecx_off;
  u32 tmp;
  u32 buf;
  u32 buf_b;
  u32 buf_c;
  u32 table;

  *(volatile u16 *)(self + 0x3eu) = 0x1eu;
  eax = (u32)cm_fn_00944e46(0xb4u);
  *(volatile u32 *)(self + 0xa7u) = eax;
  if (eax == 0u) {
    cm_fn_00944cff(0x00a8b4a0u, 0u, 0u, scratch + 0x100, scratch);
    result = (u32)cm_fn_0090d130();
    /* 0x00944c9f receives 0x1a4, the frame base, the 0x100 offset and the
       result of 0x0090d130 as its variadic tail, in that order. */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, result, (u32)(scratch + 0x100),
                   (u32)scratch, 0x1a4u);
    cm_fn_005e8290((const char *)&CM_GLOBAL(u8, 0x009870e0),
                   (const char *)&CM_GLOBAL(u8, 0x00ae24d0), 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0u;
    return 0xffffffffu;
  }

  idx = 0u;
  if ((s32)(s16)*(volatile u16 *)(self + 0x3eu) > 0) {
    ecx_off = 0u;
    do {
      table = *(volatile u32 *)(self + 0xb2u);
      buf = *(volatile u32 *)(self + 0xa7u);
      ecx_off += 6u;
      tmp = *(volatile u32 *)(table + idx * 4u);
      idx += 1u;
      *(u32 *)(buf + ecx_off - 6u) = tmp;
      buf_b = *(volatile u32 *)(self + 0xa7u);
      *(u8 *)(buf_b + ecx_off - 2u) = 0u;
      buf_c = *(volatile u32 *)(self + 0xa7u);
      *(u8 *)(buf_c + ecx_off - 1u) = 0u;
      table = *(volatile u32 *)(self + 0xb2u);
      *(u32 *)(table + idx * 4u - 4u) = 0u;
    } while ((s32)idx < (s32)(s16)*(volatile u16 *)(self + 0x3eu));
  }
  return 1u;
}
