/* OFFLINE, UNBUILT: original preferred-image VA 0x005968d0, 5289 bytes.
   CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Instruction source: ../native-root-listing.txt, contiguous 1445 sites.
   No acceptance claim: ADR0006 per-producer sensitivity remains pending.
   Root ABI: ECX receiver, two DWORD stack slots, RET 8, full EAX result.
   All indexed continuations in the retained evidence use EAX, none EDX.
   Stack names below refer to B after all four register saves (entry ESP-44).
   r is the live x87 value; rd is the separate rounded B+3c double spill.
   Original x87 operations stay separate and use original memory constants.
   All original noncompiler callees remain explicit boundaries. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_004d0c20(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053d7f0(volatile u32 *object);
extern u32 CM_CDECL cm_fn_00547790(u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00547810(u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 date_low, u32 date_high);
extern u8 CM_THISCALL cm_fn_005986b0(u32 self);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag,
                                 u32 person, u32 force);

#define R8(a) (*(volatile u8 *)(u32)(a))
#define R16(a) (*(volatile u16 *)(u32)(a))
#define R32(a) (*(volatile u32 *)(u32)(a))
#define RS8(a) ((s32)(s8)R8(a))
#define RS16(a) ((s32)(s16)R16(a))
#define RS32(a) ((s32)R32(a))
/* Preserve each original FILD m64 integer source as one eight-byte read.
 * GCC's ordinary volatile long-long load splits into two DWORD reads on this
 * 32-bit target, changing late/fault access width. This source-only load uses
 * XMM bit movement, never floating interpretation, arithmetic, MMX or writeback.
 * noipa preserves the original read/call ordering at all three R64 sites.
 * Emission must retain one MOVLPS/MOVQ memory read at both supported builds. */
typedef float cm_qword_v2sf __attribute__((vector_size(8), may_alias, aligned(1)));
typedef float cm_qword_v4sf __attribute__((vector_size(16)));
__attribute__((noinline, noipa, target("sse2")))
static long long cm_read_i64_qword(u32 address)
{
    union { cm_qword_v4sf vector; unsigned long long bits[2]; } value;
    value.vector = __builtin_ia32_loadlps((cm_qword_v4sf){0,0,0,0},
                                        (const cm_qword_v2sf *)address);
    return (long long)value.bits[0];
}
#define R64(a) cm_read_i64_qword((u32)(a))
#define D(a) CM_GLOBAL(volatile double, a)
#define F(a) CM_GLOBAL(volatile float, a)
#define G(a) CM_GLOBAL(volatile u32, a)

