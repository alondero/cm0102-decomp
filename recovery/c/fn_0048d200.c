#include "cmre.h"

extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* Thiscall with one signed stack argument (ret 4). The receiver in ECX has a pointer at
   field 0x0c (a dword table base) and a signed bound at field 0x30. Index -1 yields the
   receiver itself; indexes 0..bound read one dword from the table. */
u32 CM_THISCALL cm_fn_0048d200(u8 *param_1, s32 param_2)
{
    /* One scratch area for the 0x200 bytes after `sub esp, 0x200` (no pushes before it):
       frame[0x000..0x0ff] is the buffer passed as arg5 (L+0),
       frame[0x100..0x1ff] is the buffer passed as arg4 (L+0x100). */
    u8 frame[0x200];
    s32 bound;
    u32 table;
    char *text;

    if (param_2 >= -1) {
        bound = *(s32 *)(param_1 + 0x30);
        if (param_2 <= bound) {
            if (param_2 == -1) {
                return (u32)param_1;
            }
            table = *(u32 *)(param_1 + 0x0c);
            return *(u32 *)(table + (u32)param_2 * 4U);
        }
    }
    cm_fn_00944cff(0x0099b80cU, 0, 0, frame + 0x100, frame);
    text = cm_fn_0090d130();
    cm_fn_00944c9f((u32)0x00ae24d0, 0x009870e8U, (u32)text,
                   (u32)(frame + 0x100), (u32)frame, 0x36eU);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
    return 0;
}
