/* Preferred-image VA 00896660, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip, u8 flag, u32 person, u32 force);
extern unsigned long long CM_THISCALL cm_fn_005fd210(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_005fea50(u32 self, s8 category, u32 person);
extern u32 CM_THISCALL cm_fn_005fbe20(u32 self, u32 flag, u32 club, u32 person);
extern u32 CM_THISCALL cm_fn_005fbd00(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00896660(u32 self, u32 club, volatile u32 flag)
{
    u8 work[0x200];
    u32 person, receiver, result, id, base, index, limit, version;
    if (club == 0) {
        cm_fn_00944cff(0x00a8b908u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x27fu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    result = cm_fn_00600a50(club, 1, 1, 0, 0);
    if (result != 0) {
        person = D(club + 0xcfu);
        receiver = D(0x00b63c98u);
        result = (u32)cm_fn_005fd210(receiver, person);
        if (result != 0) {
            person = D(club + 0xcfu);
            receiver = D(0x00b63c98u);
            result = cm_fn_005fea50(receiver, 2, person);
            if ((u8)result == 0)
                goto fallback;
        }
        person = D(club + 0xcfu);
        result = flag;
        receiver = D(0x00b63c98u);
        cm_fn_005fbe20(receiver, result, club, person);
        person = D(club + 0xcfu);
        receiver = D(0x00b63c98u);
        return cm_fn_005fbd00(receiver, person);
    }
fallback:
    id = D(club);
    base = D(self + 0x601u);
    index = (u32)(s32)(s16)W(base + id * 145u);
    limit = D(self + 0x5fdu);
    if ((s32)index < (s32)limit) {
        base = D(self + 0x5f9u);
    } else {
        index -= limit;
        base = D(self + 0x5f1u);
    }
    return base + index * 6371u;
}
