#include "cmre.h"

extern char *CM_CDECL cm_fn_0094a80f(const double *value, char *output,
                                      int width, int uppercase_exponent);

char *CM_CDECL cm_fn_0094aa8c(const double *value, char *output,
                             int width, int uppercase_exponent)
{
    char *result;
    u8 flag;
    volatile int saved_uppercase_exponent = uppercase_exponent;

    CM_GLOBAL(volatile u8, 0x00de3028u) = 1;
    result = cm_fn_0094a80f(value, output, width, saved_uppercase_exponent);
    flag = CM_GLOBAL(volatile u8, 0x00de3028u);
    CM_GLOBAL(volatile u8, 0x00de3028u) = flag & 0u;
    return result;
}
