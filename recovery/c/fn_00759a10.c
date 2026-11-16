/* Faithful translation of preferred-image VA 0x00759a10 (image base 0x00400000).
   Listing-derived only.  B = L - 0x10 where L is the base of the 0x4534-byte
   probe allocation; scratch[0] == L == B+0x10.  The SEH frame is separate and
   sits at B+0x4544..B+0x454c (prev/handler/state). */

#include "cmre.h"

#define PTR(a) ((void *)(unsigned long)(u32)(a))

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
/* Evidenced from the listing: 0x0066f4e0 is a cdecl formatter called with 2,
   4, 6 and 13 stack arguments at different sites (0x759df5, 0x75a2b5, 0x759f34,
   0x759f93/.../0x75a043).  The earlier 2-parameter map is contradicted. */
extern char * CM_CDECL cm_fn_0066f4e0(void *destination, const char *format, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_CDECL cm_fn_007e4fa0(void *source_arg, void *counter_arg, void *destination_arg);
extern u32 * CM_CDECL cm_fn_0090c310(u32 param_1, char param_2);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
/* 0x00945610 is the stack-probe helper; the original passes 0x4534 in EAX. */
extern void CM_CDECL cm_fn_00945610(void);

#define DAT_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
#define DAT_00b77c7a CM_GLOBAL(volatile s16, 0x00b77c7a)

CM_STDCALL void cm_fn_00759a10(u32 arg1, u32 arg2, u32 arg3)
{
    volatile struct {
        void *prev;
        void *handler;
        s32   state;
    } frame;
    u8  scratch[0x4534];
    u32 g10;                 /* getter #10 result lives in EDI, never spilled */
    u32 ebp_addr;            /* record pointer derived from getter #10 */
    u32 ebx_addr;            /* record pointer derived from getter #11, or 0 */
    u32 i;

#define SB(off) (scratch + ((off) - 0x10))
#define RD32(off) (*(volatile u32 *)(void *)SB(off))
#define WR32(off, v) (*(volatile u32 *)(void *)SB(off) = (u32)(v))
#define RDB(off) (*(volatile u8 *)(void *)SB(off))
#define WRB(off, v) (*(volatile u8 *)(void *)SB(off) = (u8)(v))

    /* 0x00759a10..0x00759a23: SEH prologue  push -1 / push handler / push prev */
    frame.state   = -1;
    frame.handler = PTR(0x0095d99b);
    frame.prev    = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    /* 0x00759a1e/0x00759a2a: the original stages 0x4534 in EAX and calls
       0x00945610 (the compiler stack-probe/allocator).  Plain C cannot pass a
       size in EAX, and the C frame below already provides the 0x4534-byte
       allocation region, so the probe is represented by that implicit frame
       allocation rather than by a call with an unfaithful register image. */

    /* 0x00759a33..0x00759a3c */
    {
        u16 *p64 = (u16 *)SB(0x64);
        WRB(0x12, 0);                 /* mov byte [esp+0x12],0 */
        cm_fn_0054a110(p64);          /* ECX = lea [esp+0x64] */
    }

    /* Getter sequence: index byte at B+0x12, incremented after each call.
       Call i (0-based) returns in EDX:EAX; only EAX is stored. */
    {
        u8  c;
        u32 v;

        /* 0x00759a41..0x00759a60 : first getter */
        c = RDB(0x12);
        frame.state = 0;              /* 0x00759a55 mov [esp+0x4550],ebx (ebx=0) */
        WRB(0x12, (u8)(c + 1));       /* 0x00759a5c, before the call */
        v = (u32)cm_fn_00785830(PTR(arg1), (u32)c);
        WR32(0x14, v);

#define GET(off)                                                        \
        do {                                                            \
            c = RDB(0x12);                                              \
            WRB(0x12, (u8)(c + 1));                                    \
            v = (u32)cm_fn_00785830(PTR(arg1), (u32)c);                 \
            WR32((off), v);                                             \
        } while (0)

        GET(0x28);   /* getter 2  */
        GET(0x18);   /* getter 3  */
        GET(0x20);   /* getter 4  */
        GET(0x40);   /* getter 5  */
        GET(0x34);   /* getter 6  */
        GET(0x48);   /* getter 7  */
        GET(0x50);   /* getter 8  */
        GET(0x30);   /* getter 9  */
        /* getter 10 stays in EDI */
        {
            c = RDB(0x12);
            WRB(0x12, (u8)(c + 1));
            g10 = (u32)cm_fn_00785830(PTR(arg1), (u32)c);
        }
        GET(0x60);   /* getter 11 */
        GET(0x1c);   /* getter 12 */
        GET(0x3c);   /* getter 13 */
        GET(0x58);   /* getter 14 */
        GET(0x38);   /* getter 15 */
        GET(0x5c);   /* getter 16 */
        GET(0x2c);   /* getter 17 */
        GET(0x54);   /* getter 18 */
        GET(0x4c);   /* getter 19 */
        GET(0x44);   /* getter 20 */
        GET(0x24);   /* getter 21 */
#undef GET
    }

    /* 0x00759c47..0x00759c6d */
    {
        u32 dat = DAT_00ae23b4;
        u32 g11;

        ebp_addr = dat + g10 * 0x245u;            /* lea ebp,[dat+edi*0x245] */
        ebx_addr = 0;
        g11 = RD32(0x60);
        if (g11 != 0xffffffffu) {
            ebx_addr = dat + g11 * 0x245u;
        }
    }

    /* 0x00759c70..0x00759c80 */
    cm_fn_007e4fa0(PTR(arg1), (void *)SB(0x12), (void *)SB(0x64));

    {
        u32 a2 = arg2;                 /* 0x00759c80 mov eax,[esp+0x4564] */
        u32 g01;

        g01 = RD32(0x14);              /* 0x00759c87 mov esi,[esp+0x20] */

        if (a2 == 0) {
            /* ---------------- arg2 == 0 branch (0x0075a25b) ---------------- */
            u32 g02 = RD32(0x28);
            u32 g03 = RD32(0x18);
            u32 g04 = RD32(0x20);

            cm_fn_0053e140(g02, g03, g04, (u32)(unsigned long)SB(0x84), 0x190, 5, g01);

            if (RD32(0x30) != 0) {
                cm_fn_0066f4e0((void *)(unsigned long)SB(0x31bc),
                               (const char *)0x00a56ae4,
                               (u32)(unsigned long)SB(0x84));
            } else {
                cm_fn_0066f4e0((void *)(unsigned long)SB(0x31bc),
                               (const char *)0x00a56aa4,
                               (u32)(unsigned long)SB(0x84));
            }

            cm_fn_004ae660(PTR(arg3), (u32)(unsigned long)SB(0x31bc));

            {
                u32 eax = g01;
                u32 ecx = eax + eax * 4u;      /* 5*g01 */
                u32 esi;

                eax = eax + ecx * 2u;          /* 11*g01 */
                ecx = DAT_00ae23bc;
                esi = eax + eax * 4u;          /* 55*g01 */
                esi = esi << 1;                /* 110*g01 */
                if (cm_fn_0053e990(esi + ecx, g02, g03, g04, 0) != 0) {
                    u32 dat2 = DAT_00ae23bc;
                    u32 p = esi + dat2;
                    cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)SB(0x84),
                                   0x7d7, (int)p);
                }
            }
        } else {
            /* ---------------- arg2 != 0 branch (0x00759c97) ---------------- */
            u32 g02 = RD32(0x28);
            u32 g03 = RD32(0x18);
            u32 g04 = RD32(0x20);
            u32 g05, g06, g07, g08, g12, g13, g14, g15, g16, g17, g18, g19, g20, g21;
            u32 eax, ecx, edx, esi;
            s8  a, b;
            u8  r;
            u32 cmpv, other;

            cm_fn_0053e140(g02, g03, g04, (u32)(unsigned long)SB(0x84), 0x190, 1, g01);

            /* 0x00759cba..0x00759cd5 */
            cm_fn_0053e140(g02, RD32(0x18), RD32(0x20),
                           (u32)(unsigned long)SB(0x78c), 0x190, 5, g01);

            /* 0x00759cda..0x00759d03 */
            g05 = RD32(0x40);
            g08 = RD32(0x50);
            g07 = RD32(0x48);
            g06 = RD32(0x34);
            cm_fn_0053e140(g06, g07, g08, (u32)(unsigned long)SB(0x91c), 0x190, 1, g05);

            /* 0x00759d08..0x00759d34 */
            g13 = RD32(0x3c);
            g16 = RD32(0x5c);
            g15 = RD32(0x38);
            g14 = RD32(0x58);
            cm_fn_0053e140(g14, g15, g16, (u32)(unsigned long)SB(0x46c), 0x190, 5, g13);

            /* 0x00759d39..0x00759d5f */
            g17 = RD32(0x2c);
            g20 = RD32(0x44);
            g19 = RD32(0x4c);
            g18 = RD32(0x54);
            cm_fn_0053e140(g18, g19, g20, (u32)(unsigned long)SB(0x5fc), 0x190, 5, g17);

            /* 0x00759d64..0x00759daa: 0x90c310(g12,-1), strlen+copy to B+0x214 */
            g12 = RD32(0x1c);
            {
                u8 *src = (u8 *)cm_fn_0090c310(g12, (char)-1);
                u32 n = 0xffffffffu;
                u8 *sp = src;

                while (n != 0) {
                    n--;
                    if (*sp++ == 0) break;
                }
                n = ~n;
                sp -= n;
                {
                    u8 *dp = SB(0x214);
                    for (i = 0; i < n; i++) dp[i] = sp[i];
                }
            }

            {
                u32 Y = *(volatile u32 *)(void *)SB(0x75);
                u32 X = *(volatile u32 *)PTR(ebp_addr);
                a = (s8)RDB(0x7d);            /* mov al,[esp+0x7d] */
                b = (s8)RDB(0x7e);            /* mov cl,[esp+0x7e] */

                if (Y == X) {
                    if ((s32)a > (s32)b) {
                        WRB(0x13, 1);
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x278),
                                       (const char *)0x009c116c,
                                       (s32)a, (s32)b);
                    } else if ((s32)a == (s32)b) {
                        WRB(0x13, 0);
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x278),
                                       (const char *)0x009c116c,
                                       (s32)a, (s32)b);
                    } else {
                        WRB(0x13, 0xff);
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x278),
                                       (const char *)0x009c116c,
                                       (s32)b, (s32)a);
                    }
                } else {
                    if ((s32)a > (s32)b) {
                        WRB(0x13, 0);
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x278),
                                       (const char *)0x009c116c,
                                       (s32)b, (s32)a);
                    } else if ((s32)a == (s32)b) {
                        WRB(0x13, 0);
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x278),
                                       (const char *)0x009c116c,
                                       (s32)a, (s32)b);
                    } else {
                        WRB(0x13, 1);
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x278),
                                       (const char *)0x009c116c,
                                       (s32)a, (s32)b);
                    }
                }
                r = RDB(0x13);
            }

            /* 0x00759dfa..0x00759f34: second formatter, after loading getter #9 */
            if (RD32(0x30) != 0) {
                if (r == 0) {
                    s32 eb37 = (s32)*(volatile s8 *)PTR(ebx_addr + 0x37);
                    s32 ep37 = (s32)*(volatile s8 *)PTR(ebp_addr + 0x37);
                    cm_fn_0066f4e0((void *)(unsigned long)SB(0xaac),
                                   (const char *)0x00a576a0,
                                   (u32)0x64, (u32)ep37, (u32)ep37,
                                   (u32)eb37, (u32)eb37,
                                   (u32)(unsigned long)SB(0x84),
                                   (u32)(ebp_addr + 4),
                                   (u32)(unsigned long)SB(0x278),
                                   (u32)(ebx_addr + 4),
                                   (u32)(unsigned long)SB(0x78c),
                                   RD32(0x1c));
                } else {
                    s32 ep37 = (s32)*(volatile s8 *)PTR(ebp_addr + 0x37);
                    s32 eb37 = (s32)*(volatile s8 *)PTR(ebx_addr + 0x37);
                    cm_fn_0066f4e0((void *)(unsigned long)SB(0xaac),
                                   (const char *)((r == 1) ? 0x00a57504 : 0x00a5736c),
                                   (u32)0x64, (u32)ep37, (u32)ep37,
                                   (u32)eb37, (u32)eb37,
                                   (u32)(unsigned long)SB(0x84),
                                   (u32)(ebp_addr + 4),
                                   (u32)(unsigned long)SB(0x278),
                                   (u32)(ebx_addr + 4),
                                   (u32)(unsigned long)SB(0x78c),
                                   RD32(0x1c));
                }
            } else {
                if (r == 0) {
                    s32 eb37 = (s32)*(volatile s8 *)PTR(ebx_addr + 0x37);
                    s32 ep37 = (s32)*(volatile s8 *)PTR(ebp_addr + 0x37);
                    cm_fn_0066f4e0((void *)(unsigned long)SB(0xaac),
                                   (const char *)0x00a5719c,
                                   (u32)ep37, (u32)ep37,
                                   (u32)eb37, (u32)eb37,
                                   (u32)(unsigned long)SB(0x84),
                                   (u32)(unsigned long)SB(0x214),
                                   (u32)(ebp_addr + 4),
                                   (u32)(unsigned long)SB(0x278),
                                   (u32)(ebx_addr + 4),
                                   (u32)(unsigned long)SB(0x5fc),
                                   (u32)(unsigned long)SB(0x46c));
                } else {
                    s32 ep37 = (s32)*(volatile s8 *)PTR(ebp_addr + 0x37);
                    s32 eb37 = (s32)*(volatile s8 *)PTR(ebx_addr + 0x37);
                    cm_fn_0066f4e0((void *)(unsigned long)SB(0xaac),
                                   (const char *)((r == 1) ? 0x00a56fcc : 0x00a56dfc),
                                   (u32)ep37, (u32)ep37,
                                   (u32)eb37, (u32)eb37,
                                   (u32)(unsigned long)SB(0x84),
                                   (u32)(unsigned long)SB(0x214),
                                   (u32)(ebp_addr + 4),
                                   (u32)(unsigned long)SB(0x278),
                                   (u32)(ebx_addr + 4),
                                   (u32)(unsigned long)SB(0x5fc),
                                   (u32)(unsigned long)SB(0x46c));
                }
            }

            /* 0x00759f39..0x0075a070: getter #21 selects; B+0x1e34 only written
               for selectors 1/2/3, otherwise it is passed uninitialized. */
            g21 = RD32(0x24);
            if (g21 == 0) {
                /* 0x0075a072: no explicit B+0x1e34 initialization */
                cm_fn_00944c9f((u32)(unsigned long)SB(0x31bc),
                               0x00991944u,
                               (u32)(unsigned long)SB(0xaac));
            } else {
                {
                    /* Listing VAs 0x00759f48, 0x00759f5f, 0x00759f6b:
                       preserve index, base, then table volatile read order. */
                    s32 idx = (s32)DAT_00b77c7a;
                    cmpv = (u32)DAT_00ae23bc + g01 * 0x6e;
                    other = *(volatile u32 *)PTR(0x00b74c26u + (u32)idx * 0x300u);
                }
                if (cmpv == other) {
                    switch (g21) {
                    case 1:
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x1e34),
                                       (const char *)0x00a56cf8,
                                       (u32)0x64, (u32)(unsigned long)SB(0x84));
                        break;
                    case 2:
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x1e34),
                                       (const char *)0x00a56d68,
                                       (u32)0x64, (u32)(unsigned long)SB(0x84));
                        break;
                    case 3:
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x1e34),
                                       (const char *)0x00a56dd4);
                        break;
                    default:
                        break;
                    }
                } else {
                    switch (g21) {
                    case 1:
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x1e34),
                                       (const char *)0x00a56b14,
                                       (u32)0x64, (u32)0x64,
                                       (u32)(unsigned long)SB(0x46c),
                                       (u32)(unsigned long)SB(0x84));
                        break;
                    case 2:
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x1e34),
                                       (const char *)0x00a56bb8,
                                       (u32)0x64, (u32)0x64,
                                       (u32)(unsigned long)SB(0x46c),
                                       (u32)(unsigned long)SB(0x84));
                        break;
                    case 3: {
                        s32 ep37 = (s32)*(volatile s8 *)PTR(ebp_addr + 0x37);
                        cm_fn_0066f4e0((void *)(unsigned long)SB(0x1e34),
                                       (const char *)0x00a56c58,
                                       (u32)ep37, (u32)ep37,
                                       (u32)(ebp_addr + 0x38),
                                       (u32)(unsigned long)SB(0x46c));
                        break;
                    }
                    default:
                        break;
                    }
                }
                cm_fn_00944c9f((u32)(unsigned long)SB(0x31bc),
                               0x00991948u,
                               (u32)(unsigned long)SB(0xaac),
                               (u32)(unsigned long)SB(0x1e34));
            }

            /* 0x0075a08f: 4ae660(arg3, B+0x31bc) */
            cm_fn_004ae660(PTR(arg3), (u32)(unsigned long)SB(0x31bc));

            /* 0x0075a0a3..0x0075a0e8: four 4ae8a0 calls, ECX = arg3 */
            {
                u32 ebx_old = ebx_addr;

                /* call 1: p2 = ebx+0x38, p4 = ebx */
                cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)(ebx_addr + 0x38),
                               0x7d5, (int)ebx_addr);
                /* call 2: p2 = esi (= ebx+4), p4 = ebx(old) */
                cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)(ebx_old + 4),
                               0x7d5, (int)ebx_old);
                /* call 3: p2 = ebp+0x38, p4 = ebp */
                cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)(ebp_addr + 0x38),
                               0x7d5, (int)ebp_addr);
                /* call 4: p2 = edi (= ebp+4), p4 = ebp */
                cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)(ebp_addr + 4),
                               0x7d5, (int)ebp_addr);
            }

            /* 0x0075a0ed..0x0075a256: three 53e990 gates */
            {
                u32 p, dat2;

                eax = RD32(0x14);                 /* g01 */
                edx = RD32(0x20);                 /* g04 */
                ecx = eax + eax * 4u;             /* 5*g01 */
                dat2 = DAT_00ae23bc;
                eax = eax + ecx * 2u;             /* 11*g01 */
                ecx = RD32(0x28);                 /* g02 */
                esi = eax + eax * 4u;             /* 55*g01 */
                eax = RD32(0x18);                 /* g03 */
                esi = esi << 1;                   /* 110*g01 */
                p = esi + dat2;
                if (cm_fn_0053e990(p, ecx, eax, edx, 0) != 0) {
                    u32 d1 = DAT_00ae23bc;
                    u32 q = esi + d1;
                    cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)SB(0x84),
                                   0x7d7, (int)q);
                    {
                        u32 d2 = DAT_00ae23bc;
                        u32 q2 = esi + d2;
                        cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)SB(0x78c),
                                       0x7d7, (int)q2);
                    }
                }
            }
            {
                u32 p, dat2;

                eax = RD32(0x3c);                 /* g13 */
                edx = RD32(0x5c);                 /* g16 */
                ecx = eax + eax * 4u;
                dat2 = DAT_00ae23bc;
                eax = eax + ecx * 2u;             /* 11*g13 */
                ecx = RD32(0x58);                 /* g14 */
                esi = eax + eax * 4u;
                eax = RD32(0x38);                 /* g15 */
                esi = esi << 1;                   /* 110*g13 */
                p = esi + dat2;
                if (cm_fn_0053e990(p, ecx, eax, edx, 0) != 0) {
                    u32 d1 = DAT_00ae23bc;
                    u32 q = esi + d1;
                    cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)SB(0x46c),
                                   0x7d7, (int)q);
                }
            }
            {
                u32 p, dat2;

                eax = RD32(0x40);                 /* g05 */
                edx = RD32(0x50);                 /* g08 */
                ecx = eax + eax * 4u;
                dat2 = DAT_00ae23bc;
                eax = eax + ecx * 2u;
                ecx = RD32(0x34);                 /* g06 */
                esi = eax + eax * 4u;
                eax = RD32(0x48);                 /* g07 */
                esi = esi << 1;                   /* 110*g05 */
                p = esi + dat2;
                if (cm_fn_0053e990(p, ecx, eax, edx, 0) != 0) {
                    u32 d1 = DAT_00ae23bc;
                    u32 q = esi + d1;
                    cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)SB(0x91c),
                                   0x7d7, (int)q);
                }
            }
            {
                u32 p, dat2;

                eax = RD32(0x2c);                 /* g17 */
                edx = RD32(0x44);                 /* g20 */
                ecx = eax + eax * 4u;
                dat2 = DAT_00ae23bc;
                eax = eax + ecx * 2u;
                ecx = RD32(0x54);                 /* g18 */
                esi = eax + eax * 4u;
                eax = RD32(0x4c);                 /* g19 */
                esi = esi << 1;                   /* 110*g17 */
                p = esi + dat2;
                if (cm_fn_0053e990(p, ecx, eax, edx, 0) != 0) {
                    u32 d1 = DAT_00ae23bc;
                    u32 q = esi + d1;
                    cm_fn_004ae8a0((int)arg3, (u8 *)(unsigned long)SB(0x5fc),
                                   0x7d7, (int)q);
                }
            }
        }
    }

    /* 0x0075a318: epilogue */
    CM_FS(void *, 0) = frame.prev;
    return;
}
