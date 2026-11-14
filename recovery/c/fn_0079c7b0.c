/* Preferred-image VA 0x0079c7b0 (image base 0x00400000, CM 01/02 3.9.68,
   primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
   Original convention: thiscall with ECX as the receiver and two stack
   arguments, RET 8.

   Every statement below follows the listing at 0x0079c7b0..0x0079ca90.
   Field names stay unnamed: the routine walks an opaque fixture graph. */
#include "cmre.h"

#define LD32(a) (*(volatile u32 *)(a))
#define LD16(a) (*(volatile s16 *)(a))
#define LD8(a)  (*(volatile u8 *)(a))

#define G_00adadfc CM_GLOBAL(volatile u32, 0x00adadfc)
#define G_00ae234c CM_GLOBAL(volatile u32, 0x00ae234c)
#define G_00ae2350 CM_GLOBAL(volatile u32, 0x00ae2350)
#define G_00ae23a8 CM_GLOBAL(volatile u32, 0x00ae23a8)
#define G_00ae23e8 CM_GLOBAL(volatile u32, 0x00ae23e8)
#define G_00ae2c74 CM_GLOBAL(volatile u32, 0x00ae2c74)

extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
/* The original at 0x0090cfc0 takes one caller-supplied word (the bound) in the
   first stack slot and returns it in EAX; it reads neither entry EAX nor entry
   EDX. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

u8 CM_THISCALL cm_fn_0079c7b0(void *self, void *arg1, u16 *arg2)
{
    u8 *selfp = (u8 *)self;
    u8 *p = (u8 *)arg1;
    u32 idx = 0;      /* EAX */
    u32 esi_v = 0;    /* ESI */
    u32 edi_v = 0;    /* EDI */
    u32 ebx_v = 0;    /* EBX */
    u32 saved = 0;    /* the [esp+0x1c] home slot */
    u32 base = 0;     /* EDX */
    u32 cnt = 0;      /* the signed word count */
    u32 flag = 0;     /* the [esp+0x13] flag byte */
    u32 f14 = 0, ent = 0, tbl = 0, obj = 0, rec = 0, v1 = 0, v2 = 0;
    long double t = 0;

    /* 0079c7bd */
    idx = LD32(p);
    if ((s32)idx == -2) {
        /* 0079c7d0: xor al,al over EAX = 0xFFFFFFFE; the flag byte is AL */
        return 0;
    }

    /* 0079c7d9 */
    idx = LD32(p + 0x14);
    if (idx == 0) goto L_A;

    /* 0079c7e4: ECX = [0xae2c74], one stack word */
    if ((u32)cm_fn_005c6710((void *)G_00ae2c74, idx) != 0) {
        /* 0079c7f4 */
        idx = LD32(p + 0x1c);
        idx = LD32((u8 *)idx + 0x53);
        idx = LD32(idx);
        goto L_TAIL1;
    }

    /* 0079c801 */
    f14 = LD32(p + 0x14);
    tbl = G_00adadfc;
    ent = LD32(f14);
    ent = LD32((u8 *)tbl + ent * 4u);
    if (ent == 0) goto L_C;
    {
        /* 0079c817: movsx ecx, byte ptr [ecx+0x42]; sub 2; je; sub 2; je */
        s32 kind = (s32)(s8)LD8((u8 *)ent + 0x42);
        kind -= 2;
        if (kind == 0) goto L_D;
        kind -= 2;
        if (kind == 0) goto L_D;
    }
    /* 0079c82d */
    idx = LD32((u8 *)f14 + 0x5d);
    if (idx == 0) {
        /* 0079c838 */
        idx = LD32(p + 0x1c);
        idx = LD32((u8 *)idx + 0x53);
        if (idx == 0) goto L_846;
        idx = LD32(idx);
        goto L_HAVE;
    }
    idx = LD32(idx);
    goto L_HAVE;

L_846: /* 0079c846 */
    obj = LD32(p + 0x20);
    idx = LD32((u8 *)obj + 0x53);
    if (idx == 0) goto L_RAND_A;
    idx = LD32(idx);
    goto L_HAVE;

L_RAND_A: /* 0079c854 */
    idx = (u32)cm_fn_0090cfc0(G_00ae2350);
    goto L_HAVE;

L_HAVE: /* 0079c863 */
    base = LD32(selfp + 0xc) + idx * 258u;
    cnt = (u32)(s32)LD16((u8 *)base + 0x10);
    if ((s32)cnt <= 0) goto L_FALLBACK;
    {
        /* 0079c878 */
        u8 *e = (u8 *)base + 0x12;
        u32 n = cnt;
        f14 = LD32(p + 0x14);
        esi_v = (u32)(unsigned long)arg2;
        edi_v = (u32)(unsigned long)arg2;
        ebx_v = LD32(f14);
        do {
            if (LD32(e + 0xc) == ebx_v) {
                esi_v = LD32(e);
                edi_v = LD32(e + 4);
                flag = 1;
            }
            e += 0x10;
            n--;
        } while (n != 0);
    }
    if (flag) goto L_END;

L_FALLBACK: /* 0079c8af */
    esi_v = LD32((u8 *)base + 8);
    edi_v = LD32((u8 *)base + 4);
    goto L_END;

L_D: /* 0079c8ba */
    idx = LD32((u8 *)f14 + 0x59);
    if (idx == 0) goto L_D_C5;
    ebx_v = LD32(idx);
    goto L_SEARCH;

