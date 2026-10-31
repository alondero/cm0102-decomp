#include "cmre.h"

u8 CM_CDECL cm_fn_005d61d0(s8 first, s8 second, s8 minimum, s8 maximum)
{
    s16 sum = (s16)first + (s16)second;
    if ((s16)minimum > sum) return (u8)minimum;
    if ((s16)maximum < sum) return (u8)maximum;
    return (u8)sum;
}
