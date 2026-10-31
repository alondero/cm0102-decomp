#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
typedef unsigned long long wide_539c60;
extern u32 CM_CDECL cm_fn_00946a04(void);
extern wide_539c60 CM_STDCALL cm_fn_00945e40(u32, u32, u32, u32);
extern wide_539c60 CM_STDCALL cm_fn_00945e80(u32, u32, u32, u32);

static u32 divide_32768(wide_539c60 value)
{
    return (u32)cm_fn_00945e80((u32)value, (u32)(value >> 32), 0x8000, 0);
}

static wide_539c60 imul_539c60(u32 first, u32 second)
{
    return (wide_539c60)((long long)(s32)first * (long long)(s32)second);
}

static u32 scan_539c60(u32 text)
{
    u32 count = 0;
    while (count != 0xffffffffu) {
        u8 value = B(text + count);
        ++count;
        if (value == 0) break;
    }
    return count - 1u;
}

/* Preferred-image VA00539c60/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL1/full pointer EAX. Scratch14: attemptsL0/bestL4/startL8/
   countLc/signL10. The high-count path reuses the argument's low BYTE
   as the signed best score and rescans strings at every original site. */
u32 CM_CDECL cm_fn_00539c60(volatile u32 argument)
{
    u32 scratch[5], l = (u32)scratch, record, region, base, count, start;
    u32 random, index, candidate, value, length, cursor, target, global;
    u32 selected, region_base, attempts;
    u8 flag, other, score;
    wide_539c60 product;

    record = argument;
    D(l + 4) = 0;
    if (record == 0) goto fallback;
    index = D(record);
    base = D(0x00ae2338);
    region = base + index * 36u;
    count = D(region + 0x18);
    if ((s32)count > 100) {
        start = D(region);
        B((u32)&argument) = 0xff;
        D(l + 8) = start;
        D(l) = 20;
        D(l + 0xc) = count;
        D(l + 0x10) = (u32)((s32)count >> 31);
        do {
            random = cm_fn_00946a04();
            value = D(l + 0x10);
            count = D(l + 0xc);
            product = cm_fn_00945e40(random, (u32)((s32)random >> 31), count, value);
            index = divide_32768(product);
            start = D(l + 8);
            index += start;
            base = D(0x00ae23f0);
            candidate = base + index * 60u;
            score = B(candidate + 0x3b);
            flag = B((u32)&argument);
            if ((s8)score > (s8)flag) {
                if (candidate != 0) {
                    value = B(candidate);
                    if (value != 0) {
                        length = scan_539c60(candidate);
                        if (length == 1) goto next_sample;
                        cursor = 0;
                        length = scan_539c60(candidate);
                        if ((s32)length > 0) {
                            do {
                                value = B(candidate + cursor);
                                if (value == 0x2e) goto next_sample;
                                ++cursor;
                                length = scan_539c60(candidate);
                            } while ((s32)cursor < (s32)length);
                        }
                    }
                }
                B((u32)&argument) = score;
                D(l + 4) = index;
            }
next_sample:
            value = D(l);
            --value;
            D(l) = value;
        } while (value != 0);
        index = D(l + 4);
        base = D(0x00ae23f0);
        return base + index * 60u;
    }
    flag = B(record + 0x77);
    if (flag == 0) {
        count = D(0x00ae2398);
        if ((s32)count > 0x7fff) count = 0x7fff;
        random = cm_fn_00946a04();
        index = divide_32768(imul_539c60(random, count));
        base = D(0x00ae23f4);
        return base + index * 60u;
    }
    index = (u32)(s32)(s8)flag - 1u;
    if (index > 31) goto random_record;
    target = D(0x0053a060 + index * 4u);
    switch (target) {
    case 0x00539e09: global = 0x009cf204; break;
    case 0x00539e23: global = 0x009cf458; break;
    case 0x00539e2d: global = 0x009cf298; break;
    case 0x00539e37: global = 0x009cf2b8; break;
    case 0x00539e41: global = 0x009cf338; break;
    case 0x00539e4b: global = 0x009cf2e4; break;
    case 0x00539e55: global = 0x009cf2fc; break;
    case 0x00539e5f: global = 0x009cf300; break;
    case 0x00539e69: global = 0x009cf314; break;
    case 0x00539e73: global = 0x009cf31c; break;
    case 0x00539e7d: global = 0x009cf35c; break;
    case 0x00539e87: global = 0x009cf344; break;
    case 0x00539e91: global = 0x009cf360; break;
    case 0x00539e9b: global = 0x009cf36c; break;
    case 0x00539ea5:
        random = cm_fn_00946a04();
        product = cm_fn_00945e40(random, (u32)((s32)random >> 31), 2, 0);
        index = divide_32768(product);
        global = index != 0 ? 0x009cf484 : 0x009cf404;
        break;
    case 0x00539ed6: global = 0x009cf430; break;
    case 0x00539edd: global = 0x009cf434; break;
    case 0x00539ee4: global = 0x009cf444; break;
    case 0x00539eeb: global = 0x009cf448; break;
    case 0x00539ef2: global = 0x009cf408; break;
    case 0x00539ef9: global = 0x009cf2ac; break;
    case 0x00539f00: global = 0x009cf488; break;
    case 0x00539f07: global = 0x009cf4dc; break;
    case 0x00539f0e: global = 0x009cf270; break;
    case 0x00539f15: global = 0x009cf3b8; break;
    case 0x00539f1c: global = 0x009cf428; break;
    case 0x00539f23: global = 0x009cf470; break;
    case 0x00539f2a: global = 0x009cf4a8; break;
    case 0x00539f31: global = 0x009cf348; break;
    case 0x00539f38: global = 0x009cf2c0; break;
    default: __builtin_unreachable();
    }
    index = D(global);
    base = D(0x00ae23a8);
    selected = base + index * 290u;
    if (selected == 0) goto random_record;
    random = cm_fn_00946a04();
    product = cm_fn_00945e40(random, (u32)((s32)random >> 31), 20, 0);
    value = divide_32768(product);
    region_base = D(0x00ae2338);
    index = D(selected);
    value += 15;
    count = D(region_base + index * 36u + 0x18);
    if ((s32)count >= (s32)value) goto selected_record;
random_record:
    attempts = 0;
    do {
        random = cm_fn_00946a04();
        count = D(0x00ae2350);
        index = divide_32768(imul_539c60(random, count));
        base = D(0x00ae23a8);
        selected = base + index * 290u;
        flag = B(selected + 0x77);
        other = B(record + 0x77);
        region_base = D(0x00ae2338);
        if (flag == other) {
            index = D(selected);
            count = D(region_base + index * 36u + 0x18);
            if ((s32)count > 5) break;
        }
        ++attempts;
    } while ((s8)attempts < 40);
    if (selected == 0) goto fallback;
selected_record:
    index = D(selected);
    region = region_base + index * 36u;
    count = D(region + 0x18);
    if ((s32)count < 5 || selected == 0) goto fallback;
    start = D(region);
    random = cm_fn_00946a04();
    value = divide_32768(imul_539c60(random, count));
    base = D(0x00ae23f0);
    value += start;
    return base + value * 60u;
fallback:
    count = D(0x00ae2398);
    if ((s32)count > 0x7fff) count = 0x7fff;
    random = cm_fn_00946a04();
    index = divide_32768(imul_539c60(random, count));
    base = D(0x00ae23f0);
    return base + index * 60u;
}
