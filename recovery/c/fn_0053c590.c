#include "cmre.h"
/* Preferred-image VA 0x0053c590, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00542740(u32);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define G(a) CM_GLOBAL(volatile u32, a)
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

/* The four listing loops differ in load order and the EAX value that survives
   a skipped slot. Keep those distinctions through the full-width return. */
static u32 count_slots(u32 slots, u32 club, u32 filter, u8 mask,
                       u32 style, u8 *count, u32 eax)
{
    u32 i, person, limit, id, self, base;
    for (i = 0; i < 50; ++i) {
        person = D(slots + i * 4u);
        if (person == 0)
            goto next;
        if (style < 2) {
            limit = G(0x00ae2364);
            id = D(person);
        } else {
            id = D(person);
            limit = G(0x00ae2364);
        }
        eax = style == 1 ? limit : id;
        if ((s32)id >= (s32)limit)
            goto next;
        if (mask != 0 && (B(person + 0x5eu) & mask) == 0)
            goto next;
        eax = filter;
        if (eax == 0)
            goto increment;
        self = G(0x00ae1050);
        eax = cm_fn_004cf6d0(self, person);
        if (eax == 0)
            goto increment;
        eax = D(person + 0x39u);
        if (eax == club)
            goto increment;
        self = G(0x00ae1050);
        eax = cm_fn_004cf5a0(self, person);
        G(0x00ae1080) = eax;
        if (eax != 0) {
            id = D(eax + 4u);
            base = G(0x00ae23b4);
            eax = base + id * 581u;
        }
        if (eax == club)
            goto next;
increment:
        ++*count;
next:
        if (style == 3)
            eax = 49u - i;
    }
    return eax;
}

u32 CM_CDECL cm_fn_0053c590(u32 club, u8 mode, u32 filter)
{
    u8 scratch[0x20c];
    u32 other, eax;
    char *version;
    scratch[3] = 0;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 0x10c, scratch + 0x0c);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 0x10c, scratch + 0x0c, 0x382e);
        eax = cm_fn_005e8290((const char *)0x009870e0,
                            (const char *)0x00ae24d0, 0);
        G(0x00b67a34) = 0;
        return eax & 0xffffff00u;
    }
    other = cm_fn_00540a50(club, (u32)(scratch + 8), 1);
    eax = other;
    if (other != 0) {
        if (mode != 2 || D((u32)(scratch + 8)) == 0) {
            eax = count_slots(club + 0xd7u, club, filter, 0, 0, scratch + 3, eax);
            if (mode == 1) {
                eax = D((u32)(scratch + 8));
                if (eax == 0)
                    return scratch[3];
            }
            eax = other;
        }
        eax = count_slots(other + 0xd7u, club, filter, 0, 1, scratch + 3, eax);
    } else {
        eax = cm_fn_00542740(club);
        if (eax != 0)
            eax = count_slots(club + 0xd7u, club, filter, mode, 3, scratch + 3, eax);
        else
            eax = count_slots(club + 0xd7u, club, filter, 0, 2, scratch + 3, eax);
    }
    return (eax & 0xffffff00u) | scratch[3];
}
