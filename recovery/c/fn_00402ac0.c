/* Tier-2 C recovery for the 3.9.68 primary executable, preferred VA 0x00402ac0.
   Thiscall: ECX is the object; the callee pops 0x38 bytes of stack arguments.
   Parameters are named by their offset from the first stack argument
   (a00 = [esp+4] at entry, a34 = the signed word tested against -1). */
#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX. */
#define OBJ_U8(off) (*(volatile u8 *)(self + (off)))
#define OBJ_U16(off) (*(volatile u16 *)(self + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern void CM_CDECL cm_fn_0090ba90(void *arg1, void *arg2, u32 count);

void CM_THISCALL cm_fn_00402ac0(u32 self, u32 a00, u32 a04, u16 a08, u32 a0c,
                                u32 a10, u32 a14, u32 a18, u8 a1c, u32 a20,
                                u8 a24, u32 a28, u32 a2c, u16 a30, s16 a34)
{
    u32 vb;
    u32 vs;
    u32 vd;
    u32 eax_v;
    u32 ecx_v;
    u32 i;
    u32 k;
    u32 src;
    u32 dst;
    u8 cl;
    int use_copy;

    if (a34 == -1) {
        if ((s32)a0c > (s32)a14) {
            cm_fn_0090ba90(&a0c, &a14, 4u);
        }
        if ((s32)a10 > (s32)a18) {
            cm_fn_0090ba90(&a10, &a18, 4u);
        }
        vb = a0c;
        vs = a10;
        vd = a14;
    } else {
        for (i = 0; i < 7; i++) {
            OBJ_U32(0xbad + i * 4u) = 0x01010101u;
        }
        OBJ_U16(0xbc9) = 0x0101u;
        for (i = 0; i < 7; i++) {
            OBJ_U32(0xbcb + i * 4u) = 0x01010101u;
        }
        OBJ_U16(0xbe7) = 0x0101u;
        a18 = 0u;
        a1c = 1u;
        a20 = 0u;
        a24 = 0u;
        a28 = 0u;
        vb = 0u;
        vs = 0u;
        vd = 0u;
    }

    /* Header fields from the working values. */
    eax_v = ((s32)vb > 0) ? vb : 0u;
    ecx_v = ((s32)vs > 0) ? vs : 0u;
    if ((s32)vd >= 0x31f) {
        vd = 0x31fu;
    }
    if ((s32)a18 >= 0x257) {
        a18 = 0x257u;
    }
    OBJ_U32(0x00) = eax_v;
    OBJ_U32(0x04) = ecx_v;
    OBJ_U32(0x08) = vd;
    OBJ_U32(0x0c) = a18;
    OBJ_U8(0xbac) = a24;
    OBJ_U8(0xbab) = a1c;
    OBJ_U32(0x10) = 0x7fffffffu;
    OBJ_U32(0x14) = 0u;
    for (i = 0; i < 12; i++) {
        OBJ_U32(0xb7a + i * 4u) = 0u;
    }
    OBJ_U8(0xbaa) = 0u;
    OBJ_U16(0x20e) = (u16)a34;
    OBJ_U32(0x204) = a00;
    OBJ_U32(0x208) = a04;
    OBJ_U16(0xb72) = a08;
    OBJ_U16(0x20c) = a30;
    OBJ_U32(0xbed) = 0u;
    OBJ_U32(0x200) = 0u;
    OBJ_U32(0xba6) = 0u;
    OBJ_U16(0x210) = 0xffffu;
    OBJ_U32(0xbe9) = 1u;
    OBJ_U16(0xb76) = 0xffffu;
    OBJ_U32(0x18) = a2c;
    OBJ_U32(0x1fc) = 0xffffffffu;
    if ((a2c & 0x80000u) != 0u) {
        return;
    }

    /* Copy or fill the 0x1e-byte table at 0xbad from the pointer in a20. */
    src = a20;
    use_copy = 0;
    if (src != 0u) {
        cl = OBJ_U8(0xbab);
        if (cl != 0u && cl < 0x1eu) {
            use_copy = 1;
        }
    }
    if (use_copy) {
        dst = self + 0xbad;
        for (k = cl >> 2; k != 0u; k--) {
            *(volatile u32 *)dst = *(volatile u32 *)src;
            dst += 4u;
            src += 4u;
        }
        for (k = cl & 3u; k != 0u; k--) {
            *(volatile u8 *)dst = *(volatile u8 *)src;
            dst += 1u;
            src += 1u;
        }
    } else {
        cl = OBJ_U8(0xbab);
        dst = self + 0xbad;
        for (k = cl >> 2; k != 0u; k--) {
            *(volatile u32 *)dst = 0x01010101u;
            dst += 4u;
        }
        for (k = cl & 3u; k != 0u; k--) {
            *(volatile u8 *)dst = 1u;
            dst += 1u;
        }
    }

    /* Copy or fill the table at 0xbcb from the pointer in a28. */
    src = a28;
    use_copy = 0;
    if (src != 0u) {
        cl = OBJ_U8(0xbac);
        if (cl != 0u && cl < 0x1eu) {
            use_copy = 1;
        }
    }
    if (use_copy) {
        dst = self + 0xbcb;
        for (k = cl >> 2; k != 0u; k--) {
            *(volatile u32 *)dst = *(volatile u32 *)src;
            dst += 4u;
            src += 4u;
        }
        for (k = cl & 3u; k != 0u; k--) {
            *(volatile u8 *)dst = *(volatile u8 *)src;
            dst += 1u;
            src += 1u;
        }
        return;
    }
    cl = OBJ_U8(0xbac);
    dst = self + 0xbcb;
    for (k = cl >> 2; k != 0u; k--) {
        *(volatile u32 *)dst = 0x01010101u;
        dst += 4u;
    }
    for (k = cl & 3u; k != 0u; k--) {
        *(volatile u8 *)dst = 1u;
        dst += 1u;
    }
}
