#include "cmre.h"

extern u32 CM_CDECL cm_fn_00934b60(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7, u32 p8, u32 p9);

/* Cdecl with seven stack arguments (names are entry offsets), `ret 0`, no
   return value. arg_04 is an object pointer (fields 0x3c8, 0x3cc, 0x448) and
   arg_08 a record pointer (word at +2, dwords at +0x18, +0x1c, +0x20, +0x24).
   Field store order: 0x448 gets (arg_18 word != 0) before anything else.
   When arg_0c's low word is zero, the record is updated in two loops whose
   pointer arithmetic is reproduced literally: the first loop does not reset
   its source pointer, so each step subtracts the current destination
   (src -= dst; read src + dst; store dst; dst += 4). The second loop copies
   the sum (arg_1c word + record word at +2) dwords from record+0x1c-based
   source. The lifted callee 0x934b60 then gets (arg_04, arg_08, arg_10,
   arg_14, arg_0c, 0, 0, 0, 0). Afterwards record+0x24 gets
   ((record[0x20 index] - record[...]) + 0x20) >> 6 (arithmetic shift). */
u32 CM_CDECL cm_fn_00936510(u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10,
                             u32 arg_14, u32 arg_18, u32 arg_1c)
{
    u32 result;
    s32 count;
    u32 edx;
    u32 ecx;
    u32 c;
    u32 src;
    u32 v;
    u32 k;
    u32 lo;
    u32 hi;
    u32 t;

    *(volatile u32 *)(arg_04 + 0x448) = ((u16)arg_18 != 0) ? 1u : 0u;

    if ((u16)arg_0c == 0) {
        count = (s32)*(volatile s16 *)(arg_08 + 2) + (s32)(s16)arg_1c;
        edx = *(volatile u32 *)(arg_08 + 0x18);
        ecx = *(volatile u32 *)(arg_04 + 0x3c8);
        if (count > 0) {
            src = edx;
            c = ecx;
            k = (u32)count;
            do {
                src -= c;
                v = *(volatile u32 *)(src + c);
                *(volatile u32 *)c = v;
                c += 4u;
                k--;
            } while (k != 0);
        }

        ecx = (u32)(s32)*(volatile s16 *)(arg_08 + 2);
        hi = *(volatile u32 *)(edx + ecx * 4u + 4u);
        lo = *(volatile u32 *)(edx + ecx * 4u);
        edx = *(volatile u32 *)(arg_08 + 0x1c);
        hi -= lo;
        *(volatile u32 *)(arg_08 + 0x20) = hi;
        ecx = *(volatile u32 *)(arg_04 + 0x3cc);
        if (count > 0) {
            src = edx - ecx;
            c = ecx;
            k = (u32)count;
            do {
                v = *(volatile u32 *)(src + c);
                *(volatile u32 *)c = v;
                c += 4u;
                k--;
            } while (k != 0);
        }
    }

    result = cm_fn_00934b60(arg_04, arg_08, arg_10, arg_14, arg_0c, 0u, 0u, 0u, 0u);

    ecx = (u32)(s32)*(volatile s16 *)(arg_08 + 2);
    edx = *(volatile u32 *)(arg_08 + 0x18);
    hi = *(volatile u32 *)(edx + ecx * 4u + 4u);
    lo = *(volatile u32 *)(edx + ecx * 4u);
    t = hi - lo;
    t += 0x20u;
    *(volatile u32 *)(arg_08 + 0x24) = (u32)(((s32)t) >> 6);
    return result;
}
