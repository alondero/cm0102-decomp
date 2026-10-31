#include "cmre.h"

extern u32 CM_CDECL cm_fn_00940160(u32 self);
extern s32 CM_CDECL cm_fn_009446e0(s32 a, s32 b);
extern u32 CM_CDECL cm_fn_00944840(s32 a, s32 b);

/* Memory accessors by absolute address. Every access is volatile so field
   reads, pool-cursor bookkeeping and list-link stores keep the listing order.
   Object fields are named by offset only:
     self+0x044  gate word (zero selects the plain integer-division path)
     self+0x158  base of the first bucket-slot array
     self+0x0c8  base of the second bucket-slot array
     self+0x168  pool cursor (next free node; advanced by 8)
     self+0x16c  pool limit (cursor >= limit calls 0x00940160)
     self+0x2203 byte gate; zero skips the first block
   Node layout: +0 signed key, +4 next node pointer. */
#define MEM8(addr) (*(volatile u8 *)(addr))
#define MEM32(addr) (*(volatile u32 *)(addr))
#define MEMS32(addr) (*(volatile s32 *)(addr))

/* Insert `node` (key `key`) into the list whose first node is `head`, linked
   from bucket slot `slot`. Mirrors 0x0094040c..0x00940411 and the walk before it:
   front insert when head is empty or its key is greater than `key`; otherwise
   advance while the next key is <= `key` and link after the last such node. */
#define INSERT_NODE(slot, node, key, head)                                  \
    do {                                                                    \
        u32 ins_pred_ = (head);                                             \
        u32 ins_next_;                                                      \
        if (ins_pred_ == 0 || MEMS32(ins_pred_) > (s32)(key)) {             \
            MEM32((node) + 4) = (head);                                     \
            MEM32(slot) = (node);                                           \
        } else {                                                            \
            ins_next_ = MEM32(ins_pred_ + 4);                               \
            while (ins_next_ != 0 && MEMS32(ins_next_) <= (s32)(key)) {     \
                ins_pred_ = ins_next_;                                      \
                ins_next_ = MEM32(ins_pred_ + 4);                           \
            }                                                               \
            MEM32((node) + 4) = ins_next_;                                  \
            MEM32(ins_pred_ + 4) = (node);                                  \
        }                                                                   \
    } while (0)

/* Stack arguments arrive as arg2..arg5 and are reused as locals; each one is
   stored as (value | 1). The listing keeps them at frame offsets 0x38..0x44. */
