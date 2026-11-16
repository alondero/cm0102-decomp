/* fn_00519d90 (module comp\cup.cpp) - faithful plain-C transcription.
   Ground truth: CM0102 3.9.68 cm0102.exe, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86, preferred image
   base 0x00400000; every VA in this file is a preferred-image VA.
   `ret 8` + ECX read before written (0x00519db7 mov esi,ecx) => int CM_THISCALL
   with the receiver first and two stack arguments. */
#include "cmre.h"

/* Callee boundaries. Argument counts come from the pushes at the call sites in
   this routine (the callee C records are navigation only):
   0x00519ed4 / 0x00519f89: 11 stack args + ECX.
   0x00519f01:              ECX + 2 stack args; frame arithmetic after the call
                            (0x00519f06 lea ecx,[esp+0x2c]) requires ret 8.
   0x00519f1c:              ECX + 2 stack args; ret 8 (state store at 0x00519f2b
                            with [esp+0x244] resolves to the same slot as
                            0x00519f4d [esp+0x23c] only if esp returns to L-0x10).
   0x00519f33:              ECX + 2 stack args; ret 8 (0x00519f38 reloads use
                            L+0x0c / L+0x0e).
   0x00519e26 / 0x00519e36 / 0x00519e46: cdecl; their 9 argument dwords are the
                            single `add esp,0x24` at 0x00519e4b.
   0x00519dd1 / 0x00519e0c: cdecl; `add esp,0x14` at 0x00519dd6 / 0x00519e11. */
extern u32 CM_THISCALL cm_fn_005ac920(u32 self, u32 arg0, u32 arg1, u32 arg2,
                                      u32 arg3, u32 arg4, u32 arg5, u32 arg6,
                                      u32 arg7, u32 arg8, u32 arg9, u32 arg10);
/* HELD boundary, not a composed callee implementation:
   00519ec1 and 00519f76 push arg3=0 (pinned image/hash above).
   This source leaves 005ac920 explicit; it does not initialize, synthesize,
   or represent that callee's stale local upper words. The supplied caller
   listing contains no 005abf10 consumption instructions, so it cannot
   establish whether those words are consumed on this route. Caller semantic
   gate success does not discharge that outstanding callee evidence hold. */
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_0054c8e0(void *self, void *param_1, u32 param_2);
extern void CM_THISCALL cm_fn_00417c10(void *self, u32 *param_1, u32 param_2);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
/* 00519e26 takes no pushed arguments: the preceding three pushes remain
   live for 00519e36, followed by its three pushes; 00519e4b cleans all nine
   DWORDs including the three belonging to 00519e46. */
extern char *CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);

