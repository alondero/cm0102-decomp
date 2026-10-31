/* Preferred-image VA 004ae770, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 text);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_004ae770(u32 self, u32 addition)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 old, cursor, remaining, first_length, second_length, allocation, version, line;
    u8 value;
    if (addition == 0) {
        name = work + 0x100;
        extension = work;
        line = 0xb1u;
        goto diagnostic;
    }
    old = D(self);
    if (old == 0)
        return (u32)cm_fn_004ae660((void *)self, addition);
    cursor = old;
    remaining = 0xffffffffu;
    do {
        value = B(cursor);
        ++cursor;
        --remaining;
    } while (remaining != 0 && value != 0);
    first_length = ~remaining - 1u;
    cursor = addition;
    remaining = 0xffffffffu;
    do {
        value = B(cursor);
        ++cursor;
        --remaining;
    } while (remaining != 0 && value != 0);
    second_length = ~remaining - 1u;
    allocation = (u32)cm_fn_00944cf1(first_length + second_length + 1u);
    D(self) = allocation;
    if (allocation == 0) {
        name = work;
        extension = work + 0x100;
        line = 0xc0u;
        goto diagnostic;
    }
    cm_fn_00944c9f(allocation, 0x0099ec04u, old, addition);
    cm_fn_00944c94((void *)old);
    return 1;
diagnostic:
    cm_fn_00944cff(0x0099ebd8u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
