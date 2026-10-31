#include "cmre.h"

extern void CM_CDECL cm_fn_0094ab27(char *output, int width);
extern char *CM_CDECL cm_fn_0094fb44(double value);
extern void CM_CDECL cm_fn_0094facd(char *output, int width, u32 record);
extern char *CM_CDECL cm_fn_0094c3a0(char *destination, const char *source);

char *CM_CDECL cm_fn_0094a80f(const double *value, char *output,
                             int width, int uppercase_exponent)
{
    s32 count = width;
    char *record;
    char *digits;
    char *end;
    u32 output_address = (u32)(unsigned long)output;
    s32 exponent;

    if (CM_GLOBAL(volatile u8, 0x00de3028u) != 0u) {
        record = (char *)(unsigned long)CM_GLOBAL(volatile u32, 0x00de3024u);
        cm_fn_0094ab27((char *)(unsigned long)(output_address +
                           (u32)(*(volatile s32 *)record == 0x2du)),
                       (int)(width > 0));
    } else {
        record = cm_fn_0094fb44(*(const volatile double *)value);
        cm_fn_0094facd((char *)(unsigned long)(output_address +
                           (u32)(*(volatile s32 *)record == 0x2du) +
                           (u32)(width > 0)),
                       (s32)((u32)width + 1u),
                       (u32)(unsigned long)record);
    }

    end = output;
    if (*(volatile s32 *)record == 0x2du) {
        *(volatile u8 *)end = '-';
        ++end;
    }
    if (count > 0) {
        char next = *(volatile u8 *)((u32)(unsigned long)end + 1u);
        *(volatile u8 *)end = (u8)next;
        ++end;
        *(volatile u8 *)end = CM_GLOBAL(volatile u8, 0x00ad91e0u);
    }

    digits = cm_fn_0094c3a0((char *)(unsigned long)(
                                (u32)(unsigned long)end + (u32)count +
                                (u32)(CM_GLOBAL(volatile u8,
                                                0x00de3028u) == 0u)),
                            (const char *)0x009715a8u);
    if (uppercase_exponent != 0)
        *(volatile u8 *)digits = 'E';

    ++digits;
    if (**(volatile char **)(record + 0x0cu) != '0') {
        exponent = (s32)((u32)*(volatile s32 *)(record + 4u) - 1u);
        if (exponent < 0) {
            exponent = (s32)(0u - (u32)exponent);
            *(volatile u8 *)digits = '-';
        }
        ++digits;
        if (exponent >= 100) {
            *(volatile u8 *)digits =
                (u8)(*(volatile u8 *)digits + exponent / 100);
            exponent %= 100;
        }
        ++digits;
        if (exponent >= 10) {
            *(volatile u8 *)digits =
                (u8)(*(volatile u8 *)digits + exponent / 10);
            exponent %= 10;
        }
        *(volatile u8 *)((u32)(unsigned long)digits + 1u) =
            (u8)(*(volatile u8 *)((u32)(unsigned long)digits + 1u) +
                 (u8)exponent);
    }
    return output;
}