int CM_THISCALL cm_fn_00519d90(int param_1, int param_2, int param_3)
{
    /* SEH registration record: pushed at 0x00519d96/0x00519d98/0x00519d9d as
       {state=0xFFFFFFFF, handler=0x00957606, prev}, linked at 0x00519d9e.
       The three later stores 0x00519f11 (dword 0), 0x00519f2b (byte 1) and
       0x00519f4d (dword 0xFFFFFFFF) all address that state slot. */
    volatile struct { void *prev; void *handler; u32 state; } frame;

    /* Single scratch allocation for the original frame region [esp] after
       `sub esp,0x224` (0x00519da5). Offsets are frame-base relative and stay
       uninitialised except where the listing stores:
         +0x03 byte  (flag; 0x00519eab, 0x00519f48, 0x00519f5a)
         +0x04 dword (0x00519ea2, 0x00519f40)
         +0x08 WORD  (0x00519ea6 - only two bytes; +0x0a/+0x0b are never
                      written by this routine, so the dword reload at
                      0x00519ef3 carries stale upper bytes exactly as the
                      original does)
         +0x08 dword (0x00519f44)
         +0x0c dword reload, +0x0e dword reload (0x00519f38/0x00519f3c)
         +0x14 8-byte object (0x00519f26)
         +0x1c 8-byte object (0x00519f0c)
         +0x24 256-byte error buffer (0x00519dbd / 0x00519df8)
         +0x124 256-byte error buffer (0x00519dc1 / 0x00519dff) */
    u8 L[0x224];

    u32 this_v;
    u32 ent, tbl, v0, v1, p_a, p_b;
    u32 g90, g92, g88;
    u32 rv_b, rv_p, s4, s4d, rv, r;
    u32 blk_a, blk_b, code, ret;
    char *msg;

    this_v = (u32)param_1;

    frame.prev = CM_FS(void *, 0);
    frame.handler = (void *)0x00957606;
    frame.state = 0xffffffffu;
    CM_FS(void *, 0) = (void *)&frame;

    if (param_2 < 0) {
        /* 0x00519dbd..0x00519dd6 */
        cm_fn_00944cff(0x009b55acu, 0, 0, (void *)(L + 0x124), (void *)(L + 0x24));
        blk_a = (u32)(L + 0x124);
        blk_b = (u32)(L + 0x24);
        code = 0x3b9u;
    }
    else if (param_3 < 0) {
        /* 0x00519df8..0x00519e11 */
        cm_fn_00944cff(0x009b55acu, 0, 0, (void *)(L + 0x24), (void *)(L + 0x124));
        blk_a = (u32)(L + 0x24);
        blk_b = (u32)(L + 0x124);
        code = 0x3c1u;
    }
    else {
        /* 0x00519e5b..0x00519e73: row pointer, then record index 53*param_3
           (lea 3p; lea 9*(3p); shl 1; sub p). */
        ent = *(u32 *)((u32)*(u32 *)(this_v + 0xad) + (u32)param_2 * 4u);
        ent = ent + (u32)param_3 * 0x35u;
        tbl = *(u32 *)(this_v + 0xa7);
        v0 = (u32)(s32)*(s16 *)ent;             /* movsx word [eax]      */
        v1 = (u32)(s32)*(s16 *)(ent + 2);       /* movsx word [eax+2]    */
        p_a = *(u32 *)(tbl + (u32)((s32)v0 * 6));  /* [ecx + 6*v0] -> ebp */
        g92 = *(u16 *)0x00ae2c92;
        p_b = *(u32 *)(tbl + (u32)((s32)v1 * 6));  /* [ecx + 6*v1] -> ebx */
        g90 = *(u32 *)0x00ae2c90;
        g88 = *(u8 *)0x00ae2c88;

        *(u32 *)(L + 4) = g90;                  /* 0x00519ea2 dword */
        *(u16 *)(L + 8) = (u16)g92;             /* 0x00519ea6 word  */
        *(u8 *)(L + 3) = (u8)g88;               /* 0x00519eab byte  */

        rv_b = *(u32 *)p_b;                     /* 0x00519eaf */
        s4 = *(u32 *)(this_v + 4);              /* 0x00519eb1 */
        rv_p = *(u32 *)p_a;                     /* 0x00519eb4 */
        s4d = *(u32 *)s4;                       /* 0x00519eb8 */

        r = cm_fn_005ac920(*(u32 *)0x00ae2a58, (u32)(L + 8), (u32)(L + 4), (u32)(L + 3),
                           0, s4d, (u32)param_2, rv_p, rv_b,
                           0xffffffffu, 0xffffffffu, 0xffffffffu);

        if (r != 0) {
            for (;;) {
                /* 0x00519ee1 */
                if (*(u8 *)(r + 0x43) != 0xffu) {
                    ret = r;
                    goto done;
                }
                /* 0x00519eeb */
                if (*(u8 *)(L + 3) == 0) {
                    cm_fn_00549ef0((void *)(L + 0xc), *(u32 *)(L + 4), *(u32 *)(L + 8));
                    frame.state = 0;                              /* 0x00519f11 dword */
                    rv = cm_fn_0054c8e0((void *)(L + 0xc), (void *)(L + 0x1c), 1u);
                    *(volatile u8 *)&frame.state = 1u;             /* 0x00519f2b byte */
                    cm_fn_00417c10((void *)(L + 0xc), (u32 *)(L + 0x14), rv);
                    *(u32 *)(L + 4) = *(u32 *)(L + 0xc);          /* 0x00519f40 */
                    *(u32 *)(L + 8) = *(u32 *)(L + 0xe);          /* 0x00519f44 */
                    *(u8 *)(L + 3) = 2;                           /* 0x00519f48 */
                    frame.state = 0xffffffffu;                    /* 0x00519f4d dword */
                }
                else {
                    *(u8 *)(L + 3) = (u8)(*(u8 *)(L + 3) - 1u);   /* 0x00519f5a dec byte */
                }
                /* 0x00519f5e..0x00519f89, every reload repeated as listed */
                rv_b = *(u32 *)p_b;
                s4 = *(u32 *)(this_v + 4);
                rv_p = *(u32 *)p_a;
                s4d = *(u32 *)s4;

                r = cm_fn_005ac920(*(u32 *)0x00ae2a58, (u32)(L + 8), (u32)(L + 4),
                                   (u32)(L + 3), 0, s4d, (u32)param_2, rv_p, rv_b,
                                   0xffffffffu, 0xffffffffu, 0xffffffffu);
                if (r == 0)
                    break;
            }
        }
        ret = 0;
        goto done;
    }

    /* Shared error tail: the pushes preceding 00519e26 belong to the
       variadic formatter, not to the nested no-argument call. */
    msg = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)msg, blk_a, blk_b, code);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    *(u32 *)0x00b67a34 = 0;
    ret = 0;

done:
    CM_FS(void *, 0) = frame.prev;
    return (int)ret;
}
