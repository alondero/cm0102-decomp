/* Preferred-image VA 0x008cf6b0, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 0xc with ECX read first (copied to the saved slot and to ebx): thiscall
   with three stack arguments. Stack arguments are addressed by their slots:
   param_2 (arg1) is passed as &param_2 to 0x008d2fa0 and written back; param_3
   (arg2, buffer pointer) is written as a byte through &param_3 by 0x008d2fa0, and
   the listing reads that low byte as the status (dl). The saved ECX slot is local_4.
   Vtable slot 0x1c is called with ECX = the object, then arg1 = ebp, arg2 = esi
   (registers loaded at entry). Its result is overwritten by the reload of arg3
   (param_4), so the slot's EAX is dead. EAX on each return path follows the listing
   (0xffffffff, the 0x008d2fa0 result, or 0). */
#include "cmre.h"

extern u32 CM_STDCALL cm_fn_008d2fa0(u32 param_1, u8 *param_2, u32 *param_3, u32 *param_4);

/* rep stosd of ten dwords 0xffffffff followed by stosw 0xffff, from p. */
static void cm_fill_ff_42(u32 p)
{
    u32 i;

    for (i = 0; i < 10u; i++)
        *(volatile u32 *)(p + 4u * i) = 0xffffffffu;
    *(volatile u16 *)(p + 40u) = 0xffffu;
}

u32 CM_THISCALL cm_fn_008cf6b0(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 local_4;
    u32 ebx_this;
    u32 ebp_a1;
    u32 esi_a2;
    u32 eax_ret;
    u32 table;
    u32 i;
    u32 obj;
    u32 obj2;
    u32 field;
    u32 vtbl;
    u32 fnptr;
    u8 dl;
    u8 bl;

    local_4 = param_1;
    ebx_this = param_1;
    ebp_a1 = param_2;
    esi_a2 = param_3;
    cm_fill_ff_42(esi_a2);
    if (ebp_a1 == 0u)
        return 0xffffffffu;

    eax_ret = cm_fn_008d2fa0(ebp_a1, (u8 *)&param_3, &local_4, &param_2);
    dl = *(volatile u8 *)&param_3;
    if (dl == 0xffu)
        return eax_ret;

    table = ebx_this + 0x8acu;
    for (i = 0; i < 0x1cu; i++) {
        if (dl == 4u)
            continue;
        obj = *(volatile u32 *)(table + 4u * i);
        if (dl == 2u) {
            field = *(volatile u32 *)(obj + 0xeu);
            if (field == param_2 && *(volatile u8 *)(obj + 0x12u) == 2u)
                goto call_slot;
        } else {
            field = *(volatile u32 *)(obj + 0xau);
            if (field == local_4 && *(volatile u8 *)(obj + 0x12u) == dl)
                goto call_slot;
        }
    }
    goto after_loop;

call_slot:
    obj2 = *(volatile u32 *)(table + 4u * i);
    vtbl = *(volatile u32 *)obj2;
    fnptr = *(volatile u32 *)(vtbl + 0x1cu);
    ((void (CM_THISCALL *)(u32, u32, u32))fnptr)(obj2, ebp_a1, esi_a2);

after_loop:
    if (param_4 == 0u)
        return 0u;

    dl = *(volatile u8 *)(esi_a2 + 5u);
    if (dl == 0xffu) {
        dl = *(volatile u8 *)(esi_a2 + 3u);
        if (dl == 0xffu)
            goto path_b;
    }
    cm_fill_ff_42(esi_a2);
    *(volatile u8 *)(esi_a2 + 5u) = dl;
    return 0xffffffffu;

path_b:
    dl = *(volatile u8 *)(esi_a2 + 2u);
    if (dl == 0xffu) {
        dl = *(volatile u8 *)(esi_a2 + 0u);
        if (dl == 0xffu)
            goto path_c;
    }
    cm_fill_ff_42(esi_a2);
    *(volatile u8 *)(esi_a2 + 2u) = dl;
    return 0xffffffffu;

path_c:
    dl = *(volatile u8 *)(esi_a2 + 0x16u);
    if (dl == 0xffu) {
        cm_fill_ff_42(esi_a2);
        return 0xffffffffu;
    }
    bl = *(volatile u8 *)(esi_a2 + 0x17u);
    cm_fill_ff_42(esi_a2);
    *(volatile u8 *)(esi_a2 + 0x16u) = dl;
    *(volatile u8 *)(esi_a2 + 0x17u) = bl;
    return 0xffffffffu;
}
