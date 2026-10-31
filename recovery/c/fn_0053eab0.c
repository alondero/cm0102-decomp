/* Preferred-image VA 0053eab0, base 00400000; pinned 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* REPNE SCASB with ECX=-1 and DF clear. The complement includes the NUL.
   Keep the complete scan even at sites that only test for an empty string. */
static __attribute__((always_inline)) inline u32 scan(u32 p)
{
    u32 left = 0xffffffffu;
    do {
        u8 c = B(p);
        p++;
        left--;
        if (c == 0) break;
    } while (left != 0);
    return ~left;
}

u32 CM_CDECL cm_fn_0053eab0(u32 source, u32 output, s16 capacity,
                           volatile u32 selector)
{
    u8 scratch[0x200];
    u32 first, second, owner, mode, target, from, length, words, tail;
    u32 line, text1, text2, version;
    s32 ch, ch2;
    if (!source) goto bad_arguments;
    if (!output) goto bad_arguments;
    if (capacity < 0x69) goto bad_arguments;
    first = D(source + 4);
    second = D(source + 8);
    B(output) = 0;
    owner = D(source + 0x16);
    if (owner) {
        u32 id = D(owner);
        u32 current = D(0x009cf488);
        if (id == current) goto prefer_second;
    }
    mode = selector & 0xffu;
    if (mode - 1u > 5u) goto bad_selector;
    target = D(0x0053ee2c + (mode - 1u) * 4u);
    switch (target) {
    case 0x0053eb3f: mode = 1; goto pair;
    case 0x0053eb84: mode = 2; goto pair;
    case 0x0053ebd8: mode = 3; goto pair;
    case 0x0053ec29: mode = 4; goto pair;
    case 0x0053ec79: goto prefer_second;
    case 0x0053ecf7: goto initials;
    default: __builtin_unreachable();
    }
pair:
    if (!first) goto prefer_second;
    if (scan(first) - 1u == 0) goto prefer_second;
    if (!second) goto copy_first;
    if (scan(second) - 1u == 0) goto prefer_second;
    switch (mode) {
    case 1:
        cm_fn_00944c9f(output, 0x0098f0b8, first, second);
        break;
    case 2:
        ch = (s8)B(first);
        cm_fn_00944c9f(output, 0x009b7410, ch, second);
        break;
    case 3:
        cm_fn_00944c9f(output, 0x0098f6ac, second, first);
        break;
    case 4:
        ch = (s8)B(first);
        cm_fn_00944c9f(output, 0x009b7408, second, ch);
        break;
    }
    return 1;
prefer_second:
    if (!second) goto copy_first;
    if (scan(second) - 1u == 0) goto copy_first;
    from = second;
    goto copy;
copy_first:
    if (!first) return 1;
    if (scan(first) - 1u == 0) return 1;
    from = first;
copy:
    length = scan(from);
    /* Original SUB EDI,ECX recovers the starting address after this scan. */
    words = length >> 2;
    tail = length & 3u;
    while (words) {
        u32 value = D(from);
        D(output) = value;
        from += 4;
        output += 4;
        words--;
    }
    while (tail) {
        u8 value = B(from);
        B(output) = value;
        from++;
        output++;
        tail--;
    }
    return 1;
initials:
    if (!first) goto short_second;
    if (scan(first) - 1u == 0) goto short_second;
    if (!second) goto short_first;
    if (scan(second) - 1u == 0) goto short_second;
    ch2 = (s8)B(second);
    ch = (s8)B(first);
    cm_fn_00944c9f(output, 0x009b7418, ch, ch2);
    return 1;
short_second:
    if (!second) goto short_first;
    if (scan(second) - 1u == 0) goto short_first;
    ch = (s8)B(second);
    goto one_character;
short_first:
    if (!first) return 1;
    if (scan(first) - 1u == 0) return 1;
    ch = (s8)B(first);
one_character:
    cm_fn_00944c9f(output, 0x0099ec1c, ch);
    return 1;
bad_selector:
    cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 0x100, scratch);
    text1 = (u32)(scratch + 0x100);
    text2 = (u32)scratch;
    line = 0x3e29;
    goto diagnostic;
bad_arguments:
    cm_fn_00944cff(0x009b5ac8, 0, 0, scratch, scratch + 0x100);
    text1 = (u32)scratch;
    text2 = (u32)(scratch + 0x100);
    line = 0x3dc7;
diagnostic:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, text1, text2, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
