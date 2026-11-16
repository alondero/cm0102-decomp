#include "cmre.h"

/* Pinned SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   preferred-image VAs 0x008aee04-0x008aee0c push three arguments and call;
   0x008aee11-0x008aee17 return without caller stack adjustment, with ret 4. */
extern void * CM_STDCALL cm_fn_00944c46(char *, int, int);

u32 CM_STDCALL cm_fn_008aee00(u32 arg)
{
    return cm_fn_00944c46((char *)(arg + 4u), 4, 2) != (void *)0;
}
