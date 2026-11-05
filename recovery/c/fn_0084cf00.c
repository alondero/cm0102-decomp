#include "cmre.h"

/* Preferred-image VA 0x0084cf00, CM 01/02 3.9.68 (0c78e7da...f86).
   Both indexed callers consume AL (the 7fcd74 forwarded slot is read only
   as BYTE at 84cb78). The indirect call sets ECX=table and EDX=&state,
   pushes output then object, and leaves stack cleanup to the target. */
u8 CM_THISCALL cm_fn_0084cf00(u32 self)
{
    volatile u32 state = self; /* original entry PUSH ECX supplies this slot */
    u32 object;
    u32 table;
    u32 index;

    if (CM_GLOBAL(volatile u32, self + 0x28u) == 0u)
        return 0;
    for (index = 0; index < 10u; ++index) {
        object = CM_GLOBAL(volatile u32, self + index * 4u);
        if (object != 0u) {
            table = CM_GLOBAL(volatile u32, object);
            ((u32 (CM_FASTCALL *)(u32, volatile u32 *, u32, volatile u32 *))
                CM_GLOBAL(volatile u32, table + 0x24u))(table, &state, object, &state);
            if ((*(volatile u8 *)&state & 5u) == 0u)
                return (u8)index;
        }
    }
    return 0xffu;
}
