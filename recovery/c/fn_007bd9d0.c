#include "cmre.h"

/* Field accessors by byte offset from the record (param_1) and the object
   (param_2). Volatile keeps the listing's read order and re-reads after the
   helper calls. */
#define VOL_U8(addr) (*(volatile u8 *)(addr))
#define VOL_U16(addr) (*(volatile u16 *)(addr))
#define VOL_U32(addr) (*(volatile u32 *)(addr))

/* Writing the low byte of EAX (mov al, X) keeps the upper 24 bits. */
#define LOW8(full, low) (((full) & 0xffffff00u) | (u8)(low))
/* movsx of the low byte of a 32-bit register. */
#define SEXT8(v) ((s32)(s8)(u8)(v))
/* Helper at 0x007bdd70: object first, then the record, then one stack value
   (EAX pushed whole, so the callee reads only its low byte). */
#define PROBE(value) cm_fn_007bdd70(param_2, param_1, (value))

extern u8 CM_CDECL cm_fn_00543f30(u32 object, u32 argument);
extern u32 CM_CDECL cm_fn_0066f4d0(void);
extern u32 CM_CDECL cm_fn_007bdd70(u32 object, u32 record, u32 value);

u8 cm_fn_007bd9d0(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 iVar1;
    u32 dword0;
    u32 eax;
    u32 ecx;
    u32 r;
    u32 edx;
    u8 al;
    u8 cl;
    s32 t;
    s32 iVar5;
    s32 iVar6;
    u32 w;

    iVar1 = VOL_U32(param_2 + 0x39);
    if (iVar1 == 0u) {
        VOL_U16(param_3) = 0;
        VOL_U16(param_4) = 0;
        return 0;
    }
    VOL_U16(param_3) = 0;
    VOL_U16(param_4) = 0;
    r = cm_fn_0066f4d0();
    if (r == 4u) {
        return 1;
    }
    r = cm_fn_0066f4d0();
    if (r == 5u) {
        return 1;
    }

    al = cm_fn_00543f30(param_2, 5u);
    cl = (u8)(0x1eu - (u32)al);
    t = SEXT8(cl);
    dword0 = VOL_U32(param_1);
    if (t < ((s32)dword0 % 30)) {
        cl = 0u;
    } else {
        w = (u32)t & 0x80000007u;
        if ((s32)w < 0) {
            w = ((w - 1u) | 0xfffffff8u) + 1u;
        }
        cl = (u8)w;
        cl = (u8)(cl + (u8)((s32)dword0 % 7));
        cl = (u8)(cl - 3u);
        if ((cl & 1u) != 0u) {
            cl = (u8)(0u - (u32)cl);
        }
    }

    eax = VOL_U32(param_1 + 0x61);
    if (eax == 0u) {
        return 0;
    }

    eax = LOW8(eax, VOL_U8(eax + 0x2f));
    iVar5 = SEXT8(cl);
    ecx = (u32)SEXT8(eax);
    /* The original stores iVar5 into the argument-2 stack slot and re-reads
       it below; that slot is kept here as iVar5. */
    iVar6 = iVar5 + 0xe;
    if ((s32)ecx > iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x01u;
        }
    }

    edx = VOL_U32(param_1 + 0x61);
    eax = LOW8(eax, VOL_U8(edx + 0x21));
    if (SEXT8(eax) > iVar5 + 0xf) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3 + 1) |= 0x10u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x59));
    if (SEXT8(eax) > iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x02u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x5a));
    if (SEXT8(eax) > iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x04u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x58));
    ecx = (u32)SEXT8(eax);
    if ((s32)ecx > iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x08u;
        }
    }

    edx = VOL_U32(param_1 + 0x61);
    eax = (u32)SEXT8(VOL_U8(edx + 0x44));
    if ((s32)eax > iVar6) {
        /* mov cl, [param_1+0x58] keeps the ECX value left by the previous
           movsx, so the pushed value is ECX with its low byte replaced. */
        ecx = LOW8(ecx, VOL_U8(param_1 + 0x58));
        r = cm_fn_007bdd70(param_2, param_1, ecx);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x10u;
        }
    }

    if (SEXT8(VOL_U8(param_1 + 0x5b)) > iVar6) {
        eax = LOW8(eax, VOL_U8(param_1 + 0x58));
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x20u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x5d));
    ecx = (u32)SEXT8(eax);
    if ((s32)ecx > iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x40u;
        }
    }

    edx = VOL_U32(param_1 + 0x61);
    eax = LOW8(eax, VOL_U8(edx + 0x20));
    ecx = (u32)SEXT8(eax);
    if ((s32)ecx > iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_3) |= 0x80u;
        }
    }

    eax = VOL_U32(param_1 + 0x61);
    if (VOL_U8(eax + 0x0f) != 0x14u) {
        eax = LOW8(eax, VOL_U8(eax + 0x22));
        edx = (u32)SEXT8(eax);
        if ((s32)edx > iVar6) {
            r = PROBE(eax);
            eax = r;
            if ((u8)r != 0u) {
                VOL_U8(param_3 + 1) |= 0x01u;
            }
        }
    }

    eax = VOL_U32(param_1 + 0x61);
    if (VOL_U8(eax + 0x0f) != 0x14u) {
        eax = LOW8(eax, VOL_U8(eax + 0x29));
        ecx = (u32)SEXT8(eax);
        if ((s32)ecx > iVar6) {
            r = PROBE(eax);
            eax = r;
            if ((u8)r != 0u) {
                VOL_U8(param_3 + 1) |= 0x04u;
            }
        }
    }

    eax = VOL_U32(param_1 + 0x61);
    if (VOL_U8(eax + 0x0f) != 0x14u) {
        eax = LOW8(eax, VOL_U8(eax + 0x38));
        edx = (u32)SEXT8(eax);
        if ((s32)edx > iVar6) {
            r = PROBE(eax);
            eax = r;
            if ((u8)r != 0u) {
                VOL_U8(param_3 + 1) |= 0x02u;
            }
        }
    }

    eax = VOL_U32(param_1);
    edx = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    ecx = eax + eax * 4u;
    ecx = ecx << 4;
    ecx = ecx - eax;
    if ((VOL_U8(ecx + edx + 0x0b) & 0x40u) == 0u) {
        VOL_U16(param_4) = 0;
        return 1;
    }

    ecx = iVar1;
    if ((s16)VOL_U16(ecx + 0x80) < (s16)0x128e) {
        eax = LOW8(eax, VOL_U8(param_1 + 0x59));
        ecx = (u32)SEXT8(eax);
        if ((s32)ecx < iVar5 + 6) {
            r = PROBE(eax);
            eax = r;
            if ((u8)r != 0u) {
                VOL_U8(param_4) |= 0x02u;
            }
        }
    }

    edx = VOL_U32(param_1 + 0x61);
    eax = LOW8(eax, VOL_U8(edx + 0x21));
    iVar6 = iVar5 + 6;
    if (SEXT8(eax) < iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_4 + 1) |= 0x08u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x5a));
    ecx = (u32)SEXT8(eax);
    if ((s32)ecx < iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_4) |= 0x04u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x58));
    edx = (u32)SEXT8(eax);
    if ((s32)edx < iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_4) |= 0x08u;
        }
    }

    eax = VOL_U32(param_1 + 0x61);
    eax = LOW8(eax, VOL_U8(eax + 0x44));
    ecx = (u32)SEXT8(eax);
    if ((s32)ecx < iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_4) |= 0x10u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x5b));
    edx = (u32)SEXT8(eax);
    if ((s32)edx < iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_4) |= 0x20u;
        }
    }

    eax = LOW8(eax, VOL_U8(param_1 + 0x5d));
    ecx = (u32)SEXT8(eax);
    if ((s32)ecx < iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_4) |= 0x40u;
        }
    }

    edx = VOL_U32(param_1 + 0x61);
    eax = LOW8(eax, VOL_U8(edx + 0x20));
    ecx = (u32)SEXT8(eax);
    if ((s32)ecx < iVar6) {
        r = PROBE(eax);
        eax = r;
        if ((u8)r != 0u) {
            VOL_U8(param_4) |= 0x80u;
        }
    }

    return 1;
}
