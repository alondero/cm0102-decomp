#include "cmre.h"
/* Preferred-image VA 0x0058d610, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_0058d610(u32 self, u32 count_out)
{
    u8 scratch[0x204];
    u32 work = (u32)scratch, allocation, count, base, result;
    u32 selected = 0, index = 0, offset = 0, cursor, nation, value;
    char *version;
    count = D(0x00ae2350);
    allocation = (u32)cm_fn_00944e46(count * 4u);
    result = allocation;
    D(work) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x009c74f4, 0, 0, scratch + 0x104, scratch + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 0x104, scratch + 4, 0x1e3);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    count = D(0x00ae2350);
    base = D(0x00ae23a8);
    if ((s32)count > 0) {
        nation = base + 0x71;
        cursor = result;
        do {
            if (D(nation)) {
                value = offset + D(self);
                ++selected;
                D(cursor) = value;
                result = D(work);
                cursor += 4;
            }
            ++index;
            offset += 0x2c;
            nation += 0x122;
            count = D(0x00ae2350);
        } while ((s32)index < (s32)count);
    }
    cm_fn_00945333(result, selected, 4, 0x0058e240);
    D(count_out) = selected;
    return result;
}
