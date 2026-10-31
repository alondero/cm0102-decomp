/* Preferred-image VA 0x006C2BE0, image base 0x00400000 (ret 0x10, ECX = this).
   Stack arguments: arg1 = object P (fields 0x19, 0x27, 0x102, 0x103 are read),
   arg2 = byte out-pointer B, arg3 = byte out-pointer C, arg4 is unused.
   The original stores a 0/1 flag ("BL") into the low byte of arg1's own stack
   slot ([esp+0x1c]) and reads it back as a byte later, so arg1 is reassigned
   here. Locals [esp+0x11] = b11, [esp+0x12] = b12, [esp+0x13] = b13 and the
   dword [esp+0x14] = idx are scratch values. */
#include "cmre.h"

extern u8 CM_FASTCALL cm_fn_00714d30(u32 obj);
extern u8 CM_THISCALL cm_fn_00716270(u8 *self);
extern u32 CM_THISCALL cm_fn_00716310(u32 self);
extern u16 CM_FASTCALL cm_fn_00717410(u32 obj);

#define MEM_U8(a)  (*(volatile u8 *)(a))
#define MEM_S8(a)  (*(volatile s8 *)(a))
#define MEM_U32(a) (*(volatile u32 *)(a))

/* 0x006c2e54 / 0x006c2ec0 / 0x006c2ec8: byte at arg2 clamped to [0, 8]. */
#define CLAMP_B_0_8()                                  \
    do {                                               \
        if (MEM_S8(arg2) < 0) {                        \
            MEM_U8(arg2) = 0;                          \
        }                                              \
        if (MEM_S8(arg2) >= 9) {                       \
            MEM_U8(arg2) = 8;                          \
        }                                              \
    } while (0)

