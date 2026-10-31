#include "cmre.h"

u32 CM_CDECL cm_fn_004b01d0(s32 value)
{
    if (value < 1000 || value > 0x3fb) {
        if (value >= 0x473 && value <= 0x474) return 0x410;
        if (value >= 0x475 && value <= 0x479) return 0x47a;
        if (value >= 0x3fd && value <= 0x406) return 0x0a;
        if ((value >= 0x407 && value <= 0x40b) || value == 0x472)
            return 0x14;
        if (value >= 0x47e && value <= 0x47f) return 0x1e;
        if (value < 0x41f || value > 0x42e) {
            if (value >= 0x40c && value <= 0x40e) return 0x82;
            if (value != 0x413 && value != 0x414 &&
                value != 0x47b && value != 0x3fc) {
                if (value >= 0x43d && value <= 0x444) return 0x449;
                if (value >= 0x445 && value <= 0x448) return 0x44a;
                if (value >= 0x44d && value <= 0x450) return 0x451;
                return 0xffffffffu;
            }
        }
    }
    return 0x41c;
}