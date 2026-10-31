/* WIP: memory mismatch, case 11 (record page 0x3001000): the original reads two
   uninitialized caller-frame bytes at entry-8 (word, instruction 0x00521ee7) and
   entry-6 (byte, instruction 0x00521eef) that neither this routine nor 0x00549f70
   writes; they feed record fields +0x03 and +0x05. Cases 9 and 15 show the known
   0xae2000 error-formatter harness limit (0x00549f70 error path). */
#include "cmre.h"

/* Stack arguments (cdecl, nine dwords; the caller pops them):
   arg1 base pointer of a record table; arg2 signed 16-bit record index;
   arg3, arg4 pass-through values for 0x00549f70 (short and char in the callee);
   arg5 and arg8 are added (arg8 + arg5) and the sum is passed as a u16;
   arg6 passed as the callee's last argument; arg7 low byte stored in the record;
   arg8 low byte subtracted from a scratch byte; arg9 stored as a dword.
   Record stride is 0x68 bytes; written fields: +0x03 u16, +0x05 u8, +0x06 u8, +0x13 u32.

   The original passes the address of its own stack slots as ECX to 0x00549f70,
   and reads back two scratch bytes that the callee writes (scratch+0x0c u16,
   scratch+0x0e u8). Scratch offsets 0 and 4 hold the values the original pushes
   there; offset 8 holds the caller's saved EBX, which plain C cannot express. */
extern u8 *CM_THISCALL cm_fn_00549f70(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

/* Returns the record pointer in EAX and, in EDX, the leftover value the original
   leaves there (3 * index in the high word, then the copied word and byte). */
unsigned long long CM_CDECL cm_fn_00521eb0(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5,
                                           u32 arg6, u32 arg7, u32 arg8, u32 arg9)
{
    u8 scratch[16];
    u32 sum;
    u32 index;
    u32 elem;
    u32 edx;
    u16 word12;
    u8 byte14;
    u8 low;

    sum = arg8 + arg5;
    *(volatile u32 *)(scratch + 0) = sum;
    *(volatile u32 *)(scratch + 4) = arg6;
    cm_fn_00549f70(scratch, arg3, arg4, sum, arg6);

    index = (u32)(s32)(s16)arg2;
    word12 = *(volatile u16 *)(scratch + 12);
    byte14 = *(volatile u8 *)(scratch + 14);
    elem = arg1 + index * 104u;
    low = (u8)(byte14 - (u8)arg8);

    *(volatile u16 *)(elem + 0x03) = word12;
    *(volatile u8 *)(elem + 0x05) = low;
    *(volatile u8 *)(elem + 0x06) = (u8)arg7;
    *(volatile u32 *)(elem + 0x13) = arg9;

    edx = (index * 3u) & 0xffff0000u;
    edx = edx | (u32)word12;
    edx = (edx & 0xffffff00u) | (u32)(u8)arg7;
    return ((unsigned long long)edx << 32) | (unsigned long long)elem;
}
