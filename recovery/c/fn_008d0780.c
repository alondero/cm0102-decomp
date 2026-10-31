#include "cmre.h"
/* Preferred-image VA008d0780, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/fullEAX; SEH96403b, scratch210: date L0, copy L8,
   diagnostics L10/L110. Products and sums wrap before signed division.
   4d0c60 retains physical ECX and all five stack arguments (RET14h).
   417c10 copies all eight bytes into both receiver and first stack output. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053ab90(u32, u32);
extern u32 CM_CDECL cm_fn_005446d0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_STDCALL cm_fn_008d0780(volatile u32 person_arg,
                            volatile u32 club_arg, volatile u32 mode_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x210];
    u32 l = (u32)scratch, person, club, contract, p, a, b, days;
    u32 share, id, base, source, nation, tag, result = 0, version;
    u8 kind;
    frame.state = 0xffffffffu; frame.handler = 0x0096403b;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(l);
    person = person_arg;
    frame.state = 0;
    if (person == 0) goto diagnostic;
    club = club_arg;
    if (club == 0) goto diagnostic;
    if (D(person + 0x61) == 0 && D(person + 0x69) != 0) {
        p = G(0x00ae1050);
        contract = cm_fn_004cf5a0(p, person);
        if (contract == 0) goto done;
        kind = B(contract + 0x35) & 0x3f;
        if (kind != 2) goto done;
        b = G(0x00ae2c94); a = G(0x00ae2c90);
        days = cm_fn_0054cdb0(contract + 0x2d, a, b);
        share = (u32)((s32)(days * D(contract + 0xc)) / 7);
        result = share;
        if ((s32)result < 1000) result = 1000;
        else if ((s32)result > 1000000) result = 1000000;
        goto done;
    }
    if (CM_GLOBAL(volatile u8, 0x00ae233c) == 1 && mode_arg == 0) {
        p = G(0x00ae1050);
        contract = cm_fn_004cf5a0(p, person);
        if (contract == 0) goto done;
        kind = B(contract + 0x35) & 0x3f;
        if (kind != 2) goto done;
        b = G(0x00ae2c94); a = G(0x00ae2c90);
        days = cm_fn_0054cdb0(contract + 0x2d, a, b);
        share = (u32)((s32)(days * D(contract + 0xc)) / 7);
        id = D(contract + 4); base = G(0x00ae23b4);
        source = base + id * 581u;
        p = G(0x00ae1050);
        result = cm_fn_004d0c60(p, person, source, 1, 0xffffffffu, 0);
        result = (u32)((s32)(result + share) / 2);
        goto done;
    }
    p = G(0x00ae1050);
    contract = cm_fn_004cf5a0(p, person);
    if (contract == 0) goto done;
    kind = B(contract + 0x35) & 0x3f;
    if (kind == 5) goto done;
    id = D(contract + 4); base = G(0x00ae23b4);
    source = base + id * 581u;
    if (D(club + 0x53) == 0) goto done;
    if (D(source + 0x53) == 0) goto done;
    p = kind == 2 ? contract + 0x2d : 0x00ae2c90;
    cm_fn_00417c10(l, l + 8, p);
    nation = D(source + 0x53); tag = G(0x009cf4a8);
    if (D(nation) == tag) {
        nation = D(club + 0x53);
        if (D(nation) == tag) goto value;
    }
    a = cm_fn_0053ab90(person, l);
    if ((s8)(u8)a >= 0x18) goto done;
    p = D(club + 0x53);
    if (cm_fn_005446d0(p) == 0) goto done;
    p = D(source + 0x53);
    if (cm_fn_005446d0(p) == 0) goto done;
value:
    p = G(0x00ae1050);
    result = cm_fn_004d0c60(p, person, source, 1, 0xffffffffu, 0);
    result = (u32)((s32)result / 2);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a8d514, 0, 0, scratch + 0x110, scratch + 0x10);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                  l + 0x110, l + 0x10, 0x372e);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    G(0x00b67a34) = 0;
    result = 0;
done:
    a = frame.prev;
    CM_FS(volatile u32, 0) = a;
    return result;
}
