#include "cmre.h"
extern char *CM_CDECL cm_fn_0066f4e0(s32 value, char *text, ...);
void CM_STDCALL cm_fn_00923b60(s32 target, u32 selector_word)
{
    char *text;
    volatile s8 *selector_byte = (volatile s8 *)&selector_word;
    s8 selector = *selector_byte;
    void *extra = (void *)0x00ad9c64;
    switch (selector) {
    case 0: text = (char *)0x00ad4fb4; break;
    case 1: (void)cm_fn_0066f4e0(target, (char *)0x00ad4fac); return;
    case 2: (void)cm_fn_0066f4e0(target, (char *)0x00ad4fa4); return;
    case 4: (void)cm_fn_0066f4e0(target, (char *)0x00ad4f20); return;
    case 8: (void)cm_fn_0066f4e0(target, (char *)0x00ad4f94); return;
    case 16: (void)cm_fn_0066f4e0(target, (char *)0x00ad4f8c); return;
    case 32: (void)cm_fn_0066f4e0(target, (char *)0x00ad4f84); return;
    case 64: text = (char *)0x00ad4f5c; break;
    default: text = (char *)0x00ad4fb4; break;
    }
    (void)cm_fn_0066f4e0(target, text, extra);
}
