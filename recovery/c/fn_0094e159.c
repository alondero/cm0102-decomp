#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 address);
extern u32 CM_CDECL cm_fn_00946da0(u32 destination, u32 source);
extern unsigned long long CM_CDECL cm_fn_0094b2a0(u32 error);
typedef u32 (CM_STDCALL *get_full_path_fn)(const char *, u32, char *, char **);
typedef u32 (CM_STDCALL *get_last_error_fn)(void);

char *CM_CDECL cm_fn_0094e159(char *buffer, const char *filename, u32 capacity)
{
    if (filename == 0 || *(volatile u8 *)filename == 0) {
        return (char *)(u32)cm_fn_00946da0((u32)buffer, (u32)capacity);
    }
    char *target = buffer;
    if (target == 0) {
        target = (char *)(u32)cm_fn_00944e46(0x104u);
        if (target == 0) {
            CM_GLOBAL(volatile u32, 0x00de2f8c) = 0x0cu;
            return 0;
        }
        capacity = 0x104u;
    }
    u32 length = ((get_full_path_fn)CM_GLOBAL(volatile u32, 0x00967194))(
        filename, capacity, target, (char **)&filename);
    if (length < capacity) {
        if (length != 0) return target;
        if (buffer == 0) (void)cm_fn_009452ca((u32)target);
        u32 error = ((get_last_error_fn)CM_GLOBAL(volatile u32, 0x009670e8))();
        (void)cm_fn_0094b2a0(error);
        return 0;
    }
    if (buffer == 0) (void)cm_fn_009452ca((u32)target);
    CM_GLOBAL(volatile u32, 0x00de2f8c) = 0x22u;
    return 0;
}

