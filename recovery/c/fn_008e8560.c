#include "cmre.h"

/* Callee prototypes as in the brief. The original sets ECX to the object before
   CALL 0x008e8c40 (see the listing), which the STDCALL prototype does not carry. */
extern u32 CM_CDECL cm_fn_0053d740(u8 *arg1, u32 *arg2);
extern u32 cm_fn_0053d980(int param_1, int *param_2, int param_3);
extern u32 CM_STDCALL cm_fn_008e8c40(u32 arg1, u32 arg2);

/* Object fields by byte offset from `self` (ECX on entry):
   0x04 base pointer of 12-byte records, 0x08 u8 record count, 0x0a pointer whose
   0x71 field leads to a dword compared with 0x009cfa10, 0x13/0x14 bytes copied into
   locals, 0x53 pointer to a dword, 0x57 pointer to a dword, 0xd7 table of dword pointers.
   Arguments: arg1 object pointer (ESI), arg2 count-checked pointer (EBP), arg4
   record base (ESI in the second loop), arg5 optional u16 result pointer. */
u32 CM_THISCALL cm_fn_008e8560(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u16 *arg5)
{
    u8 local_6;
    u8 local_5;
    u8 local_4;
    u8 local_3;
    u8 local_2;
    u8 local_1;
    u32 eax;
    u32 edx;
    u32 ecx;
    u32 P;
    u32 q;
    u32 ebp;
    u32 piVar1;
    u32 piVar5;
    u8 bl;
    u8 cnt;

    (void)arg3; /* the third stack argument (entry esp+0x0c) is never read */
    local_4 = 0;
    local_6 = *(volatile u8 *)(self + 0x14);
    local_5 = *(volatile u8 *)(self + 0x13);
    local_2 = 0;
    local_1 = 0;
    local_3 = 0;

    P = *(volatile u32 *)(arg1 + 0x53);
    eax = *(volatile u32 *)P;
    if (eax == CM_GLOBAL(volatile u32, 0x009cf4dc)) {
        q = *(volatile u32 *)(arg1 + 0x57);
        if (q == 0) {
            local_5 = 0;
            goto after_first;
        }
        edx = *(volatile u32 *)q;
        if (edx != CM_GLOBAL(volatile u32, 0x009cf8d8)) {
            local_5 = 0;
            goto after_first;
        }
    }
    if (eax == CM_GLOBAL(volatile u32, 0x009cf488)) {
        q = *(volatile u32 *)(arg1 + 0x57);
        if (q == 0) {
            local_6 = 0;
        } else {
            edx = *(volatile u32 *)q;
            if (edx == CM_GLOBAL(volatile u32, 0x009cf624)) {
                local_6 = 4;
            } else {
                eax = *(volatile u32 *)q;
                if (eax == CM_GLOBAL(volatile u32, 0x009cf628)) {
                    local_6 = 3;
                } else {
                    local_6 = 0;
                }
            }
        }
    }

after_first:
    ebp = arg2;
    if (ebp != 0 && cm_fn_0053d980((int)ebp, (int *)P, 1) != 0 && local_5 != 0xff) {
        bl = 0;
        piVar5 = arg1 + 0xd7;
        do {
            piVar1 = *(volatile u32 *)piVar5;
            if (piVar1 == 0
                || (s32)*(volatile u32 *)piVar1 >= (s32)CM_GLOBAL(volatile u32, 0x00ae2364)) {
                local_2 = local_2 + 1;
            } else if (cm_fn_0053d980((int)piVar1, (int *)*(volatile u32 *)(arg1 + 0x53), 1) == 0) {
                local_2 = local_2 + 1;
            } else {
                local_4 = local_4 + 1;
            }
            if (local_4 == local_5) {
                goto success1;
            }
            if ((s32)(s8)local_2 > 0x32 - (s32)(s8)local_5) {
                break;
            }
            bl = bl + 1;
            piVar5 = piVar5 + 4;
        } while ((s8)bl < 0x32);
        if (local_4 == local_5) {
            goto success1;
        }
    }

    eax = *(volatile u32 *)(self + 0xa);
    if (eax != 0) {
        eax = *(volatile u32 *)(eax + 0x71);
        if (eax != 0) {
            eax = *(volatile u32 *)eax;
            if (eax == CM_GLOBAL(volatile u32, 0x009cfa10) && ebp != 0) {
                ecx = *(volatile u32 *)(arg1 + 0x53);
                if (cm_fn_0053d980((int)ebp, (int *)ecx, 1) != 0) {
                    edx = *(volatile u32 *)(arg1 + 0x53);
                    if (cm_fn_0053d740((u8 *)ebp, (u32 *)edx) == 0 && local_6 != 0xff) {
                        bl = 0;
                        piVar5 = arg1 + 0xd7;
                        do {
                            piVar1 = *(volatile u32 *)piVar5;
                            if (piVar1 == 0
                                || (s32)*(volatile u32 *)piVar1 >= (s32)CM_GLOBAL(volatile u32, 0x00ae2364)) {
                                local_1 = local_1 + 1;
                            } else if (cm_fn_0053d980((int)piVar1,
                                                      (int *)*(volatile u32 *)(arg1 + 0x53), 1) == 0) {
                                local_1 = local_1 + 1;
                            } else if (cm_fn_0053d740((u8 *)*(volatile u32 *)piVar5, (u32 *)0) != 0) {
                                local_1 = local_1 + 1;
                            } else {
                                local_3 = local_3 + 1;
                            }
                            if (local_3 == local_6) {
                                goto success2;
                            }
                            if ((s32)(s8)local_1 > 0x32 - (s32)(s8)local_6) {
                                break;
                            }
                            bl = bl + 1;
                            piVar5 = piVar5 + 4;
                        } while ((s8)bl < 0x32);
                        if (local_3 == local_6) {
                            goto success2;
                        }
                    }
                }
            }
        }
    }

    cnt = *(volatile u8 *)(self + 8);
    bl = 0;
    if ((s8)cnt > 0) {
        do {
            ecx = *(volatile u32 *)(self + 4) + (u32)(s32)(s8)bl * 12u;
            if (cm_fn_008e8c40(arg4, ecx) != 0) {
                return 1;
            }
            cnt = *(volatile u8 *)(self + 8);
            bl = bl + 1;
        } while ((s8)bl < (s8)cnt);
    }
    if (cnt != 0) {
        if (arg5 != 0) {
            *(volatile u16 *)arg5 = 9;
        }
        return 0;
    }
    return 1;

success1:
    if (arg5 != 0) {
        *(volatile u16 *)arg5 = 0x14;
    }
    return 0;

success2:
    if (arg5 != 0) {
        *(volatile u16 *)arg5 = 0x15;
    }
    return 0;
}
