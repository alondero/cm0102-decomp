#include "cmre.h"

/* Single direct caller does not read EAX after the call, so the routine is
   declared void; the MOV EAX, 1 result is not modelled as a return value. */
void CM_CDECL cm_fn_0048ce90(void)
{
}
