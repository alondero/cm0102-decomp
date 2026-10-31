#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x10a (byte) read into BL; 0x19e object reference probed through
   0x006c38c0 (returns a flag word); 0x27 (byte) flag; 0x6d a pointer whose
   byte fields at +0x17, +0x18 and +0x19 are passed to 0x0090cfc0. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_U8(off) (*(volatile u8 *)(self + (off)))

extern u32 CM_THISCALL cm_fn_006c38c0(u32 ecx_obj, u32 arg);
extern int cm_fn_0090cfc0(int param_1);

/* Reads the object reference at 0x19e; calls 0x006c38c0 with it as ECX and the
   object as the one stack argument when it is non-null, else yields zero. */
static u32 probe_19e(u32 self)
{
    u32 obj = OBJ_U32(0x19e);

    if (obj == 0) {
        return 0u;
    }
    return cm_fn_006c38c0(obj, self);
}

u32 CM_THISCALL cm_fn_00715f50(u32 self, u8 arg)
{
    u32 ret;
    u32 ptr;
    s8 val;
    int r1;
    int r2;
    int x;
    long long prod;
    u32 q;
    u8 bl;

    ret = probe_19e(self);
    if ((ret & 1u) != 0) {
        return (ret & 0xffffff00u) | arg;
    }

    bl = OBJ_U8(0x10a);
    x = cm_fn_0090cfc0(0x15);
    if ((s32)((s8)bl * 2) >= (s32)x) {
        return ((u32)x & 0xffffff00u) | arg;
    }

    ret = probe_19e(self);
    if ((ret & 0x80u) != 0) {
        bl = arg;
    } else {
        ptr = OBJ_U32(0x6d);
        val = *(volatile s8 *)(ptr + 0x18u);
        r1 = cm_fn_0090cfc0(val);
        r2 = cm_fn_0090cfc0(0x29);
        if (r1 > r2) {
            bl = (u8)(((OBJ_U8(0x27) != 0) ? 6u : 0u) + 1u);
        } else {
            bl = arg;
        }
    }

    ret = probe_19e(self);
    if ((ret & 0x200u) != 0) {
        goto add_arg;
    }
    ret = probe_19e(self);
    if ((ret & 0x100u) != 0) {
        goto add_arg;
    }
    ret = probe_19e(self);
    if ((ret & 0x400u) != 0) {
        goto add_arg;
    }
    ptr = OBJ_U32(0x6d);
    val = *(volatile s8 *)(ptr + 0x19u);
    r1 = cm_fn_0090cfc0(val);
    r2 = cm_fn_0090cfc0(0x29);
    if (r1 > r2) {
        bl = (u8)(bl + 4u);
        goto after_add;
    }

add_arg:
    bl = (u8)(bl + arg);

after_add:
    ret = probe_19e(self);
    if ((ret & 0x800u) != 0) {
        goto add_arg2;
    }
    ptr = OBJ_U32(0x6d);
    val = *(volatile s8 *)(ptr + 0x17u);
    r1 = cm_fn_0090cfc0(val);
    r2 = cm_fn_0090cfc0(0x29);
    if (r1 > r2) {
        bl = (u8)(bl + ((OBJ_U8(0x27) != 0) ? 1u : 7u));
        goto finish;
    }

add_arg2:
    bl = (u8)(bl + arg);

finish:
    prod = (long long)(s32)0x55555556 * (long long)(s32)(s8)bl;
    q = (u32)(s32)(prod >> 32);
    return q + (q >> 31);
}
