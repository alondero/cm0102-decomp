/* cm_fn_006f5810 -- recovered from the original instructions at VA 0x006f5810
   (preferred image base 0x00400000, CM0102 3.9.68).  `ret 4`, ECX read before
   written -> thiscall receiver; one dword stack argument whose home is the
   int32 scratch the original reuses as [esp+0x10].

   Boundaries kept explicit (they bind to original code).  Receiver and stack
   order are taken from the listing at each call site (`mov ecx, X` directly
   before the call, then the pushes), which overrides the earlier callee map:
     0x0062c3d0  0x006f5837  mov ecx, dword [0xb63d28]; push index; call
     0x006c38c0  0x006f588a  mov ecx, dword [esi+0x19e]; push esi; call
     0x0062c590  0x006f59a4  mov ecx, dword [0xb63d28]; push 0; push [esi+0x69]

   UNRESOLVED BOUNDARY -- x87-status-word-return (evidence-needed).
   At 0x006f582b the early exit returns EAX=0. On the normal tail,
   0x006f5aff FNSTSW AX replaces the low word with the actual x87 status,
   including exception/sticky and C1 bits, retaining the last EAX upper word.
   No subsequent instruction through RET4 at 0x006f5b11 changes EAX.
   The provisional void declaration below DOES NOT recover this projection.
   Full transitive caller non-observation or evidenced plain-C status transport
   remains required; passing the current gate does not resolve this hold. */

#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0062c3d0(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_0062c590(u32 self, u32 person,
                                                     u32 optional);
extern u32 CM_THISCALL cm_fn_006c38c0(u32 self, u32 object);

