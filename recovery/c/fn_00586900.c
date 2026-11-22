#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 param_1, u8 param_2);
extern u32 CM_CDECL cm_fn_004c08f0(volatile u32 first, volatile u32 second);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef u32 (CM_THISCALL *vt_fn_t)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);

void CM_FASTCALL cm_fn_00586900(u32 this_ptr)
{
    u8 scratch[0x220];
    u32 idx;
    s16 n;
    u32 *pvVar2;
    u32 vt_ret;
    vt_fn_t vtfn;

    /* L = esp after sub esp,0x220.  Saved regs put esp = L-0xc here.
       [esp+0x14] -> L+0x8 (counter B), [esp+0xf] -> L+0x3 (counter A). */
    scratch[0x8] = 1;
    scratch[0x3] = 1;
    idx = 0;

    if (*(u32 *)(this_ptr + 8) == 0) {
        cm_fn_00944cff(0x009c4900u, 0, 0, (void *)(scratch + 0x120), (void *)(scratch + 0x20));
        {
            char *r = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)r,
                           (u32)(scratch + 0x120), (u32)(scratch + 0x20), 0x81bu);
        }
        cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        return;
    }

    n = *(s16 *)(this_ptr + 0x36);
    pvVar2 = (u32 *)(u32)cm_fn_00944e46((u32)((u32)n << 2));

    if (pvVar2 == (u32 *)0) {
        cm_fn_00944cff(0x009c4900u, 0, 0, (void *)(scratch + 0x20), (void *)(scratch + 0x120));
        {
            char *r = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)r,
                           (u32)(scratch + 0x20), (u32)(scratch + 0x120), 0x823u);
        }
        cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        return;
    }

    {
        s16 i;
        for (i = 0; i < (s16)*(volatile u16 *)(this_ptr + 0x36); i++) {
            u32 *arr;
            u32 src;
            arr = (u32 *)(u32)*(volatile u32 *)(this_ptr + 0x14);
            src = *(volatile u32 *)((char *)arr + (s32)i * 6);
            pvVar2[i] = src;
        }
    }

    cm_fn_004c08f0((u32)pvVar2, (u32)(s32)*(volatile s16 *)(this_ptr + 0x36));

    vtfn = (vt_fn_t)(*(u32 *)(*(u32 *)this_ptr + 0x3c));
    vt_ret = vtfn(this_ptr, 0xcu, (u32)(scratch + 0x14), (u32)(scratch + 0x1e), 0u);
    *(u32 *)(scratch + 0x18) = vt_ret;

    {
        s32 v = (s32)(s16)(u16)(*(u16 *)(scratch + 0x14) - 1);
        if (v >= 0) {
            s32 iVar4 = v;
            s32 iVar6 = v + 1;
            u8 *rec = (u8 *)(vt_ret + (u32)(iVar4 * 0x68) + 0x1au);
            s16 s21c = *(s16 *)(scratch + 0x14);
            for (;;) {
                if (iVar4 == (s32)s21c - 1) {
                    if (*(s16 *)rec > 0) {
                        s16 j = 0;
                        do {
                            cm_fn_004a2540(*(u32 *)(this_ptr + 8), pvVar2[(s16)idx], (u8)*(u32 *)(scratch + 8));
                            idx++;
                            scratch[0x3] = (u8)(scratch[0x3] + 1);
                            j++;
                        } while (j < *(s16 *)rec);
                    }
                }
                scratch[0x8] = scratch[0x3];
                if (*(s16 *)rec > 0) {
                    s16 j = 0;
                    do {
                        cm_fn_004a2540(*(u32 *)(this_ptr + 8), pvVar2[(s16)idx], (u8)*(u32 *)(scratch + 8));
                        idx++;
                        scratch[0x3] = (u8)(scratch[0x3] + 1);
                        j++;
                    } while (j < *(s16 *)rec);
                }
                iVar4--;
                rec -= 0x68;
                iVar6--;
                if (iVar6 == 0) break;
                s21c = *(s16 *)(scratch + 0x14);
            }
        }
    }

    cm_fn_009452ca(*(u32 *)(scratch + 0x18));

    {
        s32 k = 2;
        while (k != 0) {
            scratch[0x8] = scratch[0x3];
            scratch[0x3] = (u8)(scratch[0x3] + 4);
            s32 m = 4;
            while (m != 0) {
                cm_fn_004a2540(*(u32 *)(this_ptr + 8), pvVar2[(s16)idx], (u8)*(u32 *)(scratch + 8));
                idx++;
                m--;
            }
            k--;
        }
    }
    {
        s32 k = 2;
        while (k != 0) {
            scratch[0x8] = scratch[0x3];
            scratch[0x3] = (u8)(scratch[0x3] + 8);
            s32 m = 8;
            while (m != 0) {
                cm_fn_004a2540(*(u32 *)(this_ptr + 8), pvVar2[(s16)idx], (u8)*(u32 *)(scratch + 8));
                idx++;
                m--;
            }
            k--;
        }
    }

    {
        s16 f = *(s16 *)(this_ptr + 0x3c);
        s32 fv = (s16)(f - 1);
        if (fv >= 0) {
            s32 off = fv * 0x68;
            s32 cnt2 = fv + 1;
            for (;;) {
                u32 pA3 = *(u32 *)((char *)this_ptr + 0xa3);
                scratch[0x8] = scratch[0x3];
                s16 c = *(s16 *)((char *)pA3 + off + 0x1a);
                if (c > 0) {
                    u32 inner = 0;
                    do {
                        cm_fn_004a2540(*(u32 *)(this_ptr + 8), pvVar2[(s16)idx], (u8)*(u32 *)(scratch + 8));
                        idx++;
                        scratch[0x3] = (u8)(scratch[0x3] + 1);
                        inner++;
                        pA3 = *(u32 *)((char *)this_ptr + 0xa3);
                    } while ((s16)inner < *(s16 *)((char *)pA3 + off + 0x1a));
                }
                off -= 0x68;
                cnt2--;
                if (cnt2 == 0) break;
            }
        }
    }

    cm_fn_009452ca((u32)pvVar2);
}
