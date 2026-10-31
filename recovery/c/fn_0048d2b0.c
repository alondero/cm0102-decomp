/* Preferred-image VA 0048d2b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_004b01d0(s32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0048d2b0(u32 self, u32 index)
{
    u8 work[0x200];
    u32 pointer, table, target, previous, scan, version;
    if ((s32)index < -1 || (s32)index > (s32)D(self + 0x30u)) {
        cm_fn_00944cff(0x0099b80cu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x36eu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        pointer = 0;
    } else if (index == 0xffffffffu) {
        pointer = self;
    } else {
        table = D(self + 12u);
        pointer = D(table + index * 4u);
    }
    target = cm_fn_004b01d0((s16)W(pointer + 0x3au));
    if ((s32)target <= -1 || (s32)index <= -1)
        return index;
    scan = index - 1u;
    do {
        if ((s32)scan < -1 || (s32)scan > (s32)D(self + 0x30u)) {
            cm_fn_00944cff(0x0099b80cu, 0, 0, work, work + 0x100);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work, work + 0x100, 0x36eu);
            cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
            pointer = 0;
            D(0x00b67a34u) = pointer;
        } else if (scan == 0xffffffffu) {
            pointer = self;
        } else {
            table = D(self + 12u);
            pointer = D(table + scan * 4u);
        }
        previous = cm_fn_004b01d0((s16)W(pointer + 0x3au));
        if (previous != target)
            break;
        --index;
        --scan;
    } while ((s32)index > -1);
    return index;
}
