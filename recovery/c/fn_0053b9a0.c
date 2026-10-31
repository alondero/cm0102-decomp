#include "cmre.h"
/* Preferred-image VA 0x0053b9a0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

static u32 available(u32 address, u32 count, u32 person)
{
    u32 i, value;
    for (i = 0; i < count; ++i) {
        value = *(volatile u32 *)(address + i * 4u);
        if (value == 0 || value == person)
            return 1;
    }
    return 0;
}

u32 CM_CDECL cm_fn_0053b9a0(u32 club, s8 role, u32 person)
{
    u8 scratch[512];
    u8 *first, *second;
    u32 line, target, value;
    char *version;
    if (club == 0) {
        first = scratch + 256; second = scratch; line = 0x34be;
        goto error;
    }
    if (person == 0) {
        first = scratch; second = scratch + 256; line = 0x34c5;
        goto error;
    }
    if ((u32)((s32)role - 1) > 14u)
        goto invalid_role;
    target = CM_GLOBAL(volatile u32, 0x0053bcc8u + ((u32)(s32)role - 1u) * 4u);
    switch (target) {
    case 0x0053ba3c: return available(club + 0xbfu, 1, person);
    case 0x0053ba5d: return available(club + 0xc3u, 3, person);
    case 0x0053ba97: return available(club + 0xcfu, 1, person);
    case 0x0053ba9f: return available(club + 0xd3u, 1, person);
    case 0x0053baa7: return available(club + 0x19fu, 5, person);
    case 0x0053bae1: return available(club + 0x1b3u, 7, person);
    case 0x0053bb1b: return available(club + 0x1cfu, 3, person);
    case 0x0053bb49: return available(club + 0xd7u, 50, person);
    case 0x0053bb77:
        value = *(volatile u32 *)(club + 0xcfu);
        if (value != 0 && value != person)
            return 0;
        return available(club + 0xd7u, 50, person);
    case 0x0053bbaf:
        value = *(volatile u32 *)(club + 0xd3u);
        if (value != 0 && value != person)
            return 0;
        return available(club + 0xd7u, 50, person);
    case 0x0053bbef:
        if (available(club + 0x19fu, 5, person) == 0)
            return 0;
        return available(club + 0xd7u, 50, person);
    default: break;
    }
invalid_role:
    first = scratch; second = scratch + 256; line = 0x3584;
error:
    cm_fn_00944cff(0x009b5ac8, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