void CM_THISCALL cm_fn_006c2be0(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u32 obj_p;
    u8 r_al;
    u8 b11;
    s8 b12;
    s8 b13;
    u32 idx;
    u8 flag;
    s8 a7;
    s8 a8;
    s8 t_eb1;
    u32 rowp;
    u32 rec;
    u32 rd;
    u8 cval;
    u8 done;

    (void)arg4;

    a7 = MEM_S8(self + 0x8ea7);
    a8 = MEM_S8(self + 0x8ea8);
    b12 = a7;
    b13 = a8;
    if (a7 < 0) {
        b12 = 0;
    } else if (a7 >= 9) {
        b12 = 8;
    }
    if (a8 < 0) {
        b13 = 0;
    } else if (a8 >= 12) {
        b13 = 11;
    }

    obj_p = arg1;
    b11 = (MEM_U8(obj_p + 0x27) == 1) ? 1u : 0u;
    r_al = cm_fn_00714d30(obj_p);

    {
        u8 c8e = MEM_U8(self + 0x8eae);
        s32 edi_v = (s32)(s8)c8e;
        u32 dval = (r_al == 0) ? 1u : 0u;

        flag = 1;
        if (edi_v == (s32)dval) {
            flag = 0;
        } else if (r_al == c8e) {
            /* keep flag */
        } else {
            u32 f57a = MEM_U32(self + 0xf57a);
            if (f57a != 0 && MEM_U8(f57a + 0x27) == r_al) {
                /* keep flag */
            } else {
                s8 a7b = MEM_S8(self + 0x8ea7);
                s8 a8b = MEM_S8(self + 0x8ea8);
                if (r_al == 0) {
                    if (a7b < 2 || a7b > 6 || a8b < 10 || a8b > 11) {
                        flag = 0;
                    }
                } else {
                    if (a7b < 2 || a7b > 6 || a8b < 0 || a8b > 1) {
                        flag = 0;
                    }
                }
                if (flag != 0 && MEM_S8(self + 0x8ea9) == -1) {
                    flag = 0;
                }
            }
        }
    }

    t_eb1 = MEM_S8(self + 0x8eb1);
    /* mov [esp+0x1c], bl: the low byte of arg1's stack slot becomes the flag. */
    arg1 = (arg1 & 0xffffff00u) | (u32)flag;

    if (t_eb1 > 0) {
        done = 0;
        if (flag != 0) {
            u8 r = cm_fn_00716270((u8 *)obj_p);
            if (r != 0) {
                u8 vb = MEM_U8(obj_p + 0x102);
                MEM_U8(arg2) = vb;
                u8 vc = MEM_U8(obj_p + 0x103);
                MEM_U8(arg3) = vc;
                done = 1;
            }
        } else {
            u8 r2 = (u8)cm_fn_00716310(obj_p);
            if (r2 != 0) {
                u8 vb = MEM_U8(obj_p + 0x102);
                MEM_U8(arg2) = vb;
                u8 vc = MEM_U8(obj_p + 0x103);
                MEM_U8(arg3) = vc;
                done = 1;
            }
        }

        if (done == 0) {
            /* 0x006c2d03: table lookups through the row chosen by b11. */
            rowp = self + 4u * (u32)(s32)(s8)b11 + 0xf56eu;
            {
                s32 p19 = MEM_S8(obj_p + 0x19);
                s32 x1 = (s32)(s8)flag + 2 * p19;
                s32 x2 = (s32)b12 + 8 * x1;
                s32 x3 = x1 + x2;
                u32 t0 = MEM_U32(rowp);
                u32 addr = t0 + 12u * (u32)x3;
                MEM_U8(arg2) = MEM_U8(addr + (u32)(s32)b13) & 0xf;
            }
            {
                s32 p19b = MEM_S8(obj_p + 0x19);
                s32 x1b = (s32)(s8)flag + 2 * p19b;
                s32 x2b = (s32)b12 + 8 * x1b;
                s32 x3b = x1b + x2b;
                u32 t0b = MEM_U32(rowp);
                u32 addr2 = t0b + 12u * (u32)x3b;
                MEM_U8(arg3) = (u8)(MEM_U8(addr2 + (u32)(s32)b13) >> 4);
            }
        }

        /* 0x006c2d5d: clamp C to [0, 11], B to [0, 8]. */
        if (MEM_S8(arg3) < 0) {
            MEM_U8(arg3) = 0;
        }
        if (MEM_S8(arg3) >= 12) {
            MEM_U8(arg3) = 11;
        }
        if (MEM_S8(arg2) < 0) {
            MEM_U8(arg2) = 0;
        }
        if (MEM_S8(arg2) >= 9) {
            MEM_U8(arg2) = 8;
        }
        return;
    }

    /* 0x006c2d8b: t_eb1 <= 0. */
    idx = (u32)(s32)(s8)b11;
    {
        s32 p19 = MEM_S8(obj_p + 0x19);
        s32 x1 = (s32)(s8)flag + 2 * p19;
        s32 x2 = (s32)b12 + 8 * x1;
        s32 x3 = x1 + x2;
        u32 t0 = MEM_U32(self + 0xf56eu + idx * 4u);
        u32 addr = t0 + 12u * (u32)x3;
        MEM_U8(arg2) = MEM_U8(addr + (u32)(s32)b13) & 0xf;
    }
    {
        s32 p19 = MEM_S8(obj_p + 0x19);
        s32 x1 = (s32)(s8)flag + 2 * p19;
        s32 x2 = (s32)b12 + 8 * x1;
        s32 x3 = x1 + x2;
        u32 t1 = MEM_U32(self + 0xf56eu + idx * 4u);
        u32 addr2 = t1 + 12u * (u32)x3;
        cval = (u8)(MEM_U8(addr2 + (u32)(s32)b13) >> 4);
        MEM_U8(arg3) = cval;
    }
    {
        u16 r_ret = cm_fn_00717410(obj_p);
        if (r_ret == 1) {
            return;
        }
    }

    rec = self + 0x9766u + 6371u * idx;
    rd = MEM_U32(rec);
    if ((rd & 0x1000u) != 0 && MEM_U8(self + idx + 6u) == 0) {
        /* 0x006c2e33: C steps down when b11 == 0, up otherwise. */
        u8 dlv = MEM_U8(arg3);
        if (b11 == 0) {
            dlv = (u8)(dlv - 1u);
        } else {
            dlv = (u8)(dlv + 1u);
        }
        MEM_U8(arg3) = dlv;
        if ((s8)dlv < 0) {
            MEM_U8(arg3) = 0;
        }
        if (MEM_S8(arg3) >= 12) {
            MEM_U8(arg3) = 11;
        }
        CLAMP_B_0_8();
    } else if ((rd & 0x10000u) != 0 && flag != 0) {
        /* 0x006c2e62: C steps up when b11 == 0, down otherwise. */
        u8 dlv = MEM_U8(arg3);
        if (b11 == 0) {
            dlv = (u8)(dlv + 1u);
        } else {
            dlv = (u8)(dlv - 1u);
        }
        MEM_U8(arg3) = dlv;
        if ((s8)dlv < 0) {
            MEM_U8(arg3) = 0;
        }
        if (MEM_S8(arg3) >= 12) {
            MEM_U8(arg3) = 11;
        }
        CLAMP_B_0_8();
    } else if ((rd & 0x8000u) != 0 && flag == 0) {
        /* 0x006c2e92: C steps down when b11 == 0, up otherwise. */
        u8 dlv = MEM_U8(arg3);
        if (b11 == 0) {
            dlv = (u8)(dlv - 1u);
        } else {
            dlv = (u8)(dlv + 1u);
        }
        MEM_U8(arg3) = dlv;
        if ((s8)dlv < 0) {
            MEM_U8(arg3) = 0;
        }
        if (MEM_S8(arg3) >= 12) {
            MEM_U8(arg3) = 11;
        }
        CLAMP_B_0_8();
    }
    /* Otherwise (0x006c2ed2) neither C nor B is adjusted here. */

    /* 0x006c2ed6 */
    rd = MEM_U32(rec);
    if ((rd & 0x4000u) == 0) {
        return;
    }

    if (flag != 0 && MEM_U8(self + idx + 6u) == 0) {
        /* 0x006c2ef1 */
        if (b11 != 0) {
            u8 dv = (u8)(MEM_U8(arg3) + 2u);
            MEM_U8(arg3) = dv;
            if ((s8)dv > 11) {
                MEM_U8(arg3) = 11;
            }
        } else {
            u8 dv = (u8)(MEM_U8(arg3) - 2u);
            MEM_U8(arg3) = dv;
            if ((s8)dv < 0) {
                MEM_U8(arg3) = 0;
            }
        }
        return;
    }

    /* 0x006c2f31 */
    {
        s32 a8s = MEM_S8(self + 0x8ea8);
        if (b11 != 0) {
            s32 cv = MEM_S8(arg3);
            s32 e = a8s + 2;
            s8 v8;
            s32 cnt;
            s32 w;
            if (e <= cv) {
                e = cv;
            }
            v8 = (s8)(u8)e;
            if (v8 < 9) {
                v8 = 9;
            }
            MEM_U8(arg3) = (u8)v8;
            cnt = (s32)MEM_S8(self + 0x8ea8) + 4;
            w = (cnt < (s32)v8) ? cnt : (s32)v8;
            if (w > 11) {
                w = 11;
            }
            MEM_U8(arg3) = (u8)(s8)w;
        } else {
            s32 cv = MEM_S8(arg3);
            s32 e = a8s - 2;
            s32 v_s;
            s32 e2;
            s8 v8;
            if (e >= cv) {
                e = cv;
            }
            v8 = (s8)(u8)e;
            if (v8 > 2) {
                v8 = 2;
            }
            MEM_U8(arg3) = (u8)v8;
            v_s = (s32)v8;
            e2 = (s32)MEM_S8(self + 0x8ea8) - 4;
            if (e2 <= v_s) {
                e2 = v_s;
            }
            MEM_U8(arg3) = (u8)(s8)e2;
            if ((s8)(u8)e2 < 0) {
                MEM_U8(arg3) = 0;
            }
        }
    }
}