u32 CM_THISCALL cm_fn_005968d0(u32 self, u32 person, u32 flags)
{
    u32 base, index, club, table, comp, id, p, q, a, b, c;
    u32 v, w, extra, minimum, related, entry_cast, normalized;
    u32 manager, target, lo, hi, date_low, date_high;
    u32 cached_360, cached_488, factor_address;
    s32 n, k, divisor, rep, rep2, class_value;
    unsigned long long bits;
    long double t, r, f, threshold, other;
    volatile double rd, rounded, rounded2;

    /* 5968db..5968e3: __ftol, low DWORD of the signed64 conversion. */
    t = (long double)R64(self);
    t = t * (long double)D(0x00968de8u);
    entry_cast = (u32)(long long)t; /* original __ftol 0x005968e3 */
    base = G(0x00ae23b4u);
    index = R32(self + 8u);
    minimum = 100u;                  /* B+2c */
    rd = 0.25;                      /* literal B+3c bits 3fd0000000000000 */
    a = base + index;
    b = ((index + index * 8u) * 16u + index) * 4u;
    c = R8(a + b + 0x64u);
    related = 0u;                   /* B+30 */
    club = a + b;                   /* B+1c: wrapped index*581 */
    if ((u8)c == 3u) return 0u;
    normalized = (u32)(RS16(club + 0x80u) / 50);
    a = cm_fn_0053b900((u32 *)club);
    if (a == 0u) class_value = (s32)(s8)cm_fn_005986b0(self);
    else class_value = 0;           /* B+13 */

    comp = R32(club + 0x53u);
    if (comp == 0u) goto table_8498;
    id = R32(comp);
    if (id == G(0x009cf510u) || id == G(0x009cf2dcu) ||
        id == G(0x009cf3e4u) || id == G(0x009cf224u)) goto table_8498;
    if (id == G(0x009cf36cu) || id == G(0x009cf4a8u) ||
        id == G(0x009cf2acu) || id == G(0x009cf430u) ||
        id == G(0x009cf408u) || id == G(0x009cf4f8u) ||
        id == G(0x009cf434u) || id == G(0x009cf2c0u) ||
        id == G(0x009cf2fcu) || id == G(0x009cf45cu) ||
        id == G(0x009cf314u)) {
        table = 0x009c8548u;
    } else if (id == G(0x009cf300u)) {
        table = 0x009c85f8u;
    } else if (R8(club + 0x82u) == 0u || class_value == 2 || class_value == 1) {
        table = 0x009c86a8u;
    } else {
        table = 0x009c85f8u;
    }
    goto table_ready;
table_8498:
    table = 0x009c8498u;
table_ready:
    /* EDI remains full DWORD until the explicit MOVSX DI below. */
    if (class_value == 0) {
        rep = RS16(club + 0x80u);
        if (rep > 0x1676) {
            if (table == 0x009c86a8u) normalized = (u32)((rep - 0x186a) / 25 + 0x73);
            else normalized += 5u;
        }
    } else if (class_value == 1) {
        rep = RS16(club + 0x80u);
        if (rep <= 0x1482) normalized += 5u;
        else if (table == 0x009c86a8u) normalized = (u32)((rep - 0x1482) / 25 + 0x69);
        else normalized += 10u;
    } else if (class_value == 2) {
        rep = RS16(club + 0x80u);
        if (rep <= 0x128e) normalized += 10u;
        else if (table == 0x009c86a8u) normalized = (u32)((rep - 0x128e) / 25 + 0x5f);
        else normalized += 15u;
    }
    t = (long double)(s16)normalized;
    t = t * (long double)D(0x0096a5e0u);
    v = (u32)(long long)t; /* original __ftol 0x00596b1a; B+14 DWORD */
    if ((s16)v > 10000) v = 10000u;
    if ((s16)normalized > 210) normalized = 210u;
    a = cm_fn_00600a50(club, 0u, 1u, 0u, 0u);
    if (a != 0u && (u8)flags == 0xffu) {
        manager = G(0x00ae1050u);
        related = cm_fn_004cf5f0(manager, person, club);
        if (related != 0u) flags = (flags & 0xffffff00u) | (R8(related + 0x4fu) >> 4);
    }
    if (person == 0u) goto no_person_bound;

    a = R32(person + 0x39u);
    w = 0u;
    if (a == club) {
        manager = G(0x00ae1050u);
        w = cm_fn_004d0c20(manager, person);
        a = R32(self + 0x14u) << 1;
        if ((s32)w > (s32)a) w = a;
    }
    a = R32(self + 0x14u);
    if ((s32)a > (s32)w) w = a;
    a = R32(person + 0x69u);
    if (a == 0u) goto extra_start;
    a = R32(person + 0x61u);
    if (a != 0u) goto extra_start;
    index = (u32)RS8(person + 0x3du) - 5u;
    if (index > 5u) {
        if ((s32)w > 750) w = 750u;
        goto extra_start;
    }
    /* 596be9: preserve the guarded original DWORD table read.
       Pinned six targets: bf0,bf0,bf0,bfe,bf7,c93. */
    target = R32(0x00597d7cu + index * 4u);
    (void)target;
    switch (index) {
    case 0: case 1: case 2: a = 35000u; break;
    case 3: a = 20000u; break;
    case 4: a = 1500u; break;
    case 5: a = 1000u; break;
    }
    if ((s32)w > (s32)a) w = a;
    goto extra_start;

no_person_bound:
    comp = R32(club + 0x53u);
    if (comp == 0u) {
        r = (long double)rd;
        goto ratio_ready;
    }
    k = RS8(comp + 0x7eu);
    if (k < 1) k = 1;
    else if (k > 3) k = 3;
    n = RS8(comp + 0x85u);
    r = (long double)n;
    r = r / (long double)(k * 20);
    rd = (double)r;                  /* 596d0e FST: retain r, do not reload. */
    if (k == 1) {
        id = R32(comp);
        if (id == G(0x009cf2e4u) || id == G(0x009cf360u) ||
            id == G(0x009cf488u) || id == G(0x009cf36cu)) {
            f = (long double)D(0x00967890u);
        } else if (id == G(0x009cf4f8u)) {
            f = (long double)D(0x00969040u);
        } else {
            if (id == G(0x009cf314u) || id == G(0x009cf300u) ||
                id == G(0x009cf45cu) || id == G(0x009cf338u)) factor_address = 0x0096a5d0u;
            else if (id == G(0x009cf260u) || id == G(0x009cf218u) ||
                     id == G(0x009cf4a8u) || id == G(0x009cf244u) ||
                     id == G(0x009cf434u)) factor_address = 0x0096a5d8u;
            else if (id == G(0x009cf2c0u) || id == G(0x009cf408u) ||
                     id == G(0x009cf2fcu)) factor_address = 0x00969510u;
            else factor_address = 0x00968e80u;
            f = (long double)(*(volatile double *)factor_address);
            if ((s32)(s16)normalized > n * 10) {
                divisor = n >= 1 ? n : 1;
                f = (long double)(s16)normalized;
                f = f / (long double)divisor;
                f = f * (long double)D(0x00967880u);
                f = f * (long double)(*(volatile double *)factor_address);
                if (factor_address == 0x00969510u) {
                    if (f > (long double)D(0x00969040u)) {
                        f = (long double)D(0x00969040u);
                        goto factor_ready; /* replacement skips the common cap */
                    }
                }
                if (f > (long double)D(0x00967890u)) f = (long double)D(0x00967890u);
            }
        }
factor_ready:
        r = f * r;
        rd = (double)r;              /* 596f1b FST: live r survives. */
    }
    p = R32(club + 0x57u);
    if (p != 0u) {
        q = R32(club + 0x5bu);
        if (q != 0u) {
            rep = RS16(q + 0x69u);
            rep2 = RS16(p + 0x69u);
            if (rep > rep2) {
                a = (u32)rep2;
                a = a * a * (u32)rep2; /* two IMUL low32 operations */
                t = (long double)(s32)a;
                t = t * r;
                t = t * (long double)D(0x0096a5c8u); /* negative 1.25 */
                b = (u32)(long long)t; /* original __ftol 0x00596f60 */
                a = (u32)rep;
                a = a * a * (u32)rep;
                t = (long double)(s32)a;
                t = t * r;
                t = t * (long double)D(0x00968928u); /* positive 1.25 */
                a = (u32)(long long)t; /* original __ftol 0x00596f83 */
                minimum = a - b;
                goto clamp_minimum;
            }
        }
        a = (u32)RS16(p + 0x69u); /* original reload even after failed pair test */
        a = a * a * a;
        t = (long double)(s32)a;
        t = t * r;
        t = t * (long double)D(0x00968e18u);
        minimum = (u32)(long long)t; /* original __ftol 0x00596fac */
clamp_minimum:
        if ((s32)minimum < 200) minimum = 200u;
    }
ratio_ready:
    n = (s32)(s16)normalized;
    divisor = (200 - n) / 10;
    if (divisor < 1) divisor = 1;
    t = (long double)n;
    t = t * (long double)n;
    t = t * (long double)n;
    t = t * r;
    t = t / (long double)divisor;
    a = (u32)(long long)t; /* original __ftol 0x0059700e */
    a += entry_cast;
    c = R32(self + 0x0cu) - (u32)((s32)a / 52);
    index = (u32)(n / 10) * 8u;
    lo = R32(table + index);
    hi = R32(table + index + 4u);
    bits = ((unsigned long long)hi << 32) | lo;
    bits -= (unsigned long long)(long long)(s32)c; /* SUB/SBB modulo64 */
    t = (long double)(long long)bits;
    t = t * r;
    rounded = (double)t;
    if (table == 0x009c86a8u) threshold = (long double)R64(index + 0x009c83e8u);
    else threshold = (long double)R64(index + 0x009c8338u);
    threshold = threshold * r;
    /* FCOMP candidate,T: ordered greater selects candidate; unordered selects T. */
    if ((long double)rounded > threshold) threshold = (long double)rounded;
    w = (u32)(long long)threshold; /* original __ftol 0x005970d8 */
    a = cm_fn_0053b900((u32 *)club);
    if (a != 0u) goto extra_start;
    k = (s32)(s8)cm_fn_005986b0(self);
    if (k == 1 || k == 2) {
        t = (long double)(s32)w;
        if (k == 2) t = t * (long double)D(0x009689c0u);
        else t = t * (long double)D(0x009689b8u);
        w = (u32)(long long)t; /* original __ftol 0x00597118 */
    }
    comp = R32(club + 0x53u);
    cached_360 = G(0x009cf360u);
    cached_488 = G(0x009cf488u);
    if (comp != 0u) {
        id = R32(comp);
        if (id == cached_360 || id == cached_488) {
            if (RS16(club + 0x80u) > 7750) {
                t = (long double)(s32)w;
                t = t * (long double)D(0x009689b8u);
                goto bound_97172;
            }
        } else if (id == G(0x009cf2e4u)) {
            if (RS16(club + 0x80u) > 7750) {
                t = (long double)(s32)w;
                t = t * (long double)D(0x00968fa0u);
bound_97172:
                w = (u32)(long long)t; /* original __ftol 0x00597172 */
            }
        }
    }
    if (comp != 0u && R8(club + 0x82u) == 0u && RS16(club + 0x80u) > 7750) {
        id = R32(comp);
        if (id == cached_360 || id == cached_488) {
            t = (long double)(s32)w;
            t = t * (long double)D(0x009689b8u);
            w = (u32)(long long)t; /* original __ftol 0x005971b8 */
        }
    }

extra_start:
    comp = R32(club + 0x53u);
    extra = 0u;                     /* B+20, ESI */
    if (comp != 0u) {
        id = R32(comp);
        if (id == G(0x009cf2e4u) || id == G(0x009cf360u) || id == G(0x009cf488u)) {
            if (RS16(club + 0x80u) > 4750 &&
                (class_value == 2 || class_value == 1 || class_value == 0)) {
                a = R8(club + 0x82u);
                n = (s32)(s16)v;
                b = a == 0u ? 750u : 1250u;
                if (n < 5750 || class_value == 0) extra = ((u32)n - b) * 6u;
                else if (class_value == 1) extra = (u32)n * 8u - b * 8u;
                else extra = ((u32)n - b) * 10u;
            }
        }
    }
    if (person == 0u) goto no_person_value;
    n = RS16(club + 0x80u);
    manager = G(0x00ae1050u);        /* 597257: before arithmetic memory reads */
    t = (long double)n;
    t = t * (long double)n;
    t = t * (long double)rd;
    t = t * (long double)D(0x0096a5c0u);
    t = t + (long double)RS32(self + 0x10u);
    rounded = (double)t;            /* B+24 double, saved self already cached */
    a = cm_fn_004d0c20(manager, person);
    if ((long double)(s32)a >= (long double)rounded) {
        manager = G(0x00ae1050u);
        a = cm_fn_004d0c20(manager, person);
        t = (long double)(s32)a;
    } else {
        n = RS16(club + 0x80u);
        t = (long double)n;
        t = t * (long double)n;
        t = t * (long double)rd;
        t = t * (long double)D(0x0096a5c0u);
        t = t + (long double)RS32(self + 0x10u);
    }
    v = (u32)(long long)t; /* original __ftol 0x005972c3; EBX and B+14 */
    p = R32(person + 0x61u);
    if (p != 0u && RS16(p + 0x0bu) > 5250) {
        a = (u8)flags;
        if ((a == 3u || a == 2u || a == 1u) && RS8(person + 0x18u) < 32) {
            if (a == 3u) {
                t = (long double)(s32)extra;
                rd = (double)t;
                t = t * (long double)D(0x009689b0u);
                rounded2 = (double)t;
                if (!((long double)(s32)w >= (long double)rounded2)) {
                    a = cm_fn_00600a50(club, 0u, 1u, 0u, 0u);
                    if (a == 0u) {
                        p = R32(person + 0x61u);
                        if (p != 0u && RS16(p + 0x0bu) > 7250) {
                            t = (long double)rd;
                            t = t * (long double)D(0x009689d8u);
                            w = (u32)(long long)t; /* original __ftol 0x00597362 */
                            goto after_extra_adjustment;
                        }
                    }
                    t = (long double)rounded2;
                    w = (u32)(long long)t; /* original __ftol 0x00597371 */
                }
            } else if (a == 2u) {
                t = (long double)(s32)extra;
                t = t * (long double)D(0x009689d8u);
                rd = (double)t;
                if (!((long double)(s32)w >= (long double)rd)) {
                    a = cm_fn_00600a50(club, 0u, 1u, 0u, 0u);
                    if (a == 0u) {
                        p = R32(person + 0x61u);
                        if (p != 0u && RS16(p + 0x0bu) > 7750) {
                            w = extra;
                            goto after_extra_adjustment;
                        }
                    }
                    t = (long double)rd;
                    w = (u32)(long long)t; /* original __ftol 0x005973cf */
                }
            } else if (a == 1u && (s32)w < (s32)extra) w = extra;
        }
    }
after_extra_adjustment:
    a = cm_fn_00600a50(club, 0u, 1u, 0u, 0u);
    if (a != 0u) {
        p = R32(person + 0x61u);
        if (p != 0u) {
            rep = RS16(p + 0x0bu);
            if (rep < 3750) {
                n = RS16(p + 5u);
                if (n < 60) b = RS8(person + 0x18u) <= 23 ? 5000u : 7500u;
                else if (n < 100) {
                    if (((s8)flags < 4 && (u8)flags != 0u) || RS8(person + 0x18u) >= 24) b = 15000u;
                    else b = 10000u;
                } else b = 25000u;
            } else if (rep < 5250) {
                b = RS16(p + 5u) >= 100 ? 30000u : 25000u;
            } else if (rep < 7250) {
                if (RS8(person + 0x18u) > 34 && RS16(p + 5u) < 120) {
                    if ((s8)flags < 4 && (u8)flags != 0u && (u8)flags != 7u) b = 37500u;
                    else b = 32500u;
                } else {
                    n = RS16(p + 5u);
                    if (n < 140 && RS16(p + 0x0du) < 3750) {
                        b = R8(person + 0x22u) != 0u ? 45000u : 40000u;
                    } else if ((s8)flags < 4 && (u8)flags != 0u && (u8)flags != 7u) {
                        if (R8(person + 0x22u) != 0u) b = 57500u;
                        else b = n >= 140 ? 80000u : 65000u;
                    } else b = 45000u;
                }
            } else if (RS16(p + 0x0du) < 6750) {
                b = R8(person + 0x22u) != 0u ? 125000u : 100000u;
            } else b = 175000u;
            if ((s32)b < (s32)w) {
                t = (long double)(s32)(w - b);
                t = t * (long double)D(0x00969518u);
                a = (u32)(long long)t; /* original __ftol 0x00597580 */
                w = b - a;
            }
        }
    }
    p = R32(person + 0x61u);
    if (p == 0u) goto status_start;
    comp = R32(club + 0x53u);
    if (comp == 0u) goto partner_adjustment;
    id = R32(comp);
    if (!(id == G(0x009cf360u) || id == G(0x009cf2e4u) ||
          id == G(0x009cf314u) || id == G(0x009cf300u) || id == G(0x009cf488u))) goto partner_adjustment;
    a = cm_fn_0053d7f0((volatile u32 *)club);
    if (a != 0u) {
        t = (long double)(s32)v;
        t = t * (long double)D(0x009689b8u);
        v = (u32)(long long)t; /* original __ftol 0x005975e8 */
    }
    p = G(0x00de1f54u);
    if (p != 0u) {
        rep = RS16(club + 0x80u);
        divisor = rep < 1000 ? 1000 : rep;
        a = R32(club);
        p = R32(p + 0x856u);
        q = R32(p + 0xdcu);
        n = RS16(q + (a + a * 8u) + 5u);
        r = (long double)n;
        r = r / (long double)divisor;
        other = (long double)D(0x00967890u);
        if (other > r) r = (long double)D(0x00967890u);
        else {
            other = (long double)D(0x00968978u);
            if (!(other >= r)) r = (long double)D(0x00968978u);
        }
        t = (long double)(s32)v;
        t = t * r;
        v = (u32)(long long)t; /* original __ftol 0x005977f5 */
    }
    rep = RS16(club + 0x80u);
    if (rep > 6250) {
        if (related != 0u && R32(person + 0x39u) == club) {
            date_high = G(0x00ae2c94u);
            date_low = G(0x00ae2c90u);
            a = cm_fn_0054cdb0(related + 0x2du, date_low, date_high);
            if ((s32)a < 200) {
                p = R32(person + 0x61u);
                if (RS16(p + 0x0du) > 5250) { factor_address = 0x00968978u; goto value_9794c; }
                rep2 = RS16(p + 0x0bu);
                if (rep2 > 7250) { factor_address = 0x009689c0u; goto value_9794c; }
                a = R8(related + 0x4fu) & 0xf0u;
                if ((a == 0x30u && rep2 > 5250) || a == 0x10u || a == 0x20u) {
                    factor_address = 0x009689b8u; goto value_9794c;
                }
                goto partner_adjustment;
            }
        }
        p = R32(person + 0x61u);
        if (RS16(p + 0x0du) > 5250) { factor_address = 0x009689c0u; goto value_9794c; }
        if (RS16(p + 0x0bu) > 6750) { factor_address = 0x009689b8u; goto value_9794c; }
        goto partner_adjustment;
    }
    if (rep <= 4250) goto partner_adjustment;
    if (related != 0u && R32(person + 0x39u) == club) {
        date_high = G(0x00ae2c94u);
        date_low = G(0x00ae2c90u);
        a = cm_fn_0054cdb0(related + 0x2du, date_low, date_high);
        if ((s32)a < 200) {
            p = R32(person + 0x61u);
            rep2 = RS16(p + 0x0bu);
            if (rep2 > 6750) { factor_address = 0x009689b8u; goto value_9794c; }
            a = R8(related + 0x4fu) & 0xf0u;
            if ((a == 0x30u && rep2 > 5250) || a == 0x10u || a == 0x20u) {
                factor_address = 0x00968f88u; goto value_9794c;
            }
            goto partner_adjustment;
        }
    }
    p = R32(person + 0x61u);
    if (RS16(p + 0x0bu) <= 7750) goto partner_adjustment;
    factor_address = 0x00968f88u;
value_9794c:
    t = (long double)(s32)v;
    t = t * (long double)(*(volatile double *)factor_address);
    v = (u32)(long long)t; /* original __ftol 0x0059794c; seven producer blocks */

partner_adjustment:
    p = R32(club + 0xbfu);
    if (p == 0u) goto flags_dispatch;
    a = R32(p + 0x69u);
    if (a == 0u) goto flags_dispatch;
    q = R32(person + 0x61u);
    rep = RS16(club + 0x80u);
    rep2 = RS16(q + 0x0bu);
    if (rep >= rep2 + 1250) goto flags_dispatch;
    n = RS8(p + 0x57u);
    t = (long double)n;
    t = t * (long double)D(0x00967898u);
    t = t + (long double)D(0x009689d8u);
    t = t * (long double)(s32)v;
    v = (u32)(long long)t; /* original __ftol 0x005979ac */
    a = cm_fn_00547810(person, p);
    if (a == 0u) a = cm_fn_00547790(person, club);
    if (a != 0u) {
        p = R32(club + 0xbfu);
        n = RS8(p + 0x57u);
        t = (long double)n;
        t = t * (long double)D(0x00967898u);
        t = t + (long double)D(0x00967890u);
        t = t * (long double)(s32)v;
        v = (u32)(long long)t; /* original __ftol 0x005979f9 */
    }
    p = R32(club + 0xbfu);
    q = R32(p + 0x69u);
    n = RS8(q + 0x0fu);
    if (n < 6) {
        a = R32(club + 0xcfu);
        if (a != 0u && RS8(a + 0x57u) > 15) {
            a = R32(a + 0x69u);
            if (a != 0u && RS16(a + 0x0au) > 7250) {
                factor_address = 0x009689c0u;
                goto value_97a59;
            }
        }
        factor_address = 0x009689b8u;
        goto value_97a59;
    } else if (n > 15) {
        factor_address = 0x009689d0u;
value_97a59:
        t = (long double)(s32)v;
        t = t * (long double)(*(volatile double *)factor_address);
        v = (u32)(long long)t; /* original __ftol 0x00597a59 */
    }
    if (R8(club + 0x82u) == 0u && RS8(q + 0x20u) > 15 && RS8(p + 0x57u) > 15) {
        rep = RS16(club + 0x80u);
        if (rep >= 2750) {
            a = R32(person + 0x61u);
            if (RS16(a + 0x0bu) >= 7250) {
                t = (long double)(s32)v;
                if (rep < 5750) t = t * (long double)D(0x009689c0u);
                else t = t * (long double)D(0x0096a5a8u);
                goto value_97aba;
            }
        }
        t = (long double)(s32)v;
        t = t * (long double)D(0x009689b8u);
value_97aba:
        v = (u32)(long long)t; /* original __ftol 0x00597aba */
    }
flags_dispatch:
    n = (s32)(s8)flags;
    if (n <= -1) goto status_start;
    index = (u32)n - 1u;
    if (index > 5u) goto status_start;
    /* 597ade: distinct native table; exact six targets ba6,ae5,b82,bef,c4e,ca1. */
    target = R32(0x00597d94u + index * 4u);
    (void)target;
    switch (index) {
    case 0: goto flag_one;
    case 1: goto flag_two;
    case 2: goto flag_three;
    case 3: goto flag_four;
    case 4: goto flag_five;
    case 5: goto flag_six;
    }
flag_two:
    a = cm_fn_00600a50(club, 1u, 1u, 0u, 0u);
    if (a == 0u) {
        p = R32(person + 0x61u);
        if (RS16(p + 0x0bu) > 6750) goto flag_one;
    }
    t = (long double)(s32)v;
    v = (u32)(long long)t; /* original __ftol 0x00597b11; identity still a cast */
    a = cm_fn_00600a50(club, 1u, 1u, 0u, 0u);
    if (a == 0u) {
        p = R32(person + 0x61u);
        if (p == 0u || RS16(p + 0x0bu) >= 7750) goto status_start;
    }
    if ((s32)v <= 85000) goto status_start;
    t = (long double)(s32)(v - 85000u);
    t = t * (long double)D(0x00968918u);
    t = t + (long double)D(0x0096a5a0u);
    v = (u32)(long long)t; /* original __ftol 0x00597b72 */
    goto status_start;
flag_three:
    a = cm_fn_00600a50(club, 1u, 1u, 0u, 0u);
    if (a == 0u) {
        p = R32(person + 0x61u);
        if (RS16(p + 0x0bu) > 6750) goto flag_one;
    }
    t = (long double)(s32)v;
    t = t * (long double)D(0x009689d0u);
    v = (u32)(long long)t; /* original __ftol 0x00597bca */
    if ((s32)v > 55000) v = 55000u;
    goto status_start;
flag_one:
    t = (long double)(s32)v;
    t = t * (long double)D(0x00968928u);
    v = (u32)(long long)t; /* original __ftol 0x00597bb0 */
    goto status_start;
flag_four:
    a = cm_fn_00600a50(club, 1u, 1u, 0u, 0u);
    if (a == 0u) {
        p = R32(person + 0x61u);
        if (RS16(p + 0x0bu) > 6750) {
            t = (long double)(s32)v;
            t = t * (long double)D(0x009689d0u);
            goto value_97c29;
        }
    }
    t = (long double)(s32)v;
    t = t * (long double)D(0x00969040u);
value_97c29:
    v = (u32)(long long)t; /* original __ftol 0x00597c29 */
    if ((s32)v > 37500) v = 37500u;
    goto status_start;
flag_five:
    if ((s32)v > 3200) {
        t = (long double)(s32)(v - 3200u);
        t = t * (long double)D(0x00968e80u);
        t = t + (long double)D(0x0096a598u);
    } else {
        t = (long double)(s32)v;
        t = t * (long double)D(0x00968e48u);
    }
    v = (u32)(long long)t; /* original __ftol 0x00597c7c */
    if ((s32)v > 12500) v = 12500u;
    goto status_start;
flag_six:
    if ((s32)v > 3200) {
        t = (long double)(s32)(v - 3200u);
        t = t * (long double)D(0x00968f20u);
        t = t + (long double)D(0x0096a590u);
    } else {
        t = (long double)(s32)v;
        t = t * (long double)D(0x00968968u);
    }
    v = (u32)(long long)t; /* original __ftol 0x00597ccf */
    if ((s32)v > 8250) v = 8250u;
    goto status_start;

no_person_value:
    t = (long double)(s16)v;          /* 59761f: normalized cast, not DI */
    t = t * (long double)(s16)v;
    t = t * (long double)rd;
    t = t * (long double)D(0x0096a5c0u);
    t = t + (long double)RS32(self + 0x10u);
    v = (u32)(long long)t; /* original __ftol 0x00597641 */
    if ((s32)v < (s32)extra) {
        t = (long double)(s32)v;
        t = t * (long double)D(0x00969040u);
        other = (long double)(s32)extra;
        other = other * (long double)D(0x009689e0u);
        t = t + other;
        v = (u32)(long long)t; /* original __ftol 0x00597668 */
    }
    a = cm_fn_0053b900((u32 *)club);
    if (a != 0u) goto status_start;
    comp = R32(club + 0x53u);
    if (comp == 0u) goto status_start;
    id = R32(comp);
    if (id == G(0x009cf314u) || id == G(0x009cf300u)) {
        k = (s32)(s8)cm_fn_005986b0(self);
        if (k == 1) goto no_person_105;
        if (k == 2) goto no_person_110;
        goto status_start;
    }
    if (!(id == G(0x009cf360u) || id == G(0x009cf2e4u) || id == G(0x009cf488u))) goto status_start;
    k = (s32)(s8)cm_fn_005986b0(self);
    if (k == 0) {
        if (RS16(club + 0x80u) <= 4250) goto status_start;
        t = (long double)(s32)v;
        t = t * (long double)D(0x00968fa0u);
        goto no_person_976fd;
    }
    if (k == 1) goto no_person_105;
    if (k != 2) goto status_start;
no_person_110:
    t = (long double)(s32)v;
    t = t * (long double)D(0x009689c0u);
    goto no_person_976fd;
no_person_105:
    t = (long double)(s32)v;
    t = t * (long double)D(0x009689b8u);
no_person_976fd:
    v = (u32)(long long)t; /* original __ftol 0x005976fd; three producer paths */

status_start:
    /* EBX may subsequently change while B+14 retains the earlier v. */
    b = v;
    a = R8(club + 0x64u);
    if ((u8)a == 1u) {
        if ((s32)b < 250) { b = 250u; v = b; }
        if (RS16(club + 0x80u) < 2250 && (s32)b > 2100) {
            t = (long double)(s32)v;
            t = t - (long double)F(0x0096a5b8u);
            t = t * (long double)D(0x00968918u);
            t = t + (long double)D(0x0096a5b0u);
            b = (u32)(long long)t; /* original __ftol 0x00597759; no B+14 store */
        }
    } else if ((u8)a == 2u) {
        t = (long double)(s32)v;
        t = t * (long double)D(0x00969040u);
        b = (u32)(long long)t; /* original __ftol 0x00597d02 */
        rep = RS16(club + 0x80u);
        v = b;
        if (rep < 2750 && (s32)b > 1200) {
            t = (long double)(s32)v;
            t = t - (long double)F(0x0096a588u);
            t = t * (long double)D(0x00968918u);
            t = t + (long double)D(0x0096a580u);
            b = (u32)(long long)t; /* original __ftol 0x00597d36; no B+14 store */
        }
        if ((s32)b < 150) b = 150u;
    }
    a = minimum + 100u;              /* 597d50 LEA modulo32 before signed CMP */
    if ((s32)b < (s32)a) b = a;
    if ((s32)minimum > (s32)w) return minimum;
    if ((s32)b < (s32)w) return b;
    return w;
}

#undef R8
#undef R16
#undef R32
#undef RS8
#undef RS16
#undef RS32
#undef R64
#undef D
#undef F
#undef G
