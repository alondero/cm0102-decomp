#include "cmre.h"

/* Preferred-image VA 00591f80, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch70: counter/date output L0, time output L8, text/time result Lc.
   Preserve forward DWORD copies with trailing bytes, including their NUL. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0053dba0(u32, u32, u16, u8);
extern u32 CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00946ebb(u32);

static u32 scan_00591f80(u32 pointer)
{
    u32 remaining = 0xffffffffu;
    while (remaining != 0) {
        u8 byte = B(pointer);
        ++pointer;
        --remaining;
        if (byte == 0) break;
    }
    return ~remaining;
}

static void copy_00591f80(u32 destination, u32 source, u32 bytes)
{
    u32 words = bytes >> 2, tail = bytes & 3;
    while (words != 0) {
        u32 value = D(source);
        D(destination) = value;
        source += 4; destination += 4;
        --words;
    }
    while (tail != 0) {
        u8 value = B(source);
        B(destination) = value;
        ++source; ++destination; --tail;
    }
}

u32 CM_CDECL cm_fn_00591f80(volatile u32 output)
{
    u8 scratch[0x70];
    u32 l = (u32)scratch, text = l + 0xc, destination, source, bytes;
    u32 pass, table, first, pointer, value, index, record, slot;
    u32 primary, secondary, old_time, total, eax;
    destination = output;
    B(destination) = 0;
    B(destination + 0x3e8) = 0;
    for (pass = 0; pass < 2; ++pass) {
        first = 1;
        table = 0x00b63d60;
        D(l) = 0x23;
        do {
            pointer = D(table);
            if (pointer != 0 && (B(pointer + 0x11c) & (pass == 0 ? 2 : 1))) {
                if (first) {
                    cm_fn_00944c9f(text, pointer + 0x38);
                    first = 0;
                } else cm_fn_0066f4e0(text, 0x009c8134, pointer + 0x38);
                bytes = scan_00591f80(text);
                destination = output;
                if (pass != 0) destination += 0x3e8;
                value = scan_00591f80(destination);
                copy_00591f80(destination + value - 1, text, bytes);
            }
            value = D(l);
            table += 0x48;
            --value;
            D(l) = value;
        } while (value != 0);
    }
    if (first) {
        destination = output;
        B(destination + 0x3e8) = 0;
    }
    destination = output;
    index = 0;
    D(l) = 0x10;
    slot = destination + 0x820;
    do {
        value = D(0x00ae2364) - index - 1u;
        pointer = D(0x00ae23bc);
        record = pointer + value * 0x6eu;
        if (B(record + 0x5f) == 1) {
            cm_fn_0053dba0(record, slot - 0x3c, 0xc8, 1);
            primary = D(record + 0x39);
            if (primary != 0) {
                secondary = D(record + 0x24);
                if (secondary != 0) {
                    cm_fn_0066f4e0(slot, 0x009c80f4, primary + 0x38, secondary + 0x38);
                } else {
                    source = primary + 0x38;
                    bytes = scan_00591f80(source);
                    copy_00591f80(slot, source, bytes);
                }
            } else {
                secondary = D(record + 0x24);
                if (secondary != 0) {
                    source = secondary + 0x38;
                    bytes = scan_00591f80(source);
                    copy_00591f80(slot, source, bytes);
                } else B(slot) = 0;
            }
        } else B(slot - 0x3c) = 0;
        value = D(l);
        ++index;
        slot += 0x78;
        --value;
        D(l) = value;
    } while (value != 0);
    destination = output;
    cm_fn_00417c10(destination + 0xf64, l, 0x00ae2c90);
    source = cm_fn_0090d130();
    bytes = scan_00591f80(source);
    copy_00591f80(destination + 0x7d0, source, bytes);
    eax = cm_fn_00946ebb(l + 8);
    old_time = D(0x00dd7144);
    total = D(0x00dd7140);
    destination = output;
    total += eax - old_time;
    D(l + 0xc) = eax;
    D(0x00dd7140) = total;
    D(destination + 0xf6c) = total;
    D(0x00dd7144) = eax;
    eax = cm_fn_00946ebb(0);
    D(destination + 0xf70) = eax;
    return eax;
}
