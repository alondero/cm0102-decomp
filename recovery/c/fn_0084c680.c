/* Translation of fn_0084c680 (preferred-image VA 0x0084c680, image base
   0x00400000, module simulated_stats.cpp).  Checked against the 324-byte
   listing; callees stay explicit original boundaries. */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(void);
extern int *cm_fn_009326e0(u32 param_1, u8 *param_2, int param_3, int param_4, u32 param_5);
extern void cm_fn_00932b00(int *param_1);
extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_0084c680(int *param_1, u32 param_2)
{
    /* One scratch allocation: local_204 at L+0 (4 bytes), buf200 at L+4
       (256 bytes), buf100 at L+0x104 (256 bytes); total 0x204 bytes. */
    u8 scratch[0x204];
    volatile int *local_204 = (volatile int *)(scratch + 0x0);
    u8 *buf200 = scratch + 0x4;
    u8 *buf100 = scratch + 0x104;
    volatile int *vparam = (volatile int *)param_1;
    u32 uVar4;
    u8 *p2;
    u8 *p3;
    int iVar1;

    *local_204 = (int)cm_fn_009326e0(param_2, (u8 *)0x00a827c0, 1, 0x16, 4);
    if (*local_204 == 0) {
        cm_fn_00944cff((u32)0x00a8278c, 0, 0, (void *)buf100, (void *)buf200);
        p2 = buf100;
        p3 = buf200;
        uVar4 = 0x209;
    } else {
        iVar1 = (int)cm_fn_00932e10((u32)param_1, 4, 1, (u32)*local_204);
        if (iVar1 == 1) {
            iVar1 = (int)cm_fn_00932e10((u32)vparam[1], 0x44, (u32)vparam[0], (u32)*local_204);
            if (iVar1 == *vparam) {
                cm_fn_00932b00((int *)local_204);
                return;
            }
            cm_fn_00944cff((u32)0x00a8278c, 0, 0, (void *)buf200, (void *)buf100);
            p2 = buf200;
            p3 = buf100;
            uVar4 = 0x218;
        } else {
            cm_fn_00944cff((u32)0x00a8278c, 0, 0, (void *)buf200, (void *)buf100);
            p2 = buf200;
            p3 = buf100;
            uVar4 = 0x210;
        }
    }
    /* cm_fn_0090d130 has signature (void); the three pushes before its call
       belong to the following cm_fn_00944c9f (deferred cdecl cleanup). */
    cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, (u32)cm_fn_0090d130(),
                   (u32)p2, (u32)p3, uVar4);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    DAT_00b67a34 = 0;
}
