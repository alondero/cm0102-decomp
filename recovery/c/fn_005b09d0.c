/* Preferred-image VA 005b09d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Direct callers discard EAX. Wrapper005AFE00 carries its upper bits to
   RET, but its sole caller006B6D07 overwrites full EAX at006B6D0C. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 output, u32 date);
/* 00544414 sign-extends only the input BYTE; this root consumes only AL. */
extern u8 CM_CDECL cm_fn_00544410(u32 profile, u8 mode);
extern u8 CM_CDECL cm_fn_0090cfc0(u32 range);

void CM_THISCALL cm_fn_005b09d0(u32 self, u32 person, volatile u32 club)
{
    volatile u8 scratch[8];
    u32 id, limit, records, other, profile, address;
    u8 mode, score, random, value;
    if (person == 0) return;
    id = D(person);
    limit = D(0x00ae2364u);
    if ((s32)id >= (s32)limit) return;
    records = D(self);
    cm_fn_00417c10(records + id * 9u + 1u, (u32)scratch, 0x00ae2c90u);
    other = club;
    if (other != 0) {
        profile = D(other + 0x69u);
        if (profile != 0) {
            id = D(person);
            records = D(self);
            if (B(records + id * 9u) == 3) return;
            mode = B(profile + 0x17u);
            score = cm_fn_00544410(profile, mode);
            random = cm_fn_0090cfc0((u32)(s32)(s8)score);
            if ((s8)score < 0xf && random == 0) {
                id = D(person);
                records = D(self);
                address = records + id * 9u;
                value = B(address);
                B(address) = (u8)(value + 1u);
                return;
            }
            id = D(person);
            records = D(self);
            address = records + id * 9u;
            if (B(address) == 0 && (s8)random < 6) B(address) = 2;
            else B(address) = 3;
            return;
        }
    }
    id = D(person);
    records = D(self);
    B(records + id * 9u) = 3;
}
