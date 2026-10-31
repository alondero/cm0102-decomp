#include "cmre.h"

/* Thiscall (ECX is read first, so ECX is the receiver), ret 0, returns u32 in EAX:
   the matching record pointer, or 0.

   Receiver fields by byte offset: 0x19 byte, 0x27 byte (signed where the listing
   sign-extends it), 0x106 byte, 0x19e dword (record base, read again where the
   listing re-reads it).
   Record fields by byte offset from the record pointer: 0x19 byte (signed test),
   0x69 dword, 0x19e dword.
   The record stride is 0x1be bytes. Two stack arguments are char-typed:
   0x005b57c0 and 0x005b58e0 each read only the low byte of their stack argument
   (`movsx eax, byte ptr [esp + 4]` is the first instruction of both), so the
   upper bits of the pushed EAX and EDX are never observed. */

extern u32 CM_THISCALL cm_fn_005b57c0(int param_1, char param_2);
extern int CM_THISCALL cm_fn_005b58e0(int param_1, char param_2);
extern u32 CM_THISCALL cm_fn_006c38c0(int param_1, int param_2);
extern u32 CM_THISCALL cm_fn_006c38f0(int param_1, int param_2);

u32 CM_THISCALL cm_fn_007160f0(u32 self)
{
    u8 a19;
    u8 bl;
    u8 dl_val;
    u8 cl;
    s32 s;
    s32 z;
    s32 b;
    s32 r2;
    u32 r;
    u32 base;
    u32 base2;
    u32 ebp;
    u32 edi;
    u32 ebx;
    u32 eax;
    u32 ecx;
    u32 flags_word;
    int found;
    int take106;

    /* First test: 0x005b57c0 with the receiver built from [self+0x19e]. */
    a19 = *(volatile u8 *)(self + 0x19u);
    s = (s32)*(volatile s8 *)(self + 0x27u);
    base = *(volatile u32 *)(self + 0x19eu);
    r = cm_fn_005b57c0((int)(base + (u32)s * 6371u + 0x91e2u), (char)a19);
    if ((r & 4u) == 0u) {
        return 0u;
    }

    /* Second call: 0x005b58e0, its result is kept in EAX for the loop compare. */
    s = (s32)*(volatile s8 *)(self + 0x27u);
    a19 = *(volatile u8 *)(self + 0x19u);
    base2 = *(volatile u32 *)(self + 0x19eu);
    r2 = cm_fn_005b58e0((int)(base2 + (u32)s * 6371u + 0x91e2u), (char)a19);

    /* Record search: start at a stride-0x1be record chosen by whether the byte
       at [self+0x27] is zero; walk up to 0x14 records. */
    bl = *(volatile u8 *)(self + 0x27u);
    ebp = *(volatile u32 *)(self + 0x19eu);
    z = (bl == 0u) ? 1 : 0;
    edi = ebp + (u32)z * 8920u + 0x4796u;

    found = 0;
    cl = 0u;
    do {
        dl_val = *(volatile u8 *)(edi + 0x19u);
        if (!((s8)dl_val < 0)) {
            if (*(volatile s32 *)(edi + 0x69u) == r2) {
                found = 1;
                break;
            }
        }
        cl = (u8)(cl + 1u);
        edi = edi + 0x1beu;
    } while (cl < 0x14u);

    if (!found) {
        return 0u;
    }

    /* Found record: 0x005b57c0 again, with the record base moved by bl. */
    b = (s32)(s8)bl;
    ebp = ebp + (u32)b;
    dl_val = *(volatile u8 *)(self + 0x19u);
    r = cm_fn_005b57c0((int)(ebp + (u32)b * 6370u + 0x91e2u), (char)dl_val);
    if ((r & 0x10u) == 0u) {
        return edi;
    }

    bl = *(volatile u8 *)(self + 0x27u);
    base2 = *(volatile u32 *)(self + 0x19eu);
    z = (bl == 0u) ? 1 : 0;
    ebx = base2 + (u32)z;
    flags_word = *(volatile u32 *)(ebx + (u32)z * 6370u + 0xaa5du);

    take106 = 0;
    if ((flags_word & 0x10000000u) != 0u) {
        take106 = 1;
    } else if ((s8)*(volatile u8 *)((u32)z + base2 + 0xafu) > 1) {
        take106 = 1;
    }
    if (take106) {
        if (*(volatile u8 *)(self + 0x106u) != 0u) {
            return 0u;
        }
    }

    /* Ranking step: two record-owned calls compared in BX/AX (16 bits). */
    ecx = *(volatile u32 *)(edi + 0x19eu);
    if (ecx == 0u) {
        ebx = 0u;
    } else {
        ebx = cm_fn_006c38f0((int)ecx, (int)edi);
    }

    ecx = *(volatile u32 *)(edi + 0x19eu);
    if (ecx == 0u) {
        eax = 0u;
    } else {
        eax = cm_fn_006c38c0((int)ecx, (int)edi);
    }
    if ((u16)ebx == (u16)eax) {
        return edi;
    }

    ecx = *(volatile u32 *)(edi + 0x19eu);
    if (ecx == 0u) {
        eax = 0u;
    } else {
        eax = cm_fn_006c38f0((int)ecx, (int)edi);
    }
    if ((eax & 0x40u) != 0u) {
        return edi;
    }

    if (*(volatile u8 *)(self + 0x106u) != 0u) {
        return 0u;
    }
    return edi;
}
