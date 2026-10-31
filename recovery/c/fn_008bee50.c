#include "cmre.h"

/* Preferred-image VA 0x008bee50, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original local records share one scratch area; untouched bitfield bits
   and constructor gaps remain uninitialized. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0085e410(u32);
extern u32 CM_THISCALL cm_fn_008e5680(u32);
extern void CM_THISCALL cm_fn_008e5710(u32);
extern void CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern void CM_THISCALL cm_fn_0085e590(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32, u32, s16);
extern void CM_THISCALL cm_fn_008be9d0(u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_008bee50(u32 self, u32 source, u32 arg2, u32 arg3,
                             u32 arg4, u32 arg5)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x9c];
    u32 local = (u32)scratch, prev, clubs, people, club, person, id, value, other;
    u32 value2, result, allocated;
    u8 byte0, byte1, byte2;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00963aee;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0085e410(local + 0x4c);
    frame.state = 0;
    cm_fn_008e5680(local);
    clubs = D(0x00ae23b4);
    *(volatile u8 *)&frame.state = 1;
    id = D(source + 12);
    D(local + 12) = id;
    byte0 = B(source + 0x11);
    B(local + 0x11) = byte0;
    people = D(0x00ae23bc);
    person = people + id * 110u;
    id = D(source + 8);
    clubs += id;
    D(local + 8) = id;
    byte0 = B(source + 0x10);
    B(local + 0x10) = byte0;
    club = clubs + id * 580u;
    value = D(source);
    D(local) = value;
    value = D(source + 4);
    D(local + 4) = value;
    cm_fn_00417c10(local + 0x12, local + 0x44, source + 0x12);
    value = D(source + 0x1b);
    byte0 = B(source + 0x1f);
    byte1 = B(source + 0x1a);
    D(local + 0x1b) = value;
    value = D(source + 0x24);
    B(local + 0x1f) = byte0;
    value2 = D(source + 0x28);
    D(local + 0x24) = value;
    byte2 = B(source + 0x2d);
    D(local + 0x28) = value2;
    byte0 = B(source + 0x2e);
    B(local + 0x2d) = byte2;
    byte2 = B(source + 0x30);
    B(local + 0x1a) = byte1;
    value = D(source + 0x20);
    B(local + 0x2e) = byte0;
    byte0 = B(local + 0x30);
    D(local + 0x20) = value;
    byte1 = B(source + 0x2c);
    B(local + 0x2c) = byte1;
    byte1 = B(source + 0x2f);
    B(local + 0x2f) = byte1;
    value = D(source + 0x31);
    B(local + 0x30) = (u8)((byte0 & 0xf0u) | (byte2 & 15u));
    other = D(local + 0x31);
    D(local + 0x31) = (other & 0xfffffff0u) | (value & 15u);
    value = D(source + 0x35);
    D(local + 0x35) = value;
    byte1 = B(source + 0x39);
    B(local + 0x39) = byte1;
    value = D(source + 0x3a);
    D(local + 0x3a) = value;
    byte0 = B(source + 0x40);
    byte1 = B(source + 0x3e);
    byte2 = B(source + 0x3f);
    B(local + 0x40) = byte0;
    B(local + 0x3e) = byte1;
    B(local + 0x3f) = byte2;
    D(local + 4) = 0;
    D(local) = 0;
    cm_fn_0085e590(local + 0x4c, person, club, arg2, arg3, arg4, arg5);
    value = D(self + 0x946);
    if (value == 0) {
        allocated = cm_fn_00944cf1(14);
        D(local + 0x44) = allocated;
        *(volatile u8 *)&frame.state = 2;
        value = allocated == 0 ? 0 : cm_fn_0068d020(allocated);
        *(volatile u8 *)&frame.state = 1;
        D(self + 0x946) = value;
    }
    value = D(self + 0x946);
    cm_fn_0068d0d0(value, local + 0x4c, 80);
    cm_fn_008be9d0(self, source, local, 0);
    result = D(local + 0x20);
    *(volatile u8 *)&frame.state = 0;
    cm_fn_008e5710(local);
    CM_FS(u32, 0) = frame.prev;
    return result;
}