void CM_THISCALL cm_fn_006f5810(u32 self, u32 arg_word)
{
    u8 *const obj = (u8 *)(unsigned long)self;
    /* [esp+0x10] is both the caller-owned argument slot and the int32 scratch
       the listing fills before each FILD. */
    volatile u32 *const home = &arg_word;
    /* qword home at [esp+4]: initialised to 1.0 (0x3ff00000_00000000) by the
       two dword stores at 0x006f5816/0x006f581e and written by FSTP. */
    volatile union {
        double value;
        s32 scratch;
    } k_slot;
    u32 eax_res;
    u8 al;

    k_slot.value = 1.0;

    /* 0x006f5826 */
    if (*(volatile u32 *)(obj + 0x6d) == 0)
        return;

    /* 0x006f5831: the index at [p69] is read before the receiver global */
    {
        u32 p69 = *(volatile u32 *)(obj + 0x69);
        u32 idx = *(volatile u32 *)(unsigned long)p69;
        eax_res = cm_fn_0062c3d0(CM_GLOBAL(u32, 0x0b63d28), idx);
    }

    /* 0x006f5842: signed word compare against 0x3e8 */
    if ((s16)eax_res > 0x3e8) {
        u32 p69 = *(volatile u32 *)(obj + 0x69);   /* 0x006f5848 reload */
        u32 recv = CM_GLOBAL(u32, 0x0b63d28);      /* 0x006f584b receiver */
        u32 idx;
        long double t;
        idx = *(volatile u32 *)(unsigned long)p69; /* 0x006f5851 index */
        eax_res = cm_fn_0062c3d0(recv, idx);
        /* [esp+4] is E0-8, NOT the argument home at E0+4.
           FILD consumes the low dword of the same local qword that FSTP
           overwrites below; the caller's byte remains intact at 0x006f5874. */
        k_slot.scratch = (s32)(s16)eax_res;        /* 0x006f5859/585c */
        t = (long double)k_slot.scratch;           /* 0x006f5860 fild */
        t = t * (long double)CM_GLOBAL(double, 0x0096a608);  /* fmul qword */
        t = (long double)CM_GLOBAL(double, 0x00968dc0) - t;  /* fsubr qword */
        k_slot.value = (double)t;                  /* 0x006f5870 fstp qword */
    }

    /* 0x006f5874: low byte of the argument home */
    al = *(volatile u8 *)((volatile u8 *)home);

    if (al == 0) {
        /* 0x006f5880 */
        u32 ecx19e = *(volatile u32 *)(obj + 0x19e);
        if (ecx19e != 0) {
            u32 r3 = cm_fn_006c38c0(ecx19e, self);
            if ((s16)r3 == 1) {
                /* 0x006f5896 */
                u32 p = *(volatile u32 *)(obj + 0x6d);
                s32 f = (s32)*(volatile s8 *)(p + 0x34);
                u32 e = (u32)f + 0x0fu;
                long double t;
                e = e + e * 4u;                    /* lea eax,[eax+eax*4] */
                e = e << 1;                        /* shl eax,1 */
                *home = e;                         /* 0x006f58a5 */
                t = (long double)(s32)(*home);     /* 0x006f58a9 fild */
                t = t * (long double)k_slot.value; /* fmul qword [esp+4] */
                t = t + (long double)*(volatile float *)(obj + 0x4d);
                /* 0x006f58b4: FST dword, the compare keeps the extended value */
                *(volatile float *)(obj + 0x4d) = (float)t;
                if (t > (long double)CM_GLOBAL(double, 0x00968910))
                    *(volatile u32 *)(obj + 0x4d) = 0x461c4000u;
                goto tail;
            }
        }

        /* 0x006f58d4 */
        if ((s8)*(volatile u8 *)(obj + 0x19) < 0)
            goto tail;
        {
            u8 b1f = *(volatile u8 *)(obj + 0x1f);          /* 0x006f58df */
            u32 p = *(volatile u32 *)(obj + 0x6d);          /* 0x006f58e2 */
            s32 f = (s32)*(volatile s8 *)(p + 0x34);        /* 0x006f58e7 */
            long double t;
            if (b1f == 0xff) {
                u32 e = (u32)f * 3u;
                *home = e * 4u + 0x145u;                    /* 0x006f58f7 */
            } else {
                u32 e = (u32)f + 0x0fu;
                *home = (e * 5u) << 1;                      /* 0x006f5909 */
            }
            t = (long double)(s32)(*home);                  /* fild */
            t = t * (long double)k_slot.value;              /* 0x006f5911 */
            t = t + (long double)*(volatile float *)(obj + 0x4d);
            *(volatile float *)(obj + 0x4d) = (float)t;     /* fstp dword */
            t = (long double)*(volatile float *)(obj + 0x4d); /* fld dword */
            if (t > (long double)CM_GLOBAL(double, 0x00968910))
                *(volatile u32 *)(obj + 0x4d) = 0x461c4000u;
            goto tail;
        }
    }

    /* ---- argument byte nonzero ---- */
    {
        u8 c20 = *(volatile u8 *)(obj + 0x20);      /* 0x006f5926 */
        s8 c = (s8)c20;

        if (c >= 0) {
            if (c <= 0x2d || c >= 0x41) {
                /* 0x006f596b: plain scaled index */
                u32 p = *(volatile u32 *)(obj + 0x6d);
                s32 f = (s32)*(volatile s8 *)(p + 0x34);
                u32 three = (u32)f * 3u;                     /* lea edx,[eax+eax*2] */
                u32 n = three * 4u + 0x55u;                  /* lea eax,[edx*4+0x55] */
                u8 b1c;
                long double t;
                *home = n;
                t = (long double)(s32)(*home);               /* 0x006f5980 fild */
                t = t * (long double)k_slot.value;           /* 0x006f5984 */
                b1c = *(volatile u8 *)(obj + 0x1c);          /* 0x006f5988 */
                t = t + (long double)*(volatile float *)(obj + 0x4d);
                *(volatile float *)(obj + 0x51) = (float)t;  /* 0x006f5990 */
                if (b1c != 0)
                    goto tail;
                goto do_1964;
            } else {
                /* 0x006f5939: letter branch */
                s32 cv = (s32)c;
                u32 p = *(volatile u32 *)(obj + 0x6d);
                s32 f;
                u32 three, n;
                u8 b1c;
                long double t;
                *home = (u32)cv;                             /* 0x006f593f */
                f = (s32)*(volatile s8 *)(p + 0x34);         /* 0x006f5943 */
                t = (long double)(s32)(*home);               /* 0x006f5947 fild */
                t = t * (long double)CM_GLOBAL(double, 0x0096d798); /* 0x006f594b */
                t = t - (long double)CM_GLOBAL(double, 0x0096d790); /* 0x006f5954 */
                three = (u32)f * 3u;                         /* lea edx,[eax+eax*2] */
                n = three * 4u + 0x55u;                      /* lea eax,[edx*4+0x55] */
                *home = n;                                   /* 0x006f5961 */
                t = t * (long double)(s32)(*home);           /* fimul dword */
                t = t * (long double)k_slot.value;           /* 0x006f5984 */
                b1c = *(volatile u8 *)(obj + 0x1c);          /* 0x006f5988 */
                t = t + (long double)*(volatile float *)(obj + 0x4d);
                *(volatile float *)(obj + 0x51) = (float)t;  /* 0x006f5990 */
                if (b1c != 0)
                    goto tail;
                goto do_1964;
            }
        } else {
            u8 c21 = *(volatile u8 *)(obj + 0x21);  /* 0x006f59e2 */
            s8 c2 = (s8)c21;

            if (c2 >= 0) {
                if (c2 <= 0x2d || c2 >= 0x41) {
                    /* 0x006f5a27: plain scaled index */
                    u32 p = *(volatile u32 *)(obj + 0x6d);
                    s32 f = (s32)*(volatile s8 *)(p + 0x34);
                    u32 three = (u32)f * 3u;                 /* lea ecx,[eax+eax*2] */
                    u32 n = three * 4u + 0x55u;              /* lea edx,[ecx*4+0x55] */
                    u8 b1c;
                    long double t;
                    *home = n;                               /* 0x006f5a38 */
                    t = (long double)(s32)(*home);           /* 0x006f5a3c fild */
                    t = t * (long double)k_slot.value;       /* 0x006f5a40 */
                    b1c = *(volatile u8 *)(obj + 0x1c);      /* 0x006f5a44 */
                    t = t + (long double)*(volatile float *)(obj + 0x4d);
                    *(volatile float *)(obj + 0x51) = (float)t; /* 0x006f5a4c */
                    if (b1c != 0)
                        goto tail;
                    goto do_1964;
                } else {
                    /* 0x006f59f5: letter branch */
                    s32 cv = (s32)c2;
                    u32 p;
                    s32 f;
                    u32 three, n;
                    u8 b1c;
                    long double t;
                    p = *(volatile u32 *)(obj + 0x6d);       /* 0x006f59f8 */
                    *home = (u32)cv;                         /* 0x006f59fb */
                    t = (long double)(s32)(*home);           /* 0x006f59ff fild */
                    f = (s32)*(volatile s8 *)(p + 0x34);     /* 0x006f5a03 */
                    t = t * (long double)CM_GLOBAL(double, 0x0096d798);
                    three = (u32)f * 3u;                     /* lea ecx,[eax+eax*2] */
                    t = t - (long double)CM_GLOBAL(double, 0x0096d790);
                    n = three * 4u + 0x55u;                   /* lea edx,[ecx*4+0x55] */
                    *home = n;                                /* 0x006f5a1d */
                    t = t * (long double)(s32)(*home);        /* fimul dword */
                    t = t * (long double)k_slot.value;        /* 0x006f5a40 */
                    b1c = *(volatile u8 *)(obj + 0x1c);       /* 0x006f5a44 */
                    t = t + (long double)*(volatile float *)(obj + 0x4d);
                    *(volatile float *)(obj + 0x51) = (float)t; /* 0x006f5a4c */
                    if (b1c != 0)
                        goto tail;
                    goto do_1964;
                }
            } else {
                /* 0x006f5a89 */
                if ((s8)*(volatile u8 *)(obj + 0x19) < 0) {
                    *(volatile u32 *)(obj + 0x51) =
                        *(volatile u32 *)(obj + 0x4d);   /* 0x006f5aea */
                    goto tail;
                }
                {
                    u32 p = *(volatile u32 *)(obj + 0x6d);   /* 0x006f5a90 */
                    s32 f = (s32)*(volatile s8 *)(p + 0x34);
                    u32 three = (u32)f * 3u;                 /* lea edx,[eax+eax*2] */
                    u32 n = three * 4u + 0x55u;              /* lea eax,[edx*4+0x55] */
                    u8 b1c;
                    long double t;
                    *home = n;                               /* 0x006f5aa1 */
                    b1c = *(volatile u8 *)(obj + 0x1c);      /* 0x006f5aa5 */
                    t = (long double)(s32)(*home);           /* 0x006f5aa8 fild */
                    t = t * (long double)k_slot.value;       /* 0x006f5aae */
                    t = t + (long double)*(volatile float *)(obj + 0x4d);
                    *(volatile float *)(obj + 0x51) = (float)t; /* 0x006f5ab5 */
                    if (b1c != 0)
                        goto tail;
                    goto do_1964;
                }
            }
        }
    }

do_1964:
    /* 0x006f5999/0x006f5a55/0x006f5aba: word-returning query clamped to 0x1964
       and compared against the float just stored at [esi+0x51]. */
    {
        u32 p69 = *(volatile u32 *)(obj + 0x69);
        unsigned long long r;
        s32 iv;
        long double cv;
        if (p69 == 0)
            goto tail;
        r = cm_fn_0062c590(CM_GLOBAL(u32, 0x0b63d28), p69, 0);
        iv = (s32)(s16)(u16)r;
        if ((s16)(u16)r > 0x1964)
            iv = 0x1964;
        *home = (u32)iv;
        cv = (long double)(s32)(*home);                    /* 0x006f59c4 fild */
        if (cv > (long double)*(volatile float *)(obj + 0x51))
            *(volatile u32 *)(obj + 0x51) = 0x45cb2000u;
    }

tail:
    /* 0x006f5af0 */
    {
        long double t = (long double)*(volatile float *)(obj + 0x59);
        t = t + (long double)*(volatile float *)(obj + 0x51);
        /* FST dword keeps the extended sum for the comparison */
        *(volatile float *)(obj + 0x51) = (float)t;
        if (t > (long double)CM_GLOBAL(double, 0x00968910))
            *(volatile u32 *)(obj + 0x51) = 0x461c4000u;
    }
}
