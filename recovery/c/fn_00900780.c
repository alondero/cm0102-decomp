#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

/* preferred-image VA 0x00900780, SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86 */
#define EXCEPTION_LIST   CM_FS(void *, 0)
#define G_CF8E8  CM_GLOBAL(int, 0x009cf8e8)
#define G_CF8EC CM_GLOBAL(int, 0x009cf8ec)
#define G_CF8F0 CM_GLOBAL(int, 0x009cf8f0)
#define G_CF8F4 CM_GLOBAL(int, 0x009cf8f4)
#define G_AE2350 CM_GLOBAL(int, 0x00ae2350)
#define G_AE235C CM_GLOBAL(int, 0x00ae235c)
#define G_AE23B4 CM_GLOBAL(int, 0x00ae23b4)
#define G_AE24D0 CM_GLOBAL(u8, 0x00ae24d0)
#define G_B67A34 CM_GLOBAL(u32, 0x00b67a34)
#define S_FILE  ((u32)0x00aa18b8)
#define S_ERR   ((u32)0x009870e0)
#define S_FMT   ((u32)0x009870e8)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 * CM_THISCALL cm_fn_0068b100(u32 *param_1,int param_2,u16 param_3,int param_4,u16 param_5, u32 param_6,u32 *param_7,u16 param_8,u8 param_9, u8 param_10,u8 param_11,int param_12,int param_13,u16 param_14, u8 param_15,u16 param_16,u8 param_17,u8 param_18, u16 param_19,u16 param_20,u16 param_21,u32 param_22, u8 param_23);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef u32 CM_THISCALL (*vt_fn)(void *self, u32 a1, void *a2, void *a3, u32 a4);

typedef struct {
    void *prev;
    void *handler;
    u32 state;
} seh_frame;

