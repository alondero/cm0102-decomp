/* fn_00683210 - plain C translation of the pinned original routine at
 * preferred-image VA 0x00683210 (image base 0x00400000).
 *
 * HELD-note resolution (the only indirect call in the body):
 *   The indirect call goes through the original global function slot at
 *   0x00b67a00.  Evidence taken from the pinned original's own run: the read
 *   at 0x0068323f of 0x00b67a00 returns 0, i.e. the slot of the pinned image
 *   is NULL, so the ordinary input reaches the plain `ret` without ever
 *   entering the indirect call.  The call is nevertheless reproduced exactly
 *   as the original performs it, so no stand-in is fabricated and the
 *   non-NULL case keeps the original semantics:
 *     - the slot is read as a volatile DWORD (0x0068323f) and tested for NULL
 *       (0x00683244/0x00683246);
 *     - the call passes two stack arguments pushed right to left
 *       (push edi; push esi -> the callee sees (param_1, param_2)), cdecl
 *       cleanup by the caller (add esp, 8), no ECX receiver is set up, so the
 *       callee ABI is plain cdecl with the result in EAX;
 *     - the EAX result is tested (0x0068324f/0x00683251) and, when non-zero,
 *       the slot is RE-READ from the global (0x00683255) before the second
 *       call, exactly like the original;
 *     - after a callback that returned zero the original reloads the object
 *       pointer global (0x00683264), which is kept here too.
 *   Receiver: none.  ECX is never loaded before the indirect call; the
 *   prologue `push ecx` only saves it, and that slot is later overwritten by
 *   `mov dword ptr [esp + 0x10], ebp` (0x00683225), so the original leaves
 *   ECX clobbered at return - the callee is not thiscall and not fastcall.
 *   The slot value read from the pinned original is the one that is called;
 *   no random corpus was narrowed and no input was dropped.
 *
 * Conventions recovered from the listing: cdecl (plain `ret`, caller cleans
 * two stack arguments), 32-bit signed result in EAX on every exit path.
 */

#include "cmre.h"

/* Original globals (preferred-image VAs). */
#define DAT_00b679fc CM_GLOBAL(volatile u32, 0x00b679fc)
#define DAT_00b67a00 CM_GLOBAL(volatile u32, 0x00b67a00)

/* DWORD target table read by the original dispatch at 0x006832f2. */
#define JMPTABLE_006833f0 0x006833f0u

/* The indirect slot at 0x00b67a00 is called with two pointer arguments. */
typedef int (*cm_slot_t)(int *, int *);

