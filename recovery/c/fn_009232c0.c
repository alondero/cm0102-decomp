/* Translation at preferred-image VA 0x009232c0, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stack arguments: signed byte at [esp+4] (switch selector, sign-extended), pointer
   at [esp+8] (word at +0x2a, byte at +0x3f); ret 8. Global word at 0x00ae2c90.
   Sole indexed caller 0x00923010 pushes both arguments and consumes AL after the
   call at 0x0092323b. Preserve full EAX, including its partial-byte writes. */
#include "cmre.h"

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
    eax = eax * (u32)ecx;

    /* Unsigned compare: negative selectors fall to the default (dl stays 0x9c). */
    if ((u32)ecx <= 6u) {
        s32 n = (s32)eax;
        /* Original guarded DWORD dispatch-table read at 0x009232e7. */
        u32 target = *(volatile u32 *)(0x00923380u + (u32)ecx * 4u);
        switch (target) {
        case 0x009232fb:
            eax = (u32)(n / 6);
            result_byte = (u8)(n % 6 + 1);
            break;
        case 0x009232ee:
            eax = (u32)(n / 6);
            result_byte = (u8)(n % 6 - 5);
            break;
        case 0x00923307:
            eax = (u32)(n / 7);
            result_byte = (u8)(n % 7 + 7);
            break;
        case 0x00923313:
            /* and/dec/or/inc on the low three bits is the signed remainder by 8. */
            result_byte = (u8)(n % 8 + 0xe);
            break;
        case 0x00923327:
            eax = (u32)(n / 7);
            result_byte = (u8)(n % 7 + 0x16);
            break;
        case 0x00923334:
            eax = (u32)(n / 6);
            result_byte = (u8)(n % 6 + 0x1d);
            break;
        case 0x00923341:
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

    /* 0x00923368 FILD, 0x0092336c FMUL m64; 0x00923372 CALL __ftol
       (0x00945640). ADR 0006: truncate through signed 64 bits, then use EAX. */
    {
        s32 scaled_source = (s32)(s8)result_byte;
        long double t = (long double)scaled_source;
        t = t * CM_GLOBAL(double, 0x009689b0);
        return (u32)(long long)t;
    }
}
