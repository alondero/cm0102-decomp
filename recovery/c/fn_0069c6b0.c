#include "cmre.h"

/* Pinned SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   VA 0069c6b0 loads EAX=0x1388 and VA 0069c6b5 calls 00945610.
   An ordinary C call cannot represent its caller-frame allocation and return
   slot relocation. The local array below is not an equivalent boundary. */
extern u32 CM_THISCALL cm_fn_00785830(void *, u32);
extern char *cm_fn_0066f4e0(char *, const char *, ...);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, const char *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);

unsigned long long CM_STDCALL cm_fn_0069c6b0(void *arg1, u32 arg2, void *arg3)
{
    char scratch[0x1388];
    u32 index, offset, base, record;
    u8 flag;
    s32 value;
    const char *format;

    index = cm_fn_00785830(arg1, 0);
    flag = (u8)cm_fn_00785830(arg1, 1);
    offset = index * 0x245u;
    base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    value = *(volatile s8 *)(base + offset + 0x37u);
    if (arg2 == 0)
        format = (const char *)(flag ? 0x00a04f48 : 0x00a04f04);
    else
        format = (const char *)(flag ? 0x00a04e70 : 0x00a04df4);
    cm_fn_0066f4e0(scratch, format, value, value, base + offset + 4u);
    record = CM_GLOBAL(volatile u32, 0x00ae23b4) + offset;
    cm_fn_004ae8a0((u32)arg3, (const char *)(record + 4u), 0x7d5, record);
    return cm_fn_004ae660(arg3, (u32)scratch);
}
