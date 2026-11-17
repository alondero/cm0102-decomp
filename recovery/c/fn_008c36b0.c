#include "cmre.h"
/*
 * Original instruction authority: preferred-image VAs 008c36b0-008c38a9,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Entry ECX is saved at 008c36b1; RET 12 at 008c38a9.
 * Calls use saved ECX and right-to-left pushes at 008c36f4-008c36ff,
 * 008c379b-008c37a6, 008c37b4-008c37bb, 008c380f-008c381a,
 * 008c389a-008c38a2; lookup receiver at 008c3722.
 */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern void CM_THISCALL cm_fn_008bf680(u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_008bfcb0(u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_008c1290(u32, u32);
extern u32 CM_THISCALL cm_fn_008c6220(u32, u32, s8, u8);
extern u32 CM_THISCALL cm_fn_008dda90(u32, u32, u32);
/* 008c378a/008c378b/008c3790, 008c3802/008c3803/008c3808,
 * 008c3868/008c3869/008c386e: one DWORD push, caller cleanup 4.
 * Only EAX is consumed at each return. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define B(a) CM_GLOBAL(volatile u8, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))

void CM_THISCALL cm_fn_008c36b0(u32 self, volatile u32 record,
                               u32 other, u32 reason)
{
    u32 eax, edx, index, mode;
    u8 value;
    value = B(record + 0x2c);
    if (value == 0x11 || value == 0x12) return;
    value = B(record + 0x2e);
    B(record + 0x2e) = (u8)(value + 1);
    eax = D(other);
    if (eax == D(record + 8)) {
        mode = reason;
        B(record + 0x2c) = 0x12;
        if ((u16)mode == 8) {
            cm_fn_008c6220(self, record, 7, 0);
            return;
        }
        index = D(record + 12);
        edx = index * 55;
        eax = D(0xae23bc) + edx * 2;
        eax = cm_fn_004cf5a0(D(0xae1050), eax);
        D(0xae1080) = eax;
        if (eax) {
            edx = (edx & 0xffffff00u) | (B(eax + 0x35) & 0x3f);
            if ((u8)edx != 5) {
                index = D(eax + 4);
                edx = D(0xae23b4) + index;
                eax = edx + index * 580;
                if (eax && cm_fn_00600a50(eax, 0, 1, 0, 0))
                    goto finish;
            }
        }
        value = B(record + 0x2e);
        if (value <= 1) goto finish;
        eax = (u32)value * 2;
        if (!cm_fn_0090cfc0(eax)) goto finish;
        cm_fn_008bf680(self, (u32)&record, 0, 6);
        return;
    }
    B(record + 0x2c) = 0x11;
    cm_fn_008c1290(self, record);
    eax = record;
    mode = reason;
    index = D(eax + 8);
    edx = D(0xae23b4) + index + index * 580;
    eax = cm_fn_00600a50(edx, 0, 1, 0, 0);
    if ((u16)mode == 8) {
        if (!eax) {
            eax = B(record + 0x2e);
            eax *= 2;
            if (cm_fn_0090cfc0(eax)) {
                cm_fn_008bfcb0(self, (u32)&record, 0, 6);
                return;
            }
        }
        cm_fn_008c6220(self, record, 7, 0);
        return;
    }
    if (!eax) {
        value = B(record + 0x2e);
        if (value > 1) {
            eax = (u32)value * 2;
            if (cm_fn_0090cfc0(eax)) {
                cm_fn_008bfcb0(self, (u32)&record, 0, 6);
                return;
            }
        }
    }
finish:
    cm_fn_008c6220(self, record, 7, 0);
    cm_fn_008dda90(self, record, mode);
}
