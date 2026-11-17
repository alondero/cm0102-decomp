#include "cmre.h"
/* Original instruction evidence: preferred-image VAs 008caee0..008cb0e3,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * ECX receiver at entry, ret 0; callee receivers and pushes at
 * 008caf19..008caf22, 008caf47..008caf4e, 008caf6b..008caf8c,
 * 008cafe4..008cafe7, 008cb029..008cb02b, 008cb061..008cb068.
 * Allocation/free cdecl pushes and cleanup: 008caeec..008caf16,
 * 008cb0c6..008cb0d9. */
extern u32 CM_THISCALL cm_fn_00599800(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00599890(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *, const u8 *);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void *CM_CDECL cm_fn_00945501(u32, u32);
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

void CM_THISCALL cm_fn_008caee0(u32 self)
{
    u32 a = (u32)cm_fn_00945501(D(0xae235c), 1);
    u32 b = (u32)cm_fn_00945501(D(0xae235c), 1);
    u32 list = self + 0x872;
    u32 n, dst, src, value, index, address, offset, cursor, count;
    u8 flag, left, right;
    long double t;
    D(list + 0xa) = D(self + 0x874);
    n = cm_fn_0068d1b0(list);
    while (n) {
        index = D(n + 8);
        address = D(0xae23b4) + index * 0x245u;
        dst = cm_fn_005a0590(D(0xae2a38), address);
        address = D(0xae23b4);
        index = D(n + 4);
        address += index * 0x245u;
        src = cm_fn_005a0590(D(0xae2a38), address);
        value = D(n);
        cm_fn_00599890(src, value, (u32)((s32)value >> 31));
        value = D(n);
        cm_fn_00599800(dst, value, (u32)((s32)value >> 31));
        flag = (u8)(B(n + 12) - 1);
        B(n + 12) = flag;
        if ((s8)flag < 4) {
            t = (long double)(s32)D(n) * (long double)CM_GLOBAL(double, 0x968918);
            /* 008cafa7: original __ftol, low DWORD consumed. */
            value = (u32)(long long)t;
            D(src + 0x1c) = D(src + 0x1c) + value;
            t = (long double)(s32)D(n) * (long double)CM_GLOBAL(double, 0x968f30);
            /* 008cafd5: original __ftol. */
            value = (u32)(long long)t;
            D(dst + 0x1c) = D(dst + 0x1c) + value;
        } else if ((s8)flag < 8) {
            t = (long double)(s32)D(n) * (long double)CM_GLOBAL(double, 0x967880);
            /* 008cafc5: original __ftol. */
            value = (u32)(long long)t;
            D(src + 0x1c) = D(src + 0x1c) + value;
            t = (long double)(s32)D(n) * (long double)CM_GLOBAL(double, 0x969518);
            /* 008cafd5: original __ftol. */
            value = (u32)(long long)t;
            D(dst + 0x1c) = D(dst + 0x1c) + value;
        }
        if (!B(n + 12)) {
            cm_fn_0068d1f0((u8 *)list, (const u8 *)n);
        } else {
            index = D(n + 8);
            address = D(0xae23b4) + index * 0x245u;
            B(D(address) + a) = 1;
            index = D(n + 4);
            address = D(0xae23b4) + index * 0x245u;
            B(D(address) + b) = 1;
        }
        n = cm_fn_0068d1b0(list);
    }
    offset = 0;
    count = 0;
    if ((s32)D(0xae235c) > 0) {
        cursor = b;
        do {
            address = D(0xae23b4) + offset;
            dst = cm_fn_005a0590(D(0xae2a38), address);
            if (dst) {
                left = B((a - b) + cursor);
                if (!left && !B(cursor)) {
                    D(dst + 0x1c) = 0;
                } else {
                    right = B(cursor);
                    if (right) {
                        if (!left && (s32)D(dst + 0x1c) > 0)
                            D(dst + 0x1c) = 0;
                    } else if (left && (s32)D(dst + 0x1c) < 0) {
                        D(dst + 0x1c) = 0;
                    }
                }
            }
            ++count;
            offset += 0x245;
            ++cursor;
        } while ((s32)count < (s32)D(0xae235c));
    }
    if (a) cm_fn_009452ca(a);
    if (b) cm_fn_009452ca(b);
}
