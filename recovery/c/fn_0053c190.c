#include "cmre.h"
/* Preferred-image VA 0x0053c190, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_THISCALL cm_fn_00417c10(u32, void *, u32);
extern u32 CM_CDECL cm_fn_0053b4f0(u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define G(a) CM_GLOBAL(volatile u32, a)
#define D(a) (*(volatile u32 *)(u32)(a))

static u32 remove_slot(u32 address, u32 count, u32 person)
{
    u32 i;
    for (i = 0; i < count; ++i) {
        if (D(address + i * 4u) == person) {
            D(address + i * 4u) = 0;
            return 1;
        }
    }
    return 0;
}

u32 CM_CDECL cm_fn_0053c190(u32 club, u32 person, u32 date,
                           u32 detach, u32 players_only)
{
    u8 scratch[0x208];
    u32 count, total, id, address, role, target, other;
    char *version;
    if (club == 0)
        return 0;
    if (person == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 0x108, scratch + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 0x108, scratch + 8, 0x36ea);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        G(0x00b67a34) = 0;
        return 0;
    }
    if (date != 0) {
        count = G(0x00ae2350);
        total = G(0x00ae235c);
        id = D(club);
        address = person + ((s32)id >= (s32)(total - count * 2u) ? 0x29u : 0x3eu);
        cm_fn_00417c10(address, scratch, date);
    }
    count = G(0x00ae2350);
    id = D(club);
    total = G(0x00ae235c);
    address = person + ((s32)id >= (s32)(total - count * 2u) ? 0x28u : 0x3du);
    role = *(volatile u8 *)address;
    if (players_only != 0)
        role = role == 11 || role == 12 || role == 13 || role == 15 ? 11 : 0;
    role = (u32)(s32)(s8)role;
    if (role > 15u)
        return 0;
    target = G(0x0053c54cu + role * 4u);
    switch (target) {
    case 0x0053c507: break;
    case 0x0053c2a6:
        if (remove_slot(club + 0xbfu, 1, person) == 0) return 0;
        break;
    case 0x0053c2bd:
        if (remove_slot(club + 0xc3u, 3, person) == 0) return 0;
        break;
    case 0x0053c2f4:
        if (remove_slot(club + 0xcfu, 1, person) == 0) return 0;
        count = G(0x00ae2350);
        total = G(0x00ae235c);
        if ((s32)id >= (s32)(total - count * 2u)) {
            count = G(0x00ae2350);
            if ((s32)id < (s32)(total - count)) {
                other = D(club + 0x53u);
                other = cm_fn_0053b4f0(other);
                if (D(other + 0xcfu) == person)
                    D(other + 0xcfu) = 0;
            }
        }
        break;
    case 0x0053c352:
        if (remove_slot(club + 0xd3u, 1, person) == 0) return 0;
        break;
    case 0x0053c369:
        if (remove_slot(club + 0x19fu, 5, person) == 0) return 0;
        break;
    case 0x0053c3a0:
        if (remove_slot(club + 0x1b3u, 7, person) == 0) return 0;
        break;
    case 0x0053c3d7:
        if (remove_slot(club + 0x1cfu, 3, person) == 0) return 0;
        break;
    case 0x0053c40e:
        if (remove_slot(club + 0xd7u, 50, person) == 0) return 0;
        break;
    case 0x0053c430:
        if (remove_slot(club + 0xcfu, 1, person) == 0) return 0;
        if (remove_slot(club + 0xd7u, 50, person) == 0) return 0;
        break;
    case 0x0053c464:
        if (remove_slot(club + 0xd3u, 1, person) == 0) return 0;
        if (remove_slot(club + 0xd7u, 50, person) == 0) return 0;
        break;
    case 0x0053c4a3:
        if (remove_slot(club + 0x19fu, 5, person) == 0) return 0;
        if (remove_slot(club + 0xd7u, 50, person) == 0) return 0;
        break;
    default: return 0;
    }
    if (detach != 0) {
        count = G(0x00ae2350);
        total = G(0x00ae235c);
        id = D(club);
        if ((s32)id >= (s32)(total - count * 2u))
            D(person + 0x24u) = 0;
        else
            D(person + 0x39u) = 0;
    }
    return 1;
}
