/* Preferred-image VA 007e9050, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0055cb60(u32 self, u32 owner, s8 type);
extern void CM_THISCALL cm_fn_0055cb20(u32 self, u32 owner, s8 first, u8 second, u32 index);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 pointer, u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007e9050(u32 self)
{
    u8 work[0x204];
    u32 count, destination, source, kept = 0, value, receiver, owner, entry, iteration, capacity, old, result, version;
    u16 word;
    u8 first, second;
    count = D(self + 0x13u);
    destination = D(self + 0xbu);
    D((u32)work) = 0;
    source = destination;
    if ((s32)count > 0) {
        do {
            if (D(source) != 0xffffffffu) {
                if (source != destination) {
                    value = D(source);
                    D(destination) = value;
                    value = D(source + 4u);
                    D(destination + 4u) = value;
                    value = D(source + 8u);
                    D(destination + 8u) = value;
                    word = W(source + 12u);
                    W(destination + 12u) = word;
                    first = B(self + 4u);
                    owner = D(source);
                    receiver = D(0x00ae28f0u);
                    entry = cm_fn_0055cb60(receiver, owner, (s8)first);
                    if (entry != 0) {
                        first = B(entry + 5u);
                        second = B(self + 5u);
                        if (first == second) {
                            D(entry + 6u) = kept;
                            goto row_done;
                        }
                    }
                    second = B(self + 5u);
                    first = B(self + 4u);
                    owner = D(source);
                    receiver = D(0x00ae28f0u);
                    cm_fn_0055cb20(receiver, owner, (s8)first, second, kept);
                }
row_done:
                destination += 14;
                ++kept;
            }
            iteration = D((u32)work);
            count = D(self + 0x13u);
            ++iteration;
            source += 14;
            D((u32)work) = iteration;
        } while ((s32)iteration < (s32)count);
    }
    capacity = kept + 100u;
    D(self + 0x13u) = kept;
    D(self + 0xfu) = capacity;
    old = D(self + 0xbu);
    result = (u32)cm_fn_00945667(old, capacity * 14u);
    D(self + 0xbu) = result;
    if (result != 0)
        return result;
    cm_fn_00944cff(0x00a7a858u, 0, 0, work + 0x104, work + 4);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x594u);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
