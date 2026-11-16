/* fn_007a1710 -- preferred-image VA 0x007a1710, image base 0x00400000, 49 bytes.

   S is the 0x20-byte scratch reserved on entry. The original saves EDI,
   stores only dwLength = 0x20 at S+0, zeroes ECX, points EDI at S (neither is
   read by the stdcall import) and calls KERNEL32!GlobalMemoryStatus(S)
   through the import slot 0x9670c4. It then reads dwAvailPhys (S+12) and
   dwAvailPageFile (S+20), adds them with 32-bit wrap, shifts the sum right
   logically by 20 and returns it in EAX (ret 0, cdecl).

   Physical byte offsets from the scratch base follow the instruction sites:
   the stdcall import removes the argument pushed at 0x007a1723,
   leaving esp = S-4 at both loads, as required by pop edi and the balanced
   epilogue. Thus [esp+0x10] is S+12 and [esp+0x18] is S+20.
   The buffer is exactly 32 bytes in
   one scratch allocation, only offset 0 is initialised here, and every other
   byte is left for the import to write, like the original.

   Import boundary (no default and no initialisation is invented here):
   0x20 is the only value the routine stores into its scratch, and it is a
   DWORD length covering the whole 32-byte block. A conforming
   KERNEL32!GlobalMemoryStatus fills every field of that block when dwLength
   equals the structure size, so the loads at S+12 and S+20 always observe
   import-written bytes: stack bytes left untouched, and partial fills, are
   impossible states for this call site (there is a single store site and no
   path rewrites the length).  The C form therefore reproduces which bytes are
   read and in which order, and its frame address cannot be observed.  ECX = 0
   and EDI = S are set before the call but the import is a stdcall routine
   whose only input is the pointer at [esp+4], so neither register is consumed
   at that boundary. */

#include "cmre.h"

typedef void (CM_STDCALL *memory_status_fn)(volatile u32 *);

u32 CM_CDECL cm_fn_007a1710(void)
{
    volatile u32 status[8];  /* S: 32 bytes at S+0..S+28, one allocation */
    u32 first;
    u32 second;

    status[0] = 0x20u;       /* dwLength; no other local byte is written */
    ((memory_status_fn)CM_GLOBAL(volatile u32, 0x009670c4))(status);
    first = status[3];       /* 0x007a172e: [esp+0x10] = S+12 */
    second = status[5];      /* 0x007a1732: [esp+0x18] = S+20 */
    return (second + first) >> 20;   /* lea eax,[edx+ecx]; shr eax,0x14 */
}
