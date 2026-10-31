#include "cmre.h"

/* Preferred-image VA 0x00600c90, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Each backedge reloads the current person; neither input flag is cleared. */
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_CDECL cm_fn_005409b0(u32 *);
extern u32 CM_THISCALL cm_fn_005fe040(u32, u32);
extern u32 CM_THISCALL cm_fn_005fd480(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

static void report(u32 filename, u32 extension, u32 line)
{
    char *version;
    cm_fn_00944cff(0x009cf11c, 0, 0, (void *)filename, (void *)extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_CDECL cm_fn_00600c90(u32 club, u32 skip_status, u8 flag)
{
    u8 scratch[512];
    u32 work = (u32)scratch, value, person, receiver, chosen, index, limit, base;
    if (!club) goto invalid;
    for (;;) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        value = D(club + 0xcf);
        if (!value || person != value) goto next_club;
        if (!skip_status) {
            receiver = D(0x00b63c98);
            if (cm_fn_005fe040(receiver, person)) goto next_club;
            receiver = D(0x00b63c98);
            chosen = person;
            if (!chosen) {
                index = (u32)(s32)(s16)W(0x00b77c7a);
                chosen = D(0x00b74c26 + index * 768u);
            }
            if (!chosen) {
                report(work + 256, work, 0x3e7);
            } else {
                value = D(chosen);
                limit = D(0x00ae2364);
                index = value - limit + 16u;
                if ((s32)index >= 0) {
                    limit = D(receiver + 4) - 1u;
                    if ((s32)index <= (s32)limit) {
                        base = D(receiver);
                        if (D(base + index * 42735u + 0x9a01)) goto next_club;
                    }
                }
            }
        }
        if (flag) return 1;
        if (cm_fn_0053b900((u32 *)club)) return 1;
        receiver = D(0x00b63c98);
        if (cm_fn_005fd480(receiver, person)) return 1;
next_club:
        value = cm_fn_005409b0((u32 *)club);
        if (!value) return 0;
        value = D(club + 0x53);
        club = cm_fn_0053b3d0(value);
        if (!club) goto invalid;
    }
invalid:
    report(work, work + 256, 0xb3f);
    return 0;
}
