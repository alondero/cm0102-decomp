#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern void CM_THISCALL cm_fn_00544f30(u32);
extern u32 CM_CDECL cm_fn_0090cd50(u32);

/* Preferred-image VA00528080/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0/full EAX from original0090CD50. All table rows checked against
   their own original loads/stores. Cached9670F4 is a RET4 boundary; the
   original also supplies the freshly read AE2340 value in ECX. */
u32 CM_CDECL cm_fn_00528080(void)
{
    static const u32 early[3][2] = {
        {0x00ae23a4,0x00ae234c}, {0x00ae23cc,0x00ae2374},
        {0x00ae23e4,0x00ae238c}
    };
    static const u32 fixed[9][2] = {
        {0x00ae23d4,0x00ae237c}, {0x00ae23c8,0x00ae2370},
        {0x00ae2404,0x00ae2400}, {0x00ae23bc,0x00ae2364},
        {0x00ae23c4,0x00ae236c}, {0x00ae23c0,0x00ae2368},
        {0x00ae23e8,0x00ae2390}, {0x00ae23b4,0x00ae235c},
        {0x00ae23f0,0x00ae2398}
    };
    static const u32 clear[25] = {
        0x00ae23f0,0x00ae23f4,0x00ae23f8,0x00ae23ac,0x00ae23e0,
        0x00ae23d0,0x00ae23d8,0x00ae23dc,0x00ae23ec,0x00ae23a8,
        0x00ae23b0,0x00ae23c0,0x00ae23c4,0x00ae2398,0x00ae239c,
        0x00ae23a0,0x00ae2354,0x00ae2388,0x00ae2378,0x00ae2380,
        0x00ae2384,0x00ae2394,0x00ae2350,0x00ae2358,0x00ae2368
    };
    static const u32 late[12][2] = {
        {0x00ae23f4,0x00ae239c}, {0x00ae23f8,0x00ae23a0},
        {0x00ae23ac,0x00ae2354}, {0x00ae23e0,0x00ae2388},
        {0x00ae23d0,0x00ae2378}, {0x00ae23d8,0x00ae2380},
        {0x00ae23dc,0x00ae2384}, {0x00ae23ec,0x00ae2394},
        {0x00ae23a8,0x00ae2350}, {0x00ae23b0,0x00ae2358},
        {0x00ae23c0,0x00ae2368}, {0x00ae23c4,0x00ae236c}
    };
    u32 handle, mode, target, i, object;
    handle = D(0x00ae23a4);
    target = D(0x009670f4);
    for (i = 0; i != 3; ++i) {
        if (i != 0)
            handle = D(early[i][0]);
        if (handle != 0) {
            mode = D(0x00ae2340);
            if (mode != 0)
                ((u32 (CM_THISCALL *)(u32, u32))target)(mode, handle);
            else
                cm_fn_009452ca(handle);
            D(early[i][0]) = 0;
            D(early[i][1]) = 0;
        }
    }
    for (i = 0; i != 8; ++i) {
        handle = D(fixed[i][0]);
        if (handle != 0) {
            cm_fn_009452ca(handle);
            D(fixed[i][0]) = 0;
            D(fixed[i][1]) = 0;
            if (i == 7) {
                D(0x00ae23b8) = 0;
                D(0x00ae2360) = 0;
            }
        }
    }
    if (D(0x00ae2340) != 0) {
        for (i = 0; i != 25; ++i)
            D(clear[i]) = 0;
        D(0x00ae236c) = 0;
    } else {
        handle = D(fixed[8][0]);
        if (handle != 0) {
            cm_fn_009452ca(handle);
            D(fixed[8][0]) = 0;
            D(fixed[8][1]) = 0;
        }
        for (i = 0; i != 12; ++i) {
            handle = D(late[i][0]);
            if (handle != 0) {
                mode = D(0x00ae2340);
                if (mode != 0)
                    ((u32 (CM_THISCALL *)(u32, u32))target)(mode, handle);
                else
                    cm_fn_009452ca(handle);
                D(late[i][0]) = 0;
                D(late[i][1]) = 0;
            }
        }
    }
    handle = D(0x00ae23fc);
    if (handle != 0) {
        cm_fn_009452ca(handle);
        D(0x00ae23fc) = 0;
    }
    object = D(0x00ae2408);
    if (object != 0) {
        cm_fn_00544f30(object);
        cm_fn_00944c94(object);
        D(0x00ae2408) = 0;
    }
    D(0x009b5984) = 0xffffffffu;
    return cm_fn_0090cd50(0x009b5b38);
}
