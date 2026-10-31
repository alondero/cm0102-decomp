#include "cmre.h"

/* Preferred-image VA 0x006d5b50, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: no arguments, plain cdecl, `ret 0`. Walks 18-byte slots from
   0x00b68790 up to 0x00b6b72a. A slot whose dword is neither 0 nor 0x00b68670
   is torn down: for each of its (signed word count) 0x110-byte records, the
   dwords at +0x108 and +0x10c are passed to cm_fn_009452ca when non-zero; then
   the slot's own dword is passed to that callee and cleared. The count word and
   the slot dword are re-read on every pass, as the listing does.
   Afterwards it stores byte 0x00b6b723 = 0 and dword 0x00a1599c = -1. */
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void cm_fn_006d5b50(void)
{
    u8 *slot = (u8 *)0x00b68790;
    s32 index;
    s32 offset;
    u32 value;

    do {
        if (*(u32 *)slot != 0x00b68670 && *(u32 *)slot != 0) {
            index = 0;
            if (0 < *(s16 *)(slot + 4)) {
                offset = 0;
                do {
                    value = *(u32 *)(*(u32 *)slot + offset + 0x108);
                    if (value != 0) {
                        cm_fn_009452ca(value);
                    }
                    value = *(u32 *)(*(u32 *)slot + offset + 0x10c);
                    if (value != 0) {
                        cm_fn_009452ca(value);
                    }
                    index = index + 1;
                    offset = offset + 0x110;
                } while (index < *(s16 *)(slot + 4));
            }
            cm_fn_009452ca(*(u32 *)slot);
            *(u32 *)slot = 0;
        }
        slot = slot + 0x12;
    } while ((u32)slot < 0x00b6b72a);

    CM_GLOBAL(u8, 0x00b6b723) = 0;
    CM_GLOBAL(u32, 0x00a1599c) = 0xffffffff;
}
