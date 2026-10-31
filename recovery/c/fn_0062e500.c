#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern void CM_THISCALL cm_fn_0062d250(u32, u32, u32, u32, u32, u32, u32, u32, u32);

/* Sole caller0062d9bf overwrites full EAX at0062d9c4 before use. */
void CM_THISCALL cm_fn_0062e500(u32 self, u32 first, u32 second)
{
    u8 scratch[0x10];
    u32 local = (u32)scratch;
    u32 record, person, base, link, total, index, offset, random, value, score;
    u32 candidate, club, owner, table, code, target, bucket, kind;
    value = B(first + 0x12);
    D(local + 0xc) = self;
    D(local + 8) = 0;
    B(local + 3) = 0;
    D(local + 4) = 0;
    if (value != 0xff)
        return;
    value = (u32)(s32)(s8)B(second);
    if (D(0x009e7ae0 + value * 13u) == 0)
        return;
    if (cm_fn_0090cfc0(20) != 0)
        return;
    value = B(second);
    B(first + 0x13) = 0;
    B(first + 0x12) = (u8)value;
    total = D(0x00ae2364);
    record = D(self);
    if ((s32)total <= 0)
        return;
    offset = 0;
    index = 0;
    do {
        if (B(record + 0x12) != 0xff) {
            base = D(0x00ae23bc);
            person = base + offset;
            random = cm_fn_0090cfc0(101);
            value = B(record + 0x13);
            score = (u8)((u8)random + (u8)(value * 251u));
            link = D(person + 0x61);
            if (link == 0)
                score = 100;
            else
                score = (u8)(score - B(link + 0x2d));
            if ((s8)score > (s8)B(local + 3)) {
                B(local + 3) = (u8)score;
                D(local + 8) = record;
                D(local + 4) = person;
            } else {
                person = D(local + 4);
            }
            if ((s8)B(local + 3) >= 95)
                goto chosen;
        }
        total = D(0x00ae2364);
        ++index;
        offset += 110;
        record += 31;
    } while ((s32)index < (s32)total);
    person = D(local + 4);
chosen:
    candidate = D(local + 8);
    if (candidate == 0)
        return;
    link = D(person + 0x61);
    if (link != 0) {
        club = D(person + 0x39);
        if (club != 0 && (s16)W(club + 0x80) > 0x109a &&
            cm_fn_00600a50(club, 0, 1, 0, 0) == 0 &&
            cm_fn_0090cfc0(3) == 0 && person != 0) {
            code = 0xff;
            value = D(person);
            total = D(0x00ae2364);
            if ((s32)value < (s32)total && D(person + 0x61) != 0) {
                owner = D(local + 0xc);
                offset = value * 31u;
                table = D(owner);
                code = B(table + offset + 0x12);
            }
            value = (u32)(s32)(s8)code - 11u;
            kind = 0x4d;
            if (value <= 0x30) {
                bucket = B(0x0062e6cc + value);
                target = D(0x0062e6c0 + bucket * 4u);
                if (target == 0x0062e66f)
                    kind = 0x4e;
                else if (target == 0x0062e676)
                    kind = 0x4f;
                /* The remaining pinned table target is0062e67d. */
            }
            value = (first & 0xffffff00u) | kind;
            owner = D(local + 0xc);
            cm_fn_0062d250(owner, value, 0, person, 0, 1, 0xffffffffu, 0, 0);
            return;
        }
    }
    candidate = D(local + 8);
    B(candidate + 0x12) = 0xff;
    B(candidate + 0x13) = 0;
}
