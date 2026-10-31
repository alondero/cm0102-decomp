#include "cmre.h"

/* Preferred-image VA 0x006618f0, image base 0x00400000, 3.9.68 executable
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The local date text occupies the original 100-byte scratch allocation.
   0x0054cfb0 reads its sole argument from ECX and does not consume EDX. */
extern u32 CM_FASTCALL cm_fn_0054cfb0(u32 date);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32 row, u32 text, u32 size, u32 mode);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output, const char *format, ...);
extern u32 CM_STDCALL cm_fn_008e8c40(u32 date, u32 other);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))

u32 CM_THISCALL cm_fn_006618f0(u32 self, u32 expected_owner, u32 object,
                               u32 output, u32 date, u32 check_date)
{
    u8 date_text[100];
    u32 other, row_index, rows;

    if (date == 0)
        date = 0x00ae2c90u;
    if (check_date && (u8)cm_fn_0054cfb0(date) == 5)
        return 1;

    other = U32_AT(self, 4);
    if (cm_fn_008e8c40(date, other) != 0)
        return 1;
    other = U32_AT(self, 4);
    if (cm_fn_008e8c40(date, other + 0x0cu) != 0)
        return 1;

    if (output != 0) {
        row_index = CM_GLOBAL(volatile u32, 0x009cf360);
        rows = CM_GLOBAL(volatile u32, 0x00ae23a8);
        cm_fn_006181d0(rows + row_index * 0x122u, (u32)date_text, 100, 10);
        if (expected_owner == U32_AT(object, 0x39)) {
            cm_fn_0066f4e0(output, (const char *)0x009cd030u, 100, 100, date_text);
            return 0;
        }
        cm_fn_0066f4e0(output, (const char *)0x009ccfbcu, 100, 100, date_text);
    }
    return 0;
}
