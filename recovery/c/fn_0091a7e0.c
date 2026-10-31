#include "cmre.h"

/* Preferred-image VA 0x0091a7e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The direct caller returns EAX unchanged. Keep each original incidental result. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
typedef u32 (CM_THISCALL *SetStageCall)(u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0091a7e0(u32 self)
{
    u8 scratch[0x20c];
    u32 local = (u32)scratch, value, table, object, club, row, position, target;
    u32 group, winner, offset, entrant, member, vtable, result, text;
    s32 stage, maximum, count;
    u8 selector, low_max;
    value = D(self + 0xfb);
    if (value != 0) return value;
    selector = B(self + 0x101);
    D(self + 0xfb) = 1;
    if (selector == 0xff) {
        value = (u32)(s32)(s8)B(self + 0x100);
        table = D(self + 0xc);
        object = D(table + value * 4u);
        row = D(object + 0xb1);
        club = D(row);
        stage = -1;
        maximum = (s8)B(self + 0xff);
        if (maximum < -1) goto error;
        do {
            if (stage == -1) object = self;
            else {
                table = D(self + 0xc);
                object = D(table + (u32)stage * 4u);
            }
            count = CM_GLOBAL(volatile s16, object + 0x3e);
            if (count > 0) {
                row = D(object + 0xb1);
                for (position = 0; (s32)position < count; ++position, row += 59) {
                    if (D(row) == club) {
                        table = D(object + 0xb1);
                        B(table + position * 59u + 0x37) = 7;
                        vtable = D(self);
                        target = D(vtable + 0x44);
                        return ((SetStageCall)target)(self, club, 7, (u32)stage, 0, 0, 0);
                    }
                }
            }
            maximum = (s8)B(self + 0xff);
            ++stage;
        } while (stage <= maximum);
        goto error;
    }
    low_max = B(self + 0xff);
    D(local + 4) = 0xffffffffu;
    if ((s8)low_max < -1) return low_max;
    stage = -1;
    do {
        if (stage == -1) group = self;
        else {
            table = D(self + 0xc);
            group = D(table + (u32)stage * 4u);
        }
        table = D(self + 0xc);
        D(local + 8) = 0;
        winner = D(table + (u32)(s32)(s8)selector * 4u);
        if (CM_GLOBAL(volatile s16, winner + 0x3e) > 0) {
            D(local) = 0;
            do {
                table = D(winner + 0xa7);
                offset = D(local);
                row = table + offset;
                if (B(row + 5) == 1) {
                    member = 0;
                    if (CM_GLOBAL(volatile s16, group + 0x3e) > 0) {
                        do {
                            object = cm_fn_006826d0(group, member);
                            value = D(object);
                            club = D(row);
                            if (value == club) {
                                object = cm_fn_006826d0(group, member);
                                B(object + 0x37) = 7;
                                break;
                            }
                            count = CM_GLOBAL(volatile s16, group + 0x3e);
                            ++member;
                        } while ((s32)member < count);
                    }
                }
                offset = D(local);
                selector = B(self + 0x101);
                table = D(self + 0xc);
                offset += 6;
                D(local) = offset;
                entrant = D(local + 8);
                ++entrant;
                winner = D(table + (u32)(s32)(s8)selector * 4u);
                D(local + 8) = entrant;
                count = CM_GLOBAL(volatile s16, winner + 0x3e);
            } while ((s32)entrant < count);
            stage = (s32)D(local + 4);
        }
        maximum = (s8)B(self + 0xff);
        ++stage;
        D(local + 4) = (u32)stage;
    } while (stage <= maximum);
    return (u32)maximum;
error:
    cm_fn_00944cff(0x00ad4a6c, 0, 0, local + 0x10c, local + 0xc);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x10c, local + 0xc, 0x65f);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}
