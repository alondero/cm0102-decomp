/* fn_00403c80, preferred-image VA 0x00403c80, image base 0x00400000,
   module area.cpp. Plain C reconstruction from the original instructions.

   Calling convention: entry reads ECX before writing it (mov esi, ecx at
   0x00403c87) and the routine ends with `ret 0` (0x00403cfd/0x00403edf), so the
   receiver arrives in ECX and nothing is popped from the stack: fastcall with
   the object in ECX. EDX is a live input too (see the colour merges below).

   Colour arguments: the original merges a 16-bit colour global into only the
   low half of a register that already holds a value:
     mov dx, word ptr [0xae317e]   (0x00403d56, 0x00403dd4)
     mov cx, word ptr [0x00aebdea] (0x00403d90)
     mov cx, word ptr [0xae317e]   (0x00403e01, 0x00403e54)
     mov ax, word ptr [0x00aebdea] (0x00403e81)
   and then pushes the FULL 32-bit register, so the pushed colour DWORD carries
   the untouched upper 16 bits of the register at that point.  This source
   models that explicitly with `hi16`: the value of those upper 16 bits is a
   declared model input, not a zero extension.
     - At 0x00403d56 the register is EDX, whose upper half is the incoming EDX
       of this fastcall (its second argument) when the 0x00403dd0 call did not
       run, so the model starts from `param_2 & 0xffff0000`.
     - At 0x00403e81 the register is EAX right after the 0x005e5df0 call at
       0x00403e7c, so EAX's upper half is that call's 32-bit result, which is
       recovered exactly below.
     - The remaining merges read ECX/EDX immediately after a call, where the
       original instruction stream supplies no value that plain C can name.
       They keep the same modelled upper half (`hi16`) rather than extending
       the 16-bit colour with zeros; nothing here claims those bits are zero. */

#include "cmre.h"

#define DAT_009cd6bc CM_GLOBAL(u8, 0x009cd6bc)
#define DAT_009cd6bd CM_GLOBAL(u8, 0x009cd6bd)
#define DAT_009cd6dc CM_GLOBAL(u8, 0x009cd6dc)
#define DAT_00ae317e CM_GLOBAL(u16, 0x00ae317e)
#define DAT_00aebdea CM_GLOBAL(u16, 0x00aebdea)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e3ce0(int param_1, int param_2, u32 param_3, u8 *param_4);
extern int * cm_fn_005e3dd0(u32 param_1, u32 param_2, u32 param_3, u32 param_4, int *param_5);
extern u32 CM_CDECL cm_fn_005e5df0(u32 left, u32 top, u32 right, u32 bottom, u32 flags,
                                   u32 color, u32 other_color);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* legacy boundary, no arguments of its own.  The single add esp 0x24 at
   0x00403ce9 covers nine pushes; of the three pushes at 0x00403cbc (0x366),
   0x00403cc1 (lea edx,[esp+4] = L+0) and 0x00403cc2 (lea eax,[esp+0x104] =
   L+0x100), only the last is written immediately before this call, so all
   three are arguments MSVC evaluated early for 0x00944c9f's variadic tail
   (right to left: the last argument is pushed first). */
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define CM_FIELD(o) (*(volatile u32 *)((u32)param_1 + (u32)(o)))
#define CM_FIELDB(o) (*(volatile u8 *)((u32)param_1 + (u32)(o)))
#define CM_COLOR(hi, w) (((hi) & 0xffff0000u) | (u32)(u16)(w))

void CM_FASTCALL cm_fn_00403c80(int param_1, u32 param_2)
{
  u8 buf[0x200];              /* sub esp, 0x200: [esp+4] = L+0x000, [esp+0x104] = L+0x100 */
  u32 hi16;                   /* modelled upper 16 bits of the pushed colour DWORD */
  u32 ret;
  u32 flags;
  u8 mode;
  int edi;
  int ebx;
  int x;
  int y;

  if (CM_FIELD(0xbed) == 0) {
    cm_fn_00944cff(0x009873c0u, 0, 0, (void *)(buf + 0x100), (void *)buf);
    ret = (u32)cm_fn_0090d130();
    /* 0x009870e8 is "v%s %s %s %d": its four variadic operands are the pushed
       dwords at 0x00403cc2 (buf+0x100), 0x00403cc1 (buf) and 0x00403cbc (0x366),
       with the 0x0090d130 result as the first. */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret, (u32)(buf + 0x100), (u32)buf, 0x366u);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    DAT_00b67a34 = 0;
    return;
  }

  if (CM_FIELD(0xba6) == 0) {
    ret = (u32)cm_fn_005e3dd0(CM_FIELD(0xb7a), CM_FIELD(0xb7e), CM_FIELD(0xb82),
                             CM_FIELD(0xb86), (int *)0);
    CM_FIELD(0xba6) = ret;
  }

  hi16 = param_2 & 0xffff0000u;

  mode = CM_FIELDB(0xbaa);
  edi = 0;
  ebx = 0;
  flags = 0x30;
  if (mode == 1) {
    edi = 2;
    flags = 0x70;
    ebx = edi;
  }

  /* 0x00403d7e */
  cm_fn_005e5df0(CM_FIELD(0xb7a), CM_FIELD(0xb7e), CM_FIELD(0xb82),
                 CM_FIELD(0xb8a) - 1, flags, CM_COLOR(hi16, DAT_00ae317e), 0);

  /* 0x00403dcf: y at 0x00403dcc, x at 0x00403dac, colour from ECX (0x00403d90) */
  y = (int)(0x14 - (int)DAT_009cd6bc) / 2 + (int)CM_FIELD(0xb7a) + ebx;
  x = (int)(0x14 - (int)DAT_009cd6bd) / 2 + (int)CM_FIELD(0xb7e) + edi;
  cm_fn_005e3ce0(y, x, CM_COLOR(hi16, DAT_00aebdea), &DAT_009cd6bc);

  /* 0x00403dfc */
  cm_fn_005e5df0(CM_FIELD(0xb7a), CM_FIELD(0xb8a), CM_FIELD(0xb82),
                 CM_FIELD(0xb8e), 2, CM_COLOR(hi16, DAT_00aebdea), 0);

  /* 0x00403e2c */
  cm_fn_005e5df0(CM_FIELD(0xb7a), CM_FIELD(0xb9e), CM_FIELD(0xb82),
                 CM_FIELD(0xba2), 0x30, CM_COLOR(hi16, DAT_00ae317e), 0);

  mode = CM_FIELDB(0xbaa);
  edi = 0;
  ebx = 0;
  flags = 0x30;
  if (mode == 2) {
    edi = 2;
    flags = 0x70;
    ebx = edi;
  }

  /* 0x00403e7c */
  ret = cm_fn_005e5df0(CM_FIELD(0xb7a), CM_FIELD(0xb8e) + 1, CM_FIELD(0xb82),
                       CM_FIELD(0xb86), flags, CM_COLOR(hi16, DAT_00ae317e), 0);

  /* 0x00403ece: colour low half from 0x00403e81 (`mov ax, word ptr [0xaebdea]`),
     whose upper half is the 0x005e5df0 result still in EAX. */
  y = (int)(0x14 - (int)DAT_009cd6bc) / 2 + (int)CM_FIELD(0xb7a) + ebx;
  x = (int)(0x14 - (int)DAT_009cd6bd) / 2 + (int)CM_FIELD(0xb8e) + 1 + edi;
  cm_fn_005e3ce0(y, x, CM_COLOR(ret, DAT_00aebdea), &DAT_009cd6dc);
}
