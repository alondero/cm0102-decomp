/* Preferred-image VA 008ced40/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
typedef u16 word_8ced40 __attribute__((may_alias));
#define W(a) CM_GLOBAL(volatile word_8ced40, (u32)(a))
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
typedef u32 (__attribute__((stdcall, regparm(3))) *method_8ced40)(
    u32 eax_value, u32 edx_value, u32 self,
    u32 club, u32 person, u32 fourth, u32 second, volatile u16 *reason);

/* THIS6/RET18/fullEAX. The callback has five stack arguments/RET14.
   Carry the exact EAX/EDX/ECX setup as well. Its reason pointer is the address
   of the actual first argument slot, not a new initialized scratch local.
   The cached original club stays independent of that overwritten low WORD.
   A successful retry restores the person's nation before examining EAX. */
u32 CM_THISCALL cm_fn_008ced40(u32 self, volatile u32 club_argument,
                              u32 second, u32 person, u32 fourth,
                              volatile u32 retry_argument,
                              volatile u32 reason_argument)
{
    u32 club = club_argument, nation, cursor, object, table, target, result;
    u32 output, old_nation;
    u8 index = 0;
    u16 code;
    nation = D(club + 0x53u);
    cursor = self + 0x8acu;
    for (;;) {
        object = D(cursor);
        if (D(object + 0xau) == nation) {
            object = D(cursor);
            break;
        }
        ++index;
        cursor += 4u;
        if ((s8)index == 28) {
            object = 0;
            break;
        }
    }
    output = reason_argument;
    if (output != 0) W(output) = 0;
    if (object == 0) return 1;
    table = D(object);
    target = D(table + 0x10u);
    result = ((method_8ced40)target)(table, second, object, club, person,
                                    fourth, second, (volatile u16 *)&club_argument);
    if (output != 0) W(output) = W((u32)&club_argument);
    if (result != 0) return result;
    if (retry_argument == 0) return 0;
    if (cm_fn_00600a50(club, 0, 1, 0, 0) == 0) {
        if (output != 0) W(output) = W((u32)&club_argument);
        return 0;
    }
    code = W((u32)&club_argument);
    if (code != 0x14 && code != 0x15 && code != 0x1f && code != 0x11) {
        if (output != 0) W(output) = code;
        return 0;
    }
    nation = D(club + 0x53u);
    old_nation = D(person + 0x1au);
    D(person + 0x1au) = nation;
    table = D(object);
    target = D(table + 0x10u);
    result = ((method_8ced40)target)(fourth, table, object, club, person,
                                    fourth, second, (volatile u16 *)&club_argument);
    D(person + 0x1au) = old_nation;
    if (result != 0) return 1;
    output = reason_argument;
    if (output != 0) W(output) = W((u32)&club_argument);
    return 0;
}
