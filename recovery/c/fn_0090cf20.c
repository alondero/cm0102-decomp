/* Preferred-image VA 0090cf20, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

/* Complemented REPNE SCASB count, including the terminating byte. */
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

u32 CM_CDECL cm_fn_0090cf20(u32 source, u32 output)
{
    u32 length = scan(source) - 1u;
    u32 count, from, to, words, tail, result;
    u8 ch;

    if ((s32)length < 0x100) {
        count = scan(source);
        from = source;
        to = output;
        words = count >> 2;
        tail = count & 3u;
        while (words != 0) {
            u32 value = D(from);
            D(to) = value;
            from += 4u;
            to += 4u;
            --words;
        }
        while (tail != 0) {
            u8 value = B(from);
            B(to) = value;
            ++from;
            ++to;
            --tail;
        }
        return count;
    }

    ch = B(source);
    count = 0;
    to = output;
    from = source + 1u;
    while (ch != 0) {
        if (ch != 0x20) {
            if (count == 0xffu) {
                B(to) = 0;
                result = cm_fn_009466d0(output + 0xf7u, source + length - 8u, 8);
                B(output + 0xffu) = 0;
                return result;
            }
            B(to) = ch;
            ++to;
            ++count;
        }
        ch = B(from);
        ++from;
    }
    B(output + count) = 0;
    return 0;
}
