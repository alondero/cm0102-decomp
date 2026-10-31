#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern void CM_THISCALL cm_fn_0055f7c0(u32, u32);
extern void CM_FASTCALL cm_fn_007fef10(u32);

static u32 cm_group(u32 self)
{
    return self + (u32)(s32)(s16)W(self + 0x382a) * 768u;
}

u32 CM_THISCALL cm_fn_00803d80(u32 self)
{
    u32 group, cursor, node, value, saved, target, context, offset, raw_dx;
    group = cm_group(self);
    if ((s16)W(group + 0x7e6) > 0) {
        value = D(group + 0x7de);
        D(group + 0x7e2) = value;
        group = cm_group(self);
        cursor = group + 0x7e2;
        node = D(cursor);
        while (node != 0) {
            value = D(node + 0x10);
            if (value != 0) {
                value = D(node + 0xc);
                if (value == 0) {
                    group = cm_group(self);
                    D(group + 0x7ea) = 1;
                    return 0;
                }
            }
            value = D(node + 0x1f4);
            D(cursor) = value;
            group = cm_group(self);
            cursor = group + 0x7e2;
            node = D(cursor);
        }
    }
    group = cm_group(self);
    node = D(group + 0x7e2);
    if (node != 0) {
        saved = node;
        context = D(group + 0x7da);
        D(group + 0x7e2) = context;
        group = cm_group(self);
        raw_dx = group - self;
        node = D(group + 0x7e2);
        while (node != 0) {
            target = D(node + 0xc);
            if (target != 0) {
                /* Preserve the original ECX/EDX at this zero-stack-argument
                   boundary. ECX is the head, then the previous node; EDX
                   starts as the group offset, then holds the next node. */
                ((void (CM_FASTCALL *)(u32, u32))target)(context, raw_dx);
            }
            group = cm_group(self);
            node = D(group + 0x7e2);
            context = node;
            value = D(node + 0x1f8);
            D(group + 0x7e2) = value;
            raw_dx = value;
            group = cm_group(self);
            node = D(group + 0x7e2);
        }
        group = cm_group(self);
        offset = group - self;
        D(group + 0x7e2) = saved;
        group = cm_group(self);
        raw_dx = group - self;
        node = D(group + 0x7e2);
        target = D(node + 4);
        if (target != 0) {
            /* Original ECX/EDX contain their separately reloaded offsets. */
            ((void (CM_FASTCALL *)(u32, u32))target)(offset, raw_dx);
        }
        group = cm_group(self);
        context = D(group + 0x7fc);
        cm_fn_0055f7c0(context, 1);
        group = cm_group(self);
        D(group + 0x7e2) = 0;
    }
    group = cm_group(self);
    D(group + 0x7ea) = 0;
    cm_fn_007fef10(self);
    return 1;
}
