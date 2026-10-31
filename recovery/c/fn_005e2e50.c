#include "cmre.h"
typedef u32 (CM_STDCALL *slot_fn)(u32);

u32 CM_CDECL cm_fn_005e2e50(void)
{
    u32 result = CM_GLOBAL(volatile u32, 0x00aebe0c);
    u32 table;
    if (result) return result;
    result = CM_GLOBAL(volatile u32, 0x00ae31a4);
    if (result) {
        table = *(volatile u32 *)result;
        result = ((slot_fn)*(volatile u32 *)(table + 0x60))(result);
        if (result) {
            result = CM_GLOBAL(volatile u32, 0x00ae31a4);
            table = *(volatile u32 *)result;
            result = ((slot_fn)*(volatile u32 *)(table + 0x6c))(result);
        }
    }
    result = CM_GLOBAL(volatile u32, 0x00aebde4);
    if (result) {
        table = *(volatile u32 *)result;
        result = ((slot_fn)*(volatile u32 *)(table + 0x60))(result);
        if (result) {
            result = CM_GLOBAL(volatile u32, 0x00aebde4);
            table = *(volatile u32 *)result;
            result = ((slot_fn)*(volatile u32 *)(table + 0x6c))(result);
        }
    }
    return result;
}
