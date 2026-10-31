#include "cmre.h"

/* Preferred-image VA 008061f0, base 00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Each field address wraps before its null check. Search calls remain
   original boundaries and stop in the original order on the first match. */
extern u32 CM_CDECL cm_fn_00806260(u32 left, u32 right, u32 words);

u32 CM_CDECL cm_fn_008061f0(u32 record, u32 query)
{
    u32 address;
    if (record == 0 || query == 0)
        return 0;
    address = record + 4u;
    if (address != 0 && cm_fn_00806260(address, query, 0) != 0)
        return 1;
    address = record + 0x38u;
    if (address != 0 && cm_fn_00806260(address, query, 0) != 0)
        return 1;
    address = record + 0x53u;
    if (address != 0 && cm_fn_00806260(address, query, 0) != 0)
        return 1;
    return 0;
}
