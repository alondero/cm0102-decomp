/* Preferred-image VA 0094A6E8, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0. Original strings at 0097159C/00971580 identify KERNEL32 and
   IsProcessorFeaturePresent. The indirect target pops its one argument:
   native CALL0094A709 is immediately followed by RET, without stack repair.
   Both import slots, the resolved target and fallback remain boundaries. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094a6aa(void);
typedef u32 (CM_STDCALL *module_lookup_0094a6e8)(u32);
typedef u32 (CM_STDCALL *symbol_lookup_0094a6e8)(u32,u32);
typedef u32 (CM_STDCALL *feature_query_0094a6e8)(u32);
u32 CM_CDECL cm_fn_0094a6e8(void)
{
    u32 module=((module_lookup_0094a6e8)
        CM_GLOBAL(volatile u32,0x00967138u))(0x0097159cu);
    if (module!=0u) {
        u32 target=((symbol_lookup_0094a6e8)
            CM_GLOBAL(volatile u32,0x009670b8u))(module,0x00971580u);
        if (target!=0u)
            return ((feature_query_0094a6e8)target)(0u);
    }
    return cm_fn_0094a6aa();
}
