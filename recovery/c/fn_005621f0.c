/* Preferred-image VA 0x005621F0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: stdcall, two stack arguments (arg1 at [esp+4], arg2 at [esp+8]), ret 8,
   no return value. Reads the record pointer from the dword global at 0x00AD9C58;
   if it is null the routine returns with no store. Otherwise it stores 1 to the
   dword at 0x00B67A34 and arg2 to 0x00B67A3C, then dispatches on arg1 (unsigned,
   > 3 takes the default). Jump table at 0x00562290: 0 -> 0x562238, 1 -> 0x562223,
   2 -> 0x562250, 3 -> 0x562268.
     0: 0x00B67A20 = rec[0] + 1; 0x00B67A14 = rec[5] + 2  (rec[i] is dword at +4*i)
     1: 0x00B67A20 = rec[0] + 1; 0x00B67A14 = rec[1] + 1
     2: 0x00B67A20 = rec[0] + 1; 0x00B67A14 = rec[10] + 2
     3: 0x00B67A20 = rec[0] + 1; 0x00B67A14 = rec[4] + 1
     default: 0x00B67A34 = 0 and 0x00B67A3C = 0.
   Globals and record reads are volatile so the listing's order is kept.
   Field meanings remain unknown. */
#include "cmre.h"

void CM_STDCALL cm_fn_005621f0(u32 arg1, u32 arg2)
{
    volatile u32 *rec;

    rec = (volatile u32 *)CM_GLOBAL(volatile u32, 0x00ad9c58);
    if (rec == 0) {
        return;
    }
    CM_GLOBAL(volatile u32, 0x00b67a34) = 1;
    CM_GLOBAL(volatile u32, 0x00b67a3c) = arg2;
    switch (arg1) {
    case 0:
        CM_GLOBAL(volatile u32, 0x00b67a20) = rec[0] + 1;
        CM_GLOBAL(volatile u32, 0x00b67a14) = rec[5] + 2;
        return;
    case 1:
        CM_GLOBAL(volatile u32, 0x00b67a20) = rec[0] + 1;
        CM_GLOBAL(volatile u32, 0x00b67a14) = rec[1] + 1;
        return;
    case 2:
        CM_GLOBAL(volatile u32, 0x00b67a20) = rec[0] + 1;
        CM_GLOBAL(volatile u32, 0x00b67a14) = rec[10] + 2;
        return;
    case 3:
        CM_GLOBAL(volatile u32, 0x00b67a20) = rec[0] + 1;
        CM_GLOBAL(volatile u32, 0x00b67a14) = rec[4] + 1;
        return;
    default:
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        CM_GLOBAL(volatile u32, 0x00b67a3c) = 0;
        return;
    }
}
