#include "cmre.h"

/* Preferred-image VA 0x0075c390, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Ret 0xc. Preserve full EAX on every exit, including the FS predecessor on
   the null first-argument path and the complete value forwarded in slot 7. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern s32 CM_CDECL cm_fn_0090cfc0(s32);
extern u32 CM_CDECL cm_fn_00757f30(u32);
extern u32 CM_THISCALL cm_fn_007dab90(u32, u32, u32, u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_STDCALL cm_fn_0075c390(u32 person, u32 club, s8 kind)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 packet[224];
    u32 eax, entry_prev, contract, value, pointer, status, nation, receiver;
    u32 high, low;
    s32 elapsed;

    entry_prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095d9fbu;
    frame.prev = entry_prev;
    CM_FS(u32, 0) = (u32)&frame;
    eax = entry_prev;
    if (person == 0) goto done;
    eax = D(person + 0xcfu);
    if (eax == 0 || club == 0) goto done;
    eax = cm_fn_0053b900((u32 *)person);
    if (eax != 0) goto done;
    cm_fn_0077bbe0(packet, 0x2e, 0);
    contract = D(person + 0xcfu);
    frame.state = 0;
    value = D(contract);
    cm_fn_00785790(packet, 0, value);
    contract = D(person + 0xcfu);
    pointer = D(contract + 4u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(packet, 1, value);
    contract = D(person + 0xcfu);
    pointer = D(contract + 8u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(packet, 2, value);
    contract = D(person + 0xcfu);
    pointer = D(contract + 12u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(packet, 3, value);
    value = D(person);
    cm_fn_00785790(packet, 4, value);
    value = D(club);
    cm_fn_00785790(packet, 5, value);
    value = (u32)cm_fn_0090cfc0(2);
    cm_fn_00785790(packet, 6, value);
    status = cm_fn_00757f30(person);
    cm_fn_00785790(packet, 7, status);
    pointer = D(person + 0x53u);
    nation = pointer != 0 ? D(pointer) : 0xffffffffu;
    receiver = D(0x00b74430u);
    value = cm_fn_007dab90(receiver, person, status, nation);
    cm_fn_00785790(packet, 8, value);
    cm_fn_00785790(packet, 9, (u32)(s32)kind);
    pointer = D(0x00b67a44u);
    value = 5000;
    if (pointer != 0) {
        value = D(person);
        pointer = D(pointer);
        value = (u32)(s32)(s16)W(value * 73u + pointer + 6u);
    }
    cm_fn_00785790(packet, 10, value);
    value = D(person + 0x57u);
    cm_fn_00785790(packet, 11, value == club);
    contract = D(person + 0xcfu);
    high = D(contract + 0x42u);
    low = D(contract + 0x3eu);
    elapsed = cm_fn_0054cdb0(0x00ae2c90u, low, high);
    eax = (u32)cm_fn_00785790(packet, 12, (u32)elapsed);
    if (elapsed < 180) goto done;
    value = (u32)(s32)(s16)W(person + 0x80u);
    cm_fn_00785790(packet, 13, value);
    receiver = D(0x00b7431cu);
    eax = (u32)cm_fn_007861e0((void *)receiver, (u32)packet, person);
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
