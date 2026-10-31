/* Preferred-image VA008e8a60/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/full EAX, including the high bytes left by each MOV/XOR AL.
   Original lookup/status callees remain explicit. Do not cache the repeated
   4CF5A0 calls or add null guards to native club-field reads. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_0053b900(u32 club);
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32 self, u32 club);
/* Native7E6F20 ignores incoming ECX, but retain the root's explicit load. */
extern u32 CM_THISCALL cm_fn_007e6f20(u32 receiver, u32 club);

u32 CM_THISCALL cm_fn_008e8a60(u32 self, u32 person, u32 match)
{
    u32 eax = D(self + 0x15u), receiver, value, state, club, base, id;
    u8 byte;
    if (eax == 0) {
        eax = D(match + 0x14u);
        receiver = D(0x00b74434);
        eax = cm_fn_007e6f20(receiver, eax);
        if (eax == 0) {
            value = D(match + 0x14u);
            receiver = D(0x00ae2c74);
            eax = (u32)cm_fn_005c6710(receiver, value);
            if (eax == 0) {
                receiver = D(0x00ae1050);
                eax = cm_fn_004cf5a0(receiver, person);
                if (eax != 0) {
                    receiver = D(0x00ae1050);
                    eax = cm_fn_004cf5a0(receiver, person);
                    byte = B(eax + 0x35u) & 0x3fu;
                    if (byte == 5) return (eax & 0xffffff00u) | 3u;
                }
            }
        }
    }
    receiver = D(0x00ae1050);
    eax = cm_fn_004cf6d0(receiver, person);
    state = eax;
    if (state == 0) goto no;
    eax = D(match + 0x1cu);
    eax = cm_fn_0053b900(eax);
    if (eax != 0) goto no;
    eax = D(person + 0x39u);
    value = D(match + 0x1cu);
    if (eax != value && eax != D(match + 0x20u)) goto no;
    byte = B(state + 0x1cu);
    if (byte == 0) {
        value = D(match + 0x14u);
        club = D(eax + 0x57u);
        if (value != club && value != D(eax + 0x5bu)) {
            receiver = D(0x00ae2c74);
            eax = (u32)cm_fn_005c6710(receiver, value);
            if (eax == 0) {
                value = D(match + 0x14u);
                receiver = D(0x00b74434);
                eax = cm_fn_007e6f20(receiver, value);
                if (eax == 0) return (eax & 0xffffff00u) | 0xau;
            }
        }
    }
    byte = B(state + 0x1du);
    eax = (eax & 0xffffff00u) | byte;
    if (byte != 0) goto no;
    receiver = D(0x00ae1050);
    eax = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080) = eax;
    if (eax != 0) {
        id = D(eax + 4u);
        base = D(0x00ae23b4);
        eax = base + id * 581u;
    }
    if (D(match + 0x1cu) == eax) return (eax & 0xffffff00u) | 0xbu;
    receiver = D(0x00ae1050);
    eax = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080) = eax;
    if (eax != 0) {
        id = D(eax + 4u);
        base = D(0x00ae23b4);
        eax = base + id * 581u;
    }
    if (D(match + 0x20u) == eax) return (eax & 0xffffff00u) | 0xbu;
no:
    return eax & 0xffffff00u;
}
