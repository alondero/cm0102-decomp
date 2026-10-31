/* Preferred-image VA 007d30f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00521e60(u32 object, u32 zero, u32 a, u32 b, u32 c, u32 d, s32 year);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32 object, u32 zero, u32 a, u32 b, u32 c, u32 d, u32 e, s32 year, u32 option);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007d30f0(u32 self, u8 kind, volatile u32 first,
                              volatile u32 second, volatile u32 optional)
{
    u8 work[0x200];
    u32 pointer, object, version;
    s32 year;
    if (kind != 0xffu)
        return 0;
    pointer = optional;
    if (pointer != 0)
        D(pointer) = 0;
    pointer = first;
    {
        u32 other = second;
        W(pointer) = 1;
        W(other) = 0;
    }
    object = (u32)cm_fn_00944e46(0x68u);
    if (object == 0) {
        cm_fn_00944cff(0x00a787d4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x95u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    year = (s16)W(self + 0x40u);
    cm_fn_00521e60(object, 0, 5, 6, 0, 5, year);
    year = (s16)W(self + 0x40u);
    if (year != 2000)
        cm_fn_00521eb0(object, 0, 12, 7, 0, 5, 1, year, 4);
    else
        cm_fn_00521eb0(object, 0, 12, 7, 0, 5, 1, 2000, 0);
    W(object + 7u) = 0;
    W(object + 9u) = 0;
    W(object + 11u) = 0;
    W(object + 13u) = 0;
    B(object + 0x17u) = 6;
    W(object + 0x18u) = 2;
    W(object + 0x1au) = 1;
    W(object + 0x1cu) = 2;
    W(object + 0x0fu) = 0;
    W(object + 0x11u) = 3;
    W(object + 0x1eu) = 0;
    B(object + 0x20u) = 1;
    B(object + 0x21u) = 2;
    B(object + 0x22u) = 0x7e;
    if (W(self + 0x40u) != 2000) {
        W(object + 0x0du) = 3;
        B(object + 0x21u) = 1;
    }
    D(object + 0x5cu) = 0;
    D(object + 0x60u) = 0;
    D(object + 0x64u) = 0;
    return object;
}
