/* Preferred-image VA 0x005d5a30, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Two instructions: MOV AL, byte ptr [ECX + 0x3d] then RET. ECX is the only
   register input (thiscall self pointer), no stack bytes are popped, and only
   AL (one byte) is the return value. The field at +0x3d is unknown in meaning. */
#include "cmre.h"

u8 CM_THISCALL cm_fn_005d5a30(void *self)
{
  return *(u8 *)((u8 *)self + 0x3d);
}
