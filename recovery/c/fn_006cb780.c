/* fn_006cb780 -- preferred-image VA 0x006cb780, image base 0x00400000 (CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
   thiscall: ECX = object (kept in EBP), one stack DWORD argument at [esp+0x14]
   (loaded into EDI), `ret 4`.

   FULL-EAX AUDIT (every return site in the listing):
     0x006cb793 -> 0x006cb902  `xor al,al` after `test al,al` on cm_fn_0070dfa0.
        AL = 0.  EAX[31:8] untouched by this routine *and* by cm_fn_0070dfa0,
        which only writes AL: the original run of case 16 returns 0x7d558700
        while its entry EAX is 0x7d55878d, i.e. the value is the caller's own
        entry EAX with AL cleared.
     0x006cb7c1 -> 0x006cb902  `mov al,[ecx+edx*2+0x21b6]` over an EAX that
        `xor eax,eax` cleared, so the whole EAX is the loaded byte; `xor al,al`
        then returns exactly 0.
     0x006cb844/0x006cb860 -> 0x006cb902, 0x006cb8c0/0x006cb8d8 -> 0x006cb8f9
        `fnstsw ax` writes AH = x87 status high byte (C0 = 0x01, C3 = 0x40,
        C2 = 0x04), then only AL is rewritten (`xor al,al` / `mov al,2`):
        AH is computed here, EAX[31:16] is whatever the ST0-returning
        cm_fn_006f09b0 left in EAX (not representable across an ST0 boundary).
     0x006cb8f3 `mov al,1` with EAX left by the tail of the second scan
        (0x45B*bank | byte on the `[esi+0x19]` skip, the full DWORD returned by
        cm_fn_00717410 on the `cmp ax,1` skip).

    UNRESOLVED FIDELITY DEFECT: the u8 declaration below validates AL only,
    not the full original EAX contract.  At 0x006cb848/0x006cb864 and
    0x006cb8c4/0x006cb8dc, FNSTSW overwrites AX, preserving the ST0 callee's
    EAX[31:16]; the epilogues overwrite only AL.  Equality on the first scan
    therefore returns C3 in AH rather than a full-width zero.  Inherited
    register bits are observable results too; they cannot be discarded merely
    because this routine did not compute them.  No claim about caller
    zero-extension resolves this contract.

    The declared float callee boundary exposes ST0 but not incidental EAX.
    The first abort also requires inherited entry EAX.  Widening this C
    return alone cannot recover either value.  A faithful repair needs an
    explicit register-state/dual-result boundary (or an authorized ABI
    implementation), beyond this source-only plain-C task.  Keep the hold
    unresolved even if the byte-width differential gate passes. */

#include "cmre.h"

#define B(a) CM_GLOBAL(volatile u8, (a))
#define D(a) CM_GLOBAL(double, (a))

/* Callee boundaries.  `self` is the receiver in ECX; cm_fn_006f09b0 pushes its
   arguments right to left, so `push 0xb; push 4` is (first = 4, second = 0xb). */
