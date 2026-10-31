/* Preferred-image VA 0x005a0380, image base 0x00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stdcall with three stack words (RET 0xC); ECX is written before any read, so
   there is no thiscall receiver. The 0x1388-byte frame is a local buffer whose
   address is passed to the formatter and to 0x004AE660. The stack probe at
   0x00945610 probes stack pages and reserves the frame. The C build allocates
   the local frame on the emulator's mapped stack without a probe dependency.
   Formatter (0x0066F4E0), object accessor (0x00785830)
   and 0x004AE660 remain original boundaries. Global and string meanings remain
   unknown. The 581-byte record stride and byte at offset 0x37 come from the
   listing. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern char *CM_CDECL cm_fn_0066f4e0(char *dst, const char *fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_005a0380(u32 arg1, u32 arg2, u32 arg3)
{
    u8 buffer[5000];
    u32 index;
    u32 base;
    u32 entry;
    s32 value;

    if (arg2 == 0) {
        cm_fn_0066f4e0((char *)buffer, (const char *)0x009c9da0);
    } else {
        index = (u32)cm_fn_00785830((void *)arg1, 0);
        base = CM_GLOBAL(u32, 0x00ae23b4);
        entry = base + index * 581u;
        value = *(s8 *)(entry + 0x37u);
        cm_fn_0066f4e0((char *)buffer, (const char *)0x009c9d10, value, value,
                       entry + 4u);
    }
    cm_fn_004ae660((void *)arg3, (u32)buffer);
}
