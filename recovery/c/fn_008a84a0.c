/* Preferred-image VA 008a84a0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u16 CM_CDECL cm_fn_006e1cf0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008ab480(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008a84a0(u32 self)
{
    u8 scratch[0x20c];
    if (D(self + 0xe40)) return 0;
    u32 club = D(self + 4);
    u8 selected = 0xff;
    if (!club) return 0;
    u16 best = 0;
    u32 entry = self + 0x2735;
    for (u32 i = 0; i < 20; ++i, entry += 0x1be) {
        if ((s8)B(entry + 0x19) >= 0) {
            u32 person = D(self);
            u16 score = cm_fn_006e1cf0(0, entry, person);
            if (selected == 0xff || (s16)score > (s16)best) {
                selected = (u8)i;
                best = score;
            }
        }
    }
    if ((s8)selected <= -1) return 0;
    u32 field = self + 0x279e + (u32)selected * 0x1be;
    if (D(field) == 0) {
        cm_fn_00944cff(0x00a8b94c, 0, 0, scratch + 0x10c, scratch + 0xc);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x10c, scratch + 0xc, 0x14e7);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    u8 current = 0xff;
    /* Only the byte was initialized; preserve the full pushed stack word. */
    B((u32)scratch + 4) = 0;
    for (u32 i = 0; i < 20; ++i) {
        u32 pushed_index = D((u32)scratch + 4);
        u32 found = cm_fn_008ab480(self, pushed_index, 0);
        if (found) {
            u32 id = D(found);
            u32 target = D(self + 0x4a20);
            if (id == target) { current = (u8)i; break; }
        }
        B((u32)scratch + 4) = (u8)(i + 1);
    }
    if (current == selected) return 1;
    u32 source = self + 0xe44;
    u32 destination = self + 0x4a63;
    for (u32 i = 0; i < 0xf07; ++i, source += 4, destination += 4) {
        u32 value = D(source);
        D(destination) = value;
    }
    u16 word = W(source);
    W(destination) = word;
    u8 byte = B(source + 2);
    B(destination + 2) = byte;
    D(self + 0xc4de) = 1;
    u32 object = D(field);
    u32 id = D(object);
    D(self + 0x4a5f) = 1;
    D(self + 0x4a20) = id;
    return 1;
}