extern u8 CM_THISCALL cm_fn_0070dfa0(u32 self);
extern float CM_THISCALL cm_fn_006fa1e0(u32 self);
extern u8 CM_THISCALL cm_fn_006f4650(volatile u8 *self);
extern float CM_THISCALL cm_fn_006f09b0(u32 self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_00717410(u32 self);

u8 CM_THISCALL cm_fn_006cb780(u32 self, u32 person)
{
    u32 a;
    u32 b;
    u32 ecx;
    u32 edx;
    u32 esi;
    u8 flag;
    u8 bl;
    u8 byte19;
    float thr;
    float v;

    /* 006cb78a mov ecx, edi; 006cb78c call 0x70dfa0 */
    if (cm_fn_0070dfa0(person) != 0) {
        /* 006cb793 jne 0x6cb902: xor al,al */
        return 0;
    }

    /* 006cb799 movsx eax, byte ptr [edi+0x102]
       006cb7a0 movsx ecx, byte ptr [edi+0x103]
       006cb7a7 lea eax, [eax+eax*2]; 006cb7ad lea eax, [ecx+eax*4]
       006cb7aa mov bl, byte ptr [edi+0x27] */
    a = (u32)(s32)(s8)B(person + 0x102);
    b = (u32)(s32)(s8)B(person + 0x103);
    flag = B(person + 0x27);
    /* 006cb7b2 lea eax,[eax+eax*4] (x5); 006cb7b5 lea edx,[eax+eax*8] (x9);
       addressing EFLAG [ecx + edx*2 + 0x21b6] doubles it again: (b + a*12) * 90,
       all in wrapping 32-bit arithmetic. */
    edx = (u32)((a * 12u + b) * 45u) * 2u;
    /* 006cb7b8-006cb7bf xor eax,eax; test bl,bl; sete al; add ecx,eax */
    ecx = self + (u32)(flag == 0);
    /* 006cb7c1 mov al, byte ptr [ecx + edx*2 + 0x21b6] */
    if (B(ecx + edx + 0x21b6) != 0) {
        /* 006cb7c8 test al,al; jne 0x6cb902 */
        return 0;
    }

    /* 006cb7d0 mov ecx, edi; 006cb7d2 call 0x6fa1e0 (ST0)
       006cb7d7 fadd qword ptr [0x968968]
       006cb7dd fstp dword ptr [esp+0x14]   (the argument home) */
    v = cm_fn_006fa1e0(person);
    thr = (float)((long double)v + (long double)D(0x00968968));
    /* 006cb7e1 mov ecx, edi; 006cb7e3 call 0x6f4650 */
    if (cm_fn_006f4650((volatile u8 *)person) == 0) {
        /* 006cb7ec fld dword [esp+0x14]; 006cb7f0 fadd qword ptr [0x967890];
           006cb7f6 fstp dword [esp+0x14] */
        thr = (float)((long double)thr + (long double)D(0x00967890));
    }

    /* 006cb7fa mov al, byte ptr [edi+0x27]
       006cb7fd xor ecx,ecx; 006cb7ff test al,al; 006cb801 sete cl
       006cb804 lea eax,[ecx*8]; 006cb80b sub eax,ecx; 006cb80d shl eax,5;
       006cb810 sub eax,ecx  -> ecx * 223
       006cb814 lea edx,[eax+eax*4] -> ecx * 1115
       006cb817 lea esi,[ebp+edx*8+0x4796] -> self + ecx * 8920 + 0x4796 */
    flag = B(person + 0x27);
    ecx = (u32)(flag == 0);
    a = ecx * 8u - ecx;
    a = a << 5;
    a = a - ecx;
    bl = 0;
    edx = a * 5u;
    esi = self + edx * 8u + 0x4796u;

    /* first scan: `test ah,0x41` (C0|C3) aborts with 0 unless value > threshold */
    do {
        /* 006cb81e mov al, byte ptr [esi+0x19]; 006cb821 test al,al; jl */
        byte19 = B(esi + 0x19);
        if ((s8)byte19 >= 0) {
            /* 006cb825 mov ecx, esi; 006cb827 call 0x717410 */
            if ((u16)cm_fn_00717410(esi) != 1) { /* 006cb82c cmp ax,1; je */
                /* 006cb832 mov al, byte ptr [edi+0x27] */
                flag = B(person + 0x27);
                if (flag == 0) {
                    /* 006cb839 push 0xb; 006cb83b push 4; 006cb83f call 0x6f09b0 */
                    v = cm_fn_006f09b0(esi, 4, 0xb);
                }
                else {
                    /* 006cb855 push 0; 006cb857 push 4; 006cb85b call 0x6f09b0 */
                    v = cm_fn_006f09b0(esi, 4, 0);
                }
                /* 006cb844/006cb860 fcomp dword ptr [esp+0x14]; fnstsw ax;
                   test ah,0x41; jne 0x6cb902 -- also taken when unordered */
                if (!((long double)v > (long double)thr)) {
                    return 0;
                }
            }
        }
        /* 006cb86f inc bl; 006cb871 add esi,0x1be; 006cb877 cmp bl,0x14; jl */
        bl = (u8)(bl + 1);
        esi = esi + 0x1be;
    } while ((s8)bl < 20);

    /* 006cb87c movsx ecx, byte ptr [edi+0x27] -- signed, unlike the first scan
       006cb880-006cb88c (ecx*8 - ecx) << 5 - ecx -> ecx * 223
       006cb88e xor bl,bl; 006cb890 lea eax,[eax+eax*4]; 006cb893 lea esi,[...] */
    ecx = (u32)(s32)(s8)B(person + 0x27);
    a = ecx * 8u - ecx;
    a = a << 5;
    a = a - ecx;
    bl = 0;
    a = a * 5u;
    esi = self + a * 8u + 0x4796u;

    /* second scan: `test ah,1` (C0) returns 2 unless value >= threshold */
    do {
        /* 006cb89a mov al, byte ptr [esi+0x19]; 006cb89d test al,al; jl */
        byte19 = B(esi + 0x19);
        if ((s8)byte19 >= 0) {
            /* 006cb8a1 mov ecx, esi; 006cb8a3 call 0x717410 */
            if ((u16)cm_fn_00717410(esi) != 1) { /* 006cb8a8 cmp ax,1; je */
                /* 006cb8ae mov al, byte ptr [edi+0x27] */
                flag = B(person + 0x27);
                if (flag == 0) {
                    /* 006cb8b5 push 0xb; 006cb8b7 push 4; 006cb8b9 call 0x6f09b0 */
                    v = cm_fn_006f09b0(esi, 4, 0xb);
                }
                else {
                    /* 006cb8cd push 0; 006cb8cf push 4; 006cb8d1 call 0x6f09b0 */
                    v = cm_fn_006f09b0(esi, 4, 0);
                }
                /* 006cb8c0/006cb8d8 fcomp dword ptr [esp+0x14]; fnstsw ax;
                   test ah,1; jne 0x6cb8f9 -- unordered also has C0 set */
                if (!((long double)v >= (long double)thr)) {
                    /* 006cb8f9 mov al,2 */
                    return 2;
                }
            }
        }
        /* 006cb8e3 inc bl; 006cb8e5 add esi,0x1be; 006cb8eb cmp bl,0x14; jl */
        bl = (u8)(bl + 1);
        esi = esi + 0x1be;
    } while ((s8)bl < 20);

    /* 006cb8f0 pop edi; pop esi; pop ebp; 006cb8f3 mov al,1; pop ebx; ret 4 */
    return 1;
}
