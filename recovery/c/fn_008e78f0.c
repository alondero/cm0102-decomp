/* Preferred-image VA 008e78f0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008e78f0(u32 self, u32 person, u32 club, u32 value)
{
    u8 scratch[0x200];
    if (!person || !club) {
        cm_fn_00944cff(0x00a9e318, 0, 0, scratch + 0x100, scratch);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x100, scratch, 0x4b1);
        u32 result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    u32 manager = D(0x00ae1050);
    u32 found = cm_fn_004cf5a0(manager, person);
    D(0x00ae1080) = found;
    u8 flag = 0;
    if (found) {
        u32 index = D(found + 4);
        u32 base = D(0x00ae23b4);
        u32 current_club = base + index * 581u;
        if (current_club) {
            manager = D(0x00ae1050);
            found = cm_fn_004cf5a0(manager, person);
            D(0x00ae1080) = found;
            if (found) {
                index = D(found + 4);
                base = D(0x00ae23b4);
                current_club = base + index * 581u;
            } else {
                current_club = 0;
            }
            u32 a = D(current_club + 0x57);
            u32 b = D(club + 0x57);
            if (a == b && cm_fn_0090cfc0(20) == 0) {
                u32 p = D(person + 0x61);
                if ((s16)W(p + 0xb) > 0x1b58) flag = 1;
            }
        }
    }
    B(self + 0x2d) = flag;
    D(self) = 0;
    D(self + 4) = 0;
    u32 club_id = D(club);
    D(self + 8) = club_id;
    u32 person_id = D(person);
    D(self + 0xc) = person_id;
    B(self + 0x39) = 10;
    B(self + 0x3e) = 10;
    B(self + 0x11) = 11;
    D(self + 0x28) = 0xffffffffu;
    D(self + 0x24) = 0xffffffffu;
    u8 old = B(self + 0x30);
    D(self + 0x1b) = value;
    u8 upper_nibble = old & 0xf0;
    B(self + 0x1f) = 0;
    D(self + 0x35) = 0;
    D(self + 0x3a) = 0;
    B(self + 0x3f) = 0;
    B(self + 0x40) = 0;
    B(self + 0x2c) = 0;
    B(self + 0x10) = 0;
    B(self + 0x2e) = 0;
    B(self + 0x2f) = 0;
    B(self + 0x30) = upper_nibble;
    return 0xffffff00u | upper_nibble;
}
