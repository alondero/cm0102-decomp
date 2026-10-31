#include "cmre.h"

/* Thiscall, no stack arguments (ret 0). Object fields by offset: 0x00 pointer
   table with a count at 0x04; 0x0c pointer table with a count at 0x10; single
   pointers at 0x862, 0x86e, 0x880, 0x88c, 0x898, 0x8a4, 0x91c, 0x920, 0x93a; 0x856
   and 0x946 are released through their own helpers; 0x8ac is a 28-entry table;
   0x92c and 0x872 are sub-objects cleared by 0x0068d040. The SEH state is 3, then
   2 and 1 (byte writes) before the sub-object clears, then -1 before 0x009462bb. */

extern void CM_FASTCALL cm_fn_008e5710(u32 self);
extern void CM_FASTCALL cm_fn_008e8540(u32 self);
extern void CM_FASTCALL cm_fn_0068d040(u32 self);
extern void CM_THISCALL cm_fn_00839d90(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);
extern void CM_STDCALL cm_fn_009462bb(u32 p1, u32 p2, u32 p3, u32 p4);

void CM_THISCALL cm_fn_008bbe20(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 i;
    u32 item;
    u32 ebx_slot;
    u32 count;

    seh.state = -1;
    seh.handler = 0x009639e3u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    item = *(volatile u32 *)self;
    seh.state = 3;
    if (item != 0) {
        count = *(volatile u32 *)(self + 4u);
        if ((s32)count > 0) {
            i = 0;
            do {
                item = *(volatile u32 *)(*(volatile u32 *)self + i * 4u);
                if (item != 0) {
                    cm_fn_008e5710(item);
                    cm_fn_00944c94(item);
                }
                i = i + 1u;
            } while ((s32)i < (s32)*(volatile u32 *)(self + 4u));
        }
        cm_fn_009452ca(*(volatile u32 *)self);
    }

    if (*(volatile u32 *)(self + 0xcu) != 0) {
        count = *(volatile u32 *)(self + 0x10u);
        if ((s32)count > 0) {
            i = 0;
            do {
                item = *(volatile u32 *)(*(volatile u32 *)(self + 0xcu) + i * 4u);
                if (item != 0) {
                    cm_fn_008e5710(item);
                    cm_fn_00944c94(item);
                }
                i = i + 1u;
            } while ((s32)i < (s32)*(volatile u32 *)(self + 0x10u));
        }
        cm_fn_009452ca(*(volatile u32 *)(self + 0xcu));
    }

    item = *(volatile u32 *)(self + 0x862u);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x856u);
    if (item != 0) {
        cm_fn_00839d90(item);
        cm_fn_00944c94(item);
    }
    item = *(volatile u32 *)(self + 0x86eu);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x91cu);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x880u);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x88cu);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x898u);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x920u);
    if (item != 0) {
        cm_fn_009452ca(item);
    }

    ebx_slot = self + 0x8acu;
    for (i = 0; i < 0x1cu; i++) {
        item = *(volatile u32 *)(ebx_slot);
        if (item != 0) {
            cm_fn_008e8540(item);
            cm_fn_00944c94(item);
        }
        ebx_slot = ebx_slot + 4u;
    }

    item = *(volatile u32 *)(self + 0x8a4u);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x93au);
    if (item != 0) {
        cm_fn_009452ca(item);
    }
    item = *(volatile u32 *)(self + 0x946u);
    if (item != 0) {
        cm_fn_0068d040(item);
        cm_fn_00944c94(item);
    }

    *(volatile u8 *)&seh.state = 2;
    cm_fn_0068d040(self + 0x92cu);

    *(volatile u8 *)&seh.state = 1;
    cm_fn_0068d040(self + 0x872u);

    seh.state = -1;
    cm_fn_009462bb(self + 0x18u + 0u, 0xeu, 0x96u, 0x0068d040u);

    CM_FS(u32, 0) = seh.prev;
}
