#include "cmre.h"
/* Preferred-image VA 0x007c1950, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u32 CM_THISCALL cm_fn_007c1950(u32 self, u32 player)
{
    u8 scratch[0x200];
    u32 index, base, address, sum, nibble, result;
    u8 byte;
    char *version;
    if (player == 0) {
        cm_fn_00944cff(0x00a78310, 0, 0, scratch + 0x100, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x100, scratch, 0x475);
        result = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return result & 0xffffff00u;
    }
    index = *(volatile u32 *)player;
    base = *(volatile u32 *)(self + 0x30u);
    address = base + index * 39u;
    byte = *(volatile u8 *)(address + 0x24u);
    nibble = byte & 15u;
    sum = nibble != 0 ? nibble * 4u : 27u;
    nibble = byte >> 4;
    sum += nibble != 0 ? nibble * 4u : 27u;
    byte = *(volatile u8 *)(address + 0x25u);
    nibble = byte & 15u;
    sum += nibble != 0 ? nibble * 4u : 27u;
    nibble = byte >> 4;
    sum += nibble != 0 ? nibble * 4u : 27u;
    nibble = *(volatile u8 *)(address + 0x26u) & 15u;
    sum += nibble != 0 ? nibble * 4u : 27u;
    /* MOVZX into CX/DX/SI/AX leaves high halves intact, but those bits
       cannot affect CX, which the original sign-extends before dividing. */
    return (u32)((s32)(s16)sum / 4);
}
