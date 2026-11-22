#include "cmre.h"

#define DAT_009cf5dc CM_GLOBAL(u32, 0x009cf5dc)
#define DAT_009cf5e0 CM_GLOBAL(u32, 0x009cf5e0)
#define DAT_009cf5e4 CM_GLOBAL(u32, 0x009cf5e4)
#define DAT_009cf5e8 CM_GLOBAL(u32, 0x009cf5e8)
#define DAT_009cfa7c CM_GLOBAL(u32, 0x009cfa7c)
#define DAT_009cfa80 CM_GLOBAL(u32, 0x009cfa80)
#define DAT_009cfa84 CM_GLOBAL(u32, 0x009cfa84)
#define DAT_009cfa88 CM_GLOBAL(u32, 0x009cfa88)
#define DAT_009cfa8c CM_GLOBAL(u32, 0x009cfa8c)
#define DAT_009cfa90 CM_GLOBAL(u32, 0x009cfa90)
#define DAT_009cfa94 CM_GLOBAL(u32, 0x009cfa94)
#define DAT_009cfa98 CM_GLOBAL(u32, 0x009cfa98)
#define DAT_009cfa9c CM_GLOBAL(u32, 0x009cfa9c)
#define DAT_009cfaa0 CM_GLOBAL(u32, 0x009cfaa0)
#define DAT_009cfbac CM_GLOBAL(u32, 0x009cfbac)
#define DAT_009cfbb0 CM_GLOBAL(u32, 0x009cfbb0)
#define DAT_009cfbb4 CM_GLOBAL(u32, 0x009cfbb4)
#define DAT_009cfbb8 CM_GLOBAL(u32, 0x009cfbb8)
#define DAT_009cfbbc CM_GLOBAL(u32, 0x009cfbbc)
#define DAT_009cfbc0 CM_GLOBAL(u32, 0x009cfbc0)
#define DAT_009cfbc4 CM_GLOBAL(u32, 0x009cfbc4)
#define DAT_009cfbc8 CM_GLOBAL(u32, 0x009cfbc8)
#define DAT_009cfbcc CM_GLOBAL(u32, 0x009cfbcc)
#define DAT_009cfc14 CM_GLOBAL(u32, 0x009cfc14)
#define DAT_009cfc18 CM_GLOBAL(u32, 0x009cfc18)
#define DAT_009cfc1c CM_GLOBAL(u32, 0x009cfc1c)
#define DAT_009cfc20 CM_GLOBAL(u32, 0x009cfc20)
#define DAT_009cfc24 CM_GLOBAL(u32, 0x009cfc24)
#define DAT_009cfc28 CM_GLOBAL(u32, 0x009cfc28)
#define DAT_009cfc2c CM_GLOBAL(u32, 0x009cfc2c)
#define DAT_009cfc30 CM_GLOBAL(u32, 0x009cfc30)
#define WORD_00ae2c92 CM_GLOBAL(u16, 0x00ae2c92)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

#define S_ERR_PATH ((u32)0x00a7b020)
#define S_FMT ((u32)0x009870e8)
#define S_ERR ((u32)0x009870e0)
#define DEST_BUF ((u32)0x00ae24d0)
#define SEH_HANDLER ((u32)0x00960eca)

