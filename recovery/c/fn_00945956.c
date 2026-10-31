#include "cmre.h"
extern void cm_fn_009459ef(u32 *param_1, u32 *param_2);
typedef void *(CM_STDCALL *get_process_fn)(void);
typedef int (CM_STDCALL *terminate_fn)(void *, u32);
typedef void (CM_STDCALL *exit_fn)(u32);

void CM_CDECL cm_fn_00945956(u32 code, s32 stop, s32 exit_now)
{
    if (CM_GLOBAL(volatile u32, 0x00de2fd4) == 1u) {
        u32 saved_code = code;
        void *process = ((get_process_fn)CM_GLOBAL(volatile u32, 0x0096716c))();
        ((terminate_fn)CM_GLOBAL(volatile u32, 0x00967168))(process, saved_code);
    }
    CM_GLOBAL(volatile u32, 0x00de2fd0) = 1;
    CM_GLOBAL(volatile u8, 0x00de2fcc) = (u8)exit_now;
    if (stop == 0) {
        u32 lower = CM_GLOBAL(volatile u32, 0x00de4638);
        if (lower != 0) {
            u32 current = CM_GLOBAL(volatile u32, 0x00de4634) - 4u;
            if (current >= lower) {
                for (;;) {
                    u32 target = *(volatile u32 *)current;
                    if (target != 0) ((void (CM_CDECL *)(void))target)();
                    current -= 4u;
                    lower = CM_GLOBAL(volatile u32, 0x00de4638);
                    if (current < lower) break;
                }
            }
        }
        cm_fn_009459ef((u32 *)0x009870bc, (u32 *)0x009870c4);
    }
    cm_fn_009459ef((u32 *)0x009870c8, (u32 *)0x009870d0);
    if (exit_now == 0) {
        CM_GLOBAL(volatile u32, 0x00de2fd4) = 1;
        ((exit_fn)CM_GLOBAL(volatile u32, 0x00967164))(code);
    }
}
