/* Preferred-image VA 005c87b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 list);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 list, u32 target);
/* The first two stack values are consumed only by 00549ef0, which reads
   the first as WORD and masks the second to WORD (VA00549ef0..00549f01). */
extern u32 CM_THISCALL cm_fn_005c6740(u32 self, u16 first, u16 second, u32 key, u32 other_key);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005c87b0(u32 self, u32 target, u32 mode)
{
    u8 work[0x200];
    u32 list, cursor, row, receiver, first_key, second_key, result, version;
    u16 first, second;
    if (target == 0 || D(self + 0x34c8u) == 0) {
        cm_fn_00944cff(0x009cd0a0u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1165u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    list = D(self + 0x34d4u);
    if (list != 0) {
        cursor = D(list + 2u);
        D(list + 10u) = cursor;
        receiver = D(self + 0x34d4u);
        row = cm_fn_0068d1b0(receiver);
        while (row != 0) {
            if (D(row) == target) {
                receiver = D(self + 0x34d4u);
                cm_fn_0068d1f0(receiver, row);
                list = D(self + 0x34d4u);
                cursor = D(list + 2u);
                D(list + 10u) = cursor;
            }
            receiver = D(self + 0x34d4u);
            row = cm_fn_0068d1b0(receiver);
        }
        list = D(self + 0x34d4u);
        cursor = D(list + 2u);
        D(list + 10u) = cursor;
    }
    if (mode != 0) {
        second_key = D(target + 0x10u);
        second = W(target + 2u);
        first = W(target);
        first_key = D(target + 0x14u);
        row = cm_fn_005c6740(self, first, second, first_key, second_key);
        if (row != 0) {
            D(row + 0x10u) = 0;
            B(row + 0x18u) = 0;
        }
    }
    receiver = D(self + 0x34c8u);
    return cm_fn_0068d1f0(receiver, target);
}
