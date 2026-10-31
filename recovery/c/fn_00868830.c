/* Preferred-image VA 0x00868830, image base 0x00400000 (ret 8, ECX = this).
   Stack argument 1 (listing [esp+0x18] at entry) is an object whose fields
   0x1c and 0x20 hold two further objects, and whose byte at 0x3a gates the
   routine. The original passes the address of argument 1's own stack slot to
   0x0048e190 (listing lea [esp+0x1c] resolves to [E+4]), so that slot is used
   as an out-pointer. Stack argument 2 ([E+8]) is read into a register and then
   overwritten with a computed value before the 0x0048e190 call, and that value
   is read back from the slot afterwards; it is modelled as a reassigned
   parameter. Locals [esp+0x10] and [esp+0x14] are out_a and out_b. */
#include "cmre.h"

extern s8 CM_THISCALL cm_fn_0048e190(u32 self, u32 obj, u32 flag, u32 out_ptr);
extern u32 CM_THISCALL cm_fn_0086f5b0(u32 self, u32 obj, u32 flag);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);

#define MEM_U8(addr)  (*(volatile u8 *)(addr))
#define MEM_S8(addr)  (*(volatile s8 *)(addr))
#define MEM_U16(addr) (*(volatile u16 *)(addr))
#define MEM_S16(addr) (*(volatile s16 *)(addr))
#define MEM_U32(addr) (*(volatile u32 *)(addr))

