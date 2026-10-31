#include "cmre.h"

/* Preferred-image VA 0x005feb60, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Only the counter's low byte is initialized. Reload the manager base after
   original calls and retain the original partial/full EAX results. */
extern u32 CM_THISCALL cm_fn_00896660(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008968f0(u32, u32, u32, u32);
extern u16 CM_THISCALL cm_fn_005b2ae0(u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_005feb60(u32 self, s8 category, u32 setting, u32 person)
{
    u8 scratch[0x208];
    u32 work = (u32)scratch, filename = work + 8, extension = work + 0x108;
    u32 line, index, value, count, base, row, club, receiver, object, saved, eax;
    u8 counter;
    char *version;
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) {
            filename = work + 0x108; extension = work + 8; line = 0x7bc;
            goto error;
        }
    }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) { line = 0x7c8; goto error; }
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) { line = 0x7c8; goto error; }
    base = D(self);
    row = index * 42735u;
    eax = (index * 8547u & 0xffffff00u) | (u8)category;
    B(base + row + (u32)(s32)category + 0x9b23) = (u8)setting;
    if (category != 3 || !setting) return eax;
    eax = D(person + 0x39);
    if (!eax) return eax;
    receiver = D(0x00de1d08);
    eax = cm_fn_00896660(receiver, eax, 0);
    counter = 0;
    D(work + 4) = eax;
    B(work) = counter;
    do {
        saved = D(work);
        club = D(person + 0x39);
        receiver = D(0x00de1d08);
        eax = cm_fn_008968f0(receiver, club, saved, 0);
        base = D(self);
        if (eax) {
            value = D(eax);
            D(base + row + 0x9b28) = value;
            if ((s8)counter < 11) {
                object = D(work + 4);
                eax = (u32)(s32)(s16)cm_fn_005b2ae0(object, (u8)saved, 0);
                base = D(self);
                D(base + row + 0x9b2c) = eax;
            } else {
                base = D(self);
                D(base + row + 0x9b2c) = 0xffffffffu;
            }
        } else {
            eax = 0xffffffffu;
            D(base + row + 0x9b28) = eax;
            base = D(self);
            D(base + row + 0x9b2c) = eax;
        }
        ++counter;
        row += 8;
        B(work) = counter;
    } while ((s8)counter < 16);
    return eax;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return eax;
}
