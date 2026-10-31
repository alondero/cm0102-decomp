#include "cmre.h"

typedef s32 (CM_STDCALL *find_first_file_fn)(const char *path, void *find_data);
typedef u32 (CM_STDCALL *get_last_error_fn)(void);
extern u32 CM_CDECL cm_fn_0094697c(void *file_time);
extern u32 *CM_CDECL cm_fn_0094c3a0(u32 *destination, u32 *source);

u32 CM_CDECL cm_fn_009467ce(const char *path, u32 *out)
{
    u8 find_data[0x140];
    u32 handle;

    handle = (u32)((find_first_file_fn)CM_GLOBAL(volatile u32, 0x0096717c))(
        path, find_data);
    if (handle == 0xffffffffu) {
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
        return 0xffffffffu;
    }

    {
        u32 attributes = *(volatile u32 *)(find_data + 0u);
        u32 mask = attributes - 0x80u;
        u32 attributes_again;
        mask = mask != 0u ? 0xffffffffu : 0u;
        attributes_again = *(volatile u32 *)(find_data + 0u);
        *(volatile u32 *)out = mask & attributes_again;
    }
    *(volatile u32 *)(out + 1) = cm_fn_0094697c(find_data + 4u);
    *(volatile u32 *)(out + 2) = cm_fn_0094697c(find_data + 0x0cu);
    *(volatile u32 *)(out + 3) = cm_fn_0094697c(find_data + 0x14u);
    *(volatile u32 *)(out + 4) = *(volatile u32 *)(find_data + 0x20u);
    (void)cm_fn_0094c3a0(out + 5, (u32 *)(find_data + 0x2cu));
    return handle;
}
