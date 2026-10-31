/* Preferred-image VA 0085a9b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007875e0(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_0085aae0(u32 self, u32 person, u32 mode, u32 zero);
extern void CM_STDCALL cm_fn_0085b020(u32 person, u32 nation, u8 mode);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
typedef u16 (CM_THISCALL *row_method)(u32 self, void *output, u32 mode);

u32 CM_THISCALL cm_fn_0085a9b0(u32 self, u32 mode)
{
    u8 work[0x10];
    u32 receiver, count, pointer, index, nation, table, object, vtable, target, person, result, phase, remaining;
    u16 year, raw_count;
    D((u32)work + 0xcu) = self;
    D((u32)work) = 0;
    if ((u8)mode == 1) {
        year = W(0x00ae2c92u);
        W(self) = year;
        count = D(0x00ae2364u);
        pointer = D(0x00ae23bcu);
        index = 0;
        if ((s32)count > 0) {
            pointer += 0x6d;
            do {
                if (B(pointer) != 0)
                    B(pointer) = 0;
                count = D(0x00ae2364u);
                ++index;
                pointer += 0x6e;
            } while ((s32)index < (s32)count);
        }
    }
    nation = mode;
    D((u32)work + 4u) = 0;
    D((u32)work + 8u) = 3;
    do {
        phase = D((u32)work + 4u);
        if (phase == 0)
            nation = D(0x009cf6e8u);
        else if (phase == 1)
            nation = D(0x009cf6f0u);
        else if (phase == 2)
            nation = D(0x009cf6f8u);
        /* The native out-of-range branch retains the previous nation. */
        table = D(0x00adadfcu);
        object = D(table + nation * 4u);
        if (object != 0) {
            vtable = D(object);
            target = D(vtable + 0x58u);
            raw_count = ((row_method)target)(object, work, mode);
            count = (u32)(s32)(s16)raw_count;
            index = 0;
            if ((s32)count > 0) {
                do {
                    pointer = D((u32)work);
                    person = D(pointer + index * 4u);
                    if (person != 0 && D(person + 0x1dbu) == nation) {
                        receiver = D(0x00b7431cu);
                        if (cm_fn_007875e0(receiver, person) != 0) {
                            receiver = D((u32)work + 0xcu);
                            (void)receiver; /* Accepted0085b020 boundary has no receiver input. */
                            cm_fn_0085b020(person, nation, (u8)mode);
                        } else {
                            receiver = D(0x00dd7ef0u);
                            cm_fn_0085aae0(receiver, person, mode, 0);
                        }
                    }
                    ++index;
                } while ((s32)index < (s32)count);
            }
            pointer = D((u32)work);
            if (pointer != 0) {
                cm_fn_009452ca(pointer);
                D((u32)work) = 0;
            }
        }
        result = D((u32)work + 4u);
        remaining = D((u32)work + 8u);
        ++result;
        --remaining;
        D((u32)work + 4u) = result;
        D((u32)work + 8u) = remaining;
    } while (remaining != 0);
    return result;
}
