#include "cmre.h"

/* Preferred-image VA004415F0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/RET0/full EAX. Incoming ECX has no value dependency: each
   string block ORs it with FFFFFFFF; both error paths overwrite it by LEA.
   DF is clear under input policy v2. Keep DWORD-then-BYTE forward copy. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_004415f0(u32 destination, u32 size_arg, u32 type_arg)
{
    u8 scratch[0x200];
    u32 L = (u32)scratch, index, target, source, cursor, remaining, length;
    u32 words, bytes, value, name, extension, line;
    if (destination == 0 || (s16)(u16)size_arg < 50) {
        name = L; extension = L + 0x100; line = 0x285;
        goto error;
    }
    index = (u32)(s32)(s8)(u8)type_arg;
    if (index > 25) {
        name = L + 0x100; extension = L; line = 0x2db;
        goto error;
    }
    /* The bounded dispatch reads the pinned image table before any copy. */
    target = D(0x441ba4 + index * 4);
    switch (target) {
    case 0x44162d: source = 0x98b254; break;
    case 0x44165d: source = 0x98b47c; break;
    case 0x44168d: source = 0x98b474; break;
    case 0x4416bd: source = 0x98b470; break;
    case 0x4416ed: source = 0x98b468; break;
    case 0x44171d: source = 0x98b460; break;
    case 0x44174d: source = 0x98b3a4; break;
    case 0x44177d: source = 0x98b458; break;
    case 0x4417ad: source = 0x98b450; break;
    case 0x4417dd: source = 0x98b44c; break;
    case 0x44180d: source = 0x98b448; break;
    case 0x44183d: source = 0x98b444; break;
    case 0x44186d: source = 0x98b43c; break;
    case 0x44189d: source = 0x98b438; break;
    case 0x4418cd: source = 0x98b430; break;
    case 0x4418fd: source = 0x98b310; break;
    case 0x44192d: source = 0x98b2dc; break;
    case 0x44195d: source = 0x98b42c; break;
    case 0x44198d: source = 0x98b424; break;
    case 0x4419bd: source = 0x98b41c; break;
    case 0x4419ed: source = 0x98b414; break;
    case 0x441a1d: source = 0x98b410; break;
    case 0x441a4d: source = 0x98b40c; break;
    case 0x441a7d: source = 0x98b408; break;
    case 0x441aad: source = 0x98b400; break;
    case 0x441add: source = 0x98b3fc; break;
    default: __builtin_unreachable(); /* All 26 pinned table entries above. */
    }
    cursor = source;
    remaining = 0xffffffff;
    do {
        value = B(cursor);
        ++cursor;
        --remaining;
        if ((u8)value == 0) break;
    } while (remaining != 0);
    length = ~remaining;
    source = cursor - length;
    words = length >> 2;
    while (words != 0) {
        value = D(source);
        D(destination) = value;
        source += 4; destination += 4; --words;
    }
    bytes = length & 3;
    while (bytes != 0) {
        value = B(source);
        B(destination) = (u8)value;
        ++source; ++destination; --bytes;
    }
    return 1;
error:
    cm_fn_00944cff(0x98b1fc, 0, 0, (void *)name, (void *)extension);
    value = cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0, 0x9870e8, value, name, extension, line);
    cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
    D(0xb67a34) = 0;
    return 0;
}
