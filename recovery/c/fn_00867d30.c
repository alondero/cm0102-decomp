#include "cmre.h"

/* Preferred-image VA 00867d30, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All 17 indexed callers consume AL (or sign-extend AL before wider use).
   Original native comparisons retain their two stack arguments and receiver.
   Their second argument is unused by the callees but is still read here. */
extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused);
extern u32 CM_THISCALL cm_fn_004a7f20(u32 self, u32 value, u32 unused);

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
/* Every original pair loads the higher-address DWORD first. */
#define BEFORE(a) (unused = D((a) + 4u), value = D(a), \
                   cm_fn_00456740(date, value, unused))
#define EQUAL(a) (unused = D((a) + 4u), value = D(a), \
                  cm_fn_004a7f20(date, value, unused))

u8 CM_THISCALL cm_fn_00867d30(u32 self)
{
    u32 date = self + 0x2du, value, unused, result;
    if ((B(self + 0x35u) & 0x3fu) == 1u)
        return 14;
    if (!BEFORE(0x00ae2c90u))
        return 14;
    if ((B(self + 0x35u) & 0x3fu) != 2u)
        return 0;
    if (BEFORE(0x00ae1090u))
        return 0;
    if (BEFORE(0x00ae1010u)) {
        if (BEFORE(0x00ae1060u)) {
            if (BEFORE(0x00ae1040u)) {
                result = EQUAL(0x00ae1090u);
                return (u8)(2u - (result != 0u));
            }
            result = EQUAL(0x00ae1040u);
            return (u8)(4u - (result != 0u));
        }
        result = EQUAL(0x00ae1060u);
        return (u8)(6u - (result != 0u));
    }
    if (BEFORE(0x00ae1030u)) {
        if (BEFORE(0x00ae1058u)) {
            result = EQUAL(0x00ae1010u);
            return (u8)(8u - (result != 0u));
        }
        result = EQUAL(0x00ae1058u);
        return (u8)(10u - (result != 0u));
    }
    if (EQUAL(0x00ae1030u))
        return 11;
    result = EQUAL(0x00ae1048u);
    return (u8)(12u + (result != 0u));
}