void CM_THISCALL cm_fn_00868830(u32 self, u32 arg1, u32 arg2)
{
    u32 obj1;
    u32 obj2;
    u32 obj_a = 0;
    u32 obj_b = 0;
    u32 out_a = 0;
    u32 out_b = 0;
    s32 v47;
    s32 c43;
    s32 c44;
    s32 c47;
    s32 c48;

    obj1 = arg1;
    if (MEM_U8(obj1 + 0x3a) == 1) {
        return;
    }

    {
        u32 p = MEM_U32(MEM_U32(obj1 + 0x1c) + 0xcf);
        if (p != 0) {
            obj_a = cm_fn_0086f5b0(self, p, 1u);
        }
    }
    {
        u32 q = MEM_U32(MEM_U32(obj1 + 0x20) + 0xcf);
        if (q != 0) {
            obj_b = cm_fn_0086f5b0(self, q, 1u);
        }
    }

    if (obj_a != 0) {
        MEM_U16(obj_a + 0x8e) = (u16)(MEM_U16(obj_a + 0x8e) + 1u);
    }
    if (obj_b != 0) {
        MEM_U16(obj_b + 0x8e) = (u16)(MEM_U16(obj_b + 0x8e) + 1u);
    }

    v47 = MEM_S8(obj1 + 0x47);
    obj2 = arg2;

    if (v47 == -1) {
        s32 v43;
        s32 v44;

        if (obj_a != 0) {
            v43 = MEM_S8(obj1 + 0x43);
            {
                u32 a96 = MEM_U32(obj_a + 0x96);
                u32 a9a = MEM_U32(obj_a + 0x9a);
                MEM_U32(obj_a + 0x96) = a96 + (u32)v43;
                MEM_U32(obj_a + 0x9a) = a9a + (u32)(s32)MEM_S8(obj1 + 0x44);
            }
        }
        if (obj_b != 0) {
            v44 = MEM_S8(obj1 + 0x44);
            {
                u32 b96 = MEM_U32(obj_b + 0x96);
                u32 b9a = MEM_U32(obj_b + 0x9a);
                MEM_U32(obj_b + 0x96) = b96 + (u32)v44;
                MEM_U32(obj_b + 0x9a) = b9a + (u32)(s32)MEM_S8(obj1 + 0x43);
            }
        }

        c43 = MEM_S8(obj1 + 0x43);
        c44 = MEM_S8(obj1 + 0x44);
        if (c43 > c44) {
            out_a = 900u;
            if (obj_a != 0) {
                MEM_U16(obj_a + 0x90) = (u16)(MEM_U16(obj_a + 0x90) + 1u);
            }
            if (obj_b != 0) {
                MEM_U16(obj_b + 0x94) = (u16)(MEM_U16(obj_b + 0x94) + 1u);
            }
        } else if (c43 < c44) {
            out_b = 900u;
            if (obj_b != 0) {
                MEM_U16(obj_b + 0x90) = (u16)(MEM_U16(obj_b + 0x90) + 1u);
            }
            if (obj_a != 0) {
                MEM_U16(obj_a + 0x94) = (u16)(MEM_U16(obj_a + 0x94) + 1u);
            }
        } else {
            out_a = 300u;
            out_b = 300u;
            if (obj_a != 0) {
                MEM_U16(obj_a + 0x92) = (u16)(MEM_U16(obj_a + 0x92) + 1u);
            }
            if (obj_b != 0) {
                MEM_U16(obj_b + 0x92) = (u16)(MEM_U16(obj_b + 0x92) + 1u);
            }
        }
    } else {
        if (obj_a != 0) {
            u32 a96 = MEM_U32(obj_a + 0x96);
            u32 a9a = MEM_U32(obj_a + 0x9a);
            MEM_U32(obj_a + 0x96) = a96 + (u32)v47;
            MEM_U32(obj_a + 0x9a) = a9a + (u32)(s32)MEM_S8(obj1 + 0x48);
        }
        if (obj_b != 0) {
            s32 v48 = MEM_S8(obj1 + 0x48);
            u32 b96 = MEM_U32(obj_b + 0x96);
            u32 b9a = MEM_U32(obj_b + 0x9a);
            MEM_U32(obj_b + 0x96) = b96 + (u32)v48;
            MEM_U32(obj_b + 0x9a) = b9a + (u32)(s32)MEM_S8(obj1 + 0x47);
        }

        c47 = MEM_S8(obj1 + 0x47);
        c48 = MEM_S8(obj1 + 0x48);
        if (c47 > c48) {
            out_a = 900u;
            if (obj_a != 0) {
                MEM_U16(obj_a + 0x90) = (u16)(MEM_U16(obj_a + 0x90) + 1u);
            }
            if (obj_b != 0) {
                MEM_U16(obj_b + 0x94) = (u16)(MEM_U16(obj_b + 0x94) + 1u);
            }
        } else if (c47 < c48) {
            out_b = 900u;
            if (obj_b != 0) {
                MEM_U16(obj_b + 0x90) = (u16)(MEM_U16(obj_b + 0x90) + 1u);
            }
            if (obj_a != 0) {
                MEM_U16(obj_a + 0x94) = (u16)(MEM_U16(obj_a + 0x94) + 1u);
            }
        } else {
            out_a = 300u;
            out_b = 300u;
            if (obj_a != 0) {
                MEM_U16(obj_a + 0x92) = (u16)(MEM_U16(obj_a + 0x92) + 1u);
            }
            if (obj_b != 0) {
                MEM_U16(obj_b + 0x92) = (u16)(MEM_U16(obj_b + 0x92) + 1u);
            }
        }
    }

    /* 0x00868a20: first object's post-processing. */
    if (obj_a != 0) {
        u8 raw128 = MEM_U8(obj2 + 0x128);
        s32 t1 = MEM_S8(obj2 + 0x11f) + (s32)(s8)raw128;
        s32 d = (s8)(u8)(MEM_U8(obj2 + 0x127) - raw128);
        s32 s129 = MEM_S8(obj2 + 0x129);
        u32 x = 5u * ((u32)s129 + 2u * ((u32)5 * (u32)t1 + (u32)d));
        u32 idx;
        u32 table;
        u32 this_ptr;
        s8 r1;
        s32 q500;
        s32 s69;
        u32 prod;
        s32 award;
        u32 ca;
        u32 b6;
        u32 p1c;
        u32 v14;

        idx = MEM_U32(obj1);
        arg2 = x;
        table = CM_GLOBAL(volatile u32, 0x00adadfc);
        this_ptr = MEM_U32(table + idx * 4u);
        r1 = cm_fn_0048e190(this_ptr, obj1, 0u, (u32)&arg1);

        q500 = (s32)MEM_S16(MEM_U32(obj1 + 0x20) + 0x80) / 500;
        s69 = MEM_S16(MEM_U32(obj1 + 0x14) + 0x69);
        prod = (u32)(s32)(s8)r1 * (u32)q500 * (u32)s69 * (arg2 + out_a);
        award = (s32)(50u * prod) / 100000;

        ca = MEM_U32(obj_a + 0xca);
        b6 = MEM_U32(obj_a + 0xb6);
        MEM_U32(obj_a + 0xb6) = b6 + (u32)award;
        MEM_U32(obj_a + 0xca) = ca + (u32)award;

        out_a = (u32)award;
        p1c = MEM_U32(obj1 + 0x1c);
        v14 = MEM_U32(obj1 + 0x14);
        if (v14 == MEM_U32(p1c + 0x57) || v14 == MEM_U32(p1c + 0x60)) {
            MEM_U32(obj_a + 0xce) = MEM_U32(obj_a + 0xce) + (u32)award;
        }
        if (cm_fn_0053b900((u32 *)MEM_U32(obj1 + 0x1c)) != 0) {
            MEM_U32(obj_a + 0xbe) = MEM_U32(obj_a + 0xbe) + out_a;
        } else {
            MEM_U32(obj_a + 0xba) = MEM_U32(obj_a + 0xba) + out_a;
        }
    }

    /* 0x00868b2f: second object's post-processing. */
    if (obj_b != 0) {
        u8 raw74d = MEM_U8(obj2 + 0x74d);
        s32 s744 = MEM_S8(obj2 + 0x744);
        s32 t1 = s744 + (s32)(s8)raw74d;
        u8 raw74c = MEM_U8(obj2 + 0x74c);
        s32 d = (s8)(u8)(raw74c - raw74d);
        s32 s74e = MEM_S8(obj2 + 0x74e);
        u32 y2 = 5u * ((u32)s74e + 2u * ((u32)5 * (u32)t1 + (u32)d));
        u32 idx;
        u32 table;
        u32 this_ptr;
        s8 r2;
        s32 q500b;
        s32 s69;
        u32 prod;
        s32 award;
        u32 ebx_ca;
        u32 b6;
        u32 p1c;
        u32 p20;
        u32 v14;

        idx = MEM_U32(obj1);
        table = CM_GLOBAL(volatile u32, 0x00adadfc);
        this_ptr = MEM_U32(table + idx * 4u);
        r2 = cm_fn_0048e190(this_ptr, obj1, 1u, (u32)&arg1);

        p1c = MEM_U32(obj1 + 0x1c);
        ebx_ca = MEM_U32(obj_b + 0xca);
        q500b = (s32)MEM_S16(p1c + 0x80) / 500;
        s69 = MEM_S16(MEM_U32(obj1 + 0x14) + 0x69);
        prod = (u32)(s32)(s8)r2 * (u32)q500b * (u32)s69 * (y2 + out_b);
        award = (s32)(50u * prod) / 100000;

        b6 = MEM_U32(obj_b + 0xb6);
        MEM_U32(obj_b + 0xb6) = b6 + (u32)award;
        MEM_U32(obj_b + 0xca) = ebx_ca + (u32)award;

        p20 = MEM_U32(obj1 + 0x20);
        v14 = MEM_U32(obj1 + 0x14);
        if (v14 == MEM_U32(p20 + 0x57) || v14 == MEM_U32(p20 + 0x60)) {
            MEM_U32(obj_b + 0xce) = MEM_U32(obj_b + 0xce) + (u32)award;
        }
        if (cm_fn_0053b900((u32 *)MEM_U32(obj1 + 0x20)) != 0) {
            MEM_U32(obj_b + 0xbe) = MEM_U32(obj_b + 0xbe) + out_a;
        } else {
            MEM_U32(obj_b + 0xba) = MEM_U32(obj_b + 0xba) + out_a;
        }
    }
}
