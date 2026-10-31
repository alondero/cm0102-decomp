#include "cmre.h"

/* Thiscall with two stack arguments (ret 8). ECX is the object `obj`.
   param_2 is an output record (u8 *): bytes +0 (band code), +1 (state),
   +2 (callee result), +3 (selector copied from the table). param_3 is a pointer
   to a record whose fields are read by offset (+0x18, +0x1c, +0x2a); it may be
   zero, in which case the listing reads address 0x1c and 0x2a-based values
   come from the global 0x00ae2c90 instead.
   Object fields by offset: +0x00 u32 table base, +0x04 u32 second base,
   +0x08 s32 table row count (110-byte rows).
   Table row fields by offset from the row pointer: +0x1e/+0x31/+0x44/+0x57 s16
   thresholds, +0x20..+0x30 s8 thresholds indexed by the band (19-byte stride).
   The 0x0a-based selector and the 0x69/0x53/0x1f/0x34/0x38/0x7a fields follow the
   listing exactly; the loads are volatile so order and faults match. */

#define RD32(addr) (*(volatile u32 *)(addr))
#define RD16(addr) (*(volatile u16 *)(addr))
#define RD8(addr)  (*(volatile u8 *)(addr))
#define WR8(addr, v) (*(volatile u8 *)(addr) = (u8)(v))

extern s32 CM_THISCALL cm_fn_009232c0(u32 self, s8 param_1, s32 param_2);

