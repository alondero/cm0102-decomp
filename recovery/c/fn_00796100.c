/* Preferred-image VA 0x00796100, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: stdcall, three stack arguments (pointer a, pointer b, selector byte),
   ret 0xc, byte result in AL. The selector is sign-extended and compared
   unsigned against 4 (ja), so negative or > 4 values take the default (1).
   Jump table at 0x00796198: 0 -> 1;
   1 -> 0 if dword b+0x61 is 0, else (signed byte b+0x18 < 0x18);
   2 -> 0 if dword b+0x61 is 0; else 0 when dword b+0x39 is nonzero and equals
        dword a+0x39, otherwise 1;
   3 -> 1 when dword b+0x39 is nonzero and equals dword a+0x39, otherwise 0;
   4 -> 0 if dword b+0x61 is 0, else as case 3.
   Field meanings remain unknown. */
#include "cmre.h"

u8 CM_STDCALL cm_fn_00796100(u8 *a, u8 *b, s8 selector)
{
    u32 value;

    switch ((u32)(s32)selector) {
    case 1:
        if (*(u32 *)(b + 0x61) == 0) {
            return 0;
        }
        return *(s8 *)(b + 0x18) < 0x18;
    case 2:
        if (*(u32 *)(b + 0x61) == 0) {
            return 0;
        }
        value = *(u32 *)(b + 0x39);
        if (value != 0 && *(u32 *)(a + 0x39) == value) {
            return 0;
        }
        return 1;
    case 3:
        value = *(u32 *)(b + 0x39);
        if (value != 0 && *(u32 *)(a + 0x39) == value) {
            return 1;
        }
        return 0;
    case 4:
        if (*(u32 *)(b + 0x61) == 0) {
            return 0;
        }
        value = *(u32 *)(b + 0x39);
        if (value != 0 && *(u32 *)(a + 0x39) == value) {
            return 1;
        }
        return 0;
    default:
        return 1;
    }
}
