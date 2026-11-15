/* Preferred-image VA 00943960, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL1. A null first home leaves incoming EAX unchanged. The register
   prefix exposes that value without adding a stack argument. Both releases
   remain original boundaries; the second owner load follows the first call.
   Native 009445D0 overwrites ECX and EAX before use on returning paths. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_009445d0(u32,u32);
u32 CM_CDECL __attribute__((regparm(1)))
cm_fn_00943960(u32 entry_eax, volatile u32 object_home)
{
    u32 object=object_home;
    u32 result=entry_eax;
    if (object!=0u) {
        u32 allocation=CM_GLOBAL(volatile u32,object+0xcu);
        u32 owner=CM_GLOBAL(volatile u32,object);
        result=cm_fn_009445d0(owner,allocation);
        owner=CM_GLOBAL(volatile u32,object);
        result=cm_fn_009445d0(owner,object);
    }
    return result;
}
