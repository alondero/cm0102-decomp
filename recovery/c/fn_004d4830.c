/* Preferred-image VA 0x004d4830, image base 0x00400000.
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern s32 CM_THISCALL cm_fn_00866830(u32 self, u32 record, u32 optional);
extern void CM_THISCALL cm_fn_00417c10(u32 self, void *result, u32 source);

/* Both original exits RET4. The sole caller next invokes 007ff010, whose
   first instruction overwrites full EAX without reading its incoming value. */
__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_004d4830(u32 self, u32 record)
{
    u32 key = D(record);
    u32 table = D(0x00ae23bc);
    u32 comparison = D(record + 4);
    u32 list = D(self + 0x34);
    u32 item, word, i;
    u32 selected = table + key * 110u;
    s32 first, next;
    volatile float base;
    volatile union { float value; u32 bits; } step;
    long double current, ratio, adjustment, total;
    volatile long long converted;
    u8 date_result[8];

    step.bits = 0x47c35000u;
    if (!list) return;
    word = D(list + 2);
    D(list + 0x0a) = word;
    item = cm_fn_0068d1b0(D(self + 0x34));
    if (!item) return;
    for (;;) {
        if (D(item) == key && D(item + 4) == comparison) break;
        item = cm_fn_0068d1b0(D(self + 0x34));
        if (!item) return;
    }
    if (B(item + 0x50) != 3) return;
    first = cm_fn_00866830(record, selected, 0);
    word = D(item + 0x10);
    /* Original FILD then FSTP32 explicitly rounds the first result. */
    D(record + 0x10) = word;
    word = D(item + 0x14);
    base = (float)(long double)first;
    D(record + 0x14) = word;
    word = D(item + 0x18);
    D(record + 0x18) = word;
    /* Forward DWORD copies retain REP MOVSD overlap behavior and read order. */
    for (i = 0; i != 5; ++i) {
        word = D(item + 0x1c + i * 4);
        D(record + 0x1c + i * 4) = word;
    }
    cm_fn_00417c10(record + 0x2d, date_result, item + 0x2d);
    next = cm_fn_00866830(record, selected, 0);
    for (;;) {
        current = (long double)next;
        ratio = current / (long double)base;
        /* Unordered FCOM sets C0: it follows the adjustment branch. */
        if (ratio >= (long double)CM_GLOBAL(volatile double, 0x009690a0)) {
            if (ratio <= (long double)CM_GLOBAL(volatile double, 0x00969098)) return;
        }
        /* TEST AH,41 also exits on unordered. */
        if (!(step.value > CM_GLOBAL(volatile float, 0x00968a78))) return;
        adjustment = (long double)base - current;
        adjustment *= (long double)CM_GLOBAL(volatile float, 0x00968f18);
        step.value = (float)adjustment;
        total = (long double)(s32)D(record + 0x0c);
        total += (long double)step.value;
        /* Original __ftol CALL at VA 0x004d4973: signed 64-bit chopping
           conversion, then retain only EAX. ADR 0006. */
        converted = (long long)total;
        D(record + 0x0c) = (u32)converted;
        next = cm_fn_00866830(record, selected, 0);
    }
}
