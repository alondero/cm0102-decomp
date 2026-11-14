/* Plain C translation of preferred-image VA 0x005d6aa0 (CM0102 3.9.68,
   image base 0x00400000, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).

   The routine picks a random starting person record, then walks `count`
   records of the person table applying the original's filters and score
   byte.  Every width, sign, reload and call order below follows the
   listing; the record/club strides (0x6e and 581) come from the original
   lea/imul sequences.

   Entry: the listing pushes the incoming ECX into a scratch slot that the
   loop counter later overwrites, so the incoming ECX value is never used;
   the routine takes no stack argument and returns with a plain ret. */

#include "cmre.h"

#define D(a)  CM_GLOBAL(volatile u32, (u32)(a))
#define S(a)  CM_GLOBAL(volatile s32, (u32)(a))
#define B(a)  CM_GLOBAL(volatile u8, (u32)(a))
#define R(a)  (*(volatile u32 *)(u32)(a))
#define RB(a) (*(volatile u8 *)(u32)(a))

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u8  CM_THISCALL cm_fn_00418f60(u8 *self, u32 *index_pointer, u32 second_arg);
extern u32 CM_THISCALL cm_fn_008b39f0(u32 self, u32 argument);
extern u8  CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern u32 CM_CDECL   cm_fn_00547810(u32 target, u32 object);
extern u32 CM_CDECL   cm_fn_00547840(u32 target, u32 object);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 low, u32 high);
extern void CM_THISCALL cm_fn_0041a360(int param_1, int *param_2);
/* Every call site pushes only the bound and adds 4 after the call.  The
   original's EAX/EDX at those sites are residues of the preceding code; its
   EAX result does not depend on them, and EDX is not read by this routine. */
extern u32 CM_CDECL   cm_fn_0090cfc0(u32 bound);

