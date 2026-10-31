#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
/* Full original 0053b620 reads only the argument BYTE and returns full 0/1. */
extern u32 CM_CDECL cm_fn_0053b620(u8);
extern u32 CM_CDECL cm_fn_0053c190(u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_0076abf0(u32);
extern u32 CM_THISCALL cm_fn_0076c120(u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static __attribute__((always_inline)) inline u32 report(u32 first, u32 second, u32 line)
{
    u32 version, eax;
    cm_fn_00944cff(0x00a5b2a4U, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0U, 0x009870e8U, version, first, second, line);
    eax = cm_fn_005e8290(0x009870e0U, 0x00ae24d0U, 0);
    D(0x00b67a34U) = 0;
    return eax;
}

/* Preferred-image VA0076a840/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full incidental EAX. Scratch208: second totalL0/counterL4,
   diagnostic buffersL8/L108. Both final allocations use the FIRST total. */
u32 CM_THISCALL cm_fn_0076a840(u32 self)
{
    u32 scratch[0x208 / 4];
    u32 l = (u32)scratch, eax, count, person, index = 0, total = 0;
    u32 pointer, nation, base, offset, row, value, sum, nation_offset;
    u16 word;
    u8 byte;

    count = D(0x00ae2350);
    D(l) = 0;
    eax = cm_fn_00945501(count, 0x6e);
    D(self + 4) = eax;
    if (eax == 0)
        return report(l + 0x108, l + 8, 0xd9);
    count = D(0x00ae2350);
    eax = cm_fn_00944e46(count * 4U);
    D(self + 8) = eax;
    count = D(0x00ae2350);
    eax = cm_fn_00945501(1, count);
    D(self + 0x36) = eax;
    count = D(0x00ae2364);
    person = D(0x00ae23bc);
    if ((s32)count > 0) {
        do {
            pointer = D(person + 0x1a);
            if (pointer != 0) {
                nation = D(pointer);
                base = D(self + 4);
                pointer = base + nation * 110U;
                word = W(pointer);
                W(pointer) = (u16)(word + 1U);
            }
            pointer = D(person + 0x24);
            if (pointer != 0) {
                byte = B(person + 0x28);
                eax = cm_fn_0053b620(byte);
                if (eax != 0) {
                    pointer = D(person + 0x24);
                    cm_fn_0053c190(pointer, person, 0, 1, 0);
                }
            }
            count = D(0x00ae2364);
            ++index;
            person += 110U;
        } while ((s32)index < (s32)count);
    }
    count = D(0x00ae2350);
    D(l + 4) = 0;
    index = 0;
    if ((s32)count > 0) {
        offset = 0;
        do {
            base = D(self + 4);
            D(base + offset + 4) = 0xffffffffU;
            base = D(self + 8);
            D(base + index * 4U) = 0xffffffffU;
            base = D(self + 4);
            D(base + offset + 8) = total;
            base = D(self + 4);
            row = base + offset;
            word = W(row);
            value = (u32)(s32)(s16)word;
            if ((s16)word < 100) {
                if ((s32)value < 10) value = 10;
            } else value = 100;
            B(row + 0xc) = (u8)value;
            base = D(self + 4);
            row = base + offset;
            byte = B(base + offset + 0xc);
            total += byte;
            if (byte < 100) {
                B(row + 0xe) = byte;
            } else {
                B(row + 0xe) = 10;
                base = D(self + 4);
                B(base + offset + 0x12) = 30;
                base = D(self + 4);
                B(base + offset + 0x16) = 30;
                base = D(self + 4);
                B(base + offset + 0x1a) = 30;
            }
            base = D(self + 4);
            value = D(l);
            D(base + offset + 0x1d) = value;
            base = D(self + 4);
            row = base + offset;
            word = W(base + offset);
            value = (u32)(s32)(s16)word;
            if ((s16)word < 50) {
                if ((s32)value < 10) value = 10;
            } else value = 50;
            B(row + 0x21) = (u8)value;
            base = D(self + 4);
            sum = D(l);
            row = base + offset;
            byte = B(row + 0x21);
            sum += byte;
            D(l) = sum;
            if (byte < 50) {
                byte = B(row + 0xc);
                B(row + 0x23) = byte;
            } else {
                B(row + 0x23) = 5;
                base = D(self + 4);
                B(base + offset + 0x27) = 15;
                base = D(self + 4);
                B(base + offset + 0x2b) = 15;
                base = D(self + 4);
                B(base + offset + 0x2f) = 15;
            }
            index = D(l + 4);
            count = D(0x00ae2350);
            ++index;
            offset += 110U;
            D(l + 4) = index;
        } while ((s32)index < (s32)count);
    }
    D(self + 0x14) = total;
    eax = cm_fn_00945501(total, 4);
    D(self + 0xc) = eax;
    if (eax == 0)
        return report(l + 8, l + 0x108, 0x12c);
    D(self + 0x18) = total;
    eax = cm_fn_00945501(total, 4);
    D(self + 0x10) = eax;
    if (eax == 0)
        return report(l + 8, l + 0x108, 0x137);
    cm_fn_0076abf0(self);
    eax = D(0x00ae2350);
    index = 1;
    if ((s32)eax > 1) {
        offset = 110;
        nation_offset = 0x122;
        do {
            base = D(0x00ae23a8);
            pointer = D(base + nation_offset + 0x71);
            nation = base + nation_offset;
            if (pointer != 0) {
                cm_fn_0076c120(self, nation, 0, 0x00ae2c90, 0, 0);
                base = D(0x00ae23a8);
                cm_fn_0076c120(self, base + nation_offset, 2, 0x00ae2c90, 0, 0);
                base = D(self + 4);
                B(base + offset + 0x43) = 0;
                base = D(self + 4);
                B(base + offset + 0x6b) = 0;
                base = D(self + 4);
                B(base + offset + 0x56) = 0x18;
                base = D(self + 4);
                B(base + offset + 0x57) = 0;
            }
            eax = D(0x00ae2350);
            ++index;
            nation_offset += 0x122;
            offset += 110U;
        } while ((s32)index < (s32)eax);
    }
    return eax;
}
