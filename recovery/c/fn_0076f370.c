/* Translation of the original routine at preferred-image VA 0x0076f370
   (CM0102 3.9.68, cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).

   Every statement below is checked against the listing in
   CM0102/Analysis/cmre/farm/20261115T1250Z-sol-commandcode-supervised/bundle/fn_0076f370.md.
   Acceptance comes only from `cmre test --c` / `cmre accept --c`.

   Frame model (L = ESP immediately after `sub esp, 0x1ac` at 0x0076f374):

     L+0x00  u32    result of cm_fn_00785830(self, 1)   ("suffix")
     L+0x04  u32    result of cm_fn_00785830(self, 3)
     L+0x08  u32    result of cm_fn_00785830(self, 5)
     L+0x0c  u32[4] club records (index 0..3)
     L+0x1c  u8[100] cm_fn_00542100 output buffer
     L+0x80  u8[300] message buffer
     (0x0c + 4*4 = 0x1c, 0x1c + 100 = 0x80, 0x80 + 300 = 0x1ac)

   The four saved registers (ebx/esi/edi at L-4/L-8/L-12, ebp at L-16 in the
   main branch) are not part of the observable state; the callee-visible
   buffers keep their original relative offsets inside one scratch block. */

#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Original globals, read at their original sites (re-read after any call,
   exactly like the original). */
#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae23d0 CM_GLOBAL(int, 0x00ae23d0)
#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)

/* The original pushes the addresses of inline string data
   (`push 0xa5c134` at 0x0076f469 and friends), so the addresses themselves
   are the arguments. */
#define STR_00a5c0f8 ((const char *)(u32)0x00a5c0f8)
#define STR_00a5c134 ((const char *)(u32)0x00a5c134)
#define STR_00a5c4e0 ((const char *)(u32)0x00a5c4e0)
#define STR_00a5c51c ((const char *)(u32)0x00a5c51c)
#define STR_00a5c58c ((const char *)(u32)0x00a5c58c)
#define STR_00a5c624 ((const char *)(u32)0x00a5c624)
#define STR_00a5c6b4 ((const char *)(u32)0x00a5c6b4)

