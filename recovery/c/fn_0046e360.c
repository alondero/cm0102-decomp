/* Preferred-image VA 0046e360, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

/* ABI taken from the original instructions in the pinned 3.9.68 image named
   above. At each call site the receiver 0x00b74450 is loaded into ECX
   immediately before the call (preferred-image VAs 0x0046e370, 0x0046e37b,
   0x0046e388, 0x0046e406 and 0x0046e44b), so each callee's first declared
   parameter is that thiscall receiver.
   Boundary resolution for the held concern: the first 0x7ff010 call consumes a
   single live stack argument (0x15); the zero pushed just before it survives
   as the trailing argument of the first 0x7ff270 call rather than being an
   output pointer passed to 0x7ff010. The second 0x7ff010 call likewise takes a
   single stack argument (0x13); 0x803320 takes the 0x7ff010 result as its one
   stack argument, and both 0x7ff270 calls take three stack arguments. Results
   are opaque u32 values: no file-search, time or OUT-buffer state is invented
   or consumed beyond what the listing shows. */
extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 id);
extern u32 CM_THISCALL cm_fn_00803320(u32 self, u32 input);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 id, u32 value, u32 mode);

u32 CM_CDECL cm_fn_0046e360(u32 input)
{
    s16 selection = (s16)(u16)input;
    u32 value, active, index, table, candidate, current;
    u16 mode;

    if (selection != -1) {
        value = cm_fn_007ff010(0x00b74450u, 0x15u);
        value = cm_fn_00803320(0x00b74450u, value);
        cm_fn_007ff270(0x00b74450u, 0x14u, value, 0);
        active = (u32)(s32)(s16)W(0x00b77c7au);
        index = (u32)(s32)selection;
        table = D(0x00b74c4cu + active * 768u);
        value = D(table + index * 396u + 0xba966u);
    } else {
        value = 0;
    }
    mode = (u16)value;
    if (mode == 0)
        mode = W(0x00dd6bdeu);
    if (mode != 10)
        return 0;

    if (selection != -1) {
        active = (u32)(s32)(s16)W(0x00b77c7au);
        index = (u32)(s32)selection;
        table = D(0x00b74c4cu + active * 768u);
        candidate = D(table + index * 396u + 0xba9a6u);
    } else {
        candidate = 0;
    }
    current = cm_fn_007ff010(0x00b74450u, 0x13u);
    if (candidate == current)
        return 0;

    if (selection != -1) {
        active = (u32)(s32)(s16)W(0x00b77c7au);
        index = (u32)(s32)selection;
        table = D(0x00b74c4cu + active * 768u);
        value = D(table + index * 396u + 0xba9a6u);
    } else {
        value = 0;
    }
    cm_fn_007ff270(0x00b74450u, 0x13u, value, 0);
    return 0xfffffff5u;
}
