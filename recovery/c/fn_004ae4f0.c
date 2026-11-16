/* Recovered from the original instructions at preferred-image VA 0x004AE4F0,
   image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

    Convention: one stack argument, ret 0. Caller 0x004a4f7a reads full
    EAX (confirmed by semantic gate's return-width check). Therefore the
    inherited high half is observable. regparm(3) explicitly captures physical
    entry EAX, EDX and ECX while retaining the record at [entry ESP+4].
    Only EAX's captured value is used. Each mov AX merges a low word;
    movsx EAX,AL replaces the entire value on the floating-point path.
    This is an explicit incoming-register boundary, not an uninitialized
    C local or a dependence on compiler register allocation.

   Byte compares against 0x14 use DL; +0x15, +0x17, +0x18 and +0x19 compare
   signed (jl). The x87 block reloads +0x11, +0x12 and +0x14 after the earlier
   compares (0x4AE522/0x4AE526/0x4AE52E) and computes
   ((s32)(s8)+0x14 + (s32)(s8)+0x12) + (s32)(s8)+0x13 through the caller's
   argument home, then compares it with (s32)(s8)+0x11 times the double at
   0x00968940 (fild / fmul qword / fild / fcompp), reading fcompp's C0 back
   through `fnstsw ax / test ah,1`. Field meanings stay unnamed. */
#include "cmre.h"

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_004ae4f0(
    u32 incoming_eax, u32 incoming_edx, u32 incoming_ecx,
    volatile u32 record)
{
    volatile u8 *r = (volatile u8 *)record;
    u32 eax = incoming_eax;
    s8 al;
    s8 dl;
    s32 p11;
    s32 p12;
    s32 p14;
    s32 sum;
    long double product;

    (void)incoming_edx;
    (void)incoming_ecx;
    if (r[0x0f] == 0x14) {
        return (eax & 0xffff0000u) | 1u;
    }
    if (r[0x10] == 0x14 || r[0x11] == 0x14 || r[0x16] == 0x14) {
        al = (s8)r[0x13];
        if ((u8)al != 0x14 && r[0x12] != 0x14 && r[0x14] != 0x14) {
            return (eax & 0xffff0000u) | 4u;
        }
        p11 = (s32)(s8)r[0x11];
        p12 = (s32)(s8)r[0x12];
        record = (u32)p11;
        p14 = (s32)(s8)r[0x14];
        p11 = (s32)record;
        eax = (u32)(s32)al;
        product = (long double)p11 *
                  (long double)CM_GLOBAL(volatile double, 0x00968940);
        sum = p14 + p12;
        sum = sum + (s32)al;
        record = (u32)sum;
        if ((long double)(s32)record < product) {
            return (eax & 0xffff0000u) | 4u;
        }
    }
    if (r[0x13] != 0x14 && r[0x12] != 0x14 && r[0x14] != 0x14) {
        return (eax & 0xffff0000u) | 0x40u;
    }
    if ((s8)r[0x15] < 0x14) {
        return (eax & 0xffff0000u) | 0x10u;
    }
    al = (s8)r[0x19];
    dl = (s8)r[0x18];
    if (al < dl) {
        return (eax & 0xffff0000u) | 0x10u;
    }
    if (al < (s8)r[0x17]) {
        return (eax & 0xffff0000u) | 0x10u;
    }
    return (eax & 0xffff0000u) | 0x40u;
}
