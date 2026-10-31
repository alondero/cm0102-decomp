/* Translation at preferred-image VA 0x0084cbc0, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Thiscall receiver (ECX) is the base of a table of u32 slots at +0..+0x24 (ten
   entries, indexed by signed 16-bit counter). Dword at +0x28 is tested for nonzero.
   Stack arguments: byte at [esp+4], dword at [esp+8], dword at [esp+0xc]; ret 0xc.
   Callees and vtable slots stay explicit boundaries. Field roles are unknown and
   are named by offset only. The failure exits use "or al, 0xff" and the success
   exit uses "mov al, bl", so the full EAX value is reproduced on every path. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0084cd10(u32 receiver, u32 index, u32 object);

typedef u32 CM_THISCALL thiscall_slot3(u32, u32, u32);
typedef u32 CM_STDCALL stdcall_slot2(u32, u32);
typedef u32 CM_STDCALL stdcall_slot4(u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0084cbc0(u32 base, s8 stack_4, u32 stack_8, u32 stack_c)
{
    u32 eax;
    u32 index = 0;
    u32 chosen = 0xffffffff;
    u32 timeout;
    u32 slot;
    u32 object;
    u32 vtable;

    /* eax holds the +0x28 dword on the failure exit; "or al, 0xff" keeps its upper bytes. */
    eax = *(volatile u32 *)(base + 0x28);
    if (eax == 0)
        return eax | 0xff;
    if (stack_8 == 0)
        return eax | 0xff;

    /* Scan slots 0..9 for the first zero entry; the loop stops once one is chosen. */
    for (;;) {
        if ((s16)index >= 10) {
            if ((s16)chosen == -1)
                return 0xff;
            break;
        }
        if (*(volatile u32 *)(base + (u32)((s32)(s16)index * 4)) == 0)
            chosen = index;
        index = index + 1;
        if ((s16)chosen != -1)
            break;
    }

    if (stack_4 == 0)
        timeout = 0xffffec78;
    else if (stack_4 == 1)
        timeout = 5000;
    else if (stack_4 == 2)
        timeout = 0;
    else
        /* movsx eax, byte; sub 0; dec; dec leaves EAX = sign-extended mode - 2. */
        return ((u32)(s32)stack_4 - 2u) | 0xffu;

    eax = cm_fn_0084cd10(base, chosen, stack_8);
    if (eax == 0)
        return 0xff;

    slot = base + (u32)((s32)(s16)chosen * 4);
    object = *(volatile u32 *)slot;
    if (object != 0) {
        /* mov ecx, [obj] makes the vtable slot at +0x40 a thiscall: receiver
           is the vtable value; pushed arguments are object, then timeout. */
        vtable = *(volatile u32 *)object;
        ((thiscall_slot3 *)*(volatile u32 *)(vtable + 0x40))(vtable, object, timeout);

        object = *(volatile u32 *)slot;
        vtable = *(volatile u32 *)object;
        ((stdcall_slot2 *)*(volatile u32 *)(vtable + 0x3c))(object, 10000);

        object = *(volatile u32 *)slot;
        vtable = *(volatile u32 *)object;
        eax = ((stdcall_slot4 *)*(volatile u32 *)(vtable + 0x30))(object, 0, 0, (stack_c != 0));
        /* mov al, bl: the callee's EAX keeps its upper three bytes. */
        return (eax & 0xffffff00u) | (chosen & 0xffu);
    }
    /* object == 0: EAX is zero here, so mov al, bl leaves only the low byte. */
    return chosen & 0xffu;
}
