#include "cmre.h"

/* Preferred-image VA0049D380/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/full EAX. Scratch468 preserves the two output slots and all
   three adjacent 200-byte labels, followed by two 256-byte error buffers. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define DRAW(slot, item, style, label, kind, sub, group, round) \
    cm_fn_0049d930((slot), (item), ui, (style), ui + 0x14, \
                  (label), (kind), (sub), (group), (round))
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32);
extern u32 CM_THISCALL cm_fn_0048d200(u32, u32);
extern u32 CM_CDECL cm_fn_004b01d0(u32);
extern u32 CM_CDECL cm_fn_0049d930(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_0049d380(volatile u32 object_arg, volatile u32 selected_arg, u32 ui)
{
    u8 scratch[0x468];
    u32 L = (u32)scratch, object = object_arg, stage, record, value, index;
    u32 p, category, kind, sub, count, end, round, mode, label, slot, item, style;
    u8 type, rounds;
    if (object == 0) {
        cm_fn_00944cff(0x99c15c, 0, 0, (void *)(L + 0x368), (void *)(L + 0x268));
        value = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, value, L + 0x368, L + 0x268, 0x1f1b);
        cm_fn_005e8290(0x9870e0, 0xae24d0, 0);
        D(0xb67a34) = 0;
        return 0;
    }
    value = D(object + 0x30);
    stage = 0xffffffff;
    if (value == 0xffffffff) {
        if (B(object + 0x43) == 2) return 0;
        if (value == stage && B(object + 0x43) == 1 && B(object + 0xac) == 1) return 0;
    }
    cm_fn_0066f4e0(L + 0x10, 0x99c518);
    cm_fn_0066f4e0(L + 0x1a0, 0x99c97c);
    cm_fn_0066f4e0(L + 0xd8, 0x99c974);
    value = D(object + 0x30);
    D(L + 4) = 0xffffffff;
    D(L + 8) = 0xffffffff;
    if ((s32)value < -1) return 0;
    do {
        record = cm_fn_0048d200(object, stage);
        kind = (u32)(s32)(s16)W(record + 0x3a);
        category = cm_fn_004b01d0(kind);
        D(L + 0xc) = category;
        if (category != 0xffffffff) {
            end = D(object + 0x30);
            index = stage + 1;
            D(L) = 1;
            if ((s32)index <= (s32)end) {
                do {
                    p = D(object + 0xc);
                    p = D(p + index * 4);
                    kind = (u32)(s32)(s16)W(p + 0x3a);
                    value = cm_fn_004b01d0(kind);
                    category = D(L + 0xc);
                    if (value != category) break;
                    count = D(L);
                    end = D(object + 0x30);
                    ++count;
                    ++index;
                    D(L) = count;
                } while ((s32)index <= (s32)end);
                category = D(L + 0xc);
            }
            end = D(object + 0x30) + 2;
            count = D(L);
            if (count == end) {
                end = count + stage;
                index = stage;
                if ((s32)stage < (s32)end) {
                    do {
                        p = cm_fn_0048d200(object, index);
                        kind = (u32)(s32)(s16)W(p + 0x3a);
                        DRAW(L + 4, 0x28f, 0x30c, L + 0x1a0, kind, 0, index, 0);
                        count = D(L);
                        ++index;
                        end = count + stage;
                    } while ((s32)index < (s32)end);
                    count = D(L);
                    stage = count + stage - 1;
                    goto next_stage;
                }
            } else {
                DRAW(L + 4, 0x28f, 0x30c, L + 0x10, category, 0, stage, 0);
                value = selected_arg;
                if ((s32)value >= (s32)stage) {
                    end = D(L) + stage;
                    if ((s32)value < (s32)end && (s32)stage < (s32)end) {
                        index = stage;
                        do {
                            p = cm_fn_0048d200(object, index);
                            kind = (u32)(s32)(s16)W(p + 0x3a);
                            DRAW(L + 8, 0x210, 0x28d, L + 0x1a0, kind, 0, index, 0);
                            count = D(L);
                            ++index;
                            end = count + stage;
                        } while ((s32)index < (s32)end);
                    }
                }
            }
            count = D(L);
            stage = count + stage - 1;
        } else {
            type = B(record + 0x43);
            if (type == 1) {
                if (D(object + 0x30) == 0xffffffff) {
                    rounds = B(record + 0xac);
                    mode = 0;
                } else {
                    rounds = B(record + 0xac);
                    if (rounds == 1) {
                        p = D(record + 0xa3);
                        kind = (u32)(s32)(s16)W(record + 0x3a);
                        sub = (u32)(s32)(s16)W(p + 7);
                        DRAW(L + 4, 0x28f, 0x30c, L + 0x10, kind, sub, stage, 0);
                        goto next_stage;
                    }
                    kind = W(record + 0x3a);
                    if (kind == 0) mode = 1;
                    else {
                        kind = (u32)(s32)(s16)(u16)kind;
                        DRAW(L + 4, 0x28f, 0x30c, L + 0x10, kind, 0, stage, 0);
                        value = selected_arg;
                        if (value != stage) goto next_stage;
                        rounds = B(record + 0xac);
                        mode = 2;
                    }
                }
                round = 0;
                if ((s8)rounds > 0) {
                    D(L) = 0;
                    label = mode == 1 ? L + 0x10 : L + 0xd8;
                    slot = mode == 2 ? L + 8 : L + 4;
                    item = mode == 2 ? 0x210 : 0x28f;
                    style = mode == 2 ? 0x28d : 0x30c;
                    do {
                        count = (u32)(s32)(s16)W(record + 0x3c);
                        if (round == count - 2) {
                            p = D(record + 0xa3);
                            if (W(p + count * 104u - 0x61) == 140) {
                                --count;
                                kind = mode == 0 ? (u32)(s32)(s16)W(record + 0x3a) : 0;
                                DRAW(slot, item, style, label, kind, 140, stage, count);
                            }
                        }
                        p = D(record + 0xa3);
                        value = D(L);
                        sub = (u32)(s32)(s16)W(p + value + 7);
                        kind = mode == 0 ? (u32)(s32)(s16)W(record + 0x3a) : 0;
                        DRAW(slot, item, style, label, kind, sub, stage, round);
                        if (mode == 1) {
                            value = D(L);
                            value += 104;
                            ++round;
                            D(L) = value;
                            count = (u32)(s32)(s8)B(record + 0xac);
                        } else {
                            count = (u32)(s32)(s8)B(record + 0xac);
                            value = D(L);
                            ++round;
                            value += 104;
                            D(L) = value;
                        }
                    } while ((s32)round < (s32)count);
                }
                object = object_arg;
            } else if (type == 2) {
                kind = (u32)(s32)(s16)W(record + 0x3a);
                DRAW(L + 4, 0x28f, 0x30c, L + 0x10, kind, 0, stage, 0);
            }
        }
next_stage:
        value = D(object + 0x30);
        ++stage;
    } while ((s32)stage <= (s32)value);
    if (W(L + 4) != 0xffff || W(L + 8) != 0xffff) return 1;
    return 0;
}
