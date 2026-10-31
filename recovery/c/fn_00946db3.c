#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00946e84(u32 path);
extern u32 * CM_CDECL cm_fn_0094c3a0(u32 *destination, u32 *source);

typedef u32 (CM_STDCALL *cm_get_current_directory_a)(u32, char *);
typedef u32 (CM_STDCALL *cm_get_full_path_name_a)(const char *, u32, char *,
                                                 char **);

u32 CM_CDECL cm_fn_00946db3(u32 path, void *destination, u32 capacity)
{
    volatile struct {
        u8 buffer[0x104];
        u32 file_part;
    } scratch;
    u32 length;
    u32 needed;

    if (path == 0u) {
        cm_get_current_directory_a get_current_directory =
            (cm_get_current_directory_a)CM_GLOBAL(volatile u32, 0x00967190u);
        length = get_current_directory(0x104u,
                                       (char *)(void *)scratch.buffer);
    } else {
        if (cm_fn_00946e84(path) == 0u) {
            CM_GLOBAL(volatile u32, 0x00de2f90u) = 0x0fu;
            CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x0du;
            return 0u;
        }

        volatile u32 *path_argument_slot = (volatile u32 *)&path;
        volatile u8 *path_argument_bytes = (volatile u8 *)path_argument_slot;
        u8 drive = path_argument_bytes[0];
        path_argument_bytes[3] = 0;
        path_argument_bytes[0] = (u8)(drive + 0x40u);
        path_argument_bytes[1] = 0x3a;
        path_argument_bytes[2] = 0x2e;

        cm_get_full_path_name_a get_full_path_name =
            (cm_get_full_path_name_a)CM_GLOBAL(volatile u32, 0x00967194u);
        length = get_full_path_name((const char *)(const void *)path_argument_bytes,
                                    0x104u, (char *)(void *)scratch.buffer,
                                    (char **)(void *)&scratch.file_part);
    }

    if (length == 0u)
        goto failure;
    needed = length + 1u;
    if (needed >= 0x105u)
        goto failure;

    if (destination == (void *)0) {
        if ((s32)needed <= (s32)capacity)
            needed = capacity;
        unsigned long long allocated = cm_fn_00944e46(needed);
        u32 *output = (u32 *)(u32)allocated;
        if (output == (u32 *)0) {
            CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x0cu;
            return 0u;
        }
        return (u32)cm_fn_0094c3a0(output,
                                   (u32 *)(void *)scratch.buffer);
    }

    if ((s32)needed > (s32)capacity) {
        CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x22u;
        return 0u;
    }
    return (u32)cm_fn_0094c3a0((u32 *)destination,
                               (u32 *)(void *)scratch.buffer);

failure:
    return 0u;
}
