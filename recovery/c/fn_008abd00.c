/* Preferred-image VA 008abd00, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_008aa9d0(u32 self, u32 index, u32 output, u32 mode);
extern u32 CM_THISCALL cm_fn_008ab480(u32 self, u32 index, u32 mode);
extern u32 CM_THISCALL cm_fn_008abb50(u32 self, u32 index, u32 mode);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008abd00(u32 self, u32 index, u32 mode)
{
    u8 work[0x204];
    u32 version, selected, opposite, raw_index, value;
    u8 next;
    if ((s8)index < 0 || (s8)index > 19) {
        cm_fn_00944cff(0x00a8b94cu, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x1f56u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    if (D(self + 0xde5u) == 0)
        return 0;
    if (cm_fn_008aa9d0(self, index, 0, mode) == 0)
        return 0;
    selected = cm_fn_008ab480(self, index, mode);
    if (selected == 0)
        return 0;
    B((u32)work) = 0;
    opposite = mode == 0;
    do {
        /* Native initializes only the low byte of the DWORD passed to helpers. */
        raw_index = D((u32)work);
        value = cm_fn_008aa9d0(self, raw_index, 0, opposite);
        if (value != 0) {
            value = cm_fn_008abb50(self, raw_index, opposite);
            if (value == selected)
                return cm_fn_008ab480(self, raw_index, opposite);
        }
        next = (u8)(B((u32)work) + 1u);
        B((u32)work) = next;
    } while ((s8)next < 20);
    return 0;
}
