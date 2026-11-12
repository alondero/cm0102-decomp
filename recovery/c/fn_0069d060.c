#include "cmre.h"
/* CM0102 3.9.68, SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VA 0x0069d060, image base 0x00400000. */
extern u32 CM_CDECL cm_fn_0053ca90(volatile u8 *);
extern u8 CM_CDECL cm_fn_00544820(u32 *);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_THISCALL cm_fn_0069ba50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile s16, (u32)(a))

/* The indexed direct caller overwrites EAX at VA0068fa88. */
void CM_THISCALL cm_fn_0069d060(u32 self)
{
    u32 club = D(0x00ae23b4);
    u32 limit = D(0x00ae235c);
    volatile u32 iteration = 0;
    volatile u32 direction = 0;
    u8 text[0x200];
    u32 club_name, list, value, cursor, id;
    char *version;
    if ((s32)limit <= 0)
        return;
    club_name = club + 4;
    do {
        if (cm_fn_0053ca90((volatile u8 *)club) == 0 &&
            cm_fn_00544820((u32 *)club) != 0) {
            value = cm_fn_00540a50(club, (u32)&direction, 1);
            if (value == 0 || direction != 0) {
                list = D(self + 8);
                value = D(list + 2);
                D(list + 0xa) = value;
                cursor = 0;
                list = D(self + 8);
                if (W(list) > 0) {
                    do {
                        value = cm_fn_0068d1b0(list);
                        if (value == 0)
                            goto diagnostic;
                        id = D(value + 0xe);
                        value = D(club);
                        if (id == value)
                            goto next_club;
                        list = D(self + 8);
                        ++cursor;
                    } while ((s32)cursor < (s32)W(list));
                }
                cm_fn_00944c9f(0x00de1f64, 0x00a05298, club_name);
                cm_fn_0069ba50(self, club, 0);
            }
        }
next_club:
        value = iteration;
        limit = D(0x00ae235c);
        ++value;
        club += 0x245;
        club_name += 0x245;
        iteration = value;
    } while ((s32)value < (s32)limit);
    return;
diagnostic:
    cm_fn_00944cff(0x009f18a4, 0, 0, text + 0x100, text);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                  text + 0x100, text, 0x311e);
    cm_fn_005e8290((const char *)0x009870e0,
                  (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}
