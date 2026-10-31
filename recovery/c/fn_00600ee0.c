#include "cmre.h"

/* Preferred-image VA 0x00600ee0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The table address is cached before the original helper call, but the mode
   is reloaded for each populated entry. Scratch+4 stays unknown without a club. */
extern u32 CM_CDECL cm_fn_0053b4f0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_FASTCALL cm_fn_00600ee0(u32 self)
{
    u8 scratch[0x208];
    u32 work = (u32)scratch, index, person, value, count, base, row, slot;
    u32 table, club, candidate, other, saved;
    u8 mode;
    char *version;

    index = (u32)(s32)(s16)W(0x00b77c7a);
    person = D(0x00b74c26 + index * 768u);
    if (!person) {
        cm_fn_00944cff(0x009cf11c, 0, 0, scratch + 0x108, scratch + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x108, scratch + 8, 0xb90);
        value = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return value;
    }
    value = D(person);
    count = D(0x00ae2364);
    base = D(self);
    row = (value - count + 16u) * 42735u;
    slot = B(base + row + 0x9a69);
    if (slot == 1) slot = 0;
    slot = (u32)(s32)(s8)slot;
    club = D(person + 0x24);
    table = base + slot * 7885u + row + 0x1e05;
    if (club) {
        value = D(club + 0x53);
        value = cm_fn_0053b4f0(value);
        D(work + 4) = value;
    }
    D(work) = 50;
    do {
        index = D(table);
        if (index != 0xffffffffu) {
            base = D(0x00ae23bc);
            candidate = base + index * 110u;
            base = D(self);
            mode = B(base + row + 0x9a69);
            if (mode == 0 || mode == 1) {
                value = D(candidate + 0x39);
                other = D(person + 0x39);
                if (value != other) D(table) = 0xffffffffu;
            } else if (mode == 2 || mode == 3) {
                value = D(candidate + 0x24);
                other = D(person + 0x24);
                if (value != other || !(B(candidate + 0x5e) & (mode == 3 ? 8u : 4u)))
                    D(table) = 0xffffffffu;
            } else if (mode == 4) {
                saved = D(work + 4);
                value = D(candidate + 0x24);
                if (value != saved || !(B(candidate + 0x5e) & 4u)) D(table) = 0xffffffffu;
            }
        }
        count = D(work);
        table += 4;
        --count;
        D(work) = count;
    } while (count);
    return 4;
}