void CM_CDECL cm_fn_00940290(u32 self, u32 arg2, u32 arg3, u32 arg4, u32 arg5)
{
    u32 a = arg2 | 1u;
    u32 c = arg3 | 1u;
    u32 d = arg4 | 1u;
    u32 f = arg5 | 1u;

    /* First block, gated by the byte at self+0x2203 (0x009402b2). */
    if (MEM8(self + 0x2203) != 0) {
        u32 lo;     /* smaller of a and d (signed) */
        u32 hi;     /* larger of a and d */
        u32 x;      /* partner value kept in the x slot */
        u32 y;     /* partner value kept in ebp */
        u32 flag;   /* 1 when a < d, else 0 (frame byte at 0x1c) */
        u32 ec;     /* first aligned boundary above lo, plus 0x20 */
        u32 diff;   /* hi - lo */
        u32 w;      /* y - x */
        u32 slot;   /* current bucket slot address */
        u32 cnt;    /* running counter stepped by 0x40 */

        if ((s32)a < (s32)d) {
            lo = a; hi = d; x = c; y = f; flag = 1u;
        } else {
            lo = d; hi = a; x = f; y = c; flag = 0u;
        }

        ec = ((lo + 0x1fu) & 0xffffffc0u) + 0x20u;
        if ((s32)ec <= (s32)hi) {
            slot = MEM32(self + 0x158) + (u32)((s32)ec >> 6) * 4u;
            diff = hi - lo;

            if (diff == 0) {
                /* 0x009404f7: single node, no walk arithmetic. */
                u32 cur = MEM32(self + 0x168);
                u32 head = MEM32(slot);
                u32 cap = MEM32(self + 0x16c);
                u32 key = (x + x) | flag;
                u32 node;
                if (cur >= cap) {
                    node = cm_fn_00940160(self);
                } else {
                    node = cur;
                }
                MEM32(self + 0x168) = MEM32(self + 0x168) + 8u;
                MEM32(node) = key;
                INSERT_NODE(slot, node, key, head);
            } else {
                u32 fl44;
                w = y - x;
                fl44 = MEM32(self + 0x44);

                if (fl44 == 0 ||
                    ((s32)diff < 0xb504 && (s32)w < 0xb504 && (s32)w > -0xb504)) {
                    /* 0x0094042d: integer-division path. acc steps by w<<6 per
                       node; the key is ((acc / diff + x) << 1) | flag. */
                    u32 acc = (ec - lo) * w;
                    u32 step = w << 6;
                    cnt = ec;
                    do {
                        u32 head = MEM32(slot);
                        u32 q = (u32)((s32)acc / (s32)diff);
                        u32 key = ((q + x) << 1) | flag;
                        u32 cap = MEM32(self + 0x16c);
                        u32 cur = MEM32(self + 0x168);
                        u32 node;
                        if (cur >= cap) {
                            node = cm_fn_00940160(self);
                        } else {
                            node = cur;
                        }
                        MEM32(self + 0x168) = MEM32(self + 0x168) + 8u;
                        MEM32(node) = key;
                        INSERT_NODE(slot, node, key, head);
                        acc += step;
                        slot += 4u;
                        cnt += 0x40u;
                    } while ((s32)cnt <= (s32)hi);
                } else {
                    /* 0x0094036a: call path through 0x00944840 and 0x009446e0. */
                    cnt = ec;
                    do {
                        u32 r1 = cm_fn_00944840((s32)(cnt - lo), (s32)diff);
                        s32 r2 = cm_fn_009446e0((s32)w, (s32)r1);
                        u32 t = (u32)r2 + x;
                        u32 cap = MEM32(self + 0x16c);
                        u32 head = MEM32(slot);
                        u32 key = (t + t) | flag;
                        u32 cur = MEM32(self + 0x168);
                        u32 node;
                        if (cur >= cap) {
                            node = cm_fn_00940160(self);
                        } else {
                            node = cur;
                        }
                        MEM32(self + 0x168) = MEM32(self + 0x168) + 8u;
                        MEM32(node) = key;
                        INSERT_NODE(slot, node, key, head);
                        slot += 4u;
                        cnt += 0x40u;
                    } while ((s32)cnt <= (s32)hi);
                }
            }
        }
    }

    /* Second block, 0x00940560. Its operands come from arg3 and arg5 (and the
       partner arg2/arg4 values), with the same shape as the first block. */
    {
        u32 lo2;
        u32 hi2;
        u32 x2;
        u32 y2;
        u32 flag2;
        u32 ec2;
        u32 diff2;
        u32 w2;
        u32 slot2;
        u32 cnt2;
        u32 acc2;
        u32 step2;

        if ((s32)c < (s32)f) {
            lo2 = c; hi2 = f; x2 = a; y2 = d; flag2 = 1u;
        } else {
            lo2 = f; hi2 = c; x2 = d; y2 = a; flag2 = 0u;
        }

        ec2 = ((lo2 + 0x1fu) & 0xffffffc0u) + 0x20u;
        if ((s32)ec2 > (s32)hi2) {
            return;
        }
        slot2 = MEM32(self + 0xc8) + (u32)((s32)ec2 >> 6) * 4u;
        diff2 = hi2 - lo2;

        if (diff2 == 0) {
            /* 0x009407a6: single node into the second array. */
            u32 cur = MEM32(self + 0x168);
            u32 head = MEM32(slot2);
            u32 cap = MEM32(self + 0x16c);
            u32 key = (x2 + x2) | flag2;
            u32 node;
            if (cur >= cap) {
                node = cm_fn_00940160(self);
            } else {
                node = cur;
            }
            MEM32(self + 0x168) = MEM32(self + 0x168) + 8u;
            MEM32(node) = key;
            INSERT_NODE(slot2, node, key, head);
            return;
        }

        w2 = y2 - x2;
        if (MEM32(self + 0x44) == 0 ||
            ((s32)diff2 < 0xb504 && (s32)w2 < 0xb504 && (s32)w2 > -0xb504)) {
            /* 0x009406d6: integer-division path. */
            acc2 = (ec2 - lo2) * w2;
            step2 = w2 << 6;
            cnt2 = ec2;
            do {
                u32 q = (u32)((s32)acc2 / (s32)diff2);
                u32 cap = MEM32(self + 0x16c);
                u32 head = MEM32(slot2);
                u32 cur = MEM32(self + 0x168);
                u32 key = ((q + x2) << 1) | flag2;
                u32 node;
                if (cur >= cap) {
                    node = cm_fn_00940160(self);
                } else {
                    node = cur;
                }
                MEM32(self + 0x168) = MEM32(self + 0x168) + 8u;
                MEM32(node) = key;
                INSERT_NODE(slot2, node, key, head);
                slot2 += 4u;
                acc2 += step2;
                cnt2 += 0x40u;
            } while ((s32)cnt2 <= (s32)hi2);
            return;
        }

        /* 0x0094060e: call path through 0x00944840 and 0x009446e0. */
        cnt2 = ec2;
        do {
            u32 r1 = cm_fn_00944840((s32)(cnt2 - lo2), (s32)diff2);
            s32 r2 = cm_fn_009446e0((s32)w2, (s32)r1);
            u32 t = (u32)r2 + x2;
            u32 cap = MEM32(self + 0x16c);
            u32 head = MEM32(slot2);
            u32 key = (t + t) | flag2;
            u32 cur = MEM32(self + 0x168);
            u32 node;
            if (cur >= cap) {
                node = cm_fn_00940160(self);
            } else {
                node = cur;
            }
            MEM32(self + 0x168) = MEM32(self + 0x168) + 8u;
            MEM32(node) = key;
            INSERT_NODE(slot2, node, key, head);
            slot2 += 4u;
            cnt2 += 0x40u;
        } while ((s32)cnt2 <= (s32)hi2);
    }
}
