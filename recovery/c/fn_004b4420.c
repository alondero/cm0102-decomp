/* Preferred-image VA004b4420/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL4/full final EAX. regparm(3) transports the initial ECX high word
   retained by MOV CX and the first full-width SUB/PUSH pair. The other
   incoming volatile registers are dead. Original formatter/text/list calls
   remain boundaries. Later 4BA8F0 inputs consume only their low WORD.
   Scratch is the original single 1F4-byte allocation, left uninitialized. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_CDECL cm_fn_004bfa30(u32 nation, u32 second, u32 first,
                                   u8 mode, u32 output, u32 size);
extern u32 CM_CDECL cm_fn_004ba8f0(s16 index, u8 mode);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output, u32 format, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 owner, u32 text);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 owner, u32 text, u32 tag, u32 value);

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_004b4420(
    u32 eax_in __attribute__((unused)), u32 edx_in __attribute__((unused)),
    u32 ecx_in, volatile u32 index_arg, u32 record, volatile u8 mode_arg,
    volatile u32 owner_arg)
{
    u8 scratch[0x1f4];
    u32 local = (u32)scratch, ecx, nation, first, second, offset, base, owner, format;
    s32 remainder = 0;
    u16 word = W(record + 0x30u), other, rounded;
    u8 currency, mode;
    s32 club_currency, match_currency;
    ecx = (ecx_in & 0xffff0000u) | word;
    if ((s16)word < 1000) remainder = (s32)(s16)word % 10;
    ecx -= (u32)remainder;
    nation = D(record + 0x14u);
    first = ecx;
    ecx = (ecx & 0xffff0000u) | W(record + 0x32u);
    second = ecx;
    cm_fn_004bfa30(nation, second, first, 2, local, 0x1f4);

    word = W(record + 0x30u);
    remainder = 0;
    if ((s16)word < 1000) remainder = (s32)(s16)word % 10;
    other = W(record + 0x32u);
    rounded = (u16)((u32)word - (u32)remainder);
    nation = D(record + 0x14u);
    if (rounded != 0) currency = (u8)cm_fn_004ba8f0((s16)rounded, 0);
    else if (other != 0) currency = (u8)cm_fn_004ba8f0((s16)other, 0);
    else if (nation != 0) currency = B(nation + 0x52u);
    else currency = 1;

    mode = mode_arg;
    format = (mode == 2 || mode == 4 || mode == 0xff) ? 0x009a3dbcu : 0x009a3e1cu;
    offset = index_arg * 581u;
    match_currency = (s8)currency;
    base = D(0x00ae23b4);
    club_currency = (s8)B(base + offset + 0x52u);
    cm_fn_0066f4e0(0x00de1f64, format, club_currency, club_currency,
                   match_currency, match_currency, base + offset + 0x38u, local);
    owner = owner_arg;
    (void)cm_fn_004ae660(owner, 0x00de1f64);
    base = D(0x00ae23b4);
    (void)cm_fn_004ae8a0(owner, base + offset + 0x38u, 0x7d5, base + offset);
    nation = D(record + 0x14u);
    return cm_fn_004ae8a0(owner, local, 0x7d0, nation);
}
