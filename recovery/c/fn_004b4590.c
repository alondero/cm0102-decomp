#include "cmre.h"

/* Entry stack layout, by byte offset from the entry ESP (return address at +0):
   arg1 +0x04, arg2 +0x08, arg3 +0x0c, arg4 (char) +0x10, arg5 +0x14 and
   arg6 +0x18. Every thiscall in the listing takes arg6 in ECX, so arg6 is the
   receiver object. The 100-byte scratch buffer is the frame area at entry
   ESP-0x64 (pushes sit below it).
   Record base: rec = arg1 * 581 (0x245), indexed with the DAT global at
   0x00ae23b4. Fields of the arg3 object by offset: +0x00 DWORD (arg5 != 0 path
   with arg4 != 0xfe), +0x37 and +0x52 signed bytes. */

#define DAT_00ae23b4 CM_GLOBAL(volatile s32, 0x00ae23b4)

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern char *cm_fn_004b4850(char *param_1, int param_2, short param_3, char param_4);
extern u32 cm_fn_004ba8f0(short param_1, char param_2);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);

void cm_fn_004b4590(u32 arg1, u32 arg2, u32 arg3, u8 arg4, u32 arg5, u32 arg6)
{
    u8 buf[100];
    u32 rec;
    u32 fmt;
    u32 addr;
    u32 w;
    s32 dat_a;
    s32 dat_b;
    s32 dat_c;
    s32 c1;
    s32 c2;
    s32 c3;

    /* Only codes 6, 7 and 0xfe reach the body; the rest return at once. */
    if (arg4 != 0x06 && arg4 != 0x07 && arg4 != 0xfe) {
        return;
    }

    rec = arg1 * 581u;

    if (arg5 != 0) {
        cm_fn_004b4850((char *)buf, 100, (short)arg2, 0);
        c2 = (s8)(u8)cm_fn_004ba8f0((short)arg2, 0);
        if (arg4 == 0xfe) {
            fmt = 0x009a3fd0u;
        } else {
            fmt = 0x009a3ed4u;
        }
        dat_a = DAT_00ae23b4;
        dat_b = DAT_00ae23b4;
        c3 = (s8)*(volatile s8 *)(arg3 + 0x37);
        c1 = (s8)*(volatile s8 *)(rec + (u32)dat_b + 0x52);
        addr = rec + (u32)dat_a + 0x38;
        cm_fn_0066f4e0(0x00de1f64u, fmt, c1, c1, c2, c2, c3, c3, addr, (u32)buf, arg3 + 4);
        cm_fn_004ae660((void *)arg6, 0x00de1f64u);
        dat_c = DAT_00ae23b4;
        cm_fn_004ae8a0((int)arg6, (u8 *)(rec + (u32)dat_c + 0x38), 0x7d5, (int)(rec + (u32)dat_c));
        cm_fn_004ae8a0((int)arg6, (u8 *)(arg3 + 4), 0x7d0, (int)arg3);
        if (arg4 == 0xfe) {
            cm_fn_004ae8a0((int)arg6, buf, 0x7e5,
                           (int)((((u32)(s32)(s16)arg2) | 0xfffff800u) << 11));
        } else {
            w = *(volatile u32 *)arg3;
            cm_fn_004ae8a0((int)arg6, buf, 0x7e5,
                           (int)(((arg2 & 0x7ffu) | (w << 11)) << 11));
        }
        return;
    }

    if (arg4 == 0xfe) {
        fmt = 0x009a3f68u;
    } else {
        fmt = 0x009a3e74u;
    }
    dat_a = DAT_00ae23b4;
    c2 = (s8)*(volatile s8 *)(arg3 + 0x52);
    c1 = (s8)*(volatile s8 *)(rec + (u32)dat_a + 0x52);
    addr = rec + (u32)dat_a + 0x38;
    cm_fn_0066f4e0(0x00de1f64u, fmt, c1, c1, c2, c2, addr, arg3 + 0x38);
    cm_fn_004ae660((void *)arg6, 0x00de1f64u);
    dat_c = DAT_00ae23b4;
    cm_fn_004ae8a0((int)arg6, (u8 *)(rec + (u32)dat_c + 0x38), 0x7d5, (int)(rec + (u32)dat_c));
    cm_fn_004ae8a0((int)arg6, (u8 *)(arg3 + 0x38), 0x7d0, (int)arg3);
}
