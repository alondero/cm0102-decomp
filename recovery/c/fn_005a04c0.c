/* fn_005a04c0 -- CM 01/02 3.9.68, preferred-image VA 0x005A04C0, image base
   0x00400000.  Target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Shape (all addresses are preferred-image VAs): the original reserves a
   0x1388-byte scratch buffer S through the stack-probe helper 0x945610. A
   null arg2 formats the string at 0x9c9b40 into S with 0x66f4e0. Otherwise
   it calls the thiscall 0x785830 on arg1 twice, for an index (selector 1)
   and a mode (selector 2, low byte sign-extended). Mode 0 and mode 1 each
   format into S from the 107-byte record dword[0xae23d0] + index * 107 (its
   signed byte at +0x37 passed twice, then entry + 4; mode 0 uses the format
   string at 0x9c9db4, mode 1 the one at 0x9c9e34); any other mode formats
   nothing. Every path finishes
   by calling the thiscall 0x4ae660 on arg3 with S and returns with ret 0xc
   (three stack words, callee cleans).

   Frame arithmetic: S is the ESP right after the __chkstk call; the epilogue
   ("pop esi; add esp,0x1388; ret 0xc") forces S = entry_ESP - 0x1388.  Hence
   [S+0x1390] = arg2, [S+0x138c] = arg1 and [S+0x1394] = arg3, and the scratch
   buffer handed to 0x66f4e0 / 0x4ae660 starts at S (0x1388 bytes).

   0x00945610 decision (HELD item): the call is MSVC's compiler-inserted
   __chkstk frame reservation, not a semantic operation of this function.  It
   takes the allocation size in EAX and reserves/probes the stack, and it is
   not reachable through a C ABI without re-deriving the original's frame
   idiom by hand (the original's "mov eax,0x1388" is the argument-passing
   convention of that helper, and binding it would double-count the frame
   that the C local buffer already reserves).  The probe is therefore
   omitted here and the 0x1388-byte frame is reserved by the local buffer
   instead; no observable behaviour of the function changes, because the only
   effect of __chkstk is the allocation/probing of that frame.  Same decision
   as recovery/c/fn_005a0380.c.  Callee and global meanings stay unknown. */
#include "cmre.h"

/* callee C */
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
/* callee legacy */
extern char *CM_CDECL cm_fn_0066f4e0(char *dst, const char *fmt, ...);
/* callee C */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

/* global 0x009c9b40 string, 0x009c9db4 string, 0x009c9e34 string;
   global 0x00ae23d0 int (4 bytes). */
#define s_009c9b40 ((const char *)0x009c9b40)
#define s_009c9db4 ((const char *)0x009c9db4)
#define s_009c9e34 ((const char *)0x009c9e34)
#define g_00ae23d0 CM_GLOBAL(u32, 0x00ae23d0)

u32 CM_STDCALL cm_fn_005a04c0(u32 arg1, u32 arg2, u32 arg3)
{
    u8 buffer[5000]; /* 0x1388: the original's scratch buffer at frame base S */
    u32 index;
    u32 entry;
    s32 value;

    if (arg2 == 0) {
        cm_fn_0066f4e0((char *)buffer, s_009c9b40);
    } else {
        index = (u32)cm_fn_00785830((void *)arg1, 1);
        value = (s8)cm_fn_00785830((void *)arg1, 2);
        if (value == 0) {
            entry = g_00ae23d0 + index * 107u;
            value = *(s8 *)(entry + 0x37u);
            cm_fn_0066f4e0((char *)buffer, s_009c9db4,
                           value, value, entry + 4u);
        } else if (value == 1) {
            entry = g_00ae23d0 + index * 107u;
            value = *(s8 *)(entry + 0x37u);
            cm_fn_0066f4e0((char *)buffer, s_009c9e34,
                           value, value, entry + 4u);
        }
    }
    return (u32)cm_fn_004ae660((void *)arg3, (u32)buffer);
}
