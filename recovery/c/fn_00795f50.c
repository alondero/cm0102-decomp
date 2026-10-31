#include "cmre.h"

/* Local frame of this routine. L is the ESP right after `sub esp, 0x200` and
   four register pushes (entry ESP - 0x210). Offsets relative to L:
   0x10 byte: low byte of the second 0x00785830 result (switch selector);
   0x14 dword: receiver ECX saved on entry;
   0x18 dword: value computed from 0x00ae23bc and the clamped index, written
        before the 0x0053e140 call and read back after the 0x00785830 calls;
   0x1c..0x20f: 500-byte text buffer, passed by address to 0x0053e140 and
        0x0066f4e0 / 0x004ae8a0 (ends at the entry ESP). */
#define FRAME_U8(off) (((u8 *)frame)[(off)])
#define FRAME_U32(off) (frame[(off) / 4])

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern u32 cm_fn_0053e140(int param_1, int param_2, int param_3, int param_4, u32 param_5, u32 param_6, int param_7);
extern u32 cm_fn_0053e990(int param_1, int param_2, int param_3, int param_4, int param_5);
extern char *cm_fn_0066f4e0(u32 dst, u32 fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern int CM_STDCALL cm_fn_00795b90(int param_1, int param_2, u8 param_3, char param_4);

u32 CM_THISCALL cm_fn_00795f50(u32 self, u32 arg1, u32 arg2, u32 arg3)
{
    u32 frame[0x210 / 4];
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    s32 idx;
    u32 ecx_value;
    u32 memval;
    u8 *buf;

    FRAME_U32(0x14) = self;

    /* Five calls on the object in arg1, indices 0..4. */
    r0 = (u32)cm_fn_00785830((void *)arg1, 0u);
    r1 = (u32)cm_fn_00785830((void *)arg1, 1u);
    FRAME_U8(0x10) = (u8)r1;
    r2 = (u32)cm_fn_00785830((void *)arg1, 2u);
    r3 = (u32)cm_fn_00785830((void *)arg1, 3u);
    r4 = (u32)cm_fn_00785830((void *)arg1, 4u);

    /* Clamp the signed index: negative, or above (count - 1), becomes 0. The
       count is read only when the index is non-negative. */
    idx = (s32)r0;
    if (idx < 0) {
        idx = 0;
    } else if (idx > (s32)((u32)CM_GLOBAL(s32, 0x00ae2364) - 1u)) {
        idx = 0;
    }

    ecx_value = CM_GLOBAL(u32, 0x00ae23bc) + (u32)idx * 110u;
    buf = (u8 *)frame + 0x1c;
    FRAME_U32(0x18) = ecx_value;
    cm_fn_0053e140((int)r2, (int)r3, (int)r4, (int)(u32)buf, 0x12cu, 1u, idx);

    if (arg2 == 0) {
        /* The listing reads the dword at frame+0x10, whose upper three bytes
           were never written in this routine. Only the low byte is passed:
           the callee's own listing reads only the byte at its third argument. */
        cm_fn_00795b90(0x00de1f64, 0x3e8, FRAME_U8(0x10), 0);
        return (u32)cm_fn_004ae660((void *)arg3, 0x00de1f64u);
    }

    switch ((s8)FRAME_U8(0x10)) {
    case 1:
        cm_fn_0066f4e0(0x00de1f64u, 0x00a69e4cu, 0x64u, (u32)buf);
        break;
    case 2:
        cm_fn_0066f4e0(0x00de1f64u, 0x00a69d9cu, (u32)buf);
        break;
    case 3:
        cm_fn_0066f4e0(0x00de1f64u, 0x00a69d24u, 0x64u, (u32)buf);
        break;
    case 4:
        cm_fn_0066f4e0(0x00de1f64u, 0x00a69f14u, 0x64u, (u32)buf);
        break;
    default:
        cm_fn_0066f4e0(0x00de1f64u, 0x00a69cbcu, (u32)buf);
        break;
    }

    cm_fn_004ae660((void *)arg3, 0x00de1f64u);
    memval = FRAME_U32(0x18);
    if (cm_fn_0053e990((int)memval, (int)r2, (int)r3, (int)r4, 0) == 0) {
        return 0u;
    }
    return cm_fn_004ae8a0((int)arg3, buf, 0x7db, (int)memval);
}
