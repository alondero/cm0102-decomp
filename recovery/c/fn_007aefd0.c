#include "cmre.h"
/* Preferred-image VA 0x007aefd0, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall: ECX carries the object, RET 0x18 pops six stack words, so the
   routine takes the object plus six caller-owned output pointers.

   Evidence: CM0102/Analysis/cmre/evidence/fn_007aefd0-531aac63b156834f33a6161a6e32a2ce7bfc352c3ee0dc37ad3934955d70f257.txt
   (listing 0x007aefd0..0x007af15d, 99 instructions) plus the three direct
   caller windows 0x007b7569, 0x008091aa, 0x0080b00f.

   Caller 0x007b7569 pushes, right to left, &this->0x190, &this->0x18c,
   &this->0x194, &this->0x195, &this->0x199, &this->0x19d; the other two
   callers push six stack words the same way.  The six outputs therefore alias
   the object itself, so every object field is re-read after the previous
   store, exactly as the listing does.

   Frame: SUB ESP,0x200 then PUSH ESI, so the scratch base L is ESP just after
   the subtraction and [ESP+4] is L while [ESP+0x104] is L+0x100.  The
   diagnostic buffers are L+0 (extension slot) and L+0x100 (name slot); the
   same layout is used by accepted recovery/c/fn_00403760.c.

   0x0090d130 is `mov eax, 0x00ad4394; ret`: it takes no argument.  The three
   pushes in front of it belong to the following 0x00944c9f call, so that call
   takes six arguments (destination, format, version, name, extension, line). */

#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

/* 0x0053e990: cdecl, five words; accepted C (recovery/c/fn_0053e990.c). */
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
/* 0x00944cff: cdecl, five words: path, 0, 0, name buffer, extension buffer. */
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 zero1, u32 zero2, void *arg4, void *arg5);
/* 0x0090d130: no argument; returns the version string pointer. */
extern char *CM_CDECL cm_fn_0090d130(void);
/* 0x00944c9f: cdecl, variadic: destination, format, then the printed values. */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
/* 0x005e8290: cdecl, three words: message, buffer, mode. */
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer, int mode);

u32 CM_THISCALL cm_fn_007aefd0(u32 self, volatile u32 out1, volatile u32 out2,
                               volatile u32 out3, volatile u32 out4,
                               volatile u32 out5, volatile u32 out6)
{
    u8 scratch[0x200];
    u32 l = (u32)scratch;
    u32 idx, base, person, third, second, first, r, v190, v18c, v195, v199, v19d;
    u32 p1, p2, p3, p4, p5, p6;

    /* 0x007aefd9: CMP BYTE [ESI+0x0a],4 / JNE 0x007af0f1. */
    if (B(self + 0x0au) != 4u)
        goto diagnostic;

    /* 0x007aefe3: the index is a signed dword; -1 or less leaves the routine. */
    idx = D(self + 0x180u);
    if ((s32)idx <= -1)
        goto diagnostic;

    /* 0x007aeff2..0x007af017: the argument reads keep their listing order
       (0x17c, then 0x178, then 0x174, then the base pointer). */
    third = D(self + 0x17cu);
    second = D(self + 0x178u);
    first = D(self + 0x174u);
    base = D(0x00ae23bcu);

    /* 0x007af00d..0x007af01c: LEA ECX,[EAX+EAX*4], LEA EAX,[EAX+ECX*2],
       LEA EDX,[EAX+EAX*4], LEA ECX,[EAX+EDX*2] gives base + 110*index. */
    person = base + (idx * 110u);
    r = cm_fn_0053e990(person, first, second, third, self + 0x184u); /* 0x007af020 */
    if (r == 0u)
        goto diagnostic;                          /* 0x007af02a */

    /* 0x007af030: the index and the base are re-read after the call. */
    idx = D(self + 0x180u);
    base = D(0x00ae23bcu);
    if (base + (idx * 110u) == 0u)
        goto diagnostic;                          /* 0x007af04a */

    /* 0x007af050: the first output value is read here and kept in EAX. */
    v190 = D(self + 0x190u);
    if ((s32)v190 > -1)
        goto store;                               /* 0x007af059 */
    if ((s32)D(self + 0x18cu) > -1)
        goto store;                               /* 0x007af062 */
    /* 0x007af064: CMP BYTE [ESI+0x194],0xFF / JG is a signed comparison, so
       the branch is taken for a byte value of 0x00..0x7f only. */
    if ((s8)B(self + 0x194u) > -1)
        goto store;                               /* 0x007af06b */
    if ((s32)D(self + 0x195u) > -1)
        goto store;                               /* 0x007af074 */
    if (D(self + 0x199u) != 0u)
        goto store;                               /* 0x007af07e */
    if (D(self + 0x19du) == 0u)
        return 0u;                                /* 0x007af088 -> 0x007af154 */

store:
    /* 0x007af08e..0x007af0e1: six stores, each reading its output pointer
       again and re-reading the object field after the previous store. */
    p1 = out1;
    p2 = out2;
    D(p1) = v190;                                 /* 0x007af09c */
    v18c = D(self + 0x18cu);                      /* 0x007af09e */
    D(p2) = v18c;                                 /* 0x007af0a4 */
    p3 = out3;
    *(volatile u8 *)(u32)p3 = B(self + 0x194u);   /* 0x007af0b3, BYTE store */
    p4 = out4;
    v195 = D(self + 0x195u);                      /* 0x007af0bc */
    D(p4) = v195;                                 /* 0x007af0c2 */
    p5 = out5;
    v199 = D(self + 0x199u);                      /* 0x007af0cb */
    D(p5) = v199;                                 /* 0x007af0d1 */
    p6 = out6;
    v19d = D(self + 0x19du);                      /* 0x007af0da */
    D(p6) = v19d;                                 /* 0x007af0e1 */
    return 1u;                                    /* 0x007af0e3 */

diagnostic:
    /* 0x007af0f1: the path split fills the name slot (L+0x100) and the
       extension slot (L+0). */
    cm_fn_00944cff(0x00a701b0u, 0u, 0u, (void *)(l + 0x100u), (void *)l); /* 0x007af107 */
    r = (u32)cm_fn_0090d130();                    /* 0x007af121 */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, r, l + 0x100u, l, 0x268u);  /* 0x007af131 */
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0); /* 0x007af142 */
    D(0x00b67a34u) = 0u;                          /* 0x007af14a */
    return 0u;                                    /* 0x007af154 */
}
