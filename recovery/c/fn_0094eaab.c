#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_0094eae4(u32 diagnostic_word);

unsigned long long CM_CDECL __attribute__((regparm(2)))
cm_fn_0094eaab(u32 incoming_eax, u32 incoming_edx)
{
    u32 mode = CM_GLOBAL(volatile u32, 0x00de3000u);
    u32 callback;
    unsigned long long result;
    (void)incoming_eax;

    if (mode != 1u && (mode != 0u ||
        CM_GLOBAL(volatile u32, 0x00ad6c44u) != 1u))
        return ((unsigned long long)incoming_edx << 32) | mode;

    cm_fn_0094eae4(0xfcu);
    callback = CM_GLOBAL(volatile u32, 0x00de3254u);
    if (callback != 0u)
        ((void (CM_CDECL *)(void))callback)();
    result = cm_fn_0094eae4(0xffu);
    return result;
}