void CM_THISCALL cm_fn_005d6aa0(u32 self)
{
    s32 value, count, age;
    u32 index, rec, base, bound, relation, club, holder, person, target;
    u8 score, tag, state44, state2f;

    (void)self;

    /* 005d6aa1..005d6abe: signed division of the global by 250, then the
       bound for the random start index (value - count - 1). */
    value = S(0x00ae2364u);
    count = value / 250;
    bound = (u32)value - (u32)count - 1u;
    /* 005d6abf..005d6ac0: the original pushes ECX, the bound. */
    index = (u32)cm_fn_0090cfc0(bound);
    /* 005d6ad3..005d6ad8: person table base plus 110 * index. */
    base = D(0x00ae23bcu);
    rec = base + index * 110u;
    if (count <= 0)
        return;                                  /* 005d6adb: jle */

    /* The original's loop tail (005d6cf9) advances the record and steps the
       counter before the branch, so the early exits above re-enter here. */
    for (; count != 0; rec += 0x6eu, count--) {
        /* 005d6ae6..005d6aeb */
        person = R(rec + 0x61u);
        if (person == 0u)
            continue;
        /* 005d6af1..005d6af5 */
        if (B(rec + 0x3du) != 0x0bu)
            continue;

        /* 005d6afb..005d6b0e */
        relation = cm_fn_004cf5a0(D(0x00ae1050u), rec);
        D(0x00ae1080u) = relation;               /* 4-byte store, as in the listing */
        if (relation == 0u)
            continue;

        /* 005d6b14..005d6b2c: club record of the relation. */
        holder = R(relation + 4u);
        base = D(0x00ae23b4u);
        club = base + holder + (holder * 145u) * 4u;
        if (club == 0u)
            continue;

        /* 005d6b32..005d6b42 */
        if (cm_fn_00418f60((u8 *)(u32)D(0x00ad9c6cu), (u32 *)rec, 0u) != 0u)
            continue;

        /* 005d6b48..005d6b61 */
        holder = cm_fn_008b39f0(D(0x00de1f44u), rec);
        if (holder == 0u)
            continue;
        if (B(holder + 0x15u) == 0u)
            continue;

        /* 005d6b67..005d6b75 */
        if (cm_fn_0062c400(D(0x00b63d28u), rec) == 2u)
            continue;

        /* 005d6b7b..005d6b9b: the score byte, built in BL. */
        person = R(rec + 0x61u);
        tag = B(rec + 0x5bu);
        state44 = RB(person + 0x44u);
        state2f = RB(person + 0x2fu);
        score = state2f;
        score = (u8)(score + B(rec + 0x18u));
        score = (u8)(score + tag);
        score = (u8)(score + state44);
        if (tag == 1u)
            score = (u8)(score + 0xfbu);
        if (state44 == 1u)
            score = (u8)(score + 0xfbu);

        /* 005d6b9e..005d6bc9: two club membership probes, each reloading
           the club's target row. */
        target = R(club + 0xcfu);
        if (cm_fn_00547840(target, rec) != 0u)
            score = (u8)(score + 0xfbu);
        target = R(club + 0xcfu);
        if (cm_fn_00547810(target, rec) != 0u)
            score = (u8)(score + 2u);

        /* 005d6bcc..005d6c4f: the age probe. */
        tag = B(rec + 0x5bu);
        if ((s32)(s8)tag < 8) {
            person = R(rec + 0x61u);
            state2f = RB(person + 0x2fu);
            if ((s32)(s8)state2f < 8 && (s32)(s8)score < 0x2b) {
                u8 s = tag;                       /* 005d6be4: mov bl, al */
                u32 high, low;
                high = R(rec + 0x42u);            /* 005d6be8 */
                low = R(rec + 0x3eu);             /* 005d6bf1 */
                age = cm_fn_0054cdb0(0x00ae2c90u, low, high);
                if (age < 30) {
                    s = (u8)(s + 0x64u);
                } else {
                    high = R(rec + 0x42u);
                    low = R(rec + 0x3eu);
                    age = cm_fn_0054cdb0(0x00ae2c90u, low, high);
                    if (age < 90) {
                        s = (u8)(s + 0x32u);
                    } else {
                        high = R(rec + 0x42u);
                        low = R(rec + 0x3eu);
                        age = cm_fn_0054cdb0(0x00ae2c90u, low, high);
                        if (age < 180)
                            s = (u8)(s + 0x19u);
                    }
                }
                /* 005d6c3b..005d6c49: the bound is the signed score byte. */
                if (cm_fn_0090cfc0((u32)(s32)(s8)s) == 0u) {
                    cm_fn_0041a360((int)D(0x00ad9c6cu), (int *)rec);
                    continue;
                }
            }
        }

        /* 005d6c4f..005d6ccb: the second filter, on the relation flags. */
        if ((s32)(s8)B(rec + 0x5bu) < 10) {
            relation = cm_fn_004cf5a0(D(0x00ae1050u), rec);
            D(0x00ae1080u) = relation;
            if (relation != 0u) {
                holder = R(relation + 4u);
                base = D(0x00ae23b4u);
                club = base + holder + (holder * 145u) * 4u;
            } else {
                club = 0u;
            }
            holder = cm_fn_004cf5f0(D(0x00ae1050u), rec, club);
            if (holder != 0u) {
                u32 flags = R(holder + 0x45u);
                if ((flags & 0x8000u) != 0u) {     /* 005d6c9e: test ah,0x80 */
                    u32 b = (u32)(s32)(s8)B(rec + 0x5bu);
                    if (cm_fn_0090cfc0(b) == 0u) {
                        cm_fn_0041a360((int)D(0x00ad9c6cu), (int *)rec);
                        continue;
                    }
                }
                flags = R(holder + 0x45u);        /* 005d6cb4: reloaded */
                if ((flags & 0x30000u) != 0u) {
                    u32 b = (u32)(s32)(s8)B(rec + 0x5bu);
                    if (cm_fn_0090cfc0(b) == 0u) {
                        cm_fn_0041a360((int)D(0x00ad9c6cu), (int *)rec);
                        continue;
                    }
                }
                flags = R(holder + 0x45u);        /* 005d6cd5: reloaded */
                tag = B(rec + 0x5bu);
                if ((s32)(s8)tag < 5 && (flags & 0x7800u) != 0u) {
                    u32 b = (u32)(s32)(s8)tag;
                    if (cm_fn_0090cfc0(b) == 0u) {
                        cm_fn_0041a360((int)D(0x00ad9c6cu), (int *)rec);
                    }
                }
            }
        }

    }
}
