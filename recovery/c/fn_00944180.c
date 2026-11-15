/* Preferred-image VA00944180, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native CDECL2 terminal wrapper. Physical EAX/EDX prefix exposes unchanged
   incoming EDX without adding stack words. Before CALL00944197, EAX is
   buffer+16 and ECX is the cached second word; preserve all three inputs.
   Original0094791C restores EBP and other context from the supplied buffer
   and jumps to its target; it remains an explicit original boundary. */
#include "cmre.h"
extern void CM_CDECL __attribute__((regparm(3),noreturn))
    cm_fn_0094791c(u32,u32,u32,u32,u32);
void CM_CDECL __attribute__((regparm(2),noreturn))
cm_fn_00944180(u32 entry_eax,u32 entry_edx,
               volatile u32 buffer_home,volatile u32 value_home)
{
    u32 buffer=buffer_home;
    u32 value=value_home;
    (void)entry_eax;
    CM_GLOBAL(volatile u32,buffer+0x9cu)=0x5500aaffu;
    cm_fn_0094791c(buffer+0x10u,entry_edx,value,buffer+0x10u,value);
}
