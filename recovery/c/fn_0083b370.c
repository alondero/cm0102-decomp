#include "cmre.h"

#define DAT_00ae235c CM_GLOBAL(int, 0x00ae235c)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* Legacy signature is (void); the errcode + two scratch buffers pushed before
   0x83b625 are trailing varargs of the 0x944c9f sprintf (format 0x9870e8). */
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_0083b370(u32 *param_1, u32 param_2)
{
    /* One scratch allocation: the two lea buffers are 0x100 apart.
       bufA = [esp+0x1c] = L+0xc, bufB = [esp+0x11c] = L+0x10c (esp = L-0x10). */
    u8 scratch[0x20c];
    u8 *bufA = scratch + 0xc;
    u8 *bufB = scratch + 0x10c;

    u32 r;
    int n;
    int n2;
    u32 *p;
    int *q;
    u8 *errBuf1;
    u8 *errBuf2;
    u32 errCode;
    u32 d130res;
    u32 row;
    u32 rowBase;
    u32 savedThis;
    int datAfter;
    int lim;
    u32 ptr;
    u32 offset;
    int v;

    savedThis = (u32)param_1;

    /* 0x83b386: esi = this+0x10; 0x83b390: f(esi,4,1,param_2) */
    q = (int *)(param_1 + 4);
    r = cm_fn_00932e10((u32)q, 4, 1, param_2);
    if (r == 1) {
        /* 0x83b3d1-0x83b3da: f(*this,8,*(this+0x10),param_2) */
        r = cm_fn_00932e10(*param_1, 8, (u32)*q, param_2);
        if (r == (u32)*q) {
            /* 0x83b41c-0x83b43e: 16 entries at this+0x98 */
            n = 0;
            p = param_1 + 0x26;
            do {
                if (cm_fn_00932e10((u32)p, 4, 1, param_2) != 1) {
                    /* 0x83b48c, err 0x3c7: 944cff(...,bufA,bufB), d130(bufA,bufB) */
                    cm_fn_00944cff(0x00a825a4, 0, 0, bufA, bufB);
                    errBuf1 = bufA;
                    errBuf2 = bufB;
                    errCode = 0x3c7;
                    goto LAB_ERR;
                }
                n++;
                p++;
            } while (n < 0x10);

            /* 0x83b440-0x83b450: f(this+0xd8,4,1,param_2) */
            if (cm_fn_00932e10((u32)(param_1 + 0x36), 4, 1, param_2) != 1) {
                /* 0x83b458, err 0x3cf */
                cm_fn_00944cff(0x00a825a4, 0, 0, bufA, bufB);
                errBuf1 = bufA;
                errBuf2 = bufB;
                errCode = 0x3cf;
                goto LAB_ERR;
            }

            /* 0x83b4c1: ebx = this+0x18; row loop over 2 rows of 16 */
            rowBase = (u32)(param_1 + 6);
            row = 0;
            do {
                n2 = 0;
                p = (u32 *)rowBase;
                q = (int *)(savedThis + 0x98);
                do {
                    if (cm_fn_00932e10(*p, 0xc, (u32)*q, param_2) != (u32)*q) {
                        /* 0x83b57c, err 0x3db */
                        cm_fn_00944cff(0x00a825a4, 0, 0, bufA, bufB);
                        errBuf1 = bufA;
                        errBuf2 = bufB;
                        errCode = 0x3db;
                        goto LAB_ERR;
                    }
                    n2++;
                    p++;
                    q++;
                } while (n2 < 0x10);
                row++;
                rowBase += 0x40;
            } while (row < 2);

            /* 0x83b520-0x83b53a: f(*(this+0xdc),9,DAT,param_2), reload DAT */
            r = cm_fn_00932e10(*(u32 *)(savedThis + 0xdc), 9, (u32)DAT_00ae235c, param_2);
            datAfter = DAT_00ae235c;
            if (r == (u32)datAfter) {
                n = 0;
                lim = datAfter;
                if (lim <= 0)
                    return;
                offset = 0;
                for (;;) {
                    ptr = *(volatile u32 *)(savedThis + 0xdc);
                    v = *(int *)(ptr + offset);
                    if (v != 0) {
                        r = cm_fn_00932e10((u32)v, 0x25, 0x28, param_2);
                        if (r != 0x28)
                            break;
                        lim = DAT_00ae235c; /* 0x83b5da reload after successful call */
                    }
                    n++;
                    offset += 9;
                    if (lim <= n)
                        return;
                }
                /* 0x83b5f5, err 0x3f3 */
                cm_fn_00944cff(0x00a825a4, 0, 0, bufA, bufB);
                errBuf1 = bufA;
                errBuf2 = bufB;
                errCode = 0x3f3;
                goto LAB_ERR;
            } else {
                /* 0x83b547, err 0x3e5 */
                cm_fn_00944cff(0x00a825a4, 0, 0, bufA, bufB);
                errBuf1 = bufA;
                errBuf2 = bufB;
                errCode = 0x3e5;
                goto LAB_ERR;
            }
        } else {
            /* 0x83b3e8, err 0x3be */
            cm_fn_00944cff(0x00a825a4, 0, 0, bufA, bufB);
            errBuf1 = bufA;
            errBuf2 = bufB;
            errCode = 0x3be;
            goto LAB_ERR;
        }
    } else {
        /* 0x83b39d, err 0x3b6: 944cff(...,bufB,bufA), d130(bufB,bufA) */
        cm_fn_00944cff(0x00a825a4, 0, 0, bufB, bufA);
        errBuf1 = bufB;
        errBuf2 = bufA;
        errCode = 0x3b6;
        goto LAB_ERR;
    }
    return;

LAB_ERR:
    d130res = (u32)cm_fn_0090d130();
    /* sprintf(0xae24d0, 0x9870e8, A(), bufB/bufA, bufA/bufB, errcode): the
       last three pushes before 0x83b625 are its fourth-to-sixth varargs. */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8, d130res,
                   (u32)errBuf1, (u32)errBuf2, errCode);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
}
