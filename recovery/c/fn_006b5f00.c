#include "cmre.h"
/* Preferred-image VA 0x006b5f00, image base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   `ret 4` (0x006b61d2) => stdcall with one dword argument: the match report
   pointer, read at 0x006b5f06 from [esp+0x25c].

   Frame map.  L = esp immediately after `sub esp,0x258` (0x006b5f00).
     L+0x000  two bytes, written at 0x006b5f11 / 0x006b5f16; read back as the
              dword at L+0 (0x006b619a `mov eax,[esp+8]`, esp = L-8) and as the
              dword at L+1 (0x006b61ae `mov eax,[esp+0x1d]`, esp = L-0x1c).
              Bytes L+2 and L+3 are never written by the original and stay
              uninitialised.
     L+0x008  79-byte copy of report+4 (0x006b6031 `lea edi,[esp+0x18]`).
     L+0x058  256-byte buffer (0x006b5f1d `lea eax,[esp+0x60]`).
     L+0x158  256-byte buffer (0x006b5f21 `lea ecx,[esp+0x160]`).
   One scratch block keeps the original adjacency: the 0xff bytes written at
   0x006b6042..0x006b605e land inside the copied record (L+0x08..L+0x56), and
   the word masked at 0x006b6066 is that copy's last word.

   Evidence (current canonical hints):
     `cmre test fn_006b5f00 --c --source <this file>` passes at -O0 and -O2:
     600 cases, 0 mismatches, 0 inconclusive, coverage 0.7436.  The blocks
     the harness still reports uncovered include the deep loop body
     (0x006b5fce..0x006b6010) and the 0xfd copy block (0x006b6027..0x006b6052),
     i.e. the fixtures reach the later match dispatch calls through the
     0x004ca6d0 boundary (0x006b60d2) but do not enter the 0xfd copy or the
     per-club checks.  Acceptance is only recorded by `cmre accept`.

   Boundaries: every callee below stays an explicit original call.  This
   source is not callee composition and carries no runtime proof of the
   callees; the coverage figure above is harness evidence for this
   translation only. */

/* Callee map, unchanged from the recovered sources listed in the brief.
   0x0090d130 is a frozen legacy record and takes no parameters; the three
   words the original pushes before 0x006b5f4d are therefore carried as the
   trailing varargs of the following 0x00944c9f call, which reproduces the
   original's stack layout at 0x006b5f5d and 0x006b5f6e exactly. */
extern void CM_THISCALL cm_fn_004134c0(u32 self, u32 record, u32 base);
extern u32 CM_THISCALL cm_fn_00444930(int param_1, u32 *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target);
extern void CM_THISCALL cm_fn_004ca6d0(u32 self_, u32 rec_, u32 arr_);
extern u32 CM_THISCALL cm_fn_0055ca00(u32 self, u32 p, u32 argb);
extern u8 CM_THISCALL cm_fn_005aa680(int param_1, int param_2, int param_3);
extern u32 CM_THISCALL cm_fn_005ab510(u32 self, u32 fixture);
extern u32 CM_THISCALL cm_fn_0059a5d0(u32 self, u32 match, u32 data);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern void CM_THISCALL cm_fn_0062c080(u32 self, u8 *param_2, u8 *param_3);
extern void CM_THISCALL cm_fn_006951e0(u32 self, volatile u8 *record, u32 arg);
extern u32 CM_CDECL cm_fn_006b6cc0(u32 report, u32 packet);
extern void cm_fn_006b74a0(int param_1);
/* The original pushes the mode as a full dword (0x006b619a loads the dword at
   L+0, 0x006b61ae the dword at L+1); the recovered callee declares the
   parameter 8-bit, so the 32-bit slot is kept to preserve the two bytes the
   original initialises. */
