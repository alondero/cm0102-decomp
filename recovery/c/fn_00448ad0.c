#include "cmre.h"
/* Original preferred-image VAs below refer to CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   448af2/448afe and 448bca/448bd6 push create,index with ECX=self.
   448b9e/448c6f push type,id with ECX=self.
   448b30/448c0e push club, caller removes four bytes.
   Entry reads ECX; 448c9a/448ca5 return with twelve stack bytes popped. */
extern u32 CM_THISCALL cm_fn_004528d0(u32 self, u32 *index, u32 create);
extern u32 CM_THISCALL cm_fn_00452710(u32 self, u32 *index, u32 create);
extern u32 CM_THISCALL cm_fn_00452cb0(u32 self, u32 id, u8 type);
extern u32 CM_CDECL cm_fn_00669950(u32 club);
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

u32 CM_THISCALL cm_fn_00448ad0(u32 self, u32 amount, u32 club1, u32 club2)
{
    u32 cur, best, value, old, stamp, count, id, p, i;
    s32 threshold;
    if (!club1 || !club2)
        return 1;
    cur = cm_fn_004528d0(self, (u32 *)club2, 1);
    best = cm_fn_00452710(self, (u32 *)club2, 1);
    value = D(cur + 0x36b);
    value = value == 0xffffffffu ? amount : value + amount;
    D(cur + 0x36b) = value;
    W(cur + 0x36f) = 0;
    stamp = cm_fn_00669950(club2);
    value = D(cur + 0x36b);
    old = D(best + 0x36b);
    if ((s32)value > (s32)old) {
        threshold = ((s32)(s16)W(club2 + 0x80) + 500) * 1000;
        if ((s32)value >= threshold && (u16)stamp != W(best + 0x36f)) {
            count = D(self + 0x2c);
            i = 0;
            if ((s32)count > 0) {
                id = D(club2);
                p = D(self + 0x24);
                do {
                    if (D(p) == id && B(p + 4) == 0x1f)
                        break;
                    ++i;
                    p += 6;
                } while ((s32)i < (s32)count);
            }
            if ((s32)i >= (s32)count)
                cm_fn_00452cb0(self, D(club2), 0x1f);
        }
        value = D(cur + 0x36b);
        D(best + 0x36b) = value;
        W(best + 0x36f) = (u16)stamp;
    }
    cur = cm_fn_004528d0(self, (u32 *)club1, 1);
    best = cm_fn_00452710(self, (u32 *)club1, 1);
    value = D(cur + 0x365);
    if (value == 0xffffffffu)
        D(cur + 0x365) = amount;
    else
        D(cur + 0x365) = value + amount;
    W(cur + 0x369) = 0;
    stamp = cm_fn_00669950(club1);
    old = D(best + 0x365);
    value = D(cur + 0x365);
    if ((s32)value > (s32)old) {
        threshold = ((s32)(s16)W(club1 + 0x80) + 500) * 1000;
        if ((s32)old >= threshold) {
            count = D(self + 0x2c);
            i = 0;
            if ((s32)count > 0) {
                id = D(club1);
                p = D(self + 0x24);
                do {
                    if (D(p) == id && B(p + 4) == 0x1e)
                        break;
                    ++i;
                    p += 6;
                } while ((s32)i < (s32)count);
            }
            if ((s32)i >= (s32)count)
                cm_fn_00452cb0(self, D(club1), 0x1e);
        }
        value = D(cur + 0x365);
        D(best + 0x365) = value;
        W(best + 0x369) = (u16)stamp;
    }
    return 1;
}
