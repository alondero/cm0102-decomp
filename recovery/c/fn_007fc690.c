#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0055f270(u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern void CM_THISCALL cm_fn_0077ac00(u32, u32);
extern u8 CM_THISCALL cm_fn_0077ac30(u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_007ff990(u32, u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

/* The null guard at VA 007fc69c-007fc6a2 preserves incoming EAX;
   plain C cannot express that register input in this thiscall signature.
   Binary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The upper half of the pushed word at 007fc78f-007fc799 is also
   inherited from EDX following the preceding call. */
void CM_THISCALL cm_fn_007fc690(u32 self)
{
    u8 scratch[0x200];
    u32 i;
    if (*(volatile u32 *)(self + 0x3840) == 0)
        return;
    cm_fn_0055f270(self + 0x132ea5);
    if (*(volatile u32 *)(self + 0x3830) == 0)
        return;
    *(volatile u32 *)(self + 0x3830) = 0;
    *(volatile u32 *)(self + 0x3802) = 0;
    if (*(volatile u32 *)(self + 0x3806) == 0) {
        char *text;
        cm_fn_00944cff(0x9c3734, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, text,
                       scratch + 0x100, scratch, 0x85);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        CM_GLOBAL(volatile u32, 0xb67a34) = 0;
    } else {
        u32 p = *(volatile u32 *)(self + 0x37fe);
        *(volatile u8 *)p = 4;
        *(volatile u32 *)(self + 0x3802) =
            *(volatile u32 *)(self + 0x3802) + 1;
    }
    {
        u32 len = *(volatile u32 *)(self + 0x3802);
        u32 p = *(volatile u32 *)(self + 0x37fe);
        u32 net = *(volatile u32 *)(self + 0x3840);
        cm_fn_0077ac30(net, p, len, 0);
    }
    for (i = 0; i < 15; ++i) {
        u32 counter = self + 0x380c + i * 2;
        u32 entry = self + 0xad4 + i * 0x300;
        while (*(volatile s16 *)counter > 0) {
            u32 p = *(volatile u32 *)(entry + 2);
            u16 id;
            u32 net;
            cm_fn_007ff990(self, p, 0xffffffff);
            id = *(volatile u16 *)entry;
            net = *(volatile u32 *)(self + 0x3840);
            cm_fn_0077ac00(net, id);
        }
    }
}
