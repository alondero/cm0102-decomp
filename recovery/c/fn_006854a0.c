#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

extern u32 CM_CDECL cm_fn_004b0700(u32 v);
extern u32 CM_THISCALL cm_fn_006827d0(int param_1,int param_2);
extern void CM_THISCALL cm_fn_00685f60(int *param_1,u32 param_2,u32 param_3,short param_4,int param_5,short *param_6, short *param_7);
extern u32 * CM_FASTCALL cm_fn_00687710(void *param_1);
extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);

u32 CM_THISCALL cm_fn_006854a0(int param_1,int param_2,volatile int param_3)
{
    u8 *self = (u8 *)param_1;
    u8 *arg3 = (u8 *)param_3;

    if (*(u8 *)(arg3 + 0x42) != 0xff)
        return 0;
    if (cm_fn_004b0700(*(u32 *)(*(u32 *)(self + 4))) == 0)
        return 0;
    if ((*(u8 *)(self + 0xd9) & 1) == 0)
        return 0;
    if (*(s16 *)(self + 0xa9) < 0x18)
        return 0;

    {
        s16 si = *(s16 *)(self + 0x3e);
        u8 *base = (u8 *)(u32)(*(u32 *)(self + 0xb1));
        s16 i = 0;
        u8 *first = NULL;

        if (si > 0) {
            for (;;) {
                u8 *entry = base + (int)i * 0x3b;
                if (*(u32 *)entry == (u32)param_2) {
                    first = entry;
                    break;
                }
                i++;
                if (!(i < si))
                    break;
            }
        }
        if (first == NULL)
            return 0;

        if ((u32)first == 0)
            return 0;
        {
            u8 c = *(u8 *)(first + 0x37);
            if (c == 5 || c == 2 || c == 3 || c == 0xfe || c == 0xfc || c == 0xfb)
                return 0;
        }

        {
            u8 *esi = (u8 *)(u32)cm_fn_00687710((void *)param_1);
            s16 cnt;

            if (esi == NULL)
                return 0;

            cnt = *(s16 *)(self + 0x3e);
            cm_fn_00685f60((int *)param_1, (u32)param_3, (u32)esi, (short)cnt, 0, 0, 0);

            cm_fn_006827d0(param_1, (int)esi);

            {
                s16 si2 = *(s16 *)(self + 0x3e);
                s16 j = 0;
                volatile u32 second = 0;

                if (si2 > 0) {
                    for (;;) {
                        u8 *entry = esi + (int)j * 0x3b;
                        if (*(u32 *)entry == (u32)param_2) {
                            second = (u32)entry;
                            break;
                        }
                        j++;
                        if (!(j < si2))
                            break;
                    }
                }
                {
                    if (*(volatile u8 *)(second + 4u) != 0) {
                        cm_fn_00687970((u8 *)param_1, esi);
                        return 0;
                    }
                }

                param_3 = 1;
                if (si2 > 1) {
                    s16 dx_a7 = *(volatile s16 *)(self + 0xa7);
                    s8 bx = *(volatile s8 *)(self + 0xc2);
                    u32 k = 1;

                    for (;;) {
                        u8 *entry = (u8 *)((u32)esi + (u32)((s32)(s16)k * 0x3b));
                        s16 cx = (s16)(s8)*(entry + 0x11);
                        s16 bp = (s16)((dx_a7 - cx) * bx + *(s16 *)(entry + 0x19));

                        if (bp >= *(s16 *)(esi + 0x19)) {
                            if ((s8)*(esi + 0x11) != dx_a7 || cx != dx_a7) {
                                cm_fn_00687970((u8 *)param_1, esi);
                                return 0;
                            }
                        }
                        k = (u32)param_3 + 1u;
                        {
                            s16 count = *(volatile s16 *)(self + 0x3e);
                            param_3 = (int)k;
                            if ((s16)k >= count)
                                break;
                        }
                    }
                }
                cm_fn_00687970((u8 *)param_1, esi);
                return 1;
            }
        }
    }
}
