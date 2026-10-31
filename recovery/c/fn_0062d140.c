/* Preferred-image VA 0062d140, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define LB(o) (*(volatile u8 *)(work + (o)))
extern u32 CM_THISCALL cm_fn_0062ed70(u32 self, u32 person);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 limit);

u8 CM_THISCALL cm_fn_0062d140(u32 self, volatile u8 type, volatile u8 category, u32 person)
{
    u8 work[4];
    u32 pointer, id, table, row, bound;
    u8 code, kind, index, end, choice, candidate;
    LB(2) = 100;
    LB(3) = 0xffu;
    LB(1) = 0xffu;
    pointer = cm_fn_0062ed70(self, person);
    if (pointer != 0) {
        code = B(pointer);
        if (code > 4 && code != 0x4c && code != 0x4d && code != 0x4f &&
            code != 0x4e && code != 0x50 && code != 0x51)
            return code;
    }
    kind = type;
    if (person != 0) {
        id = D(person);
        table = D(self);
        code = B(table + id * 31u + 0x12u);
        if (code != 0xffu && B(0x009e7ad9u + (u32)(s32)(s8)code * 13u) == kind)
            LB(1) = code;
    }
    row = (u32)(s32)(s8)kind * 3u;
    index = B(0x009e7f05u + row);
    bound = 0x009e7f06u + row;
    end = B(bound);
    if ((s8)index <= (s8)end) {
        do {
            kind = category;
            if (kind != 0xffu) {
                code = B(0x009e7ae4u + (u32)(s32)(s8)index * 13u);
                if (code != kind && code != 0xffu)
                    goto next;
            }
            if (kind == 3)
                goto next;
            choice = LB(1);
            if (index == choice) {
                candidate = (u8)cm_fn_0090cfc0(3);
                candidate = (u8)(candidate - 1u);
            } else {
                code = B(0x009e7adau + (u32)(s32)(s8)index * 13u);
                candidate = (u8)cm_fn_0090cfc0((s8)code);
            }
            if ((s8)candidate < (s8)LB(2)) {
                LB(2) = candidate;
                LB(3) = index;
            }
next:
            end = B(bound);
            index = (u8)(index + 1u);
        } while ((s8)index <= (s8)end);
    }
    return LB(3);
}
