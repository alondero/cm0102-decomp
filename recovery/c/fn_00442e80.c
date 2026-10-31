#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32, u32);
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern u32 CM_THISCALL cm_fn_0048d200(u32, u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA00442e80/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full EAX0/1. Scratch23C, including packed record11 at LC,
   packed record21 at L24, and error buffersL3C/L13C. Original file-list
   calls retain their dynamic record-size boundary and uninitialized scratch.
   Signed iteration includes competition index -1. */
u32 CM_THISCALL cm_fn_00442e80(u32 self, volatile u32 competition_arg)
{
    u32 scratch[0x23c / 4];
    u32 l = (u32)scratch, root = competition_arg, current, p, q, base, id;
    u32 context, entry, receiver, allocation, index, offset, count, limit;
    u32 round_index, round_offset, pair, slot, nation, code, year;
    u32 first_error, second_error, line, version;
    if (root == 0) {
        first_error = l + 0x13c;
        second_error = l + 0x3c;
        line = 0xab;
        goto error;
    }
    if (B(root + 0x44) != 0xff) {
        first_error = l + 0x3c;
        second_error = l + 0x13c;
        line = 0xb3;
        goto error;
    }
    p = D(root + 4);
    receiver = D(0x00ae2c74);
    if ((u32)cm_fn_005c6710(receiver, p) != 0)
        return 0;
    p = D(root + 4);
    code = D(p);
    if (cm_fn_004b0700(code) == 0)
        goto collect;
    current = root;
    for (;;) {
        limit = (u32)(s32)(s16)W(current + 0xa9);
        nation = D(0x009cf858);
        if ((s32)limit > 7) {
            p = D(root + 4);
            if (D(p) != nation)
                goto write_current;
        }
        p = D(root + 4);
        if (D(p) != nation)
            goto next_current;
        if (B(current + 0x44) != 1)
            goto next_current;
write_current:
        cm_fn_006827d0(current, 0);
        p = D(root + 4);
        year = W(root + 0x40);
        D(l + 8) = 0;
        code = D(p);
        W(l + 0x28) = (u16)year;
        D(l + 0x24) = code;
        if ((s16)W(current + 0x3e) > 0) {
            offset = 0;
            do {
                base = D(current + 0xb1);
                context = D(self + 8);
                p = D(offset + base);
                q = offset + base;
                id = D(p);
                entry = context + id * 16u;
                B(l + 0x2a) = B(q + 4);
                base = D(current + 0xb1);
                p = offset + base + 0xd;
                D(l + 0x2b) = D(p);
                D(l + 0x2f) = D(p + 4);
                D(l + 0x33) = D(p + 8);
                W(l + 0x37) = W(p + 0xc);
                p = D(current + 4);
                code = D(p);
                nation = D(0x009cf858);
                if (code == nation)
                    B(l + 0x2f) = (u8)(B(l + 0x2f) + 0xe);
                receiver = D(self);
                cm_fn_0058e340(receiver, entry, entry + 4, l + 0x24);
                count = D(l + 8);
                offset += 0x3b;
                limit = (u32)(s32)(s16)W(current + 0x3e);
                ++count;
                D(l + 8) = count;
            } while ((s32)count < (s32)limit);
        }
next_current:
        root = competition_arg;
        index = (u32)(s32)(s8)B(current + 0x44);
        if ((s32)index >= (s32)D(root + 0x30))
            break;
        current = cm_fn_0048d200(root, index + 1);
        if (B(current + 0x43) != 2)
            break;
    }
    return 1;

collect:
    if (D(root + 0x45) == 0)
        return 0;
    count = D(0x00ae235c);
    allocation = cm_fn_00945501(count, 4);
    if (allocation == 0) {
        first_error = l + 0x3c;
        second_error = l + 0x13c;
        line = 0xfd;
        goto error;
    }
    p = D(root + 4);
    year = W(root + 0x40);
    code = D(p);
    W(l + 0x10) = (u16)year;
    D(l + 0xc) = code;
    index = D(root + 0x30);
    D(l + 8) = index;
    if ((s32)index < -1)
        goto release;
    for (;;) {
        current = cm_fn_0048d200(root, index);
        if (B(current + 0x43) == 2) {
            offset = 0;
            D(l + 4) = 0;
            if ((s16)W(current + 0x3e) > 0) {
                D(l) = 0;
                do {
                    base = D(current + 0xb1);
                    q = offset + base;
                    p = D(q);
                    id = D(p);
                    if (D(allocation + id * 4u) == 0) {
                        context = D(self + 8);
                        entry = context + id * 16u;
                        B(l + 0x12) = B(q + 0x37);
                        W(l + 0x13) = W(current + 0x3a);
                        receiver = D(self + 4);
                        W(l + 0x15) = 0xffff;
                        cm_fn_0058e340(receiver, entry + 8, entry + 0xc, l + 0xc);
                        base = D(current + 0xb1);
                        offset = D(l);
                        p = D(offset + base);
                        id = D(p);
                        D(allocation + id * 4u) = 1;
                    }
                    count = D(l + 4);
                    offset = D(l);
                    limit = (u32)(s32)(s16)W(current + 0x3e);
                    ++count;
                    offset += 0x3b;
                    D(l + 4) = count;
                    D(l) = offset;
                } while ((s32)count < (s32)limit);
            }
        } else {
            round_index = (u32)(s32)(s16)W(current + 0x3c) - 1;
            D(l + 4) = round_index;
            if ((s32)round_index >= 0) {
                round_offset = round_index * 0x68u;
                D(l + 0x18) = round_offset;
                do {
                    base = D(current + 0xa3);
                    D(l + 0x1c) = 0;
                    if ((s16)W(round_offset + base + 0x1a) > 0) {
                        D(l) = 0;
                        do {
                            base = D(current + 0xad);
                            pair = D(base + round_index * 4u);
                            offset = D(l);
                            pair += offset;
                            D(l + 0x20) = pair;
                            slot = W(pair);
                            if (slot != 0xffff) {
                                base = D(current + 0xa7);
                                q = base + (u32)(s32)(s16)slot * 6u;
                                p = D(q);
                                id = D(p);
                                if (D(allocation + id * 4u) == 0) {
                                    B(l + 0x12) = B(q + 5);
                                    W(l + 0x13) = W(current + 0x3a);
                                    base = D(current + 0xa3);
                                    context = D(self + 8);
                                    entry = context + id * 16u;
                                    W(l + 0x15) = W(round_offset + base + 7);
                                    receiver = D(self + 4);
                                    cm_fn_0058e340(receiver, entry + 8, entry + 0xc, l + 0xc);
                                    round_offset = D(l + 0x18);
                                    pair = D(l + 0x20);
                                    D(allocation + id * 4u) = 1;
                                }
                            }
                            slot = W(pair + 2);
                            if (slot != 0xffff) {
                                base = D(current + 0xa7);
                                q = base + (u32)(s32)(s16)slot * 6u;
                                p = D(q);
                                id = D(p);
                                if (D(allocation + id * 4u) == 0) {
                                    B(l + 0x12) = B(q + 5);
                                    W(l + 0x13) = W(current + 0x3a);
                                    base = D(current + 0xa3);
                                    context = D(self + 8);
                                    entry = context + id * 16u;
                                    W(l + 0x15) = W(round_offset + base + 7);
                                    receiver = D(self + 4);
                                    cm_fn_0058e340(receiver, entry + 8, entry + 0xc, l + 0xc);
                                    round_offset = D(l + 0x18);
                                    D(allocation + id * 4u) = 1;
                                }
                            }
                            offset = D(l);
                            count = D(l + 0x1c);
                            offset += 0x35;
                            ++count;
                            D(l) = offset;
                            base = D(current + 0xa3);
                            D(l + 0x1c) = count;
                            limit = (u32)(s32)(s16)W(round_offset + base + 0x1a);
                            round_index = D(l + 4);
                        } while ((s32)count < (s32)limit);
                    }
                    --round_index;
                    round_offset -= 0x68;
                    D(l + 4) = round_index;
                    D(l + 0x18) = round_offset;
                } while ((s32)round_index >= 0);
            }
        }
        index = D(l + 8) - 1;
        D(l + 8) = index;
        if ((s32)index < -1)
            break;
        root = competition_arg;
    }
release:
    cm_fn_009452ca(allocation);
    return 1;
error:
    cm_fn_00944cff(0x0098b644, 0, 0, first_error, second_error);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first_error, second_error, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
