#include "cmre.h"
extern u8 * CM_CDECL cm_fn_0094fb44(double value);
extern void CM_CDECL cm_fn_0094facd(char *text, int value, int limit);
extern void CM_CDECL cm_fn_0094aab3(u32 object, u32 start, u32 limit);
extern void CM_CDECL cm_fn_0094aa8c(u32 object, u32 start, u32 limit, u32 fallback);

void CM_CDECL cm_fn_0094a9f1(u32 object, s32 start, s32 limit, u32 fallback)
{
    double value = *(volatile double *)object;
    u32 allocated = (u32)cm_fn_0094fb44(value);
    CM_GLOBAL(volatile u32, 0x00de3024) = allocated;
    u32 count = *(volatile u32 *)(allocated + 4u);
    u32 last = count - 1u;
    u32 ebx = (u32)limit;
    CM_GLOBAL(volatile u32, 0x00de302c) = last;
    u32 first = *(volatile u32 *)allocated;
    s32 marker = first == 0x2du;
    u32 offset = (u32)start + (u32)marker;
    u32 esi = offset;
    cm_fn_0094facd((char *)esi, (int)ebx, (int)allocated);
    allocated = CM_GLOBAL(volatile u32, 0x00de3024);
    count = *(volatile u32 *)(allocated + 4u);
    last = count - 1u;
    s32 previous_last = (s32)CM_GLOBAL(volatile u32, 0x00de302c);
    u8 less = previous_last < (s32)last;
    CM_GLOBAL(volatile u8, 0x00de3030) = less;
    count = *(volatile u32 *)(allocated + 4u);
    last = count - 1u;
    CM_GLOBAL(volatile u32, 0x00de302c) = last;
    if ((s32)last < -4 || (s32)last >= (s32)ebx) {
        cm_fn_0094aa8c(object, (u32)start, ebx, fallback);
    } else {
        u8 condition = less;
        if (condition != 0) {
            for (;;) {
                u8 byte = *(volatile u8 *)esi;
                ++esi;
                if (byte == 0) break;
            }
            *(volatile u8 *)(esi - 2u) &= 0;
        }
        cm_fn_0094aab3(object, (u32)start, ebx);
    }
}

