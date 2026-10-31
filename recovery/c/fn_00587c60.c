#include "cmre.h"

/* Preferred-image VA 0x00587c60, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep the original RNG call order and late field reads. The null first
   pointer returns the saved SEH predecessor in full EAX. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007875e0(u32, u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00547d60(u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

u32 CM_CDECL cm_fn_00587c60(u32 first, u32 second, volatile u32 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe0];
    u32 local = (u32)scratch, prev, result, pointer, value, manager, owner;
    u32 choice, profile;
    s8 selected;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00958b7b;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    result = prev;
    if (first == 0) goto done;
    result = D(first + 0x61);
    if (result == 0) goto done;
    if (second == 0) goto done;
    result = D(first + 0x39);
    if (result != 0) {
        manager = D(0x00b7431c);
        result = cm_fn_007875e0(manager, result);
        if (result == 0) goto done;
    }
    cm_fn_0077bbe0(local, 0xbc7, 0);
    value = D(first);
    frame.state = 0;
    cm_fn_00785790(local, 0, value);
    pointer = D(first + 4);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 1, value);
    pointer = D(first + 8);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 2, value);
    pointer = D(first + 12);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 3, value);
    value = D(second);
    cm_fn_00785790(local, 4, value);
    pointer = D(second + 4);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 5, value);
    pointer = D(second + 8);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 6, value);
    pointer = D(second + 12);
    value = D(pointer + 0x33);
    cm_fn_00785790(local, 7, value);
    selected = (s8)mode;
    cm_fn_00785790(local, 8, (u32)(s32)selected);
    if (selected == 0) {
        if (B(first + 0x5b) == 1) {
            value = cm_fn_0090cfc0(4);
            if (value != 0) { choice = 0; goto chosen; }
        }
        if ((s8)B(first + 0x5b) < 5) {
            owner = D(first + 0x39);
            value = cm_fn_00547d60(first, owner);
            if (value != 0) {
                choice = cm_fn_0090cfc0(4);
                if (choice == 0) goto chosen;
            }
            if ((s8)B(first + 0x5b) < 5) {
                value = cm_fn_0090cfc0(4);
                if (value != 0) { choice = 1; goto chosen; }
            }
        }
        if ((s8)B(first + 0x5b) > 15) {
            value = cm_fn_0090cfc0(4);
            if (value != 0) { choice = 2; goto chosen; }
        }
        choice = 3;
    } else if (selected == 2) {
        if ((s8)B(first + 0x5b) > 15) {
            if ((s8)B(second + 0x5b) < 5) { choice = 0; goto chosen; }
            profile = D(second + 0x61);
            if (profile != 0) {
                if ((s8)B(profile + 0x44) < 5) { choice = 1; goto chosen; }
                if ((s8)B(profile + 0x3f) < 5) { choice = 2; goto chosen; }
            }
        }
        choice = cm_fn_0090cfc0(3) + 3u;
    } else {
        choice = cm_fn_0090cfc0(3);
    }
chosen:
    cm_fn_00785790(local, 9, choice);
    owner = D(first + 0x39);
    manager = D(0x00b7431c);
    result = cm_fn_007861e0(manager, local, owner);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
