/* fn_0051d2d0 (VA 0x0051d2d0, preferred image base 0x00400000, 3.9.68
   SHA-256 0c78e7da...f86), module comp/cup.cpp.
   thiscall, one 4-byte stack slot holding a 16-bit signed argument, `ret 4`,
   result unused (EAX at both RETs is the callee's free() result; declared void
   like the recovered signature).

   Fatal-path resolution (held concern): the malloc-NULL block ends at 0x51d3be
   with `push -1; call 0x945934` and falls straight into 0x51d3c3 with no
   `add esp,4`.  The original instruction stream therefore only stays
   frame-consistent if that boundary call does not return: a return would shift
   every subsequent [esp+...] scratch slot and the four saved registers by 4 and
   would corrupt `ret 4`.  That is original-instruction evidence for a
   non-returning fatal call, not a Ghidra or scaffold claim, so the translation
   stops after the boundary call instead of continuing into a frame the original
   can never reach.  No termination, cleanup or default is invented: the
   boundary call is still made with its original argument and the function just
   ends there (the only statement plain C can make about a path the original
   never continues). */

#include "cmre.h"

/* width-exact, order-exact memory accessors (the original reloads every one of
   these; a cached plain pointer would be legal under C aliasing rules) */
static u8 ld8(u32 a) { return *(volatile u8 *)a; }
static u16 ld16(u32 a) { return *(volatile u16 *)a; }
static u32 ld32(u32 a) { return *(volatile u32 *)a; }
static void st8(u32 a, u8 v) { *(volatile u8 *)a = v; }
static void st16(u32 a, u16 v) { *(volatile u16 *)a = v; }

extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);
extern char *cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3, ...);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                    void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 ptr);
extern unsigned long long CM_CDECL cm_fn_00945333(u32 base, u32 count,
                                                   u32 width, u32 compare);
extern void cm_fn_00945934(u32 param_1);

#define CM_B67A34 CM_GLOBAL(u32, 0x00b67a34)