void CM_THISCALL cm_fn_00900780(int *param_1, u32 param_2)
{
    u8 *obj = (u8 *)param_1;
    u8 scratch[0x420];       /* scratch[0] = S-0x428 (S = ESP on entry) */
    volatile seh_frame seh;
    u32 sel, selected, pv, count, rec, alloc2, result;
    int i, num;
    s8 final_index;

#define COUNT     (*(u32 *)(scratch + 0x000))  /* S-0x428 */
#define SELECTED  (*(u32 *)(scratch + 0x004))  /* S-0x424 */
#define STRUCT8   (scratch + 0x008)            /* S-0x420 */
#define PVAR2     (*(u32 *)(scratch + 0x00c))  /* S-0x41c */
#define VTRET     (*(u32 *)(scratch + 0x010))  /* S-0x418 */
#define L414      (scratch + 0x014)            /* S-0x414 */
#define L410      (scratch + 0x018)            /* S-0x410 */
#define B1        (scratch + 0x01c)            /* S-0x40c */
#define B2        (scratch + 0x11c)            /* S-0x30c */
#define B3        (scratch + 0x21c)            /* S-0x20c */
#define B4        (scratch + 0x31c)            /* S-0x10c */

    /* prologue: SEH {prev, handler=0x9650ae, state=-1} linked at fs:[0] */
    seh.state = 0xffffffffu;
    seh.handler = (void *)0x009650aeu;
    seh.prev = EXCEPTION_LIST;
    EXCEPTION_LIST = (void *)&seh;

    /* 0x009007ba-0x009007cf: initialize struct and dispatch vtable slot [eax+0x3c] */
    COUNT = 0;
    STRUCT8[0] = 0;
    STRUCT8[1] = 2;
    STRUCT8[2] = 0;
    STRUCT8[3] = 0;
    {
        vt_fn fn = ((vt_fn *)(*(u32 **)obj))[0x3c / 4];
        VTRET = fn(obj, param_2, L410, L414, 0);
    }

    /* 0x009007d6: al = byte [esp+0x43c] = low byte of the stack argument */
    sel = (u8)param_2;
    if (sel == 0) selected = (u32)G_CF8E8;
    else if (sel == 1) selected = (u32)G_CF8EC;
    else if (sel == 2) selected = (u32)G_CF8F0;
    else selected = (u32)G_CF8F4;
    SELECTED = selected;

    /* 0x00900812: cm_fn_00944e46(0x28) */
    pv = (u32)cm_fn_00944e46(0x28);
    PVAR2 = pv;

    if (pv == 0) {
        /* 0x00900824 */
        cm_fn_00944cff(S_FILE, 0, 0, B1, B2);
        cm_fn_00944c9f(0x00ae24d0u, S_FMT, (u32)cm_fn_0090d130(),
                       (u32)B1, (u32)B2, 0x299u);
        cm_fn_005e8290((const char *)S_ERR, (const char *)&G_AE24D0, 0);
        G_B67A34 = 0;
        EXCEPTION_LIST = seh.prev;
        return;
    }

    /* 0x00900856: scan records */
    count = 0;
    {
        u32 t50 = (u32)G_AE2350;
        u32 t5c = (u32)G_AE235C;
        rec = (u32)G_AE23B4;
        num = (int)(t5c - 2u * t50);
    }
    if (num > 0) {
        i = 0;
        for (;;) {
            u32 f57 = *(u32 *)(rec + 0x57);
            u32 f60;
            if (f57 != 0) {
                f60 = *(u32 *)(rec + 0x60);
                if (f60 != 0 && *(u32 *)f60 == SELECTED &&
                    f57 == (u32)param_1[1]) {
                    if ((s16)count < 10) {
                        /* 0x00900904 */
                        *(u32 *)(pv + (s32)(s16)(u16)count * 4) = rec;
                        count++;
                        COUNT = count;
                    } else {
                        /* 0x009008a3: buffer full */
                        cm_fn_00944cff(S_FILE, 0, 0, B2, B1);
                        cm_fn_00944c9f(0x00ae24d0u, S_FMT, (u32)cm_fn_0090d130(),
                                       (u32)B2, (u32)B1, 0x2aau);
                        cm_fn_005e8290((const char *)S_ERR, (const char *)&G_AE24D0, 0);
                        G_B67A34 = 0;
                        *(u32 *)(rec + 0x57) = 0;
                    }
                }
            }
            /* 0x00900913 */
            i++;
            rec += 0x245;
            {
                u32 t50 = (u32)G_AE2350;
                u32 t5c = (u32)G_AE235C;
                num = (int)(t5c - 2u * t50);
            }
            if (i >= num) break;
        }
    }

    if ((s16)count >= 10) {
        /* 0x0090096c */
        alloc2 = (u32)cm_fn_00944cf1(0xee);
        COUNT = alloc2;
        seh.state = 0;
        if (alloc2 == 0) {
            result = 0;
        } else {
            /* 0x00900988-0x009009e9: 22 stack arguments, this in ECX */
            result = (u32)cm_fn_0068b100(
                (u32 *)alloc2,
                (int)obj,
                10,
                (int)pv,
                2,
                *(u32 *)(obj + 4),
                (u32 *)VTRET,
                (u16)*(u32 *)(L410),
                (u8)obj[0xc2],
                (u8)obj[0xc3],
                (u8)obj[0xc4],
                (int)(obj + 0xc5),
                (int)(u32)STRUCT8,
                (u16)*(u16 *)(obj + 0x40),
                (u8)param_2,
                (u16)*(u32 *)(L414),
                (u8)obj[0x51],
                1,
                0,
                (u16)*(u16 *)(obj + 0xd9),
                (u16)0xffff,
                0,
                2);
        }
        seh.state = 0xffffffffu;

        /* 0x009009f2: edi = movsx byte [esp+0x43c] = sign-extended stack-arg byte */
        final_index = (s8)(u8)param_2;
        *(u32 *)(*(u32 *)(obj + 0xc) + (s32)final_index * 4) = result;
        if (*(u32 *)(*(u32 *)(obj + 0xc) + (s32)final_index * 4) != 0) {
            /* 0x00900a77: success */
            cm_fn_009452ca(pv);
            cm_fn_009452ca(VTRET);
            *(u32 *)(obj + 0x30) = (u32)final_index;
            EXCEPTION_LIST = seh.prev;
            return;
        }
        /* 0x00900a13: final error */
        cm_fn_00944cff(S_FILE, 0, 0, B4, B3);
        cm_fn_00944c9f(0x00ae24d0u, S_FMT, (u32)cm_fn_0090d130(),
                       (u32)B4, (u32)B3, 0x2c2u);
        cm_fn_005e8290((const char *)S_ERR, (const char *)&G_AE24D0, 0);
        G_B67A34 = 0;
    } else {
        /* 0x0090093a: count < 10 */
        cm_fn_00944cff(S_FILE, 0, 0, B2, B1);
        cm_fn_00944c9f(0x00ae24d0u, S_FMT, (u32)cm_fn_0090d130(),
                       (u32)B2, (u32)B1, 0x2b7u);
        cm_fn_005e8290((const char *)S_ERR, (const char *)&G_AE24D0, 0);
        G_B67A34 = 0;
    }

    EXCEPTION_LIST = seh.prev;
}
