/* fn_00942f50 (VA 0x00942f50), cdecl, ret 0. Arguments: arg1 = object pointer at
   entry+4 (the listing's [esp+8] after push esi), arg2 = 16-bit key at entry+8 (read as
   the low word of that slot). Returns 1 if the key is in the 10-entry table at
   object[0x28] + 0x18, else 0. EAX is 0 or 1 on every path.

   Path A (object[0x28] != 0): search the table only.
   Path B (object[0x28] == 0): find the record whose tag at +4 is 'pamc' (0x636d6170) in
   the table at P[0x10] (count = signed word P[8]); the record is 0 if none is found, and the
   listing then reads address 0x10 and 0x0c. It then calls 0x00941630, 0x00942c00
   (stores its result at object[0x28]) and 0x00941760, and falls into the search.

   Callee EDX: after 0x00941630 the listing's EDX high half is the callee's leftover, so
   only the low word from object[0x32] is passed as the third argument (the callee reads
   a 16-bit value). The second argument keeps the high half of the first call's EAX
   result, as in `mov ax, word [esi+0x30]`. */
#include "cmre.h"

extern u32 * cm_fn_00941630(int param_1,u32 *param_2,int param_3,int param_4,int param_5,int *param_6);
extern void CM_CDECL cm_fn_00941760(u8 *object, u32 *output);
extern u32 * cm_fn_00942c00(u32 param_1,u32 param_2,u32 param_3,u32 param_4);

u32 cm_fn_00942f50(u32 param_1, u16 param_2)
{
    u32 obj;
    u32 p;
    s32 count;
    s32 i;
    u32 table;
    u32 entry;
    u32 record;
    u32 rec_ecx;
    u32 rec_edx;
    u32 a54;
    u32 a5c;
    u32 ret;
    u32 w32;
    u32 w30;
    u32 eax_val;
    u32 edx_val;
    u32 ret2;
    u32 cur;

    obj = param_1;

    if (*(volatile u32 *)(obj + 0x28) == 0) {
        /* Path B: find the tagged record in the table at P[0x10]. */
        p = *(volatile u32 *)(obj + 0);
        count = (s32)(s16)*(volatile u16 *)(p + 8);
        record = 0;
        if (count > 0) {
            table = *(volatile u32 *)(p + 0x10);
            for (i = 0; i < count; i++) {
                entry = *(volatile u32 *)(table + 4 * (u32)i);
                if (*(volatile u32 *)(entry + 4) == 0x636d6170u) {
                    record = *(volatile u32 *)(table + 4 * (u32)i);
                    break;
                }
            }
        }

        /* 0x00942f84: fields of the record (0x10 first, then 0x0c), then object fields. */
        rec_ecx = *(volatile u32 *)(record + 0x10);
        rec_edx = *(volatile u32 *)(record + 0x0c);
        a54 = *(volatile u32 *)(obj + 0x54);
        a5c = *(volatile u32 *)(obj + 0x5c);
        ret = (u32)cm_fn_00941630((int)a5c, (u32 *)a54, (int)rec_edx, (int)rec_ecx, 0, 0);

        w32 = *(volatile u16 *)(obj + 0x32);
        a5c = *(volatile u32 *)(obj + 0x5c);
        w30 = *(volatile u16 *)(obj + 0x30);
        eax_val = (ret & 0xffff0000u) | w30;
        edx_val = w32;
        ret2 = (u32)cm_fn_00942c00(a5c, eax_val, edx_val, ret);
        *(volatile u32 *)(obj + 0x28) = ret2;
        cm_fn_00941760((u8 *)ret, 0);
    }

    /* 0x00942fc3: search the 10-entry table at object[0x28] + 0x18 for the key. */
    cur = *(volatile u32 *)(obj + 0x28) + 0x18;
    for (i = 0; i < 10; i++) {
        if (*(volatile u16 *)(cur + 2 * (u32)i) == param_2) {
            return 1;
        }
    }
    return 0;
}
