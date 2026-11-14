/* CM0102 3.9.68, preferred-image VA 0084af00, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The seven reviewed indexed callers consume AL. Other callers remain unknown.
   The signed64 scalar occupies homes3/4; all eight DWORDs are callee
   cleaned. The original argument8 home is a BYTE mode and a DWORD FP scratch.
   Original engine helpers remain numeric boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define SW(a) CM_GLOBAL(volatile s16, (u32)(a))
#define FP(a) CM_GLOBAL(volatile double, (u32)(a))
/* GCC's alias type retains the original DWORD views of the signed64 home. */
typedef u32 cm_offer_word_0084af00 __attribute__((may_alias));
typedef long long cm_offer_scalar_0084af00 __attribute__((aligned(4)));

extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004d42d0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004d1600(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053ca90(u32);
extern u32 CM_CDECL cm_fn_005444a0(u32, u32);
extern u32 CM_CDECL cm_fn_00544590(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_005fd210(u32, u32);
extern u32 CM_THISCALL cm_fn_005fea50(u32, s8, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern u32 CM_THISCALL cm_fn_00867b50(u32);

u8 CM_STDCALL __attribute__((target("no-sse3")))
cm_fn_0084af00(volatile u32 club_home, volatile u32 person_home,
              volatile cm_offer_scalar_0084af00 offered, volatile u32 context_home,
              volatile u32 target_low, volatile u32 target_high,
              volatile u32 mode_home)
{
    volatile cm_offer_word_0084af00 *offered_words =
        (volatile cm_offer_word_0084af00 *)&offered;
    u32 manager = D(0x00ae1050);
    u32 club = club_home;
    u32 person = person_home;
    u32 record = cm_fn_004cf5f0(manager, person, club);
    u32 target;
    u32 offered_high;

    if (club == 0) return 0;
    if (record != 0 && B(record + 0x49u) != 0) return 0;
    if (D(club + 0xcfu) == person) return 2;
    if (record == 0 || (B(record + 0x35u) & 0x3fu) != 2)
        return B(&mode_home) != 0 ? 2 : 0;

    if (B(&mode_home) != 0) {
        u32 status = cm_fn_00600a50(club, 0, 1, 0, 0);
        u32 role;
        u32 context;
        if (status != 0) return 0;
        status = cm_fn_00544590(person, status);
        context = context_home; /* 0084af84: cdecl arguments still on stack. */
        if (status != 0) {
            u32 contract = D(person + 0x61u);
            s32 first = SW(contract + 0xbu);
            s32 second = SW(context + 0x80u);
            long double a, b;
            mode_home = (u32)first;
            a = (s32)mode_home;
            mode_home = (u32)second;
            b = (s32)mode_home;
            b *= FP(0x00968928);
            if (b > a) return 0;
        }
        role = B(record + 0x4fu) & 0xf0u;
        if (role != 0x60u && role != 0x70u) {
            u32 contract = D(person + 0x61u);
            if (B(contract + 0xfu) != 0x14u) return 2;
        }
        {
            u32 contract = D(person + 0x61u);
            s32 second = SW(context + 0x80u);
            s32 first = SW(contract + 0xbu);
            long double a, b;
            mode_home = (u32)first;
            a = (s32)mode_home;
            mode_home = (u32)second;
            a *= FP(0x00969040);
            b = (s32)mode_home;
            if (!(b > a)) return 2;
        }
        if ((s8)B(record + 0x3du) >= (s8)0xceu) return 2;
        {
            u32 nation = D(club + 0x1dbu);
            u32 compared = D(0x009cf6e8);
            if (nation != compared) return 0;
        }
        return 2;
    }

    if (cm_fn_00600a50(club, 0, 1, 0, 0) != 0) {
        u32 context = context_home;
        u32 receiver = D(0x00ae1050);
        long double converted;
        mode_home = cm_fn_004d42d0(receiver, club, context, person);
        converted = (s32)mode_home;
        converted *= FP(0x009689b0);
        /* Original compiler-inserted __ftol CALL at0084b051, ADR0006. */
        target = (u32)(long long)converted;
        mode_home = target;
        if (cm_fn_00867b50(record) == 0) {
            u32 owner = cm_fn_0053ca90(club);
            u32 service = D(0x00b63c98);
            if ((u32)cm_fn_005fd210(service, owner) != 0) {
                s32 category;
                owner = cm_fn_0053ca90(club);
                service = D(0x00b63c98);
                category = (s8)cm_fn_005fea50(service, 0, owner);
                if (category == 1) return 2;
                if (category == 2) {
                    target = D(person + 0x52u);
                    mode_home = target;
                }
            }
        }
    } else {
        u32 high = target_high;
        target = target_low;
        /* High==-1 always falls through original CMP low,-1 / JA. */
        if ((s32)high < 0) {
            u32 contract = D(person + 0x61u);
            if (contract == 0) {
                target = D(person + 0x52u);
            } else {
                u32 context = context_home;
                u32 receiver = D(0x00ae1050);
                target = cm_fn_004d1600(receiver, club, context, person, 0, 1, 1);
            }
        }
        mode_home = target;
    }

    offered_high = offered_words[1];
    {
        s32 target_high_word = (s32)target < 0 ? -1 : 0;
        if ((s32)offered_high > target_high_word) return 0;
        if ((s32)offered_high == target_high_word && offered_words[0] >= target)
            return 0;
    }
    if (cm_fn_00600a50(club, 0, 1, 0, 0) != 0) {
        u32 threshold = target - 0x16e360u;
        s32 high = (s32)threshold < 0 ? -1 : 0;
        if (high < (s32)offered_high ||
            (high == (s32)offered_high && threshold < offered_words[0])) {
            if (cm_fn_005444a0(person, 0) != 0) return 1;
        }
    }
    {
        long double amount = offered; /* original FILD QWORD */
        long double scaled = (s32)mode_home;
        scaled *= FP(0x0096f9c0);
        return scaled > amount ? 2 : 1;
    }
}
