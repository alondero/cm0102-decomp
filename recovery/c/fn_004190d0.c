#include "cmre.h"

/* Preferred-image VA004190D0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0/void: sole caller next enters569ED0, which overwrites EAX.
   Original RNG order, opposite zero tests and XOR flag bug are retained. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern void CM_THISCALL cm_fn_00419710(u32, u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32, u32, u32);
extern s32 CM_CDECL cm_fn_0090cfc0(s32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32, u32);
extern void CM_THISCALL cm_fn_0041a360(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008676f0(u32);
extern u32 CM_THISCALL cm_fn_00867560(u32, u32);
extern void CM_THISCALL cm_fn_0072c1f0(u32, u32, u32, u32, u32, u32);

void CM_THISCALL cm_fn_004190d0(u32 self)
{
    u8 scratch[0x1c];
    u32 L = (u32)scratch, list, entry, i, now, extra, p, receiver, id, base;
    u32 person, other, club, relation, flag_address, count, value;
    s32 a, b;
    u16 current_year, record_year;
    u8 flag;
    D(L) = self;
    list = D(self);
    if (list != 0) {
        value = D(list + 2);
        i = 0;
        D(list + 0xa) = value;
        list = D(self);
        if ((s16)W(list) > 0) {
            for (;;) {
                entry = cm_fn_0068d1b0(list);
                if (entry == 0) return;
                now = D(0xae2c90);
                extra = D(0xae2c94);
                D(L + 0x14) = now;
                current_year = W(L + 0x16);
                D(L + 0x18) = extra;
                record_year = W(entry + 6);
                if (current_year > record_year ||
                    (current_year == record_year && (s16)(u16)now > (s16)W(entry + 4))) {
                    cm_fn_00419710(self, entry);
                    break;
                }
                list = D(self);
                ++i;
                count = (u32)(s32)(s16)W(list);
                if ((s32)i >= (s32)count) break;
            }
        }
    }
    list = D(self + 4);
    if (list != 0) {
        value = D(list + 2);
        receiver = D(L);
        D(list + 0xa) = value;
        i = 0;
        list = D(receiver + 4);
        if ((s16)W(list) > 0) {
            for (;;) {
                entry = cm_fn_0068d1b0(list);
                D(L + 4) = entry;
                if (entry == 0) return;
                now = D(0xae2c90);
                extra = D(0xae2c94);
                D(L + 0x14) = now;
                D(L + 0x18) = extra;
                p = cm_fn_0054c770(entry + 4, L + 0xc, 1);
                record_year = W(p + 2);
                current_year = W(L + 0x16);
                if (current_year > record_year ||
                    (current_year == record_year && (s16)(u16)now > (s16)W(p))) break;
                receiver = D(L);
                ++i;
                list = D(receiver + 4);
                count = (u32)(s32)(s16)W(list);
                if ((s32)i >= (s32)count) goto third_list;
            }
            flag = B(entry + 0xc);
            id = D(entry);
            base = D(0xae23bc);
            person = base + id * 110u;
            if (flag != 0) {
                p = D(person + 0x61);
                if (p == 0) goto remove_second;
                a = cm_fn_0090cfc0((s32)(s8)B(person + 0x5b));
                if (a != 0) goto remove_second;
                a = cm_fn_0090cfc0((s32)(s8)B(person + 0x5d));
                if (a != 0) goto remove_second;
                flag = B(person + 0x5b);
                if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0)
                    B(person + 0x5b) = (u8)(B(person + 0x5b) - 1u);
                flag = B(person + 0x5d);
                if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0)
                    B(person + 0x5d) = (u8)(B(person + 0x5d) - 1u);
                if (entry != 0) {
                    receiver = D(L);
                    id = D(entry);
                    p = D(receiver + 0xc);
                    flag_address = id + p;
                    flag = B(flag_address);
                    if (flag != 0) {
                        /* Original OR DL,2 can never yield zero. */
                        B(flag_address) = flag ^ 2;
                        receiver = D(L);
                        list = D(receiver + 4);
                        cm_fn_0068d1f0(list, entry);
                    }
                }
                receiver = D(L);
                cm_fn_0041a360(receiver, person);
                goto third_list;
            }
            receiver = D(0xae1050);
            relation = cm_fn_004cf5a0(receiver, person);
            D(0xae1080) = relation;
            if (relation != 0) {
                id = D(relation + 4);
                base = D(0xae23b4);
                club = base + id * 581u;
                D(L + 8) = club;
            } else { D(L + 8) = 0; club = 0; }
            D(L + 0x14) = 0;
            D(L + 0xc) = club + 0xd7;
            for (;;) {
                if (club == 0) break;
                p = D(L + 0xc);
                other = D(p);
                if (other != 0 && other != person && D(other + 0x61) != 0) {
                    receiver = D(0xae1050);
                    relation = cm_fn_004cf5f0(receiver, person, club);
                    if (relation != 0) {
                        value = cm_fn_008676f0(relation);
                        if ((value & 2) == 0) {
                            flag = B(other + 0x5b);
                            if ((s8)flag > 15 && (s8)B(other + 0x57) > 10) {
                                a = cm_fn_0090cfc0((s32)(s8)flag);
                                b = cm_fn_0090cfc0(30);
                                if (a > b) {
                                    flag = B(relation + 0x4f) & 0xf0;
                                    if ((flag == 0x10 || flag == 0x20) && (s8)B(other + 0x18) > 23) {
                                        p = D(other + 0x61);
                                        if ((s16)W(p + 0xb) > 0x1482)
                                            cm_fn_00867560(relation, 2);
                                    }
                                }
                                entry = D(L + 4);
                            }
                        }
                        if ((s8)B(other + 0x18) < 21) {
                            p = D(other + 0x61);
                            if ((s16)W(p + 0xb) > 0x1482) {
                                flag = B(other + 0x5b);
                                if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0)
                                    B(other + 0x5b) = (u8)(B(other + 0x5b) - 1u);
                                p = D(other + 0x61);
                                if ((s8)B(p + 0x44) > 1) {
                                    a = cm_fn_0090cfc0((s32)(s8)B(other + 0x5b));
                                    if (a == 0) {
                                        p = D(other + 0x61);
                                        B(p + 0x44) = (u8)(B(p + 0x44) - 1u);
                                    }
                                }
                                flag = B(other + 0x5d);
                                if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0)
                                    B(other + 0x5d) = (u8)(B(other + 0x5d) - 1u);
                            }
                        }
                    }
                    club = D(L + 8);
                }
                count = D(L + 0x14);
                p = D(L + 0xc);
                ++count;
                p += 4;
                D(L + 0x14) = count;
                D(L + 0xc) = p;
                if ((s32)count >= 50) break;
            }
            if (D(person + 0x61) != 0) {
                a = cm_fn_0090cfc0((s32)(s8)B(person + 0x5b));
                b = cm_fn_0090cfc0(15);
                if ((s8)(u8)a < (s8)(u8)((u8)b + 1u)) {
                    flag = B(person + 0x5b);
                    if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) != 0)
                        B(person + 0x5b) = (u8)(B(person + 0x5b) - 1u);
                    p = D(person + 0x61);
                    flag = B(p + 0x44);
                    if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) != 0) {
                        p = D(person + 0x61);
                        B(p + 0x44) = (u8)(B(p + 0x44) - 1u);
                    }
                    entry = D(L + 4);
                    receiver = D(L);
                    if (entry != 0) {
                        p = D(receiver + 0xc);
                        id = D(entry);
                        flag_address = p + id;
                        flag = B(flag_address);
                        if (flag != 0) {
                            B(flag_address) = flag ^ 2;
                            list = D(receiver + 4);
                            cm_fn_0068d1f0(list, entry);
                        }
                    }
                    cm_fn_0041a360(receiver, person);
                    if (club != 0 && D(person + 0x61) != 0) {
                        receiver = D(0xb6b7c8);
                        cm_fn_0072c1f0(receiver, person, club, 8, 0, 0);
                    }
                    goto third_list;
                }
                entry = D(L + 4);
            }
