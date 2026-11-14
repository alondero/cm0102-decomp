/* Preferred-image VA007701f0, base00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original helper calls, uninitialized mode padding and native faults retained. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_0053b4f0(u32);
extern u32 CM_CDECL cm_fn_0053c590(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u32, u32, u32);
extern u32 CM_STDCALL cm_fn_0062ec00(u32);
extern u32 CM_THISCALL cm_fn_0062ed70(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

u32 CM_STDCALL cm_fn_007701f0(volatile u32 person_arg, volatile u32 nation_arg,
                            volatile u32 kind_arg, volatile u32 fourth_arg)
{
    struct {
        union { volatile u32 word; volatile u8 bytes[4]; } mode;
        u8 news[224];
        volatile struct { u32 prev, handler, state; } seh;
    } local;
    u32 previous = CM_FS(u32, 0);
    local.seh.state = 0xffffffffu;
    local.seh.handler = 0x0095de7bu;
    local.seh.prev = previous;
    u8 kind = *(volatile u8 *)&kind_arg;
    CM_FS(u32, 0) = (u32)&local.seh;
    u32 nation = nation_arg;
    u32 club, flag;
    if (kind == 0) {
        club = cm_fn_0053b3d0(nation);
        flag = 0;
        local.mode.bytes[0] = 4;
    } else if (kind == 1) {
        club = cm_fn_0053b3d0(nation);
        flag = 1;
        local.mode.bytes[0] = 8;
    } else {
        club = cm_fn_0053b4f0(nation);
        flag = 0;
        local.mode.bytes[0] = 4;
    }
    cm_fn_0077bbe0(local.news, 0x22u, 0);
    u32 person = person_arg;
    local.seh.state = 0;
    u32 value = R32(person);
    cm_fn_00785790(local.news, 0, value);
    value = R32(person + 4u);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.news, 1u, value);
    value = R32(person + 8u);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.news, 2u, value);
    value = R32(person + 0x0cu);
    value = R32(value + 0x33u);
    cm_fn_00785790(local.news, 3u, value);
    value = R32(club);
    cm_fn_00785790(local.news, 4u, value);
    cm_fn_00785790(local.news, 5u, flag);
    u32 receiver = R32(0x00b63d28u);
    u32 eax = cm_fn_0062ed70(receiver, person);
    if (eax != 0) {
        receiver = R32(0x00b63d28u);
        eax = cm_fn_0062ed70(receiver, person);
        value = R8(eax);
        receiver = R32(0x00b63d28u);
        /* Native62ec00 consumes only this BYTE, kills EAX/ECX and makes no
           calls or writes. The caller replaces its popped DWORD before reuse. */
        eax = cm_fn_0062ec00(value);
        value = (u32)(s32)(s8)eax;
    } else {
        value = 0;
    }
    cm_fn_00785790(local.news, 6u, value);
    cm_fn_00785790(local.news, 7u, 0);
    /* Original770320 loads the whole word after initializing only its BYTE.
       No padding initialization or guessed high24 bits. Original53c590 reads
       its formal mode as BYTE; the next call overwrites the popped arguments.
       Pointer aliases into native frame storage remain outside this proof. */
    value = local.mode.word;
    eax = cm_fn_0053c590(club, value, 0);
    value = (u32)(s32)(s8)eax;
    cm_fn_00785790(local.news, 8u, value);
    value = (u32)(s32)*(volatile s8 *)&fourth_arg;
    cm_fn_00785790(local.news, 9u, value);
    value = (u32)(s32)*(volatile s8 *)&kind_arg;
    cm_fn_00785790(local.news, 10u, value);
    value = R32(nation);
    cm_fn_00785790(local.news, 11u, value);
    eax = cm_fn_00600a50(club, 0, 1u, 0, 0);
    if (eax != 0) {
        receiver = R32(0x00b7431cu);
        eax = (u32)cm_fn_007861e0((void *)receiver, (u32)local.news, club);
    } else {
        value = cm_fn_0053b3d0(nation);
        eax = cm_fn_00600a50(value, 0, 1u, 0, 0);
        if (eax != 0) {
            value = cm_fn_0053b3d0(nation);
            receiver = R32(0x00b7431cu);
            eax = (u32)cm_fn_007861e0((void *)receiver, (u32)local.news, value);
        }
    }
    CM_FS(u32, 0) = local.seh.prev;
    return eax;
}
