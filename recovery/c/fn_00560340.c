#include "cmre.h"

extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* Thiscall with one stack argument (ret 4), a dword. The receiver in ECX has field 0 as a
   base pointer, field 1 as a write offset and field 2 as a limit. */
void CM_THISCALL cm_fn_00560340(int *param_1, u32 param_2)
{
    /* One scratch area for the 0x200 bytes after `sub esp, 0x200`:
       frame[0x000..0x0ff] is the buffer passed as arg5 (L+0),
       frame[0x100..0x1ff] is the buffer passed as arg4 (L+0x100). */
    u8 frame[0x200];
    u32 offset;
    u32 limit;
    char *text;

    offset = (u32)param_1[1];
    limit = (u32)param_1[2];
    if (limit < offset + 4U) {
        cm_fn_00944cff(0x009c3734U, 0, 0, frame + 0x100, frame);
        text = cm_fn_0090d130();
        cm_fn_00944c9f((u32)0x00ae24d0, 0x009870e8U, (u32)text,
                       (u32)(frame + 0x100), (u32)frame, 0x8dU);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(u32, 0x00b67a34) = 0;
        return;
    }
    {
        u32 base;

        base = (u32)param_1[0];
        *(u32 *)(base + offset) = param_2;
        param_1[1] = (int)((u32)param_1[1] + 4U);
    }
}
