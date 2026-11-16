/* fn_0085d8a0 - reconstructed in plain C from the disassembly listing at
   preferred-image VA 0x0085d8a0 (image base 0x00400000).

   The original runs on x87 with the control word the fixtures carry (0x037f):
   every value lives in an 80-bit register and is rounded to a double only
   where the listing stores to memory (fstp qword).  Each of those rounding
   points is reproduced here with an explicit `double` local and `long double`
   intermediates, so the 64-bit significand of the subtractions, products,
   sum, quotient and squarings is reproduced in listing order.

   Retraction fix (evidence-needed hold, provenance "finite-coordinate FSQRT
   mismatch", instruction 0x0085d8e3): the square root is the original's FSQRT
   - the operand's exact square root rounded once, to the 64-bit extended
   significand - and not a Newton-Raphson approximation.  cm_sqrt_ext below
   documents the construction; the withdrawn revision stopped at approximately
   80-bit accuracy, which is a different double after the 0085d8e5 store.

   The listing pops the FPU stack with `fstp st(0)`, which is FSTP ST(0)
   (DD D8): the store is a no-op and the pop shifts the stack up, so the
   quotient that FDIVP ST(2), ST(0) leaves in ST(1) survives that pop and
   stays live, while the copy of it that was in ST(0) is what disappears.
   The value carried on to the clamps is therefore the quotient.

   The return is the value left in ST(0), which the gate compares as a raw
   80-bit register, so the fall-through path returns the 80-bit `value`
   unchanged rather than a narrowed copy. */

#include "cmre.h"

/* ---- x87 FSQRT in plain C, without a math library ----------------------
   0085d8e3 is the hardware square root: the exact square root of the 80-bit
   operand, correctly rounded to the 64-bit extended significand selected by
   the 0x037f control word.  The gate's C lane links neither libm (`sqrtl` is
   undefined) nor the wide soft-float helpers, so the operation is written out
   in the target's own 80-bit arithmetic:

     1. an extended-precision Newton estimate y of sqrt(m);
     2. the exact residual m - y*y, carried as an unevaluated sum of two
        64-bit-significand numbers (Dekker/Veltkamp two-product and Knuth
        two-sum, both exact in this format);
     3. one correction delta = residual / (2y), also carried unevaluated;
     4. a single rounding of y + delta to the 64-bit significand, which is the
        rounding FSQRT performs.

   After step 3 the carried value differs from the exact square root by about
   2^-126 relative, so step 4 returns the correctly rounded extended square
   root exactly as FSQRT does; the only operands that could round differently
   are those whose exact root lies within 2^-126 relative of a midpoint of the
   extended format.  No approximation is returned: the estimate in step 1 only
   seeds the exact residual correction. */

/* s + e == a + b exactly (Knuth's two-sum). */
static void cm_two_sum(long double a, long double b, long double *s,
                       long double *e)
{
    long double t = a + b;
    long double bv = t - a;

    *s = t;
    *e = (a - (t - bv)) + (b - bv);
}

/* p + e == a * b exactly (Dekker/Veltkamp two-product): the 2^32+1 split
   keeps every partial product within the 64-bit significand. */
static void cm_two_prod(long double a, long double b, long double *p,
                        long double *e)
{
    long double c = a * 4294967297.0L;
    long double ahi = c - (c - a);
    long double alo = a - ahi;
    long double d = b * 4294967297.0L;
    long double bhi = d - (d - b);
    long double blo = b - bhi;

    *p = a * b;
    *e = ((ahi * bhi - *p) + ahi * blo + alo * bhi) + alo * blo;
}

static long double cm_sqrt_ext(long double x)
{
    long double m;
    long double y;
    long double p;
    long double ep;
    long double s1;
    long double e1;
    long double resid;
    long double delta;
    long double s;
    long double e;
    int k = 0;
    int i;

    /* FSQRT(+0) and FSQRT(-0) are that zero; FSQRT of a NaN is a NaN.  A
       negative operand is unreachable: x is dy * dy + dx * dx. */
    if (!(x > 0.0L))
        return x;
    if (x * 0.5L == x)                 /* +infinity */
        return x;

    /* x = m * 4^k with 1 <= m < 4; both directions are exact. */
    m = x;
    while (m >= 4.0L) { m = m * 0.25L; k = k + 1; }
    while (m < 1.0L) { m = m * 4.0L; k = k - 1; }

    /* Extended-precision Newton estimate; quadratic from a start whose
       relative error is at most one quarter. */
    y = (m + 1.0L) * 0.5L;
    for (i = 0; i < 6; i++)
        y = (y + m / y) * 0.5L;

    /* Exact residual m - y*y.  The two-product gives y*y == p + ep and the
       two-sum gives m - p == s1 + e1, so the residual is (m - p) - ep and both
       parts are carried without rounding. */
    cm_two_prod(y, y, &p, &ep);
    cm_two_sum(m, -p, &s1, &e1);
    resid = s1 + (e1 - ep);

    /* One exact-residual correction step. */
    delta = resid / (y + y);
    cm_two_sum(y, delta, &s, &e);

    /* Scale back by 2^k; exact, and it commutes with the final rounding. */
    while (k > 0) { s = s + s; e = e + e; k = k - 1; }
    while (k < 0) { s = s * 0.5L; e = e * 0.5L; k = k + 1; }

    /* The single rounding to the 64-bit significand: FSQRT's own rounding. */
    return s + e;
}

