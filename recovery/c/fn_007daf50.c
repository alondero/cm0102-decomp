#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern void CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32, u32, u32);
extern void CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32, u32);
extern u32 CM_THISCALL cm_fn_0055d270(u32, u32, u32, u32, u32);
extern void CM_STDCALL cm_fn_007570b0(u32, u32, u32);
extern void CM_THISCALL cm_fn_00556940(u32, u32, u32);
extern void CM_CDECL cm_fn_00547940(u32, u32, u32);

/* The sole direct caller jumps to VA006b7973 after ADD ESP,12. At
   006b7979 it overwrites full EAX before use; EDX is also overwritten.
   007570b0 consumes three stack arguments and ignores incoming ECX. */
void CM_CDECL cm_fn_007daf50(u32 first, u32 second, u32 third)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x24];
    u32 previous, object, value, context, id, limit, competition, table, date, flags, kind, result, other;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095fd70;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110((u32)scratch + 0x14);
    frame.state = 0;
    if (first == 0)
        goto done;
    object = D(first + 0x39);
    if (object == 0 || D(object + 0x57) == 0 || second == 0 || third == 0)
        goto done;
    if ((s32)D(third) < 0)
        goto done;
    value = D(third + 0x14);
    context = D(0x00ae2c74);
    if ((u32)cm_fn_005c6710(context, value) != 0)
        goto done;
    id = D(first);
    limit = D(0x00ae2364);
    if ((s32)id >= (s32)limit)
        goto done;
    competition = D(third);
    table = D(0x00adadfc);
    if (D(table + competition * 4u) == 0)
        goto done;
    table = D(0x00ae2d0c);
    if ((B(table + id * 79u + 0xb) & 0x40u) == 0)
        goto done;
    B((u32)scratch + 0x10) = 1;
    W((u32)scratch + 0x12) = 0xffff;
    B((u32)scratch + 0x11) = 0;
    date = cm_fn_0054c5b0(0x00ae2c90, (u32)scratch, 14);
    B((u32)&frame.state) = 1;
    cm_fn_00417c10((u32)scratch + 0x14, (u32)scratch + 8, date);
    flags = B((u32)scratch + 0x23);
    table = D(0x00adadfc);
    flags &= 0xfdu;
    B((u32)scratch + 0x20) = 5;
    B((u32)scratch + 0x23) = (u8)flags;
    competition = D(third);
    W((u32)scratch + 0x21) = 1;
    object = D(table + competition * 4u);
    value = D(third + 0x14);
    kind = B(object + 0x50);
    context = D(0x00ae28f0);
    B((u32)&frame.state) = 0;
    B((u32)scratch + 0x1c) = (u8)kind;
    B((u32)scratch + 0x1d) = 0xff;
    B((u32)scratch + 0x1e) = 0xff;
    B((u32)scratch + 0x1f) = 0xff;
    result = cm_fn_0055d270(context, first, value, 1, 0);
    flags = B((u32)scratch + 0x23);
    flags = result != 0 ? flags | 1u : flags & 0xfeu;
    (void)D(0x00b6b7c8);
    B((u32)scratch + 0x23) = (u8)flags;
    B((u32)scratch + 0x23) = (u8)(flags & 0xfdu);
    cm_fn_007570b0(first, second, third);
    id = D(first);
    context = D(0x00ae28f0);
    cm_fn_00556940(context, id, (u32)scratch + 0x10);
    other = D(third + 0x1c);
    if (other == second)
        other = D(third + 0x20);
    cm_fn_00547940(other, first, 1);
done:
    CM_FS(u32, 0) = frame.prev;
}
