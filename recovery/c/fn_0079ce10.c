/* Preferred-image VA 0x0079CE10, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object) with one stack argument, ret 4. The
   object's dword +0x00 is an array base of 0x11-byte records; the argument
   points at a signed dword index followed by bytes +4, +5 and +6. Nothing
   happens if the base or argument is null, or the index is negative or not
   below dword[0x00AE2390]. Otherwise, re-reading the index and the base before
   each step as the listing does, for record e = base + index * 0x11:
   byte e+0x0f += arg byte +4; word e+0x08 += (s8)arg byte +5; word e+0x0a +=
   (s8)arg byte +6; byte e+0x10 = arg byte +6; word e+0x0c += 1. When word
   e+0x0c is then nonzero, the float at e+0x04 = (s16)word e+0x0a divided by
   (s16)word e+0x0c in x87 extended precision, stored as single precision.
   Field meanings remain unknown. */
#include "cmre.h"

static volatile u8 *cm_fn_0079ce10_record(void *self, u8 *arg)
{
    s32 index = *(volatile s32 *)arg;
    u8 *base = *(u8 *volatile *)self;

    return (volatile u8 *)(base + (u32)index * 0x11u);
}

void CM_THISCALL cm_fn_0079ce10(void *self, u8 *arg)
{
    volatile u8 *e;
    s32 index;
    u8 value;
    s16 divisor;

    if (*(u8 **)self == 0 || arg == 0) {
        return;
    }
    index = *(s32 *)arg;
    if (index < 0 || index >= CM_GLOBAL(s32, 0x00ae2390)) {
        return;
    }
    e = cm_fn_0079ce10_record(self, arg);
    e[0x0f] = (u8)(e[0x0f] + arg[4]);
    e = cm_fn_0079ce10_record(self, arg);
    *(volatile u16 *)(e + 0x08) = (u16)(*(volatile u16 *)(e + 0x08) + (u16)(s16)(s8)arg[5]);
    e = cm_fn_0079ce10_record(self, arg);
    *(volatile u16 *)(e + 0x0a) = (u16)(*(volatile u16 *)(e + 0x0a) + (u16)(s16)(s8)arg[6]);
    value = arg[6];
    e = cm_fn_0079ce10_record(self, arg);
    e[0x10] = value;
    e = cm_fn_0079ce10_record(self, arg);
    *(volatile u16 *)(e + 0x0c) = (u16)(*(volatile u16 *)(e + 0x0c) + 1);
    e = cm_fn_0079ce10_record(self, arg);
    divisor = *(volatile s16 *)(e + 0x0c);
    if (divisor != 0) {
        *(volatile float *)(e + 0x04) = (float)((long double)(s32)*(volatile s16 *)(e + 0x0a)
                                                / (long double)(s32)divisor);
    }
}
