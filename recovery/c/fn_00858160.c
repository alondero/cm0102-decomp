/* fn_00858160 -- preferred-image VA 0x00858160 (image base 0x00400000).
   Module unknown. Behaviour recovered from the original listing only:
   thiscall entry, ECX receiver, two caller stack arguments, `ret 8`.

   Listing shape:
     push esi
     mov  esi, ecx
     cmp  dword ptr [esi+0x30], 7
     jne  end
     mov  eax, dword ptr [esi+0x0c]
     push 0
     push 0
     mov  ecx, dword ptr [eax+0x1c]
     mov  edx, dword ptr [ecx]
     call dword ptr [edx+0x10]
     test al, al
     je   end
     mov  eax, dword ptr [esi]
     mov  ecx, esi
     call dword ptr [eax+0x28]
   end:
     mov  ecx, dword ptr [esp+0x0c]   ; second stack argument
     mov  edx, dword ptr [esp+0x08]   ; first stack argument
     push ecx
     push edx
     mov  ecx, esi
     call 0x006847c0
     pop  esi
     ret  8
*/
#include "cmre.h"

/* vtable slot shapes are unknown; the listing fixes only the receiver in ECX,
   the 8 bytes of stack arguments cleaned by the callee and the AL result of
   slot +0x10, and the absence of stack arguments for slot +0x28. */
typedef u8  (CM_THISCALL *cm_vtbl_slot10_t)(void *self, u32 arg0, u32 arg1);
typedef u32 (CM_THISCALL *cm_vtbl_slot28_t)(void *self);

extern u32 CM_THISCALL cm_fn_006847c0(int *param_1, short *param_2, int param_3);

u32 CM_THISCALL cm_fn_00858160(int *param_1, u32 param_2, u32 param_3)
{
    int *this_ = param_1;
    u32  field_0x0c;
    u32  sub_object;
    u32  vtable;
    u8   slot_result;

    if (*(volatile u32 *)((u8 *)this_ + 0x30) == 7) {
        field_0x0c = *(volatile u32 *)((u8 *)this_ + 0x0c);
        sub_object  = *(volatile u32 *)((u8 *)field_0x0c + 0x1c);
        vtable      = *(volatile u32 *)sub_object;
        slot_result = (*(cm_vtbl_slot10_t *)(vtable + 0x10))((void *)sub_object, 0, 0);
        if (slot_result != 0) {
            vtable = *(volatile u32 *)this_;
            (*(cm_vtbl_slot28_t *)(vtable + 0x28))((void *)this_);
        }
    }

    return cm_fn_006847c0(this_, (short *)param_2, (int)param_3);
}
