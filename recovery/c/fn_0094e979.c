#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern unsigned long long CM_CDECL cm_fn_0094ab50(u32 destination, u32 source, u32 length);

typedef u16 * (CM_STDCALL *cm_get_environment_w_fn)(void);
typedef u8 * (CM_STDCALL *cm_get_environment_a_fn)(void);
typedef int (CM_STDCALL *cm_wide_to_multi_fn)(u32, u32, const u16 *, s32,
                                              u8 *, s32, const u8 *, u32 *);
typedef int (CM_STDCALL *cm_free_environment_w_fn)(u16 *);
typedef int (CM_STDCALL *cm_free_environment_a_fn)(u8 *);

u8 * CM_CDECL cm_fn_0094e979(void)
{
    s32 mode = CM_GLOBAL(volatile s32, 0x00de3250);
    u32 get_w_address = CM_GLOBAL(volatile u32, 0x009670fc);
    u16 *wide_environment = 0;
    u8 *ansi_environment = 0;
    u8 *result;
    cm_get_environment_w_fn get_w = (cm_get_environment_w_fn)get_w_address;

    if (mode == 0) {
        wide_environment = get_w();
        if (wide_environment != 0) {
            CM_GLOBAL(volatile s32, 0x00de3250) = 1;
            goto wide_path;
        }

        ansi_environment =
            ((cm_get_environment_a_fn)CM_GLOBAL(volatile u32, 0x00967084))();
        if (ansi_environment == 0)
            return 0;
        CM_GLOBAL(volatile s32, 0x00de3250) = 2;
        goto ansi_path;
    }

    if (mode == 1)
        goto wide_path;
    if (mode != 2)
        return 0;
    goto ansi_path;

wide_path:
    if (wide_environment == 0) {
        wide_environment = get_w();
        if (wide_environment == 0)
            return 0;
    }

    {
        volatile u16 *scan = (volatile u16 *)wide_environment;
        u32 length;
        s32 byte_count;
        u8 *allocated = 0;
        cm_wide_to_multi_fn convert;

        if (*scan != 0) {
            do {
                ++scan;
            } while (*scan != 0);
            ++scan;
            while (*scan != 0) {
                do {
                    ++scan;
                } while (*scan != 0);
                ++scan;
            }
        }
        length = (u32)(((s32)((u32)scan - (u32)wide_environment) >> 1) + 1u);
        convert = (cm_wide_to_multi_fn)CM_GLOBAL(volatile u32, 0x009670d8);
        byte_count = convert(0, 0, wide_environment, (s32)length, 0, 0, 0, 0);
        if (byte_count != 0) {
            allocated = (u8 *)(u32)cm_fn_00944e46((u32)byte_count);
            if (allocated != 0) {
                if (convert(0, 0, wide_environment, (s32)length,
                            allocated, byte_count, 0, 0) == 0) {
                    (void)cm_fn_009452ca((u32)allocated);
                    allocated = 0;
                }
            }
        }
        ((cm_free_environment_w_fn)CM_GLOBAL(volatile u32, 0x00967088))(wide_environment);
        result = allocated;
    }
    return result;

ansi_path:
    if (ansi_environment == 0) {
        ansi_environment =
            ((cm_get_environment_a_fn)CM_GLOBAL(volatile u32, 0x00967084))();
        if (ansi_environment == 0)
            return 0;
    }

    {
        volatile u8 *scan = (volatile u8 *)ansi_environment;
        u32 length;
        u8 *allocated;

        if (*scan != 0) {
            do {
                ++scan;
            } while (*scan != 0);
            ++scan;
            while (*scan != 0) {
                do {
                    ++scan;
                } while (*scan != 0);
                ++scan;
            }
        }
        length = (u32)scan - (u32)ansi_environment + 1u;
        allocated = (u8 *)(u32)cm_fn_00944e46(length);
        if (allocated != 0)
            (void)cm_fn_0094ab50((u32)allocated, (u32)ansi_environment, length);
        ((cm_free_environment_a_fn)CM_GLOBAL(volatile u32, 0x0096708c))(ansi_environment);
        return allocated;
    }
}
