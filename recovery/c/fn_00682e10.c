/* Preferred-image VA 00682e10, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller VA00689ba4 overwrites EAX at00689ba9. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 club);
extern void CM_THISCALL cm_fn_005999a0(u32 self, u32 amount);
extern void CM_THISCALL cm_fn_0048e3e0(u32 self, u32 club, u32 low, u32 high, u8 rank, u16 zero, u32 other_zero, s8 last);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);
typedef void (CM_THISCALL *Method)(u32 receiver, u32 league);

void CM_THISCALL cm_fn_00682e10(u32 self)
{
    u8 work[0x204];
    u8 mode;
    s16 count;
    volatile s32 dividend, divisor;
    u32 table, record, index, receiver, vtable, target, sum, i, amount, club, offset, version;
    mode = B(self + 0xe1u);
    if (mode == 2)
        return;
    if (mode == 3) {
        table = D(0x00adadfcu);
        if (table == 0) {
            cm_fn_00944cff(0x009f15f4u, 0, 0, work + 0x104, work + 4);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x1dfu);
            cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
            D(0x00b67a34u) = 0;
            return;
        }
        record = D(self + 4u);
        index = D(record);
        receiver = D(table + index * 4u);
        vtable = D(receiver);
        target = D(vtable + 0x6cu);
        ((Method)target)(receiver, self);
        return;
    }
    count = (s16)W(self + 0x3eu);
    sum = 0;
    i = 0;
    if (count > 0) {
        do {
            sum += i + 1u;
            ++i;
        } while ((s16)i < count);
    }
    divisor = (s16)sum;
    dividend = (s32)D(self + 0x28u);
    D((u32)work) = (u32)(dividend / divisor);
    i = 0;
    if (count <= 0)
        return;
    do {
        index = (u32)(s32)(s16)i;
        amount = ((u32)(s32)count - index) * D((u32)work);
        offset = index * 59u;
        table = D(self + 0xb1u);
        club = D(table + offset);
        receiver = D(0x00ae2a38u);
        receiver = cm_fn_005a0590(receiver, club);
        cm_fn_005999a0(receiver, amount);
        if ((s32)amount > 1000) {
            table = D(self + 0xb1u);
            club = D(table + offset);
            cm_fn_0048e3e0(self, club, amount, 0, (u8)(i + 1u), 0, 0, -2);
        }
        count = (s16)W(self + 0x3eu);
        ++i;
    } while ((s16)i < count);
}
