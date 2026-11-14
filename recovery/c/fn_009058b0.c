/* Preferred-image VA 0x009058b0, image base 0x00400000, pinned 3.9.68 executable
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall, `ret 8`: receiver in ECX, two stack arguments. Adds a sign-extended
   byte to one 16-bit entry of an 18-byte-stride table at receiver+0xf, indexed by
   the signed byte at +0x1df of the record 0x0053b3d0 returns, and returns the
   entry's address in EAX. Field meanings are unknown. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);

u32 CM_THISCALL cm_fn_009058b0(u32 self, u32 nation, u32 amount)
{
    u32 record = cm_fn_0053b3d0(nation);
    s32 index = *(volatile s8 *)(record + 0x1df);
    u32 entry = self + (u32)(index * 9) * 2u + 0xfu;
    *(volatile u16 *)entry = (u16)(*(volatile u16 *)entry + (u16)(s16)(s8)amount);
    return entry;
}
