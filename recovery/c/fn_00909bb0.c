/* WIP: detail mismatch, case 1290. Synthetic HeapAlloc result 0x6e5d0d17
   faults in the original but overlaps the recovered image at LINK_BASE.
   Keep the fixture and original read; no acceptance until mapping is resolved. */
#include "cmre.h"

/* Preferred-image VA 0x00909bb0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The allocation multiplies in AX before sign extension. Each original
   count/table reload is retained, including the fixed twelve award calls. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004a2540(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00909bb0(u32 self)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, owner, allocated, index, offset, value, table;
    u32 nested, count, scan, output_index, text, name, ext, line, i, flag;
    u16 triple;
    triple = W(self + 0x3e);
    owner = D(self + 8);
    triple = (u16)(triple * 3u);
    if (owner == 0) {
        name = local + 0x100; ext = local; line = 0x675;
        goto error;
    }
    allocated = cm_fn_00944e46((u32)(s32)(s16)triple * 4u);
    if (allocated == 0) {
        name = local; ext = local + 0x100; line = 0x67d;
        goto error;
    }
    scan = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            index = (u32)(s32)(s16)(u16)scan;
            table = D(self + 0xb1);
            offset = index * 59u;
            value = D(table + offset);
            D(allocated + index * 4u) = value;
            table = D(self + 0x0c);
            count = (u32)(s32)(s16)W(self + 0x3e);
            nested = D(table);
            output_index = count + index;
            ++scan;
            table = D(nested + 0xb1);
            value = D(table + offset);
            D(allocated + output_index * 4u) = value;
            table = D(self + 0x0c);
            count = (u32)(s32)(s16)W(self + 0x3e);
            nested = D(table + 4);
            output_index = index + count * 2u;
            table = D(nested + 0xb1);
            value = D(table + offset);
            D(allocated + output_index * 4u) = value;
        } while ((s16)(u16)scan < (s16)W(self + 0x3e));
    }
    count = (u32)(s32)(s16)W(self + 0x3e);
    cm_fn_00945333(allocated, count, 4, 0x004b07d0);
    count = (u32)(s32)(s16)W(self + 0x3e);
    cm_fn_00945333(allocated + count * 4u, count, 4, 0x004b07d0);
    count = (u32)(s32)(s16)W(self + 0x3e);
    cm_fn_00945333(allocated + count * 8u, count, 4, 0x004b07d0);
    for (i = 0; i != 12; ++i) {
        value = D(allocated + i * 4u);
        owner = D(self + 8);
        flag = 1u + (i % 4u) * 3u;
        cm_fn_004a2540(owner, value, flag);
    }
    return cm_fn_009452ca(allocated);
error:
    cm_fn_00944cff(0x00aa1fb4, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    value = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return value;
}
