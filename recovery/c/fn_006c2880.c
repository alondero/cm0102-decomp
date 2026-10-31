#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define F(a) CM_GLOBAL(volatile float, (a))
extern u32 CM_THISCALL cm_fn_00717410(u32);
extern s32 CM_CDECL cm_fn_0090cfc0(s32);

static void cm_multiply(volatile float *slot, u32 address)
{
    long double value = (long double)*slot;
    float factor = F(address);
    *slot = (float)(value * (long double)factor);
}

/* Sole caller006bf268: non-FF path MOVSX EAX,AL at006bf275; FF path
   reaches006bff2a, overwriting full EAX before use. Return width is AL.
   The original argument slot doubles as a persistent FLOAT weight. */
u8 CM_THISCALL cm_fn_006c2880(u32 self, u32 argument)
{
    volatile union { u32 bits; float value; } weight;
    volatile float best = 0.0f;
    volatile u8 best_index = 0xff;
    u32 row = self + (u32)(s32)(s8)argument * 8920u + 0x4796;
    u32 flags, object, index;
    s8 value;
    s32 random;
    long double score;
    weight.bits = argument;
    for (index = 0; index != 20; ++index, row += 0x1be) {
        if ((s8)B(row + 0x19) < 0)
            continue;
        flags = cm_fn_00717410(row);
        if ((flags & 1) != 0)
            continue;
        if ((flags & 2) != 0 || (flags & 4) != 0)
            weight.bits = 0x3f99999a;
        else if ((flags & 8) != 0)
            weight.bits = 0x3f266666;
        else if ((flags & 0x10) != 0)
            weight.bits = 0x3f000000;
        else if ((flags & 0x20) != 0)
            weight.bits = 0x3eb33333;
        else if ((flags & 0x40) != 0)
            weight.bits = (flags & 0x700) != 0 ? 0x3e800000u : 0x3e19999au;
        object = D(row + 0x6d);
        if (object != 0) {
            value = (s8)B(object + 0x44);
            if (value > 15)
                cm_multiply(&weight.value, 0x0096a33c);
            else if (value > 10)
                cm_multiply(&weight.value, 0x0096d3b8);
            else if (value < 5)
                cm_multiply(&weight.value, 0x0096b014);
            value = (s8)B(object + 0x39);
            if (value > 15)
                cm_multiply(&weight.value, 0x0096a33c);
            else if (value < 5)
                cm_multiply(&weight.value, 0x0096b014);
            value = (s8)B(object + 0x3e);
            if (value > 15)
                cm_multiply(&weight.value, 0x0096a33c);
            else if (value < 5)
                cm_multiply(&weight.value, 0x0096b014);
        }
        random = cm_fn_0090cfc0(25);
        score = (long double)random * (long double)weight.value;
        if (best_index == 0xff || score > (long double)best) {
            best = (float)score;
            best_index = (u8)index;
        }
    }
    return best_index;
}
