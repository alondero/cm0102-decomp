#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define S_00ad572c ((u32)0x00ad572c)
#define S_009870e0 ((const char *)0x009870e0)
#define S_009870e8 ((u32)0x009870e8)

typedef struct {
    volatile u32 prev;
    volatile u32 handler;
    volatile u32 state;
} seh_frame_t;

typedef u32 (CM_THISCALL *vt_fn_t)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);

extern u32 CM_THISCALL cm_fn_0051c410(u32 self, u32 index);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, u32 count, u32 clubs,
    u32 rounds, u32 competition, u32 schedule, u32 year, u32 flag_a, u32 flag_b,
    u32 round_flag, u32 flag_c, u32 builder, u32 numbered, u32 value_38,
    u32 club_flags);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_FASTCALL cm_fn_00928650(u32 param_1)
{
    seh_frame_t frame;
    u8 scratch[0x414];
    u32 *puVar1;
    u32 alloc_b2;
    u32 uVar6;
    u32 result;
    u32 v;
    u32 selfval;
    u32 concat;
    u32 puVar4;
    u32 puVar5;

    /* SEH frame: prev from fs:[0], handler 0x965e3e, state -1, link */
    frame.prev = (u32)CM_FS(void *, 0);
    frame.handler = (u32)0x00965e3e;
    frame.state = 0xffffffffu;
    CM_FS(seh_frame_t *, 0) = &frame;

    /* byte locals at scratch+0..7 */
    scratch[0] = 2; scratch[1] = 1; scratch[2] = 2; scratch[3] = 1;
    scratch[4] = 2; scratch[5] = 1; scratch[6] = 2; scratch[7] = 1;

    puVar1 = (u32 *)(u32)cm_fn_00944e46(0x20);

    if (puVar1 == (u32 *)0) {
        cm_fn_00944cff(S_00ad572c, 0, 0, (void *)(scratch + 0x214), (void *)(scratch + 0x14));
        puVar4 = (u32)(scratch + 0x214);
        puVar5 = (u32)(scratch + 0x14);
        uVar6 = 0x27c;
    } else {
        puVar1[0] = *(*(u32 **)((u8 *)param_1 + 0xb1));
        puVar1[1] = *(u32 *)(*(u32 *)((u8 *)param_1 + 0xee) + 0x20);
        selfval = *(*(u32 **)((u8 *)param_1 + 0xc));
        puVar1[2] = *(u32 *)(u32)cm_fn_006826d0((void *)selfval, 0);
        puVar1[3] = *(u32 *)(*(u32 *)((u8 *)param_1 + 0xee) + 0x24);
        selfval = *(*(u32 **)((u8 *)param_1 + 0xc));
        puVar1[4] = *(u32 *)(u32)cm_fn_006826d0((void *)selfval, 1);
        puVar1[5] = *(u32 *)(*(u32 *)((u8 *)param_1 + 0xee) + 0x28);
        puVar1[6] = *(u32 *)(*(u32 *)((u8 *)param_1 + 0xb1) + 0x3b);
        puVar1[7] = *(u32 *)(*(u32 *)((u8 *)param_1 + 0xee) + 0x2c);

        {
            u32 *vtbl = *(u32 **)((u8 *)param_1 + 0);
            vt_fn_t vt = (vt_fn_t)vtbl[0x3c / 4];
            uVar6 = vt(param_1, 1, (u32)(scratch + 0xc), (u32)(scratch + 0x8), 0);
        }

        alloc_b2 = (u32)cm_fn_00944cf1(0xb2);
        *(u32 *)(scratch + 0x10) = alloc_b2;
        frame.state = 0;

        if (alloc_b2 == 0) {
            result = 0;
        } else {
            concat = ((((u32)(scratch + 0)) >> 16) & 0xffffu) << 16 |
                     (u32)(u16)(*(u16 *)((u8 *)param_1 + 0x40));
            result = cm_fn_00522e00(alloc_b2,
                param_1,
                8,
                (u32)puVar1,
                *(u32 *)(scratch + 0xc),
                *(u32 *)((u8 *)param_1 + 4),
                uVar6,
                concat,
                1, 1,
                *(u32 *)(scratch + 0x8),
                0x14, 0, 0, 0,
                (u32)(scratch + 0));
        }

        frame.state = 0xffffffffu;
        *(*(u32 **)((u8 *)param_1 + 0xc) + 1) = result;
        cm_fn_009452ca((u32)puVar1);
        cm_fn_009452ca(uVar6);
        v = *(*(u32 **)((u8 *)param_1 + 0xc) + 1);
        if (v != 0) {
            cm_fn_0051c410(v, 0);
            CM_FS(void *, 0) = (void *)frame.prev;
            return;
        }

        cm_fn_00944cff(S_00ad572c, 0, 0, (void *)(scratch + 0x314), (void *)(scratch + 0x114));
        puVar4 = (u32)(scratch + 0x314);
        puVar5 = (u32)(scratch + 0x114);
        uVar6 = 0x297;
    }

    cm_fn_00944c9f((u32)&DAT_00ae24d0, S_009870e8,
                   (u32)cm_fn_0090d130(), puVar4, puVar5, uVar6);
    cm_fn_005e8290(S_009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    CM_FS(void *, 0) = (void *)frame.prev;
}
