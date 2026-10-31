#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 text);
extern unsigned long long CM_CDECL cm_fn_006181d0(
    u32 source, u32 destination, u32 size, u32 count);
extern char *CM_CDECL cm_fn_0066f4e0(char *destination,
    const char *format, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_006aadb0(void *object, u32 enabled, void *receiver)
{
    volatile u8 text[0x78];
    u32 first;
    u32 second;
    u32 third;
    u32 fourth;
    u32 ebx_value;
    u32 ebp_value;
    u32 table_source;
    u32 text_source;
    s32 country;
    u32 compare_ptr;
    u32 common_base;

    if (enabled == 0) {
        (void)cm_fn_0066f4e0((char *)0x00de1f64u,
            (const char *)0x00a11fbcu);
        (void)cm_fn_004ae660(receiver, 0x00de1f64u);
        return;
    }

    first = (u32)cm_fn_00785830(object, 0);
    second = (u32)cm_fn_00785830(object, 1);
    third = (u32)cm_fn_00785830(object, 2);
    fourth = (u32)cm_fn_00785830(object, 3);
    common_base = CM_GLOBAL(volatile u32, 0x00ae23f4u);
    ebx_value = common_base + second * 0x3cu;
    ebp_value = common_base + third * 0x3cu;
    table_source = CM_GLOBAL(volatile u32, 0x00ae23bcu) + fourth * 0x6eu;
    text_source = CM_GLOBAL(volatile u32, 0x00ae23a8u) + first * 0x122u;
    (void)cm_fn_006181d0(text_source, (u32)(unsigned long)text, 0x78, 9);

    country = (s16)CM_GLOBAL(volatile s16, 0x00b77c7au);
    compare_ptr = CM_GLOBAL(volatile u32, 0x00b74c26u +
        (u32)(country * 0x300));
    if (table_source == compare_ptr) {
        (void)cm_fn_0066f4e0((char *)0x00de1f64u,
            (const char *)0x00a12148u, 100, 100, (const char *)text,
            ebx_value, ebp_value);
    } else {
        (void)cm_fn_0066f4e0((char *)0x00de1f64u,
            (const char *)0x00a11fd8u, 100, 100, 100, (const char *)text,
            ebx_value, ebp_value);
    }
    (void)cm_fn_004ae660(receiver, 0x00de1f64u);
}
