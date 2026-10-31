#include "cmre.h"

/* Preferred-image VA 0x00803710, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Signed WORD copy length, forward DWORD/byte copies and continuing
   diagnostics retain the original overflow and overlap behavior. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00564080(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0077ac30(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static u32 network_error(u32 file, u32 name, u32 ext, u32 line)
{
    u32 text, result;
    cm_fn_00944cff(file, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}

u32 CM_THISCALL cm_fn_00803710(u32 self, u32 string)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, buffer = self + 0x37fe;
    u32 used, capacity, data, scan, remaining, length, copy_length, dest;
    u32 words, tail, value, selected, raw_target, owner, result;
    u8 byte;
    if (string == 0) string = 0x00ad9c64;
    D(self + 0x3802) = 0;
    used = D(buffer + 4);
    capacity = D(buffer + 8);
    if (used + 1u > capacity) {
        network_error(0x009c3734, local + 0x100, local, 0x85);
    } else {
        data = D(buffer);
        B(data + used) = 0x19;
        used = D(buffer + 4);
        D(buffer + 4) = used + 1u;
    }
    scan = string;
    remaining = 0xffffffffu;
    do {
        byte = B(scan);
        ++scan;
        --remaining;
    } while (byte != 0 && remaining != 0);
    used = D(buffer + 4);
    length = ~remaining;
    capacity = D(buffer + 8);
    if (used + 2u > capacity) {
        network_error(0x009c3734, local, local + 0x100, 0x86);
    } else {
        data = D(buffer);
        W(data + used) = (u16)length;
        used = D(buffer + 4);
        D(buffer + 4) = used + 2u;
    }
    used = D(buffer + 4);
    copy_length = (u32)(s32)(s16)length;
    data = D(buffer);
    dest = data + used;
    words = copy_length >> 2;
    scan = string;
    while (words != 0) {
        value = D(scan);
        D(dest) = value;
        scan += 4;
        dest += 4;
        --words;
    }
    tail = copy_length & 3u;
    while (tail != 0) {
        byte = B(scan);
        B(dest) = byte;
        ++scan;
        ++dest;
        --tail;
    }
    used = D(buffer + 4);
    D(buffer + 4) = used + copy_length;
    selected = (u32)(s32)(s16)W(self + 0x382a);
    raw_target = selected * 768u;
    raw_target = (raw_target & 0xffff0000u) | W(self + raw_target + 0x7d4);
    if ((u16)raw_target == 0) {
        owner = D(self + 0x3840);
        return cm_fn_00564080(self + 0x3844, owner, 0, buffer);
    }
    used = D(self + 0x3802);
    data = D(buffer);
    owner = D(self + 0x3840);
    result = cm_fn_0077ac30(owner, data, used, raw_target);
    if (result == 0)
        result = network_error(0x00a7ddbc, local, local + 0x100, 0xd02);
    return result;
}
