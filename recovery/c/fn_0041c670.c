/* Preferred-image VA 0041c670, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007fee80(u32 self);
extern void CM_THISCALL cm_fn_007feeb0(u32 self, u32 text);
extern u32 CM_CDECL cm_fn_0041cdd0(u32 query, u32 alternate);
extern u32 CM_CDECL cm_fn_0041c9d0(u32 query, u32 alternate);
extern u32 CM_CDECL cm_fn_0090bde0(u32 first, u32 second);
extern void CM_CDECL cm_fn_0041bfb0(void);

u32 CM_CDECL cm_fn_0041c670(u32 query, u32 alternate, u32 flag)
{
    u32 text, pointer;
    pointer = cm_fn_007fee80(0x00b74450u);
    if (B(pointer) != 0x5cu) {
        pointer = cm_fn_007fee80(0x00b74450u);
        if (B(pointer) != 0)
            return 0;
    }
    if (D(0x00989858u) == 0) {
        cm_fn_007feeb0(0x00b74450u, 0x00989adcu);
        return 1;
    }
    if (query != 0) {
        text = cm_fn_0041cdd0(query, alternate);
        if (text != 0)
            goto install;
        if (flag != 0)
            return 0;
        pointer = D(query + 0x38u);
        if (pointer != 0) {
            pointer = D(pointer + 0x1fu);
            if (pointer != 0) {
                text = cm_fn_0041c9d0(pointer, alternate);
                if (text != 0)
                    goto install;
            }
        }
    } else if (flag != 0) {
        return 0;
    }
    text = cm_fn_0041cdd0(0, alternate);
    if (text == 0) {
        cm_fn_0041bfb0();
        return 1;
    }
install:
    text = cm_fn_0090bde0(0x009899e0u, text);
    cm_fn_007feeb0(0x00b74450u, text);
    return 1;
}
