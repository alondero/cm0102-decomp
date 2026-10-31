/* Preferred-image VA 00522240, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All three direct callers overwrite EAX immediately after the call. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define LD(o) (*(volatile u32 *)(work + (o)))
#define LB(o) (*(volatile u8 *)(work + (o)))
extern void CM_THISCALL cm_fn_0048e3e0(u32 self, u32 club, u32 low, u32 high,
                                      u32 sentinel, u32 next, u32 object, u32 flag);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 club);
extern void CM_THISCALL cm_fn_005999a0(u32 self, u32 amount);

void CM_THISCALL cm_fn_00522240(u32 self, u32 first, u32 second, s16 stage, u32 object)
{
    u8 work[8];
    u8 flag;
    u16 next_word;
    u32 table, row, winner, loser, raw_flag, next, club, receiver, account, balance;
    s32 maximum, divisor, quotient;
    flag = B(first + 5u);
    LD(4) = 0;
    LB(0) = flag;
    if (W(object + 0x30u) == 0x136u && flag == 1)
        LB(0) = 3;
    table = D(self + 0xa3u);
    row = table + (u32)(s32)stage * 104u;
    winner = D(row + 0x60u);
    loser = D(row + 0x64u);
    if (winner == 0xffffffffu) {
        maximum = (s16)W(self + 0x3cu);
        balance = D(self + 0x28u);
        divisor = maximum - (s32)stage;
        quotient = (s32)balance / divisor;
        divisor = (s16)W(row + 0x1au);
        quotient /= divisor;
        if (loser == 0xffffffffu) {
            /* Native LEA wraps the product before truncating signed division. */
            winner = (u32)((s32)((u32)quotient * 3u) / 4);
            loser = (u32)(quotient / 4);
        } else {
            winner = (u32)quotient;
        }
    }
    maximum = (s16)W(self + 0x3cu);
    if ((s32)stage + 1 < maximum) {
        next_word = W(row + 0x6fu);
        if (next_word != 0x8cu)
            LD(4) = (row & 0xffff0000u) | next_word;
    }
    if (winner != 0) {
        /* Only the flag byte is initialized, but the native push reads a DWORD. */
        raw_flag = LD(0);
        next = LD(4);
        club = D(first);
        cm_fn_0048e3e0(self, club, winner, (s32)winner < 0 ? 0xffffffffu : 0,
                       0xffffffffu, next, object, raw_flag);
        club = D(first);
        receiver = D(0x00ae2a38u);
        account = cm_fn_005a0590(receiver, club);
        cm_fn_005999a0(account, winner);
        balance = D(self + 0x28u);
        D(self + 0x28u) = balance - winner;
    }
    if (loser != 0) {
        club = D(second);
        receiver = D(0x00ae2a38u);
        account = cm_fn_005a0590(receiver, club);
        cm_fn_005999a0(account, loser);
        balance = D(self + 0x28u);
        D(self + 0x28u) = balance - loser;
    }
}
