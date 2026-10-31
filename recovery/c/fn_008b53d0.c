#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x29, 0x04, 0x09, 0x15, 0x16, 0x18 and 0x2e are buffer addresses passed to
   0x00945ce6 and 0x00944cff; 0x93 and 0x97 are dwords set to 1 on success. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

/* 0x00945b00 returns a value that the original keeps in ESI (listing 0x8b53f0),
   so its return width is declared here even though the brief lists void. */
extern u32 CM_CDECL cm_fn_00945b00(u32 param_1, u32 param_2);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_008b53d0(u32 self, u32 arg1)
{
    u32 stream;

    if (arg1 == 0) {
        return 0;
    }

    stream = cm_fn_00945b00(arg1, (u32)&CM_GLOBAL(u8, 0x009a7f28));
    if (stream == 0) {
        return 0;
    }

    if (cm_fn_00945ce6(self + 0x29, 0x6a, 1, stream) != 1) {
        cm_fn_00945a09(stream);
        return 0;
    }
    if (cm_fn_00945ce6(self + 0x04, 1, 5, stream) != 5) {
        cm_fn_00945a09(stream);
        return 0;
    }
    if (cm_fn_00945ce6(self + 0x09, 1, 0x0c, stream) != 0x0c) {
        cm_fn_00945a09(stream);
        return 0;
    }
    if (cm_fn_00945ce6(self + 0x15, 1, 1, stream) != 1) {
        cm_fn_00945a09(stream);
        return 0;
    }
    if (cm_fn_00945ce6(self + 0x16, 2, 1, stream) != 1) {
        cm_fn_00945a09(stream);
        return 0;
    }
    if (cm_fn_00945ce6(self + 0x18, 0x11, 1, stream) != 1) {
        cm_fn_00945a09(stream);
        return 0;
    }

    cm_fn_00945a09(stream);
    cm_fn_00944cff(arg1, 0, 0, (void *)(self + 0x2e), (void *)0);
    OBJ_U32(0x93) = 1;
    OBJ_U32(0x97) = 1;
    return 1;
}