remove_second:
            if (entry != 0) {
                receiver = D(L);
                id = D(entry);
                p = D(receiver + 0xc);
                flag_address = p + id;
                flag = B(flag_address);
                if (flag != 0) {
                    B(flag_address) = flag ^ 2;
                    receiver = D(L);
                    list = D(receiver + 4);
                    cm_fn_0068d1f0(list, entry);
                }
            }
        }
    }
third_list:
    receiver = D(L);
    list = D(receiver + 8);
    if (list == 0) return;
    value = D(list + 2);
    i = 0;
    D(list + 0xa) = value;
    list = D(receiver + 8);
    if ((s16)W(list) <= 0) return;
    for (;;) {
        entry = cm_fn_0068d1b0(list);
        if (entry == 0) return;
        now = D(0xae2c90);
        extra = D(0xae2c94);
        D(L + 0x14) = now;
        D(L + 0x18) = extra;
        p = cm_fn_0054c770(entry + 8, L + 0xc, 1);
        record_year = W(p + 2);
        current_year = W(L + 0x16);
        if (current_year > record_year ||
            (current_year == record_year && (s16)(u16)now > (s16)W(p))) {
            flag = B(entry + 0x10);
            id = D(entry);
            base = D(0xae23bc);
            person = base + id * 110u;
            if (flag != 0) {
                if (D(person + 0x61) == 0) goto remove_third;
                a = cm_fn_0090cfc0((s32)(s8)B(person + 0x5b));
                if (a != 0) goto next_third;
                a = cm_fn_0090cfc0((s32)(s8)B(person + 0x5d));
                if (a != 0) goto next_third;
                flag = B(person + 0x5b);
                if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0)
                    B(person + 0x5b) = (u8)(B(person + 0x5b) - 1u);
                flag = B(person + 0x5d);
                if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0)
                    B(person + 0x5d) = (u8)(B(person + 0x5d) - 1u);
                goto remove_third;
            }
            a = cm_fn_0090cfc0((s32)(s8)B(person + 0x5b));
            b = cm_fn_0090cfc0(5);
            if ((s8)(u8)a < (s8)(u8)((u8)b + 1u)) {
                flag = B(person + 0x5b);
                if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0)
                    B(person + 0x5b) = (u8)(B(person + 0x5b) - 1u);
                p = D(person + 0x61);
                if (p != 0) {
                    flag = B(p + 0x44);
                    if ((s8)flag > 1 && cm_fn_0090cfc0((s32)(s8)flag) == 0) {
                        p = D(person + 0x61);
                        B(p + 0x44) = (u8)(B(p + 0x44) - 1u);
                    }
                }
                goto remove_third;
            }
        }
next_third:
        receiver = D(L);
        ++i;
        list = D(receiver + 8);
        count = (u32)(s32)(s16)W(list);
        if ((s32)i >= (s32)count) return;
    }
remove_third:
    receiver = D(L);
    list = D(receiver + 8);
    cm_fn_0068d1f0(list, entry);
}
