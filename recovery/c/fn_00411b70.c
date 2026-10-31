/* Preferred-image VA 00411b70, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club, u8 flag, u32 base);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00411b70(u32 self)
{
    u8 work[0x200];
    u32 allocation, count = 0, scanned = 0, total, club, record, id, selected;
    u32 version, line;
    void *name, *extension;
    W(self + 0x3eu) = 13;
    allocation = (u32)cm_fn_00944e46(0x2ffu);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x241u;
        goto diagnostic;
    }
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    if ((s32)total > 0) {
        do {
            if ((s16)count >= (s16)W(self + 0x3eu))
                break;
            record = D(club + 0x57u);
            if (record != 0) {
                id = D(record);
                selected = D(0x009cf89cu);
                if (id == selected) {
                    u8 index = (u8)count;
                    count += 1u;
                    cm_fn_00687430(self, index, club, 0, 0);
                }
            }
            total = D(0x00ae235cu);
            scanned += 1u;
            club += 0x245u;
        } while ((s32)(s16)scanned < (s32)total);
    }
    if ((u16)count == W(self + 0x3eu))
        return 1;
    name = work;
    extension = work + 0x100;
    line = 0x252u;
diagnostic:
    cm_fn_00944cff(0x00987c20u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
