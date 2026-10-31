#include "cmre.h"
/* Preferred-image VA 0x00729500, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_THISCALL cm_fn_00729500(u32 self, u32 index, u32 first, u32 second,
                              u16 date_first, u16 date_second)
{
    u8 scratch[512];
    u32 limit, base, record, current;
    char *version;
    if ((s32)index < -1) goto error;
    limit = D(self + 0x1c) - 1u;
    if ((s32)index > (s32)limit) goto error;
    if (index == 0xffffffffu) return 0;
    base = D(self);
    record = base + index * 12u;
    current = D(record);
    if (current == first) {
        if (D(record + 4) == second) goto dates;
    }
    if (current != second) return 0;
    if (D(record + 4) != first) return 0;
dates:
    if (W(record + 8) != date_first) return 0;
    if (W(record + 10) != date_second) return 0;
    return 1;
error:
    cm_fn_00944cff(0x00a17ba0, 0, 0, scratch + 256, scratch);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0xea);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
