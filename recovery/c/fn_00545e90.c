#include "cmre.h"
/* CM0102 3.9.68, preferred-image VA 0x00545e90, base 0x00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both indexed callers discard EAX's high half with MOVSX r32,AX.
   Partial AX returns and the final ADD therefore return the low WORD. */
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))
#define Q(a) CM_GLOBAL(volatile double, (a))
extern u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern long double CM_THISCALL cm_fn_007c1e30(u32 self, u32 player,
                                           u32 category, u32 field, float incoming);

__attribute__((target("no-sse3")))
u16 CM_CDECL cm_fn_00545e90(u32 subject, u32 context)
{
    u32 record, table, entry, stats, receiver, other, pointer, index, current;
    u32 bonus = 0;
    s32 level, position;
    u16 scale, limit;
    u8 count;
    volatile double saved;
    volatile float incoming;
    volatile long double value, weight, first, second, bound;
    record = D(subject + 0x57u);
    if (record == 0) return W(subject + 0x80u);
    index = D(record);
    table = D(0x00adadfc);
    entry = D(table + index * 4u);
    if (entry == 0) goto unchanged;
    level = (s32)(s16)W(record + 0x69u);
    position = (s32)(s16)W(subject + 0x80u);
    if (level * 500 <= position) goto unchanged;
    receiver = D(entry + 8u);
    stats = cm_fn_004a4850(receiver, subject);
    if (stats == 0) return 0;
    scale = W(entry + 0x3eu);
    level = (s32)B(stats + 0x73u) - 1;
    if (scale != 0 && scale != 20) level = (level * 20) / (s32)(s16)scale;
    if (level < 0) level = 0;
    else if (level > 19) level = 19;
    value = (long double)Q(0x009b59c0u + (u32)level * 8u);
    count = B(stats + 4u);
    saved = (double)value; /* 0x00545f3c FST64 does not replace live ST(0). */
    if (count < 20) {
        other = D(subject + 0x5bu);
        record = D(subject + 0x57u);
        if (other == record) {
            if (scale != 0 && scale != 20) {
                position = (s32)(s8)B(subject + 0x5fu);
                level = (position * 20 - 20) / (s32)(s16)scale;
                if (level < 0) level = 0;
                else if (level > 19) level = 19;
                weight = (long double)Q(0x009b59c0u + (u32)level * 8u);
            } else {
                position = (s32)(s8)B(subject + 0x5fu);
                if (position > 0 && position < 21)
                    weight = (long double)Q(0x009b59b8u + (u32)position * 8u);
                else weight = (long double)Q(0x00968de8);
            }
        } else weight = (long double)Q(0x00968de8);
        first = (long double)(s32)(20u - (u32)count);
        first = first * weight;
        second = (long double)(s32)(u32)count;
        second = second * value;
        first = first + second;
        value = first * (long double)Q(0x00967888);
        saved = (double)value; /* 0x00545fdb FST64 retains extended ST(0). */
    }
    if (context != 0) {
        pointer = D(context + 0x61u);
        if (pointer != 0) {
            receiver = D(0x00ae1050);
            pointer = cm_fn_004cf5a0(receiver, context);
            D(0x00ae1080) = pointer;
            if (pointer != 0) {
                index = D(pointer + 4u);
                table = D(0x00ae23b4);
                current = table + index * 0x245u;
            } else current = 0;
            if (current == subject) {
                value = (long double)saved;
                pointer = D(context + 0x61u);
                receiver = D(0x00b74350);
                incoming = (float)value; /* Original FSTP32 argument. */
                value = cm_fn_007c1e30(receiver, pointer, 6, 20, incoming);
                pointer = D(context + 0x65u);
                value = value + (long double)saved;
                value = value * (long double)Q(0x00968968);
                if (pointer != 0) {
                    if (D(pointer + 4u) == subject || D(pointer + 8u) == subject ||
                        D(pointer + 12u) == subject) bonus = 500;
                    else if (D(pointer + 16u) == subject || D(pointer + 20u) == subject ||
                             D(pointer + 24u) == subject) bonus = (u32)-500;
                }
            } else value = (long double)saved;
        }
    }
    value = value * (long double)Q(0x00969558);
    record = D(subject + 0x57u);
    value = value - (long double)Q(0x00969550);
    bound = (long double)Q(0x009689a0);
    limit = W(record + 0x69u);
    limit = (u16)((u32)limit * 500u);
    if (bound > value) value = (long double)Q(0x009689a0);
    else {
        bound = (long double)Q(0x009688b0);
        /* FCOMP sets C0 for less-than and unordered. */
        if (!(bound >= value)) value = (long double)Q(0x009688b0);
    }
    position = (s32)(s16)W(subject + 0x80u);
    first = (long double)Q(0x009688b0);
    first = first - value;
    second = (long double)position;
    first = first * second;
    second = (long double)(s32)(s16)limit;
    second = second * value;
    first = first + second;
    first = first * (long double)Q(0x00967880);
    /* 0x00546105 CALL 0x00945640 (__ftol), ADR 0006: chop to signed
       64-bit, then use EAX. Remaining original ST(0) is discarded at 0x54610f. */
    return (u16)((u32)(long long)first + bonus);

unchanged:
    if (context != 0) {
        pointer = D(context + 0x61u);
        if (pointer != 0) {
            receiver = D(0x00ae1050);
            pointer = cm_fn_004cf5a0(receiver, context);
            D(0x00ae1080) = pointer;
            if (pointer != 0) {
                index = D(pointer + 4u);
                table = D(0x00ae23b4);
                current = table + index * 0x245u;
            } else current = 0;
            if (current == subject) {
                pointer = D(context + 0x65u);
                if (pointer != 0) {
                    if (D(pointer + 4u) == subject || D(pointer + 8u) == subject ||
                        D(pointer + 12u) == subject) {
                        current = W(subject + 0x80u);
                        return (u16)(current + 500u);
                    }
                    if (D(pointer + 16u) == subject || D(pointer + 20u) == subject ||
                        D(pointer + 24u) == subject) bonus = (u32)-500;
                }
            }
        }
    }
    current = W(subject + 0x80u);
    return (u16)(current + bonus);
}
