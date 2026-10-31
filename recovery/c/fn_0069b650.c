/* Preferred-image VA 0069b650, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 list);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern void CM_THISCALL cm_fn_0069b790(u32 self, u32 club, u32 first, u32 second, u32 zero);
extern u8 CM_CDECL cm_fn_00544820(u32 *club);
extern void CM_THISCALL cm_fn_0069ba50(u32 self, u32 club, u32 manager);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0069b650(u32 self)
{
    u8 work[0x204];
    u32 result, list, head, node, id, base, club, manager, first, second, version;
    s32 count;
    result = D(self + 4u);
    if (result == 0)
        return 0;
    head = D(result + 2u);
    D((u32)work) = 0;
    D(result + 0xau) = head;
    list = D(self + 4u);
    if ((s16)W(list) <= 0)
        return result;
    do {
        node = cm_fn_0068d1b0(list);
        if (node != 0) {
            id = D(node + 2u);
            base = D(0x00ae23b4u);
            club = base + id * 0x245u;
            manager = D(club + 0xcfu);
            if (manager == 0) {
                list = D(self + 4u);
                cm_fn_0068d1f0((u8 *)list, (const u8 *)node);
            } else {
                second = (id * 145u & 0xffffff00u) | B(node + 1u);
                first = (id & 0xffffff00u) | B(node);
                cm_fn_0069b790(self, club, first, second, 0);
                if (cm_fn_00544820((u32 *)club) != 0)
                    cm_fn_0069ba50(self, club, manager);
                list = D(self + 4u);
                cm_fn_0068d1f0((u8 *)list, (const u8 *)node);
            }
        } else {
            cm_fn_00944cff(0x009f18a4u, 0, 0, work + 0x104, work + 4);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x2b70u);
            cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
            D(0x00b67a34u) = 0;
        }
        list = D(self + 4u);
        result = D((u32)work) + 1u;
        count = (s32)(s16)W(list);
        D((u32)work) = result;
    } while ((s32)result < count);
    return result;
}
