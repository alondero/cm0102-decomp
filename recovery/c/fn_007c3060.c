#include "cmre.h"
/* Independent grouped-offset correction review complete; canonical gate/controls pending. */

/* Preferred-image VA 007c3060, image base 00400000.
   Original SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   RET 001c; seven stack arguments. Incoming ECX is overwritten by
   OR ECX,-1 before any meaningful use. Return is full EAX.
   All callees below remain original-code boundaries. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

#define V8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define V16(a) CM_GLOBAL(volatile u16, (u32)(a))
#define V32(a) CM_GLOBAL(volatile u32, (u32)(a))
#define LOCAL32(o) V32((u32)scratch + (u32)(o))

u32 CM_STDCALL cm_fn_007c3060(u32 object, u32 selector, u32 destination,
                            u32 size, u32 split, u32 detail, u32 show_zero)
{
    /* L is original ESP immediately after SUB ESP,020c.
       After the three saved registers, ESP=L-0c.
       Both error buffers and the arithmetic temporaries share this allocation:
       ESP+18 = L+0c; ESP+118 = L+10c. No initialization is invented. */
    u8 scratch[0x20c];
    u32 source, scan, remaining, count, out, word, index, dispatch;
    u32 a, b, c, offset, line, filename, extension, version;
    s32 denominator, numerator;
    u8 byte;
    s16 sample;
    long double t;
    long long converted;
    volatile double rounded;

    if (destination == 0 || (s32)size < 0x33)
        goto bad_arguments;

    source = (u8)selector == 0x11 ? 0x0099cc4cu : 0x0098ba0cu;
    scan = source;
    remaining = 0xffffffffu;
    while (remaining != 0) {
        byte = V8(scan);
        scan += 1u;
        remaining -= 1u;
        if (byte == 0)
            break;
    }
    count = ~remaining;
    source = scan - count;
    out = destination;
    remaining = count >> 2;
    while (remaining != 0) {
        word = V32(source);
        V32(out) = word;
        source += 4u;
        out += 4u;
        remaining -= 1u;
    }
    remaining = count & 3u;
    while (remaining != 0) {
        byte = V8(source);
        V8(out) = byte;
        source += 1u;
        out += 1u;
        remaining -= 1u;
    }

    if (object == 0) {
        /* 007c30bc: this branch reverses the two buffer arguments. */
        (void)cm_fn_00944cff(0x00a78310u, 0, 0,
                             scratch + 0x10c, scratch + 0x0c);
        filename = (u32)(scratch + 0x10c);
        extension = (u32)(scratch + 0x0c);
        line = 0x944u;
        goto report_error;
    }

    index = (u32)(s32)(s8)(u8)selector - 1u;
    if (index > 0x10u)
        goto bad_selector;
    /* 007c30fc: retain the physical DWORD read from the pinned,
       read-only .text jump table, including its original index guard. */
    dispatch = V32(0x007c38dcu + index * 4u);
    switch (dispatch) {
    case 0x007c3103u:
        a = V32(object + 4u);
        if (a != 0 && split != 0) {
            b = V32(object);
            (void)cm_fn_00944c9f(destination, 0x0099cc58u, b - a, a);
            return 1;
        }
        a = V32(object);
        if (a != 0 || show_zero != 0)
            (void)cm_fn_00944c9f(destination, 0x0098806cu, a);
        return 1;

    case 0x007c3207u:
        a = V32(object + 0x10u);
        if (a != 0 && split != 0) {
            if (detail != 0) {
                b = V32(object);
                c = V32(object + 0x14u);
                (void)cm_fn_00944c9f(destination, 0x00a784c0u, a, c, b);
            } else {
                b = V32(object + 0x14u);
                (void)cm_fn_00944c9f(destination, 0x0099cc58u, a, b);
            }
            return 1;
        }
        a = V32(object);
        if (a != 0 || show_zero != 0) {
            b = V32(object + 0x14u);
            if (detail != 0)
                (void)cm_fn_00944c9f(destination, 0x00a784d0u, b, a);
            else
                (void)cm_fn_00944c9f(destination, 0x0098806cu, b);
        }
        return 1;

    case 0x007c315du:
    case 0x007c31b2u:
    case 0x007c32b2u:
    case 0x007c3307u:
    case 0x007c335cu:
    case 0x007c33b1u:
    case 0x007c3406u:
    case 0x007c345bu:
    case 0x007c34b0u:
    case 0x007c3505u:
        /* Table entries 2,3,5..12 have the same memory/call sequence;
           2/3 use offsets 08/0c; 5..12 use 18..34 (selector 4 has two fields). */
        offset = (index + (index >= 4u ? 2u : 1u)) * 4u;
        a = V32(object);
        if (a != 0 || show_zero != 0) {
            b = V32(object + offset);
            if (detail != 0)
                (void)cm_fn_00944c9f(destination, 0x00a784d0u, b, a);
            else
                (void)cm_fn_00944c9f(destination, 0x0098806cu, b);
        }
        return 1;

    case 0x007c355au:
        a = V32(object);
        if (a == 0) {
            /* 007c35c8: a second scan/copy, even after the initial copy.
               Keep DWORD copies before the residual byte copies. */
            source = 0x0099cc4cu;
            scan = source;
            remaining = 0xffffffffu;
            while (remaining != 0) {
                byte = V8(scan);
                scan += 1u;
                remaining -= 1u;
                if (byte == 0)
                    break;
            }
            count = ~remaining;
            source = scan - count;
            out = destination;
            remaining = count >> 2;
            while (remaining != 0) {
                word = V32(source);
                V32(out) = word;
                source += 4u;
                out += 4u;
                remaining -= 1u;
            }
            remaining = count & 3u;
            while (remaining != 0) {
                byte = V8(source);
                V8(out) = byte;
                source += 1u;
                out += 1u;
                remaining -= 1u;
            }
            return 1;
        }
        /* FILD precedes the original local denominator stores. */
        t = (long double)(s32)V32(object + 0x38u);
        LOCAL32(4) = a;
        LOCAL32(8) = 0;
        t = t / (long double)(s32)LOCAL32(4);
        rounded = (double)t; /* 007c3578 / 007c35a9: FSTP m64. */
        if (detail != 0)
            (void)cm_fn_00944c9f(destination, 0x00a784b4u, rounded, a);
        else
            (void)cm_fn_00944c9f(destination, 0x0098fe94u, rounded);
        return 1;

    case 0x007c35f8u:
        denominator = (s32)(s16)V16(object + 0x3cu);
        if (denominator <= 0x13)
            return 1;
        if (detail != 0) {
            a = V32(object);
            numerator = (s32)(s16)V16(object + 0x3eu);
            LOCAL32(0) = (u32)numerator;
            t = (long double)(s32)LOCAL32(0);
            t = t * (long double)CM_GLOBAL(volatile float, 0x00968960u);
            LOCAL32(0) = (u32)denominator;
            t = t / (long double)(s32)LOCAL32(0);
            /* Original __ftol call site 007c362b; only low EAX is pushed. */
            converted = (long long)t;
            (void)cm_fn_00944c9f(destination, 0x00a784a8u, (u32)converted, a);
        } else {
            numerator = (s32)(s16)V16(object + 0x3eu);
            LOCAL32(0) = (u32)numerator;
            t = (long double)(s32)LOCAL32(0);
            t = t * (long double)CM_GLOBAL(volatile float, 0x00968960u);
            LOCAL32(0) = (u32)denominator;
            t = t / (long double)(s32)LOCAL32(0);
            /* Original __ftol call site 007c366a; only low EAX is pushed. */
            converted = (long long)t;
            (void)cm_fn_00944c9f(destination, 0x0098e8d4u, (u32)converted);
        }
        return 1;

    case 0x007c368cu:
    case 0x007c36ecu:
        a = V32(object + 0x48u);
        LOCAL32(0) = a;
        if ((s32)a < 0x96)
            return 1;
        offset = dispatch == 0x007c368cu ? 0x40u : 0x42u;
        if (detail != 0) {
            a = V32(object);
            numerator = (s32)(s16)V16(object + offset);
            LOCAL32(4) = (u32)numerator;
            t = (long double)(s32)LOCAL32(4);
            t = t * (long double)CM_GLOBAL(volatile float, 0x0096d848u);
            t = t / (long double)(s32)LOCAL32(0);
            rounded = (double)t; /* Original FSTP m64 at 007c36bf. */
            (void)cm_fn_00944c9f(destination, 0x00a7849cu, rounded, a);
        } else {
            numerator = (s32)(s16)V16(object + offset);
            LOCAL32(4) = (u32)numerator;
            t = (long double)(s32)LOCAL32(4);
            t = t * (long double)CM_GLOBAL(volatile float, 0x0096d848u);
            t = t / (long double)(s32)LOCAL32(0);
            rounded = (double)t; /* Original FSTP m64 at 007c3729. */
            (void)cm_fn_00944c9f(destination, 0x0099ccc8u, rounded);
        }
        return 1;

    case 0x007c3748u:
        sample = (s16)V16(object + 0x44u);
        if (sample <= 0x18) {
            if (sample > 4) {
                a = V32(object + 0x48u);
                /* Positive denominator 5..24 makes this IDIV well-defined,
                   including the original signed negative-numerator cases. */
                if ((s32)a / (s32)sample <= 0x2d)
                    goto sample_ready;
            }
            /* Keep this second load; it is the original 007c3768 CMP. */
            a = V32(object + 0x48u);
            if ((s32)a <= 0x21c)
                return 1;
            if (sample <= 0x0a)
                return 1;
        }
    sample_ready:
        if (detail != 0) {
            a = V32(object);
            numerator = (s32)(s16)V16(object + 0x46u);
            LOCAL32(4) = (u32)numerator;
            t = (long double)(s32)LOCAL32(4);
            t = t * (long double)CM_GLOBAL(volatile float, 0x00968960u);
            LOCAL32(4) = (u32)(s32)sample;
            t = t / (long double)(s32)LOCAL32(4);
            /* Original __ftol call site 007c37a4; only low EAX is pushed. */
            converted = (long long)t;
            (void)cm_fn_00944c9f(destination, 0x00a784a8u, (u32)converted, a);
        } else {
            numerator = (s32)(s16)V16(object + 0x46u);
            LOCAL32(4) = (u32)numerator;
            t = (long double)(s32)LOCAL32(4);
            t = t * (long double)CM_GLOBAL(volatile float, 0x00968960u);
            LOCAL32(4) = (u32)(s32)sample;
            t = t / (long double)(s32)LOCAL32(4);
            /* Original __ftol call site 007c37e3; only low EAX is pushed. */
            converted = (long long)t;
            (void)cm_fn_00944c9f(destination, 0x0098e8d4u, (u32)converted);
        }
        return 1;
    }
    /* All seventeen pinned read-only table targets are handled above. */
    return 1;

bad_selector:
    (void)cm_fn_00944cff(0x00a78310u, 0, 0,
                         scratch + 0x0c, scratch + 0x10c);
    filename = (u32)(scratch + 0x0c);
    extension = (u32)(scratch + 0x10c);
    line = 0xa50u;
    goto report_error;

bad_arguments:
    (void)cm_fn_00944cff(0x00a78310u, 0, 0,
                         scratch + 0x0c, scratch + 0x10c);
    filename = (u32)(scratch + 0x0c);
    extension = (u32)(scratch + 0x10c);
    line = 0x932u;

report_error:
    /* Native 0090d130 is MOV EAX,00ad4394; RET, with no arguments.
       Filename, extension and line pushes preceding that call belong to
       00944c9f. Its six arguments plus the subsequent three-argument
       005e8290 call account for the original ADD ESP,024. */
    version = cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                         version, filename, extension, line);
    (void)cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    V32(0x00b67a34u) = 0;
    return 0;
}

#undef LOCAL32
#undef V32
#undef V16
#undef V8
