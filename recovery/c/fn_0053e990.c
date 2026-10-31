#include "cmre.h"
/* Preferred-image VA 0x0053e990, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_THISCALL cm_fn_004a7f20(u32, u32, u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second,
                           u32 third, u32 date)
{
    u8 scratch[512];
    u32 pointer, value, high, low;
    char *version;
    if (person == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 256, scratch, 0x3d90);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }
    pointer = D(person + 0x61u);
    if (pointer == 0) {
        pointer = D(person + 0x69u);
        if (pointer == 0)
            return 0;
    }
    pointer = D(person + 4u);
    if (pointer != 0) {
        value = D(pointer + 0x33u);
        if (value != first) return 0;
    }
    pointer = D(person + 8u);
    if (pointer != 0) {
        value = D(pointer + 0x33u);
        if (value != second) return 0;
    }
    pointer = D(person + 12u);
    if (pointer != 0) {
        value = D(pointer + 0x33u);
        if (value != third) return 0;
    }
    if (date != 0) {
        high = D(person + 0x14u);
        low = D(person + 0x10u);
        if (cm_fn_004a7f20(date, low, high) == 0)
            return 0;
    }
    return 1;
}
