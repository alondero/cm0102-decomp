/* CM0102 3.9.68; preferred-image VA 004426e0, base 00400000.
 * Image SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Cdecl: three DWORD slots, signed FILD qword input and low-byte flags;
 * full EDX:EAX result. ADR0006 registered __ftol casts below.
 * Volatile doubles preserve original rounded scratch stores. arg.d represents
 * the overwritten incoming qword slot, after its signed FILD has consumed it.
 * !(x > t) includes unordered, exactly FNSTSW AH & 0x41 != 0.
 */
#include "cmre.h"
#define M CM_GLOBAL(volatile double, 0x00ad9cc0)
#define K(va) CM_GLOBAL(volatile double, va)
__attribute__((target("no-sse3")))
unsigned long long cm_fn_004426e0(u32 low, u32 high, u32 flags)
{
    volatile union { unsigned long long u; double d; } arg;
    volatile double s0, s8, s16;
    long double x, t, value;
    if (!(flags & 4) && !(flags & 8)) return 1000;
    arg.u = ((unsigned long long)high << 32) | low;
    x = (long double)(long long)arg.u;
    if (flags & 4) {
        t = (long double)M;
        t *= (long double)K(0x00968910);
        s8 = (double)t;
        t = (long double)s8;
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x00968908);
            return (unsigned long long)(long long)value; /* __ftol 0x0044271c */
        }
        t = (long double)M;
        t *= (long double)K(0x00968900);
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x009688f8);
            return (unsigned long long)(long long)value; /* __ftol 0x0044274a */
        }
        t = (long double)M;
        t *= (long double)K(0x009688f0);
        arg.d = (double)t;
        t = (long double)arg.d;
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x009688e8);
            return (unsigned long long)(long long)value; /* __ftol 0x0044277c */
        }
        t = (long double)M;
        t *= (long double)K(0x009688e0);
        s0 = (double)t;
        t = (long double)s0;
        if (!(x > t)) {
            value = (long double)s8;
            return (unsigned long long)(long long)value; /* __ftol 0x004427a6 */
        }
        t = (long double)M;
        t *= (long double)K(0x009688d8);
        s8 = (double)t;
        t = (long double)s8;
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x009688d0);
            return (unsigned long long)(long long)value; /* __ftol 0x004427d8 */
        }
        t = (long double)M;
        t *= (long double)K(0x009688c8);
        if (!(x > t)) {
            value = (long double)arg.d;
            return (unsigned long long)(long long)value; /* __ftol 0x004427fe */
        }
        t = (long double)M;
        t *= (long double)K(0x009688c0);
        if (!(x > t)) {
            value = (long double)s0;
            return (unsigned long long)(long long)value; /* __ftol 0x00442824 */
        }
    } else {
        t = (long double)M;
        t *= (long double)K(0x009688b0);
        arg.d = (double)t;
        t = (long double)arg.d;
        if (!(x > t)) {
            value = (long double)M;
            return (unsigned long long)(long long)value; /* __ftol 0x00442884 */
        }
        t = (long double)M;
        t *= (long double)K(0x009688a8);
        s8 = (double)t;
        t = (long double)s8;
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x009688a0);
            return (unsigned long long)(long long)value; /* __ftol 0x004428b6 */
        }
        t = (long double)M;
        t *= (long double)K(0x00968898);
        s0 = (double)t;
        t = (long double)s0;
        if (!(x > t)) {
            value = (long double)arg.d;
            return (unsigned long long)(long long)value; /* __ftol 0x004428e0 */
        }
        t = (long double)M;
        t *= (long double)K(0x00968908);
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x00968890);
            return (unsigned long long)(long long)value; /* __ftol 0x0044290e */
        }
        t = (long double)M;
        t *= (long double)K(0x009688e8);
        s16 = (double)t;
        t = (long double)s16;
        if (!(x > t)) {
            value = (long double)s8;
            return (unsigned long long)(long long)value; /* __ftol 0x00442938 */
        }
        t = (long double)M;
        t *= (long double)K(0x00968888);
        if (!(x > t)) {
            value = (long double)s0;
            return (unsigned long long)(long long)value; /* __ftol 0x0044295e */
        }
        t = (long double)M;
        t *= (long double)K(0x00968880);
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x00968878);
            return (unsigned long long)(long long)value; /* __ftol 0x0044298c */
        }
        t = (long double)M;
        t *= (long double)K(0x009688f0);
        arg.d = (double)t;
        t = (long double)arg.d;
        if (!(x > t)) {
            value = (long double)s16;
            return (unsigned long long)(long long)value; /* __ftol 0x004429b6 */
        }
        t = (long double)M;
        t *= (long double)K(0x009688e0);
        s0 = (double)t;
        t = (long double)s0;
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x00968910);
            return (unsigned long long)(long long)value; /* __ftol 0x004429e8 */
        }
        t = (long double)M;
        t *= (long double)K(0x009688d8);
        s8 = (double)t;
        t = (long double)s8;
        if (!(x > t)) {
            value = (long double)M;
            value *= (long double)K(0x009688d0);
            return (unsigned long long)(long long)value; /* __ftol 0x00442a1a */
        }
        t = (long double)M;
        t *= (long double)K(0x009688c8);
        if (!(x > t)) {
            value = (long double)arg.d;
            return (unsigned long long)(long long)value; /* __ftol 0x00442a40 */
        }
        t = (long double)M;
        t *= (long double)K(0x009688c0);
        if (!(x > t)) {
            value = (long double)s0;
            return (unsigned long long)(long long)value; /* __ftol 0x00442a66 */
        }
    }
    t = (long double)M;
    t *= (long double)K(0x009688b8);
    if (x > t) {
        value = (long double)s8;
        return (unsigned long long)(long long)value; /* __ftol 0x0044284c */
    }
    value = (long double)M;
    value *= (long double)K(0x00968870);
    return (unsigned long long)(long long)value; /* __ftol 0x00442a96 */
}
