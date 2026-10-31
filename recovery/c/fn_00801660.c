#include "cmre.h"
/* Preferred-image VA00801660, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/fullEAX; scratch200 diagnosticsL0/L100. Four independent
   unsigned capacity checks, including wrapping additions, continue on errors.
   Retain aliased buffer/length reloads and late WORD/BYTE formal reads.
   564080 and77ac30 are physical THIS3/RET12; capture their whole EAX.
   Final send's mode uses MOV AX and retains the scaled offset's upper half. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_THISCALL cm_fn_00564080(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0077ac30(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00801660(u32 self, volatile u16 word_arg,
                              volatile u8 byte_arg)
{
    u8 scratch[0x200];
    u32 l = (u32)scratch, buffer = self + 0x37fe, length, capacity, pointer;
    u32 eax, version, offset, record, context, mode;
    u16 word;
    u8 byte;
#define REPORT(source, first, second, line) do { \
        cm_fn_00944cff(source, 0, 0, first, second); \
        version = cm_fn_0090d130(); \
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line); \
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0); \
        G(0x00b67a34) = 0; \
    } while (0)
    D(self + 0x3802) = 0;
    length = D(buffer + 4); capacity = D(buffer + 8);
    if (length + 1u > capacity) {
        REPORT(0x009c3734, l + 0x100, l, 0x85);
    } else {
        pointer = D(buffer); B(pointer + length) = 8;
        length = D(buffer + 4); D(buffer + 4) = length + 1u;
    }
    length = D(buffer + 4); capacity = D(buffer + 8);
    if (length + 2u > capacity) {
        REPORT(0x009c3734, l, l + 0x100, 0x86);
    } else {
        pointer = D(buffer); word = word_arg;
        W(pointer + length) = word;
        length = D(buffer + 4); D(buffer + 4) = length + 2u;
    }
    length = D(buffer + 4); capacity = D(buffer + 8);
    if (length + 1u > capacity) {
        REPORT(0x009c3734, l, l + 0x100, 0x85);
    } else {
        pointer = D(buffer); B(pointer + length) = 6;
        length = D(buffer + 4); D(buffer + 4) = length + 1u;
    }
    length = D(buffer + 4); capacity = D(buffer + 8);
    if (length + 1u > capacity) {
        REPORT(0x009c3734, l, l + 0x100, 0x85);
    } else {
        pointer = D(buffer); byte = byte_arg;
        B(pointer + length) = byte;
        length = D(buffer + 4); D(buffer + 4) = length + 1u;
    }
    offset = (u32)(s32)(s16)W(self + 0x382a) * 768u;
    record = self + offset;
    word = W(record + 0x7d4);
    mode = (offset & 0xffff0000u) | word;
    if (word == 0) {
        context = D(self + 0x3840);
        return cm_fn_00564080(self + 0x3844, context, 0, buffer);
    }
    pointer = D(record + 0x7fc);
    if (D(pointer + 0x12f53d) != 0) {
        context = D(self + 0x3840);
        return cm_fn_00564080(self + 0x132ea5, context, 0, buffer);
    }
    length = D(self + 0x3802); pointer = D(buffer);
    context = D(self + 0x3840);
    eax = cm_fn_0077ac30(context, pointer, length, mode);
    if (eax == 0) {
        REPORT(0x00a7ddbc, l, l + 0x100, 0xb01);
    }
#undef REPORT
    return eax;
}
