#include "cmre.h"
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

void CM_THISCALL cm_fn_00567ff0(u32 *obj, u8 *dest)
{
    /* 0056805e reads frame+0, never written by this routine. The stale
       caller stack word is not representable as a defined plain-C input.
       Preferred-image VAs; SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
    u8 frame[0x204];
    u32 pos = *(volatile u32 *)(obj + 1);
    u32 end = *(volatile u32 *)(obj + 2);
    u32 next = pos + 2;
    s32 length;
    u32 source, count, i;
    if (next > end) {
        char *text;
        cm_fn_00944cff(0x9c3734, 0, 0, frame + 0x104, frame + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, text, frame + 0x104, frame + 4, 0x90);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        length = (s16)*(u16 *)frame;
        CM_GLOBAL(u32, 0xb67a34) = 0;
    } else {
        u32 base = *(volatile u32 *)obj;
        length = (s16)*(volatile u16 *)(base + pos);
        *(volatile u32 *)(obj + 1) = next;
    }
    source = *(volatile u32 *)obj;
    pos = *(volatile u32 *)(obj + 1);
    source += pos;
    count = (u32)length >> 2;
    for (i = 0; i < count; ++i) {
        *(volatile u32 *)dest = *(volatile u32 *)source;
        dest += 4;
        source += 4;
    }
    count = (u32)length & 3;
    for (i = 0; i < count; ++i) {
        *dest++ = *(volatile u8 *)source;
        ++source;
    }
    pos = *(volatile u32 *)(obj + 1);
    *(volatile u32 *)(obj + 1) = pos + (u32)length;
}
