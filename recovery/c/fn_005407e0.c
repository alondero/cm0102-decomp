#include "cmre.h"

/* Preferred-image VA 0x005407e0, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full physical EAX, including the high WORD left by each MOV AX.
   Six callers save EAX before later uses; no deadness assumed. The callee
   remains original code: its accepted AL-only C implementation does not
   establish this wider boundary or composed execution. */
extern u32 CM_CDECL cm_fn_005408c0(u32 first, u32 second);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define U16_AT(base, offset) (*(volatile u16 *)((base) + (offset)))
#define S16_AT(base, offset) (*(volatile s16 *)((base) + (offset)))

u32 CM_CDECL cm_fn_005407e0(u32 first, u32 second, u32 mode)
{
    u32 returned, kind, record, base, high;
    s32 upper, lower;

    returned = cm_fn_005408c0(first, second);
    kind = (u8)returned;
    /* For second==0 the callee jumps directly to MOV AL,4. Native entry
       EAX is zero on BOTH root paths: mode==0 remains zero at 540869;
       mode!=0 loads second into EAX before 5407f3. Other callee paths
       overwrite EAX before returning. Reconstruct this known input effect
       without depending on the C compiler's caller-saved EAX allocation. */
    high = second == 0 ? 0u : returned & 0xffff0000u;
    if (mode != 0) {
        if (kind == 4) {
            record = U32_AT(first, 0x69);
            return high | U16_AT(record, 0x0c);
        }
        if (kind == 2) {
            record = U32_AT(first, 0x69);
            return high | U16_AT(record, 8);
        }
        if (kind == 0) {
            record = U32_AT(first, 0x69);
            return (record & 0xffff0000u) | U16_AT(record, 0x0a);
        }
        record = U32_AT(first, 0x69);
        base = 8;
    } else {
        if (kind == 4) {
            record = U32_AT(first, 0x61);
            return (record & 0xffff0000u) | U16_AT(record, 0x0d);
        }
        if (kind == 2) {
            record = U32_AT(first, 0x61);
            return high | U16_AT(record, 9);
        }
        if (kind == 0) {
            record = U32_AT(first, 0x61);
            return high | U16_AT(record, 0x0b);
        }
        record = U32_AT(first, 0x61);
        base = 9;
    }
    /* Each signed half truncates separately. Native EAX contains the lower
       half before MOV AX,CX inserts the sum's low WORD. */
    upper = S16_AT(record, base + (kind == 3 ? 4 : 2));
    lower = S16_AT(record, base);
    lower /= 2;
    return ((u32)lower & 0xffff0000u) | (u16)(upper / 2 + lower);
}
