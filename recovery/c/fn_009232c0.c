/* Translation at preferred-image VA 0x009232c0, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stack arguments: signed byte at [esp+4] (switch selector, sign-extended), pointer
   at [esp+8] (word at +0x2a, byte at +0x3f); ret 8. Global word at 0x00ae2c90.
   Float tail: fild/fmul by the double at 0x009689b0, then a no-argument call to the
   original helper at 0x00945640 that consumes ST(0). That operand transport is not
   expressible in plain C; see the WIP marker. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00945640(void);

u32 CM_STDCALL cm_fn_009232c0(s8 selector, u32 record)
{
    s32 ecx = (s32)selector;
    u32 eax;
    u8 result_byte = 0x9c;
    u8 tail_byte;

    if (record == 0)
        eax = (u32)(s32)CM_GLOBAL(volatile s16, 0x00ae2c90);
    else
        eax = *(volatile u16 *)(record + 0x2a);
    eax = (u32)((s32)eax * ecx);

    /* Unsigned compare: negative selectors fall to the default (dl stays 0x9c). */
    if ((u32)ecx <= 6u) {
        s32 n = (s32)eax;
        switch (ecx) {
        case 0:
            eax = (u32)(n / 6);
            result_byte = (u8)(n % 6 + 1);
            break;
        case 1:
            eax = (u32)(n / 6);
            result_byte = (u8)(n % 6 - 5);
            break;
        case 2:
            eax = (u32)(n / 7);
            result_byte = (u8)(n % 7 + 7);
            break;
        case 3:
            /* and/dec/or/inc on the low three bits is the signed remainder by 8. */
            result_byte = (u8)(n % 8 + 0xe);
            break;
        case 4:
            eax = (u32)(n / 7);
            result_byte = (u8)(n % 7 + 0x16);
            break;
        case 5:
            eax = (u32)(n / 6);
            result_byte = (u8)(n % 6 + 0x1d);
            break;
        case 6:
            eax = (u32)(n / 6);
            result_byte = (u8)(n % 6 + 0x23);
            break;
        default:
            break;
        }
    }

    /* mov al, [record+0x3f]: with a null record this reads address 0x3f (original fault). */
    tail_byte = *(volatile u8 *)(record + 0x3f);
    eax = (eax & 0xffffff00u) | tail_byte;
    if (tail_byte != 2) {
        eax = (eax & 0xffffff00u) | result_byte;
        return eax;
    }
    if ((s8)result_byte < 10) {
        eax = (eax & 0xffffff00u) | result_byte;
        return eax - 2u;
    }

    /* WIP: float tail. The x87 operand (ST(0) = (double)(s8)result_byte * 0x009689b0)
       must reach cm_fn_00945640 implicitly; plain C has no way to supply it. */
    {
        s32 scaled_source = (s32)(s8)result_byte;
        double scaled = (double)scaled_source * CM_GLOBAL(double, 0x009689b0);
        (void)scaled;
        return (u32)cm_fn_00945640();
    }
}
