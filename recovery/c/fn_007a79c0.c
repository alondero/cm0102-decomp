/* Recovered from the 3.9.68 listing at VA 0x007a79c0 (image base 0x00400000),
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Two stack arguments, callee pops 8 bytes (stdcall), result is the low byte
   of the flag byte at [esp+0x13], initialised to 0 at 0x007a79c8. */
#include "cmre.h"

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
#define F(a) (*(volatile double *)(u32)(a))

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 *output, u32 *source);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 low, u32 high);
extern u8 CM_CDECL cm_fn_00669340(u32 value);
extern u8 CM_CDECL cm_fn_00669360(u32 club);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

u8 CM_STDCALL cm_fn_007a79c0(u32 *param_1, u32 *param_2)
{
    u8 result;
    s8 cVar3, cVar6, thresh;
    u32 self, v, obj, p39, head, q;
    u16 d1lo, d1hi, sum;
    s16 sft;
    u8 idx;
    u32 local[2];
    u32 p2d, pb, pe;
    u16 ehi, bhi;
    s32 r;
    long double t;

    result = 0;

    if (D((u32)param_1 + 0x39) == 0)
        return 0;

    self = G(0x00ae1050);
    v = cm_fn_004cf5a0(self, (u32)param_1);
    G(0x00ae1080) = v;
    if (v == 0) {
        obj = 0;
    } else {
        obj = G(0x00ae23b4) + D(v + 4) * 0x245u;
    }

    obj = cm_fn_004cf5f0(G(0x00ae1050), (u32)param_1, obj);
    if (obj == 0) {
        cVar6 = (s8)-100;              /* 0x007a7a1d: mov byte [esp+0x14],0x9c */
    } else {
        cVar6 = (s8)B(obj + 0x3d);
    }

    cVar3 = (s8)B((u32)param_1 + 0x18);
    obj = D((u32)param_1 + 0x61);

    if (obj == 0) {
        if (D((u32)param_1 + 0x69) != 0) {
            thresh = (s8)B((u32)param_1 + 0x3d);
            if (thresh == 0) {
                thresh = (s8)B((u32)param_1 + 0x28);
                if (thresh == 0)
                    thresh = (s8)(u8)(u32)param_2;
            }
            switch (thresh) {
            case 1:
            case 2:
            case 4:                    /* 0x007a7a87 */
                cVar6 = (s8)(u8)((u8)cm_fn_0090cfc0(10) + 0x46);
                break;
            case 8:
            case 9:                    /* 0x007a7a71 */
                cVar6 = (s8)(u8)((u8)cm_fn_0090cfc0(5) + 0x3c);
                break;
            default:                   /* 0x007a7a9d */
                if (D((u32)param_1 + 0x39) == 0 &&
                    D((u32)param_1 + 0x24) == 0)
                    cVar6 = (s8)(u8)((u8)cm_fn_0090cfc0(10) + 0x37);
                else
                    cVar6 = (s8)(u8)((u8)cm_fn_0090cfc0(5) + 0x3d);
                break;
            }
            if (cVar3 <= cVar6)
                return 0;
        }
        B((u32)param_2 + 0x18) = 0x20; /* 0x007a7abf */
        result = 1;                    /* 0x007a7ac7 */
        goto acc;
    }

    /* 0x007a7afb: object/form present */
    thresh = (s8)B(obj + 0xf);
    if (thresh < 0x0f) {
        if ((s8)B(obj + 0x15) >= 0x0f) {
            thresh = 0x1e;
        } else {
            if ((s8)B(obj + 0x14) < 0x0f)
                thresh = 0x1f;
            else
                thresh = 0x1e;
        }
    } else {
        thresh = 0x22;
    }
    if (cVar3 <= thresh)
        return result;                 /* 0x007a7b1a -> 0x007a7e87 */

    /* 0x007a7b20: x87 sequence, fild/fisub/fmul 0x00967880/fsubr 0x00967890
       then __ftol at 0x00945640; result low word sign-extended. */
    t = (long double)(s32)cVar3 - (long double)(s32)thresh;
    t = t * (long double)F(0x00967880);
    t = (long double)F(0x00967890) - t;
    sft = (s16)(long long)t;
    t = (long double)(s32)sft * (long double)(s32)(s16)W(obj + 0xb);
    sft = (s16)(long long)t;

    if (cVar3 > 0x2d) {
        B((u32)param_2 + 0x18) = 1;    /* 0x007a7b68 */
        result = 1;
        goto acc;
    }

    /* 0x007a7b7a */
    p39 = D((u32)param_1 + 0x39);
    if (p39 != 0 && D(p39 + 0x57) != 0)
        goto L_be6;
    if (p39 == 0) {
        u32 p1a = D((u32)param_1 + 0x1a);
        if (p1a == 0 || B(p1a + 0x11c) != 0)
            goto L_be6;
    }

    /* 0x007a7b9d */
    if (cm_fn_0090cfc0(3) != 0) {
        B((u32)param_2 + 0x18) = 1;
        result = 1;
        goto acc;
    }
    if (D((u32)param_1 + 0x39) == 0)   /* 0x007a7bbd */
        return result;
    if (cVar6 >= 0)
        return result;
    B((u32)param_2 + 0x18) = 1;
    result = 1;
    goto acc;

L_be6:
    if (sft >= 0x1388)                 /* 0x007a7be6 */
        return result;
    if ((s16)W(obj + 5) > 0x50)        /* 0x007a7bff */
        return result;
    if ((s16)W(obj + 5) < 10) {        /* 0x007a7c1c */
        B((u32)param_2 + 0x18) = 1;
        result = 1;
        goto acc;
    }
    if (sft < 0x5dc) {                 /* 0x007a7c34 */
        B((u32)param_2 + 0x18) = 1;
        result = 1;
        goto acc;
    }
    if (p39 != 0) {                    /* 0x007a7c4c */
        if (cVar6 < 0) {
            B((u32)param_2 + 0x18) = 1;
            result = 1;
            goto acc;
        }
        return result;                 /* 0x007a7c6c */
    }
    r = (s32)(s8)B(obj + 0x3c) * 25;   /* 0x007a7c72 */
    if (cm_fn_0090cfc0((u32)r) != 0)
        return result;
    B((u32)param_2 + 0x18) = 2;
    result = 1;
    goto acc;

acc:                                   /* 0x007a7acc */
    head = D((u32)param_1);
    B((u32)param_2 + 0x19) = 0;
    D((u32)param_2) = head;
    q = D((u32)param_1 + 0x39);
    if (q == 0) {
        D((u32)param_2 + 4) = 0xffffffffu;
    } else {
        D((u32)param_2 + 4) = D(q);
    }

    q = D((u32)param_1 + 0x39);        /* 0x007a7ca1 */
    if (q == 0) {
        if (D((u32)param_1 + 0x1a) == 0) {
            /* 0x007a7caf */
            W((u32)param_2 + 8) = (u16)(W(0x00ae2c90) + 1);
            W((u32)param_2 + 0xa) = W(0x00ae2c92);
            W((u32)param_2 + 0x10) = (u16)(W(0x00ae2c90) + 1);
            W((u32)param_2 + 0x12) = W(0x00ae2c92);
            return result;
        }
        idx = cm_fn_00669340(D((u32)param_1 + 0x1a));
    } else {
        idx = cm_fn_00669360(q);
    }

    if ((s8)idx < 0 || (s8)idx >= 0x23)
        goto L_e5c;

    /* 0x007a7d23 */
    {
        u16 rv = (u16)cm_fn_0090cfc0(0x5f);
        u16 lowv = (u16)(rv + W(0x00ae2c90) + 1);
        if ((s16)lowv > 0x16d) {
            d1lo = (u16)cm_fn_0090cfc0(0x3c);
            d1hi = (u16)(W(0x00ae2c92) + 1);
        } else {
            d1lo = lowv;
            d1hi = W(0x00ae2c92);
        }
        W((u32)param_2 + 0xa) = d1hi;
        W((u32)param_2 + 8) = d1lo;
    }

    /* 0x007a7d68 */
    {
        u16 rv = (u16)cm_fn_0090cfc0(0x1e);
        sum = (u16)(rv + W(0x00b63d75 + (u32)(s32)(s8)idx * 72));
    }
    W((u32)param_2 + 0x10) = sum;
    if ((s16)sum > 0x16d) {
        W((u32)param_2 + 0x10) = (u16)cm_fn_0090cfc0(0x1e);
        if (B(0x00b63d7d + (u32)(s32)(s8)idx * 72) != 0)
            W((u32)param_2 + 0x12) = (u16)(W(0x00ae2c92) + 2);
        else
            W((u32)param_2 + 0x12) = (u16)(W(0x00ae2c92) + 1);
    } else {
        if (B(0x00b63d7d + (u32)(s32)(s8)idx * 72) != 0)
            W((u32)param_2 + 0x12) = (u16)(W(0x00ae2c92) + 1);
        else
            W((u32)param_2 + 0x12) = W(0x00ae2c92);
    }

    /* 0x007a7de9: copy date2 (param_2+0x10, 8 bytes) and compare with date1 */
    p2d = (u32)param_2;
    pb = p2d + 8;
    pe = p2d + 0x10;
    local[0] = D(pe);
    local[1] = D(pe + 4);
    bhi = W(pb + 2);
    ehi = (u16)(local[0] >> 16);
    if (ehi >= bhi) {
        if (ehi > bhi)
            goto L_e2e;
        if ((s16)(u16)local[0] >= (s16)W(pb))
            goto L_e2e;
    }
    cm_fn_00417c10(pe, local, (u32 *)pb);   /* 0x007a7e1b */
    return result;

L_e2e:                                  /* 0x007a7e2e */
    r = cm_fn_0054cdb0(pe, D(pb), D(pb + 4));
    if (r >= 0x2d)
        return result;
    cm_fn_00417c10(pb, local, (u32 *)pe);   /* 0x007a7e49 */
    return result;

L_e5c:                                  /* 0x007a7e5c */
    W((u32)param_2 + 8) = (u16)(W(0x00ae2c90) + 1);
    W((u32)param_2 + 0xa) = W(0x00ae2c92);
    W((u32)param_2 + 0x10) = (u16)(W(0x00ae2c90) + 1);
    W((u32)param_2 + 0x12) = W(0x00ae2c92);
    return result;
}
