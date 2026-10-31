#include "cmre.h"

/* Plain cdecl routine (ret 0). Stack arguments:
     [esp+4]  source cursor (ebp+8)
     [esp+8]  output buffer (ebp+0xc)
     [esp+12] signed count, decremented in place (ebp+0x10)
   Behaviour per the listing: skip leading ';', copy one token (plain bytes, or
   bytes inside double quotes) to the output while the count lasts, skip trailing
   ';', write a NUL terminator at the current output position, and return the
   cursor when it moved past the token start, otherwise 0. The count is a stack
   argument slot in the original; it is kept as a local with the same wrap width. */
u32 cm_fn_0095128c(u32 arg_src, u32 arg_dst, u32 arg_count)
{
    u32 ecx;
    u32 eax;
    u32 esi;
    u32 count;
    u8 dl;

    ecx = arg_src;
    while (*(volatile u8 *)ecx == ';') {
        ecx = ecx + 1u;
    }

    count = arg_count - 1u;
    eax = ecx;
    if (count == 0u) {
        goto L_ef;
    }

    dl = *(volatile u8 *)ecx;
    esi = arg_dst;

L_dl_check:
    if (dl == 0u) {
        goto L_e3;
    }
    if (dl == ';') {
        goto L_e3;
    }
    if (dl == '"') {
        goto L_quote;
    }

    /* plain byte */
    *(volatile u8 *)esi = dl;
    esi = esi + 1u;
    ecx = ecx + 1u;
    count = count - 1u;
    if (count == 0u) {
        goto L_eb;
    }
    goto L_dd;

L_quote:
    ecx = ecx + 1u;

L_c1:
    dl = *(volatile u8 *)ecx;
    if (dl == 0u) {
        goto L_d7;
    }
    if (dl == '"') {
        goto L_d7;
    }
    *(volatile u8 *)esi = dl;
    esi = esi + 1u;
    ecx = ecx + 1u;
    count = count - 1u;
    if (count == 0u) {
        goto L_eb;
    }
    goto L_c1;

L_d7:
    if (*(volatile u8 *)ecx != 0u) {
        ecx = ecx + 1u;
    }

L_dd:
    dl = *(volatile u8 *)ecx;
    goto L_dl_check;

L_e3:
    while (*(volatile u8 *)ecx == ';') {
        ecx = ecx + 1u;
    }
    goto L_f2;

L_eb:
    eax = ecx;
    goto L_f2;

L_ef:
    esi = arg_dst;

L_f2:
    {
        u8 old = *(volatile u8 *)esi;
        *(volatile u8 *)esi = (u8)(old & 0u);
    }
    /* sub eax,ecx / neg / sbb / and: nonzero only when eax != ecx */
    return (eax != ecx) ? ecx : 0u;
}
