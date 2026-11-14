#include "cmre.h"
/* Preferred-image VA 0x008b39f0, image base 0x00400000, CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THISCALL; ECX receiver read into EBP at 0x008b3a02; one DWORD stack argument
   at 0x008b39f8 [esp+0x20c] = entry_esp+4; RET 4 at 0x008b3ada/0x008b3b5e/
   0x008b3bae/0x008b3bd9/0x008b3c4a; full-EAX u32 pointer result (never void).
   Original frame L = entry_ESP-0x200 holds ONE contiguous uninitialized
   0x200-byte scratch extent; L and L+0x100 are the two diagnostic buffers.
   Signed DWORD compares, 32-bit wrapping index strides (0x245 record table,
   0x60e / 0x9b receiver strides) and every original reload, repeat cache store
   and diagnostic call order are retained. Unknowns stay unknown: no receiver
   field, record layout or buffer extent is named beyond the raw original
   offsets that the instructions themselves address. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 arg2, u32 arg3, u32 out_first,
                                  u32 out_second);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008b39f0(u32 self, u32 argument)
{
    u8 scratch[0x200];
    u32 l = (u32)scratch;
    u32 buf_a = l;
    u32 buf_b = l + 0x100u;
    u32 index, record, row, other, base, i, version, line, first, second;
    u8 state;

    /* 0x008b3a00/0x008b3a0d: null person or null [person+0x61] -> path C. */
    if (argument == 0u || D(argument + 0x61u) == 0u) {
        cm_fn_00944cff(0x00a8ccd0u, 0u, 0u, buf_a, buf_b);
        first = buf_a;
        second = buf_b;
        line = 0x6adu;
        goto diagnostic;
    }

    /* 0x008b3a15..0x008b3a46: first lookup and its cache store; the computed
       record is only null-tested here and then discarded. */
    record = cm_fn_004cf5a0(D(0x00ae1050u), argument);
    D(0x00ae1080u) = record;                       /* first 0x00ae1080 store */
    if (record == 0u)
        goto fallback;
    index = D(record + 4u);
    index = D(0x00ae23b4u) + index * 0x245u;
    if (index == 0u)
        goto fallback;

    /* 0x008b3a4c..0x008b3a7b: second lookup, repeat cache store, record reload.
       A null lookup leaves EAX=0 and the original still reads [0xcf], so a
       genuine null-record fault is preserved rather than repaired. */
    record = cm_fn_004cf5a0(D(0x00ae1050u), argument);
    D(0x00ae1080u) = record;                       /* repeat 0x00ae1080 store */
    if (record != 0u) {
        index = D(record + 4u);
        record = D(0x00ae23b4u) + index * 0x245u;
    } else {
        record = 0u;
    }
    row = D(record + 0xcfu);                       /* EDI row pointer */
    if (row == 0u)
        goto fallback;
    index = D(0x00ae2364u) - 0x10u;
    if ((s32)D(row) < (s32)index)                  /* signed JL at 0x008b3a95 */
        goto fallback;

    state = cm_fn_0062c400(D(0x00b63d28u), argument);
    if (state == 2u) {                             /* CMP AL,2 at 0x008b3aa7 */
        index = D(0x00ae2364u);                    /* global read precedes [edi] */
        i = index - D(row) - 1u;
        if ((s32)i < 0 || (s32)i >= 0x10) {        /* path A, 0x008b3add */
            cm_fn_00944cff(0x00a8ccd0u, 0u, 0u, buf_b, buf_a);
            first = buf_b;
            second = buf_a;
            line = 0x68fu;
            goto diagnostic;
        }
        return self + 0x4d8u + i * 0x60eu;
    }

    /* 0x008b3b11..0x008b3b31: AL!=2 branch. The receiver-indexed byte is read
       before the range check, exactly as the original loads CL at 0x008b3b28
       and only then tests JS/JGE. */
    other = D(argument + 0x61u);
    index = D(row);
    base = D(self + 0x651du);
    other = D(other);
    other = other + other * 2u;
    i = D(0x00ae2364u) - index - 1u;
    state = B(base + other * 2u);
    if ((s32)i < 0 || (s32)i >= 0x10) {            /* path B, 0x008b3b61 */
        cm_fn_00944cff(0x00a8ccd0u, 0u, 0u, buf_a, buf_b);
        first = buf_a;
        second = buf_b;
        line = 0x68fu;
        goto diagnostic;
    }
    if ((s8)state < 0 || state >= 0x0au)           /* TEST CL,CL / CMP CL,0xa */
        state = 1u;
    return self + (u32)((s32)(s8)state + (s32)i * 10 + 7) * 0x9bu;

fallback:                                          /* 0x008b3b93 */
    state = cm_fn_0062c400(D(0x00b63d28u), argument);
    if (state == 2u)
        return self;
    other = D(argument + 0x61u);
    other = D(other);
    other = other + other * 2u;
    base = D(self + 0x651du);
    state = B(base + other * 2u);
    return self + (u32)((s32)(s8)state * 0x9b);

diagnostic:                                        /* shared 0x008b3c0c tail */
    version = cm_fn_0090d130();                    /* no-argument boundary */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, first, second, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
    D(0x00b67a34u) = 0u;
    return 0u;
}
