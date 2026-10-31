/* Preferred-image VA 0x00947480, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_009488e0(u32 source);
extern s32 CM_CDECL cm_fn_0094dede(u32 left, u32 right, u32 count);
extern u32 CM_CDECL cm_fn_0094df1d(void);

u32 CM_CDECL cm_fn_00947480(u32 name)
{
    u32 enabled = D(0x00de462cu);
    u32 table = D(0x00de2fb4u);
    u32 length, item;
    if (!enabled) return 0;
    if (!table) {
        if (!D(0x00de2fbcu)) return 0;
        if (cm_fn_0094df1d()) return 0;
        table = D(0x00de2fb4u);
        if (!table) return 0;
    }
    if (!name) return 0;
    length = cm_fn_009488e0(name);
    for (;;) {
        item = D(table);
        if (!item) return 0;
        if (cm_fn_009488e0(item) > length) {
            item = D(table);
            if (B(item + length) == '=' && !cm_fn_0094dede(item, name, length)) {
                item = D(table);
                return item + length + 1u;
            }
        }
        table += 4;
    }
}
