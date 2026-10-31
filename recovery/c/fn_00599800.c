#include "cmre.h"

u32 CM_THISCALL cm_fn_00599800(u32 self, u32 low, u32 high)
{
    u32 a, sum, carry, upper;
    a = *(volatile u32 *)(self + 0x78u);
    sum = a + low; carry = sum < a;
    *(volatile u32 *)(self + 0x78u) = sum;
    upper = *(volatile u32 *)(self + 0x7cu);
    *(volatile u32 *)(self + 0x7cu) = upper + high + carry;
    a = *(volatile u32 *)(self + 0x118u);
    sum = a + low; carry = sum < a;
    *(volatile u32 *)(self + 0x118u) = sum;
    upper = *(volatile u32 *)(self + 0x11cu);
    *(volatile u32 *)(self + 0x11cu) = upper + high + carry;
    a = *(volatile u32 *)self;
    sum = a + low; carry = sum < a;
    *(volatile u32 *)self = sum;
    upper = *(volatile u32 *)(self + 4u) + high + carry;
    *(volatile u32 *)(self + 4u) = upper;
    return upper;
}
