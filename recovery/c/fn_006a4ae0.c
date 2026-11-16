#include "cmre.h"

/*
 * Preferred-image VAs, CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * 006a4ae0/006a4ae5 allocate 0x1388 bytes through stack-probe lowering.
 * Use one uninitialized C object, not a void helper call that loses EAX/ESP.
 * The native compiler owns allocation, probing and its relocated return word.
 * After three saves, [ESP+0xc] is local base L; [ESP+0x1398],
 * [ESP+0x139c], [ESP+0x13a0] are the three incoming argument homes.
 * No original return address or guessed physical C frame is constructed.
 * ECX saved at 006a4af4, forwarded at 006a4bf4; RET 0xc at 006a4c3c.
 * 006a4afa/006a4b05: ECX=EDI, one pushed index.
 * 006a4bf7: four pushes, ECX=EBX, retaining forwarded receiver.
 * 006a4c0a: ECX=EDI, one push; 006a4c2e: ECX=EDI, three pushes.
 * The latter's EAX is preserved through the epilogue.
 */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern char * CM_CDECL cm_fn_0066f4e0(char *, const char *);
extern char * CM_THISCALL cm_fn_00694440(void *, u32, u32, char *, u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, const char *, u32, u32);

u32 CM_THISCALL cm_fn_006a4ae0(void *self, void *input, u32 mode,
                              void *output)
{
    char scratch[0x1388];
    u32 index, flags, base, address;
    const char *text;
    index = (u32)cm_fn_00785830(input, 0);
    flags = (u32)cm_fn_00785830(input, 1);
    if (mode == 0) {
        u32 first = CM_GLOBAL(volatile u32, 0x00ae2350);
        u32 last = CM_GLOBAL(volatile u32, 0x00ae235c);
        if ((s32)index > (s32)(last - first * 2u))
            text = (const char *)0x00a0abcc;
        else if (flags & 0x400u)
            text = (const char *)0x00a0abb8;
        else if (flags & 2u)
            text = (const char *)0x00a0aba0;
        else if (flags & 0x85u)
            text = (const char *)0x00a0ab7c;
        else if (flags & 0x30u)
            text = (const char *)0x00a0ab58;
        else if (flags & 0x308u)
            text = (const char *)0x00a0ab34;
        else
            text = (const char *)0x00a0ab18;
        cm_fn_0066f4e0(scratch, text);
    } else {
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        address = base + index * 0x245u;
        cm_fn_00694440(self, address, flags, scratch, 0x1388);
    }
    cm_fn_004ae660(output, (u32)scratch);
    base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    address = base + index * 0x245u;
    return cm_fn_004ae8a0((u32)output, (const char *)(address + 4u),
                         0x7d5, address);
}
