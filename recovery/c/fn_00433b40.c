#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_004c08f0(u32, u32);
extern void CM_THISCALL cm_fn_004a2540(u32, u32, u8);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00433b40(u32 self)
{
    u8 scratch[0x200];
    u32 count, allocation, index, offset, table, value, receiver, rank, name, extension, line, text, result;
    if (D(self + 8) == 0) {
        name = (u32)scratch + 0x100;
        extension = (u32)scratch;
        line = 0x304;
        goto error;
    }
    count = (u32)(s32)(s16)W(self + 0x3e);
    allocation = (u32)cm_fn_00944e46(count * 4u);
    if (allocation == 0) {
        name = (u32)scratch;
        extension = (u32)scratch + 0x100;
        line = 0x30c;
        goto error;
    }
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            offset = (u32)(s32)(s16)index;
            table = D(self + 0xb1);
            ++index;
            value = D(table + offset * 59u);
            D(allocation + offset * 4u) = value;
        } while ((s16)index < (s16)W(self + 0x3e));
    }
    count = (u32)(s32)(s16)W(self + 0x3e);
    cm_fn_004c08f0(allocation, count);
    /* The original always reads these eight slots, even for smaller counts. */
    for (index = 0; index != 8; ++index) {
        value = D(allocation + index * 4u);
        receiver = D(self + 8);
        rank = index == 0 ? 1u : index == 1 ? 2u : index < 4 ? 3u : 5u;
        cm_fn_004a2540(receiver, value, (u8)rank);
    }
    count = W(self + 0x3e);
    rank = 9;
    index = 8;
    if ((s16)count >= 9) {
        while ((s16)index < (s16)count) {
            offset = (u32)(s32)(s16)index;
            ++index;
            value = D(allocation + offset * 4u);
            receiver = D(self + 8);
            cm_fn_004a2540(receiver, value, (u8)rank);
            count = W(self + 0x3e);
            ++rank;
            if ((s16)rank > (s16)count)
                break;
        }
    }
    return (u32)cm_fn_009452ca(allocation);
error:
    cm_fn_00944cff(0x0098ae90, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}
