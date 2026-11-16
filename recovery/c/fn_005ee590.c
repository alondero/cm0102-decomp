/* fn_005ee590, preferred-image VA 0x005ee590 (image base 0x00400000).
   thiscall: ECX = object (EBP in the listing), ret 4 -> one stack dword, which
   the routine reuses as a WORD colour home and then as the sprintf destination
   / string handed to cm_fn_005e6d80 (entry ESP+4 == [esp+0x20]/[esp+0x24]/
   [esp+0x2c] at the three sites below).

   Bound-target width review, not composition or runtime proof:
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All addresses below are preferred-image VAs.
   005ee6ef/005ee6fb write only CX after calls; 005ee9c9 writes CX,
   005eea6d/005eeb0c/005eebcd write DX. No pre-call register upper
   half is substituted here. These arguments are represented by their consumed
   low WORD, with a zero-extended C carrier, on the following original evidence:
   - 005e5df0 arg6 is [esp+ec] after c4 locals and four saves.
     Colour decoding masks it at 005e5fce, 005e6185, 005e64c5,
     005e6610, 005e683d, 005e6a93 and 005e6bd2; zero tests at
     005e647b/005e67e6/005e681c test CX. Other paths forward to
     005e3b50/005e3730 or 005e7840, not to an unknown import.
     005e3b50 forwards arg6 to 005e3730. That target reads only WORDs
     at 005e37df/005e3873/005e399e/005e3a11/005e3a9c/005e3b0b.
     005e7840 forwards arg6 at 005e7965/005e79ff to 005e7aa0 arg8,
     whose reads at 005e7b78/005e7be0/005e7c3d/005e7cb3/005e7d78/
     005e7de7/005e7e49/005e7ec0 are WORDs; its other path forwards
     to 005e3730 at 005e7a59..005e7a6e.
   - 005e6d80 arg7 is [esp+2e4] after the SEH frame, 2b0 locals
     and three saves (2e8 after saving EBP). Its only value use is
     005e75e6..005e75f7 forwarding to 005e5160 arg4. That target
     masks at 005e5580 or reads WORD at 005e5711. The hardware branch
     forwards through 005e529c/005e52cd..005e540a to 005b0d80 arg6;
     005b0e30..005b0e70 extracts only bits 0..15 into byte channels.
     Underline paths forward it to 005e3730 at 005e5459..005e5513
     and 005e5a02..005e5a1b (WORD consumers above).
   Explicitly determined upper halves at 005ee6e6/005ee716 and
   005eebca/005eebd8 are retained. Callees remain original boundaries. */
#include "cmre.h"

#define DAT_0099ec1c CM_GLOBAL(u8, 0x0099ec1c)
#define DAT_009cc2c4 CM_GLOBAL(int, 0x009cc2c4)
#define DAT_009cd67c CM_GLOBAL(u8, 0x009cd67c)
#define DAT_009cd67d CM_GLOBAL(u8, 0x009cd67d)
#define DAT_009cd69c CM_GLOBAL(u8, 0x009cd69c)
#define DAT_009cd69d CM_GLOBAL(u8, 0x009cd69d)
#define DAT_009cd6bc CM_GLOBAL(u8, 0x009cd6bc)
#define DAT_009cd6bd CM_GLOBAL(u8, 0x009cd6bd)
#define DAT_009cd6dc CM_GLOBAL(u8, 0x009cd6dc)
#define DAT_009cd6dd CM_GLOBAL(u8, 0x009cd6dd)
#define DAT_009cd6fc CM_GLOBAL(u8, 0x009cd6fc)
#define DAT_009cd6fd CM_GLOBAL(u8, 0x009cd6fd)
#define DAT_009cd730 CM_GLOBAL(u8, 0x009cd730)
#define DAT_009cd731 CM_GLOBAL(u8, 0x009cd731)
#define DAT_009cd774 CM_GLOBAL(u8, 0x009cd774)
#define DAT_009cd775 CM_GLOBAL(u8, 0x009cd775)
#define DAT_009cd7bc CM_GLOBAL(u8, 0x009cd7bc)
#define DAT_009cd7bd CM_GLOBAL(u8, 0x009cd7bd)
#define DAT_00ae291c CM_GLOBAL(short, 0x00ae291c)
#define DAT_00ae2920 CM_GLOBAL(u8, 0x00ae2920)
#define DAT_00ae2a24 CM_GLOBAL(int, 0x00ae2a24)
#define DAT_00ae3184 CM_GLOBAL(short, 0x00ae3184)
#define DAT_00ae31a8 CM_GLOBAL(u16, 0x00ae31a8)
#define DAT_00aebdd4 CM_GLOBAL(u16, 0x00aebdd4)
#define DAT_00aebdea CM_GLOBAL(short, 0x00aebdea)

