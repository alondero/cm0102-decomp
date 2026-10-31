/* Recovered from official cm0102.exe 3.9.68, SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86
 *
 * VA 0x0090D0A0 (init), VA 0x0090CFC0 (bounded PRNG),
 * VA 0x009469FA (LCG store), VA 0x00946A04 (LCG step).
 * Addresses are preferred-image VAs, image base 0x00400000.
 * Status: decompiled from instructions; compiled in the harness; not
 * integrated into the original process.
 */
#ifndef CM_RNG_H
#define CM_RNG_H

#include <stdint.h>

#define CM_RNG_TABLE_VA 0x00AA2454u
#define CM_RNG_TABLE_END_VA 0x00AD4130u
#define CM_RNG_TABLE_COUNT 51000u
#define CM_RNG_LOG_SIZE 64u

#ifdef _WIN32
#define CM_RNG_API __declspec(dllexport)
#else
#define CM_RNG_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

CM_RNG_API void cm_rng_srand(uint32_t *lcg_seed, uint32_t seed);
CM_RNG_API int32_t cm_rng_rand(uint32_t *lcg_seed);

/* If seed is 0, time0 must return a 32-bit timestamp (original VA 0x00946ebb). */
CM_RNG_API void cm_rng_init(uint32_t seed,
                            uint32_t *lcg_seed,
                            uint32_t *mix,
                            uint32_t *cursor,
                            uint32_t *table_end,
                            char *log,
                            uint32_t log_size,
                            uint32_t (*time0)(void));

CM_RNG_API int32_t cm_rng_random(int32_t bound,
                                 uint32_t *lcg_seed,
                                 uint32_t *mix,
                                 uint32_t *cursor,
                                 uint32_t table_end,
                                 const uint32_t *table);

#ifdef __cplusplus
}
#endif

#endif
