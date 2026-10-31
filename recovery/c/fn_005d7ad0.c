/* Preferred-image VA 0x005D7AD0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the destination record) with one stack argument
   (source record pointer), ret 4; returns the destination record in EAX.
   Stores: dword +0x00 = the source pointer argument itself; dwords +0x04 and
   +0x08 copied from the source; byte +0x0c and word +0x0d copied; then
   memcpy(+0x0f, source+0x0f, 0x103); byte +0x112 = 0; memcpy(+0x113, 0xff
   bytes); byte +0x212 = 0; dwords +0x213 and +0x217 copied; memcpy(+0x21b,
   0xff bytes); byte +0x31a = 0. The three memcpy calls are cdecl with
   (destination, source, count); the caller's single `add esp, 0x24` clears
   all nine pushed words. Field meanings remain unknown. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

u8 *CM_THISCALL cm_fn_005d7ad0(u8 *record, u8 *source)
{
    *(u32 *)(record + 0x00) = (u32)source;
    *(u32 *)(record + 0x04) = *(u32 *)(source + 0x04);
    *(u32 *)(record + 0x08) = *(u32 *)(source + 0x08);
    *(u8 *)(record + 0x0c) = *(u8 *)(source + 0x0c);
    *(u16 *)(record + 0x0d) = *(u16 *)(source + 0x0d);
    cm_fn_009466d0((u32)(record + 0x0f), (u32)(source + 0x0f), 0x103u);
    *(u8 *)(record + 0x112) = 0;
    cm_fn_009466d0((u32)(record + 0x113), (u32)(source + 0x113), 0xffu);
    *(u8 *)(record + 0x212) = 0;
    *(u32 *)(record + 0x213) = *(u32 *)(source + 0x213);
    *(u32 *)(record + 0x217) = *(u32 *)(source + 0x217);
    cm_fn_009466d0((u32)(record + 0x21b), (u32)(source + 0x21b), 0xffu);
    *(u8 *)(record + 0x31a) = 0;
    return record;
}
