#include "cm_rng.h"

#include <stdio.h>

/* IEEE-754 encoding of the qword at VA 0x00970A70; equals 1/65535. */
static const union {
    uint64_t bits;
    double value;
} k_large_bound_scale = {0x3EF0001000100010ull};

static int32_t idiv_remainder(int32_t dividend, int32_t divisor)
{
    /* x86 IDIV and C99 both truncate toward zero. */
    return dividend % divisor;
}

void cm_rng_srand(uint32_t *lcg_seed, uint32_t seed)
{
    *lcg_seed = seed;
}

int32_t cm_rng_rand(uint32_t *lcg_seed)
{
    uint32_t seed = (*lcg_seed * 0x343FDu) + 0x269EC3u;
    *lcg_seed = seed;
    return ((int32_t)seed >> 16) & 0x7FFF;
}

void cm_rng_init(uint32_t seed,
                 uint32_t *lcg_seed,
                 uint32_t *mix,
                 uint32_t *cursor,
                 uint32_t *table_end,
                 char *log,
                 uint32_t log_size,
                 uint32_t (*time0)(void))
{
    if (seed == 0u) {
        seed = time0();
    }
    cm_rng_srand(lcg_seed, seed);
    if (log != 0 && log_size != 0u) {
        snprintf(log, log_size, "Random Number seed: %d", (int32_t)seed);
    }
    {
        int32_t first = cm_rng_rand(lcg_seed);
        *cursor = CM_RNG_TABLE_VA + (uint32_t)idiv_remainder(first, 0xC738) * 4u;
    }
    {
        int32_t second = cm_rng_rand(lcg_seed);
        *table_end = CM_RNG_TABLE_END_VA;
        *mix = (uint32_t)idiv_remainder(second, 0xFFFF);
    }
    if (*cursor < CM_RNG_TABLE_VA || *cursor > CM_RNG_TABLE_END_VA) {
        *cursor = CM_RNG_TABLE_VA;
    }
}

static int32_t table_at(const uint32_t *table, uint32_t cursor)
{
    uint32_t index = (cursor - CM_RNG_TABLE_VA) / 4u;
    return (int32_t)table[index];
}

static int32_t magic_times10_bound(int32_t bound)
{
    int32_t times10 = bound * 10;
    int64_t product = (int64_t)(int32_t)0x80008001 * (int64_t)times10;
    int32_t edx = (int32_t)(product >> 32);
    edx += times10;
    edx >>= 15;
    edx += (int32_t)((uint32_t)edx >> 31);
    return edx;
}

static int32_t magic_div10(int32_t value)
{
    int64_t product = (int64_t)(int32_t)0x66666667 * (int64_t)value;
    int32_t edx = (int32_t)(product >> 32);
    edx >>= 2;
    edx += (int32_t)((uint32_t)edx >> 31);
    return edx;
}

int32_t cm_rng_random(int32_t bound,
                      uint32_t *lcg_seed,
                      uint32_t *mix,
                      uint32_t *cursor,
                      uint32_t table_end,
                      const uint32_t *table)
{
    uint32_t ecx;
    uint32_t eax_mix;

    if (bound == 0) {
        return 0;
    }

    ecx = *cursor + 4u;
    *cursor = ecx;
    if (ecx > table_end) {
        *cursor = CM_RNG_TABLE_VA;
        eax_mix = (uint32_t)cm_rng_rand(lcg_seed) & 0xFFFFu;
        *mix = eax_mix;
        ecx = *cursor;
    } else {
        eax_mix = *mix;
    }

    if (bound <= 0xFFFF && bound >= (int32_t)0xFFFF0001) {
        int32_t sum = (int32_t)(eax_mix + (uint32_t)table_at(table, ecx));
        int32_t rem = (int32_t)((int64_t)sum % (int64_t)bound);
        if (bound <= 0) {
            rem = -rem;
        }
        return rem;
    }

    {
        int32_t table_value = table_at(table, ecx);
        int32_t sum = (int32_t)(eax_mix + (uint32_t)table_value);
        int32_t remainder = idiv_remainder(sum, 0xFFFF);
        int32_t scaled;
        int32_t recursive_bound;
        int32_t recursive_value;
        int32_t folded;
        double produced;

        produced = (double)remainder * (double)bound * k_large_bound_scale.value;
        scaled = (int32_t)produced;
        recursive_bound = magic_times10_bound(bound);
        recursive_value = cm_rng_random(recursive_bound, lcg_seed, mix, cursor, table_end, table);
        folded = scaled + magic_div10(recursive_value + 5);
        if (folded > bound - 1) {
            folded = bound - 1;
        }
        return folded;
    }
}
