#include "cmre.h"

#define DAT_00ae235c CM_GLOBAL(u32, 0x00ae235c)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

#define S_FMT_009ca1e0 ((u32)0x009ca1e0)
#define S_FINE_DAT_009ca1d4 ((u8 *)0x009ca1d4)
#define S_ERROR_009870e0 ((char *)0x009870e0)
#define S_VS_009870e8 ((u32)0x009870e8)

extern void cm_fn_005e8290(const char * param_1, const char * param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern char * cm_fn_0090d130(...);
extern int * cm_fn_009326e0(u32 param_1, u8 *param_2, int param_3, int param_4, u32 param_5);
extern void cm_fn_00932b00(int *param_1);
extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u8 CM_THISCALL cm_fn_005a53d0(int **param_1, int param_2)
{
    u8 scratch[0x208];
    u32 line;
    u32 r;
    int i;
    int *p0;
    int *p1;
    u32 tmp;

#define L208 (*(u32 *)(scratch + 0x000))
#define L204 (*(u32 *)(scratch + 0x004))
#define B200 (scratch + 0x008)
#define B100 (scratch + 0x108)

    if (param_2 == 0) {
        cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B100, (void *)B200);
        r = (u32)cm_fn_0090d130((char *)B100, (char *)B200, 0x109);
        cm_fn_00944c9f((u32)&DAT_00ae24d0, S_VS_009870e8, r, (u32)B100, (u32)B200, 0x109);
        cm_fn_005e8290(S_ERROR_009870e0, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        return 0;
    }

    L208 = (u32)cm_fn_009326e0((u32)param_2, S_FINE_DAT_009ca1d4, 1, 0x16, 4);
    if (L208 == 0) {
        cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
        line = 0x111;
        goto error_tail;
    }

    L204 = (u32)(s16)*(short *)param_1[0];
    r = cm_fn_00932e10((u32)&L204, 4, 1, L208);
    if (r != 1) {
        cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
        line = 0x119;
        goto error_tail;
    }

    p0 = param_1[0];
    tmp = *(u32 *)((u8 *)p0 + 2);
    *(u32 *)((u8 *)p0 + 0xa) = tmp;
    p0 = (int *)*(volatile u32 *)&param_1[0];
    if ((s16)*(short *)p0 > 0) {
        i = 0;
        do {
            r = cm_fn_0068d1b0((u32)p0);
            if (r == 0) {
                cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
                line = 0x123;
                goto error_tail;
            }
            r = cm_fn_00932e10(r, 0xe, 1, L208);
            if (r != 1) {
                cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
                line = 0x129;
                goto error_tail;
            }
            p0 = param_1[0];
            i++;
        } while (i < (s16)*(short *)p0);
    }

    r = cm_fn_00932e10((u32)&DAT_00ae235c, 4, 1, L208);
    if (r != 1) {
        cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
        line = 0x132;
        goto error_tail;
    }

    r = cm_fn_00932e10((u32)param_1[2], 1, DAT_00ae235c, L208);
    if (r != DAT_00ae235c) {
        cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
        line = 0x13a;
        goto error_tail;
    }

    p1 = param_1[1];
    L204 = (u32)(s16)*(short *)p1;
    r = cm_fn_00932e10((u32)&L204, 4, 1, L208);
    if (r != 1) {
        cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
        line = 0x143;
        goto error_tail;
    }

    p1 = param_1[1];
    tmp = *(u32 *)((u8 *)p1 + 2);
    *(u32 *)((u8 *)p1 + 0xa) = tmp;
    p1 = (int *)*(volatile u32 *)&param_1[1];
    if ((s16)*(short *)p1 > 0) {
        i = 0;
        do {
            r = cm_fn_0068d1b0((u32)p1);
            if (r == 0) {
                cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
                line = 0x14d;
                goto error_tail;
            }
            r = cm_fn_00932e10(r, 0x11, 1, L208);
            if (r != 1) {
                cm_fn_00944cff(S_FMT_009ca1e0, 0, 0, (void *)B200, (void *)B100);
                line = 0x153;
                goto error_tail;
            }
            p1 = param_1[1];
            i++;
        } while (i < (s16)*(short *)p1);
    }

    cm_fn_00932b00((int *)&L208);
    return 1;

error_tail:
    r = (u32)cm_fn_0090d130((char *)B200, (char *)B100, line);
    cm_fn_00944c9f((u32)&DAT_00ae24d0, S_VS_009870e8, r, (u32)B200, (u32)B100, line);
    cm_fn_005e8290(S_ERROR_009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    cm_fn_00932b00((int *)&L208);
    return 0;
}
