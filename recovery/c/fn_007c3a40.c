#include "cmre.h"
/* Preferred-image VA 0x007c3a40, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern u32 CM_THISCALL cm_fn_007c3e40(u32, u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_STDCALL cm_fn_007c3a40(u32 player, u32 club)
{
    u8 scratch[0x204];
    u8 *first, *second;
    u32 id, total, line, receiver, data, count, offset, pointer, sum, result;
    char *version;
    if (player == 0) {
        first = scratch + 0x104; second = scratch + 4; line = 0xabd;
        goto error;
    }
    if (club == 0) {
        first = scratch + 4; second = scratch + 0x104; line = 0xac5;
        goto error;
    }
    id = D(player);
    total = D(0x00ae2364);
    if ((s32)id >= (s32)total)
        return player & 0xffff0000u;
    if (D(player + 0x61u) == 0) {
        first = scratch + 4; second = scratch + 0x104; line = 0xad7;
        goto error;
    }
    receiver = D(0x00b74350);
    data = cm_fn_007c3e40(receiver, player, (u32)scratch);
    sum = 0;
    if (data != 0) {
        count = D((u32)scratch);
        pointer = data + 0xd9u;
        for (offset = 0; (s32)offset < (s32)count; offset += 6u, pointer += 0x102u) {
            id = D(pointer);
            if ((s32)id >= 0 && id == D(club)) {
                id = *(volatile u8 *)(pointer + 4u);
                sum += id;
            }
        }
    }
    result = (u32)cm_fn_009452ca(data);
    /* MOV AX,BX preserves the helper's upper EAX half. Intermediate high
       halves in MOVZX CX contribute multiples of 65536 and vanish in BX. */
    return (result & 0xffff0000u) | (sum & 0xffffu);
error:
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    result = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result & 0xffff0000u;
}