extern u32 CM_THISCALL cm_fn_00403760(u32 self, volatile u32 first_out,
                                     volatile u32 second_out);
extern void cm_fn_005e3ce0(int param_1, int param_2, u32 param_3, u8 *param_4);
extern int *cm_fn_005e3dd0(u32 param_1, u32 param_2, u32 param_3, u32 param_4,
                           int *param_5);
extern void cm_fn_005e3fd0(int param_1, int param_2, int *param_3);
extern int *cm_fn_005e40d0(int param_1, int *param_2);
extern void CM_CDECL cm_fn_005e42b0(u32 object);
extern u32 CM_CDECL cm_fn_005e4890(u32 mask_arg, u32 alpha_arg, u32 config_arg);
extern u32 CM_CDECL cm_fn_005e5df0(u32 left, u32 top, u32 right, u32 bottom,
                                   u32 flags, u32 color, u32 other_color);
extern void cm_fn_005e6d80(int param_1, int param_2, int param_3, int param_4,
                           u32 param_5, u32 param_6, u32 param_7,
                           char *param_8, int param_9);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

/* strlen as the listing's `or ecx,-1; repne scasb; not ecx` (length only). */
static u32 cmre_len0(const u8 *p)
{
    u32 n = 0;
    while (*(volatile u8 *)(p + n) != 0) n = n + 1u;
    return n;
}

/* Wrapped numerator, signed truncation, then wrapping ADDs:
   005ee7e9..005ee7f7 (parallel 005ee849..005ee857,
   005ee8b2..005ee8c2, 005ee90e..005ee91e).
   Y uses the same sequence at 005ee939..005ee945,
   005ee973..005ee986, 005eea1a..005eea29,
   005eeabe..005eeacd and 005eeb5d..005eeb6c.
   Converting the quotient to u32 BEFORE adding avoids signed C overflow. */
#define CMRE_CX(right, left, d) \
    ((u32)((int)((u32)(right) - (u32)(d) - (u32)(left) + 1u) / 2) + (u32)(left))

