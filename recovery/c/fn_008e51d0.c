/* Semantic reconstruction of fn_008e51d0; passes cmre test --c at -O0 and -O2.
   Preferred-image VA 0x008e51d0, image base 0x00400000.
   Pinned binary CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86 */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

#define DAT_00a9e1cc CM_GLOBAL(u8, 0x00a9e1cc)
#define DAT_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)
#define LAB_009648cb CM_GLOBAL(u8, 0x009648cb)
#define STR_00a9e2bc ((const char *)0x00a9e2bc)

/* Callees remain explicit boundaries.  Signatures follow the original
   instructions at preferred-image VAs (callee C in the brief is navigation). */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *self, u32 flags);
extern u32 CM_CDECL cm_fn_00441c10(u32 output, u32 size, u32 low, u32 high, u32 mode);
extern char * CM_CDECL cm_fn_0066f4e0(u32 a1, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);

typedef struct {
    void *prev;
    void *handler;
    u32 state;
} CMRE_SEH;

u32 CM_STDCALL cm_fn_008e51d0(u32 arg1, u32 arg2, u32 arg3)
{
    volatile CMRE_SEH frame;
    u8 scratch[0x1dc];
    u32 r0, r1, r2, r3, r4, r5, r6, r7;
    u32 a, b;
    u32 ret = 0;

    /* SEH prologue: pushes of state(-1), handler(0x9648cb), prev(old fs:[0])
       then mov fs:[0],esp.  One volatile {prev,handler,state} record. */
    frame.state = 0xffffffffu;
    frame.handler = (void *)0x009648cb;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    /* Eight thiscall lookups; EAX of each captured in original order. */
    r0 = (u32)cm_fn_00785830((void *)arg1, 0);
    r1 = (u32)cm_fn_00785830((void *)arg1, 1);
    *(volatile u32 *)(scratch + 0x0c) = r1;   /* 008e520e: [L+0xc] */
    r2 = (u32)cm_fn_00785830((void *)arg1, 2);
    *(volatile u32 *)(scratch + 0x08) = r2;   /* 008e521b: [L+8] */
    r3 = (u32)cm_fn_00785830((void *)arg1, 3);
    *(volatile u32 *)(scratch + 0x04) = r3;   /* 008e5228: [L+4] */
    r4 = (u32)cm_fn_00785830((void *)arg1, 4);
    r5 = (u32)cm_fn_00785830((void *)arg1, 5);
    *(volatile u32 *)(scratch + 0x00) = r5;   /* 008e5240: [L+0] */
    r6 = (u32)cm_fn_00785830((void *)arg1, 6);
    r7 = (u32)cm_fn_00785830((void *)arg1, 7);

    if (arg2 == 0) {
        /* Branch A: 008e5265 */
        u32 i6;
        u32 sb;
        cm_fn_0053e140(r1, r2, r3, (u32)(scratch + 0x4c), 400, 5, r0);
        a = (u32)DAT_00ae23b4;
        i6 = r6 * 0x245u;                       /* lea/shl/add */
        sb = (u32)(s32)(s8)(*(volatile u8 *)(i6 + a + 0x52)); /* movsx byte */
        cm_fn_0066f4e0((u32)&DAT_00de1f64, (u32)STR_00a9e2bc,
                       sb, sb, 100, i6 + a + 0x38, (u32)(scratch + 0x4c));
        cm_fn_004ae660((void *)arg3, (u32)&DAT_00de1f64);
        a = (u32)DAT_00ae23b4;
        cm_fn_004ae8a0(arg3, (const char *)(i6 + a + 0x38), 0x7d5u, i6 + a);
        b = (u32)DAT_00ae23bc;
        ret = cm_fn_0053e990(r0 * 0x6eu + b, r1, r2, r3, 0);
        if (ret == 0)
            goto done;
        b = (u32)DAT_00ae23bc;
        ret = cm_fn_004ae8a0(arg3, (const char *)(scratch + 0x4c), 0x7d7u,
                             r0 * 0x6eu + b);
    } else {
        /* Branch B: 008e5336 */
        unsigned long long rv;
        u32 i4, i6, i7, sb4, sb6, sb7, low, high;
        *(volatile u32 *)(scratch + 0x10) = *(volatile u32 *)(scratch + 0x00);
        frame.state = 0;                        /* 008e5346: mov [esp+0x1fc],0 */
        rv = cm_fn_0043f460((const int *)(scratch + 0x10), 6);
        low = (u32)rv;
        high = (u32)(rv >> 32);
        cm_fn_00441c10((u32)(scratch + 0x18), 0x32u, low, high, 6);
        cm_fn_0053e140(r1, r2, r3, (u32)(scratch + 0x4c), 400, 1, r0);
        a = (u32)DAT_00ae23b4;
        i4 = r4 * 0x245u;
        i6 = r6 * 0x245u;
        i7 = r7 * 0x245u;
        sb6 = (u32)(s32)(s8)(*(volatile u8 *)(i6 + a + 0x52));
        *(volatile u32 *)(scratch + 0x14) = sb6;
        sb4 = (u32)(s32)(s8)(*(volatile u8 *)(i4 + a + 0x52));
        sb7 = (u32)(s32)(s8)(*(volatile u8 *)(i7 + a + 0x52));
        cm_fn_0066f4e0((u32)&DAT_00de1f64, (u32)&DAT_00a9e1cc,
                       sb6, sb6, 100, sb7, sb7, sb4, sb4,
                       i6 + a + 0x38, (u32)(scratch + 0x18),
                       (u32)(scratch + 0x4c), i7 + a + 0x38, i4 + a + 0x38);
        cm_fn_004ae660((void *)arg3, (u32)&DAT_00de1f64);
        a = (u32)DAT_00ae23b4;
        cm_fn_004ae8a0(arg3, (const char *)(i6 + a + 0x38), 0x7d5u, i6 + a);
        a = (u32)DAT_00ae23b4;
        cm_fn_004ae8a0(arg3, (const char *)(i7 + a + 0x38), 0x7d5u, i7 + a);
        a = (u32)DAT_00ae23b4;
        cm_fn_004ae8a0(arg3, (const char *)(i4 + a + 0x38), 0x7d5u, i4 + a);
        b = (u32)DAT_00ae23bc;
        ret = cm_fn_0053e990(r0 * 0x6eu + b, r1, r2, r3, 0);
        if (ret == 0)
            goto done;
        b = (u32)DAT_00ae23bc;
        ret = cm_fn_004ae8a0(arg3, (const char *)(scratch + 0x4c), 0x7d7u,
                             r0 * 0x6eu + b);
    }

done:
    CM_FS(void *, 0) = frame.prev;
    return ret;
}
