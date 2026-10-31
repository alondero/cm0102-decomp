#include "cmre.h"

u8 CM_STDCALL cm_fn_006cb960(s8 first, s8 second, s8 third)
{
    if (second < 2 || second > 6) return 0;
    if (first == 1) return third >= 10 && third <= 11;
    return third >= 0 && third <= 1;
}
