#include "cmre.h"

u32 CM_STDCALL cm_fn_004cbb20(u32 arg)
{
    u8 value = (u8)arg;
    return value == 0 || value == 6 || value == 7 || value == 8 ||
           value == 9 || value == 10 || value == 11 || value == 12 ||
           value == 13 || value == 15;
}
