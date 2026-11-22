/* Preferred-image VA 0x007ad7c0, image base 0x00400000.
   Pinned binary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 4 pops one stack argument; ECX is read at 0x007ad7d0 before being written,
   so the entry convention is thiscall with ECX as param_1.
   All callees are cdecl (caller cleans every push group).  cm_fn_0090d130 has
   signature (void) at 0x007ad84e/0x007ad8cc/...: the three pushes made before
   each of those calls are arguments 4..6 of the following cm_fn_00944c9f call,
   per the right-to-left push / deferred-cleanup pattern (single add esp,0x24).
   L is ESP immediately after `sub esp,0x204` at 0x007ad7c4.  With two saved
   registers (push ebx, push edi) local_204 = L+0, local_200 = L+4,
    local_100 = L+0x104. */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define S_FMT_PATH   ((u32)0x00a6bf9c)
#define S_RETIRE_DAT ((u8 *)0x00a701a4)
#define S_ERROR      ((const char *)0x009870e0)
#define S_FMT_LINE   ((u32)0x009870e8)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(void);
extern int *cm_fn_009326e0(u32 param_1, u8 *param_2, int param_3, int param_4, u32 param_5);
extern void cm_fn_00932b00(int *param_1);
extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_007ad7c0(int param_1, int param_2)
{
    struct {
        int stream;
        u8 buffers[512];
    } frame;
#define local_204 frame.stream
#define local_200 (frame.buffers)
#define local_100 (frame.buffers + 256)

    if (param_2 == 0) {
        /* 0x007ad7d4..0x007ad802 */
        cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_100, (void *)local_200);
        cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                       (u32)cm_fn_0090d130(),
                       (u32)local_100, (u32)local_200, 0x1a46u);
        cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        return;
    }

    local_204 = (int)cm_fn_009326e0((u32)param_2, S_RETIRE_DAT, 1, 0x16, 4);
    if (local_204 == 0) {
        /* 0x007ad820..0x007ad84d */
        cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
        cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                       (u32)cm_fn_0090d130(),
                       (u32)local_200, (u32)local_100, 0x1a4eu);
        cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        return;
    }

    /* 0x007ad887..0x007ad904: this+8 / 4 / 1 */
    if (cm_fn_00932e10((u32)(param_1 + 8), 4, 1, (u32)local_204) != 1) {
        cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
        cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                       (u32)cm_fn_0090d130(),
                       (u32)local_200, (u32)local_100, 0x1a55u);
        cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        cm_fn_00932b00(&local_204);
    }
    if (*(u32 *)(param_1 + 8) != 0) {
        if (cm_fn_00932e10(*(u32 *)(param_1 + 4), 0x1a,
                           *(u32 *)(param_1 + 8), (u32)local_204)
            != *(u32 *)(param_1 + 8)) {
            cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
            cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                           (u32)cm_fn_0090d130(),
                           (u32)local_200, (u32)local_100, 0x1a5cu);
            cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
            DAT_00b67a34 = 0;
            cm_fn_00932b00(&local_204);
        }
    }

    /* 0x007ad98e..0x007ada0e: this+0x14 / 4 / 1 */
    if (cm_fn_00932e10((u32)(param_1 + 0x14), 4, 1, (u32)local_204) != 1) {
        cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
        cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                       (u32)cm_fn_0090d130(),
                       (u32)local_200, (u32)local_100, 0x1a63u);
        cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        cm_fn_00932b00(&local_204);
    }
    if (*(u32 *)(param_1 + 0x14) != 0) {
        if (cm_fn_00932e10(*(u32 *)(param_1 + 0x10), 0xc,
                           *(u32 *)(param_1 + 0x14), (u32)local_204)
            != *(u32 *)(param_1 + 0x14)) {
            cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
            cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                           (u32)cm_fn_0090d130(),
                           (u32)local_200, (u32)local_100, 0x1a6au);
            cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
            DAT_00b67a34 = 0;
            cm_fn_00932b00(&local_204);
        }
    }

    /* 0x007ada98..0x007adb18: this+0x20 / 4 / 1 */
    if (cm_fn_00932e10((u32)(param_1 + 0x20), 4, 1, (u32)local_204) != 1) {
        cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
        cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                       (u32)cm_fn_0090d130(),
                       (u32)local_200, (u32)local_100, 0x1a71u);
        cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        cm_fn_00932b00(&local_204);
    }
    if (*(u32 *)(param_1 + 0x20) != 0) {
        if (cm_fn_00932e10(*(u32 *)(param_1 + 0x1c), 0x10,
                           *(u32 *)(param_1 + 0x20), (u32)local_204)
            != *(u32 *)(param_1 + 0x20)) {
            cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
            cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                           (u32)cm_fn_0090d130(),
                           (u32)local_200, (u32)local_100, 0x1a78u);
            cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
            DAT_00b67a34 = 0;
            cm_fn_00932b00(&local_204);
        }
    }

    /* 0x007adba2..0x007adc22: this+0x40 / 4 / 1 */
    if (cm_fn_00932e10((u32)(param_1 + 0x40), 4, 1, (u32)local_204) != 1) {
        cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
        cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                       (u32)cm_fn_0090d130(),
                       (u32)local_200, (u32)local_100, 0x1a7fu);
        cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        cm_fn_00932b00(&local_204);
    }
    if (*(u32 *)(param_1 + 0x40) != 0) {
        if (cm_fn_00932e10(*(u32 *)(param_1 + 0x3c), 0x10,
                           *(u32 *)(param_1 + 0x40), (u32)local_204)
            != *(u32 *)(param_1 + 0x40)) {
            cm_fn_00944cff(S_FMT_PATH, 0, 0, (void *)local_200, (void *)local_100);
            cm_fn_00944c9f((u32)&DAT_00ae24d0, S_FMT_LINE,
                           (u32)cm_fn_0090d130(),
                           (u32)local_200, (u32)local_100, 0x1a86u);
            cm_fn_005e8290(S_ERROR, (const char *)&DAT_00ae24d0, 0);
            DAT_00b67a34 = 0;
            cm_fn_00932b00(&local_204);
        }
    }

    /* 0x007adcac..0x007adcbc */
    cm_fn_00932b00(&local_204);
}
#undef local_204
#undef local_200
#undef local_100
