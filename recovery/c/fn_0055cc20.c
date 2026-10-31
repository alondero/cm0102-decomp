/* WIP: memory mismatch, case 51. Original 54a650 reads an uninitialized
   month BYTE at VA 0054a724 when negative day values skip every assignment.
   Preserve the caller translation; do not initialize the callee's stale byte. */
#include "cmre.h"

/* Preferred-image VA 0x0055cc20, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry ECX is forwarded to 55ce40. The three actual stack arguments
   remain object, destination and capacity; preserve all formatter words. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0055ce40(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00456740(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054a650(u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_007e5780(s32, u32, s32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0055cc20(u32 self, u32 object, u32 destination, s32 capacity)
{
    u8 scratch[0x2b8];
    u32 local = (u32)scratch, flag, scope, low, high, date, result, text;
    s32 scope_word, kind;
    if (capacity < 50) {
        cm_fn_00944cff(0x009b8f18, 0, 0, local + 0x1b8, local + 0xb8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x1b8, local + 0xb8, 0x1403);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    flag = (B(object + 0x13) >> 1) & 1u;
    scope = cm_fn_0055ce40(self, object + 12, local + 0x54, 100, flag, 4);
    low = D(0x00ae2c90);
    high = D(0x00ae2c94);
    date = cm_fn_00456740(object + 4, low, high);
    scope_word = (s32)(s8)scope;
    if (date != 0) {
        cm_fn_0054a650(object + 4, local + 0x20, 50, 255);
        kind = (s32)(s8)B(object);
        if (kind <= 0) {
            cm_fn_007e5780((s32)(s16)W(object + 2), local, 30, 0);
            return cm_fn_0066f4e0(destination, 0x009c332c, 100, scope_word, scope_word,
                                 local, local + 0x54, local + 0x20);
        }
        if (kind == 1)
            return cm_fn_0066f4e0(destination, 0x009c3400, scope_word, scope_word,
                                 100, local + 0x54, local + 0x20);
        return cm_fn_0066f4e0(destination, 0x009c3398, scope_word, scope_word,
                             100, kind, local + 0x54, local + 0x20);
    }
    kind = (s32)(s8)B(object);
    if (kind <= 0) {
        cm_fn_007e5780((s32)(s16)W(object + 2), local, 30, 0);
        return cm_fn_0066f4e0(destination, 0x009c327c, 100, scope_word, scope_word,
                             local, local + 0x54);
    }
    if (kind == 1)
        return cm_fn_0066f4e0(destination, 0x009c3300, scope_word, scope_word, local + 0x54);
    return cm_fn_0066f4e0(destination, 0x009c32c0, kind, scope_word, scope_word,
                         kind, local + 0x54);
}
