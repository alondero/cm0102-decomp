#include "cmre.h"

typedef s32 (CM_STDCALL *find_next_file_fn)(u32 handle, void *find_data);
typedef u32 (CM_STDCALL *get_last_error_fn)(void);
extern u32 CM_CDECL cm_fn_0094697c(void *file_time);
extern u32 *CM_CDECL cm_fn_0094c3a0(u32 *destination, u32 *source);

s32 CM_CDECL cm_fn_00946898(u32 handle, u32 *out)
{
    u8 find_data[0x140];
    u32 result;

    result = (u32)((find_next_file_fn)CM_GLOBAL(volatile u32, 0x00967180))(
        handle, find_data);
    if (result == 0u) {
        u32 error = ((get_last_error_fn)CM_GLOBAL(volatile u32, 0x009670e8))();
        if (error < 2u) {
            CM_GLOBAL(volatile u32, 0x00de2f8c) = 0x16u;
        } else if (error <= 3u) {
            CM_GLOBAL(volatile u32, 0x00de2f8c) = 2u;
        } else if (error == 8u) {
            CM_GLOBAL(volatile u32, 0x00de2f8c) = 0xcu;
        } else if (error == 0x12u) {
            CM_GLOBAL(volatile u32, 0x00de2f8c) = 2u;
        } else {
            CM_GLOBAL(volatile u32, 0x00de2f8c) = 0x16u;
        }
        return -1;
    }

    result = *(volatile u32 *)(find_data + 0u);
    {
        u32 mask = result - 0x80u;
        u32 attributes;
        mask = mask != 0u ? 0xffffffffu : 0u;
        attributes = *(volatile u32 *)(find_data + 0u);
        result = mask & attributes;
    }
    *(volatile u32 *)out = result;
    *(volatile u32 *)(out + 1) = cm_fn_0094697c(find_data + 4u);
    *(volatile u32 *)(out + 2) = cm_fn_0094697c(find_data + 0x0cu);
    *(volatile u32 *)(out + 3) = cm_fn_0094697c(find_data + 0x14u);
    *(volatile u32 *)(out + 4) = *(volatile u32 *)(find_data + 0x20u);
    (void)cm_fn_0094c3a0(out + 5, (u32 *)(find_data + 0x2cu));
    return 0;
}
