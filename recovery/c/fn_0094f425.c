/* Preferred-image VA 0094F425, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL1 exception filter. Native 0094A58C is terminal (its final path
   reaches original 0094F564); it remains an original boundary. The callback
   slot is reloaded after validation. CALL0094F45D has no caller stack repair,
   so the indirect target pops its one stack argument. Native exception
   dispatch across these boundaries is not established by returning fixtures. */
#include "cmre.h"
extern void CM_CDECL __attribute__((noreturn)) cm_fn_0094a58c(void);
extern u32 CM_CDECL cm_fn_0094f4c1(u32);
typedef u32 (CM_STDCALL *filter_0094f425)(u32);
u32 CM_STDCALL cm_fn_0094f425(volatile u32 argument_home)
{
    u32 argument=argument_home;
    u32 record=CM_GLOBAL(volatile u32,argument);
    u32 target;
    if (CM_GLOBAL(volatile u32,record)==0xe06d7363u &&
        CM_GLOBAL(volatile u32,record+0x10u)==3u &&
        CM_GLOBAL(volatile u32,record+0x14u)==0x19930520u)
        cm_fn_0094a58c();
    target=CM_GLOBAL(volatile u32,0x00de325cu);
    if (target!=0u && cm_fn_0094f4c1(target)!=0u) {
        volatile u32 result;
        target=CM_GLOBAL(volatile u32,0x00de325cu);
        result=((filter_0094f425)target)(argument);
        return result;
    }
    return 0u;
}
