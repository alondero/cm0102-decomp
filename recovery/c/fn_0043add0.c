/* Preferred-image VA 0043add0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);
extern void CM_CDECL cm_fn_004c08f0(u32 table, s32 count);
extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 club, u8 rank);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0043add0(u32 self)
{
    u8 work[0x200];
    u32 table, size, i, index, source, club, receiver, version, result, line;
    void *name, *extension;
    if (D(self + 8u) == 0) {
        name = work + 0x100;
        extension = work;
        line = 0x33fu;
        goto error;
    }
    size = (u32)(s32)(s16)W(self + 0x3eu) * 4u;
    table = (u32)cm_fn_00944e46(size);
    if (table == 0) {
        name = work;
        extension = work + 0x100;
        line = 0x347u;
        goto error;
    }
    i = 0;
    if ((s16)W(self + 0x3eu) > 0) {
        do {
            index = (u32)(s32)(s16)i;
            source = D(self + 0xb1u);
            ++i;
            club = D(source + index * 59u);
            D(table + index * 4u) = club;
        } while ((s16)i < (s16)W(self + 0x3eu));
    }
    size = (u32)(s32)(s16)W(self + 0x3eu);
    cm_fn_004c08f0(table, (s32)size);
    club = D(table);
    receiver = D(self + 8u);
    cm_fn_004a2540(receiver, club, 1);
    club = D(table + 4u);
    receiver = D(self + 8u);
    cm_fn_004a2540(receiver, club, 2);
    i = 3;
    if ((s16)W(self + 0x3eu) >= 3) {
        do {
            index = (u32)(s32)(s16)(i - 1u);
            receiver = D(self + 8u);
            club = D(table + index * 4u);
            cm_fn_004a2540(receiver, club, (u8)i);
            ++i;
        } while ((s16)i <= (s16)W(self + 0x3eu));
    }
    return (u32)cm_fn_009452ca(table);
error:
    cm_fn_00944cff(0x0098b014u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
