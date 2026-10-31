/* Preferred-image VA 006697c0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00669470(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_006697c0(u32 input_year, u32 record, u32 destination, u32 size)
{
    u8 scratch[0x200];
    if (!destination || (s32)size < 10) {
        cm_fn_00944cff(0x009eaff4, 0, 0, scratch + 0x100, scratch);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x100, scratch, 0x350);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    u16 year = (u16)input_year;
    if (record) {
        s32 index = (s8)cm_fn_00669470(record);
        if (index != 0 && index != 1) {
            u32 row = 0x00b63d60u + (u32)index * 72u;
            u32 marker = D(row + 0x17);
            if (marker) {
                if (year == 0) {
                    year = W(0x00ae2c92);
                    u32 threshold = W(row + 0x15);
                    s32 day = (s16)W(0x00ae2c90);
                    if (day < (s32)threshold) year = (u16)(year - 1);
                }
                u32 next_digit = ((u32)year + 1) % 10u;
                cm_fn_00944c9f(destination, 0x009eb024, (u32)year, next_digit);
                return 1;
            }
        }
    }
    if (!year) year = W(0x00ae2c92);
    cm_fn_00944c9f(destination, 0x0098806c, (u32)year);
    return 1;
}
