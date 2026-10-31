#include "cmre.h"

u32 CM_CDECL cm_fn_005481a0(volatile u8 *object)
{
    u32 result = 0;
    if (object == 0) return result;
    u32 nested = *(volatile u32 *)(object + 0x61);
    if (nested == 0) return 0;
    if (*(volatile s8 *)(nested + 0x0f) == 0x14) return 1;

    s8 at18 = *(volatile s8 *)(nested + 0x18);
    s8 at19 = 0;
    s8 dl = 0x0a;
    if (at18 == 0x14) {
        at19 = *(volatile s8 *)(nested + 0x19);
        result = 0x80;
        if (at19 > dl) result = 0x180;
    }
    if (*(volatile s8 *)(nested + 0x17) == 0x14) {
        at19 = *(volatile s8 *)(nested + 0x19);
        result |= 0x800;
        if (at19 > dl) result |= 0x400;
    }
    if (*(volatile s8 *)(nested + 0x19) == 0x14) result |= 0x700;

    s8 at10 = *(volatile s8 *)(nested + 0x10);
    if (at10 == 0x14) result |= 2;
    s8 at11 = *(volatile s8 *)(nested + 0x11);
    if (at11 == 0x14) result |= 4;
    s8 at12 = *(volatile s8 *)(nested + 0x12);
    if (at12 == 0x14) result |= 0x18;
    s8 at13 = *(volatile s8 *)(nested + 0x13);
    if (at13 == 0x14) result |= 0x38;
    s8 at14 = *(volatile s8 *)(nested + 0x14);
    if (at14 == 0x14) result |= 0x30;
    s8 at15 = *(volatile s8 *)(nested + 0x15);
    if (at15 == 0x14) {
        result |= 0x40;
        if (at14 != at15 && at13 != at15 && at12 != at15 &&
            at11 != at15 && at10 != at15)
            result |= 0x700;
    }

    if (result != 0) return result;
    if (at10 >= 0x0f) return 2;
    if (at11 >= 0x0f) return 4;
    if (at12 >= 0x0f) return 0x18;
    if (at13 >= 0x0f) return 0x38;
    if (at14 >= 0x0f) return 0x30;
    if (at15 < 0x0f) return 0x10;
    if (at14 != 0x14 && at13 != 0x14 && at12 != 0x14 &&
        at11 != 0x14 && at10 != 0x14)
        return 0x740;
    return 0x40;
}
