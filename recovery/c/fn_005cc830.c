/* Recovered C for preferred-image VA 0x005cc830 (cm0102.exe 3.9.68).
   Listing:
     005cc830  push 0x5a53c0
     005cc835  push 0xaf0
     005cc83a  push 0x78
     005cc83c  push ecx
     005cc83d  call 0x9462bb
     005cc842  ret
   ECX is read before it is written, so the incoming value arrives in the
   first register slot (CM_FASTCALL).  The routine ends in a plain `ret`,
   so it cleans no stack bytes of its own. */
#include "cmre.h"

/* The caller's own listing pushes four words and then returns without any
   `add esp, N`, so the callee removes those sixteen bytes itself: the
   original callee is a __stdcall-style routine. */
extern void CM_STDCALL cm_fn_009462bb(u32 param_1, u32 param_2, int param_3, void *param_4);

void CM_FASTCALL cm_fn_005cc830(u32 param_1)
{
  /* Arguments are pushed right to left, so the call is
     cm_fn_009462bb(ecx, 0x78, 0xaf0, 0x005a53c0).
     0x005a53c0 is pushed as an immediate address; the 1-byte global living
     there is not read by this routine. */
  cm_fn_009462bb(param_1, 0x78u, 0xaf0, (void *)0x005a53c0u);
}
