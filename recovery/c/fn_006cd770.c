#include "cmre.h"

/* Offsets from the object pointer (self, ECX on entry):
   0xf582 pointer released through 0x006f0970 when nonzero,
   0x8ea7 / 0x8ea8 / 0x8eaa / 0x8ea9 / 0x8eab / 0x8eae byte fields,
   0x8ed8 word field, 0x8eb2 byte flag, 0x8d46 + index byte table,
   0x4796 + 8920*k + 446*m record rows with byte fields 0x19 and 0x105.
   Record at arg7 has a byte field at 0x27. Word table at VA 0x00b67c30. */
#define REC_U32(addr, off) (*(volatile u32 *)((u32)(addr) + (u32)(off)))
#define REC_U16(addr, off) (*(volatile u16 *)((u32)(addr) + (u32)(off)))
#define REC_U8(addr, off) (*(volatile u8 *)((u32)(addr) + (u32)(off)))

extern u32 CM_CDECL cm_fn_0090cfc0(u32 value);
extern void CM_FASTCALL cm_fn_006f0970(u32 self);
extern u32 CM_THISCALL cm_fn_006cd490(u32 self, u32 value);
extern void CM_THISCALL cm_fn_007160d0(u32 self, u32 value);
extern void CM_THISCALL cm_fn_0070c040(u32 self, u32 value);
extern void CM_THISCALL cm_fn_0070a910(u32 self, u32 value);

void CM_THISCALL cm_fn_006cd770(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4,
                                u32 arg5, u32 arg6, u32 arg7)
{
    u32 ecx_val;
    u32 a;
    u32 r;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u32 g;
    u32 p2;
    u32 e;
    u32 m;
    u32 edi;
    u32 s7;
    u32 to_a02;
    u32 idx;
    u8 al;
    u8 al2;
    u8 cl;
    u8 dl;
    u8 bl;
    u8 v;
    u8 k;
    u16 ax;

    ecx_val = REC_U32(self, 0xf582);
    if (ecx_val != 0u) {
        cm_fn_006f0970(ecx_val);
    }
    al = (u8)arg1;
    cl = (u8)arg2;
    REC_U8(self, 0x8ea7) = al;
    al = REC_U8(self, 0x8eae);
    REC_U8(self, 0x8ea8) = cl;
    if (al != 0xffu) {
        REC_U8(self, 0x8eae) = 0xffu;
    }

    if ((u8)arg4 == 0xffu) {
        /* sete writes only the low byte of the arg4 slot */
        arg4 = (arg4 & 0xffffff00u) | (cm_fn_0090cfc0(2u) == 0u);
    }

    p2 = 0u;
    if ((u8)arg5 == 0u) {
        al = REC_U8(self, 0x8ea7);
        if (((s8)al < 2 || (s8)al > 6) && cm_fn_0090cfc0(4u) != 0u) {
            p2 = 1u;
        }
    }

    if (p2 != 0u) {
        g = ((s8)REC_U8(self, 0x8ea7) >= 2);
        arg1 = (arg1 & 0xffffff00u) | (g ? 9u : 0xffu);
        a = (u32)((s32)(s8)arg3 + 1);
        r1 = cm_fn_0090cfc0(a);
        bl = (u8)(REC_U8(self, 0x8ea8) - (u8)r1);
        r2 = cm_fn_0090cfc0(a);
        bl = (u8)(bl + (u8)r2);
    } else {
        do {
            a = (u32)((s32)(s8)arg3 + 1);
            r1 = cm_fn_0090cfc0(a);
            bl = (u8)(REC_U8(self, 0x8ea7) - (u8)r1);
            r2 = cm_fn_0090cfc0(a);
            bl = (u8)(bl + (u8)r2);
            arg1 = (arg1 & 0xffffff00u) | bl;
            r3 = cm_fn_0090cfc0(a);
            bl = (u8)(REC_U8(self, 0x8ea8) - (u8)r3);
            r4 = cm_fn_0090cfc0(a);
            bl = (u8)(bl + (u8)r4);
        } while ((u8)arg5 != 0u && ((s8)bl < 0 || (s8)bl > 11));
    }

    if ((u8)arg3 == 0u) {
        v = 0u;
    } else {
        v = (u8)(cm_fn_0090cfc0((u32)((s32)(s8)arg3 + 1)) + 1u);
    }

    cl = REC_U8(self, 0x8ea7);
    dl = REC_U8(self, 0x8ea8);
    REC_U8(self, 0x8eaa) = v;
    al = (u8)((u8)arg1 - cl);
    cl = (u8)(bl - dl);
    if (al != 0u || cl != 0u) {
        ax = REC_U16(0x00b67c30u, 2u * ((u32)(s32)(s8)cl + 24u * (u32)(s32)(s8)al));
    } else {
        ax = REC_U16(self, 0x8ed8);
    }
    REC_U16(self, 0x8ed8) = ax;
    r = cm_fn_0090cfc0(4u);
    REC_U8(self, 0x8ea9) = (u8)r;
    REC_U8(self, 0x8ea7) = (u8)arg1;
    al = (u8)arg4;
    REC_U8(self, 0x8ea8) = bl;
    REC_U8(self, 0x8eab) = al;

    if (al != 0xffu) {
        idx = (u32)(s32)(s8)al;
        bl = (u8)(REC_U8(self, 0x8d46 + idx) + 1u);
        REC_U8(self, 0x8d46 + idx) = bl;
        e = (al == 0u);
        al2 = REC_U8(self, 0x8d46 + e);
        if ((s8)al2 > 0) {
            REC_U8(self, 0x8d46 + e) = (u8)(al2 - 10u);
        }
    }

    k = 0u;
    while ((u8)arg6 != 0u) {
        ecx_val = (u32)(s32)(s8)k;
        m = 0x14u;
        edi = self + 0x4796u + 8920u * ecx_val;
        do {
            if ((s8)REC_U8(edi, 0x19) >= 0) {
                if (REC_U8(edi, 0x105) == 0u) {
                    if (cm_fn_0090cfc0(3u) == 0u) {
                        r = cm_fn_0090cfc0(2u);
                        cm_fn_007160d0(edi, r);
                    }
                }
            }
            edi = edi + 0x1beu;
            m = m - 1u;
        } while (m != 0u);
        k = (u8)(k + 1u);
        if ((s8)k >= 2) {
            break;
        }
    }

    s7 = arg7;
    to_a02 = 0u;
    if (s7 != 0u && REC_U8(self, 0x8ea7) == 4u) {
        al = REC_U8(self, 0x8ea8);
        if (al == 0xffu || al == 0x0cu) {
            if (cm_fn_0090cfc0(4u) != 0u) {
                al = REC_U8(self, 0x8ea8);
                if (al == 0xffu) {
                    if (REC_U8(s7, 0x27) != 0u) {
                        cm_fn_0070a910(s7, 0x3f800000u);
                    } else {
                        cm_fn_0070c040(s7, 0x3f800000u);
                    }
                } else if (al == 0x0cu) {
                    if (REC_U8(s7, 0x27) == 0u) {
                        cm_fn_0070a910(s7, 0x3f800000u);
                    } else {
                        cm_fn_0070c040(s7, 0x3f800000u);
                    }
                }
                to_a02 = 1u;
            }
        }
    }
    if (to_a02 != 0u && REC_U8(self, 0x8eb2) != 0u) {
        return;
    }
    cm_fn_006cd490(self, arg4);
}