void CM_THISCALL cm_fn_0051d2d0(u32 self, short arg)
{
    u8 buf[512]; /* L+0x2c and L+0x12c: the two adjacent 256-byte message buffers */
    u32 p;       /* [esp+0x18]: allocation (also edi across the routine) */
    s32 cnt;     /* [esp+0x1c]: record count kept in bx */
    s32 i;       /* [esp+0x34]: record loop index */
    s32 stride;  /* [esp+0x24]: 0x68 * (signed) argument */
    u32 argi;    /* ebp */
    u32 r;       /* [esp+0x14]: candidate slot index */
    u32 r0;      /* [esp+0x38]: first candidate, for the wrap test */
    u32 fmod;    /* [esp+0x10]: fill counter, later the leg flag */
    u32 fdup;    /* [esp+0x20] */
    u32 f18;     /* [esp+0x28]: object byte 0xb & 4 */
    u32 f20;     /* [esp+0x30]: second-search-pass flag */
    u32 base;    /* this->0xa3 */
    u32 adr;     /* this->0xad[arg] */
    u32 slot;    /* [esp+0x2c] */
    u32 recp;    /* edi: current 7-byte record */
    u32 a7;      /* this->0xa7 */
    u32 t;
    u32 uu;
    u16 w;
    u16 w2;
    s32 s;

    argi = (u32)(s32)arg;
    stride = (s32)arg * 0x68;
    cnt = 0;
    f20 = 0;

    /* 0x51d2e8-0x51d312: element count of this round, times 7-byte records */
    p = (u32)cm_fn_00944e46(
        (u32)((s32)(s16)ld16(ld32(self + 0xa3u) + 0x18u + (u32)stride) * 7));
    if (p == 0) {
        /* 0x51d328-0x51d36b (RAM-allocation failure report; file/line macro).
           0x51d344-0x51d370 uses one `add esp,0x1c`: the four pushes at
           0x51d34f-0x51d35a are 944c9f's arguments 4..7 (they are also
           90d130's arguments, run as the nested call for argument 3), so the
           944c9f call carries 0xbf8 and 0x9b56dc itself. */
        cm_fn_00944cff(0x009b55acu, 0u, 0u, buf + 0x100, buf);
        cm_fn_00944c9f(
            0x00ae24d0u, 0x009a8120u,
            (u32)cm_fn_0090d130((u32)(buf + 0x100), (u32)buf, 0xbf8u,
                                0x009b56dcu),
            (u32)(buf + 0x100), (u32)buf, 0xbf8u, 0x009b56dcu);
        cm_fn_0066f4e0((int)0x00ae28bcu, (char *)0x009a8108u);
        cm_fn_0066f4e0((int)0x00ae26c4u, (char *)0x009a7ff8u, 0x00ae24d0u);
        cm_fn_005e8290((const char *)0x00ae28bcu, (const char *)0x00ae26c4u, 0);
        CM_B67A34 = p;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
        /* see the note at the top: 0x51d3c3 has no `add esp,4` after 0x51d3be */
        return;
    }

    /* 0x51d3c3-0x51d3e8 */
    base = ld32(self + 0xa3u);
    fmod = 0;
    f18 = (u32)(ld8(base + 0xbu + (u32)stride) & 4u);

    if ((s16)ld16(base + 0x1au + (u32)stride) > 0) {
        /* 0x51d3ee-0x51d530: expand both legs of every slot into 7-byte records */
        do {
            s = (s32)(s16)fmod;
            adr = ld32(ld32(self + 0xadu) + argi * 4u);
            w = ld16(adr + (u32)(s * 0x35));
            if (w == 0xffffu) {
                recp = p + (u32)(7 * (s32)(s16)cnt);
                cnt = cnt + 1;
                st16(recp, 0xffffu);
                st8(recp + 6, 0xffu);
            } else {
                recp = p + (u32)(7 * (s32)(s16)cnt);
                cnt = cnt + 1;
                st16(recp, w);
                st16(recp + 2,
                     ld16(ld32(ld32(self + 0xadu) + argi * 4u) + 0xeu +
                          (u32)(s * 0x35)));
                st16(recp + 4,
                     ld16(ld32(ld32(self + 0xadu) + argi * 4u) + 0xau +
                          (u32)(s * 0x35)));
                a7 = ld32(self + 0xa7u);
                st8(recp + 6,
                    ld8(a7 + 4u + (u32)((s32)(s16)ld16(recp) * 6)));
                st16(ld32(ld32(self + 0xadu) + argi * 4u) + (u32)(s * 0x35),
                     0xfffeu);
            }
            /* 0x51d488-0x51d514: second leg of the same slot */
            w = ld16(ld32(ld32(self + 0xadu) + argi * 4u) + 2u +
                     (u32)(s * 0x35));
            if (w == 0xffffu) {
                recp = p + (u32)(7 * (s32)(s16)cnt);
                cnt = cnt + 1;
                st16(recp, 0xffffu);
                st8(recp + 6, 0xffu);
            } else {
                recp = p + (u32)(7 * (s32)(s16)cnt);
                cnt = cnt + 1;
                st16(recp, w);
                st16(recp + 2,
                     ld16(ld32(ld32(self + 0xadu) + argi * 4u) + 0x10u +
                          (u32)(s * 0x35)));
                st16(recp + 4,
                     ld16(ld32(ld32(self + 0xadu) + argi * 4u) + 0xcu +
                          (u32)(s * 0x35)));
                a7 = ld32(self + 0xa7u);
                st8(recp + 6,
                    ld8(a7 + 4u + (u32)((s32)(s16)ld16(recp) * 6)));
                st16(ld32(ld32(self + 0xadu) + argi * 4u) + 2u +
                         (u32)(s * 0x35),
                     0xfffeu);
            }
            /* 0x51d518-0x51d530 */
            fmod = fmod + 1;
            if ((s16)fmod >=
                (s16)ld16(ld32(self + 0xa3u) + 0x1au + (u32)stride)) {
                break;
            }
        } while (1);
    }

    /* 0x51d53a-0x51d54b: order the records (compare callback 0x0051ebc0) */
    cm_fn_00945333(p, (u32)(s32)(s16)cnt, 7u, 0x0051ebc0u);

    i = 0;
    if ((s16)cnt <= 0) {
        goto L_9cb;
    }

L_55f:
    /* 0x51d55f-0x51d575 */
    recp = p + (u32)(7 * (s32)(s16)i);
    if (ld8(recp + 6) == 0xffu) {
        goto L_9cb;
    }
    /* 0x51d57b-0x51d597 */
    r = (u32)cm_fn_0090cfc0(
        (u32)(s32)(s16)ld16(ld32(self + 0xa3u) + 0x1au + (u32)stride));
    r0 = r;

L_59b:
    /* 0x51d59b-0x51d5a9 */
    fdup = 0;
    if (f20 != 0) {
        /* 0x51d5af-0x51d6ad */
        s = (s32)(s16)r;
        slot = ld32(ld32(self + 0xadu) + argi * 4u) + (u32)(s * 0x35);
        if (ld16(slot) == 0xfffeu) {
            if (f18 != 0 && (s16)ld16(recp) >= 0 &&
                (s16)ld16(slot + 2) >= 0) {
                a7 = ld32(self + 0xa7u);
                t = ld32(a7 + (u32)((s32)(s16)ld16(recp) * 6));
                if (t != 0) {
                    uu = ld32(a7 + (u32)((s32)(s16)ld16(slot + 2) * 6));
                    if (uu != 0 && ld32(uu + 0x53u) == ld32(t + 0x53u)) {
                        goto L_62e;
                    }
                }
            }
            fmod = 1;
            fdup = 1;
        }
    L_62e:
        /* 0x51d62e-0x51d633 */
        if (ld16(slot + 2) == 0xfffeu) {
            /* 0x51d635-0x51d64b */
            if (fdup != 0) {
                t = (u32)cm_fn_0090cfc0(2u);
                if (t != 0) {
                    goto L_87d;
                }
            }
            /* 0x51d64f-0x51d6ad */
            if (f18 != 0 && (s16)ld16(recp) >= 0) {
                w = ld16(ld32(ld32(self + 0xadu) + argi * 4u) +
                         (u32)(s * 0x35));
                if ((s16)w >= 0) {
                    a7 = ld32(self + 0xa7u);
                    t = ld32(a7 + (u32)((s32)(s16)ld16(recp) * 6));
                    if (t != 0) {
                        uu = ld32(a7 + (u32)((s32)(s16)w * 6));
                        if (uu != 0 &&
                            ld32(uu + 0x53u) == ld32(t + 0x53u)) {
                            goto L_6b7;
                        }
                    }
                }
            }
            /* 0x51d76f-0x51d773 */
            fmod = 0;
            goto L_77b;
        }
        goto L_6b7;
    }

    /* 0x51d709-0x51d751: first pass, first record whose both legs are negative */
    s = (s32)(s16)r;
    slot = ld32(ld32(self + 0xadu) + argi * 4u) + (u32)(s * 0x35);
    w = ld16(slot);
    if ((s16)w >= 0) {
        goto L_6c3;
    }
    w2 = ld16(slot + 2);
    if ((s16)w2 >= 0) {
        goto L_6c3;
    }
    if (w == 0xfffeu) {
        fmod = 1;
        fdup = 1;
    }
    if (w2 == 0xfffeu) {
        /* 0x51d856-0x51d874 */
        if (fdup != 0) {
            t = (u32)cm_fn_0090cfc0(2u);
            if (t != 0) {
                goto L_87d;
            }
        }
        fmod = 0;
        goto L_77b;
    }
    /* 0x51d751 */

L_6b7:
    /* 0x51d6b7-0x51d6bd */
    if (fdup != 0) {
        goto L_87d;
    }

L_6c3:
    /* 0x51d6c3-0x51d704: advance the candidate slot, wrapping at the bound */
    r = r + 1;
    if ((s16)r >=
        (s16)ld16(ld32(self + 0xa3u) + 0x1au + (u32)stride)) {
        r = 0;
    }
    if ((u16)r0 != (u16)r) {
        goto L_59b;
    }
    if (f20 != 0) {
        /* 0x51d756-0x51d76a */
        if (f18 == 0) {
            goto L_94d;
        }
        f18 = 0;
        goto L_59b;
    }
    f20 = 1;
    goto L_59b;

L_77b:
    /* 0x51d77b-0x51d7c7: place the record into the slot's second leg */
    s = (s32)(s16)r;
    st16(ld32(ld32(self + 0xadu) + argi * 4u) + 2u + (u32)(s * 0x35),
         ld16(recp));
    st16(ld32(ld32(self + 0xadu) + argi * 4u) + 0x10u + (u32)(s * 0x35),
         ld16(recp + 2));
    st16(ld32(ld32(self + 0xadu) + argi * 4u) + 0xcu + (u32)(s * 0x35),
         ld16(recp + 4));
    if (ld16(recp + 2) == 0xffffu) {
        goto L_837;
    }
    /* 0x51d7c9-0x51d837 */
    t = ld32(ld32(self + 0xadu) +
             (u32)((s32)(s16)ld16(recp + 2) * 4));
    st16(t + 0x12u + (u32)((s32)(s16)ld16(recp + 4) * 0x35), (u16)arg);
    t = ld32(ld32(self + 0xadu) +
             (u32)((s32)(s16)ld16(recp + 2) * 4));
    st16(t + 6u + (u32)((s32)(s16)ld16(recp + 4) * 0x35), (u16)r);
    t = ld32(ld32(self + 0xadu) +
             (u32)((s32)(s16)ld16(recp + 2) * 4));
    st16(t + 8u + (u32)((s32)(s16)ld16(recp + 4) * 0x35), 0u);
    goto L_837;

L_87d:
    /* 0x51d87d-0x51d883 */
    if (fmod == 0) {
        goto L_77b;
    }
    /* 0x51d889-0x51d8d4: place the record into the slot's first leg */
    s = (s32)(s16)r;
    st16(ld32(ld32(self + 0xadu) + argi * 4u) + (u32)(s * 0x35),
         ld16(recp));
    st16(ld32(ld32(self + 0xadu) + argi * 4u) + 0xeu + (u32)(s * 0x35),
         ld16(recp + 2));
    st16(ld32(ld32(self + 0xadu) + argi * 4u) + 0xau + (u32)(s * 0x35),
         ld16(recp + 4));
    if (ld16(recp + 2) == 0xffffu) {
        goto L_837;
    }
    /* 0x51d8da-0x51d948 */
    t = ld32(ld32(self + 0xadu) +
             (u32)((s32)(s16)ld16(recp + 2) * 4));
    st16(t + 0x12u + (u32)((s32)(s16)ld16(recp + 4) * 0x35), (u16)arg);
    t = ld32(ld32(self + 0xadu) +
             (u32)((s32)(s16)ld16(recp + 2) * 4));
    st16(t + 6u + (u32)((s32)(s16)ld16(recp + 4) * 0x35), (u16)r);
    t = ld32(ld32(self + 0xadu) +
             (u32)((s32)(s16)ld16(recp + 2) * 4));
    st16(t + 8u + (u32)((s32)(s16)ld16(recp + 4) * 0x35), 1u);

L_837:
    /* 0x51d837-0x51d851 */
    i = i + 1;
    recp = p;
    if ((s16)i < (s16)cnt) {
        goto L_55f;
    }
    goto L_9cb;

L_94d:
    /* 0x51d94d-0x51d9c4: no free slot for the record.  As at 0x51d34f, the
       three pushes at 0x51d976-0x51d97c are 944c9f's arguments 4..6 and
       90d130's arguments 1..3 (nested call for argument 3); one shared
       `add esp,0x28` at 0x51d9b7 covers 3+3+3+1 dwords. */
    cm_fn_00944cff(0x009b55acu, 0u, 0u, buf, buf + 0x100);
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                   (u32)cm_fn_0090d130((u32)buf, (u32)(buf + 0x100), 0xcffu),
                   (u32)buf, (u32)(buf + 0x100), 0xcffu);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_B67A34 = 0;
    cm_fn_009452ca(p);
    return;

L_9cb:
    /* 0x51d9cb-0x51d9de */
    cm_fn_009452ca(p);
    return;
}
