#include "cmre.h"
/* Preferred-image VA 0x007c1e30, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern long double CM_STDCALL cm_fn_007c2150(u32, s8, float);
extern u32 CM_THISCALL cm_fn_007c3920(u32, u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))
#define F(a) (*(volatile float *)(u32)(a))
#define Q(a) (*(volatile double *)(u32)(a))

long double CM_THISCALL cm_fn_007c1e30(u32 self, u32 player, u32 category,
                                      u32 field, float incoming)
{
    u8 scratch[0x20c];
    u32 local = (u32)scratch;
    u32 id, total, pointer, count, integer;
    u8 combined_count, byte;
    u16 combined_sum, word;
    long double value, other, floating;
    char *version;
    combined_count = 0;
    combined_sum = 0;
    B(local) = 0;
    if (player == 0) {
        cm_fn_00944cff(0x00a78310, 0, 0, scratch + 0x10c, scratch + 0xc);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x10c, scratch + 0xc, 0x659);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        value = (long double)F(0x00968970);
        D(0x00b67a34) = 0;
        return value;
    }
    id = D(player);
    total = D(0x00ae236c);
    if ((s32)id >= (s32)total) return (long double)F(0x00968970);
    if ((u8)field != 19 && (s8)category <= 6) {
        pointer = cm_fn_007c3920(self, player, category);
        if (pointer != 0) return cm_fn_007c2150(pointer, (s8)field, incoming);
        if ((u8)field == 18 || (u8)field == 20) return (long double)incoming;
        return (long double)F(0x00968970);
    }
    pointer = cm_fn_007c3920(self, player, 2);
    if (pointer != 0) {
        combined_count = B(pointer);
        combined_sum = W(pointer + 0xeu);
        B(local) = combined_count;
    }
    pointer = cm_fn_007c3920(self, player, 3);
    if (pointer != 0) {
        byte = B(pointer);
        word = W(pointer + 0xeu);
        combined_count = (u8)(combined_count + byte);
        combined_sum = (u16)(combined_sum + word);
        B(local) = combined_count;
    }
    count = D(local) & 0xffu;
    if (combined_count < 20) {
        value = (long double)(s32)(20u - count);
        value *= (long double)Q(0x009688a0);
        value += (long double)(s16)combined_sum;
        value *= (long double)F(0x0096d61c);
    } else {
        value = (long double)(s16)combined_sum / (long double)(s32)count;
    }
    F(local + 4u) = (float)value;
    pointer = cm_fn_007c3920(self, player, 4);
    if (pointer != 0) {
        count = B(pointer);
        integer = (u32)(s32)(s16)W(pointer + 0xeu);
        if (count < 8) {
            value = (long double)(s32)(8u - count);
            value *= (long double)Q(0x00968930);
            value += (long double)(s32)integer;
            value *= (long double)F(0x0096e2bc);
        } else {
            value = (long double)(s32)integer / (long double)(s32)count;
        }
        F(local) = (float)value;
    }
    /* Missing category 4 leaves the low count byte and uninitialized upper
       bytes at L+0. Missing category 5 reads the uninitialized float at L+8. */
    pointer = cm_fn_007c3920(self, player, 5);
    if (pointer != 0) {
        count = B(pointer);
        integer = (u32)(s32)(s16)W(pointer + 0xeu);
        if (count < 5) {
            value = (long double)(s32)(5u - count);
            value += (long double)(s32)integer;
            value *= (long double)F(0x0096d668);
        } else {
            value = (long double)(s32)integer / (long double)(s32)count;
        }
    } else {
        value = (long double)F(local + 8u);
    }
    switch ((s8)category) {
    case 102:
        value *= (long double)Q(0x009688a0);
        other = (long double)F(local);
        other *= (long double)Q(0x00968938);
        value += other;
        value += (long double)F(local + 4u);
        return value * (long double)Q(0x00967880);
    case 101:
        value *= (long double)Q(0x00968930);
        other = (long double)F(local);
        other *= (long double)Q(0x009688a0);
        value += other;
        floating = (long double)F(local + 4u);
        floating += floating;
        value += floating;
        return value * (long double)Q(0x00967880);
    case 100:
        value *= (long double)Q(0x00968940);
        other = (long double)F(local);
        other *= (long double)Q(0x00968e18);
        value += other;
        floating = (long double)F(local + 4u);
        floating *= (long double)Q(0x00969000);
        value += floating;
        return value * (long double)Q(0x00967880);
    default:
        return (long double)F(0x00968970);
    }
}
