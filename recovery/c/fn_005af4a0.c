#include "cmre.h"

/* Globals whose VALUES are loaded/stored by original instructions. */
#define G_ae2a48 CM_GLOBAL(u32, 0x00ae2a48)
#define G_ae2a4a CM_GLOBAL(u32, 0x00ae2a4a)
#define G_ae2a52 CM_GLOBAL(u32, 0x00ae2a52)
#define G_b67a34 CM_GLOBAL(u32, 0x00b67a34)

/* Original callee boundaries (signatures from brief). */
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 cm_fn_0090bde0(u8 *arg1, u8 *arg2);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text);
extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);

void cm_fn_005af4a0(void)
{
    /* One scratch allocation representing the 0x204 bytes at L (ESP after
       sub esp,0x204).  local_204=L+0, local_200=L+4, local_100=L+0x104. */
    u8 scratch[0x204];
    u32 *const p_local_204 = (u32 *)(scratch + 0x0);
    u8 *const p_local_200 = scratch + 0x4;
    u8 *const p_local_100 = scratch + 0x104;

    u32 dword_ae2a48;
    volatile u32 *esi;
    u32 iVar3;

    dword_ae2a48 = G_ae2a48;
    *p_local_204 = dword_ae2a48;          /* mov [esp+4],eax -> L+0 */

    if ((u16)dword_ae2a48 != 0) {         /* test ax,ax ; je epilogue */
        u32 path;

        /* push 0x9a7f28 deferred to the next call; 0x90bde0 takes 2 args. */
        path = cm_fn_0090bde0((u8 *)0x00987da0, (u8 *)0x009cc258);
        esi = cm_fn_00945b00(path, (volatile s8 *)0x009a7f28);

        if (esi != (volatile u32 *)0) {
            /* cm_fn_00945ce6(&local_204, 2, 1, esi) */
            cm_fn_00945ce6((u32)p_local_204, 2, 1, (u32)esi);

            /* mov ecx,[0xae2a4a] ; mov [0xae2a52],ecx  (dword copy) */
            G_ae2a52 = G_ae2a4a;

            iVar3 = cm_fn_0068d1b0(0x00ae2a48);   /* mov ecx,0xae2a48 */
            while (iVar3 != 0) {
                u32 rd;
                rd = cm_fn_00945ce6(iVar3, 0xf, 1, (u32)esi);
                if (rd != 1) {
                    cm_fn_00944cff((u32)0x009cc208, 0, 0,
                                   (void *)p_local_100, (void *)p_local_200);
                    /* call 0x90d130 (void) then 0x944c9f with 6 args;
                       single add esp,0x24 covers both 0x944c9f and 0x5e8290. */
                    cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8,
                                   (u32)cm_fn_0090d130(),
                                   (u32)p_local_100, (u32)p_local_200,
                                   0x10e0);
                    cm_fn_005e8290((const char *)0x009870e0,
                                   (const char *)0x00ae24d0, 0);
                    G_b67a34 = 0;
                }
                iVar3 = cm_fn_0068d1b0(0x00ae2a48);
            }
            cm_fn_00945a09((u32)esi);
        }
    }
}