extern void CM_CDECL cm_fn_006b79a0(u32 club, u32 arg, u32 report, u32 mode, u32 match);
extern u32 CM_THISCALL cm_fn_0076b600(u32 self, volatile u32 match_arg, volatile u32 statistics_arg);
extern u32 CM_THISCALL cm_fn_007c0d70(u32 self, u32 match, u32 packet);
extern u32 CM_THISCALL cm_fn_007f8c00(u32 self, u32 param_1, u32 param_2);
extern void CM_THISCALL cm_fn_00868830(u32 self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_008c39f0(u32 self, u32 *param_2, u32 param_3);
extern char *cm_fn_0090d130(void);
/* Call-boundary evidence for the pinned image/hash above (preferred-image VAs):
   0x006b603b loads ECX from [0x00de2f74]; nothing changes ECX before
   0x006b6072 push eax, 0x006b6073 push edx, 0x006b6074 call 0x00924050.
   Preserve that ECX input and both stack arguments; this is not a claim
   about callee behavior, composition, or runtime validation. */
extern void CM_THISCALL cm_fn_00924050(u32 receiver, void *object, s8 param);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
/* Both indirect calls (0x006b60ad, 0x006b612f) go through vtable slot +4
   with the object in ECX and the callee popping its two stack words: the
   stack balance of the surrounding `add esp` proves the thiscall cleanup. */
typedef void (CM_THISCALL *callback)(u32, u32, u32);

u32 CM_STDCALL cm_fn_006b5f00(volatile u32 report_home)
{
    /* 006b5f06 captures the initial caller-owned DWORD before any copy.
       006b601a and 006b60e4 reload that same entry ESP+4 home. Keep each
       register capture separate: the latter survives the 0062c080 call. */
    u32 report = report_home;
    u8 scratch[0x258];
    u32 work = (u32)scratch;
    u32 source, destination, base, packet, club, eax, ecx, edx, table, target;
    u32 i, index, receiver, left, right, flag;
    u16 flags;
    char *text;

    /* 0x006b5f11 / 0x006b5f16: the only bytes the original writes here. */
    B(work) = 0;
    B(work + 1) = 0;
    if (!report) {
        /* 0x006b5f1d..0x006b5f33: five cdecl arguments (0x14 cleanup). */
        cm_fn_00944cff(0x00a15708, 0, 0, scratch + 0x158, scratch + 0x58);
        /* 0x006b5f46..0x006b5f4d pushes 0x73f, L+0x58 and L+0x158 before the
           legacy call; the same three words are the trailing varargs of the
           next call, so the stack at 0x006b5f5d is the original's. */
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, scratch + 0x158, scratch + 0x58, 0x73f);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    /* 0x006b5f8f..0x006b5f9d: 19 dwords + word + byte (79 bytes) from
       report+4 to *(report). */
    destination = D(report);
    source = report + 4;
    for (i = 0; i < 19; ++i) {
        eax = D(source);
        D(destination) = eax;
        source += 4;
        destination += 4;
    }
    flags = W(source);
    W(destination) = flags;
    eax = B(source + 2);
    B(destination + 2) = (u8)eax;
    /* 0x006b5f9e: record pointer, re-read after the copy; 0x006b5fa0:
       [report+0x5f]. */
    base = D(report);
    packet = D(report + 0x5f);
    eax = D(base + 0x14);
    if (eax) {
        for (i = 0; i < 2; ++i) {
            club = D(base + 0x20); /* reloaded by both iterations, 0x006b5fb0 */
            if (!club) continue;
            if (!cm_fn_00600a50(club, 0, 1, 0, 0)) continue;
            eax = D(club + 0x57);
            if (!eax) continue;
            ecx = D(eax);
            eax = D(base);
            if (ecx != eax) continue;
            edx = D(base + 0x14);
            eax = D(edx);
            table = D(0x00adadfc);
            eax = D(table + eax * 4u);
            if (!eax) continue;
            if (!D(eax + 8)) continue;
            ecx = D(table + ecx * 4u);
            receiver = D(ecx + 8); /* thiscall receiver, 0x006b5ffa */
            eax = cm_fn_004a4850(receiver, club);
            if (!eax) continue;
            ecx = D(eax + 0x2d);
            if ((s32)ecx > 1) B(work + i) = (u8)ecx;
        }
        report = report_home; /* 006b601a: entry ESP+4, not initial EAX */
        if (B(report + 0x47) == 0xfd) {
            /* 0x006b6027..0x006b6041: second 79-byte record copy to L+8. */
            source = report + 4;
            destination = work + 8;
            for (i = 0; i < 19; ++i) {
                eax = D(source);
                D(destination) = eax;
                source += 4;
                destination += 4;
            }
            flags = W(source);
            W(destination) = flags;
            receiver = D(0x00de2f74); /* 0x006b603b: ECX at 0x006b6074 */
            eax = B(source + 2);
            B(destination + 2) = (u8)eax;
            /* 0x006b6042..0x006b605e: 0xff bytes at L+0x4b..L+0x52. */
            B(work + 0x4b) = 0xff;
            B(work + 0x4c) = 0xff;
            B(work + 0x4d) = 0xff;
            B(work + 0x4e) = 0xff;
            B(work + 0x51) = 0xff;
            B(work + 0x52) = 0xff;
            B(work + 0x50) = 0xff;
            B(work + 0x4f) = 0xff;
            flags = W(base + 0x4d);
            W(work + 0x55) = W(work + 0x55) & 0xcfffu;
            /* 0x006b6062..0x006b6074: AL is (word >> 12) & 3; the recovered
               callee takes the parameter 8-bit, so the stale upper half of
               EAX that the original shifts in is not observable there. */
            cm_fn_00924050(receiver, (void *)(u32)(report + 4), (s8)((flags >> 12) & 3));
            receiver = D(0x00ae2a58);
            cm_fn_005ab510(receiver, work + 8);
            receiver = D(0x00ae2a58);
            cm_fn_005aa680(receiver, work + 8, 1);
        } else {
            /* 0x006b609d: vtable slot +4 of [0xadadfc + 4*record->[0]]. */
            index = D(base);
            table = D(0x00adadfc);
            receiver = D(table + index * 4u);
            table = D(receiver);
            target = D(table + 4);
            ((callback)target)(receiver, base, packet);
        }
    }
    D(0x00a801bc) = 1;
    cm_fn_006b6cc0(base, packet);
    cm_fn_006b74a0((int)base);
    receiver = D(0x00ae1050);
    cm_fn_004ca6d0(receiver, base, packet);
    receiver = D(0x00ae28f0);
    cm_fn_0055ca00(receiver, base, packet);
    report = report_home; /* 006b60e4: saved ESI for 006b60eb/006b610b */
    if (B(report + 0x47) != 0xfd) {
        receiver = D(0x00b74318);
        cm_fn_0076b600(receiver, base, packet);
    }
    receiver = D(0x00b63d28);
    cm_fn_0062c080(receiver, (u8 *)(u32)base, (u8 *)(u32)packet);
    if (B(report + 0x47) == 0xfd) return 1;
    eax = D(base);
    ecx = D(0x009cf684);
    if (eax != ecx) {
        receiver = D(0x00b74434);
        table = D(receiver);
        target = D(table + 4);
        ((callback)target)(receiver, base, packet);
    }
    receiver = D(0x00b74350);
    cm_fn_007c0d70(receiver, base, packet);
    receiver = D(0x00de1f54);
    cm_fn_008c39f0(receiver, (u32 *)(u32)base, packet);
    receiver = D(0x00ad9dac);
    cm_fn_00444930((int)receiver, (u32 *)(u32)base, (int)packet);
    receiver = D(0x00dd7f00);
    cm_fn_00868830(receiver, base, packet);
    receiver = D(0x00ad9c60);
    cm_fn_004134c0(receiver, base, packet);
    receiver = D(0x00b67a44);
    cm_fn_006951e0(receiver, (volatile u8 *)(u32)base, packet);
    receiver = D(0x00ae2a38);
    cm_fn_0059a5d0(receiver, base, packet);
    receiver = D(0x00b74448);
    cm_fn_007f8c00(receiver, base, packet);
    /* 0x006b619a: mode = dword at L+0 ([esp+8] with esp = L-8). */
    flag = D(work);
    right = D(base + 0x20);
    left = D(base + 0x1c);
    cm_fn_006b79a0(left, right, base, flag, packet);
    /* 0x006b61ae: mode = dword at L+1 ([esp+0x1d]), clubs swapped. */
    flag = D(work + 1);
    left = D(base + 0x1c);
    right = D(base + 0x20);
    cm_fn_006b79a0(right, left, base, flag, packet);
    return 1;
}
