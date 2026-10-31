#include "cmre.h"

/* Thiscall with two stack arguments (ret 8). ECX is the object pointer; it is
   saved in the frame and reloaded for the 0x0086f5b0 calls. arg1 is an array
   of dword pointers whose entries are copied to arg1[count] for the accepted
   entries; arg2 contributes its low word (signed) as the entry count.
   Returns the low byte (bl) = the number of accepted entries.

   SEH frame at entry esp E: prev at E-0x0c (fs:[0] is linked to it), handler
   at E-0x08 (0x00962e08), state at E-0x04 (-1, then 0 before 0x00945333).
   The 8-byte scratch record filled by 0x0054cab0 (two dwords; 0x1c is a byte
   argument, not a size) sits at E-0x14. Its first two
   words are read as signed (E-0x14, local_14) and unsigned (E-0x12, local_12). */

extern void CM_THISCALL cm_fn_0054cab0(u32 self, void *arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_0086f5b0(u32 self, u32 player, u32 mode);
extern void CM_CDECL cm_fn_00945333(u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u8 CM_THISCALL cm_fn_0086fab0(u32 self, u32 arg1, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    s16 scratch[4]; /* scratch[0] = local_14, scratch[1] = local_12 */
    s32 iVar5;
    s32 iVar3;
    s8 cVar2;
    u32 piVar4;
    u32 iVar1;
    u32 ptrval;
    u32 idx;

    seh.state = -1;
    seh.handler = 0x00962e08u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cVar2 = 0;
    cm_fn_0054cab0(0x00ae2c90u, scratch, 0x1cu);
    iVar5 = (s16)arg2;
    iVar3 = 0;
    seh.state = 0;
    cm_fn_00945333(arg1, (u32)iVar5, 4u, 0x00870220u);
    piVar4 = arg1;
    if (iVar5 > 0) {
        do {
            if (cVar2 >= 3) {
                CM_FS(u32, 0) = seh.prev;
                return (u8)cVar2;
            }
            ptrval = *(volatile u32 *)piVar4;
            iVar1 = *(volatile u32 *)(ptrval + 0xcfu);
            if (iVar1 != 0) {
                u16 f40 = *(volatile u16 *)(iVar1 + 0x40u);
                s16 l14 = scratch[0];
                u16 l12 = (u16)scratch[1];
                int proceed;

                if (l12 > f40) {
                    proceed = 1;
                } else if (l12 < f40) {
                    proceed = 0;
                } else {
                    s16 f3e = *(volatile s16 *)(iVar1 + 0x3eu);
                    proceed = (l14 > f3e);
                }
                if (proceed) {
                    iVar1 = cm_fn_0086f5b0(self, iVar1, 1u);
                    if (iVar1 != 0 && *(volatile u16 *)(iVar1 + 0x8eu) != 0) {
                        u32 v;
                        idx = (u32)(s32)cVar2;
                        cVar2 = (s8)(cVar2 + 1);
                        v = *(volatile u32 *)piVar4;
                        *(volatile u32 *)(arg1 + idx * 4u) = v;
                    }
                }
            }
            iVar3 = iVar3 + 1;
            piVar4 = piVar4 + 4u;
        } while (iVar3 < iVar5);
    }
    CM_FS(u32, 0) = seh.prev;
    return (u8)cVar2;
}
