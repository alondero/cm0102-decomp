/* Preferred-image VA 0049b830, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0049ecd0(u32 key, void *first, void *second, void *third);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flags);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_0049b830(u32 argument)
{
    u8 work[0x20c];
    u32 sign, magnitude, result, value, version, line;
    void *name, *extension;
    D((u32)work) = 0;
    D((u32)work + 4u) = 0;
    if (argument == 0) {
        name = work + 0x10c;
        extension = work + 12;
        line = 0x1bb1u;
        goto error;
    }
    sign = (argument & 0x80000000u) != 0 ? 0xffffffffu : 0;
    magnitude = (argument ^ sign) - sign;
    if ((s32)magnitude < 10000)
        return magnitude;
    result = cm_fn_0049ecd0(argument, work + 8, work, work + 4);
    if (result == 0 && D((u32)work) == 0 && D((u32)work + 4u) == 0) {
        name = work + 12;
        extension = work + 0x10c;
        line = 0x1bbdu;
        goto error;
    }
    result = cm_fn_007fe600(0x00b74450u, 0x0049b990u, 0x0049bef0u, 0, 0, 0);
    if (result == 0)
        return result;
    value = D((u32)work + 8u);
    cm_fn_007ff270(0x00b74450u, 0, value, 0);
    value = D((u32)work);
    cm_fn_007ff270(0x00b74450u, 1, value, 0);
    value = D((u32)work + 4u);
    cm_fn_007ff270(0x00b74450u, 2, value, 0);
    cm_fn_007ff270(0x00b74450u, 3, 0, 0);
    return cm_fn_007ff270(0x00b74450u, 4, 0, 0);
error:
    cm_fn_00944cff(0x0099c15cu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
