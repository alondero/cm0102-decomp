/* Preferred-image VA 0x005a0230, image base 0x00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stdcall with three stack words (RET 0xC); ECX is written before any read, so
   there is no thiscall receiver. The 0x1388-byte frame is a local buffer whose
   address is passed to the formatter and to 0x004AE660. The stack probe at
   0x00945610 reserves the frame, and the C build allocates the local frame
   without a probe dependency (as in fn_005a0380). The second argument is tested
   against zero: zero formats one fixed string, nonzero calls 0x00785830 three
   times on the first argument (indexes 0, 1, 2; the index-1 result is
   discarded, the index-0 result feeds the record stride at 581 bytes, the
   index-2 result feeds the 78-byte stride), then formats with signed bytes at
   offset 0x37 of both records and the pointer at record+4. The third argument
   is the receiver for 0x004AE660, which is called on every path.
   Formatter (0x0066F4E0), object accessor (0x00785830) and 0x004AE660 remain
   original boundaries. Global and string meanings remain unknown. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern char *CM_CDECL cm_fn_0066f4e0(char *dst, const char *fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_005a0230(u32 arg1, u32 arg2, u32 arg3)
{
    u8 buffer[5000];
    u32 first;
    u32 third;
    u32 base1;
    u32 base2;
    u32 entry1;
    u32 entry2;
    s32 value1;
    s32 value2;

    if (arg2 == 0) {
        cm_fn_0066f4e0((char *)buffer, (const char *)0x009c9cf0);
    } else {
        first = (u32)cm_fn_00785830((void *)arg1, 0);
        (void)cm_fn_00785830((void *)arg1, 1);
        third = (u32)cm_fn_00785830((void *)arg1, 2);
        base1 = CM_GLOBAL(u32, 0x00ae23b0);
        base2 = CM_GLOBAL(u32, 0x00ae23b4);
        value1 = *(s8 *)(base2 + first * 581u + 0x37u);
        value2 = *(s8 *)(base1 + third * 78u + 0x37u);
        entry1 = base1 + third * 78u + 4u;
        entry2 = base2 + first * 581u + 4u;
        cm_fn_0066f4e0((char *)buffer, (const char *)0x009c9bf4, value1, value1,
                       value2, value2, entry2, entry1);
    }
    cm_fn_004ae660((void *)arg3, (u32)buffer);
}
