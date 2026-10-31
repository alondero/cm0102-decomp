#include "cmre.h"

/* Original callees. Both pop their own stack arguments (no add esp after the call
   in the listing), so they are STDCALL. The socket and address arguments are passed
   as the dwords the listing pushes. */
extern u16 CM_STDCALL cm_fn_00944c64(u32 hostshort);
extern u32 CM_STDCALL cm_fn_00944c5e(u32 s, u32 buf, s32 len, s32 flags, u32 to, s32 tolen);

/* Little-endian byte stores into the 16-byte address scratch at [esp+0x14].
   Offset 0: family word, offset 2: port word, offsets 4, 8 and 0xc: dwords. */
static void store_u16(u8 *p, u32 value)
{
    p[0] = (u8)value;
    p[1] = (u8)(value >> 8);
}

static void store_u32(u8 *p, u32 value)
{
    p[0] = (u8)value;
    p[1] = (u8)(value >> 8);
    p[2] = (u8)(value >> 16);
    p[3] = (u8)(value >> 24);
}

int CM_THISCALL
cm_fn_008aec90(u32 self, u32 param_2, s32 param_3, u32 param_4, u32 param_5,
               s32 param_6, u32 param_7)
{
    u8 address_buf[0x10];
    u32 sum;
    u32 slot;
    u32 record;
    u32 counter;
    u32 word0;
    u32 word4;
    u32 word8;
    u32 wordc;
    u16 port_net;
    u32 sent;

    sum = 0u;
    if (param_2 == 0u) {
        return 0;
    }
    if (param_3 <= 0) {
        return 0;
    }
    if (param_5 != 0u) {
        s32 index = (s32)(s16)param_7;
        sent = cm_fn_00944c5e(*(volatile u32 *)(self + 0x288u + (u32)index * 4u),
                              param_2, param_3, 0, param_5, param_6);
        return (int)sent;
    }

    slot = self + 0x288u;
    record = self + 0x18u;
    counter = 0x14u;
    do {
        if (*(volatile u32 *)slot != 0xffffffffu) {
            word0 = *(volatile u32 *)(record + 0u);
            store_u32(address_buf + 0u, word0);
            store_u16(address_buf + 0u, 2u);
            word4 = *(volatile u32 *)(record + 4u);
            store_u32(address_buf + 4u, word4);
            word8 = *(volatile u32 *)(record + 8u);
            store_u32(address_buf + 8u, word8);
            wordc = *(volatile u32 *)(record + 0xcu);
            store_u32(address_buf + 0xcu, wordc);
            port_net = cm_fn_00944c64(param_4);
            store_u16(address_buf + 2u, (u32)port_net);
            sent = cm_fn_00944c5e(*(volatile u32 *)slot, param_2, param_3, 0,
                                  (u32)(unsigned long)address_buf, 0x10);
            sum = sum + sent;
        }
        slot = slot + 4u;
        record = record + 0x20u;
        counter = counter - 1u;
    } while (counter != 0u);

    return (s32)(s16)(u16)sum;
}
