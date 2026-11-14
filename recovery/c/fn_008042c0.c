/* fn_008042c0 -- preferred-image VA 0x008042c0, image base 0x00400000
   (CM0102 3.9.68, cm0102.exe).

   Original listing (VA, instruction):
     008042c0  push 0x7ae4f0
     008042c5  push 0xa
     008042c7  inc ecx
     008042c8  push 0x2b0
     008042cd  push ecx
     008042ce  call 0x9462bb
     008042d3  ret

   Evidence used here:
     - ECX is read before it is written, so ECX carries an input value; it is
       incremented as a 32-bit register (`inc ecx`), i.e. the forwarded count
       is (ecx + 1) mod 2^32 (unsigned wrap, not signed overflow).
     - The four arguments are pushed right to left, so the callee receives
       (ecx+1, 0x2b0, 0xa, 0x007ae4f0) in that order.
     - `ret` follows the call directly with no `add esp`, so the callee pops
       its own 16 bytes of arguments: the callee is __stdcall.
     - 0x007ae4f0 is a data global of unknown meaning (the brief lists it as
       a 1-byte global).  The original pushes its *address*; it neither reads
       it nor calls it, so no memory access happens at that address here.
     - The callee is void and the original returns nothing, so the entry
       function is void. */

#include "cmre.h"

/* Data global at 0x007ae4f0 (unknown meaning, 1 byte).  Only its address is
   used by the original, so the lvalue is never loaded. */
#define CM_GLOBAL_007AE4F0 ((void *)&CM_GLOBAL(u8, 0x007ae4f0))

extern void CM_STDCALL cm_fn_009462bb(u32 param_1, u32 param_2, int param_3,
                                      void *param_4);

void CM_FASTCALL cm_fn_008042c0(u32 param_1)
{
  u32 count = param_1 + 1u; /* inc ecx: 32-bit register increment */

  cm_fn_009462bb(count, 0x2b0u, 10, CM_GLOBAL_007AE4F0);
}
