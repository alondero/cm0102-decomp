#include "cmre.h"

#define G_9cf858 CM_GLOBAL(u32, 0x009cf858)
#define G_9cf85c CM_GLOBAL(u32, 0x009cf85c)
#define G_9cfe30 CM_GLOBAL(u32, 0x009cfe30)
#define G_9cfe34 CM_GLOBAL(u32, 0x009cfe34)
#define G_9cfe38 CM_GLOBAL(u32, 0x009cfe38)
#define G_9cfe3c CM_GLOBAL(u32, 0x009cfe3c)
#define G_9cfe40 CM_GLOBAL(u32, 0x009cfe40)
#define G_9cfe44 CM_GLOBAL(u32, 0x009cfe44)
#define G_9cfe48 CM_GLOBAL(u32, 0x009cfe48)
#define G_9cfe4c CM_GLOBAL(u32, 0x009cfe4c)
#define G_9cfe50 CM_GLOBAL(u32, 0x009cfe50)
#define G_9cfe54 CM_GLOBAL(u32, 0x009cfe54)
#define G_ae2c92 CM_GLOBAL(u16, 0x00ae2c92)
#define G_ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define G_b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_00766e10(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28, u32 arg_2c, u32 arg_30);
extern u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28);
extern char *cm_fn_0090d130(void);
extern u32 * CM_THISCALL cm_fn_0092fe80(u32 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

struct cm_seh_frame { void *prev; void *handler; u32 state; };

#define OBJPTR ((u32)(*(u32 *)((u32)param_1 + 4)))

void CM_THISCALL cm_fn_007cd1c0(int param_1, u32 param_2)
{
    volatile struct cm_seh_frame frame;
    u8 scratch[512];
    union { u16 w; u32 dw; } local_210;
    u16 ax;
    u32 edi_val;
    u32 ctr;
    u32 a;
    u32 r;

    /* push -1; push 0x95f98c; push fs:[0]; fs:[0] = esp */
    frame.prev = CM_FS(void *, 0);
    frame.handler = (void *)0x0095f98c;
    frame.state = (u32)-1;
    CM_FS(void *, 0) = (void *)&frame;

    ax = G_ae2c92;
    edi_val = *(u32 *)((u32)param_1 + 0x13);
    local_210.w = ax;
    *(u16 *)((u32)param_1 + 8) = 10;

    a = (u32)cm_fn_00944e46(0x28);
    *(u32 *)((u32)param_1 + 4) = a;
    ctr = 0;
    if (a == 0) {
        char *s;
        cm_fn_00944cff((u32)0x00a78680, 0, 0, scratch + 256, scratch);
        s = cm_fn_0090d130();
        cm_fn_00944c9f((u32)&G_ae24d0, (u32)0x009870e8, (u32)s,
                       (u32)(scratch + 256), (u32)scratch, 0x6b);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_ae24d0, 0);
        G_b67a34 = 0;
    } else {
        a = (u32)cm_fn_00944cf1(0x25d);
        frame.state = 0;
        if (a == 0) {
            r = 0;
        } else {
            ctr = 1;
            r = cm_fn_008af520((void *)a, edi_val, 0, G_9cfe30, 0, 0,
                               G_9cf858, 1, 0, param_2, 1);
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x0) = r;

        a = (u32)cm_fn_00944cf1(0x2b);
        frame.state = 1;
        if (a == 0) {
            r = 0;
        } else {
            r = (u32)cm_fn_0092fe80((u32 *)a, edi_val, ctr, G_9cfe34, 0xbb8, 0,
                                    G_9cf858, 1, 0, 0, param_2, 1);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x4) = r;

        a = (u32)cm_fn_00944cf1(0x2b);
        frame.state = 2;
        if (a == 0) {
            r = 0;
        } else {
            r = (u32)cm_fn_0092fe80((u32 *)a, edi_val, ctr, G_9cfe38, 0x7d0, 0,
                                    G_9cf858, 1, 0, 0, param_2, 1);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x8) = r;

        a = (u32)cm_fn_00944cf1(0x2b);
        frame.state = 3;
        if (a == 0) {
            r = 0;
        } else {
            r = (u32)cm_fn_0092fe80((u32 *)a, edi_val, ctr, G_9cfe3c, 0x9c4, 0x4b0,
                                    G_9cf858, 1, 0, 0, param_2, 0);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0xc) = r;

        a = (u32)cm_fn_00944cf1(0xa2);
        frame.state = 4;
        if (a == 0) {
            r = 0;
        } else {
            r = cm_fn_00766e10((void *)a, edi_val, ctr, G_9cfe40, 2000, 1000,
                               G_9cf858, 6, local_210.dw, 4, local_210.dw + 1,
                               param_2, 0);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x10) = r;

        a = (u32)cm_fn_00944cf1(0x25d);
        frame.state = 5;
        if (a == 0) {
            r = 0;
        } else {
            r = cm_fn_008af520((void *)a, edi_val, ctr, G_9cfe44, 0, 0,
                               G_9cf85c, 1, 0, param_2, 1);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x14) = r;

        a = (u32)cm_fn_00944cf1(0x2b);
        frame.state = 6;
        if (a == 0) {
            r = 0;
        } else {
            r = (u32)cm_fn_0092fe80((u32 *)a, edi_val, ctr, G_9cfe48, 0x7d0, 0,
                                    G_9cf85c, 1, 0, 0, param_2, 1);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x18) = r;

        a = (u32)cm_fn_00944cf1(0x2b);
        frame.state = 7;
        if (a == 0) {
            r = 0;
        } else {
            r = (u32)cm_fn_0092fe80((u32 *)a, edi_val, ctr, G_9cfe4c, 0x3e8, 0,
                                    G_9cf85c, 1, 0, 0, param_2, 1);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x1c) = r;

        a = (u32)cm_fn_00944cf1(0x2b);
        frame.state = 8;
        if (a == 0) {
            r = 0;
        } else {
            r = (u32)cm_fn_0092fe80((u32 *)a, edi_val, ctr, G_9cfe50, 0x5dc, 0x3e8,
                                    G_9cf85c, 1, 0, 0, param_2, 0);
            ctr++;
        }
        frame.state = (u32)-1;
        *(u32 *)(OBJPTR + 0x20) = r;

        a = (u32)cm_fn_00944cf1(0xa2);
        frame.state = 9;
        if (a == 0) {
            r = 0;
        } else {
            r = cm_fn_00766e10((void *)a, edi_val, ctr, G_9cfe54, 1000, 500,
                               G_9cf85c, 6, local_210.dw, 4, local_210.dw + 1,
                               param_2, 0);
        }
        *(u32 *)(OBJPTR + 0x24) = r;
    }

    CM_FS(void *, 0) = frame.prev;
    return;
}
