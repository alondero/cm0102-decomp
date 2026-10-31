/* Preferred-image VA 00669470, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define LOW(a,b) (((a) & 0xffffff00u) | (u8)(b))
extern u32 CM_CDECL cm_fn_00669ae0(u32 record);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_00669470(u32 record)
{
    u8 work[0x200];
    u32 key, eax, table, value, version;
    u8 index;
    key = D(record + 0x5du);
    if (key != 0) {
        index = 0;
        do {
            eax = (u32)(s32)(s8)index * 9u;
            value = D(0x00b63d60u + eax * 8u);
            if (value == key)
                return LOW(eax, index);
            index = (u8)(index + 1u);
        } while ((s8)index < 35);
        return LOW(eax, 4);
    }
    if (record == 0) {
        cm_fn_00944cff(0x009eaff4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x3d7u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = record;
    } else {
        eax = D(record);
        if (eax == D(0x009cf79cu) || eax == D(0x009cf77cu) ||
            eax == D(0x009cf774u) || eax == D(0x009cf770u) ||
            eax == D(0x009cf780u) || eax == D(0x009cf76cu) ||
            eax == D(0x009cf778u))
            return LOW(eax, 0);
    }
    eax = cm_fn_00669ae0(record);
    if (eax != 0)
        return LOW(eax, 1);
    eax = D(record);
    if (eax == D(0x009cf6e4u) || eax == D(0x009cf7bcu))
        return LOW(eax, 2);
    eax = LOW(eax, 0);
    for (;;) {
        table = (u32)(s32)(s8)(u8)eax * 72u;
        value = D(0x00b63d60u + table);
        if (value == 0) {
            key = D(record + 0x59u);
            if (key != 0 && key == D(0x00b63d64u + table))
                return eax;
        }
        eax = LOW(eax, (u8)(eax + 1u));
        if ((s8)(u8)eax >= 35)
            return LOW(eax, 3);
    }
}
