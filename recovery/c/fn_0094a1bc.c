#include "cmre.h"
/* WIP: cm_fn_0094a282 observes parent EBP locals and live registers; this C boundary cannot transport that context. */
typedef struct { void *prev; void *scope; void *handler; u32 state; } cm_seh_frame;
extern u32 CM_CDECL cm_fn_00944fbf(u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern void cm_fn_0094a282(void);

u32 CM_CDECL cm_fn_0094a1bc(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7)
{
    volatile cm_seh_frame frame;
    frame.state = 0xffffffffu;
    frame.handler = (void *)0x00971508;
    frame.scope = (void *)0x0094bf5c;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    u32 local_2c = p5;
    u32 zero = 0;
    u32 local_24 = zero;
    u32 esi = p2;
    u32 local_28 = *(volatile u32 *)(esi - 4u);
    u32 old14 = CM_GLOBAL(volatile u32, 0x00de3014);
    u32 old18 = CM_GLOBAL(volatile u32, 0x00de3018);
    u32 edi = p1;
    CM_GLOBAL(volatile u32, 0x00de3014) = edi;
    u32 value18 = p3;
    CM_GLOBAL(volatile u32, 0x00de3018) = value18;
    volatile u32 state = zero;
    state = 1;
    u32 result = cm_fn_00944fbf(esi, p4, p5, p6, p7);
    local_2c = result;
    state = zero;
    state |= 0xffffffffu;
    cm_fn_0094a282();
    result = local_2c;
    (void)old14; (void)old18; (void)local_24; (void)local_28; (void)state;
    CM_FS(void *, 0) = frame.prev;
    return result;
}


