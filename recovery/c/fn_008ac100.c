#include "cmre.h"

/* Preferred-image VA 008ac100, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep original forward DWORD/WORD/BYTE copy widths, signed BYTE counters,
   pointer reloads and the second store of a capped elapsed BYTE. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

static void copy_008ac100(u32 destination, u32 source, u32 words, u32 byte_tail)
{
    u32 value;
    while (words != 0) {
        value = D(source); D(destination) = value;
        source += 4; destination += 4; --words;
    }
    value = W(source); W(destination) = (u16)value;
    if (byte_tail) {
        value = B(source + 2); B(destination + 2) = (u8)value;
    }
}

u32 CM_THISCALL cm_fn_008ac100(u32 self, volatile s8 first_arg,
                              volatile s8 second_arg)
{
    u8 scratch[0x200];
    u32 l = (u32)scratch, base, source, destination, found;
    u32 index, mode, value, version;
    s32 elapsed, kind;
    u8 first, second, count, scan, byte;
    first = (u8)first_arg;
    if ((s8)first < 0 || (s8)first > 19) goto invalid;
    second = (u8)second_arg;
    if ((s8)second < 0 || (s8)second > 19) goto invalid;
    base = D(self + 4);
    if (base == 0) return 0;
    copy_008ac100(self + 0x4a63, self + 0xe44, 0xf07, 1);
    count = B(self + 0x4a1f);
    scan = 0;
    D(self + 0xc4de) = 1;
    if ((s8)count > 0) {
        do {
            if (second == B(self + 0x4a0d + (u32)(s32)(s8)scan)) goto replace;
            ++scan;
        } while ((s8)scan < (s8)count);
    }
    index = (u32)(s32)(s8)count;
    B(self + 0x4a0d + index) = first;
    index = (u32)(s32)(s8)B(self + 0x4a1f);
    B(self + 0x4a16 + index) = second;
    byte = B(self + 0x4a1f);
    ++byte;
    B(self + 0x4a1f) = byte;
    goto update;

replace:
    mode = (u32)(s32)(s8)B(self + 0xde9);
    index = (u32)(s32)(s8)scan;
    value = (u32)(s32)(s8)B(self + 0x4a0d + index);
    found = self + index;
    base = D(self + 4);
    source = base + (value + mode * 20u) * 0x1beu + 0x4796;
    destination = self + value * 0x1beu + 0x2735;
    copy_008ac100(destination, source, 0x6f, 0);
    mode = (u32)(s32)(s8)B(self + 0xde9);
    value = (u32)(s32)(s8)B(found + 0x4a16);
    base = D(self + 4);
    source = base + (value + mode * 20u) * 0x1beu + 0x4796;
    destination = self + value * 0x1beu + 0x2735;
    copy_008ac100(destination, source, 0x6f, 0);
    B(found + 0x4a0d) = first;
    second = B(found + 0x4a16);

update:
    destination = self + (u32)(s32)(s8)first * 0x1beu + 0x2735;
    source = self + (u32)(s32)(s8)second * 0x1beu + 0x2735;
    base = D(self + 4);
    elapsed = (s32)(s16)W(base + 0x8ed0) / 11;
    B(destination + 0x1f) = (u8)elapsed;
    base = D(self + 4);
    kind = (s16)W(base + 0x8ed4);
    byte = (u8)elapsed;
    if (kind == 0x1ef) {
        if ((s8)byte >= 45) byte = 45;
        B(destination + 0x1f) = byte;
    } else if (kind == 0x3de) {
        if ((s8)byte >= 90) byte = 90;
        B(destination + 0x1f) = byte;
    } else if (kind == 0x483) {
        if ((s8)byte >= 105) byte = 105;
        B(destination + 0x1f) = byte;
    } else if (kind == 0x528) {
        if ((s8)byte >= 120) byte = 120;
        B(destination + 0x1f) = byte;
    }
    byte = B(source + 0x19);
    if (byte == 0xfe) {
        byte = B(source + 0x1a);
        B(destination + 0x19) = byte;
    } else B(destination + 0x19) = byte;
    byte = B(destination + 0x1f);
    B(source + 0x19) = 0xfc;
    B(source + 0x20) = byte;
    B(source + 0x1e) = 1;
    D(self + 0x4a5f) = 1;
    return 1;

invalid:
    cm_fn_00944cff(0x00a8b94c, 0, 0, (void *)(l + 0x100), (void *)l);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x100, l, 0x1fde);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
