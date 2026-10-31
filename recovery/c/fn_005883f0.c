#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);

/* Sole direct caller overwrites full EAX at VA0078208d. */
void CM_CDECL cm_fn_005883f0(u32 source, u32 mode, u32 output)
{
    u8 scratch[0x198];
    u32 local = (u32)scratch;
    u32 person_id, first, second, third, flag, base, offset, person, result;
    person_id = (u32)cm_fn_00785830(source, 0);
    first = (u32)cm_fn_00785830(source, 1);
    second = (u32)cm_fn_00785830(source, 2);
    third = (u32)cm_fn_00785830(source, 3);
    if (mode != 0) {
        D(local + 4) = third;
        cm_fn_00785830(source, 4);
        flag = (u32)cm_fn_00785830(source, 5);
        third = D(local + 4);
        B(local + 3) = (u8)flag;
        cm_fn_0053e140(first, second, third, local + 8, 400, 1, person_id);
        flag = B(local + 3);
        cm_fn_0066f4e0(0x00de1f64, flag != 0 ? 0x009c5b80 : 0x009c5b00, local + 8);
        cm_fn_004ae660(output, 0x00de1f64);
        third = D(local + 4);
        base = D(0x00ae23bc);
        offset = person_id * 110u;
        person = base + offset;
        result = cm_fn_0053e990(person, first, second, third, 0);
        if (result != 0) {
            base = D(0x00ae23bc);
            cm_fn_004ae8a0(output, local + 8, 2010, base + offset);
        }
        return;
    }
    cm_fn_0053e140(first, second, third, local + 8, 400, 5, person_id);
    cm_fn_0066f4e0(0x00de1f64, 0x009c5ac8, local + 8);
    cm_fn_004ae660(output, 0x00de1f64);
    base = D(0x00ae23bc);
    offset = person_id * 110u;
    person = base + offset;
    result = cm_fn_0053e990(person, first, second, third, 0);
    if (result != 0) {
        base = D(0x00ae23bc);
        cm_fn_004ae8a0(output, local + 8, 2010, base + offset);
    }
}
