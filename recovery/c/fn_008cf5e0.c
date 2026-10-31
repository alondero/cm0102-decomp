/* Plain C recovery of the routine at preferred-image VA 0x008cf5e0 (image base 0x00400000).
   Thiscall receiver in ECX (self); two stack arguments (param_2, param_3; ret 8).
   Returns EAX as the original leaves it: the vtable-call result, or the low byte
   set to 9 / 0 with the upper EAX bits left from the last call or pointer value.
   Callees: 0x0053b900 (cdecl, one argument), 0x00418f60 (thiscall; receiver is the
   global at 0x00ad9c6c), 0x008d2fa0 (stdcall, four arguments, ECX not read). */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_THISCALL cm_fn_00418f60(u8 *self, u32 *index_pointer, u32 second_arg);
extern u32 CM_STDCALL cm_fn_008d2fa0(int param_1, u8 *param_2, int *param_3, u32 *param_4);

u32 CM_THISCALL cm_fn_008cf5e0(int param_1, u32 param_2, int param_3)
{
    /* ebp is copied from the stack argument at entry; the callee later writes
       the low byte of that argument slot, so the vtable call must use this copy. */
    u32 self = (u32)param_1;
    u32 ebp = (u32)param_3;
    u32 local_8;
    u32 local_4;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esi;
    u32 edi;
    u32 row;
    u32 match;
    u8 dl;

    if (ebp != 0) {
        eax = cm_fn_0053b900((u32 *)*(volatile u32 *)(ebp + 0x1c));
        if (eax == 0) {
            ecx = param_2;
            eax = cm_fn_00418f60((u8 *)(u32)CM_GLOBAL(volatile u32, 0x00ad9c6c),
                                 (u32 *)ecx, eax);
            if ((u8)eax != 0) {
                return (eax & 0xffffff00u) | 9u;
            }
        }
    }

    /* Callee writes one byte through &param_3, one dword through &local_4 and &local_8. */
    eax = cm_fn_008d2fa0((int)ebp, (u8 *)&param_3, (int *)&local_4, &local_8);
    dl = (u8)param_3;
    if (dl == 0xff) {
        return eax & 0xffffff00u;
    }
    ebx = local_4;
    eax = self + 0x8ac;
    esi = local_8;
    edi = 0;
    for (;;) {
        if (dl == 4) {
            break;
        }
        row = eax;
        ecx = *(volatile u32 *)row;
        if (dl == 2) {
            match = (*(volatile u32 *)(ecx + 0xe) == esi) &&
                    (*(volatile u8 *)(ecx + 0x12) == 2);
        } else {
            match = (*(volatile u32 *)(ecx + 0xa) == ebx) &&
                    (*(volatile u8 *)(ecx + 0x12) == dl);
        }
        if (match) {
            /* 0x008cf68a: call the object's vtable slot 6 with the object as ECX. */
            eax = *(volatile u32 *)row;
            ecx = param_2;
            edx = *(volatile u32 *)eax;
            return ((u32 (CM_THISCALL *)(void *, u32, u32))
                        (*(volatile u32 *)(edx + 0x18)))
                   ((void *)eax, param_2, ebp);
        }
        edi = edi + 1;
        eax = eax + 4;
        if (edi >= 0x1c) {
            break;
        }
    }
    return eax & 0xffffff00u;
}
