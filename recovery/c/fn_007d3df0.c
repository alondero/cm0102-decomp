#include "cmre.h"

/* Preferred-image VA 0x007d3df0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The direct constructor caller overwrites EAX at VA 0x007d3dd6. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define B(a) (*(volatile u8 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

void CM_THISCALL cm_fn_007d3df0(u32 self)
{
    u8 scratch[512];
    u32 rows;
    char *version;

    B(self + 8u) = 2;
    rows = (u32)cm_fn_00944e46(24);
    D(self + 4u) = rows;
    if (rows == 0) {
        cm_fn_00944cff(0x00a78844u, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                      scratch + 256, scratch, 0x4f);
        cm_fn_005e8290((const char *)0x009870e0u,
                      (const char *)0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return;
    }
    B(rows) = 0x13;
    B(rows + 1u) = 0;
    B(rows + 2u) = 0xff;
    B(rows + 3u) = 1;
    B(rows + 4u) = 5;
    B(rows + 5u) = 1;
    rows = D(self + 4u);
    B(rows + 6u) = 0x13;
    rows += 6u;
    B(rows + 1u) = 0;
    B(rows + 2u) = 0xff;
    B(rows + 3u) = 1;
    B(rows + 4u) = 8;
    B(rows + 5u) = 0;
    rows = D(self + 4u) + 12u;
    B(rows) = 0x13;
    B(rows + 1u) = 1;
    B(rows + 2u) = 0xff;
    B(rows + 3u) = 15;
    B(rows + 4u) = 11;
    B(rows + 5u) = 1;
    rows = D(self + 4u);
    B(rows + 22u) = 0;
    rows += 18u;
    B(rows + 5u) = 0;
    B(rows) = 0x13;
    B(rows + 1u) = 1;
    B(rows + 2u) = 0xff;
    B(rows + 3u) = 15;
}
