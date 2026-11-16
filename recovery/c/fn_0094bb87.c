/* Preferred-image VA 0x0094bb87, image base 0x00400000, CM 01/02 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/ECX/EDX register inputs, four caller stack words, RET0.

   regparm(3) carries the incoming EAX/EDX/ECX without adding stack
   arguments, so the four original caller stack words stay at
   EBP+8..EBP+0x14 and the three register inputs keep their original
   registers: EAX is the record's first word, ECX its second word and EDX
   the operation code handed to 0x00950ecb.

   Original body (0094bb8d..0094bbc2):
     [EBP-0x20] = EAX,  [EBP-0x1C] = ECX,
     [EBP-0x18] = [EBP+0x10],  [EBP-0x14] = [EBP+0x14],
     [EBP-8]    = the popped ST0 kept as a double,
     call 0x00950ecb(EDX, &[EBP-0x20], &[EBP+8]),
     ST0 reloaded from [EBP-8],
     control word loaded from [EBP+8] when it is not 0x027F. */
#include "cmre.h"

extern void CM_CDECL cm_fn_00950ecb(s32 operation, u32 *record,
                                     u16 *codepage_pointer);

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_0094bb87(
    u32 eax_in, u32 edx_in, u32 ecx_in,
    u16 codepage_arg, u32 unused_arg2, u32 arg3, u32 arg4)
{
    u8 frame[0x20];
    volatile u32 *record = (volatile u32 *)(void *)frame;
    volatile u16 codepage = codepage_arg;

    /* EBP-0x20, EBP-0x1C, EBP-0x18, EBP-0x14 of the original record.
       EBP-0x10..EBP-0x09 stay uninitialised, as in the original. */
    record[0] = eax_in;
    record[1] = ecx_in;
    record[2] = arg3;
    record[3] = arg4;

    /* EBP-8: the ST0 the original pops into a double.  With no x87 value
       live the pop yields the real indefinite value. */
    *(volatile unsigned long long *)(void *)(frame + 0x18) =
        0xfff8000000000000ULL;

    cm_fn_00950ecb((s32)edx_in, (u32 *)(void *)record,
                    (u16 *)(void *)&codepage);

    /* The original loads the control word from the caller word unless it
       already is 0x027F.  The x87 control word cannot be written from plain
       C, so the word is only read here, in the original's order. */
    if (codepage != 0x27fu)
        (void)codepage;

    (void)unused_arg2;

    /* 0x00950ecb hands the record's first word back in EAX; for the record
       word 7 it rewrites that word to 1 before doing so. */
    return record[0];
}
