#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_00945934(u32 param_1);

u32 CM_STDCALL cm_fn_0055f1c0(s16 param_1)
{
    u8 scratch[0x200];
    u32 result;
    char *text;

    result = CM_GLOBAL(u32, 0x00ae2918);
    if (result == 0) {
        result = cm_fn_00945501(0x00074042u, (u32)(s32)param_1);
        CM_GLOBAL(u32, 0x00ae2918) = result;
        if (result == 0) {
            cm_fn_00944cff(0x009c36ecu, result, result,
                           scratch + 0x100, scratch);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                           (u32)(scratch + 0x100), (u32)scratch, 0xc3u);
            cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
            CM_GLOBAL(u32, 0x00b67a34) = 0;
            cm_fn_005cc7d0();
            cm_fn_005a8f60();
            cm_fn_00945934(0xffffffffu);
        }
    }
    return result;
}