extern void cm_fn_005e8290(const char *param_1,const char *param_2,int param_3);
extern u32 CM_THISCALL cm_fn_00766e10(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28, u32 arg_2c, u32 arg_30);
extern u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28);
extern char * cm_fn_0090d130(void);
extern u32 * CM_THISCALL cm_fn_0092fe80(u32 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

void CM_THISCALL cm_fn_007f52b0(int param_1, u32 param_2)
{
    volatile struct { u32 prev; u32 handler; u32 state; } frame;
    u8 scratch[0x208];
    u32 edi_val;
    u16 w;
    u32 obj;
    u32 counter;
    u32 result;
    u32 *tp;
    u32 local210;

    frame.state = 0xffffffff;
    frame.handler = SEH_HANDLER;
    frame.prev = (u32)CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    w = WORD_00ae2c92;
    edi_val = *(volatile u32 *)((char *)param_1 + 0x13);
    *(volatile u16 *)(scratch + 4) = w;
    *(volatile u16 *)((char *)param_1 + 8) = 0x1b;

    obj = (u32)cm_fn_00944e46(0x6c);
    *(volatile u32 *)((char *)param_1 + 4) = obj;
    if (obj == 0) {
        cm_fn_00944cff(S_ERR_PATH, 0, 0, (void *)(scratch + 0x108), (void *)(scratch + 8));
        result = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(DEST_BUF, S_FMT, result, (u32)(scratch + 0x108), (u32)(scratch + 8), 0x88);
        cm_fn_005e8290((const char *)S_ERR, (const char *)DEST_BUF, 0);
        DAT_00b67a34 = 0;
    } else {
        counter = 0;

        obj = (u32)cm_fn_00944cf1(0x2b);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0;
        if (obj == 0) {
            result = 0;
        } else {
            counter = 1;
            result = (u32)cm_fn_0092fe80((u32 *)(u32)obj, edi_val, 0, DAT_009cfa7c, 6000, 2000, 0xffffffff, 1, 0, 0, param_2, 1);
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x0) = result;

        obj = (u32)cm_fn_00944cf1(0x2b);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 1;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)(u32)obj, edi_val, counter, DAT_009cfbbc, 5000, 0x5dc, 0xffffffff, 1, 0, 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x4) = result;

        obj = (u32)cm_fn_00944cf1(0x2b);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 2;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)(u32)obj, edi_val, counter, DAT_009cfa80, 4000, 1000, 0xffffffff, 1, 0, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x8) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 3;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfa94, 2000, 0, DAT_009cf5dc, 1, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0xc) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 4;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfc18, 3000, 1000, DAT_009cf5dc, 7, local210, 4, local210 + 1, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x10) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 5;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfa84, 0xdac, 500, DAT_009cf5dc, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x14) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 6;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfbc0, 0x9c4, 0, DAT_009cf5dc, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x18) = result;

        obj = (u32)cm_fn_00944cf1(0x2b);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 7;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)(u32)obj, edi_val, counter, DAT_009cfc14, 5000, 0x5dc, DAT_009cf5dc, 1, 0, 0, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x1c) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 8;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfbac, 4000, 500, DAT_009cf5dc, 1, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x20) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 9;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfa98, 1000, 0, DAT_009cf5e0, 1, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x24) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0xa;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfc20, 2000, 0, DAT_009cf5e0, 7, local210, 4, local210 + 1, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x28) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0xb;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfa88, 0x5dc, 0, DAT_009cf5e0, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x2c) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0xc;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfbc4, 1000, 0, DAT_009cf5e0, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x30) = result;

        obj = (u32)cm_fn_00944cf1(0x2b);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0xd;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)(u32)obj, edi_val, counter, DAT_009cfc1c, 3000, 0, DAT_009cf5e0, 1, 0, 0, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x34) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0xe;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfbb0, 3000, 0, DAT_009cf5e0, 1, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x38) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0xf;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfa9c, 0, 0, DAT_009cf5e4, 1, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x3c) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x10;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfc28, 500, 0, DAT_009cf5e4, 7, local210, 4, local210 + 1, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x40) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x11;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfa8c, 500, 0, DAT_009cf5e4, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x44) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x12;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfbc8, 200, 0, DAT_009cf5e4, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x48) = result;

        obj = (u32)cm_fn_00944cf1(0x2b);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x13;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)(u32)obj, edi_val, counter, DAT_009cfc24, 1000, 0, DAT_009cf5e4, 1, 0, 0, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x4c) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x14;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfbb4, 1000, 0, DAT_009cf5e4, 1, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x50) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x15;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfaa0, 0, 0, DAT_009cf5e8, 1, 0, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x54) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x16;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfc30, 200, 0, DAT_009cf5e8, 7, local210, 4, local210 + 1, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x58) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x17;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfa90, 200, 0, DAT_009cf5e8, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x5c) = result;

        obj = (u32)cm_fn_00944cf1(0xa2);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x18;
        if (obj == 0) {
            result = 0;
        } else {
            local210 = *(volatile u32 *)(scratch + 4);
            result = (u32)cm_fn_00766e10((void *)(u32)obj, edi_val, counter, DAT_009cfbcc, 100, 0, DAT_009cf5e8, 7, local210, 4, local210 + 1, param_2, 1);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x60) = result;

        obj = (u32)cm_fn_00944cf1(0x2b);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x19;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)(u32)obj, edi_val, counter, DAT_009cfc2c, 500, 0, DAT_009cf5e8, 1, 0, 0, param_2, 0);
            counter = counter + 1;
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        frame.state = 0xffffffff;
        *(volatile u32 *)((char *)tp + 0x64) = result;

        obj = (u32)cm_fn_00944cf1(0x25d);
        *(volatile u32 *)(scratch + 0) = obj;
        frame.state = 0x1a;
        if (obj == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_008af520((void *)(u32)obj, edi_val, counter, DAT_009cfbb8, 500, 0, DAT_009cf5e8, 1, 0, param_2, 1);
        }
        tp = (u32 *)(u32)*(volatile u32 *)((char *)param_1 + 4);
        *(volatile u32 *)((char *)tp + 0x68) = result;
    }
    CM_FS(void *, 0) = (void *)frame.prev;
}