long double CM_STDCALL cm_fn_0085d8a0(
    volatile u8 * volatile object, volatile u8 *point, volatile u8 *other,
    volatile double incoming)
{
    volatile u8 *obj = object;
    s32 scale;
    u16 flag30;
    u8 flag3a;
    u8 flag3c;
    u32 owner;
    u32 same_owner;
    double loc0;      /* stack slot L+0: the distance, rounded to a double */
    double loc8;      /* stack slot L+8: the byte field, rounded to a double */
    long double dx;
    long double dy;
    long double value;

    /* 0085d8ac  mov eax, dword ptr [ecx + 0x20] */
    owner = *(volatile u32 *)(obj + 0x20);
    /* 0085d8af  cmp word ptr [ecx + 0x30], 0xa0 */
    flag30 = *(volatile u16 *)(obj + 0x30);
    /* 0085d8b5  mov edx, dword ptr [eax + 0x53] */
    same_owner = *(volatile u32 *)(owner + 0x53);
    /* 0085d8b8  movsx eax, byte ptr [edx + 0x7e] */
    scale = (s32)*(volatile s8 *)(same_owner + 0x7e);

    /* 0085d8bc  mov dword ptr [esp + 0x18], eax   (the incoming arg1 slot)
       0085d8c4  fild dword ptr [esp + 0x18]        (reload of that slot)
       0085d8c8  fstp qword ptr [esp + 0xc]         (L+8, rounded to double) */
    object = (volatile u8 *)(u32)scale;
    loc8 = (double)(long double)(s32)(u32)object;

    /* 0085d8cc  fld qword ptr [eax + 0x2b] */
    dx = (long double)*(volatile double *)(point + 0x2b);
    /* 0085d8cf  fsub qword ptr [esi + 0x2b] */
    dx = dx - (long double)*(volatile double *)(other + 0x2b);
    /* 0085d8d2  fld qword ptr [eax + 0x23] */
    dy = (long double)*(volatile double *)(point + 0x23);
    /* 0085d8d5  fsub qword ptr [esi + 0x23] */
    dy = dy - (long double)*(volatile double *)(other + 0x23);

    /* 0085d8d9  fld st(0) / fmul st(1)      -> dy * dy
       0085d8dd  fld st(2) / fmul st(3)      -> dx * dx
       0085d8e1  faddp st(1)                 -> dy * dy + dx * dx
       0085d8e3  fsqrt                       -> FSQRT, 64-bit significand
       0085d8e5  fstp qword ptr [esp]        -> rounded to a double */
    loc0 = (double)cm_sqrt_ext(dy * dy + dx * dx);

    /* 0085d8ed  jne 0x85d903 (from cmp word ptr [ecx + 0x30], 0xa0)
       0085d8ef  cmp byte ptr [ecx + 0x3a], 5
       0085d8f5  fld qword ptr [esp] / fmul qword ptr [0x968968]
       0085d8ff  fstp qword ptr [esp] */
    if (flag30 == 0xa0 && *(volatile u8 *)(obj + 0x3a) == 5) {
        loc0 = (double)((long double)loc0 *
                        (long double)CM_GLOBAL(volatile double, 0x968968));
    }

    /* 0085d903  fld qword ptr [esp] / fmul qword ptr [0x96afe0] */
    value = (long double)loc0 *
            (long double)CM_GLOBAL(volatile double, 0x96afe0);
    /* 0085d90d  mov al, byte ptr [ecx + 0x3c] / test al, al */
    flag3c = *(volatile u8 *)(obj + 0x3c);
    /* 0085d912  fsubr qword ptr [0x967890] */
    value = (long double)CM_GLOBAL(volatile double, 0x967890) - value;
    /* 0085d91a  fld st(0) / 0085d91c  fmulp st(1) */
    if (flag3c == 0) {
        value = value * value;
    }

    /* 0085d91e  fld qword ptr [esp] / 0085d922  fcomp qword ptr [0x9689a0]
       0085d92a  test ah, 0x41 / 0085d92d  jne 0x85d952   (skip unless >)
       0085d92f  fld qword ptr [esp + 8] / 0085d933  fcomp qword ptr [0x9689a0]
       0085d93b  test ah, 0x41 / 0085d93e  jne 0x85d952   (skip unless >) */
    if (loc0 > CM_GLOBAL(volatile double, 0x9689a0)) {
        if (loc8 > CM_GLOBAL(volatile double, 0x9689a0)) {
            /* 0085d940  fld qword ptr [esp + 8]
               0085d944  fadd qword ptr [0x967890]     -> c1
               0085d94a  fld st(0) / 0085d94c  fmul st(1) -> c1 * c1
               0085d94e  fdivp st(2)                    -> value / (c1 * c1)
               0085d950  fstp st(0)                     -> the quotient stays */
            long double c1 = (long double)loc8 +
                             (long double)CM_GLOBAL(volatile double, 0x967890);
            value = value / (c1 * c1);
        }
    }

    /* 0085d952  fld qword ptr [esp + 0x20] / 0085d956  fcomp qword ptr [0x969558]
       0085d95e  test ah, 0x41 / 0085d961  jne 0x85d994   (enter unless <=) */
    if (incoming > CM_GLOBAL(volatile double, 0x969558)) {
        /* 0085d963  fld qword ptr [0x968ea8]
           0085d969  fsub qword ptr [esp + 0x20]
           0085d96d  fmul qword ptr [0x968f20]
           0085d973  fsubr qword ptr [0x967890]
           0085d979  fstp qword ptr [esp + 0x20] */
        long double tmp =
            (long double)CM_GLOBAL(volatile double, 0x968ea8) -
            (long double)incoming;
        tmp = tmp * (long double)CM_GLOBAL(volatile double, 0x968f20);
        tmp = (long double)CM_GLOBAL(volatile double, 0x967890) - tmp;
        incoming = (double)tmp;
        /* 0085d97d  fcom qword ptr [esp + 0x20]
           0085d983  test ah, 1 / 0085d986  je 0x85d994  (take unless >=) */
        if (!(value >= (long double)incoming)) {
            /* 0085d988  fstp st(0) / 0085d98a  fld qword ptr [esp + 0x20] */
            return (long double)incoming;
        }
    }

    /* 0085d994  cmp byte ptr [ecx + 0x3a], 0xa
       0085d99a  fcom qword ptr [0x968918]
       0085d9a2  test ah, 1 / 0085d9a5  je 0x85d9b5  (take unless >=) */
    flag3a = *(volatile u8 *)(obj + 0x3a);
    if (flag3a == 10) {
        if (!(value >= (long double)CM_GLOBAL(volatile double, 0x968918))) {
            /* 0085d9a7  fstp st(0) / 0085d9a9  fld qword ptr [0x968918] */
            return (double)CM_GLOBAL(volatile double, 0x968918);
        }
    }

    /* 0085d9b5  fcom qword ptr [0x967888]
       0085d9bd  test ah, 1 / 0085d9c0  je 0x85d9d8  (take unless >=)
       0085d9c2  mov ecx, dword ptr [ecx + 0x1c]
       0085d9c5  cmp dword ptr [ecx + 0x53], edx */
    if (!(value >= (long double)CM_GLOBAL(volatile double, 0x967888))) {
        if (*(volatile u32 *)(*(volatile u32 *)(obj + 0x1c) + 0x53) ==
            same_owner) {
            /* 0085d9ca  fstp st(0) / 0085d9cc  fld qword ptr [0x967888] */
            return (double)CM_GLOBAL(volatile double, 0x967888);
        }
    }

    /* 0085d9d8  fcom qword ptr [0x96d2c8]
       0085d9e0  test ah, 1 / 0085d9e3  je 0x85d9ed  (take unless >=) */
    if (!(value >= (long double)CM_GLOBAL(volatile double, 0x96d2c8))) {
        /* 0085d9e5  fstp st(0) / 0085d9e7  fld qword ptr [0x96d2c8] */
        return (double)CM_GLOBAL(volatile double, 0x96d2c8);
    }

    return value;
}
