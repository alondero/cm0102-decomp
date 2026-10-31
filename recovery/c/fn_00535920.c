/* Preferred-image VA 0x00535920, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: mov eax, dword ptr [0x9b5984]; ret. Zero-argument routine that returns
   the 32-bit value stored at global 0x009b5984 in EAX, with no stack bytes popped.
   No EDX read by callers. The global's meaning remains unknown. */
#include "cmre.h"

u32 cm_fn_00535920(void)
{
  return CM_GLOBAL(u32, 0x009b5984);
}
