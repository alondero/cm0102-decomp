#include "cmre.h"

/* Field offsets. Object (first argument, ebp): +0x1a pointer compared with
   record+0x53; +0x61 pointer to a record R. R+5, R+7, R+0xb: signed words.
   Record (second argument, edi): +0x00 dword key; +0x53 pointer; +0x57 pointer
   X whose first dword indexes the table at 0x00adadfc; +0x7f signed byte;
   +0x80 signed word; +0xcf pointer Q whose +0x69 holds pointer Z, where Z+0x18
   is an unsigned byte and Z+0x22 a signed byte. Third argument: signed byte. */

#define RD_U32(a) (*(volatile u32 *)(u32)(a))
#define RD_U8(a) (*(volatile u8 *)(u32)(a))
#define RD_S8(a) ((s8)RD_U8(a))
#define RD_S16(a) ((s16)*(volatile u16 *)(u32)(a))

extern int CM_CDECL cm_fn_0090cfc0(int param_1);
extern u32 CM_CDECL cm_fn_00600a50(int param_1, int param_2, char param_3, int *param_4, int param_5);
extern s8 CM_CDECL cm_fn_00544410(u32 arg1, u32 arg2);

int CM_STDCALL cm_fn_007ad220(u32 arg1, u32 arg2, s8 arg3)
{
    u32 obj = arg1;
    u32 ent = arg2;
    u32 rec;
    u32 d;
    u32 q;
    u32 x;
    u32 kx;
    u32 g;
    u32 piv;
    u32 o53;
    u32 p0;
    u32 e45c;
    u32 ebpv;
    u32 v;
    s16 score;
    s32 cxv;
    s32 cx2;
    s32 sg;
    s32 h;
    s32 t1;
    s32 t2;
    s32 m7;
    s32 m5;
    s32 abs1;
    s32 abs2;
    s32 abs3;
    s32 c;
    s32 c1;
    s32 c2;
    s32 sum;

    rec = RD_U32(obj + 0x61);
    cxv = RD_S16(ent + 0x80);
    abs1 = RD_S16(rec + 0xb) - cxv;
    abs1 = abs1 < 0 ? -abs1 : abs1;
    m7 = (s16)(RD_S16(rec + 7) * 50);
    abs2 = m7 - cxv;
    abs2 = abs2 < 0 ? -abs2 : abs2;
    m5 = (s16)(RD_S16(rec + 5) * 50);
    abs3 = m5 - cxv;
    abs3 = abs3 < 0 ? -abs3 : abs3;
    score = (s16)(cxv - abs1 - abs2 - abs3 + 0x1388);

    t2 = cm_fn_0090cfc0(0x14);
    rec = RD_U32(obj + 0x61);
    if (RD_S16(rec + 7) > t2 + 100) {
        if (RD_S8(ent + 0x7f) == 0x14) {
            cx2 = RD_S16(ent + 0x80);
            sg = cx2 < 0 ? -1 : 0;
            h = (cx2 - sg) >> 1;
            score = (s16)(score + h + 0x1b58);
        } else {
            t1 = cm_fn_0090cfc0((s32)RD_S8(ent + 0x7f));
            t2 = cm_fn_0090cfc0(0x14);
            if (t1 > t2) {
                cx2 = RD_S16(ent + 0x80);
                sg = cx2 < 0 ? -1 : 0;
                h = (cx2 - sg) >> 1;
                score = (s16)(score + h + 50 * ((s32)RD_S8(ent + 0x7f) + 100));
            }
        }
    }

    rec = RD_U32(obj + 0x61);
    if (RD_S16(rec + 7) <= 0x32) goto L3aa;
    x = RD_U32(ent + 0x57);
    if (x == 0) goto L3aa;
    kx = RD_U32(x);
    if (RD_U32(CM_GLOBAL(u32, 0x00adadfc) + kx * 4u) == 0) goto L3aa;

    t1 = cm_fn_00600a50((s32)ent, 1, 1, 0, 0);
    if (t1 != 0) {
        g = CM_GLOBAL(u32, 0x00ae2d14);
        if (g != 0 && RD_S16(RD_U32(obj + 0x61) + 7) > 0x78) {
            c = RD_S8(g + RD_U32(ent) * 6u + 4u);
            if (c < 0) {
                score = (s16)(score + 0x1f4);
            } else if (c == 0) {
                score = (s16)(score + 0xfa);
            }
        }
    }

    cx2 = RD_S16(ent + 0x80);
    if (cx2 > 0x1676) {
        if (arg3 < 0x28) {
            score = (s16)(score + 0x3e8);
        } else if (arg3 < 0x2d) {
            score = (s16)(score + 0x2ee);
        } else {
            score = (s16)(score + 0xfa);
        }
    } else if (cx2 > 0x109a) {
        if (arg3 < 0x23) {
            score = (s16)(score + 0x2ee);
        } else if (arg3 < 0x28) {
            score = (s16)(score + 0x1f4);
        } else {
            score = (s16)(score + 0xfa);
        }
    } else {
        score = (s16)(score + 0xfa);
    }

L3aa:
    d = RD_U32(ent + 0xcf);
    if (d == 0) goto L48f;
    q = RD_U32(d + 0x69);
    if (q == 0) goto L48f;
    t1 = cm_fn_00600a50((s32)ent, 1, 1, 0, 0);
    if (t1 != 0) goto L48f;

    d = RD_U32(ent + 0xcf);
    q = RD_U32(d + 0x69);
    t2 = RD_S8(q + 0x22) - 10;
    score = (s16)(score + 30 * t2);
    t1 = cm_fn_0090cfc0(0x4b);
    rec = RD_U32(obj + 0x61);
    if (RD_S16(rec + 7) > t1 + 100) {
        d = RD_U32(ent + 0xcf);
        q = RD_U32(d + 0x69);
        c1 = cm_fn_00544410(q, (rec & 0xffffff00u) | RD_U8(q + 0x18));
        score = (s16)(score + 60 * ((s32)c1 - 10));
    }

    /* 0x007ad434: the second call's ECX is the value left by the first call
       when the branch above ran (see the WIP header); otherwise it is rec. */
    d = RD_U32(ent + 0xcf);
    q = RD_U32(d + 0x69);
    c2 = cm_fn_00544410(q, (rec & 0xffffff00u) | RD_U8(q + 0x18));
    d = RD_U32(ent + 0xcf);
    q = RD_U32(d + 0x69);
    sum = (s32)(s8)c2 + RD_S8(q + 0x22) + RD_S8(ent + 0x7f);
    if (sum > 0x28) {
        cx2 = RD_S16(ent + 0x80);
        if (cx2 > 0x109a && RD_S16(RD_U32(obj + 0x61) + 7) > 0x50) {
            if (score < cx2) {
                score = (s16)cx2;
            } else {
                score = (s16)(score + 0xfa);
            }
        }
    }
    goto L4d6;

L48f:
    t1 = cm_fn_00600a50((s32)ent, 1, 1, 0, 0);
    if (t1 == 0) goto L4d6;
    rec = RD_U32(obj + 0x61);
    score = (s16)(score + 0xc8);
    if (RD_S16(rec + 7) <= 0x78) goto L4d6;
    c = RD_S8(ent + 0x7f);
    if (c == 0x14) {
        cx2 = RD_S16(ent + 0x80);
        if (score < cx2) {
            score = (s16)cx2;
        }
    }
    score = (s16)(score + 25 * c);

L4d6:
    piv = RD_U32(obj + 0x1a);
    o53 = RD_U32(ent + 0x53);
    if (piv == o53) {
        score = (s16)(score + 0x3e8);
    }
    p0 = RD_U32(ent);
    ebpv = CM_GLOBAL(u32, 0x009cf510);
    if (p0 == CM_GLOBAL(u32, 0x009d0430) || p0 == CM_GLOBAL(u32, 0x009d0434) ||
        (p0 == CM_GLOBAL(u32, 0x009d043c) && piv != 0)) {
        v = RD_U32(piv);
        if (v == ebpv) {
            score = (s16)(score + 0x3e8);
        } else if (v == CM_GLOBAL(u32, 0x009cf2e4)) {
            score = (s16)(score - 0x2ee);
        }
    }

    e45c = CM_GLOBAL(u32, 0x009cf45c);
    v = RD_U32(o53);
    if (v == e45c) {
        v = RD_U32(piv);
        if (v == ebpv) {
            score = (s16)(score - 0x2ee);
            return cm_fn_0090cfc0((s32)score);
        }
        if (v == CM_GLOBAL(u32, 0x009cf2e4)) {
            score = (s16)(score - 0x2ee);
            return cm_fn_0090cfc0((s32)score);
        }
        if (v == e45c) {
            score = (s16)(score + 0x3e8);
        }
    }
    return cm_fn_0090cfc0((s32)score);
}
