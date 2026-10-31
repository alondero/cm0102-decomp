/* Preferred-image VA 0x005bf770, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, ret 0. Returns -1 on allocation failure, 0 on later errors, 1 on success.
   Scratch L is entry ESP-0x200: both 256-byte buffers share that allocation. */
#include "cmre.h"

extern unsigned long long cm_fn_00944e46(u32 size);
extern u32 cm_fn_004afd80(u32 record, char which);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);

u32 CM_FASTCALL cm_fn_005bf770(u32 self)
{
    u8 scratch[0x200];
    u32 p;
    u32 index;
    u32 base;
    u32 result;
    u32 second;
    u32 line;
    u32 error_return = 0;
    void *first_buffer = scratch;
    void *second_buffer = scratch + 0x100;
    char *text;

    *(volatile u16 *)(self + 0x3eu) = 2;
    p = (u32)cm_fn_00944e46(12);
    *(volatile u32 *)(self + 0xa7u) = p;
    if (p == 0) {
        first_buffer = scratch + 0x100;
        second_buffer = scratch;
        line = 0xc4u;
        error_return = 0xffffffffu;
        goto error;
    }
    index = CM_GLOBAL(volatile u32, 0x009cf5cc);
    base = CM_GLOBAL(volatile u32, 0x00ae23d0);
    result = cm_fn_004afd80(index * 0x6bu + base, 0);
    p = *(volatile u32 *)(self + 0xa7u);
    *(volatile u32 *)p = result;
    p = *(volatile u32 *)(self + 0xa7u);
    if (*(volatile u32 *)p == 0) {
        line = 0xcdu;
        goto error;
    }
    index = CM_GLOBAL(volatile u32, 0x009cf70c);
    base = CM_GLOBAL(volatile u32, 0x00ae23d0);
    result = cm_fn_004afd80(index * 0x6bu + base, 0);
    p = *(volatile u32 *)(self + 0xa7u);
    *(volatile u32 *)(p + 6u) = result;
    p = *(volatile u32 *)(self + 0xa7u);
    second = *(volatile u32 *)(p + 6u);
    if (second == 0) {
        line = 0xd6u;
        goto error;
    }
    if (*(volatile u32 *)p == second) {
        index = CM_GLOBAL(volatile u32, 0x009cf5cc);
        base = CM_GLOBAL(volatile u32, 0x00ae23d0);
        result = cm_fn_004afd80(index * 0x6bu + base, 1);
        p = *(volatile u32 *)(self + 0xa7u);
        *(volatile u32 *)(p + 6u) = result;
        p = *(volatile u32 *)(self + 0xa7u);
        if (*(volatile u32 *)(p + 6u) == 0) {
            line = 0xe2u;
            goto error;
        }
    }
    *(volatile u8 *)(p + 4u) = 0;
    p = *(volatile u32 *)(self + 0xa7u);
    *(volatile u8 *)(p + 5u) = 0;
    p = *(volatile u32 *)(self + 0xa7u);
    *(volatile u8 *)(p + 10u) = 0;
    p = *(volatile u32 *)(self + 0xa7u);
    *(volatile u8 *)(p + 11u) = 0;
    return 1;

error:
    cm_fn_00944cff(0x009ccc48u, 0, 0, first_buffer, second_buffer);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                   first_buffer, second_buffer, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return error_return;
}
