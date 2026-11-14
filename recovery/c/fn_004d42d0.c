#include "cmre.h"

#define G_ae1050 CM_GLOBAL(u32 *, 0x00ae1050)
#define G_ae1080 CM_GLOBAL(u32, 0x00ae1080)
#define G_ae2364 CM_GLOBAL(s32, 0x00ae2364)
#define G_ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_ae2d0c CM_GLOBAL(u32, 0x00ae2d0c)
#define G_b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_00867b50(u32 self);
extern u32 CM_THISCALL cm_fn_004d1600(u32 self, u32 a1, u32 a2, u32 person,
                                     u32 z, u32 one1, u32 one2);
extern u32 CM_CDECL cm_fn_00544520(u32 person, u32 zero);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 z1, u32 z2, void *b, void *a);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 dest, u32 fmt, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 buffer, u32 zero);

u32 CM_THISCALL cm_fn_004d42d0(u32 self, u32 arg1, u32 arg2, u32 person)
{
    u8 scratch[512];
    u32 p;
    u32 club;
    u32 rec;
    u32 ebx;
    u32 edi;
    u32 t;

    if (person == 0) {
        p = 0;
    } else {
        s32 a = *(s32 *)person;
        if (a < G_ae2364) {
            t = (u32)a * 79u;
            a = *(s32 *)(t + G_ae2d0c);
            if (a < 0)
                p = 0;
            else
                p = (u32)a * 80u + *G_ae1050;
        } else {
            p = 0;
        }
    }
    G_ae1080 = p;
    if (p == 0) {
        club = 0;
    } else {
        t = *(u32 *)(p + 4);
        club = G_ae23b4 + t * 581u;
    }
    if (cm_fn_00600a50(club, 0, 1, 0, 0) == 0) {
        cm_fn_00944cff(0x009a8190, 0, 0, scratch + 0x100, scratch);
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, cm_fn_0090d130(),
                       scratch + 0x100, scratch, 0x1f28);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        G_b67a34 = 0;
        return *(u32 *)(person + 0x52);
    }
    rec = cm_fn_004cf5f0(self, person, club);
    if (rec == 0 || (*(u8 *)(rec + 0x35) & 0x3f) != 2 || *(u8 *)(rec + 0x49) != 0)
        return 0;
    ebx = *(u32 *)(person + 0x52);
    if (cm_fn_00867b50(rec) != 0) {
        s32 cap = *(s32 *)(rec + 0x21);
        if ((s32)ebx >= cap)
            ebx = (u32)cap;
    }
    edi = cm_fn_004d1600(self, arg1, arg2, person, 0, 1, 1);
    if ((s32)edi <= (s32)ebx)
        edi = ebx;
    if (cm_fn_00867b50(rec) != 0) {
        s32 cap = *(s32 *)(rec + 0x21);
        if ((s32)edi >= cap)
            edi = (u32)cap;
    }
    if (cm_fn_00544520(person, 0) != 0)
        return ebx;
    if ((*(u8 *)(rec + 0x4e) & 4) == 0)
        return ebx;
    {
        u8 hi = *(u8 *)(rec + 0x4f) & 0xf0;
        if (hi == 0x10 || hi == 0x50)
            return edi;
        if (hi == 0x20 || hi == 0x30) {
            u32 sum = edi + ebx;
            return (u32)((s32)sum / 2);
        }
    }
    return ebx;
}