void CM_THISCALL cm_fn_00923010(u32 obj, u32 param_2, u32 param_3)
{
    u32 t;
    u32 a;
    u32 r;
    u32 row;
    u32 recp;
    u32 band;
    u32 c0;
    u32 obj1;
    u32 t2;
    s32 k;
    s32 ret;
    s8 char_arg;
    s8 c1;
    s8 c2;
    s8 c3;
    s16 sv;
    s8 s3;
    u8 al2;

    if (RD32(obj) == 0) {
        return;
    }
    if (RD32(obj + 8) == 0) {
        return;
    }

    t = 0;
    if (param_3 == 0) {
        goto l_92304c;
    }
    a = RD32(param_3 + 0x18);
    if (a == 0) {
        goto l_92304c;
    }
    t = RD32(a + 0x38);
    if (t != 0) {
        goto l_923092;
    }

l_92304c:
    a = RD32(param_3 + 0x1c);
    if (a == 0) {
        goto l_92307d;
    }
    r = RD32(a + 0x69);
    if (r != 0) {
        r = RD32(r + 0x38);
        if (r != 0) {
            t = r;
            goto l_923075;
        }
    }
    r = RD32(a + 0x53);
    if (r == 0) {
        goto l_92307d;
    }
    r = RD32(r + 0x7a);
    if (r == 0) {
        goto l_92307d;
    }
    t = r;

l_923075:
    if (t != 0) {
        goto l_923092;
    }

l_92307d:
    WR8(param_2 + 3, 0);

l_923085:
    row = RD32(obj);

l_923088:
    if (param_3 == 0) {
        goto l_9230ee;
    }
    sv = (s16)RD16(param_3 + 0x2a);
    goto l_9230f3;

l_9230ee:
    sv = (s16)CM_GLOBAL(volatile u32, 0x00ae2c90);

l_9230f3:
    if (sv >= (s16)RD16(row + 0x1e)) {
        goto l_9230fd;
    }
    band = 3;
    goto l_92311b;

l_9230fd:
    if (sv >= (s16)RD16(row + 0x31)) {
        goto l_923107;
    }
    band = 0;
    goto l_92311b;

l_923107:
    if (sv >= (s16)RD16(row + 0x44)) {
        goto l_923111;
    }
    band = 1;
    goto l_92311b;

l_923111:
    band = 2u + (u32)(sv >= (s16)RD16(row + 0x57));

l_92311b:
    if (t != 0) {
        goto l_92312c;
    }
    WR8(param_2 + 0, 0);
    WR8(param_2 + 1, 0);
    char_arg = 2;
    goto l_923239;

l_92312c:
    recp = row + band * 19u;
    c0 = RD32(t);
    obj1 = RD32(obj + 4);
    c2 = (s8)RD8(obj1 + c0 * 4u + 2u);
    if (c2 >= (s8)RD8(recp + 0x20)) {
        goto l_923165;
    }
    WR8(param_2 + 0, 0);

l_92314b:
    c0 = RD32(t);
    obj1 = RD32(obj + 4);
    c1 = (s8)RD8(obj1 + c0 * 4u + 1u);
    if (c1 >= (s8)RD8(recp + 0x25)) {
        goto l_923191;
    }

l_92315f:
    WR8(param_2 + 1, 0);
    goto l_9231a2;

l_923165:
    if (c2 >= (s8)RD8(recp + 0x21)) {
        goto l_92316f;
    }
    WR8(param_2 + 0, 1);
    goto l_92314b;

l_92316f:
    if (c2 >= (s8)RD8(recp + 0x22)) {
        goto l_923179;
    }
    WR8(param_2 + 0, 2);
    goto l_92314b;

l_923179:
    if (c2 >= (s8)RD8(recp + 0x23)) {
        goto l_923183;
    }
    WR8(param_2 + 0, 3);
    goto l_92314b;

l_923183:
    WR8(param_2 + 0, ((((u32)(c2 >= (s8)RD8(recp + 0x24))) - 1u) & 4u));
    goto l_92314b;

l_923191:
    if (c1 >= (s8)RD8(recp + 0x26)) {
        goto l_9231bf;
    }
    WR8(param_2 + 1, 1);
    goto l_9231a2;

l_9231bf:
    if (c1 >= (s8)RD8(recp + 0x27)) {
        goto l_9231ca;
    }
    WR8(param_2 + 1, 2);
    goto l_9231a2;

l_9231ca:
    if (c1 >= (s8)RD8(recp + 0x28)) {
        goto l_9231d5;
    }
    WR8(param_2 + 1, 3);
    goto l_9231a2;

l_9231d5:
    if (c1 >= (s8)RD8(recp + 0x29)) {
        goto l_92315f;
    }
    WR8(param_2 + 1, 5);

l_9231a2:
    t2 = RD32(t);
    obj1 = RD32(obj + 4);
    c3 = (s8)RD8(obj1 + t2 * 4u + 2u);
    if (c3 >= (s8)RD8(recp + 0x2a)) {
        goto l_9231e0;
    }
    char_arg = 1;
    goto l_923239;

l_9231e0:
    if (c3 >= (s8)RD8(recp + 0x2b)) {
        goto l_9231ee;
    }
    char_arg = 0;
    goto l_923239;

l_9231ee:
    if (c3 >= (s8)RD8(recp + 0x2c)) {
        goto l_9231fc;
    }
    char_arg = 2;
    goto l_923239;

l_9231fc:
    if (c3 >= (s8)RD8(recp + 0x2d)) {
        goto l_923208;
    }
    char_arg = 3;
    goto l_923239;

l_923208:
    if (c3 >= (s8)RD8(recp + 0x2e)) {
        goto l_923216;
    }
    char_arg = 4;
    goto l_923239;

l_923216:
    if (c3 >= (s8)RD8(recp + 0x2f)) {
        goto l_923224;
    }
    char_arg = 5;
    goto l_923239;

l_923224:
    if (c3 >= (s8)RD8(recp + 0x30)) {
        goto l_923232;
    }
    char_arg = 6;
    goto l_923239;

l_923232:
    char_arg = 3;

l_923239:
    ret = cm_fn_009232c0(obj, char_arg, (s32)param_3);
    WR8(param_2 + 2, (u8)ret);
    if ((s8)(u8)ret >= 0) {
        return;
    }
    al2 = RD8(param_2 + 1);
    if (al2 == 1) {
        WR8(param_2 + 1, 6);
        return;
    }
    if (al2 == 2) {
        s3 = (s8)RD8(param_2 + 3);
        if (s3 == 0) {
            WR8(param_2 + 1, 6);
            return;
        }
        if (s3 == 1) {
            WR8(param_2 + 1, 7);
            return;
        }
        WR8(param_2 + 1, 8);
        return;
    }
    if (al2 == 3) {
        s3 = (s8)RD8(param_2 + 3);
        if (s3 == 0) {
            WR8(param_2 + 1, 6);
            return;
        }
        if (s3 == 1) {
            WR8(param_2 + 1, 7);
            return;
        }
        WR8(param_2 + 1, 8);
        return;
    }
    if (al2 == 5) {
        s3 = (s8)RD8(param_2 + 3);
        if (s3 == 0) {
            WR8(param_2 + 1, 8);
            return;
        }
        if (s3 == 1) {
            WR8(param_2 + 1, 7);
            return;
        }
        WR8(param_2 + 1, 9);
        return;
    }
    return;

l_923092:
    c0 = RD32(t);
    obj1 = RD32(obj + 4);
    WR8(param_2 + 3, RD8(obj1 + c0 * 4u + 3u));
    k = (s32)RD32(t + 0x34);
    if (k < 0) {
        goto l_9230c3;
    }
    if (k >= (s32)RD32(obj + 8)) {
        goto l_9230c3;
    }
    row = RD32(obj) + (u32)k * 110u;
    goto l_923088;

l_9230c3:
    r = RD32(t + 0x1f);
    if (r == 0) {
        goto l_923085;
    }
    r = RD32(r + 0x7a);
    if (r == 0) {
        goto l_923085;
    }
    k = (s32)RD32(r + 0x34);
    if (k < 0) {
        goto l_923085;
    }
    if (k >= (s32)RD32(obj + 8)) {
        goto l_923085;
    }
    row = RD32(obj) + (u32)k * 110u;
    goto l_923088;
}
