#include "cmre.h"

typedef u32 (CM_STDCALL *fn_set_current_directory_a)(const char *path);
typedef u32 (CM_STDCALL *fn_get_current_directory_a)(u32 size, char *buffer);
typedef u32 (CM_STDCALL *fn_set_environment_variable_a)(const char *name, const char *value);
typedef u32 (CM_STDCALL *fn_get_last_error)(void);

extern u32 CM_CDECL cm_fn_009533e0(u32 value);
extern unsigned long long CM_CDECL cm_fn_0094b2a0(u32 error);

u32 CM_CDECL cm_fn_00953344(const char *path)
{
    volatile u8 local[0x108];
    fn_set_current_directory_a set_current = CM_GLOBAL(volatile fn_set_current_directory_a, 0x00967140);
    if (set_current(path) == 0) {
        fn_get_last_error get_error = CM_GLOBAL(volatile fn_get_last_error, 0x009670e8);
        u32 error = get_error();
        cm_fn_0094b2a0(error);
        return 0xffffffffu;
    }

    fn_get_current_directory_a get_current = CM_GLOBAL(volatile fn_get_current_directory_a, 0x00967190);
    if (get_current(0x105u, (char *)&local[0]) == 0) {
        fn_get_last_error get_error = CM_GLOBAL(volatile fn_get_last_error, 0x009670e8);
        u32 error = get_error();
        cm_fn_0094b2a0(error);
        return 0xffffffffu;
    }

    u8 first = local[0];
    if (first == 0x5cu || first == 0x2fu) {
        if (first == local[1])
            return 0;
    }

    volatile u8 *argument = (volatile u8 *)&path;
    argument[0] = 0x3d;
    u32 converted = cm_fn_009533e0((u32)first);
    argument[3] = 0;
    argument[1] = (u8)converted;
    argument[2] = 0x3a;

    fn_set_environment_variable_a set_environment = CM_GLOBAL(volatile fn_set_environment_variable_a, 0x00967048);
    if (set_environment((const char *)&path, (const char *)&local[0]) == 0) {
        fn_get_last_error get_error = CM_GLOBAL(volatile fn_get_last_error, 0x009670e8);
        u32 error = get_error();
        cm_fn_0094b2a0(error);
        return 0xffffffffu;
    }
    return 0;
}
