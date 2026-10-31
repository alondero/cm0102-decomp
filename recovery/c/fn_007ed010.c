/* Preferred-image VA 007ed010, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007ed010(u32 self)
{
    u8 work[0x200];
    u32 allocation, version, total, club, iteration = 0, used = 0, offset = 0;
    u32 pointer, id, expected;
    W(self + 0x3eu) = 30;
    allocation = (u32)cm_fn_00944e46(180);
    D(self + 0xa7u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00a7ad70u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x114u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0xffffffffu;
    }
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    if ((s32)total > 0) {
        do {
            pointer = D(club + 0x57u);
            if (pointer != 0) {
                id = D(pointer);
                expected = D(0x009cf5e8u);
                if (id != expected) {
                    expected = D(0x009cf5e4u);
                    if (id != expected) {
                        expected = D(0x009cf5e0u);
                        if (id != expected)
                            goto next;
                    }
                }
                if ((s32)used < (s32)(s16)W(self + 0x3eu)) {
                    pointer = D(self + 0xa7u);
                    ++used;
                    D(pointer + offset) = club;
                    pointer = D(self + 0xa7u);
                    B(pointer + offset + 4u) = 0;
                    pointer = D(self + 0xa7u);
                    B(pointer + offset + 5u) = 0;
                    offset += 6;
                }
            }
next:
            ++iteration;
            club += 0x245u;
            total = D(0x00ae235cu);
        } while ((s32)(s16)iteration < (s32)total);
    }
    expected = (u32)(s32)(s16)W(self + 0x3eu);
    return used == expected;
}
