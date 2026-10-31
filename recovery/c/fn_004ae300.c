/* WIP: one canonical failure, coverage0.3669/inc0. Constructor-proven
   vtable00969934 slots009699b4/b8 target00522910/00522c50, absent inventory. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern void CM_FASTCALL cm_fn_005a53c0(u32);
extern u32 CM_THISCALL cm_fn_004a4850(u32, u32);

u32 CM_THISCALL cm_fn_004ae300(u32 self, volatile u32 target,
                              volatile u32 first, volatile u32 second,
                              volatile u32 third)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[8];
    u32 local = (u32)scratch;
    u32 pointer, owner, table, address, argument, value, allocation;
    u32 subobject, pair, destination, result;
    u8 index, flag;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x009568c8;
    CM_FS(u32, 0) = (u32)&frame;
    pointer = D(self + 0x502);
    index = B(pointer + 0x30);
    while ((s8)index >= -1) {
        pointer = D(self + 0x502);
        if (index == 0xff) {
            owner = pointer;
        } else {
            pointer = D(pointer + 0xc);
            owner = D(pointer + (u32)(s32)(s8)index * 4u);
        }
        argument = target;
        table = D(owner);
        address = D(table + 0x80);
        value = ((u32 (CM_THISCALL *)(u32, u32))address)(owner, argument);
        if (value == 0) {
            index = (u8)(index - 1u);
            continue;
        }
        allocation = (u32)cm_fn_00944cf1(0x76);
        D(local) = allocation;
        frame.state = 0;
        if (allocation != 0) {
            for (pair = 0; pair != 4; ++pair) {
                subobject = allocation + 0x19u + pair * 20u;
                if (pair != 0)
                    *(volatile u8 *)&frame.state = (u8)(pair * 2u);
                D(local + 4) = subobject;
                cm_fn_0054a110(subobject + 4);
                *(volatile u8 *)&frame.state = (u8)(pair * 2u + 1u);
                cm_fn_0054a110(subobject + 0xc);
            }
        }
        argument = target;
        D(self + 0x566) = allocation;
        table = D(owner);
        frame.state = 0xffffffffu;
        address = D(table + 0x84);
        value = ((u32 (CM_THISCALL *)(u32, u32))address)(owner, argument);
        if (value != 0) {
            pointer = D(self + 0x566);
            destination = first;
            flag = B(pointer + 0x74);
            B(destination) = flag;
            pointer = D(self + 0x566);
            destination = second;
            flag = B(pointer + 0x73);
            B(destination) = flag;
            pointer = D(self + 0x566);
            destination = third;
            flag = B(pointer + 0x75);
            B(destination) = flag;
            pointer = D(self + 0x566);
            if (pointer != 0) {
                cm_fn_005a53c0(pointer + 0x19);
                cm_fn_00944c94(pointer);
            }
            D(self + 0x566) = 0;
            result = 1;
            goto done;
        }
        pointer = D(self + 0x566);
        if (pointer != 0) {
            cm_fn_005a53c0(pointer + 0x19);
            cm_fn_00944c94(pointer);
        }
        D(self + 0x566) = 0;
        break;
    }
    argument = target;
    pointer = cm_fn_004a4850(self, argument);
    if (pointer != 0) {
        destination = first;
        flag = B(pointer + 0x74);
        B(destination) = flag;
        destination = second;
        flag = B(pointer + 0x73);
        B(destination) = flag;
        flag = B(pointer + 0x75);
        destination = third;
        B(destination) = flag;
    }
    result = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
