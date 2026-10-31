#include "cmre.h"

/* Thiscall with no stack arguments. Object fields: 0x00 record table pointer,
   0x08 record count (signed). Records are 0x17 bytes; a record whose dword at
   0x13 is 0xbdf is compared with a key from 0x0054c8e0 and, when the key is
   greater, replaced by the last record (count and index drop by one). The
   frame array is the original's 0x24-byte local area: index at +0, copies of
   the key at +4 and +8, the second key copy at +0xc and +0x10, and the
   callee output buffers at +0x14 and +0x1c. */

extern s32 CM_CDECL cm_fn_0090cfc0(s32 size);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32 self, u32 out, u32 value);
extern void CM_CDECL cm_fn_00945333(u32 table, u32 count, u32 size, u32 callback);

void CM_THISCALL cm_fn_00757c70(u32 self)
{
    u8 frame[0x24];
    u32 off = 0;
    u32 buf;
    u32 count;
    u32 k;
    u32 key;
    u32 key_copy;
    u32 hi_key;
    u32 lo_key;
    u32 rec_hi;
    u32 rec_lo;
    u32 ret;
    u32 raw;
    u32 last;
    u32 n;
    u32 i;
    int take;
    u32 hi_off;

    if (*(volatile u32 *)self == 0) {
        return;
    }

    count = *(volatile u32 *)(self + 8u);
    k = 0;
    if ((s32)count > 0) {
        do {
            buf = *(volatile u32 *)self;
            if (*(volatile u32 *)(buf + off + 0x13u) == 0x0bdfu) {
                raw = (u32)cm_fn_0090cfc0(0x28);
                ret = cm_fn_0054c8e0(0x00ae2c90u, (u32)(frame + 0x14), raw + 0x28u);
                buf = *(volatile u32 *)self;
                key = *(volatile u32 *)ret;
                key_copy = *(volatile u32 *)(ret + 4u);
                *(volatile u32 *)(frame + 4) = key;
                *(volatile u32 *)(frame + 8) = key_copy;
                hi_off = 6u;
            } else {
                ret = cm_fn_0054c8e0(0x00ae2c90u, (u32)(frame + 0x1c), 0x64u);
                key_copy = *(volatile u32 *)(ret + 4u);
                key = *(volatile u32 *)ret;
                *(volatile u32 *)(frame + 0x10) = key_copy;
                buf = *(volatile u32 *)self;
                *(volatile u32 *)(frame + 0xc) = key;
                hi_off = 0xeu;
            }

            hi_key = *(volatile u16 *)(frame + hi_off) & 0xffffu;
            rec_hi = *(volatile u16 *)(buf + off + 2u);
            take = 0;
            if (hi_key > rec_hi) {
                take = 1;
            } else if (hi_key == rec_hi) {
                lo_key = key & 0xffffu;
                rec_lo = *(volatile u16 *)(buf + off);
                take = ((s16)lo_key > (s16)rec_lo);
            }

            if (take) {
                n = *(volatile u32 *)(self + 8u);
                last = buf + (n * 0x17u) - 0x17u;
                for (i = 0; i < 5u; i++) {
                    *(volatile u32 *)(buf + off + i * 4u) = *(volatile u32 *)(last + i * 4u);
                }
                *(volatile u16 *)(buf + off + 0x14u) = *(volatile u16 *)(last + 0x14u);
                *(volatile u8 *)(buf + off + 0x16u) = *(volatile u8 *)(last + 0x16u);
                k = k - 1;
                *(volatile u32 *)(self + 8u) = *(volatile u32 *)(self + 8u) - 1u;
                off = off - 0x17u;
            }

            k = k + 1;
            off = off + 0x17u;
            count = *(volatile u32 *)(self + 8u);
        } while ((s32)k < (s32)count);
    }

    count = *(volatile u32 *)(self + 8u);
    if (count != 0) {
        buf = *(volatile u32 *)self;
        cm_fn_00945333(buf, count, 0x17u, 0x00757ee0u);
    }
}
