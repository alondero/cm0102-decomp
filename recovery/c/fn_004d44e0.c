#include "cmre.h"

/* Cdecl, two stack arguments: a record pointer and a byte. Returns a byte. */
extern u8 CM_CDECL cm_fn_00544410(u32 record, u8 value);
/* Stdcall, two stack arguments (selector byte, object pointer); returns a dword. */
extern u32 CM_STDCALL cm_fn_004cb990(s8 selector, volatile u8 *object);

/* Thiscall with one stack argument (ret 4) returning a byte result code. ECX is the
   receiver, arg1 is the object P. Fields are used by offset: +0x24 dword, +0x28 byte,
   +0x39 dword, +0x3d byte, +0x61 dword, +0x69 pointer to a record whose bytes at +0x10
   and +0x11 are passed to 0x00544410. Returns the first matching result code. */
u8 CM_THISCALL cm_fn_004d44e0(u8 *param_1, u8 *arg1)
{
    u8 *y;
    u32 r;
    u8 c3;
    u8 c28;
    u32 z;
    u32 w;

    (void)param_1;
    if (*(u32 *)(arg1 + 0x61) != 0) {
        y = *(u8 **)(arg1 + 0x69);
        if (y == 0) {
            return 0x0b;
        }
        r = cm_fn_00544410((u32)y, *(u8 *)(y + 0x10));
        if ((s8)(u8)r >= 0x0a) {
            return 0x0f;
        }
        y = *(u8 **)(arg1 + 0x69);
        r = cm_fn_00544410((u32)y, *(u8 *)(y + 0x11));
        if ((s8)(u8)r < 0x0a) {
            return 0x0b;
        }
        return 0x0f;
    }

    y = *(u8 **)(arg1 + 0x69);
    if (y == 0) {
        return 0x09;
    }
    c3 = *(u8 *)(arg1 + 0x3d);
    if (c3 == 5 || c3 == 0x0c) {
        return 0x06;
    }
    c28 = *(u8 *)(arg1 + 0x28);
    if (c28 == 5 || c28 == 0x0c) {
        return 0x06;
    }
    z = *(u32 *)(arg1 + 0x39);
    if (z != 0) {
        if (cm_fn_004cb990((s8)*(u8 *)(arg1 + 0x3d), arg1) != 0) {
            return *(u8 *)(arg1 + 0x3d);
        }
    }
    w = *(u32 *)(arg1 + 0x24);
    if (w != 0) {
        if (cm_fn_004cb990((s8)*(u8 *)(arg1 + 0x28), arg1) != 0) {
            return *(u8 *)(arg1 + 0x28);
        }
    }
    c3 = *(u8 *)(arg1 + 0x3d);
    if (cm_fn_004cb990((s8)c3, arg1) != 0) {
        return *(u8 *)(arg1 + 0x3d);
    }

    y = *(u8 **)(arg1 + 0x69);
    r = cm_fn_00544410((u32)y, *(u8 *)(y + 0x10));
    if ((s8)(u8)r >= 0x0f) {
        return 0x08;
    }
    y = *(u8 **)(arg1 + 0x69);
    r = cm_fn_00544410((u32)y, *(u8 *)(y + 0x11));
    if ((s8)(u8)r < 0x0f) {
        return 0x09;
    }
    return 0x08;
}
