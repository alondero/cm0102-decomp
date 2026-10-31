#include "cmre.h"
/* Preferred-image VA 0x007c1350, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_THISCALL cm_fn_004cf6d0(u32, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern s8 CM_CDECL cm_fn_00669360(u32);
extern u32 CM_THISCALL cm_fn_007c5fe0(u32, u32, u32, s8, s8, s32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_007c1350(u32 self, u32 country_record)
{
    u8 scratch[0x20c];
    u32 count, offset, club, base, category, roster, remaining, person, index;
    u32 total, receiver, found;
    s8 country;
    char *version;
    if (country_record == 0) {
        cm_fn_00944cff(0x00a78310, 0, 0, scratch + 0x10c, scratch + 0xc);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x10c, scratch + 0xc, 0x396);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    total = D(0x00ae235c);
    count = 0;
    if ((s32)total <= 0) return 1;
    offset = 0;
    do {
        base = D(0x00ae23b4);
        club = base + offset;
        if (cm_fn_0053b900((u32 *)club) != 0) return 1;
        country = cm_fn_00669360(club);
        category = 0x00b63d60u + (u32)(s32)country * 72u;
        if (category == country_record) {
            roster = club + 0xd7u;
            remaining = 50;
            do {
                person = D(roster);
                if (person != 0) {
                    found = D(person + 0x61u);
                    if (found != 0) {
                        index = D(person);
                        total = D(0x00ae2364);
                        if ((s32)index < (s32)total) {
                            receiver = D(0x00ae1050);
                            found = cm_fn_004cf6d0(receiver, person);
                            if (found == 0 || D(person + 0x39u) == club)
                                cm_fn_007c5fe0(self, person, 0xffffffffu, 0, 0, -1, 0);
                        }
                    }
                }
                roster += 4u;
                --remaining;
            } while (remaining != 0);
        }
        total = D(0x00ae235c);
        ++count;
        offset += 0x245u;
    } while ((s32)count < (s32)total);
    return 1;
}
