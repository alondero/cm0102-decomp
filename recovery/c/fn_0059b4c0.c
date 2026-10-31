/* Preferred-image VA 0059b4c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_CDECL cm_fn_00669360(u32 club);
extern u32 CM_THISCALL cm_fn_0054cfb0(u32 self);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 low, u32 high);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_0059b4c0(u32 club, u32 output)
{
    u8 work[0x200];
    u32 offset, value, low, high, result, version;
    u16 limit_year, scheduled_year;
    u8 kind, phase, month, distance;
    D(output) = 0;
    kind = cm_fn_00669360(club);
    if ((s8)kind < 0) {
        cm_fn_00944cff(0x009c8784u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1185u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    offset = (u32)(s32)(s8)kind * 72u;
    value = D(0x00b63d77u + offset);
    if (value != 0)
        limit_year = 2002;
    else
        limit_year = (u16)(W(0x00dd6ecau) + 1u);
    if (W(0x00ae2c92u) >= limit_year)
        D(output) = 1;
    phase = B(0x00b63d94u + offset);
    scheduled_year = W(0x00b63d8cu + offset);
    month = (u8)cm_fn_0054cfb0(0x00ae2c90u);
    if ((s8)phase < 2) {
        if (scheduled_year != W(0x00ae2c92u)) {
            if ((s8)month >= (s8)phase)
                return 0;
        } else {
            if ((s32)(s8)month <= 12 - (s32)(s8)phase)
                return 0;
        }
    } else {
        distance = (u8)(phase - month);
        if ((s8)distance > 2 || (s8)distance < 0)
            return 0;
    }
    high = D(0x00ae2c94u);
    low = D(0x00ae2c90u);
    result = cm_fn_0054cdb0(0x00b63d8au + offset, low, high);
    if ((s32)result <= 0)
        return 0;
    return result;
}