int CM_THISCALL cm_fn_005ee590(int *this_, u32 arg_in)
{
    u8 *s = (u8 *)this_;
    union {
        u32 d;
        u16 w[2];
        u8 b[0x100];
    } home;
    volatile u32 L4;            /* [esp+0x14]: first coordinate out */
    volatile u32 L8;            /* [esp+0x18]: second coordinate out */
    u32 L0;                     /* [esp+0x10]: -1, later an index */
    u32 esi;                    /* xor esi,esi at 0x005ee598 */
    u32 ebx;                    /* xor ebx,ebx at 0x005ee6f5 */
    u32 flags;

    home.d = arg_in;
    L0 = 0xffffffffu;
    esi = 0;
    ebx = 0;

    /* 0x005ee59b..0x005ee5c3 */
    {
        u32 v184 = *(volatile u32 *)(s + 0x184);

        if (v184 != 0) {
            /* mov ax,[ebp+0x72] keeps the high half of EAX (= s[0x61]). */
            u32 arg1 = (v184 & 0xffff0000u) | (u32)*(volatile u16 *)(s + 0x72);

            *(volatile u32 *)(s + 0x184) = 0;
            *(volatile u16 *)(s + 0x72) = (u16)cm_fn_005e4890(arg1, 0x6e, 0);
        }
    }

    /* 0x005ee5c7..0x005ee5dd */
    {
        u32 v4c = *(volatile u32 *)(s + 0x4c);

        if (v4c != 0 && home.d != 0) {
            cm_fn_005e42b0(v4c);
            *(volatile u32 *)(s + 0x4c) = 0;
        }
    }

    /* 0x005ee5e0..0x005ee604 */
    if ((*(volatile u8 *)(s + 0x38) & 0x10u) == 0
        && *(volatile u32 *)(s + 0x4c) == 0) {
        u32 b7 = *(volatile u32 *)(s + 0x1c);
        u32 r6 = *(volatile u32 *)(s + 0x18);
        u32 t5 = *(volatile u32 *)(s + 0x14);
        u32 l4 = *(volatile u32 *)(s + 0x10);

        *(volatile u32 *)(s + 0x4c) =
            (u32)cm_fn_005e3dd0(l4, t5, r6, b7, (int *)0);
    }

    /* 0x005ee607..0x005ee6ce: name/string cache maintenance */
    flags = *(volatile u32 *)(s + 0xc);
    if ((flags & 0x400u) != 0
        && *(volatile u32 *)(s + 0x5c) == 0) {
        u8 *txt = s + 0x80;
        volatile u8 *g = &DAT_00ae2920;
        u8 *q = txt;
        int cmp = 0;

        /* byte-wise strcmp(&DAT_00ae2920, s+0x80); result sign only */
        for (;;) {
            u8 c1 = g[0];
            u8 c2 = *(volatile u8 *)(q);
            u8 d1, d2;

            if (c1 != c2) {
                cmp = (c1 < c2) ? -1 : 1;
                break;
            }
            if (c1 == 0) {
                cmp = 0;
                break;
            }
            d1 = g[1];
            d2 = *(volatile u8 *)(q + 1);
            if (d1 != d2) {
                cmp = (d1 < d2) ? -1 : 1;
                break;
            }
            g += 2;
            q += 2;
            if (d1 == 0) {
                cmp = 0;
                break;
            }
        }

        if (cmp == 0 && DAT_00ae2a24 != 0) {
            *(volatile u32 *)(s + 0x5c) = (u32)DAT_00ae2a24;
            DAT_00ae291c = (short)((u16)DAT_00ae291c + 1u);
        } else {
            u32 r = (u32)cm_fn_005e40d0((int)(u32)txt, (int *)0);

            *(volatile u32 *)(s + 0x5c) = r;
            if (r != 0 && DAT_00ae291c == 0) {
                u32 t = (u32)DAT_00ae2a24;

                if (t != 0) cm_fn_005e42b0(t);
                DAT_00ae2a24 = (int)*(volatile u32 *)(s + 0x5c);
                {
                    u32 n = 1;
                    u32 k;

                    while (*(volatile u8 *)(txt + (n - 1u)) != 0) n = n + 1u;
                    for (k = 0; k < n; k = k + 1u) {
                        *(volatile u8 *)(&DAT_00ae2920 + k) =
                            *(volatile u8 *)(txt + k);
                    }
                }
                DAT_00ae291c = 1;
            }
        }
    }

    /* 0x005ee6ce..0x005ee6e3 */
    {
        u32 v5c = *(volatile u32 *)(s + 0x5c);

        if (v5c != 0) {
            u32 l4 = *(volatile u32 *)(s + 0x10);
            u32 t5 = *(volatile u32 *)(s + 0x14);

            cm_fn_005e3fd0((int)l4, (int)t5, (int *)v5c);
        }
    }

    /* 0x005ee6e6..0x005ee736 */
    {
        u32 x188 = *(volatile u32 *)(s + 0x188);
        u32 f38 = *(volatile u32 *)(s + 0x38);
        u32 colour;

        /* Consumed WORD, not an assertion about post-call ECX. */
        colour = (u32)*(volatile u16 *)(s + 0x72);
        esi = 0;
        ebx = 0;
        if (x188 != 0) {
            u16 c74 = *(volatile u16 *)(s + 0x74);   /* mov cx,[ebp+0x74] */

            colour = (u32)c74;
            if ((f38 & 0x20u) != 0) {
                if ((*(volatile u8 *)(s + 0xc) & 4u) == 0) f38 = f38 | 0x40u;
            }
        }
        if ((f38 & 0x40u) != 0) {
            esi = 2;
            ebx = esi;
        }
        {
            /* mov dx,[ebp+0x78] keeps the high half of EDX (= s[0x62]) */
            u32 other = (x188 & 0xffff0000u) | (u32)*(volatile u16 *)(s + 0x78);
            u32 t5 = *(volatile u32 *)(s + 0x14);
            u32 r6 = *(volatile u32 *)(s + 0x18);
            u32 b7 = *(volatile u32 *)(s + 0x1c);
            u32 l4 = *(volatile u32 *)(s + 0x10);

            cm_fn_005e5df0(l4, t5, r6, b7, f38, colour, other);
        }
    }

    /* 0x005ee732..0x005ee94e */
    {
        u16 idx = *(volatile u16 *)(s + 0x7c);

        if (idx != 0xffffu && (*(volatile u8 *)(s + 0xc) & 0x80u) == 0) {
            u32 off1 = (u32)(s16)idx * 3057u;
            int x = 0;
            u32 dy = 0;
            u8 *glyph = (u8 *)0;
            int draw = 0;
            u8 *txt = s + 0x80;

            cm_fn_00403760(*(volatile u32 *)s + off1, (u32)&L8, (u32)&L4);
            {
                u16 idx2 = *(volatile u16 *)(s + 0x7c);
                u8 *rec = (u8 *)(*(volatile u32 *)s
                                 + (u32)(s16)idx2 * 3057u);
                u32 cw;
                u32 rec3, rec1;
                u32 l4v;

                cw = (u32)(u16)DAT_00aebdea;
                if (*(volatile u16 *)(s + 0x72) == (u16)DAT_00aebdea)
                    cw = (u32)(u16)DAT_00ae3184;
                home.w[0] = (u16)cw;

                rec3 = *(volatile u32 *)(rec + 0xc);
                rec1 = *(volatile u32 *)(rec + 4);
                l4v = (u32)L4;
                if ((int)(rec3 - rec1 + l4v) < (int)*(volatile u32 *)(s + 0x14)) {
                    u32 rv = *(volatile u32 *)(s + 0x18);

                    glyph = &DAT_009cd6bc;
                    if (txt != (u8 *)0 && cmre_len0(txt) != 0) {
                        x = (int)(rv - (u32)DAT_009cd6bc - 4u + ebx);
                    } else {
                        u32 lv = *(volatile u32 *)(s + 0x10);

                        x = (int)(CMRE_CX(rv, lv, (u32)DAT_009cd6bc) + ebx);
                    }
                    dy = (u32)DAT_009cd6bd;
                    draw = 1;
                } else if ((int)l4v > (int)*(volatile u32 *)(s + 0x1c)) {
                    u32 rv = *(volatile u32 *)(s + 0x18);

                    glyph = &DAT_009cd6dc;
                    if (txt != (u8 *)0 && cmre_len0(txt) != 0) {
                        x = (int)(rv - (u32)DAT_009cd6dc - 4u + ebx);
                    } else {
                        u32 lv = *(volatile u32 *)(s + 0x10);

                        x = (int)(CMRE_CX(rv, lv, (u32)DAT_009cd6dc) + ebx);
                    }
                    dy = (u32)DAT_009cd6dd;
                    draw = 1;
                } else {
                    u32 rec2 = *(volatile u32 *)(rec + 8);
                    u32 rec0 = *(volatile u32 *)(rec + 0);
                    u32 l8v = (u32)L8;

                    if ((int)(rec2 - rec0 + l8v) < (int)*(volatile u32 *)(s + 0x10)) {
                        u32 rv = *(volatile u32 *)(s + 0x18);

                        glyph = &DAT_009cd67c;
                        if (txt != (u8 *)0 && cmre_len0(txt) != 0) {
                            u32 lv = *(volatile u32 *)(s + 0x10);

                            x = (int)(lv + ebx + 4u);
                        } else {
                            u32 lv = *(volatile u32 *)(s + 0x10);

                            x = (int)(CMRE_CX(rv, lv, (u32)DAT_009cd67c) + ebx);
                        }
                        dy = (u32)DAT_009cd67d;
                        draw = 1;
                    } else {
                        u32 s6b = *(volatile u32 *)(s + 0x18);

                        if ((int)l8v > (int)s6b) {
                            glyph = &DAT_009cd69c;
                            if (txt != (u8 *)0 && cmre_len0(txt) != 0) {
                                x = (int)(s6b - (u32)DAT_009cd69c - 4u + ebx);
                            } else {
                                u32 lv = *(volatile u32 *)(s + 0x10);

                                x = (int)(CMRE_CX(s6b, lv, (u32)DAT_009cd69c) + ebx);
                            }
                            dy = (u32)DAT_009cd69d;
                            draw = 1;
                        }
                    }
                }
            }
            if (draw) {
                u32 col = home.d;
                u32 s7v = *(volatile u32 *)(s + 0x1c);
                u32 s5v = *(volatile u32 *)(s + 0x14);
                int y = (int)(CMRE_CX(s7v, s5v, dy) + esi);

                cm_fn_005e3ce0(x, y, col, glyph);
            }
        }
    }

    /* 0x005ee951..0x005ee99d */
    flags = *(volatile u32 *)(s + 0xc);
    if ((flags & 0x2000u) != 0) {
        u32 l4 = *(volatile u32 *)(s + 0x10);
        u32 s5v = *(volatile u32 *)(s + 0x14);
        u32 d = (u32)DAT_009cd6fd;
        int x = (int)(l4 + ebx + 4u);
        u32 s7v = *(volatile u32 *)(s + 0x1c);
        int y = (int)(CMRE_CX(s7v, s5v, d) + esi);
        u32 c;

        c = (s5v & 0xffff0000u) | (u32)(u16)DAT_00aebdea;
        if (*(volatile u16 *)(s + 0x72) == (u16)DAT_00aebdea)
            c = (s5v & 0xffff0000u) | (u32)(u16)DAT_00ae3184;
        cm_fn_005e3ce0(x, y, c, &DAT_009cd6fc);
    }

    /* 0x005ee9a0..0x005eea47 */
    flags = *(volatile u32 *)(s + 0xc);
    if ((flags & 0x4000u) != 0) {
        if (DAT_009cc2c4 != -1) {
            cm_fn_00944c9f((u32)&home, (u32)&DAT_0099ec1c, 0x2b);
            {
                u16 c78 = *(volatile u16 *)(s + 0x78);
                u32 bot = *(volatile u32 *)(s + 0x1c);
                u32 col = (u32)c78; /* CX at 005ee9c9; consumed WORD */
                u32 r6 = *(volatile u32 *)(s + 0x18);
                u32 l4 = *(volatile u32 *)(s + 0x10);
                u32 t5 = *(volatile u32 *)(s + 0x14);

                cm_fn_005e6d80((int)(l4 + ebx), (int)(esi + t5),
                               (int)(r6 + ebx - 4u), (int)(bot + esi), 0x48, 0,
                               col, (char *)&home, -1);
            }
        } else {
            u32 s5v = *(volatile u32 *)(s + 0x14);
            u32 d = (u32)DAT_009cd731;
            u32 s7v = *(volatile u32 *)(s + 0x1c);
            u32 s6v = *(volatile u32 *)(s + 0x18);
            int y = (int)(CMRE_CX(s7v, s5v, d) + esi);
            int x = (int)(s6v - (u32)DAT_009cd730 - 4u + ebx);

            cm_fn_005e3ce0(x, y, (u32)DAT_00aebdd4, &DAT_009cd730);
        }
    }

    /* 0x005eea47..0x005eeae3 */
    flags = *(volatile u32 *)(s + 0xc);
    if ((flags & 0x20000u) != 0) {
        if (DAT_009cc2c4 != -1) {
            cm_fn_00944c9f((u32)&home, (u32)&DAT_0099ec1c, 0x2c);
            {
                u16 c78 = *(volatile u16 *)(s + 0x78);
                u32 bot = *(volatile u32 *)(s + 0x1c);
                u32 col = (u32)c78; /* DX at 005eea6d; consumed WORD */
                u32 r6 = *(volatile u32 *)(s + 0x18);
                u32 l4 = *(volatile u32 *)(s + 0x10);
                u32 t5 = *(volatile u32 *)(s + 0x14);

                cm_fn_005e6d80((int)(l4 + ebx), (int)(esi + t5),
                               (int)(r6 + ebx - 4u), (int)(bot + esi), 0x48, 0,
                               col, (char *)&home, -1);
            }
        } else {
            u32 s5v = *(volatile u32 *)(s + 0x14);
            u32 d = (u32)DAT_009cd775;
            u32 s7v = *(volatile u32 *)(s + 0x1c);
            u32 s6v = *(volatile u32 *)(s + 0x18);
            int y = (int)(CMRE_CX(s7v, s5v, d) + esi);
            int x = (int)(s6v - (u32)DAT_009cd774 - 4u + ebx);

            cm_fn_005e3ce0(x, y, (u32)DAT_00aebdd4, &DAT_009cd774);
        }
    }

    /* 0x005eeae6..0x005eeb82 */
    flags = *(volatile u32 *)(s + 0xc);
    if ((flags & 0x40000u) != 0) {
        if (DAT_009cc2c4 != -1) {
            cm_fn_00944c9f((u32)&home, (u32)&DAT_0099ec1c, 0x2d);
            {
                u16 c78 = *(volatile u16 *)(s + 0x78);
                u32 bot = *(volatile u32 *)(s + 0x1c);
                u32 col = (u32)c78; /* DX at 005eeb0c; consumed WORD */
                u32 r6 = *(volatile u32 *)(s + 0x18);
                u32 l4 = *(volatile u32 *)(s + 0x10);
                u32 t5 = *(volatile u32 *)(s + 0x14);

                cm_fn_005e6d80((int)(l4 + ebx), (int)(esi + t5),
                               (int)(r6 + ebx - 4u), (int)(bot + esi), 0x48, 0,
                               col, (char *)&home, -1);
            }
        } else {
            u32 s5v = *(volatile u32 *)(s + 0x14);
            u32 d = (u32)DAT_009cd7bd;
            u32 s7v = *(volatile u32 *)(s + 0x1c);
            u32 s6v = *(volatile u32 *)(s + 0x18);
            int y = (int)(CMRE_CX(s7v, s5v, d) + esi);
            int x = (int)(s6v - (u32)DAT_009cd7bc - 4u + ebx);

            cm_fn_005e3ce0(x, y, (u32)DAT_00ae31a8, &DAT_009cd7bc);
        }
    }

    /* 0x005eeb85..0x005eec1a */
    flags = *(volatile u32 *)(s + 0xc);
    if ((flags & 0x400u) != 0) return (int)flags;

    if ((flags & 0x800u) != 0) {
        u8 c = *(volatile u8 *)(s + 0x80);
        u32 i = 0;

        while (c != 1 && c != 0) {
            c = *(volatile u8 *)(s + 0x81 + i);
            i = i + 1u;
        }
        if (*(volatile u8 *)(s + 0x80 + i) == 1) {
            *(volatile u8 *)(s + 0x80 + i) = 0;
            L0 = i;
        }
    }
    {
        u32 p9 = *(volatile u32 *)(s + 0x44);
        u16 w78 = *(volatile u16 *)(s + 0x78);
        u8 *text = s + 0x80;
        u32 col = (u32)w78; /* DX at 005eebcd; consumed WORD */
        u32 parm6 = (p9 & 0xffff0000u) | (u32)*(volatile u16 *)(s + 0x76);
        u32 p5 = *(volatile u32 *)(s + 0x3c);
        u32 b7 = *(volatile u32 *)(s + 0x1c);
        u32 r6 = *(volatile u32 *)(s + 0x18);
        u32 t5 = *(volatile u32 *)(s + 0x14);
        u32 l4 = *(volatile u32 *)(s + 0x10);

        cm_fn_005e6d80((int)(l4 + ebx), (int)(esi + t5), (int)(r6 + ebx),
                       (int)(b7 + esi), p5, parm6, col, (char *)text, (int)p9);
    }
    {
        u32 e = L0;

        if (e != 0xffffffffu) *(volatile u8 *)(s + 0x80 + e) = 1;
        return (int)e;
    }
}
