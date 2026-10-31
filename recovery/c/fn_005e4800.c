/* Preferred-image VA 0x005e4800, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl-style routine with four stack arguments (RET 0, nothing popped).
   If global 0x00aebe0c is nonzero, returns EAX = that value with the low 16 bits
   cleared (XOR AX,AX).
   Otherwise the fourth argument selects a record: a NULL pointer is replaced by the
   address of global 0x00ae3158. The 32-bit word at record offset 0x14 is compared
   with 0x7e0. Only the low bytes of the first three arguments are used:
     equal:     ((((a1 & 0xf8) << 5) | (a2 & 0xfc)) << 3) | ((a3 & 0xff) >> 3)
     otherwise: ((((a1 & 0xf8) << 5) | (a2 & 0xf8)) << 2) | ((a3 & 0xff) >> 3)
   The meanings of the globals, the record fields and the arguments remain unknown.
   Only EAX is the result; no EDX read by callers. */
#include "cmre.h"

u32 cm_fn_005e4800(u32 arg1, u32 arg2, u32 arg3, u32 *arg4)
{
  u32 *record;
  u32 packed;

  if (CM_GLOBAL(u32, 0x00aebe0c) != 0) {
    return CM_GLOBAL(u32, 0x00aebe0c) & 0xffff0000u;
  }
  record = arg4;
  if (record == (u32 *)0) {
    record = &CM_GLOBAL(u32, 0x00ae3158);
  }
  if (record[5] == 0x7e0u) {
    packed = (((arg1 & 0xf8u) << 5) | (arg2 & 0xfcu)) << 3;
    return packed | ((arg3 & 0xffu) >> 3);
  }
  packed = (((arg1 & 0xf8u) << 5) | (arg2 & 0xf8u)) << 2;
  return packed | ((arg3 & 0xffu) >> 3);
}
