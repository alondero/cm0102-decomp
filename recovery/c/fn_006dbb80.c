/* Preferred-image VA 006dbb80/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_008968f0(u32, u32, u32, u32);
extern u8 CM_THISCALL cm_fn_0062c400(u32, u32);
extern u32 CM_THISCALL cm_fn_0055d270(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007757d0(u32, u32, u32, u32, u32, u32);
extern u8 CM_THISCALL cm_fn_006e0d00(u32, u32, u32, u32);

/* THIS0/RET0/u8: caller 006db9a9 copies AL. L is the address immediately
   after SUB ESP,8. Only L+3 and L+4 are initialized. Keep the DWORD read
   at L+4 and full pushed slots; do not initialize its three unwritten bytes.
   8968f0 consumes only the index BYTE. 6e0d00 consumes AL as its index and
   forwards the other high bytes only into BYTE arguments of 6eaea0.
   All callees here remain original-code boundaries. */
u8 CM_THISCALL cm_fn_006dbb80(u32 self)
{
    u32 scratch[2];
    u32 l = (u32)scratch, object, bound, tag, index, input, receiver, person;
    u32 key, result;
    u8 counter = 0;
    B(l + 3u) = 0;
    object = D(self + 0x37u);
    B(l + 4u) = 0;
    bound = (B(object + 0x4du) & 15u) + 11u;
    do {
        tag = (u32)(s32)(s8)B(self + 0x19u);
        index = D(l + 4u);
        input = D(self + 0x2fu);
        receiver = D(0x00de1d08u);
        person = cm_fn_008968f0(receiver, input, index, tag);
        if (person != 0) {
            receiver = D(0x00b63d28u);
            if (cm_fn_0062c400(receiver, person) != 2) {
                object = D(self + 0x37u);
                if (D(object) != 0xfffffffeu) {
                    key = D(object + 0x14u);
                    receiver = D(0x00ae28f0u);
                    result = cm_fn_0055d270(receiver, person, key, 1, 0);
                    if (result != 0) goto next;
                }
                input = D(self + 0x2fu);
                receiver = D(0x00b74318u);
                result = cm_fn_007757d0(receiver, person, 0x00ae2c90u, input, 0, 0);
                if (result == 0 && cm_fn_006e0d00(self, index, person, index) != 0)
                    ++B(l + 3u);
            }
        }
next:
        object = D(self + 0x37u);
        ++counter;
        bound = (B(object + 0x4du) & 15u) + 11u;
        B(l + 4u) = counter;
    } while ((s32)(s8)counter < (s32)bound);
    return B(l + 3u);
}
