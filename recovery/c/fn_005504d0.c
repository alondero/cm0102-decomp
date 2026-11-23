#include "cmre.h"

extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *, const char *, int);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00932c30(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern void cm_fn_00945934(u32);
extern u32 CM_CDECL cm_fn_00945bfe(u32, u32, u32, u32);

u32 cm_fn_005504d0(u32 a, u32 b, u32 c, u32 d)
{
    u8 scratch[512];
    u32 count = *(volatile u32 *)(d + 0x37);
    u32 result;
    if (*(volatile u32 *)(d + 0x3f) != 1) {
        char *message;
        cm_fn_00944cff(0x009b8dc0, 0, 0, scratch + 256, scratch);
        message = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, message,
                       scratch + 256, scratch, 0x68d);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(u32, 0x00b67a34) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffff);
    }
    if (b)
        result = cm_fn_00932c30(c, 0x3a, count, a);
    else
        result = cm_fn_00945bfe(c, 0x3a, count, a);
    return result == count;
}
