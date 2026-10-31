#include "cmre.h"

/* Field reads by byte offset from an object address, in listing order. */
#define FIELD_U8(base, off) (*(volatile u8 *)((u32)(base) + (u32)(off)))
#define FIELD_U16(base, off) (*(volatile u16 *)((u32)(base) + (u32)(off)))
#define FIELD_U32(base, off) (*(volatile u32 *)((u32)(base) + (u32)(off)))
#define GLOBAL_U32(va) CM_GLOBAL(volatile u32, (va))

extern u8 CM_THISCALL cm_fn_0048db80(volatile u8 *self, volatile u32 *argument);
extern int CM_CDECL cm_fn_004b0870(int param_1);
extern int CM_CDECL cm_fn_0053b3d0(int *param_1);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern u32 CM_STDCALL cm_fn_007e6f20(int *param_1);

u32 cm_fn_006b61e0(u32 param_1)
{
    u32 r;
    u32 t;
    u32 t2;
    u32 tbl;
    u32 rec;
    u32 rec3;
    u32 idx;
    u32 cnt;
    u32 esi_base;
    u32 c;
    u32 q;
    u32 rr;
    u32 rr3;
    u32 x;
    u32 x2;
    u32 xx;
    u32 yy;
    u32 a;
    u32 b;
    u32 g;
    u32 v;
    u32 res;
    u32 edi_v;
    u8 al;
    u16 si;
    u16 bp;

    /* 6b61e1: EDX = [0x9cf668] is read before the object.
       6b61ee..6b61f5: ECX = [arg1 + 0x14], EAX = first dword of that object. */
    g = GLOBAL_U32(0x009cf668);
    r = FIELD_U32(param_1, 0x14);
    t = FIELD_U32(r, 0x00);
    if (t == g) goto L_64b1;
    if (t == GLOBAL_U32(0x009cf684)) goto L_64b1;
    if (t == GLOBAL_U32(0x009cf794)) return 0;
    if (t == GLOBAL_U32(0x009cf798)) return 0;

    /* 6b6217: push ecx (object); mov ecx,[0xae2c74]; call 0x5c6710 (thiscall). */
    if ((u32)cm_fn_005c6710((void *)GLOBAL_U32(0x00ae2c74), r) == 0) goto L_6281;

    /* 6b6227 loop: si = 0 .. 15 over records of 0x6e bytes at [0xae23bc]. */
    si = 0;
    do {
        idx = GLOBAL_U32(0x00ae2364) - (u32)(s32)(s16)si - 1u;
        rec = GLOBAL_U32(0x00ae23bc) + idx * 110u;
        if (FIELD_U8(rec, 0x5f) == 1u) {
            x = FIELD_U32(rec, 0x39);
            if (x != 0) {
                /* 6b6257: edx = [arg1+0x14]; ecx = [0xadadfc]; push x;
                   eax = [edx]; ecx = [ecx + eax*4]; call 0x48db80 (thiscall). */
                rr = FIELD_U32(param_1, 0x14);
                tbl = GLOBAL_U32(0x00adadfc);
                t = FIELD_U32(rr, 0x00);
                c = FIELD_U32(tbl, t * 4u);
                al = cm_fn_0048db80((volatile u8 *)(u32)c, (volatile u32 *)(u32)x);
                if (al != 0) return 1;
            }
        }
        si = (u16)(si + 1u);
    } while ((s16)si < 0x10);
    return 0;

L_6281:
    /* 6b6281: edx = [arg1+0x14]; ecx = [0x9cf6f8]; eax = [edx]; cmp eax, ecx. */
    rr = FIELD_U32(param_1, 0x14);
    g = GLOBAL_U32(0x009cf6f8);
    t = FIELD_U32(rr, 0x00);
    if (t == g) {
        /* 6b6290: mov cx, word [arg1+0x30] keeps ECX's high word from g. */
        v = (g & 0xffff0000u) | (u32)FIELD_U16(param_1, 0x30);
        /* push ecx; call 0x4b0870 (cdecl); add esp, 4; cmp ax, 0x96. */
        if ((u16)cm_fn_004b0870((int)v) != 0x96) return 0;
    }

    /* 6b62a3: eax = [arg1+0x14]; ecx = [eax+0x5d]. */
    rr = FIELD_U32(param_1, 0x14);
    c = FIELD_U32(rr, 0x5d);
    if (c == 0) goto L_634b;
    if (GLOBAL_U32(0x00b74434) != 0) {
        /* push eax; call 0x7e6f20 (stdcall, pops its argument). */
        if (cm_fn_007e6f20((int *)(u32)rr) != 0) return 0;
    }

    /* 6b62c5: esi = [0xae23bc] and edi = [0xae2364] cached for the loop. */
    esi_base = GLOBAL_U32(0x00ae23bc);
    cnt = GLOBAL_U32(0x00ae2364);
    si = 0;
    do {
        idx = cnt - (u32)(s32)(s16)si - 1u;
        rec = esi_base + idx * 110u;
        if (FIELD_U8(rec, 0x5f) == 1u) {
            xx = FIELD_U32(rec, 0x39);
            if (xx != 0) {
                /* 6b62fb: ebp = [arg1+0x14]; edx = [xx+0x53]; cmp with [ebp+0x5d]. */
                rr = FIELD_U32(param_1, 0x14);
                yy = FIELD_U32(xx, 0x53);
                if (yy == FIELD_U32(rr, 0x5d)) goto L_6318;
            }
        }
        si = (u16)(si + 1u);
    } while ((s16)si < 0x10);
    return 0;

L_6318:
    /* 6b6318: ecx = arg1; edx = [arg1+0x1c]; ecx = [arg1+0x14]. */
    q = FIELD_U32(param_1, 0x1c);
    rr3 = FIELD_U32(param_1, 0x14);
    if (FIELD_U32(q, 0x57) == rr3) goto L_6333;
    if (FIELD_U32(q, 0x60) == rr3) goto L_6333;
    return 1;

L_6333:
    /* 6b6333: eax is still the record address from the loop. */
    x2 = FIELD_U32(rec, 0x39);
    if (FIELD_U32(x2, 0x57) == rr3) return 1;
    if (FIELD_U32(x2, 0x60) != rr3) return 0;
    return 1;

L_634b:
    /* 6b634b: eax is still arg1's object (rr); edi = [rr+0x59]. */
    edi_v = FIELD_U32(rr, 0x59);
    if (edi_v != 0) goto L_6400;
    t2 = FIELD_U32(rr, 0x00);
    if (t2 == GLOBAL_U32(0x009cf77c)) goto L_63d1;
    if (t2 == GLOBAL_U32(0x009cf7ac)) goto L_63d1;
    if (t2 != GLOBAL_U32(0x009cf774)) goto L_6379;
    c = GLOBAL_U32(0x009cfa0c);
    goto L_63d7;

L_6379:
    if (t2 != GLOBAL_U32(0x009cf770)) goto L_6389;
    c = GLOBAL_U32(0x009cfa14);
    goto L_63d7;

L_6389:
    if (t2 == GLOBAL_U32(0x009cf780)) goto L_63c9;
    if (t2 == GLOBAL_U32(0x009cf7a4)) goto L_63c9;
    if (t2 == GLOBAL_U32(0x009cf784)) goto L_63c9;
    if (t2 != GLOBAL_U32(0x009cf76c)) goto L_63b1;
    c = GLOBAL_U32(0x009cfa18);
    goto L_63d7;

L_63b1:
    if (t2 == GLOBAL_U32(0x009cf778)) goto L_63c1;
    if (t2 != GLOBAL_U32(0x009cf7a8)) goto L_63ee;

L_63c1:
    c = GLOBAL_U32(0x009cfa1c);
    goto L_63d7;

L_63c9:
    c = GLOBAL_U32(0x009cfa10);
    goto L_63d7;

L_63d1:
    c = GLOBAL_U32(0x009cfa08);

L_63d7:
    /* 6b63d7: edi = [0xae23a4] + c * 198. */
    edi_v = GLOBAL_U32(0x00ae23a4) + c * 198u;
    if (edi_v != 0) goto L_6400;

L_63ee:
    if (t2 == GLOBAL_U32(0x009cf7a0)) goto L_6400;
    return 1;

L_6400:
    /* 6b6400 loop: bp = 0 .. 15 over records of 0x6e bytes at [0xae23bc]. */
    bp = 0;
    do {
        idx = GLOBAL_U32(0x00ae2364) - (u32)(s32)(s16)bp - 1u;
        rec3 = GLOBAL_U32(0x00ae23bc) + idx * 110u;
        if (FIELD_U8(rec3, 0x5f) == 1u) {
            /* 6b642b: push [rec3+0x1a]; call 0x53b3d0 (cdecl); add esp, 4. */
            a = (u32)cm_fn_0053b3d0((int *)(u32)FIELD_U32(rec3, 0x1a));
            /* edx = arg1 (from the stack slot); push a; eax = [arg1+0x14]. */
            rr = FIELD_U32(param_1, 0x14);
            tbl = GLOBAL_U32(0x00adadfc);
            t = FIELD_U32(rr, 0x00);
            c = FIELD_U32(tbl, t * 4u);
            al = cm_fn_0048db80((volatile u8 *)(u32)c, (volatile u32 *)(u32)a);
            if (al != 0) return 1;

            xx = FIELD_U32(rec3, 0x39);
            if (xx != 0) {
                yy = FIELD_U32(xx, 0x53);
                if (FIELD_U32(yy, 0x71) == edi_v) return 1;
                if (FIELD_U32(FIELD_U32(rec3, 0x1a), 0x71) == edi_v) return 1;
                /* 6b6476: push yy; call 0x53b3d0; add esp, 4; then the thiscall. */
                b = (u32)cm_fn_0053b3d0((int *)(u32)yy);
                rr = FIELD_U32(param_1, 0x14);
                tbl = GLOBAL_U32(0x00adadfc);
                t = FIELD_U32(rr, 0x00);
                c = FIELD_U32(tbl, t * 4u);
                al = cm_fn_0048db80((volatile u8 *)(u32)c, (volatile u32 *)(u32)b);
                if (al != 0) return 1;
            }
        }
        bp = (u16)(bp + 1u);
    } while ((s16)bp < 0x10);
    return 0;

L_64b1:
    /* 6b64b1: first test on [arg1+0x1c]. */
    res = cm_fn_0053b900((u32 *)(u32)FIELD_U32(param_1, 0x1c));
    if (res != 0) {
        c = FIELD_U32(param_1, 0x1c);
        q = FIELD_U32(c, 0x53);
        al = FIELD_U8(q, 0x11c);
        if (al != 0) return 1;
    }
    /* 6b64d1: second test on [arg1+0x20]. */
    res = cm_fn_0053b900((u32 *)(u32)FIELD_U32(param_1, 0x20));
    if (res == 0) return 0;
    c = FIELD_U32(param_1, 0x20);
    q = FIELD_U32(c, 0x53);
    al = FIELD_U8(q, 0x11c);
    if (al == 0) return 0;
    return 1;
}
