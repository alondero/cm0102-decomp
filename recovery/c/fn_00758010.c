#include "cmre.h"
/* Preferred-image VAs below refer to CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   7581f9 pops four arguments. 7580ba/c2 and 7580e7/ee set ECX
   receivers; 7581a1-b2 passes five stack arguments and ECX, with
   no caller cleanup. 7581bc-be and 7581cd-cf use EBP as receiver. */
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00544520(u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00548150(u32 object, u32 expected);
extern u32 CM_CDECL cm_fn_005481a0(volatile u8 *object);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern void CM_THISCALL cm_fn_0072c1f0(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e);
extern u32 CM_THISCALL cm_fn_00867560(u32 self, u32 flag);
extern u32 CM_THISCALL cm_fn_008676f0(volatile u8 *self);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
#define R16(a) CM_GLOBAL(volatile s16, (u32)(a))
#define R8(a) CM_GLOBAL(volatile s8, (u32)(a))

void CM_STDCALL cm_fn_00758010(u32 person, volatile u32 club_home, u32 a3, u32 a4)
{
    u32 club, p, t, table, cursor, other, record, v;
    s32 rating, ability, secondary;
    u32 first, second, kind;
    int sent = 0;
    (void)a3; (void)a4;
    if (!person) return;
    p = R32(person + 0x61);
    if (!p) return;
    club = club_home;
    if (!club) return;
    t = cm_fn_005481a0((volatile u8 *)person);
    v = cm_fn_00548150(club, t);
    *(volatile u8 *)&club_home = (u8)v;
    if ((s8)v <= 2) return;
    t = R32(club + 0x57);
    if (!t) return;
    t = R32(t);
    table = R32(0xadadfc);
    if (!R32(table + t * 4u)) return;
    p = R32(person + 0x61);
    if (R16(p + 0xb) <= 0x128e) return;
    cursor = club + 0xd7;
    for (u32 count = 50; count; --count, cursor += 4) {
        other = R32(cursor);
        if (!other) continue;
        p = R32(other + 0x61);
        if (!p) continue;
        if (other == person) continue;
        t = R32(0xae1050);
        record = cm_fn_004cf5f0(t, other, club);
        if (!record) continue;
        p = R32(other + 0x61);
        if (!p) continue;
        if (R8(record + 0x3d) <= 0) continue;
        t = R32(0xb63d28);
        if (cm_fn_0062c400(t, person) == 2) continue;
        if (R32(record + 0x45) & 0x101) continue;
        if (cm_fn_00544520(other, club)) continue;
        if (R8(other + 0x18) <= 0x14) continue;
        kind = (u8)R8(record + 0x4f) & 0xf0;
        if (kind != 0x10 && kind != 0x20 && kind != 0x30) continue;
        rating = *(volatile s8 *)&club_home;
        ability = R8(other + 0x5a);
        if (ability >= rating + 5) continue;
        secondary = R8(other + 0x5b);
        if (secondary >= rating + 10) continue;
        first = (u32)cm_fn_0090cfc0((u32)rating, (u32)(rating + 10), (u32)rating);
        second = (u32)cm_fn_0090cfc0(first, first, (u32)ability);
        if ((s32)second >= (s32)first) continue;
        first = cm_fn_005481a0((volatile u8 *)person);
        second = cm_fn_005481a0((volatile u8 *)other);
        if (first != second) continue;
        p = R32(other + 0x61);
        if (R16(p + 0xb) <= 0xea6) continue;
        if (!sent) {
            t = R32(0xb6b7c8);
            cm_fn_0072c1f0(t, other, club, 0x25, 0, person);
            sent = 1;
        }
        v = cm_fn_008676f0((volatile u8 *)record);
        if (!(v & 0x800))
            cm_fn_00867560(record, 0x800);
    }
}
