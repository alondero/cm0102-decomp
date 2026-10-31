#include "cmre.h"
/* Preferred-image VA 0x00542740, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define G(a) CM_GLOBAL(volatile u32, a)
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_CDECL cm_fn_00542740(u32 club)
{
    u8 scratch[512];
    u32 total, count, id, nation, tag, continent;
    char *version;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 256, scratch, 0x46fa);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        G(0x00b67a34) = 0;
        return 0;
    }
    total = G(0x00ae235c);
    count = G(0x00ae2350);
    id = D(club);
    if ((s32)id >= (s32)(total - count))
        return 0;
    if (cm_fn_00540a50(club, 0, 1) != 0)
        return 0;
    count = G(0x00ae2350);
    total = G(0x00ae235c);
    id = D(club);
    if ((s32)id >= (s32)(total - count * 2u)) {
        nation = D(club + 0x53u);
        if (nation != 0) {
            tag = D(nation);
            if (tag == G(0x009cf488) || tag == G(0x009cf408)
                || tag == G(0x009cf360) || tag == G(0x009cf260)
                || tag == G(0x009cf218))
                return 0;
            continent = D(nation + 0x71u);
            if (continent == 0)
                return 1;
            id = D(continent);
            if (id == G(0x009cfa18) || id == G(0x009cfa08))
                return 0;
            return 1;
        }
    }
    /* The null-nation branch deliberately reloads club+0x53. */
    nation = D(club + 0x53u);
    if (nation == 0)
        return 1;
    tag = D(nation);
    if (tag == G(0x009cf488) || tag == G(0x009cf314)
        || tag == G(0x009cf434))
        return 0;
    return 1;
}
