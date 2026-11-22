/* Translation of original VA 0x00905510 (comp/uefa_seeding.cpp, image base 0x00400000).
   Callees remain explicit original-code boundaries. */
#include "cmre.h"

#define DAT_009a7f28 CM_GLOBAL(u8, 0x009a7f28)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 cm_fn_0090bde0(u8 *arg1, u8 *arg2);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text);
extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);

void CM_FASTCALL cm_fn_00905510(u32 param_1)
{
    /* L+0x00 .. L+0x0ff and L+0x100 .. L+0x1ff of the original 0x200 frame */
    u8 scratch[512];
    u32 uVar2;
    volatile u32 *stream;
    u32 iVar4;
    u32 line;
    void *arg4buf;
    void *arg5buf;

    /* push 0x9a7f28; push 0xaa1a50; push 0x987da0; call 0x90bde0; add esp,8
       -> cm_fn_0090bde0 takes two arguments; the first push belongs to 0x945b00 */
    uVar2 = cm_fn_0090bde0((u8 *)0x00987da0, (u8 *)0x00aa1a50);
    /* push eax; call 0x945b00; add esp,8 -> (path = uVar2, mode_text = 0x9a7f28) */
    stream = cm_fn_00945b00(uVar2, (volatile s8 *)&DAT_009a7f28);

    if (stream == (volatile u32 *)0) {
        /* 00905544: lea eax,[esp+0x10]; lea ecx,[esp+0x110]; push eax; push ecx */
        cm_fn_00944cff((u32)0x00aa1a1c, 0, 0,
                       (void *)(scratch + 0x100), (void *)(scratch + 0));
        /* 00905560-00905571: push 0x64f; push [esp+0x10]; push [esp+0x110] */
        line = 0x64f;
        arg4buf = (void *)(scratch + 0x100);
        arg5buf = (void *)(scratch + 0);
        goto common_tail;
    }

    iVar4 = cm_fn_00945ce6((u32)(param_1 + 0x438), 1, 1, (u32)stream);
    if (iVar4 != 1) {
        cm_fn_00944cff((u32)0x00aa1a1c, 0, 0,
                       (void *)(scratch + 0), (void *)(scratch + 0x100));
        line = 0x657;
        arg4buf = (void *)(scratch + 0);
        arg5buf = (void *)(scratch + 0x100);
        goto common_tail;
    }

    /* movsx edx,[ebp] before the call; movsx ecx,[ebp] after the call */
    iVar4 = cm_fn_00945ce6(param_1, 0x12, (u32)(s32)*(s8 *)(param_1 + 0x438), (u32)stream);
    if (iVar4 != (u32)(s32)*(s8 *)(param_1 + 0x438)) {
        cm_fn_00944cff((u32)0x00aa1a1c, 0, 0,
                       (void *)(scratch + 0), (void *)(scratch + 0x100));
        line = 0x660;
        arg4buf = (void *)(scratch + 0);
        arg5buf = (void *)(scratch + 0x100);
        goto common_tail;
    }

    iVar4 = cm_fn_00945ce6((u32)(param_1 + 0x439), 2, 1, (u32)stream);
    if (iVar4 != 1) {
        cm_fn_00944cff((u32)0x00aa1a1c, 0, 0,
                       (void *)(scratch + 0), (void *)(scratch + 0x100));
        line = 0x668;
        arg4buf = (void *)(scratch + 0);
        arg5buf = (void *)(scratch + 0x100);
        goto common_tail;
    }

    iVar4 = cm_fn_00945ce6((u32)(param_1 + 0x43f), 1, 1, (u32)stream);
    if (iVar4 != 1) {
        cm_fn_00944cff((u32)0x00aa1a1c, 0, 0,
                       (void *)(scratch + 0), (void *)(scratch + 0x100));
        line = 0x66f;
        arg4buf = (void *)(scratch + 0);
        arg5buf = (void *)(scratch + 0x100);
        goto common_tail;
    }

    /* mov al,[ebp]; cmp al,bl (bl==0) */
    if (*(u8 *)(param_1 + 0x43f) != 0) {
        /* mov ecx,[edi+0x43b]; movsx eax,al; push esi; push eax; push 4; push ecx */
        iVar4 = cm_fn_00945ce6(*(u32 *)(param_1 + 0x43b), 4,
                               (u32)(s32)*(s8 *)(param_1 + 0x43f), (u32)stream);
        /* movsx edx,[ebp] after the call */
        if (iVar4 != (u32)(s32)*(s8 *)(param_1 + 0x43f)) {
            cm_fn_00944cff((u32)0x00aa1a1c, 0, 0,
                           (void *)(scratch + 0), (void *)(scratch + 0x100));
            line = 0x679;
            arg4buf = (void *)(scratch + 0);
            arg5buf = (void *)(scratch + 0x100);
            goto common_tail;
        }
    }

    /* add edi,0x440; push esi; push 0x32; push 0xc; push edi */
    iVar4 = cm_fn_00945ce6((u32)(param_1 + 0x440), 0xc, 0x32, (u32)stream);
    if (iVar4 == 0x32) {
        cm_fn_00945a09((u32)stream);
        return;
    }
    cm_fn_00944cff((u32)0x00aa1a1c, 0, 0,
                   (void *)(scratch + 0), (void *)(scratch + 0x100));
    line = 0x682;
    arg4buf = (void *)(scratch + 0);
    arg5buf = (void *)(scratch + 0x100);

common_tail:
    /* pushes before 0x90d130 plus its result belong to 0x944c9f (one add esp,0x24) */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8,
                   (u32)cm_fn_0090d130(), (u32)arg4buf, (u32)arg5buf, line);
    /* push ebx; push 0xae24d0; push 0x9870e0; call 0x5e8290 */
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
}
