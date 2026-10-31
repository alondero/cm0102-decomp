#include "cmre.h"

void CM_THISCALL cm_fn_006f56c0(void *self, float amount, s16 kind)
{
    volatile u8 *object = (volatile u8 *)self;
    volatile float *primary = (volatile float *)(object + 0x4d);
    long double change;
    long double updated;

    if (kind == -1) {
        const volatile float *multiplier;
        float field_value = *primary;
        float threshold = CM_GLOBAL(volatile float, 0x0096d78c);
        if (!(field_value > threshold)) {
            field_value = *primary;
            threshold = CM_GLOBAL(volatile float, 0x0096d788);
            if (!(field_value > threshold)) {
                u32 above;
                field_value = *primary;
                threshold = CM_GLOBAL(volatile float, 0x0096b040);
                above = field_value > threshold;
                change = *(volatile float *)&amount;
                if (!above)
                    multiplier = &CM_GLOBAL(volatile float, 0x00969084);
                else
                    multiplier = &CM_GLOBAL(volatile float, 0x0096d784);
            } else {
                change = *(volatile float *)&amount;
                multiplier = &CM_GLOBAL(volatile float, 0x00968a78);
            }
        } else {
            change = *(volatile float *)&amount;
            multiplier = &CM_GLOBAL(volatile float, 0x0096d690);
        }

        {
            float branch_scale = *multiplier;
            change *= branch_scale;
        }
        {
            float common_scale = CM_GLOBAL(volatile float, 0x0096d780);
            change *= common_scale;
        }
        updated = (long double)*primary - change;
        *primary = (float)updated;
    } else {
        long double penalty;
        const volatile double *scale;
        float lower_bound;

        if (kind == 0x13 || kind == 0x21)
            scale = &CM_GLOBAL(volatile double, 0x0096d770);
        else if (kind == 0x15 || kind == 0x24 || kind == 0x2e)
            scale = &CM_GLOBAL(volatile double, 0x0096d778);
        else
            scale = &CM_GLOBAL(volatile double, 0x0096d768);

        penalty = *(volatile float *)(object + 0xd5);
        {
            double multiplier = CM_GLOBAL(volatile double, 0x0096a790);
            penalty *= multiplier;
        }
        {
            double base = CM_GLOBAL(volatile double, 0x0096a648);
            penalty = (long double)base - penalty;
        }
        {
            double branch_scale = *scale;
            penalty *= branch_scale;
        }
        updated = (long double)*primary - penalty;
        *primary = (float)updated;
        lower_bound = CM_GLOBAL(volatile float, 0x00968970);
        if (!(updated >= (long double)lower_bound))
            *(volatile u32 *)primary = 0;
        return;
    }

    {
        float field_value = *primary;
        float lower_bound = CM_GLOBAL(volatile float, 0x00968970);
        if (!(field_value >= lower_bound))
        *(volatile u32 *)primary = 0;
    }
}
