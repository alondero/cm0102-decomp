#include "cmre.h"

#define ExceptionList CM_FS(void *, 0)
#define DAT_009cf5cc CM_GLOBAL(u32, 0x009cf5cc)
#define DAT_009cf5d0 CM_GLOBAL(u32, 0x009cf5d0)
#define DAT_009cf5d4 CM_GLOBAL(u32, 0x009cf5d4)
#define DAT_009cfad8 CM_GLOBAL(u32, 0x009cfad8)
#define DAT_009cfadc CM_GLOBAL(u32, 0x009cfadc)
#define DAT_009cfae0 CM_GLOBAL(u32, 0x009cfae0)
#define DAT_009cfae4 CM_GLOBAL(u32, 0x009cfae4)
#define DAT_009cfae8 CM_GLOBAL(u32, 0x009cfae8)
#define DAT_009cfaec CM_GLOBAL(u32, 0x009cfaec)
#define DAT_009cfaf0 CM_GLOBAL(u32, 0x009cfaf0)
#define DAT_009cfaf4 CM_GLOBAL(u32, 0x009cfaf4)
#define DAT_009cfaf8 CM_GLOBAL(u32, 0x009cfaf8)
#define DAT_009cfafc CM_GLOBAL(u32, 0x009cfafc)
#define DAT_009cfb00 CM_GLOBAL(u32, 0x009cfb00)
#define DAT_009cfbf4 CM_GLOBAL(u32, 0x009cfbf4)
#define DAT_009cfce4 CM_GLOBAL(u32, 0x009cfce4)
#define DAT_009cfce8 CM_GLOBAL(u32, 0x009cfce8)
#define DAT_009cff58 CM_GLOBAL(u32, 0x009cff58)
#define DAT_009cff5c CM_GLOBAL(u32, 0x009cff5c)
#define DAT_009cff60 CM_GLOBAL(u32, 0x009cff60)
#define DAT_009cff64 CM_GLOBAL(u32, 0x009cff64)
#define DAT_009cff68 CM_GLOBAL(u32, 0x009cff68)
#define DAT_009cff6c CM_GLOBAL(u32, 0x009cff6c)
#define DAT_009cff74 CM_GLOBAL(u32, 0x009cff74)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08,
    u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20,
    u32 arg_24, u32 arg_28);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 *CM_THISCALL cm_fn_0092fe80(u32 *self, u32 arg1, u32 arg2, u32 arg3,
    u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10,
    u32 arg11);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
    void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

typedef struct {
    volatile void *prev;
    volatile void *handler;
    volatile u32 state;
} cm_seh_frame;

