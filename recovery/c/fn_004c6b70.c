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

void CM_FASTCALL cm_fn_004c6b70(u8 *self)
{
    u8 scratch[0x200];
    u32 fp;
    u32 stream;
    u32 line;
    u32 arg4;
    u32 arg5;
    u32 r;
    u8 *field;

    fp = cm_fn_0090bde0((u8 *)0x00987da0, (u8 *)0x009a7f10);
    stream = (u32)cm_fn_00945b00(fp, (volatile s8 *)&DAT_009a7f28);

    if (stream == 0) {
        cm_fn_00944cff((u32)0x009a7ed8, 0, 0,
                       (void *)(scratch + 0x100), (void *)(scratch + 0));
        arg4 = (u32)(scratch + 0x100);
        arg5 = (u32)(scratch + 0);
        line = 0x2ee;
    } else {
        field = self + 0x6e;
        if (cm_fn_00945ce6((u32)field, 1, 1, stream) == 1) {
            r = cm_fn_00945ce6((u32)self, 10, (u32)(s8)*field, stream);
            if (r == (u32)(s8)*field) {
                if (cm_fn_00945ce6((u32)(self + 0x6f), 2, 1, stream) != 1) {
                    cm_fn_00944cff((u32)0x009a7ed8, 0, 0,
                                   (void *)(scratch + 0), (void *)(scratch + 0x100));
                    arg4 = (u32)(scratch + 0);
                    arg5 = (u32)(scratch + 0x100);
                    line = 0x307;
                    goto tail;
                }
                if (cm_fn_00945ce6((u32)(self + 0x71), 4, 1, stream) == 1) {
                    cm_fn_00945a09(stream);
                    return;
                }
                cm_fn_00944cff((u32)0x009a7ed8, 0, 0,
                               (void *)(scratch + 0), (void *)(scratch + 0x100));
                arg4 = (u32)(scratch + 0);
                arg5 = (u32)(scratch + 0x100);
                line = 0x30f;
            } else {
                cm_fn_00944cff((u32)0x009a7ed8, 0, 0,
                               (void *)(scratch + 0), (void *)(scratch + 0x100));
                arg4 = (u32)(scratch + 0);
                arg5 = (u32)(scratch + 0x100);
                line = 0x2ff;
            }
        } else {
            cm_fn_00944cff((u32)0x009a7ed8, 0, 0,
                           (void *)(scratch + 0), (void *)(scratch + 0x100));
            arg4 = (u32)(scratch + 0);
            arg5 = (u32)(scratch + 0x100);
            line = 0x2f6;
        }
    }

tail:
    r = (u32)cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8, r, arg4, arg5, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
}
