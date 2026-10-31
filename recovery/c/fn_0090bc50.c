#include "cmre.h"

/* Original routine 0x0090bc50 (preferred-image VA, image base 0x00400000).
   Convention: ret 0 (caller cleans up), so cdecl with one pointer argument
   (arg1, read from [entry+4]). The routine returns the first matching
   constant of a fixed chain of comparisons of arg1 against data addresses
   through 0x00951c50; the constants are returned as addresses, not read. */

extern u32 * cm_fn_00945a60(u32 *param_1, char *param_2);
extern int cm_fn_00947480(u8 *param_1);
extern s32 CM_CDECL cm_fn_00951c50(const volatile u8 *first, const volatile u8 *second);

CM_CDECL u32 cm_fn_0090bc50(u8 *arg1)
{
    u32 eax_value;
    u32 esi_value;

    eax_value = (u32)cm_fn_00945a60((u32 *)0x00ae2c98u, (char *)0x00ad4190u);
    if (eax_value == 0) {
        esi_value = (u32)cm_fn_00947480(arg1);
        if (esi_value != 0) {
            return esi_value;
        }
    }

    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009c3718u) == 0) {
        return 0x009c3824u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009b5348u) == 0) {
        return 0x009c3824u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x00987da0u) == 0) {
        return 0x00ad9c64u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009b8df0u) == 0) {
        return 0x00ad9c64u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009cd36cu) == 0) {
        return 0x00ad9c64u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009ccae4u) == 0) {
        return 0x009c3824u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x00ad4180u) == 0) {
        return 0x009898f4u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009cedb8u) == 0) {
        return 0x00ad4178u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009899e0u) == 0) {
        return 0x009c3840u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x009c382cu) == 0) {
        return 0x009c3838u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x00ad416cu) == 0) {
        return 0x00ad4164u;
    }
    if (cm_fn_00951c50(arg1, (const volatile u8 *)0x00a80204u) == 0) {
        return 0x00ad415cu;
    }
    return 0;
}
