#include "cmre.h"
/* CM0102 3.9.68, preferred-image VA 0x005ce060, base 0x00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stack byte is signed; every loop reload and full EAX return is retained.
   Virtual boundary: ECX receiver, two callee-cleaned stack arguments.
   The nonzero-byte path returns the original tail callee's raw EAX. */
#define D(a) CM_GLOBAL(volatile u32, (a))
typedef u32 (CM_THISCALL *slot14_fn)(u32, u32, s32);
extern u32 CM_THISCALL cm_fn_005aaf00(u32);

u32 CM_CDECL cm_fn_005ce060(s8 flag)
{
    u32 count = D(0x00adadf8);
    u32 index = 0;
    if ((s32)count > 0) {
        do {
            u32 table = D(0x00adadfc);
            u32 receiver = D(table + index * 4u);
            if (receiver != 0) {
                u32 vtable = D(receiver);
                slot14_fn method = (slot14_fn)D(vtable + 0x14u);
                method(receiver, 0x00ae2c90u, (s32)flag);
            }
            count = D(0x00adadf8);
            index += 1u;
        } while ((s32)index < (s32)count);
    }
    if (flag != 0) {
        u32 receiver = D(0x00ae2a58);
        return cm_fn_005aaf00(receiver);
    }
    return count;
}
