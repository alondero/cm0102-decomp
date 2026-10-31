/* Preferred-image VA 0068a850, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern void CM_CDECL cm_fn_004c08f0(u32 table, u32 count);
extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 club, u8 position);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0068a850(u32 self)
{
    u8 work[0x200];
    u8 *name, *extension;
    u32 line, version, result, table, count, rows, value, i, offset, receiver;
    if (D(self + 8u) == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x189eu;
        goto error;
    }
    count = (u32)(s32)(s16)W(self + 0x3eu);
    table = (u32)cm_fn_00944e46(count * 4u);
    if (table == 0) {
        name = work;
        extension = work + 0x100;
        line = 0x18a6u;
        goto error;
    }
    i = 0;
    offset = 0;
    if ((s16)W(self + 0x3eu) > 0) {
        do {
            rows = D(self + 0xb1u);
            value = D(rows + offset);
            offset += 59u;
            D(table + i * 4u) = value;
            count = (u32)(s32)(s16)W(self + 0x3eu);
            ++i;
        } while ((s32)i < (s32)count);
    }
    count = (u32)(s32)(s16)W(self + 0x3eu);
    cm_fn_004c08f0(table, count);
    i = 0;
    if ((s16)W(self + 0x3eu) > 0) {
        do {
            value = D(table + i * 4u);
            if (value != 0) {
                receiver = D(self + 8u);
                cm_fn_004a2540(receiver, value, (u8)(i + 1u));
            }
            count = (u32)(s32)(s16)W(self + 0x3eu);
            ++i;
        } while ((s32)i < (s32)count);
    }
    return (u32)cm_fn_009452ca(table);
error:
    cm_fn_00944cff(0x009f15f4u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
