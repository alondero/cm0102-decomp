/* Preferred-image VA 0048e1c0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   MOV AX,SI at0048e344 preserves the allocation's upper EAX half.
   The failure's XOR AX,AX likewise preserves the error callee's upper half. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0048e1c0(u32 self, u32 output, u32 unused __attribute__((unused)))
{
    u8 scratch[0x208];
    D((u32)scratch + 4) = self;
    u32 buffer = (u32)cm_fn_00944e46(0x320);
    if (!buffer) {
        cm_fn_00944cff(0x0099b80c, 0, 0, scratch + 0x108, scratch + 8);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x108, scratch + 8, 0x681);
        u32 result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        D(output) = 0;
        return result & 0xffff0000u;
    }
    u32 found = 0;
    s32 limit = (s32)D(self + 0x30);
    u8 stage_index = 0xff;
    B((u32)scratch + 3) = stage_index;
    if (limit >= -1) {
        do {
            u32 stage;
            if (stage_index == 0xff) stage = self;
            else {
                u32 table = D(self + 0xc);
                stage = D(table + (u32)(s32)(s8)stage_index * 4u);
            }
            u8 kind = B(stage + 0x43);
            u32 row = 0;
            if ((s16)W(stage + 0x3e) > 0) {
                do {
                    u32 index = (u32)(s32)(s16)row;
                    u32 club;
                    if (kind == 2) {
                        u32 table = D(stage + 0xb1);
                        club = D(table + index * 59u);
                    } else {
                        u32 table = D(stage + 0xa7);
                        club = D(table + index * 6u);
                    }
                    if (club) {
                        u32 search = 0;
                        if ((s16)found > 0) {
                            do {
                                if (D(buffer + (u32)(s32)(s16)search * 4u) == club) break;
                                ++search;
                            } while ((s16)search < (s16)found);
                        }
                        if ((s16)search == (s16)found && (s16)found < 200) {
                            u32 position = (u32)(s32)(s16)found;
                            ++found;
                            D(buffer + position * 4u) = club;
                        }
                    }
                    ++row;
                } while ((s16)row < (s16)W(stage + 0x3e));
                stage_index = B((u32)scratch + 3);
                self = D((u32)scratch + 4);
            }
            limit = (s32)D(self + 0x30);
            stage_index = (u8)(stage_index + 1);
            B((u32)scratch + 3) = stage_index;
        } while ((s32)(s8)stage_index <= limit);
    }
    D(output) = buffer;
    return (buffer & 0xffff0000u) | (u16)found;
}
