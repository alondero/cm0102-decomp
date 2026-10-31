#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 address);
extern s32 CM_CDECL cm_fn_0094c0fe(u32 locale, u32 flags, const char *source,
    s32 count, void *destination, s32 capacity, u32 code_page, s32 option);
extern u32 *CM_CDECL cm_fn_0094c3a0(u32 *destination, u32 *source);

char *CM_CDECL cm_fn_009532a6(char *source)
{
    u32 locale = CM_GLOBAL(volatile u32, 0x00de3048);
    u32 temporary = 0;
    if (locale == 0) {
        u8 *cursor = (u8 *)source;
        s8 ch = *(volatile s8 *)cursor;
        while (ch != 0) {
            ch = *(volatile s8 *)cursor;
            if (ch >= 0x41 && ch <= 0x5a)
                *(volatile u8 *)cursor = (u8)(ch + 0x20);
            ++cursor;
            ch = *(volatile s8 *)cursor;
        }
    } else {
        s32 required = cm_fn_0094c0fe(locale, 0x100u, source, -1, 0, 0, 0, 1);
        if (required != 0) {
            temporary = (u32)cm_fn_00944e46((u32)required);
            if (temporary != 0) {
                locale = CM_GLOBAL(volatile u32, 0x00de3048);
                s32 converted = cm_fn_0094c0fe(
                    locale, 0x100u, source, -1, (void *)temporary,
                    required, 0, 1);
                if (converted != 0)
                    (void)cm_fn_0094c3a0((u32 *)source, (u32 *)temporary);
            }
        }
        (void)cm_fn_009452ca(temporary);
    }
    return source;
}
