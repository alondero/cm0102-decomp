/* Preferred-image VA 0x00948aeb, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
typedef u32 (CM_STDCALL *heap_create_fn)(u32 flags, u32 initial, u32 maximum);
typedef u32 (CM_STDCALL *heap_destroy_fn)(u32 heap);
extern u32 CM_CDECL cm_fn_009489a3(void);
extern u32 CM_CDECL cm_fn_00948b48(u32 size);
extern unsigned long long CM_CDECL cm_fn_0094968f(void);

u32 CM_CDECL cm_fn_00948aeb(u32 argument)
{
    u32 heap = ((heap_create_fn)D(0x00967110u))(argument == 0, 0x1000u, 0);
    u32 mode, result;
    D(0x00de4400u) = heap;
    if (!heap) return 0;
    mode = cm_fn_009489a3();
    D(0x00de4404u) = mode;
    if (mode == 3) result = cm_fn_00948b48(0x3f8u);
    else if (mode == 2) result = (u32)cm_fn_0094968f();
    else return 1;
    if (result) return 1;
    ((heap_destroy_fn)D(0x00967114u))(D(0x00de4400u));
    return 0;
}
