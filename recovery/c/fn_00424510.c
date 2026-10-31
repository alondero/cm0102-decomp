#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008af520(u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32);

/* Preferred-image VA00424510/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full EAX. All eleven original initializer tuples independently
   checked. Scratch204: allocated-object slotL0, error extL4/filenameL104.
   Linked SEH9542AA; final state10 remains live until unlink. IDs increment
   only on successful object allocation; the final row does not increment. */
u32 CM_THISCALL cm_fn_00424510(u32 self, volatile u32 stream_arg)
{
    static const struct {
        u32 large, title, nation, first_prize, second_prize, first_flag, third_flag, last_flag;
    } rows[11] = {
        {0,0x009cfbd0,0,6000,1500,1,0,0},
        {0,0x009cfaa4,0,5000,2500,0,0,1},
        {1,0x009cfcf8,0x009cf59c,0,0,1,0,1},
        {0,0x009cfd54,0x009cf59c,3000,1500,1,1,1},
        {0,0x009cfaac,0x009cf59c,5000,1500,1,0,1},
        {0,0x009cfd58,0x009cf59c,4000,1500,1,0,1},
        {1,0x009cfcfc,0x009cf5a0,0,0,1,0,1},
        {0,0x009cfd5c,0x009cf5a0,1500,750,1,1,1},
        {0,0x009cff3c,0x009cf5a0,1000,700,1,0,1},
        {1,0x009cfd60,0x009cf7c4,0,0,1,0,1},
        {0,0x009cfd68,0x009cf7c4,750,350,1,1,1}
    };
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x204 / 4];
    u32 l = (u32)scratch, predecessor, owner, array, p, stream, title, nation;
    u32 result, version, i, id, counter = 0;
    predecessor = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x009542aa;
    frame.prev = predecessor;
    CM_FS(u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 11;
    result = cm_fn_00944e46(0x2c);
    D(self + 4) = result;
    if (result == 0) {
        cm_fn_00944cff(0x00989dfc, 0, 0, l + 0x104, l + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x69);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    p = cm_fn_00944cf1(0x2b);
    D(l) = p;
    stream = stream_arg;
    for (i = 0; i < 11; ++i) {
        if (i != 0) {
            p = cm_fn_00944cf1(rows[i].large ? 0x25d : 0x2b);
            D(l) = p;
        }
        frame.state = i;
        if (p == 0) result = 0;
        else {
            nation = rows[i].nation ? D(rows[i].nation) : 0xffffffffu;
            title = D(rows[i].title);
            id = counter;
            if (i == 0) counter = 1;
            else if (i != 10) ++counter;
            if (rows[i].large)
                result = cm_fn_008af520(p, owner, id, title, 0, 0, nation,
                                        1, 0, stream, rows[i].last_flag);
            else
                result = cm_fn_0092fe80(p, owner, id, title,
                                        rows[i].first_prize, rows[i].second_prize, nation,
                                        rows[i].first_flag, 0, rows[i].third_flag,
                                        stream, rows[i].last_flag);
        }
        array = D(self + 4);
        if (i != 10) frame.state = 0xffffffffu;
        D(array + i * 4u) = result;
    }
done:
    predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return result;
}
