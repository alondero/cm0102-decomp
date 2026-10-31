#include "cmre.h"
/* Preferred-image VA 0x00517fa0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern u32 CM_THISCALL cm_fn_008af520(void *, u32, u32, u32, u32, u32,
                                    u32, u32, u32, u32, u32);
extern u32 *CM_THISCALL cm_fn_0092fe80(u32 *, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
#define G(a) CM_GLOBAL(volatile u32, a)
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_00517fa0(u32 self, u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 previous = CM_FS(volatile u32, 0);
    u32 context, array, object, group, title, index = 0, i, result;
    char *version;
    static const struct {
        u32 size, group, title, first, second, kind;
    } rows[] = {
        {0x25d,0x009cf8a4,0x009cfe5c,0,0,2},
        {0x02b,0x009cf8a4,0x009cfe58,1500,800,0},
        {0x02b,0x009cf8a4,0x009cfe60,1200,600,1},
        {0x25d,0x009cf8a8,0x009cfe68,0,0,2},
        {0x02b,0x009cf8a8,0x009cfe64,700,400,0},
        {0x02b,0x009cf8a8,0x009cfe6c,700,400,1},
        {0x25d,0x009cf8ac,0x009cfe74,0,0,2},
        {0x02b,0x009cf8ac,0x009cfe70,700,400,0},
        {0x02b,0x009cf8ac,0x009cfe78,700,400,1}
    };
    frame.state = 0xffffffffu;
    frame.handler = 0x0095759e;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;

    context = D(self + 0x13u);
    *(volatile u16 *)(self + 8u) = 9;
    result = (u32)cm_fn_00944e46(36);
    D(self + 4u) = result;
    if (result == 0) {
        cm_fn_00944cff(0x009b5538, 0, 0, scratch + 0x104, scratch + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8,
                      version, scratch + 0x104, scratch + 4, 0x68);
        result = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        G(0x00b67a34) = 0;
    } else {
        for (i = 0; i != 9; ++i) {
            object = (u32)cm_fn_00944cf1(rows[i].size);
            D((u32)scratch) = object;
            frame.state = i;
            if (object != 0) {
                group = G(rows[i].group);
                title = G(rows[i].title);
                if (rows[i].size == 0x25d) {
                    u32 old_index = index;
                    ++index;
                    result = cm_fn_008af520((void *)object, context, old_index,
                                           title, 0, 0, group, 1, 2, argument, 1);
                } else {
                    u32 old_index = index;
                    if (i != 8)
                        ++index;
                    result = (u32)cm_fn_0092fe80((u32 *)object, context, old_index,
                                               title, rows[i].first, rows[i].second,
                                               group, 1, 0, rows[i].kind, argument, 1);
                }
            } else {
                result = 0;
            }
            array = D(self + 4u);
            if (i != 8)
                frame.state = 0xffffffffu;
            D(array + i * 4u) = result;
        }
    }
    previous = frame.prev;
    CM_FS(volatile u32, 0) = previous;
    return result;
}
