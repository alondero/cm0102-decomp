/* fn_00757da0 - preferred-image VA 0x00757da0 (image base 0x00400000).
 *
 * this (ECX): object with { u32 base; u32 pad; s32 count; } at +0/+8.
 * stack args (ret 0x14 pops five dwords, S = entry ESP):
 *   arg0 S+0x04 : u32 *   (key source A, nullable)
 *   arg1 S+0x08 : u32 *   (key source B)
 *   arg2 S+0x0c : u32     (selector, compared with 0x0bdf)
 *   arg3 S+0x10 : word    (key word)
 *   arg4 S+0x14 : u32 *   (bound record, nullable)
 *
 * Local frame: L = S-0x20 (the 32 bytes allocated by `sub esp,0x20`);
 * three pushed registers (ebp/esi/edi) sit below L.
 *   L+0x00 .. L+0x07 : copy of *arg4 and arg4[1] (dead stores, kept)
 *   L+0x08 .. L+0x0f : search context built by cm_fn_0054a110
 *   L+0x10          : context+0x08 = arg0 ? *arg0 : 0xffffffff
 *   L+0x14          : context+0x0c = *arg1
 *   L+0x19          : context+0x11 = (u16)arg3
 *   L+0x1b          : context+0x13 = arg2
 */

#include "cmre.h"

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3,
                          int param_4, void *param_5);

/* Comparator bytes at preferred-image VA 0x00757ee0:
 * 0x00757efd..0x00757f19 compare DWORDs at +8, +0xc, +0x13.
 * Each mismatch branches to 0x00757f27: sub eax,esi (field difference).
 * 0x00757f1b/0x00757f1f sign-extend WORDs at +0x11;
 * 0x00757f23 subtracts the record WORD from the key WORD.
 * 0x00757ee0 loads [entry ESP+4] into EDX (key); after push esi,
 * 0x00757eee loads [ESP+0xc] = [entry ESP+8] into ECX (record).
 * 0x00757ee9 returns -1 for a null first argument; 0x00757ef6
 * returns +1 for a null second argument.
 * The search helper pushes record then key at 0x0094649d/0x0094649e
 * (and 0x009464da/0x009464dd), so this callback is cdecl (key, record).
 * No register-to-stack argument-swapping bridge is involved.
 */
static s32 cm_cmp_00757ee0(volatile u8 *key, volatile u8 *record)
{
    u32 a;
    u32 b;
    s32 key_word;
    s32 record_word;

    if (key == (volatile u8 *)0)
        return -1;
    if (record == (volatile u8 *)0)
        return 1;
    a = *(volatile u32 *)(key + 8);
    b = *(volatile u32 *)(record + 8);
    if (a != b)
        return (s32)(a - b);
    a = *(volatile u32 *)(key + 0x0c);
    b = *(volatile u32 *)(record + 0x0c);
    if (a != b)
        return (s32)(a - b);
    a = *(volatile u32 *)(key + 0x13);
    b = *(volatile u32 *)(record + 0x13);
    if (a != b)
        return (s32)(a - b);
    key_word = *(volatile s16 *)(key + 0x11);
    record_word = *(volatile s16 *)(record + 0x11);
    return key_word - record_word;
}

u32 CM_THISCALL cm_fn_00757da0(volatile u8 *self, volatile u32 *param_2,
                                volatile u32 *param_3, s32 param_4,
                                s16 param_5, volatile u32 *param_6)
{
    u8 local[0x20];
    s32 count;
    s32 index;
    u32 base;
    u32 entry;
    u32 found;
    u32 first;
    u32 second;
    u32 keyb;
    u16 keyw;
    u16 bound_high;
    u16 found_high;
    s16 bound_low;
    s16 found_low;

    /* 0x00757da8: lea ecx,[esp+0x14] -> L+8 */
    cm_fn_0054a110((u16 *)(local + 8));

    /* 0x00757db1: cmp eax,0xbdf (arg2) */
    if ((u32)param_4 == 0x0bdfu) {
        /* 0x00757dbc: count = self->count */
        count = *(volatile s32 *)(self + 8);
        if (count > 0) {
            base = *(volatile u32 *)self;      /* 0x00757dc5 */
            keyw = (u16)param_5;               /* 0x00757dc7: mov si,word */
            entry = base + 0x0cu;              /* 0x00757dd0 */
            for (index = 0; index < count; ++index) {
                if (*(volatile u32 *)(entry + 7) == 0x0bdfu &&
                    *(volatile u32 *)entry == *(volatile u32 *)param_3 &&
                    *(volatile u16 *)(entry + 5) == keyw)
                    return 1;                   /* 0x00757e85 */
                entry += 0x17u;                /* 0x00757ded */
            }
        }
        return 0;
    }

    /* 0x00757dff */
    keyb = (param_2 != (volatile u32 *)0) ? *(volatile u32 *)param_2
                                          : 0xffffffffu;
    *(volatile u32 *)(local + 0x10) = keyb;   /* 0x00757e09 / 0x00757e0f */
    *(volatile u32 *)(local + 0x1b) = (u32)param_4; /* 0x00757e1b */
    count = *(volatile s32 *)(self + 8);      /* 0x00757e1f */
    keyb = *(volatile u32 *)param_3;          /* 0x00757e27 */
    keyw = (u16)param_5;                      /* 0x00757e29 */
    *(volatile u32 *)(local + 0x14) = keyb;   /* 0x00757e2e */
    base = *(volatile u32 *)self;             /* 0x00757e32 */
    *(volatile u16 *)(local + 0x19) = keyw;   /* 0x00757e34 */

    /* 0x00757e42: context, base, count, stride 0x17, comparator */
    found = cm_fn_00946464((u32)(local + 8), base, (u32)count, 0x17,
                            (void *)cm_cmp_00757ee0);
    if (found == 0)
        return 0;                             /* 0x00757e4c */

    if (param_6 != (volatile u32 *)0) {       /* 0x00757e52 */
        first = *(volatile u32 *)param_6;     /* 0x00757e56 */
        second = *(volatile u32 *)(param_6 + 1); /* 0x00757e58 */
        *(volatile u32 *)(local + 0x00) = first;  /* 0x00757e5b */
        bound_high = *(volatile u16 *)(local + 0x02); /* 0x00757e5f */
        *(volatile u32 *)(local + 0x04) = second; /* 0x00757e64 */
        found_high = *(volatile u16 *)(found + 2); /* 0x00757e68 */
        if (bound_high > found_high)
            return 0;                         /* 0x00757e6f */
        if (bound_high < found_high)
            return 1;                         /* 0x00757e71 */
        bound_low = (s16)(u16)first;          /* 0x00757e75 (dx) */
        found_low = *(volatile s16 *)found;   /* 0x00757e75 */
        if (bound_low > found_low)
            return 0;                         /* 0x00757e7f */
    }
    return 1;                                 /* 0x00757e85 */
}
