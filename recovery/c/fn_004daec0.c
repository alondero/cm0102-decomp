#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);

/* Sole caller 00781dca overwrites the full EAX result. */
void CM_STDCALL cm_fn_004daec0(u32 input, u32 mode, u32 output)
{
    u8 scratch[0x130];
    u32 local = (u32)scratch;
    u32 person, first, second, third, club, base, pointer, flag, offset;
    person = (u32)cm_fn_00785830(input, 0);
    first = (u32)cm_fn_00785830(input, 1);
    second = (u32)cm_fn_00785830(input, 2);
    third = (u32)cm_fn_00785830(input, 3);
    if (mode != 0) {
        D(local) = third;
        club = (u32)cm_fn_00785830(input, 4);
        third = D(local);
        cm_fn_0053e140(first, second, third, local + 4, 0x12c, 1, person);
        base = D(0x00ae23b4);
        offset = club * 0x245;
        pointer = offset + base + 4;
        flag = (u32)(s32)(s8)B(offset + base + 0x37);
        cm_fn_0066f4e0(0x00de1f64, 0x009af07c, 0x64, flag, flag,
                      flag, flag, local + 4, pointer, pointer);
        cm_fn_004ae660(output, 0x00de1f64);
        base = D(0x00ae23b4);
        pointer = offset + base;
        cm_fn_004ae8a0(output, pointer + 4, 0x7d5, pointer);
        offset = person * 0x6e;
        third = D(local);
        base = D(0x00ae23bc);
        if (cm_fn_0053e990(offset + base, first, second, third, 0) == 0)
            return;
    } else {
        cm_fn_0053e140(first, second, third, local + 4, 0x12c, 1, person);
        cm_fn_0066f4e0(0x00de1f64, 0x009af040, 0x64, local + 4);
        cm_fn_004ae660(output, 0x00de1f64);
        base = D(0x00ae23bc);
        offset = person * 0x6e;
        if (cm_fn_0053e990(offset + base, first, second, third, 0) == 0)
            return;
    }
    base = D(0x00ae23bc);
    cm_fn_004ae8a0(output, local + 4, 0x7d7, offset + base);
}
