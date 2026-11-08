/* Preferred-image VAs, base 00400000; SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Byte getters preserve EAX[31:8] from record. Three direct wrappers forward
 * EAX unchanged, so retain its full physical value, including error-helper EAX.
 * __ftol follows ADR 0006; all other callees remain original boundaries. */
#include "cmre.h"
extern long long CM_CDECL cm_fn_005442f0(u32 record, u32 adjustment);
extern long long CM_CDECL cm_fn_00544380(u32 record, u32 adjustment);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 text, u32 flags);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 file, u32 zero1, u32 zero2, void *first, void *second);
#define BYTE_AT(a) CM_GLOBAL(volatile u8, (a))
#define SBYTE_AT(a) CM_GLOBAL(volatile s8, (a))
#define SWORD_AT(a) CM_GLOBAL(volatile s16, (a))
#define CONSTANT(a) CM_GLOBAL(volatile double, (a))

__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_00543540(u32 person, u32 record, s8 selector)
{
    /* L = original ESP after SUB ESP,204h. FILD temporary is L+0;
     * related diagnostic buffers are L+4 and L+104h, uninitialized. */
    u8 scratch[0x204];
    u8 *first, *second;
    u32 line, index, target, field, byte_value, error_result;
    s32 attribute, word;
    long double t, a, b, square;
    char *version;
    if (!person || selector < 1 || selector > 31) {
        first = scratch + 4; second = scratch + 0x104; line = 0x48fc;
        goto diagnostic;
    }
    if (!record) {
        first = scratch + 0x104; second = scratch + 4; line = 0x4903;
        goto diagnostic;
    }
    index = (u32)((s32)selector - 1);
    if (index > 30) {
        first = scratch + 4; second = scratch + 0x104; line = 0x4981;
        goto diagnostic;
    }
    target = CM_GLOBAL(volatile u32, 0x00543a94u + index * 4u);
    switch (target) {
    case 0x5435be: field = 0x1b; goto byte_getter;
    case 0x54365b: field = 0x1c; goto byte_getter;
    case 0x543665: field = 0x1d; goto byte_getter;
    case 0x54366f: field = 0x1e; goto normal;
    case 0x5436c7: field = 0x1f; goto byte_getter;
    case 0x5436d1: field = 0x20; goto byte_getter;
    case 0x5436db: field = 0x43; goto category20;
    case 0x543722: field = 0x23; goto category20;
    case 0x5435c8: field = 0x24; goto normal;
    case 0x54376c:
        byte_value = BYTE_AT(person + 0x58u);
        return (record & 0xffffff00u) | byte_value;
    case 0x543776: field = 0x26; goto category20;
    case 0x54396c:
        if (BYTE_AT(record + 0xfu) == 20) {
            byte_value = BYTE_AT(record + 0x27u);
            return (u32)cm_fn_005442f0(record, (person & 0xffffff00u) | byte_value);
        }
        byte_value = BYTE_AT(record + 0x27u);
        return (u32)cm_fn_00544380(record, byte_value);
    case 0x5437c0: field = 0x28; goto byte_getter;
    case 0x5437ca:
        field = 0x2a;
        if (SBYTE_AT(record + 0xfu) < 15) goto scaled_word;
        goto normal;
    case 0x543814: field = 0x2b; goto normal;
    case 0x543833: field = 0x2f; goto byte_getter;
    case 0x54383d: field = 0x2e; goto byte_getter;
    case 0x5439d6:
        byte_value = BYTE_AT(record + 0x31u);
        return (u32)cm_fn_00544380(record, byte_value);
    case 0x543847: field = 0x32; goto category20;
    case 0x543891: field = 0x33; goto category20;
    case 0x5438db: field = 0x36; goto byte_getter;
    case 0x5438e5: field = 0x37; goto normal;
    case 0x543904: field = 0x39; goto normal;
    case 0x543923:
        field = 0x3a;
        if (SBYTE_AT(record + 0xfu) < 15) goto scaled_word;
        byte_value = BYTE_AT(record + 0x3au);
        return (u32)cm_fn_00544380(record, byte_value);
    case 0x543962: field = 0x29; goto byte_getter;
    case 0x54399a: field = 0x3c; goto byte_getter;
    case 0x5439a4: field = 0x3d; goto byte_getter;
    case 0x5439ae:
        byte_value = BYTE_AT(record + 0x3eu);
        return (u32)cm_fn_00544380(record, (person & 0xffffff00u) | byte_value);
    case 0x5439c2: field = 0x3f; goto byte_getter;
    case 0x5439cc: field = 0x40; goto byte_getter;
    case 0x5439ea: field = 0x44; goto byte_getter;
    /* All 31 guarded entries in the pinned read-only target table are above. */
    default: __builtin_unreachable();
    }
byte_getter:
    byte_value = BYTE_AT(record + field);
    return (record & 0xffffff00u) | byte_value;
category20:
    if (BYTE_AT(record + 0xfu) == 20) goto scaled_word;
normal:
    attribute = SBYTE_AT(record + field);
    word = SWORD_AT(record + 5u);
    a = (long double)attribute;
    a += a;
    b = (long double)word;
    t = a + b;
    goto polynomial;
scaled_word:
    word = SWORD_AT(record + 5u);
    attribute = SBYTE_AT(record + field);
    a = (long double)word;
    a *= CONSTANT(0x967880);
    b = (long double)attribute;
    b += b;
    t = a + b;
polynomial:
    t *= CONSTANT(0x967888);
    t += CONSTANT(0x9688b0);
    square = t;
    square *= t;
    square *= CONSTANT(0x967880);
    t = square + t;
    t *= CONSTANT(0x968a80);
    t += CONSTANT(0x968968);
    if (t > CONSTANT(0x967890)) {
        if (t >= CONSTANT(0x968ea8)) {
            t = CONSTANT(0x968ea8);
            goto convert_second;
        }
    }
    if (t > CONSTANT(0x967890)) goto convert_second;
    t = CONSTANT(0x967890);
    /* Original 0054363b CALL 00945640: low clamp, 64-bit truncation. */
    return (u32)(long long)t;
convert_second:
    /* Original 0054364f CALL 00945640: interior/upper clamp, 64-bit truncation. */
    return (u32)(long long)t;
diagnostic:
    cm_fn_00944cff(0x9b5ac8, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0, 0x9870e8, version, first, second, line);
    error_result = cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
    CM_GLOBAL(volatile u32, 0xb67a34) = 0;
    return error_result | 0xffu;
}
