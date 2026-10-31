/* Preferred-image VA 0062c400, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern void CM_THISCALL cm_fn_0062c680(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_0054cc30(u32 self, void *output, u32 low, u32 high);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person)
{
    u8 work[0x208];
    u32 id, count, table, injury, high, low, year, elapsed;
    u32 text;
    s32 day, threshold;
    if (person == 0) {
        cm_fn_00944cff(0x009e7f28u, 0, 0, work + 0x108, work + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x108, work + 8, 0x556u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    id = D(person);
    count = D(0x00ae2364u);
    if ((s32)id >= (s32)count)
        return 0;
    cm_fn_0062c680(self, person);
    id = D(person);
    table = D(self);
    injury = D(table + id * 31u + 0xeu);
    if (injury == 0xffffffffu)
        return 0;
    table = D(self + 4u);
    injury = table + injury * 48u;
    high = D(injury + 0x24u);
    low = D(injury + 0x20u);
    cm_fn_0054cc30(0x00ae2c90u, work, low, high);
    /* Original reads a DWORD at +2, then keeps only its low WORD. */
    year = *(volatile u32 *)(work + 2);
    day = *(volatile s16 *)work;
    year &= 0xffffu;
    threshold = (s16)W(injury + 0x2au);
    elapsed = year * 365u + (u32)day;
    if ((s32)elapsed < threshold)
        return 2;
    threshold = (s16)W(injury + 0x28u);
    return (s32)elapsed < threshold;
}
