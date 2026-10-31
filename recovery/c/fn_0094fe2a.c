#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_0094fded(u32 index);
typedef u32 (CM_STDCALL *flush_fn)(u32);
typedef u32 (CM_STDCALL *error_fn)(void);

u32 CM_CDECL cm_fn_0094fe2a(u32 index)
{
    u32 table, result;
    if (index >= CM_GLOBAL(volatile u32, 0x00de43e0)) goto fail;
    table = *(volatile u32 *)(0x00de42e0u + (u32)((s32)index >> 5) * 4);
    if (!(*(volatile u8 *)(table + (index & 31) * 8 + 4) & 1)) goto fail;
    result = (u32)cm_fn_0094fded(index);
    result = ((flush_fn)CM_GLOBAL(volatile u32, 0x00967064))(result);
    if (!result)
        result = ((error_fn)CM_GLOBAL(volatile u32, 0x009670e8))();
    else
        result = 0;
    if (!result) return 0;
    CM_GLOBAL(volatile u32, 0x00de2f90) = result;
fail:
    CM_GLOBAL(volatile u32, 0x00de2f8c) = 9;
    return 0xffffffff;
}
