#include "cmre.h"

/* Preferred-image VA 0x005b45b0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep the full pushed argument words and the post-call argument reloads. */
extern u32 CM_THISCALL cm_fn_005b3c70(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005b3560(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_005b45b0(u32 self, volatile u32 first,
                              volatile u32 second, volatile u32 alternate,
                              volatile u32 grid)
{
    u8 scratch[0x208];
    u8 *name, *extension;
    u32 a = first, b, mode, coordinates, value, line;
    s8 index;
    s16 y;
    char *version;

    index = (s8)a;
    if (index < 0 || index > 10) {
        line = 0x48b; goto invalid_index;
    }
    index = *(volatile s8 *)&second;
    if (index < 0 || index > 10) {
        line = 0x493; goto invalid_index;
    }
    mode = alternate;
    if ((s8)mode < 0 || (s8)mode > 1) {
        line = 0x49b; goto invalid_index;
    }
    coordinates = grid;
    if ((s16)coordinates < 0 || (s16)coordinates > 2) {
        name = scratch + 0x108; extension = scratch + 8;
        line = 0x4a4; goto error;
    }
    y = *(volatile s16 *)((u32)&grid + 2);
    if (y < 0 || y > 3) {
        name = scratch + 0x108; extension = scratch + 8;
        line = 0x4a4; goto error;
    }
    cm_fn_005b3c70(self, a, mode, coordinates, (u32)scratch);
    b = second;
    cm_fn_005b3c70(self, b, mode, coordinates, (u32)(scratch + 4));
    value = *(volatile u32 *)(scratch + 4);
    a = first;
    cm_fn_005b3560(self, a, mode, coordinates, value);
    value = *(volatile u32 *)scratch;
    b = second;
    cm_fn_005b3560(self, b, mode, coordinates, value);
    *(volatile u32 *)(self + 0x187f) = 0;
    return 1;

invalid_index:
    name = scratch + 8; extension = scratch + 0x108;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, name, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
