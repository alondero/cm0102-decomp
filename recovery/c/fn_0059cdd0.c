/* Preferred-image VA 0059cdd0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00456770(u32 self, u32 low, u32 high);
extern u32 CM_CDECL cm_fn_0053b900(u32 club);
extern void CM_THISCALL cm_fn_0059cfa0(u32 self, u32 club);
extern u8 CM_CDECL cm_fn_00669360(u32 club);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0059cdd0(u32 self, volatile u32 group)
{
    u8 work[0x208];
    u32 high, low, result, club, offset, pointer, target, saved, source, destination, value, i, index, total, version;
    u8 kind;
    high = D(0x00dd6eccu);
    low = D(0x00dd6ec8u);
    D((u32)work + 4u) = self;
    result = cm_fn_00456770(0x00ae2c90u, low, high);
    if (result != 0) return result;
    target = group;
    if (target == 0) {
        cm_fn_00944cff(0x009c8784u, 0, 0, work + 0x108, work + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x1612u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    result = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    D((u32)work) = 0;
    offset = 0;
    if ((s32)result <= 0) return result;
    for (;;) {
        result = cm_fn_0053b900(club);
        if (result != 0) return result;
        pointer = D(club + 0x57u);
        if (pointer != 0) {
            pointer = D(club + 0x53u);
            if (pointer != 0) {
                kind = B(pointer + 0x11cu);
                if (kind != 0 || (s16)W(club + 0x80u) >= 0x1388) {
                    kind = cm_fn_00669360(club);
                    pointer = 0x00b63d60u + (u32)(s32)(s8)kind * 72u;
                    target = group;
                    if (pointer == target) {
                        cm_fn_0059cfa0(self, club);
                        pointer = D(self);
                        saved = D((u32)work + 4u);
                        source = pointer + offset + 0x110u;
                        destination = pointer + offset + 0xc0u;
                        for (i = 0; i < 20u; ++i) {
                            value = D(source);
                            D(destination) = value;
                            source += 4u;
                            destination += 4u;
                        }
                        pointer = D(saved);
                        self = D((u32)work + 4u);
                        destination = pointer + offset + 0x110u;
                        for (i = 0; i < 20u; ++i) {
                            D(destination) = 0;
                            destination += 4u;
                        }
                    }
                }
            }
        }
        index = D((u32)work);
        total = D(0x00ae235cu);
        result = index + 1u;
        offset += 0x167u;
        club += 0x245u;
        D((u32)work) = result;
        if ((s32)result >= (s32)total) return result;
    }
}