int cm_fn_00683210(int *param_1, int *param_2)
{
    u32 ebx;        /* ebx: object pointer taken from 0x00b679fc */
    u32 local_ad;   /* stack slot [esp+0x10]: the ebp value read at 0x0068321e */
    u32 ecx;        /* ecx: ebp*14 index base */
    u32 idx;        /* ebp: loop counter */
    u32 sel;        /* al: dispatch byte */
    u32 sel_idx;    /* eax: sel-1 */
    u32 jmp_target; /* DWORD read from the target table */
    u32 slot;       /* indirect function pointer from 0x00b67a00 */
    u32 s1, s2;     /* *(u32 *)param_1 / *(u32 *)param_2 */
    u32 ra, rb;     /* loaded DWORDs */
    u32 diff;       /* 32-bit wrapped difference */
    u16 ax, cx16, dx16;
    s16 w_dx, w_ax; /* dx/ax at 0x00683275/0x0068327a */
    s32 a, b, d, e; /* sign extended values */
    u8 *p1;
    u8 *p2;
    int r;

    p1 = (u8 *)param_1;
    p2 = (u8 *)param_2;

    ebx = DAT_00b679fc;                                /* 00683212 */
    local_ad = *(volatile u32 *)(ebx + 0xad);          /* 0068321e */
    /* 00683225: mov dword ptr [esp + 0x10], ebp  (kept in local_ad) */

    if (*(volatile u32 *)param_1 == 0) return 0;       /* 00683229 / 0068322c */
    if (*(volatile u32 *)param_2 == 0) return 0;       /* 00683236 / 00683239 */

    slot = DAT_00b67a00;                               /* 0068323f */
    if (slot != 0) {                                   /* 00683244 / 00683246 */
        r = ((cm_slot_t)(unsigned long)slot)(param_1, param_2); /* 00683248/4a */
        if (r != 0) {                                 /* 0068324f / 00683251 */
            slot = DAT_00b67a00;                      /* 00683255: slot re-read */
            return ((cm_slot_t)(unsigned long)slot)(param_1, param_2);
        }
        ebx = DAT_00b679fc;                           /* 00683264 */
    }

    /* 0068326a lea ecx,[ebp*8] / 00683271 sub ecx,ebp / 00683273 shl ecx,1 */
    ecx = local_ad * 14u;

    w_dx = *(volatile s16 *)(p1 + ecx + 0x19);        /* 00683275 */
    w_ax = *(volatile s16 *)(p2 + ecx + 0x19);        /* 0068327a */
    if (w_dx != w_ax) {                               /* 0068327f / 00683282 */
        return (int)w_ax - (int)w_dx;                 /* 00683284..00683291 */
    }

    if (*(volatile u8 *)(p1 + ecx + 0x11) != 0)      /* 00683292 / 00683298 */
        goto cmp_loop;
    if (*(volatile u8 *)(p2 + ecx + 0x11) != 0)      /* 0068329a / 006832a0 */
        goto cmp_loop;
    goto fallback;                                    /* 006832a2 */

cmp_loop:
    for (idx = 0u; idx < 4u; idx++) {                 /* 006832c2, 00683367..6b */
        if (local_ad == 0u) {                          /* 006832c4/c8/ca */
            if (*(volatile u8 *)(ebx + idx + 0xc5) == 6u) {   /* 006832cc */
                d = (s32)*(volatile s8 *)(p1 + 4);    /* 006833c6 */
                e = (s32)*(volatile s8 *)(p2 + 4);    /* 006833ca */
                return (int)(d - e);                  /* 006833d1 */
            }
        }
        sel = *(volatile u8 *)(ebx + idx + 0xc5);      /* 006832da */
        if (sel == 0u) break;                         /* 006832e1/e3 -> 00683371 */
        sel_idx = (u32)((s32)(s8)sel - 1);            /* 006832e9 / 006832ec */
        if (sel_idx > 4u) continue;                   /* 006832ed/f0 -> 00683367 */
        jmp_target = CM_GLOBAL(volatile u32,
                               JMPTABLE_006833f0 + 4u * sel_idx); /* 006832f2 */
        switch (jmp_target) {
        case 0x006832f9u:                             /* sel == 1 */
            a = (s32)*(volatile s16 *)(p1 + ecx + 0x17);   /* 006832f9 */
            d = (s32)*(volatile s16 *)(p1 + ecx + 0x15);   /* 006832fe */
            b = (s32)*(volatile s16 *)(p2 + ecx + 0x17);   /* 00683303 */
            d = d - a;                                     /* 00683308 */
            e = (s32)*(volatile s16 *)(p2 + ecx + 0x15);   /* 0068330a */
            e = e - b;                                     /* 0068330f */
            if (d != e) return (int)(e - d);                /* 00683311/13 -> 006833de */
            ebx = DAT_00b679fc;                            /* 00683319 */
            break;
        case 0x00683321u:                             /* sel == 2 */
            d = (s32)*(volatile s16 *)(p1 + ecx + 0x15);   /* 00683321 */
            e = (s32)*(volatile s16 *)(p2 + ecx + 0x15);   /* 00683326 */
            if (d != e) return (int)(e - d);                /* 0068332b/2d -> 006833de */
            break;
        case 0x00683347u:                             /* sel == 3 */
            d = (s32)*(volatile s8 *)(p1 + ecx + 0x12);    /* 00683347 */
            e = (s32)*(volatile s8 *)(p2 + ecx + 0x12);    /* 0068334c */
            if (d != e) return (int)(e - d);                /* 00683351/53 -> 006833de */
            break;
        case 0x0068335bu:                             /* sel == 4 */
            d = (s32)*(volatile s8 *)(p1 + 4);             /* 0068335b */
            e = (s32)*(volatile s8 *)(p2 + 4);             /* 0068335f */
            if (e - d != 0) {                             /* 00683363/65 -> 006833d6 */
                d = (s32)*(volatile s8 *)(p1 + 4);         /* 006833d6 */
                e = (s32)*(volatile s8 *)(p2 + 4);         /* 006833da */
                return (int)(e - d);                      /* 006833e1 */
            }
            break;
        case 0x00683335u:                             /* sel == 5 */
            d = (s32)*(volatile s16 *)(p1 + 0x31);        /* 00683335 */
            e = (s32)*(volatile s16 *)(p2 + 0x31);        /* 00683339 */
            if (d != e) return (int)(e - d);                /* 0068333d/3f -> 006833de */
            break;
        default:
            break;
        }
    }

    /* 00683371 */
    if (local_ad == 0u) {                              /* 00683375/00683377 */
        cx16 = (u16)(s16)*(volatile s8 *)(p1 + 0x11);   /* 00683379 */
        ax = *(volatile u16 *)(ebx + 0xa7);            /* 0068337e */
        if (cx16 != ax) goto fallback;                 /* 00683385/00683388 */
        dx16 = (u16)(s16)*(volatile s8 *)(p2 + 0x11);   /* 0068338a */
        if (dx16 != ax) goto fallback;                 /* 0068338f/00683392 */
        s2 = *(volatile u32 *)p2;                      /* 00683394 */
        s2 = *(volatile u32 *)(s2 + 0x69);             /* 00683396 */
        if (s2 == 0u) goto fallback;                   /* 00683399/0068339b */
        s1 = *(volatile u32 *)p1;                      /* 0068339d */
        s1 = *(volatile u32 *)(s1 + 0x69);             /* 0068339f */
        if (s1 == 0u) goto fallback;                   /* 006833a2/006833a4 */
        ra = *(volatile u32 *)s2;                      /* 006833a6 */
        rb = *(volatile u32 *)s1;                      /* 006833a8 */
        diff = ra - rb;                               /* 006833aa */
        if (diff != 0u) return (int)diff;             /* 006833ac -> 006833e8 */
    }

fallback:
    s1 = *(volatile u32 *)p1;                          /* 006832a2 / 006833ae */
    if (s1 == 0u) return 0;                            /* 006832a4/a6, 006833b0/b2 */
    s2 = *(volatile u32 *)p2;                          /* 006832ac / 006833b4 */
    if (s2 == 0u) return 0;                            /* 006832ae/b0, 006833b6/b8 */
    ra = *(volatile u32 *)s2;                          /* 006832b6 / 006833ba */
    rb = *(volatile u32 *)s1;                          /* 006832b8 / 006833bc */
    /* 006832bd / 006833c1: sub eax, ecx -> eax = [arg1 inner] - [arg2 inner] */
    return (int)(rb - ra);
}