/* Callees stay explicit boundaries (original code). */
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_00542100(u32 object, u32 suffix, u32 output, s32 size, s8 mode, u32 brackets);
extern char *cm_fn_0066f4e0(void *target, const char *fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

/* One scratch block for the whole frame (offsets as listed above). */
typedef union {
    u32 align;
    struct {
        u32 idx1;      /* +0x00 */
        u32 idx3;      /* +0x04 */
        u32 idx5;      /* +0x08 */
        u32 club[4];   /* +0x0c */
        u8  name[100]; /* +0x1c */
        u8  msg[300];  /* +0x80 */
    } s;
} cm_frame;

/* 0x0076f422: copy the club name (with its terminator) into the message
   buffer.  The original measures the string with `repne scasb` and copies
   strlen+1 bytes. */
static void cm_copy_name(u8 *dst, u32 club)
{
    const char *src = (const char *)(club + 0x38);
    u32 n = 0;
    u32 i;
    while (src[n] != '\0') {
        n++;
    }
    n += 1; /* the terminating NUL is copied as well */
    for (i = 0; i < n; i++) {
        dst[i] = (u8)src[i];
    }
}

/* 0x0076f44e / 0x0076f4a0 / 0x0076f4d7: "club x and club y" message.
   Argument order (cdecl): target, format, value(x), value(x), value(y),
   value(y), name(x), name(y). */
static void cm_format_two(u8 *msg, u32 x, u32 y)
{
    u32 val_x = (u32)(s32)(s8)*((const s8 *)(x + 0x52));
    u32 val_y = (u32)(s32)(s8)*((const s8 *)(y + 0x52));
    cm_fn_0066f4e0((void *)msg, STR_00a5c134,
                   val_x, val_x, val_y, val_y,
                   (void *)(x + 0x38), (void *)(y + 0x38));
}

/* 0x0076f4fa: three-club message.
   Argument order: target, format, value(x), value(x), value(y), value(y),
   value(z), value(z), name(x), name(y), name(z). */
static void cm_format_three(u8 *msg, u32 x, u32 y, u32 z)
{
    u32 val_x = (u32)(s32)(s8)*((const s8 *)(x + 0x52));
    u32 val_y = (u32)(s32)(s8)*((const s8 *)(y + 0x52));
    u32 val_z = (u32)(s32)(s8)*((const s8 *)(z + 0x52));
    cm_fn_0066f4e0((void *)msg, STR_00a5c0f8,
                   val_x, val_x, val_y, val_y, val_z, val_z,
                   (void *)(x + 0x38), (void *)(y + 0x38), (void *)(z + 0x38));
}

/* The original ends with `ret 0xc` (0x0076f6fd, 0x0076f7a3, 0x0076f7c9):
   the callee pops the three 4-byte arguments, so the entry convention is
   the callee-cleanup one (stdcall). */
u32 CM_STDCALL cm_fn_0076f370(u32 arg1, u32 arg2, u32 arg3)
{
    cm_frame f;
    u32 self;
    u32 r0, r1, r3, r4, r5;
    u32 eaxv;
    s16 bp;
    u32 club0;
    u32 edi_scaled;

    /* 0x0076f37a: `test eax, eax` on the second argument. */
    if (arg2 == 0) {
        /* 0x0076f700 */
        u32 i0, i1, edx_v, esi_v, base, byte52, out_self;

        i0 = (u32)cm_fn_00785830((void *)arg1, 0);
        i1 = (u32)cm_fn_00785830((void *)arg1, 1);

        /* 0x0076f721: edx = 145 * i0 ; esi = 581 * i0 */
        edx_v = i0 + i0 * 8u;
        edx_v <<= 4;
        edx_v += i0;
        esi_v = i0 + edx_v * 4u;

        /* 0x0076f72b..0x0076f73e */
        cm_fn_00542100(esi_v + (u32)DAT_00ae23b4, i1,
                       (u32)(void *)f.s.name, 100, 2, 1);

        /* 0x0076f743: the base is re-read after the call. */
        base = (u32)DAT_00ae23b4;
        byte52 = (u32)(s32)(s8)*((const s8 *)(esi_v + base + 0x52));

        /* 0x0076f754..0x0076f761 */
        cm_fn_0066f4e0((void *)&DAT_00de1f64, STR_00a5c4e0,
                       byte52, byte52, (void *)f.s.name);

        /* 0x0076f766: ebx = third argument (the output object). */
        out_self = arg3;
        /* 0x0076f772 */
        (void)cm_fn_004ae660((void *)out_self, (u32)(void *)&DAT_00de1f64);

        /* 0x0076f77c: `test edi, edi` on the second index. */
        if (i1 != 0) {
            /* 0x0076f780 */
            u32 object = esi_v + (u32)DAT_00ae23b4;
            return cm_fn_004ae8a0(out_self, (const char *)(void *)f.s.name,
                                  0x7d6u, object);
        } else {
            /* 0x0076f7a6 */
            u32 object = esi_v + (u32)DAT_00ae23b4;
            return cm_fn_004ae8a0(out_self, (const char *)(void *)f.s.name,
                                  0x7d5u, object);
        }
    }

    /* 0x0076f385: esi = first argument (the object of cm_fn_00785830). */
    self = arg1;

    /* 0x0076f38d..0x0076f3cc: five calls; each result is stored in the
       frame slot the original used. */
    r0 = (u32)cm_fn_00785830((void *)self, 0);
    f.s.club[3] = r0;                        /* 0x0076f39a: L+0x18 */
    r1 = (u32)cm_fn_00785830((void *)self, 1);
    f.s.idx1 = r1;                           /* 0x0076f3a7: L+0x00 */
    r3 = (u32)cm_fn_00785830((void *)self, 3);
    f.s.idx3 = r3;                           /* 0x0076f3b4: L+0x04 */
    r4 = (u32)cm_fn_00785830((void *)self, 4);
    r5 = (u32)cm_fn_00785830((void *)self, 5);
    f.s.idx5 = r5;                           /* 0x0076f3cc: L+0x08 */

    /* 0x0076f3c1: ebp = the fourth result; `cmp bp, 4` is a 16-bit compare. */
    bp = (s16)r4;
    /* EAX at 0x0076f3d9 (before the first `mov al, bl`). */
    eaxv = r5;

    /* 0x0076f3c8..0x0076f408: collect the club records. */
    if (!(bp >= 4) && bp > 0) {
        u8 bl = 0;
        for (;;) {
            /* 0x0076f3d9: only AL is replaced, the upper bytes of EAX are
               the ones of the previous computed club pointer. */
            u32 arg = (eaxv & 0xffffff00u) | (((u32)(u8)bl + 8u) & 0xffu);
            u32 idx = (u32)cm_fn_00785830((void *)self, arg);
            u32 ecx = idx + idx * 8u;
            u32 edx;
            ecx <<= 4;
            ecx += idx;
            edx = (u32)DAT_00ae23b4 + idx;
            eaxv = edx + ecx * 4u;
            f.s.club[(s8)bl] = eaxv;           /* 0x0076f404 */
            bl++;
            if (!((s16)bl < bp)) {             /* 0x0076f683 */
                break;
            }
        }
    }

    /* 0x0076f40a: ebx = club[0] */
    club0 = f.s.club[0];

    if (bp == 1) {
        /* 0x0076f422 */
        cm_copy_name(f.s.msg, club0);
    } else if (bp == 2) {
        u32 c1 = f.s.club[1];
        if (club0 != c1) {
            /* 0x0076f44e */
            cm_format_two(f.s.msg, club0, c1);
        } else {
            /* 0x0076f422 */
            cm_copy_name(f.s.msg, club0);
        }
    } else if (bp == 3) {
        u32 c1 = f.s.club[1];
        if (club0 != c1) {
            u32 c2 = f.s.club[2];
            if (club0 != c2) {
                /* 0x0076f4cb */
                if (c1 != c2) {
                    /* 0x0076f4fa */
                    cm_format_three(f.s.msg, club0, c1, c2);
                } else {
                    /* 0x0076f4d7: club[1] == club[2] */
                    cm_format_two(f.s.msg, c1, c2);
                }
            } else {
                /* 0x0076f4a0: club[0] == club[2] */
                cm_format_two(f.s.msg, club0, c1);
            }
        } else {
            u32 c2 = f.s.club[2];
            if (club0 != c2) {
                /* 0x0076f44e */
                cm_format_two(f.s.msg, club0, c2);
            } else {
                /* 0x0076f496 -> 0x0076f422 */
                cm_copy_name(f.s.msg, club0);
            }
        }
    }

    /* 0x0076f525 */
    {
        u32 club3 = f.s.club[3];
        u32 suffix = f.s.idx1;
        u32 edx = club3 + club3 * 8u;
        edx <<= 4;
        edx += club3;
        edi_scaled = club3 + edx * 4u;         /* 0x0076f541 */

        /* 0x0076f52d..0x0076f54e */
        cm_fn_00542100(edi_scaled + (u32)DAT_00ae23b4, suffix,
                       (u32)(void *)f.s.name, 100, 2, 1);
    }

    /* 0x0076f553: the third result selects the message. */
    if (f.s.idx3 != 0) {
        /* 0x0076f55e */
        u32 idx5 = f.s.idx5;
        u32 off = idx5 + idx5 * 2u;
        u32 base2;
        u32 val_p, val_c, pname;
        off = off + off * 8u;
        off <<= 2;
        off -= idx5;                           /* 107 * idx5 */
        base2 = (u32)DAT_00ae23d0;
        val_p = (u32)(s32)(s8)*((const s8 *)(base2 + off + 0x37));
        pname = base2 + off + 4u;
        val_c = (u32)(s32)(s8)*((const s8 *)(edi_scaled + (u32)DAT_00ae23b4 + 0x37));
        /* 0x0076f587..0x0076f59b */
        cm_fn_0066f4e0((void *)&DAT_00de1f64, STR_00a5c6b4,
                       val_c, val_c, val_p, val_p,
                       (void *)f.s.name, (void *)pname);
    } else if (bp == 1) {
        /* 0x0076f5ae */
        u32 base = (u32)DAT_00ae23b4;
        u32 val0 = (u32)(s32)(s8)*((const s8 *)(club0 + 0x52));
        u32 val3 = (u32)(s32)(s8)*((const s8 *)(edi_scaled + base + 0x52));
        /* 0x0076f5c4..0x0076f5d8 */
        cm_fn_0066f4e0((void *)&DAT_00de1f64, STR_00a5c624,
                       val3, val3, val0, val0,
                       (void *)f.s.name, (void *)f.s.msg);
    } else {
        /* 0x0076f5e2 */
        u32 base = (u32)DAT_00ae23b4;
        u32 val3 = (u32)(s32)(s8)*((const s8 *)(edi_scaled + base + 0x52));
        if (bp >= 4) {
            /* 0x0076f615 */
            cm_fn_0066f4e0((void *)&DAT_00de1f64, STR_00a5c51c,
                           val3, val3, (void *)f.s.name);
        } else {
            /* 0x0076f5f2 */
            cm_fn_0066f4e0((void *)&DAT_00de1f64, STR_00a5c58c,
                           val3, val3, (void *)f.s.name, (void *)f.s.msg);
        }
    }

    /* 0x0076f62e: esi = third argument (the output object). */
    {
        u32 out_self = arg3;
        u32 key;

        /* 0x0076f635 */
        (void)cm_fn_004ae660((void *)out_self, (u32)(void *)&DAT_00de1f64);

        /* 0x0076f641..0x0076f686 */
        if (!(bp >= 4) && bp > 0) {
            u8 bl = 0;
            for (;;) {
                u32 club = f.s.club[(s8)bl];
                u32 k = (f.s.idx1 != 0) ? 0x7d6u : 0x7d5u;
                (void)cm_fn_004ae8a0(out_self, (const char *)(void *)(club + 0x38),
                                      k, club);
                bl++;
                if (!((s16)bl < bp)) {
                    break;
                }
            }
        }

        /* 0x0076f688 */
        key = (f.s.idx1 != 0) ? 0x7d6u : 0x7d5u;
        {
            u32 object = edi_scaled + (u32)DAT_00ae23b4;
            /* 0x0076f6bb: the result is overwritten by the reload at
               0x0076f6c0, but the call itself still happens. */
            (void)cm_fn_004ae8a0(out_self, (const char *)(void *)f.s.name,
                                  key, object);
            /* 0x0076f6c0 */
            if (f.s.idx3 == 0) {
                return 0;
            }
            /* 0x0076f6cc */
            {
                u32 idx5 = f.s.idx5;
                u32 off = idx5 + idx5 * 2u;
                u32 pbase;
                off = off + off * 8u;
                off <<= 2;
                off -= idx5;
                pbase = (u32)DAT_00ae23d0 + off;
                return cm_fn_004ae8a0(out_self, (const char *)(void *)(pbase + 4u),
                                      0x7d0u, pbase);
            }
        }
    }
}
