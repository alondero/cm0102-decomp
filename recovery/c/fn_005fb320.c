#include "cmre.h"

extern u32 *CM_CDECL cm_fn_0090bde0(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_00945b00(u32 first, u32 second);
extern void CM_CDECL cm_fn_00945ce6(u32 first, u32 second, u32 third,
                                    u32 fourth);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  void *local100, void *local0);

void CM_THISCALL cm_fn_005fb320(u32 self)
{
    u8 scratch[0x200];
    u32 first = (u32)cm_fn_0090bde0(0x00987da0u, 0x009cf10cu);
    u32 stream = cm_fn_00945b00(first, 0x009a7f28u);

    if (stream == 0) {
        (void)cm_fn_00944cff(0x009cf0d8u, 0, 0,
                             scratch + 0x100, scratch);
        u32 message = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                             scratch + 0x100, scratch, 0x475u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, (int)stream);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = stream;
        return;
    }

    u32 output = (u32)(self + 4u);
    cm_fn_00945ce6(output, 2, 1, stream);
    s16 count = *(volatile s16 *)(self + 4u);
    u32 root = *(volatile u32 *)self;
    cm_fn_00945ce6(root, 0x22u, (u32)(s32)count, stream);
    (void)cm_fn_00945a09(stream);
}
