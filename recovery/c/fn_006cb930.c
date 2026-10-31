#include "cmre.h"

u8 CM_STDCALL cm_fn_006cb930(s8 first, s8 second)
{
    if (first == 1) return second < 3;
    if (first == 0) return second > 8;
    return 0;
}
