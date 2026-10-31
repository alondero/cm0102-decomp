#include "cmre.h"

/* Preferred-image VA 0x005fd2f0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Count transitions before the field store; retain ordered news calls and their EAX results. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern u32 CM_FASTCALL cm_fn_006b5040(u32);
extern u32 CM_THISCALL cm_fn_007874b0(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_005fd2f0(u32 self, u32 new_value, u32 person)
{
    u8 scratch[512];
    u32 filename = (u32)scratch, extension = (u32)scratch + 256;
    u32 line, index, value, count, base;
    char *version;

    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) {
            filename = (u32)scratch + 256; extension = (u32)scratch;
            line = 0x41c;
            goto error;
        }
    }
    value = D(person);
    count = D(0x00ae2364);
    index = value - count + 16u;
    if ((s32)index < 0) { line = 0x428; goto error; }
    count = D(self + 4) - 1u;
    if ((s32)index > (s32)count) { line = 0x428; goto error; }
    base = D(self);
    base += index * 42735u + 0x9a01;
    value = D(base);
    if (new_value && !value) {
        count = D(self + 12);
        D(self + 12) = count + 1u;
    } else if (!new_value && value) {
        count = D(self + 12);
        D(self + 12) = count - 1u;
    }
    D(base) = new_value;
    value = base;
    if (new_value) {
        value = D(person + 0x39);
        if (value) {
            count = D(0x00b7431c);
            value = cm_fn_007874b0(count, person, value);
        }
        value = D(person + 0x24);
        if (value) {
            count = D(0x00b7431c);
            value = cm_fn_007874b0(count, person, value);
        }
    }
    count = D(0x00b67a50);
    if (count) return cm_fn_006b5040(count);
    return value;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    value = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return value;
}
