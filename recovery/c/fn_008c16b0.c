#include "cmre.h"

/* Thiscall with three stack arguments (ret 0x0c). ECX is the object pointer
   (self, copied to esi). arg1 is a pointer whose dword is read on the second
   path; arg2 contributes only its low byte (dl); arg3 is a dword.

   Frame layout from the entry esp E (sub esp, 0x20c; two pushes):
   - local_200 is E-0x200 .. E-0x101 and local_100 is E-0x100 .. E-1. They are
     adjacent, so they share one scratch array (local_200 first, then local_100).
   - The 9-byte record passed to 0x0068d0d0 is E-0x20c (dword), E-0x208 (byte),
     E-0x207 (dword), in that order; it is built in its own 9-byte array.

   Returns EAX as left by the last original call on each path: 0x005e8290 on
   the first path and the low half of 0x0068d0d0's result on the second. Neither
   path changes EAX afterwards (pops, add esp and the global store do not). */

extern u32 CM_THISCALL cm_fn_004cf6d0(u32 self, u32 arg1);
extern u32 CM_CDECL cm_fn_005e8290(u32 arg1, u32 arg2, u32 arg3);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self,
                                                     const u8 *src, s16 count);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);

#define PUT_U32_LE(p, off, v)                 \
    do {                                      \
        (p)[(off) + 0] = (u8)(v);             \
        (p)[(off) + 1] = (u8)((v) >> 8);      \
        (p)[(off) + 2] = (u8)((v) >> 16);     \
        (p)[(off) + 3] = (u8)((v) >> 24);     \
    } while (0)

u32 CM_THISCALL cm_fn_008c16b0(u32 self, u32 arg1, u32 arg2, u32 arg3)
{
    u8 scratch[512]; /* [0..255] = local_200, [256..511] = local_100 */
    u8 rec[9];       /* dword, byte, dword: the record for 0x0068d0d0 */
    u32 flag;
    u32 uVar2;
    u32 result;
    u32 v;

    /* Receiver is the dword stored at 0x00ae1050 (mov ecx, [0xae1050]). */
    flag = cm_fn_004cf6d0(CM_GLOBAL(volatile u32, 0x00ae1050u), arg1);
    if (flag != 0) {
        cm_fn_00944cff(0x00a8d514u, 0u, 0u, scratch + 256, scratch);
        /* 0x0090d130 is `mov eax, 0xad4394; ret` and reads no stack. The pushes
           0xbe6, scratch and scratch + 256 before it are the trailing varargs of
           the following 0x00944c9f call (one add esp, 0x24 cleans both calls). */
        uVar2 = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, uVar2, scratch + 256, scratch,
                       0xbe6u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return result;
    }

    v = *(volatile u32 *)arg1;
    PUT_U32_LE(rec, 0, v);
    rec[4] = (u8)arg2;
    PUT_U32_LE(rec, 5, arg3);
    return (u32)cm_fn_0068d0d0((volatile u8 *)(self + 0x92cu), rec, 9);
}
