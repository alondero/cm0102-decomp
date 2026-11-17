#include "cmre.h"
/* Original preferred-image VAs below refer to CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   85ab6a/85ab70/85ab71 and 85ab91/85ab97/85ab98 supply ECX
   and one stack argument to the two thiscall boundaries.
   85aaf7..85ab0a supplies three cdecl arguments; 85ac25..85acd5
   supplies fifteen cdecl arguments. 85adc6 pops twelve entry bytes. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32, u32);
extern u32 CM_CDECL cm_fn_00541f90(u32, u32, u32);
extern u32 CM_CDECL cm_fn_006e2000(int *, char, u8 *, char, u32,
    int *, u32, int, int, u32, u32, u32 *, u8 *, u32, int);

#define B(o) (*(u8 *)(scratch + (o)))
#define D(o) (*(u32 *)(scratch + (o)))
#define W(o) (*(u16 *)(scratch + (o)))
#define MEMB(a) CM_GLOBAL(volatile u8, (a))
#define MEMD(a) CM_GLOBAL(volatile u32, (a))

u32 CM_STDCALL cm_fn_0085aae0(u32 club, u32 mode, u32 flag)
{
    /* Offsets are from L, ESP immediately after sub esp,29c. */
    u32 storage[0x29c / 4];
    u8 *scratch = (u8 *)storage;
    u8 bl, i;
    u32 p, r, n, result;
    s32 bound;
    B(2) = 0;
    B(3) = 0;
    B(4) = 0;
    cm_fn_00541f90(club, (u32)(scratch + 0x1d4), 0);
    bl = (u8)mode;
    B(8) = bl == 1 ? (flag != 0 ? 14 : 9) : 14;
    for (n = 0; n < 50; ++n)
        D(0x44 + n * 4) = 0;
    D(0xc) = 50;
    n = 0;
    do {
        p = D(0x1d4 + n * 4);
        if (p) {
            r = MEMD(p);
            result = MEMD(0xae2364);
        }
        if (p && (s32)r < (s32)result) {
            r = cm_fn_004cf5a0(MEMD(0xae1050), p);
            MEMD(0xae1080) = r;
            if (!r || (MEMB(r + 0x35) & 0x3f) != 5) {
                r = cm_fn_004cf6d0(MEMD(0xae1050), p);
                if (!r && !(MEMB(p + 0x6d) & 0x18)) {
                    i = B(4);
                    D(0x10c + (u32)(s32)(s8)i * 4) = p;
                    B(4) = (u8)(i + 1);
                    i = MEMB(p + 0x6d);
                    if (i && bl != 1) {
                        if (i == 1)
                            MEMB(p + 0x6d) = 0;
                        else if (i == 4)
                            MEMB(p + 0x6d) = 2;
                        if (MEMB(p + 0x6d) == 2)
                            B(2) = (u8)(B(2) + 1);
                    }
                }
            }
        }
        ++n;
        D(0xc) = D(0xc) - 1;
    } while (D(0xc));
    W(0x18) = 0x204;
    W(0x1a) = 0x204;
    W(0x20) = 0x240;
    W(0x22) = 0x240;
    W(0x10) = 1;
    W(0x16) = 0x210;
    W(0x1e) = 0x210;
    W(0x28) = 1;
    W(0x36) = 1;
    W(0x2c) = 0x40;
    W(0x2e) = 0x40;
    W(0x34) = 0x40;
    W(0x3a) = 0x40;
    W(0x3c) = 0x40;
    r = D(8);
    W(0x26) = 0x10;
    W(0x30) = 0x10;
    W(0x3e) = 0x10;
    n = D(4);
    W(0x12) = 0x804;
    W(0x14) = 0x84;
    W(0x1c) = 0x810;
    W(0x24) = 0x90;
    W(0x2a) = 4;
    W(0x32) = 4;
    W(0x38) = 4;
    W(0x40) = 4;
    cm_fn_006e2000((int *)(scratch + 0x10c), (char)n,
        scratch + 0x10, 11, r, (int *)(scratch + 0x44),
        0x5840, (int)club, 0, 0, 0, 0, 0, 0, 0);
    if (bl == 3) {
        bl = B(4);
        i = 0;
        if ((s8)bl > 0) {
            do {
                u8 j;
                if ((s8)B(3) >= 2)
                    break;
                p = D(0x10c + (u32)(s32)(s8)i * 4);
                if (p && MEMB(p + 0x6d) == 2) {
                    for (j = 0; (s8)j < 25; ++j)
                        if (D(0x44 + (u32)(s32)(s8)j * 4) == p)
                            break;
                    if (j == 25) {
                        MEMB(p + 0x6d) = flag ? 4 : 0;
                        B(3) = (u8)(B(3) + 1);
                        B(2) = (u8)(B(2) - 1);
                        if ((s8)B(2) <= 21)
                            B(2) = 21;
                    }
                }
                ++i;
            } while ((s8)i < (s8)bl);
        }
    }
    result = (u32)(s32)(s8)B(8);
    bound = (s32)result + 11;
    bl = 0;
    if (bound > 0) {
        result = 0;
        do {
            p = D(0x44 + result * 4);
            result = p;
            if (p) {
                if ((u8)mode == 1) {
                    MEMB(p + 0x6d) = flag ? 1 : 2;
                    B(2) = (u8)(B(2) + 1);
                } else {
                    i = MEMB(p + 0x6d);
                    if (i != 2 && !(i & 8) && (s32)(s8)B(2) < bound) {
                        MEMB(p + 0x6d) = flag ? 1 : 2;
                        B(2) = (u8)(B(2) + 1);
                    }
                }
            }
            ++bl;
            result = (u32)(s32)(s8)bl;
        } while ((s32)result < bound);
    }
    return (result & 0xffffff00u) | B(2);
}
