/* Preferred-image VA 009473A7, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0. Preserve the uninitialized 0x104-byte output buffer, import result
   guard, second-byte test and unsigned first-byte load. The import and
   original 009471CD remain boundaries; no directory/API output is invented. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_009471cd(u32);
typedef u32 (CM_STDCALL *get_directory_009473a7)(u32,u32);
u32 CM_CDECL cm_fn_009473a7(void)
{
    volatile u8 buffer[0x104];
    u32 result=((get_directory_009473a7)
        CM_GLOBAL(volatile u32,0x00967190u))(0x104u,(u32)buffer);
    if (result!=0u && buffer[1]==0x3au) {
        u32 first=buffer[0];
        return cm_fn_009471cd(first)-0x40u;
    }
    return 0u;
}
