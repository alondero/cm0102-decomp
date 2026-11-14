/* Preferred-image VA0076fd10, base00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original helpers, physical receiver transport and native SEH remain boundaries. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_0053b4f0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);
/* One stack word/RET4 plus the actual caller-set ECX. The recovered callee's
   narrower stdcall declaration needs reconciliation before selected binding. */
extern u32 CM_THISCALL cm_fn_007875e0(u32 physical_receiver, u32 club);

u32 CM_STDCALL cm_fn_0076fd10(volatile u32 person_arg, volatile u32 nation_arg,
                            volatile u32 kind_arg)
{
    struct {
        u8 scratch[224];
        volatile struct { u32 prev, handler, state; } seh;
    } local;
    u32 previous = CM_FS(u32, 0);
    local.seh.state = 0xffffffffu;
    local.seh.handler = 0x0095de5bu;
    local.seh.prev = previous;
    CM_FS(u32, 0) = (u32)&local.seh;
    u32 person = person_arg;
    u32 eax = R32(person + 0x39u);
    u32 receiver, id, base, value;
    if (eax != 0) {
        receiver = R32(0x00b7431cu);
        eax = cm_fn_007875e0(receiver, eax);
        if (eax != 0)
            goto construct;
    }
    receiver = R32(0x00ae1050u);
    eax = cm_fn_004cf5a0(receiver, person);
    R32(0x00ae1080u) = eax;
    if (eax == 0)
        goto done;
    id = R32(eax + 4u);
    base = R32(0x00ae23b4u);
    eax = base + id * 581u;
    if (eax == 0)
        goto done;
    receiver = R32(0x00ae1050u);
    eax = cm_fn_004cf5a0(receiver, person);
    R32(0x00ae1080u) = eax;
    if (eax != 0) {
        id = R32(eax + 4u);
        base = R32(0x00ae23b4u);
        eax = base + id * 581u;
    }
    receiver = R32(0x00b7431cu);
    eax = cm_fn_007875e0(receiver, eax);
    if (eax == 0)
        goto done;

construct:
    cm_fn_0077bbe0(local.scratch, 0x21u, 0);
    value = R32(person);
    local.seh.state = 0;
    cm_fn_00785790(local.scratch, 0, value);
    value = R32(person + 4u);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.scratch, 1u, value);
    value = R32(person + 8u);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.scratch, 2u, value);
    value = R32(person + 0x0cu);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.scratch, 3u, value);

    u8 kind = *(volatile u8 *)&kind_arg;
    value = nation_arg;
    if (kind == 0) {
        eax = cm_fn_0053b3d0(value);
        value = R32(eax);
        cm_fn_00785790(local.scratch, 4u, value);
        cm_fn_00785790(local.scratch, 5u, 0);
    } else if (kind == 1) {
        eax = cm_fn_0053b3d0(value);
        value = R32(eax);
        cm_fn_00785790(local.scratch, 4u, value);
        cm_fn_00785790(local.scratch, 5u, 1u);
    } else {
        eax = cm_fn_0053b4f0(value);
        value = R32(eax);
        cm_fn_00785790(local.scratch, 4u, value);
        cm_fn_00785790(local.scratch, 5u, 0);
    }

    value = R32(person + 0x39u);
    if (value != 0) {
        receiver = R32(0x00b7431cu);
        cm_fn_007861e0((void *)receiver, (u32)local.scratch, value);
    }
    receiver = R32(0x00ae1050u);
    eax = cm_fn_004cf5a0(receiver, person);
    R32(0x00ae1080u) = eax;
    if (eax != 0) {
        id = R32(eax + 4u);
        base = R32(0x00ae23b4u);
        eax = base + id * 581u;
    }
    if (eax == R32(person + 0x39u))
        goto done;
    receiver = R32(0x00ae1050u);
    eax = cm_fn_004cf5a0(receiver, person);
    R32(0x00ae1080u) = eax;
    if (eax != 0) {
        id = R32(eax + 4u);
        base = R32(0x00ae23b4u);
        eax = base + id * 581u;
    }
    receiver = R32(0x00b7431cu);
    eax = (u32)cm_fn_007861e0((void *)receiver, (u32)local.scratch, eax);
done:
    CM_FS(u32, 0) = local.seh.prev;
    return eax;
}
