/* Faithful C candidate for preferred-image VA 0x00482a40, CM0102 3.9.68. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0

#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)
#define s_Apply_for_Job_0099155c ((char *)0x0099155c)
#define s_Invite_Club_0099157c ((char *)0x0099157c)
#define s_Scout_Club_00991588 ((char *)0x00991588)
#define s_Take_Control_00991594 ((char *)0x00991594)
#define s_Uninvite_Club_0099156c ((char *)0x0099156c)

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053ca90(volatile u8 *self);
extern u8 CM_CDECL cm_fn_00544820(u32 *object);
extern int CM_FASTCALL cm_fn_005c6830(int param_1);
extern u32 CM_THISCALL cm_fn_005c68b0(u32 self);
extern u32 CM_THISCALL cm_fn_005c6920(u32 self);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 CM_CDECL cm_fn_00600c90(u32 club, u32 skip_status, u8 flag);
extern char * cm_fn_0066f4e0(int param_1, char *param_2);
extern u32 CM_THISCALL cm_fn_007f8420(u32 self);
extern u32 CM_THISCALL cm_fn_007f8500(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_00804030(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11, u32 arg12);
extern u32 CM_CDECL cm_fn_00838de0(void);

/* Reconstructed from the pushes at 0x482a8c/8e, 0x482baf-bb9, 0x482ce1-cec.
   ECX = 0xb74450 at 0x482c8b / 0x482cf1 is the thiscall receiver. */
#define FINAL_CALL(pu, a11) \
    cm_fn_00804030(0xb74450u, 0x294u, 4u, 0x311u, 0x18u, 0x30u, \
                   param_3, 0xc, 1, param_2, (u32)(&DAT_00de1f64), \
                   (a11), (u32)(pu))

u32 cm_fn_00482a40(int param_1, u32 param_2, u32 param_3)
{
    u8 bVar1;
    s8 cVar2;
    u32 iVar3;
    u32 iVar4;
    u32 *puVar6;

    iVar3 = cm_fn_00838de0();
    if (iVar3 == 0) {
        iVar3 = cm_fn_007f8420(CM_GLOBAL(u32, 0xb74448));
        if (((iVar3 != 0) &&
             (iVar3 = cm_fn_007f8500(CM_GLOBAL(u32, 0xb74448), (u32)param_1), iVar3 == 0)) &&
            (iVar3 = cm_fn_0053b900((u32 *)(param_1)), iVar3 == 0)) {
            cm_fn_0066f4e0((int)(&DAT_00de1f64), s_Scout_Club_00991588);
            FINAL_CALL(0, 0x6a);
            return 1;
        }
        iVar3 = cm_fn_00600c90((u32)param_1, 0, 1);
        if (iVar3 == 0) {
            puVar6 = (u32 *)cm_fn_005c68b0(CM_GLOBAL(u32, 0xae2c74));
            if (puVar6 != (u32 *)0) {
                iVar3 = cm_fn_0053b900((u32 *)(puVar6[3]));
                iVar4 = cm_fn_0053b900((u32 *)(param_1));
                if ((iVar4 != 0) == (iVar3 != 0)) {
                    cm_fn_0066f4e0((int)(&DAT_00de1f64), s_Invite_Club_0099157c);
                    FINAL_CALL(puVar6, 0x66);
                    return 1;
                }
            }
            puVar6 = (u32 *)cm_fn_005c6920(CM_GLOBAL(u32, 0xae2c74));
            if ((puVar6 != (u32 *)0) && (*(int *)((int)param_1 + 0x53) == (int)puVar6[1])) {
                iVar3 = cm_fn_0053b900((u32 *)(param_1));
                iVar4 = cm_fn_0053b900((u32 *)(*puVar6));
                if ((iVar3 != 0) == (iVar4 != 0)) {
                    cm_fn_0066f4e0((int)(&DAT_00de1f64), s_Invite_Club_0099157c);
                    FINAL_CALL(puVar6, 0x67);
                    return 1;
                }
            }
            puVar6 = (u32 *)cm_fn_005c6830((int)CM_GLOBAL(u32, 0xae2c74));
            if (puVar6 != (u32 *)0) {
                iVar3 = cm_fn_0053b900((u32 *)(param_1));
                iVar4 = cm_fn_0053b900((u32 *)(*puVar6));
                if ((iVar3 != 0) == (iVar4 != 0)) {
                    s16 dx = *(s16 *)((int)puVar6 + 0x13);
                    bVar1 = false;
                    cVar2 = '\0';
                    if (0 < dx) {
                        do {
                            if (bVar1) goto LAB_00482c29;
                            if (*(int *)((int)puVar6 + cVar2 * 4 + 0x15) == param_1) {
                                bVar1 = true;
                            }
                            cVar2 = cVar2 + '\x01';
                        } while ((s16)cVar2 < dx);
                        if (bVar1) {
LAB_00482c29:
                            cm_fn_0066f4e0((int)(&DAT_00de1f64), s_Uninvite_Club_0099156c);
                            FINAL_CALL(puVar6, 0x68);
                            return 1;
                        }
                    }
                    cm_fn_0066f4e0((int)(&DAT_00de1f64), s_Invite_Club_0099157c);
                    FINAL_CALL(puVar6, 0x69);
                    return 1;
                }
            }
        }
        iVar3 = cm_fn_0053ca90((volatile u8 *)(param_1));
        if (iVar3 != 0) {
            return 0;
        }
        bVar1 = cm_fn_00544820((u32 *)(param_1));
        if (bVar1 == 0) {
            return 0;
        }
        cm_fn_0066f4e0((int)(&DAT_00de1f64), s_Apply_for_Job_0099155c);
        FINAL_CALL(0, 0x65);
        return 1;
    }
    else {
        iVar3 = cm_fn_00600a50((u32)param_1, 1, 1, 0, 0);
        if ((iVar3 != 0) || (bVar1 = cm_fn_00544820((u32 *)(param_1)), bVar1 == 0)) {
            return 0;
        }
        cm_fn_0066f4e0((int)(&DAT_00de1f64), s_Take_Control_00991594);
        FINAL_CALL(0, 100);
        return 1;
    }
}
