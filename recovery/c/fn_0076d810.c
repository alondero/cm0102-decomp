#include "cmre.h"

/* Stack arguments (ret 0xc):
   arg1: object pointer; its field at 0x61 holds a pointer to a block of signed bytes.
   arg2: pointer to a signed 16-bit value, compared with 100 when arg3 is zero and with 50 otherwise.
   arg3: flag byte; only its low byte is read (test al, al).
   The original returns through al only, so the result is a u8. */
u8 CM_STDCALL cm_fn_0076d810(u32 arg1, u32 arg2, u8 arg3)
{
    u32 block;

    if (arg3 == 0) {
        if (*(volatile s16 *)arg2 < 100) {
            return 0u;
        }
    } else {
        if (*(volatile s16 *)arg2 < 50) {
            return 0u;
        }
    }

    block = *(volatile u32 *)(arg1 + 0x61);
    if (*(volatile s8 *)(block + 0x0f) >= 15) {
        return 0u;
    }
    if (*(volatile s8 *)(block + 0x10) >= 15 ||
        *(volatile s8 *)(block + 0x11) >= 15) {
        return 1u;
    }
    if (*(volatile s8 *)(block + 0x12) >= 15 ||
        *(volatile s8 *)(block + 0x13) >= 15 ||
        *(volatile s8 *)(block + 0x14) >= 15) {
        return 2u;
    }
    return 3u;
}