void CM_THISCALL cm_fn_005c0a20(u32 this, u32 param_2)
{
    cm_seh_frame frame;
    void *saved_prev;
    u32 edi_val;
    u32 alloc;
    u32 result;
    u32 target;
    u32 esi;
    char *str_result;
    struct {
        volatile u32 home;
        u8 buffers[512];
    } scratch;
    u8 *buf_low = scratch.buffers;
    u8 *buf_high = scratch.buffers + 0x100;

    saved_prev = CM_FS(void *, 0);
    frame.state = (u32)-1;
    frame.handler = (void *)0x00959ca6;
    frame.prev = saved_prev;
    CM_FS(void *, 0) = &frame;

    edi_val = *(u32 *)(this + 0x13);
    *(u16 *)(this + 8) = 0x15;

    alloc = (u32)cm_fn_00944e46(0x54);
    *(u32 *)(this + 4) = alloc;

    if (alloc == 0) {
        cm_fn_00944cff(0x009cccbc, 0, 0, buf_high, buf_low);
        str_result = cm_fn_0090d130();
        cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)(char *)0x009870e8,
                       (u32)str_result, (u32)buf_high, (u32)buf_low, 0x74);
        cm_fn_005e8290((const char *)0x009870e0,
                       (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
    } else {
        esi = 0;

        /* 0 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0;
        if (alloc == 0) {
            result = 0;
        } else {
            esi = 1;
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, 0,
                DAT_009cfbf4, 0x1770, 0x9c4, 0xffffffff, esi, 0, 0,
                param_2, 0);
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x0) = result;

        /* 1 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 1;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfad8, 0x1b58, 0x1194, 0xffffffff, 0, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x4) = result;

        /* 2 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 2;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfadc, 0x1964, 0xdac, 0xffffffff, 0, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x8) = result;

        /* 3 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 3;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfae0, 0x1388, 0x7d0, 0xffffffff, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0xc) = result;

        /* 4 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 4;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfae4, 0x1388, 0x5dc, 0xffffffff, 0, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x10) = result;

        /* 5 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 5;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfae8, 0, 0, 0xffffffff, 0, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x14) = result;

        /* 6 */
        alloc = (u32)cm_fn_00944cf1(0x25d);
        scratch.home = alloc;
        frame.state = 6;
        if (alloc == 0) {
            result = 0;
        } else {
            result = cm_fn_008af520((void *)alloc, edi_val, esi,
                DAT_009cfce4, 0, 0, DAT_009cf5cc, 1, 0, param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x18) = result;

        /* 7 */
        alloc = (u32)cm_fn_00944cf1(0x25d);
        scratch.home = alloc;
        frame.state = 7;
        if (alloc == 0) {
            result = 0;
        } else {
            result = cm_fn_008af520((void *)alloc, edi_val, esi,
                DAT_009cff58, 0, 0, DAT_009cf5cc, 1, 0, param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x1c) = result;

        /* 8 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 8;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfaec, 0x1770, 0x7d0, DAT_009cf5cc, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x20) = result;

        /* 9 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 9;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfaf0, 0x1770, 0x9c4, DAT_009cf5cc, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x24) = result;

        /* 10 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0xa;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfaf4, 0x157c, 0x5dc, DAT_009cf5cc, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x28) = result;

        /* 11 */
        alloc = (u32)cm_fn_00944cf1(0x25d);
        scratch.home = alloc;
        frame.state = 0xb;
        if (alloc == 0) {
            result = 0;
        } else {
            result = cm_fn_008af520((void *)alloc, edi_val, esi,
                DAT_009cfce8, 0, 0, DAT_009cf5d0, 1, 0, param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x2c) = result;

        /* 12 */
        alloc = (u32)cm_fn_00944cf1(0x25d);
        scratch.home = alloc;
        frame.state = 0xc;
        if (alloc == 0) {
            result = 0;
        } else {
            result = cm_fn_008af520((void *)alloc, edi_val, esi,
                DAT_009cff5c, 0, 0, DAT_009cf5d0, 1, 0, param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x30) = result;

        /* 13 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0xd;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfaf8, 0x1194, 0x3e8, DAT_009cf5d0, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x34) = result;

        /* 14 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0xe;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfafc, 0x1194, 0x5dc, DAT_009cf5d0, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x38) = result;

        /* 15 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0xf;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cfb00, 0xfa0, 0x1f4, DAT_009cf5d0, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x3c) = result;

        /* 16 */
        alloc = (u32)cm_fn_00944cf1(0x25d);
        scratch.home = alloc;
        frame.state = 0x10;
        if (alloc == 0) {
            result = 0;
        } else {
            result = cm_fn_008af520((void *)alloc, edi_val, esi,
                DAT_009cff68, 0, 0, DAT_009cf5d4, 1, 0, param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x40) = result;

        /* 17 */
        alloc = (u32)cm_fn_00944cf1(0x25d);
        scratch.home = alloc;
        frame.state = 0x11;
        if (alloc == 0) {
            result = 0;
        } else {
            result = cm_fn_008af520((void *)alloc, edi_val, esi,
                DAT_009cff6c, 0, 0, DAT_009cf5d4, 1, 0, param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x44) = result;

        /* 18 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0x12;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cff74, 0x1194, 0x3e8, DAT_009cf5d4, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x48) = result;

        /* 19 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0x13;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cff60, 0x5dc, 0x2bc, DAT_009cf5d4, 1, 0, 0,
                param_2, 1);
            esi++;
        }
        target = *(volatile u32 *)(this + 4);
        frame.state = (u32)-1;
        *(u32 *)(target + 0x4c) = result;

        /* 20 */
        alloc = (u32)cm_fn_00944cf1(0x2b);
        scratch.home = alloc;
        frame.state = 0x14;
        if (alloc == 0) {
            result = 0;
        } else {
            result = (u32)cm_fn_0092fe80((u32 *)alloc, edi_val, esi,
                DAT_009cff64, 0x3e8, 0x1f4, DAT_009cf5d4, 1, 0, 0,
                param_2, 1);
        }
        *(u32 *)((u32)*(u32 *)(this + 4) + 0x50) = result;
    }

    saved_prev = (void *)frame.prev;
    CM_FS(void *, 0) = saved_prev;
}
