#include "cmre.h"
/* Preferred-image VA004cd6d0, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/fullEAX. Scratch210: percentage BYTE L0, dates L0/L8,
   diagnostics L10/L110. Original DWORD percentage loads have upper three
   bytes discarded by AND ff before any use; retain only the observed byte.
   54c8e0 THIS2/RET8 returns its fully written eight-byte output pointer.
   Auxiliary records leave byte+12 untouched, exactly as the original. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);

u32 CM_THISCALL cm_fn_004cd6d0(u32 self, volatile u32 person_arg,
                              volatile u32 date_arg, volatile u8 type_arg,
                              volatile u8 mode_arg)
{
    u8 scratch[0x210];
    u32 l = (u32)scratch, capacity, count, base, pointer, eax, version;
    u32 person, date, id, limit, map, record, percentage, output;
    u8 mode, byte;
    B(l) = 100;
    capacity = D(self + 0x10); count = D(self + 0x14);
    if ((s32)count > (s32)(capacity - 4u)) {
        pointer = D(self + 0xc);
        capacity += 100;
        D(self + 0x10) = capacity;
        if (pointer != 0) eax = (u32)cm_fn_00945667(pointer, capacity * 14u);
        else eax = (u32)cm_fn_00944e46(capacity * 14u);
        D(self + 0xc) = eax;
        if (eax == 0) {
            cm_fn_00944cff(0x009a8190, 0, 0, l + 0x110, l + 0x10);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009a8120, version,
                          l + 0x110, l + 0x10, 0x103e, 0x009a83e0);
            cm_fn_0066f4e0(0x00ae28bc, 0x009a8108);
            cm_fn_0066f4e0(0x00ae26c4, 0x009a7ff8, 0x00ae24d0);
            return cm_fn_005e8290(0x00ae28bc, 0x00ae26c4, 0);
        }
    }
    mode = mode_arg;
    date = date_arg;
    person = person_arg;
    if (mode == 1) {
        eax = D(person + 0x61);
        if (eax != 0) {
            pointer = 0;
            if (person != 0) {
                id = D(person); limit = G(0x00ae2364);
                if ((s32)id < (s32)limit) {
                    map = G(0x00ae2d0c);
                    id = D(map + id * 79u);
                    if ((s32)id >= 0) {
                        base = D(self);
                        pointer = base + id * 80u;
                    }
                }
            }
            byte = (u8)(B(pointer + 0x44) + 100u);
            B(l) = byte;
            if (byte <= 80) goto final_record;
            if (byte > 150) {
                count = D(self + 0x14); base = D(self + 0xc);
                percentage = B(l);
                id = D(person); D(base + count * 14u) = id;
                output = cm_fn_0054c8e0(date, l, percentage * 90u / 200u);
                base = D(self + 0xc); count = D(self + 0x14);
                record = base + count * 14u + 4;
                cm_fn_00417c10(record, l + 8, output);
                count = D(self + 0x14); base = D(self + 0xc);
                B(base + count * 14u + 0xd) = 2;
                count = D(self + 0x14); D(self + 0x14) = count + 1u;
                goto second_record;
            }
        }
        percentage = B(l);
second_record:
        count = D(self + 0x14); base = D(self + 0xc);
        id = D(person); D(base + count * 14u) = id;
        output = cm_fn_0054c8e0(date, l + 8, percentage * 50u / 200u);
        base = D(self + 0xc); count = D(self + 0x14);
        record = base + count * 14u + 4;
        cm_fn_00417c10(record, l, output);
        count = D(self + 0x14); base = D(self + 0xc);
        B(base + count * 14u + 0xd) = 2;
        count = D(self + 0x14); D(self + 0x14) = count + 1u;
    }
final_record:
    count = D(self + 0x14); base = D(self + 0xc);
    id = D(person); D(base + count * 14u) = id;
    count = D(self + 0x14); base = D(self + 0xc);
    byte = type_arg;
    B(base + count * 14u + 0xc) = byte;
    count = D(self + 0x14); base = D(self + 0xc);
    cm_fn_00417c10(base + count * 14u + 4, l + 8, date);
    count = D(self + 0x14); base = D(self + 0xc);
    byte = mode_arg;
    B(base + count * 14u + 0xd) = byte;
    eax = D(self + 0x14) + 1u;
    D(self + 0x14) = eax;
    return eax;
}
