/* Preferred-image VA 0049b6e0/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_THISCALL cm_fn_007ff010(u32, u32);
extern u32 CM_THISCALL cm_fn_00803320(u32, u32);
extern void CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);

/* CDECL1/fullEAX, sole caller tests EAX. The zero pushed before 7ff010
   is the eventual third stack argument of 7ff270. Keep DWORD record loads
   even where only AX is tested, and repeat manager/record loads after calls. */
u32 CM_CDECL cm_fn_0049b6e0(u32 input)
{
    s16 index = (s16)input;
    u32 value, receiver, manager, offset, wanted;
    u16 kind;
    offset = (u32)(s32)index * 396u;
    if (index != -1) {
        value = cm_fn_007ff010(0x00b74450u, 0x19);
        value = cm_fn_00803320(0x00b74450u, value);
        cm_fn_007ff270(0x00b74450u, 0x18, value, 0);
        manager = (u32)(s32)(s16)W(0x00b77c7au);
        receiver = D(0x00b74c4cu + manager * 768u);
        value = D(receiver + offset + 0xba966u);
        kind = (u16)value;
    } else {
        kind = 0;
    }
    if (kind == 0) kind = W(0x00dd6bdeu);
    if (kind != 9) return 0;
    if (index != -1) {
        manager = (u32)(s32)(s16)W(0x00b77c7au);
        receiver = D(0x00b74c4cu + manager * 768u);
        wanted = D(receiver + offset + 0xba9a6u);
    } else {
        wanted = 0;
    }
    value = cm_fn_007ff010(0x00b74450u, 0x17);
    if (wanted == value) return 0;
    if (index != -1) {
        manager = (u32)(s32)(s16)W(0x00b77c7au);
        receiver = D(0x00b74c4cu + manager * 768u);
        value = D(receiver + offset + 0xba9a6u);
    } else {
        value = 0;
    }
    cm_fn_007ff270(0x00b74450u, 0x17, value, 0);
    return 0xfffffff5u;
}
