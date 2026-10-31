#include "cmre.h"

/* Preferred-image VA 0x008025e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep wrapping unsigned capacity checks, continuation after reports, and
   the original unchecked string payload copy with its signed WORD length.
   Full EAX is preserved, including the high half of the send argument. */
extern u32 CM_THISCALL cm_fn_0084cbc0(u32, s8, u32, u32);
extern u32 CM_THISCALL cm_fn_0084ccc0(u32, s8, u32);
extern u32 CM_THISCALL cm_fn_0077ac30(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

static u32 report(u32 file, u8 *filename, u8 *extension, u32 line)
{
    char *version;
    u32 eax;
    cm_fn_00944cff(file, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    eax = cm_fn_005e8290((const char *)0x009870e0u,
                        (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return eax;
}

u32 CM_THISCALL cm_fn_008025e0(u32 self, u32 output, u32 kind,
                             u32 text, u32 flag)
{
    u8 scratch[512];
    u32 eax, receiver, stage, offset, capacity, size, buffer, value;
    u32 remaining, scan, full_count, copy_count, words, tail, source, destination;
    u8 byte;

    B(output) = 0xff;
    eax = (u32)(s32)(s16)W(self + 0x382au) * 768u;
    if (W(eax + self + 0x7d4u) == 0) {
        receiver = D(0x00dd7eecu);
        if (receiver == 0) return eax;
        eax = cm_fn_0084cbc0(receiver, (s8)kind, text, flag);
        B(output) = (u8)eax;
        receiver = D(0x00dd7eecu);
        return cm_fn_0084ccc0(receiver, (s8)eax, 100);
    }
    D(self + 0x3802u) = 0;
    for (stage = 0; stage < 4; ++stage) {
        offset = stage == 0 ? 0 : D(self + 0x3802u);
        capacity = D(self + 0x3806u);
        size = stage == 1 ? 4 : 1;
        if (offset + size > capacity) {
            if (stage == 0)
                report(0x009c3734u, scratch + 256, scratch, 0x85);
            else
                report(0x009c3734u, scratch, scratch + 256,
                       stage == 1 ? 0x89 : 0x85);
        } else {
            buffer = D(self + 0x37feu);
            if (stage == 1) {
                D(buffer + offset) = output;
            } else {
                value = stage == 0 ? 10u : (stage == 2 ? kind : flag);
                B(buffer + offset) = (u8)value;
            }
            offset = D(self + 0x3802u);
            D(self + 0x3802u) = offset + size;
        }
    }
    source = text;
    scan = source;
    remaining = 0xffffffffu;
    capacity = D(self + 0x3806u);
    do {
        byte = B(scan);
        ++scan;
        --remaining;
    } while (byte != 0 && remaining != 0);
    offset = D(self + 0x3802u);
    full_count = ~remaining;
    if (offset + 2u > capacity) {
        report(0x009c3734u, scratch, scratch + 256, 0x86);
    } else {
        buffer = D(self + 0x37feu);
        W(buffer + offset) = (u16)full_count;
        offset = D(self + 0x3802u);
        D(self + 0x3802u) = offset + 2u;
    }
    offset = D(self + 0x3802u);
    copy_count = (u32)(s32)(s16)full_count;
    buffer = D(self + 0x37feu);
    destination = buffer + offset;
    words = copy_count >> 2;
    tail = copy_count & 3u;
    while (words != 0) {
        value = D(source);
        D(destination) = value;
        source += 4u; destination += 4u; --words;
    }
    while (tail != 0) {
        value = B(source);
        B(destination) = (u8)value;
        ++source; ++destination; --tail;
    }
    offset = D(self + 0x3802u) + copy_count;
    D(self + 0x3802u) = offset;
    buffer = D(self + 0x37feu);
    eax = (u32)(s32)(s16)W(self + 0x382au) * 768u;
    value = W(eax + self + 0x7d4u);
    eax = (eax & 0xffff0000u) | value;
    receiver = D(self + 0x3840u);
    eax = cm_fn_0077ac30(receiver, buffer, offset, eax);
    if (eax == 0)
        eax = report(0x00a7ddbcu, scratch, scratch + 256, 0xbb0);
    return eax;
}
