/* Preferred-image VA 007aee80, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 list, u32 target);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 pointer);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007aee80(u32 self)
{
    u8 work[0x200];
    u32 index, first, second, third, base, person, old, version;
    if (B(self + 10u) != 4)
        goto error;
    index = D(self + 0x180u);
    if ((s32)index <= -1)
        goto error;
    third = D(self + 0x17cu);
    second = D(self + 0x178u);
    first = D(self + 0x174u);
    base = D(0x00ae23bcu);
    person = base + index * 110u;
    if (cm_fn_0053e990(person, first, second, third, self + 0x184u) == 0)
        goto error;
    index = D(self + 0x180u);
    base = D(0x00ae23bcu);
    person = base + index * 110u;
    if (person == 0)
        goto error;
    old = D(self + 0x170u);
    D(self + 0x190u) = 0xffffffffu;
    if (old != 0) {
        cm_fn_0068d1f0(0x00b74438u, old);
        cm_fn_00944c94(old);
        D(self + 0x170u) = 0;
    }
    D(self + 0x18cu) = 0xffffffffu;
    B(self + 0x194u) = 0xff;
    D(self + 0x195u) = 0xffffffffu;
    D(self + 0x199u) = 0;
    D(self + 0x19du) = 0;
    D(self + 0x1a1u) = 0xffffffffu;
    return 1;
error:
    cm_fn_00944cff(0x00a701b0u, 0, 0, work + 0x100, work);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x237u);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
