#include "cmre.h"

extern void CM_THISCALL cm_fn_008e5710(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 pointer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);
extern void CM_STDCALL cm_fn_009462bb(u32 base, u32 stride, u32 count,
                                     u32 callback);

void CM_THISCALL cm_fn_005fb730(u32 self)
{
    u32 base;
    s32 index;
    u32 offset;
    s32 count;

    base = *(volatile u32 *)self;
    if (base == 0u)
        return;

    count = *(volatile s32 *)(self + 4u);
    index = 0;
    if (count > 0) {
        offset = 0u;
        do {
            u32 current_base = *(volatile u32 *)self;
            u32 pointer = *(volatile u32 *)(current_base + offset + 0x9a59u);
            if (pointer != 0u) {
                *(volatile u32 *)pointer = 0u;
                current_base = *(volatile u32 *)self;
                pointer = *(volatile u32 *)(current_base + offset + 0x9a59u);
                if (pointer != 0u) {
                    cm_fn_008e5710(pointer);
                    (void)cm_fn_00944c94(pointer);
                }
            }

            current_base = *(volatile u32 *)self;
            pointer = *(volatile u32 *)(current_base + offset + 0x9a5du);
            if (pointer != 0u) {
                *(volatile u32 *)pointer = 0u;
                current_base = *(volatile u32 *)self;
                pointer = *(volatile u32 *)(current_base + offset + 0x9a5du);
                if (pointer != 0u) {
                    cm_fn_008e5710(pointer);
                    (void)cm_fn_00944c94(pointer);
                }
            }

            current_base = *(volatile u32 *)self;
            pointer = *(volatile u32 *)(current_base + offset + 0xa6e3u);
            if (pointer != 0u)
                (void)cm_fn_009452ca(pointer);

            count = *(volatile s32 *)(self + 4u);
            ++index;
            offset += 0xa6efu;
        } while (index < count);
    }

    base = *(volatile u32 *)self;
    if (base != 0u) {
        u32 header_count = *(volatile u32 *)(base - 4u);
        cm_fn_009462bb(base, 0xa6efu, header_count, 0x005fb700u);
        (void)cm_fn_00944c94(base - 4u);
    }
    *(volatile u32 *)self = 0u;
    *(volatile u32 *)(self + 4u) = 0u;
}
