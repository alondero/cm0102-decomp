/* Preferred-image VA 0053dba0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00600740(u32 self, u32 key, u32 person);
extern u32 CM_THISCALL cm_fn_006005e0(u32 self, u32 key, u32 person);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                   void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 text, u32 flags);

/* The complemented REPNE SCASB count includes the terminating byte. */
static __attribute__((always_inline)) inline u32 scan(u32 p)
{
    u32 left = 0xffffffffu;
    do {
        u8 value = B(p);
        ++p;
        --left;
        if (value == 0) break;
    } while (left != 0);
    return ~left;
}

u32 CM_CDECL cm_fn_0053dba0(u32 person, u32 output, u32 capacity, u32 selector)
{
    u8 scratch[0x200];
    u32 manager, first, second, preferred, index, target, from, length, words, tail;
    u32 line, version;
    void *name, *extension;
    s32 initial, other_initial;
    if (!person) goto bad_arguments;
    if (!output) goto bad_arguments;
    if ((s16)capacity < 0x69) goto bad_arguments;
    manager = D(0x00b63c98u);
    first = D(person + 4u);
    second = D(person + 8u);
    if (manager && cm_fn_00600740(manager, person, 0)) {
        manager = D(0x00b63c98u);
        preferred = cm_fn_006005e0(manager, person, 0);
    } else {
        preferred = D(person + 0xcu);
    }
    B(output) = 0;
    index = (selector & 0xffu) - 1u;
    if (index > 6) goto bad_selector;
    target = D(0x0053e118u + index * 4u);
    switch (target) {
    case 0x0053dc28: goto mode1;
    case 0x0053dc8c: goto mode2;
    case 0x0053dd38: goto mode3;
    case 0x0053ddf2: goto mode4;
    case 0x0053de9c: goto mode5;
    case 0x0053dfc5: goto mode6;
    case 0x0053df32: goto mode7;
    default: __builtin_unreachable();
    }
mode1:
    if (preferred && scan(preferred) - 1u != 0) goto copy_preferred;
    if (!first || scan(first) - 1u == 0) goto prefer_second;
    if (!second) goto prefer_preferred;
    if (scan(second) - 1u == 0) goto prefer_second;
pair:
    cm_fn_00944c9f(output, 0x0098f0b8u, first, second);
    return 1;
mode2:
    if (preferred && scan(preferred) - 1u != 0) goto copy_preferred;
    if (!first || scan(first) - 1u == 0) goto second_then_first;
    if (!second) goto prefer_first;
    if (scan(second) - 1u == 0) goto second_then_first;
    initial = (s8)B(first);
    cm_fn_00944c9f(output, 0x009b7410u, initial, second);
    return 1;
second_then_first:
    if (!second || scan(second) - 1u == 0) goto prefer_first;
    from = second;
    goto copy;
copy_preferred:
    from = preferred;
    goto copy;
mode3:
    if (preferred && scan(preferred) - 1u != 0) goto copy_preferred;
    if (!first || scan(first) - 1u == 0) goto mode3_second;
    if (!second) goto mode3_first;
    if (scan(second) - 1u == 0) goto mode3_second;
    cm_fn_00944c9f(output, 0x0098f6acu, second, first);
    return 1;
mode3_second:
    if (!second || scan(second) - 1u == 0) goto mode3_first;
    from = second;
    goto copy;
mode3_first:
    if (!first || scan(first) - 1u == 0) return 1;
    from = first;
    goto copy;
mode4:
    if (preferred && scan(preferred) - 1u != 0) {
        from = preferred;
        goto copy;
    }
    if (!first || scan(first) - 1u == 0) goto mode4_second;
    if (!second) goto prefer_first;
    if (scan(second) - 1u == 0) goto mode4_second;
    initial = (s8)B(first);
    cm_fn_00944c9f(output, 0x009b7408u, second, initial);
    return 1;
mode4_second:
    if (!second || scan(second) - 1u == 0) goto prefer_first;
    from = second;
    goto copy;
mode5:
    if (preferred && scan(preferred) - 1u != 0) goto copy_preferred;
    if (second && scan(second) - 1u != 0) {
        from = second;
        goto copy;
    }
    if (!first || scan(first) - 1u == 0) return 1;
    from = first;
    goto copy;
mode7:
    if (!first || scan(first) - 1u == 0) goto prefer_second;
    if (!second) goto prefer_preferred;
    if (scan(second) - 1u != 0) goto pair;
prefer_second:
    if (second && scan(second) - 1u != 0) {
        from = second;
        goto copy;
    }
prefer_preferred:
    if (preferred && scan(preferred) - 1u != 0) goto copy_preferred;
prefer_first:
    if (!first || scan(first) - 1u == 0) return 1;
    from = first;
copy:
    length = scan(from);
    words = length >> 2;
    tail = length & 3u;
    while (words != 0) {
        u32 value = D(from);
        D(output) = value;
        from += 4u;
        output += 4u;
        --words;
    }
    while (tail != 0) {
        u8 value = B(from);
        B(output) = value;
        ++from;
        ++output;
        --tail;
    }
    return 1;
mode6:
    if (preferred && scan(preferred) - 1u != 0) {
        initial = (s8)B(preferred);
        goto one_initial;
    }
    if (!first || scan(first) - 1u == 0) goto second_initial;
    if (!second) goto first_initial;
    if (scan(second) - 1u == 0) goto second_initial;
    other_initial = (s8)B(second);
    initial = (s8)B(first);
    cm_fn_00944c9f(output, 0x009b7400u, initial, other_initial);
    return 1;
second_initial:
    if (!second || scan(second) - 1u == 0) goto first_initial;
    initial = (s8)B(second);
    goto one_initial;
first_initial:
    if (!first || scan(first) - 1u == 0) return 1;
    initial = (s8)B(first);
one_initial:
    cm_fn_00944c9f(output, 0x0099ec1cu, initial);
    return 1;
bad_selector:
    cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 0x100, scratch);
    name = scratch + 0x100;
    extension = scratch;
    line = 0x3cb9u;
    goto error;
bad_arguments:
    cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch, scratch + 0x100);
    name = scratch;
    extension = scratch + 0x100;
    line = 0x3c3eu;
error:
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