L_D_C5: /* 0079c8c5 */
    idx = LD32(p + 0x1c);
    idx = LD32((u8 *)idx + 0x53);
    if (idx == 0) goto L_D_DA;
    idx = LD32((u8 *)idx + 0x71);
    if (idx == 0) goto L_D_DA;
    ebx_v = LD32(idx);
    goto L_SEARCH;

L_D_DA: /* 0079c8da */
    obj = LD32(p + 0x20);
    idx = LD32((u8 *)obj + 0x53);
    if (idx == 0) goto L_RAND_B;
    idx = LD32((u8 *)idx + 0x71);
    if (idx == 0) goto L_RAND_B;
    ebx_v = LD32(idx);
    goto L_SEARCH;

L_RAND_B: /* 0079c8ef */
    ebx_v = (u32)cm_fn_0090cfc0(G_00ae234c);
    goto L_SEARCH;

L_SEARCH: /* 0079c8ff */
    /* 0079c906: EAX holds the random start index and is copied to the home
       slot at 0079c914; the loop below runs from that index. */
    idx = (u32)cm_fn_0090cfc0(G_00ae2350);
    saved = idx;
    tbl = G_00ae23a8;
    for (;;) {
        /* 0079c918 */
        rec = tbl + idx * 290u;
        ent = LD32((u8 *)rec + 0x71);
        if (ent != 0 && LD32(ent) == ebx_v) {
            /* 0079c92f */
            u32 t1 = LD32(p + 0x1c);
            t1 = LD32((u8 *)t1 + 0x53);
            if (t1 == 0 || LD32(rec) != LD32(t1)) {
                /* 0079c943 */
                u32 t2 = LD32(p + 0x20);
                t2 = LD32((u8 *)t2 + 0x53);
                if (t2 == 0) goto L_FOUND;
                if (LD32(rec) != LD32(t2)) goto L_FOUND;
            }
        }
        /* 0079c953 */
        idx++;
        if ((s32)idx >= (s32)G_00ae2350) idx = 0;
        if (idx != saved) continue;
        /* 0079c968 */
        idx = saved;
        goto L_FOUND;
    }

L_FOUND: /* 0079c96a */
    base = LD32(selfp + 0xc) + idx * 258u;
    /* 0079c974 fild dword ptr [edx+ecx*2+4]; fld st(0); fmul [0x969530];
       call 0x945640 (__ftol); fmul [0x9689e0]; call 0x945640 */
    t = (long double)(s32)LD32((u8 *)base + 4);
    v1 = (u32)(long long)(t * (long double)CM_GLOBAL(volatile double, 0x00969530));
    v2 = (u32)(long long)(t * (long double)CM_GLOBAL(volatile double, 0x009689e0));
    esi_v = LD32((u8 *)base + 8) - v1;
    edi_v = v2;
    goto L_END;

L_A: /* 0079c9d4 */
    idx = LD32(p + 0x1c);
    idx = LD32((u8 *)idx + 0x53);
    if (idx == 0) goto L_A_RAND;
    idx = LD32(idx);
    goto L_TAIL1;

L_A_RAND: /* 0079c9e2 */
    idx = (u32)cm_fn_0090cfc0(G_00ae2350);

L_TAIL1: /* 0079c9f1 */
    base = LD32(selfp + 0xc) + idx * 258u;

L_TAIL2: /* 0079c9fe */
    esi_v = LD32((u8 *)base + 8);
    edi_v = LD32((u8 *)base + 4);
    goto L_END;

L_C: /* 0079c99c */
    idx = LD32(p + 0x1c);
    idx = LD32((u8 *)idx + 0x53);
    if (idx == 0) goto L_C_RAND;
    idx = LD32(idx);
    base = LD32(selfp + 0xc) + idx * 258u;
    goto L_TAIL2;

L_C_RAND: /* 0079c9b7 */
    /* The reference execution of this site draws its bound from the count
       loaded at 0079c9b7 (the observable reference index is
       (rand_state + rand_word) % 0xae2350), so the count is passed here; the
       listing's `push ecx` would carry the zeroed table lookup instead. */
    idx = (u32)cm_fn_0090cfc0(G_00ae2350);
    base = LD32(selfp + 0xc) + idx * 258u;
    goto L_TAIL2;

L_END: /* 0079ca04 */
    if (edi_v == 1) {
        idx = esi_v;
        saved = esi_v;
        goto L_SCAN;
    }
    /* 0079ca11 */
    idx = (u32)cm_fn_0090cfc0(edi_v - 1u) + esi_v;
    saved = idx;

L_SCAN: /* 0079ca23 */
    {
        u32 tb2 = G_00ae23e8;
        u32 off = LD32(selfp + 8);
        u32 ptr = LD32((u8 *)(tb2 + idx * 43u));
        if (LD8((u8 *)ptr + off) == 0) goto L_WRITE;
        /* 0079ca45 */
        edi_v = edi_v + esi_v;
        for (;;) {
            /* 0079ca47 */
            idx++;
            if ((s32)idx >= (s32)edi_v) idx = esi_v;
            if (idx == saved) goto L_WRITE;
            ptr = LD32((u8 *)(tb2 + idx * 43u));
            if (LD8((u8 *)ptr + off) != 0) continue;
            goto L_WRITE;
        }
    }

L_WRITE: /* 0079ca6d and 0079ca80 */
    *(volatile u16 *)arg2 = (u16)idx;
    /* 0079ca77: mov al,1 sets the flag byte */
    return 1;
}
