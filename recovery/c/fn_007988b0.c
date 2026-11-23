/* fn_007988b0 — oceania_nations.cpp, preferred-image VA 0x007988b0, 453 bytes,
   pinned CM0102 3.9.68 (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
   Plain C translation of the listing; constructor fragment for the Oceania
   nations object (thiscall, `ret 8`, two stack arguments).

   Held concern — case119/events32 at -O2 (FileTime conversions versus
   FindNextFileA, trace through 0x009468f4 / 0x00946907 / 0x00946982), the same
   shutdown stale-stack obligation as fn_004259a0 — is resolved here by leaving
   that obligation where the original puts it:

   - One scratch allocation covers the original L+0 .. L+0x207 span: `self` at
     L+4 (0x7988cf), buffer A at L+8 (0x79899d, 0x798a08), buffer B at L+0x108
     (0x7989a1, 0x798a01), and the L+0 dword holding the 0x00944cf1 result
     (0x7989d7). Nothing in it is initialised, so the bytes the original CRT
     file-enumeration path reads stay exactly as uninitialised here — no default
     initialisation is added, and the two buffers keep their adjacency inside a
     single allocation.
   - The shutdown / file-enumeration work itself is never reimplemented: it
     stays behind the original call boundaries 0x00944e46 (0x798987),
     0x00944cf1 (0x7989cf), 0x00799690 (0x798994), 0x0090d130 (0x798a2c),
     0x00944c9f (0x798a3c), 0x00944cff (0x7989b1/0x798a15) and 0x005e8290
     (0x798a4c), with the original push order, so the stack state those original
     instructions see is theirs to define.
   - SEH linkage follows the listing: state dword -1 then handler 0x0095eb79 then
     the previous fs:[0] head (0x7988b0..0x7988be); the state is then written as a
     DWORD 0 (0x7988ed) and twice as a BYTE (0x7989dd, 0x7989f5), which the low
     byte of `frame.state` represents; the predecessor is restored at 0x798a65.

   Call argument mapping (pushes are right-to-left, one `add esp` after each
   group; 0x0090d130 is bound as the (void) legacy edge, so the three values
   pushed at 0x7989c4/0x798a28 belong to the next call 0x00944c9f):
     path 0x57: pushes 0x57, L+8, L+0x108 -> 00944c9f(dest, fmt, text, L+0x108, L+8, 0x57)
     path 0x5e: pushes 0x5e, L+0x108, L+8 -> 00944c9f(dest, fmt, text, L+8, L+0x108, 0x5e)
   and 0x00944cff takes (0x00a6a0ec, 0, 0, <arg4>, <arg5>) with the same pair in
   listing order for each path (0x7989a8..0x7989b1 and 0x798a0c..0x798a15).

   Boundary (unproven consumption): the original pushes the three values at
   0x7989c4..0x7989c8 (path 0x57) or 0x798a28..0x798a2b (path 0x5e) *before*
   0x0090d130 runs at 0x798a2c, and only 0x00944c9f consumes them afterwards
   (single `add esp, 0x24` at 0x798a51). No listing VA inside 0x0090d130 shows it
   reading those slots, so whether it consumes them stays an explicit unknown:
   the receiver-free legacy edge is called as (void) and the values are bound to
   0x00944c9f, whose argument order is the one recorded above. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
/* 0x007988d3: ECX is still the incoming receiver. */
extern u32 CM_THISCALL cm_fn_00682200(u32);
/* 0x0079895a: ECX = ESI (0x798954); argument 1 pushed at 0x798952. */
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
/* 0x00798994: ECX = ESI (0x79898f). */
extern u32 CM_THISCALL cm_fn_00799690(u32);
/* 0x007989ea: ECX = EAX (allocation), ESI pushed at 0x7989e7. */
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007988b0(u32 self, u16 year, u32 competition)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 value, name, extension, line, text;
    s32 remainder;
    /* 0x7988b0..0x7988be: link the SEH record, state -1, handler, predecessor. */
    frame.state = 0xffffffffu;
    frame.handler = 0x0095eb79;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    /* 0x7988cf: the receiver is buffered in scratch at L+4. */
    D(local + 4) = self;
    cm_fn_00682200(self);                      /* 0x7988d3 */
    frame.state = 0;                           /* 0x7988ed */
    D(self) = 0x0096e0cc;                      /* 0x7988f4 vtable */
    W(self + 0x40) = year;                     /* 0x7988fa */
    D(self + 4) = competition;                 /* 0x7988fe */
    B(self + 0x42) = 4;                        /* 0x798901 */
    D(self + 0x1c) = 0xffffffffu;              /* 0x798905 */
    D(self + 0x20) = 0xffffffffu;              /* 0x79890c */
    B(self + 0x50) = 0;                        /* 0x798913 */
    if ((s16)year < 2002)                      /* 0x7988e9 cmp ax / 0x798916 jge */
        W(self + 0x40) = 2002;                 /* 0x798918 */
    /* 0x79891e: signed remainder by two, loop until even. */
    remainder = ((s32)(s16)W(self + 0x40) - 1996) % 2;
    while (remainder != 0) {
        W(self + 0x40) = (u16)(W(self + 0x40) + 1u);   /* 0x798937 */
        remainder = ((s32)(s16)W(self + 0x40) - 1996) % 2;
    }
    B(self + 0x51) = 15;                       /* 0x798956 */
    if (cm_fn_00687b10(self, 1) != 0)          /* 0x798952 push 1 / 0x79895a call */
        goto done;
    W(self + 0xd9) = 40;                       /* 0x798969 */
    D(self + 0x45) = 0;                        /* 0x798972 */
    B(self + 0x44) = 0xff;                     /* 0x798975 */
    D(self + 0x30) = 0xffffffffu;              /* 0x798979 */
    D(self + 0x2c) = 2;                        /* 0x798980 */
    value = (u32)cm_fn_00944e46(8);            /* 0x798967 push 8 / 0x798987 call */
    D(self + 0xc) = value;                     /* 0x798991 stores EAX only */
    if (cm_fn_00799690(self) == 0) {           /* 0x798994 / 0x79899b jne */
        name = local + 0x108;                  /* 0x7989bd */
        extension = local + 8;                 /* 0x7989b9 */
        line = 0x57;                           /* 0x7989c4 */
        goto error;
    }
    value = (u32)cm_fn_00944cf1(0x5ce);        /* 0x7989ca push 0x5ce / 0x7989cf call */
    D(local) = value;                          /* 0x7989d7 */
    *(volatile u8 *)&frame.state = 1;          /* 0x7989dd byte store */
    if (value != 0)
        value = cm_fn_0049ee70(value, self);   /* 0x7989e7 push esi / 0x7989ea call */
    *(volatile u8 *)&frame.state = 0;          /* 0x7989f5 byte store */
    D(self + 8) = value;                       /* 0x7989fc */
    if (value != 0)
        goto done;
    name = local + 8;                          /* 0x798a08 */
    extension = local + 0x108;                 /* 0x798a01 */
    line = 0x5e;                               /* 0x798a28 */
error:
    cm_fn_00944cff(0x00a6a0ec, 0, 0, name, extension);   /* 0x7989b1 / 0x798a15 */
    text = cm_fn_0090d130();                   /* 0x798a2c */
    /* 0x798a31 push eax / 0x798a32 fmt / 0x798a37 dest, then the three values
       still live from 0x7989c4..0x798a2b. */
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0); /* 0x798a41..0x798a4c */
    D(0x00b67a34) = 0;                         /* 0x798a54 */
done:
    CM_FS(u32, 0) = frame.prev;                /* 0x798a5a / 0x798a65 */
    return self;                               /* 0x798a61 EAX = ESI */
}
