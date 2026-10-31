#include "cmre.h"

u32 CM_THISCALL cm_fn_00599890(u32 self, u32 low, u32 high)
{
    u32 a, sum, carry, upper;
    a = *(volatile u32 *)(self + 0x70u);
    sum = a + low; carry = sum < a;
    *(volatile u32 *)(self + 0x70u) = sum;
    upper = *(volatile u32 *)(self + 0x74u);
    *(volatile u32 *)(self + 0x74u) = upper + high + carry;
    a = *(volatile u32 *)(self + 0x110u);
    sum = a + low; carry = sum < a;
    *(volatile u32 *)(self + 0x110u) = sum;
    upper = *(volatile u32 *)(self + 0x114u);
    *(volatile u32 *)(self + 0x114u) = upper + high + carry;
    a = *(volatile u32 *)self;
    sum = a - low; carry = a < low;
    *(volatile u32 *)self = sum;
    upper = *(volatile u32 *)(self + 4u) - high - carry;
    *(volatile u32 *)(self + 4u) = upper;
    return upper;
}
