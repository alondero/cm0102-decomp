/* fn_004ca6d0 -- plain C reconstruction.
 *
 * Preferred-image VA 0x004ca6d0 (image base 0x00400000), original body 2502 B.
 * Conventions and widths follow the listing; callees are explicit boundaries.
 * The original stack slots are modelled with C locals (the frame layout is not
 * evidence), but every load/store width, signedness, call order and x87
 * sequence follows the listing.
 *
 * Original callee conventions were taken from the call sites (push counts and
 * the receiver setup in ECX); the register-derived parameters reported for
 * cm_fn_004cbb60 / cm_fn_0090cfc0 cannot be expressed in a C prototype, so
 * the prototypes below reproduce the original stack footprint exactly.
 */
#include "cmre.h"

#define G_9CF684 CM_GLOBAL(int, 0x009cf684)
#define G_AE2364 CM_GLOBAL(int, 0x00ae2364)
#define G_AE23BC CM_GLOBAL(int, 0x00ae23bc)
#define G_AE28F0 CM_GLOBAL(u32, 0x00ae28f0)
#define G_AE2A38 CM_GLOBAL(u32, 0x00ae2a38)
#define G_AE2C74 CM_GLOBAL(u32, 0x00ae2c74)
#define G_AE2D0C CM_GLOBAL(u32, 0x00ae2d0c)
#define G_B63D28 CM_GLOBAL(u32, 0x00b63d28)
#define G_B74318 CM_GLOBAL(u32, 0x00b74318)
#define G_B74350 CM_GLOBAL(u32, 0x00b74350)

#define D_967888 CM_GLOBAL(double, 0x00967888)
#define D_968928 CM_GLOBAL(double, 0x00968928)
#define D_968998 CM_GLOBAL(double, 0x00968998)
#define D_9689A0 CM_GLOBAL(double, 0x009689a0)
#define D_968D68 CM_GLOBAL(double, 0x00968d68)
#define D_968D70 CM_GLOBAL(double, 0x00968d70)

extern void CM_THISCALL cm_fn_004cb0a0(u32 self, int param_1, int param_2, int param_3);
extern void CM_THISCALL cm_fn_004cb250(u32 self, int *param_1, int param_2, int param_3);
extern u32 CM_THISCALL cm_fn_004cbb60(u32 self, u32 record, u32 club);
extern u32 CM_THISCALL cm_fn_004cbd20(u32 self, u32 person, u32 club, u32 output);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_00540980(u32 address, u32 flags);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_THISCALL cm_fn_0055d270(u32 self, u32 person, u32 club_key, u32 amount_arg, u32 match_arg);
extern u32 CM_THISCALL cm_fn_00599960(u32 self, u32 value);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern void CM_CDECL cm_fn_006d26b0(int param_1, int param_2, int param_3, int param_4,
                                     int *param_5, float param_6, u32 param_7, char param_8);
extern u32 CM_THISCALL cm_fn_007757d0(u32 self, u32 person_argument, u32 date_argument,
                                      u32 club_filter_argument, u32 text_argument, u32 match_argument);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);

#define RB(p)  (*(volatile u8 *)(p))
#define RW(p)  (*(volatile u16 *)(p))
#define RD(p)  (*(volatile u32 *)(p))
#define WB(p, v) (*(volatile u8 *)(p) = (u8)(v))

/* 0x4caa4f..0x4caa69 / 0x4caa92..0x4caaae:
   mov eax,0xb60b60b7 ; imul ecx ; add edx,ecx ; sar edx,6 ; add edx,(edx>>31)
   i.e. the signed division of x by 90 as MSVC lowers it. */
static u32 cm_div90(s32 x)
{
    long long prod = (long long)x * -1240768329L; /* 0xb60b60b7 */
    u32 hi = (u32)((unsigned long long)prod >> 32);
    u32 t = hi + (u32)x;
    s32 st = (s32)t;
    st = st >> 6;
    st = st + (s32)((u32)st >> 31);
    return (u32)st;
}

/* fcomp against a memory operand: C3 (ah bit 6, tested at 0x4caf9e) is set
   when ST0 == mem or the comparison is unordered. */
static int cm_fcomp_c3(double a, double b)
{
    if (a < b) return 0;
    if (a > b) return 0;
    return 1;
}

void CM_THISCALL cm_fn_004ca6d0(u32 self_, u32 rec_, u32 arr_)
{
    u32 rec = rec_;
    u32 arr = arr_;
    u32 buf[100];
    s32 flag_a = 0;
    s32 flag_b = 0;
    u32 sel;
    int i;

    /* 0x4ca6e5: two guard bytes of the argument record. */
    if (RB(rec + 0x40) == 0) return;
    if (RB(rec + 0x41) == 0) return;

    /* 0x4ca6fe: pick the direction pair. */
    {
        u8 c47 = RB(rec + 0x47);
        if (c47 == 0xff) {
            s8 c43 = (s8)RB(rec + 0x43);
            s8 c44 = (s8)RB(rec + 0x44);
            if (c43 > c44) { flag_a = 1; flag_b = 0; }
            else if (c43 < c44) { flag_a = 0; flag_b = 1; }
            else { flag_a = 0; flag_b = 0; }
        } else {
            s8 c47s = (s8)c47;
            s8 c48s = (s8)RB(rec + 0x48);
            if (c47s > c48s) { flag_a = 1; flag_b = 0; }
            else if (c47s < c48s) { flag_a = 0; flag_b = 1; }
            else { flag_a = 0; flag_b = 0; }
        }
    }

    /* 0x4ca750: copy both 50-entry pointer tables into one contiguous block. */
    {
        u32 src = RD(rec + 0x1c) + 0xd7;
        for (i = 0; i < 0x32; i++) buf[i] = RD(src + 4 * (u32)i);
        src = RD(rec + 0x20) + 0xd7;
        for (i = 0; i < 0x32; i++) buf[0x32 + i] = RD(src + 4 * (u32)i);
    }

    /* ================= first pass over the two clubs ================= */
    for (sel = 0; sel < 2; sel++) {
        u32 club = (sel == 0) ? RD(rec + 0x1c) : RD(rec + 0x20);
        s32 out;
        u32 res = cm_fn_00540a50(club, (u32)&out, 1);
        u32 mgr;
        u32 base;
        u32 ptr1;
        u32 ptr2;
        u32 ptr3;
        u8 counter;

        /* 0x4ca7a9: a live club with no answer is skipped. */
        if (res != 0 && out == 0) continue;

        /* 0x4ca7b9: 1.25 scale comparison between the two clubs. */
        if (RB(rec + 0x3a) == 1) {
            u32 other_rec = (sel == 0) ? RD(rec + 0x20) : RD(rec + 0x1c);
            s16 v = (s16)RW(club + 0x80);
            s16 o = (s16)RW(other_rec + 0x80);
            long double prod = (long double)(s32)v * (long double)D_968928;
            double lim = (double)(long double)(s32)((s32)v + 0x5dc);
            long double mx;
            long double oth;
            /* fcom qword [mem]: keep ST0 when it is greater, else take mem. */
            mx = (prod > (long double)lim) ? prod : (long double)lim;
            oth = (long double)(s32)o;
            /* fcompp: skip when the maximum is greater than the other value. */
            if (mx > oth) continue;
        }

        /* 0x4ca885: manager object, then the 20-entry list of the club. */
        mgr = cm_fn_005a0590(G_AE2A38, club);
        base = arr + 1573u * sel;
        ptr1 = base + 0x190;
        ptr2 = base + 0x1c2;
        ptr3 = base + 0x154;

        for (counter = 0; counter < 0x14; counter++, ptr1 += 0x49, ptr2 += 0x49, ptr3 += 2) {
            u32 id = RD(ptr1);
            u32 id2;
            u32 tbl;
            u32 entry;
            u32 person = 0;
            u8 bl;

            if (id == 0xffffffffu) continue;
            id2 = RD(ptr1 + 4);
            if (id2 == 0xffffffffu) continue;

            tbl = (u32)G_AE23BC + 110u * id;
            entry = tbl;
            if (tbl != 0) {
                if ((s32)RD(tbl) >= G_AE2364) continue;
            }

            if (cm_fn_0053b900((u32 *)club) != 0) {
                /* 0x4ca91f: no person record for this club. */
                person = 0;
            } else {
                person = cm_fn_004cf5f0(self_, entry, club);
                if (person == 0) continue;
            }

            if (person != 0) {
                /* 0x4ca93b: rating adjustments of the person. */
                u32 f14 = RD(rec + 0x14);
                int skip = 0;
                if (f14 != 0) {
                    if ((u32)cm_fn_005c6710((void *)G_AE2C74, f14) != 0) {
                        skip = 1;
                    } else if ((s32)RD(rec) == (s32)G_9CF684) {
                        skip = 1;
                    }
                }
                if (!skip) {
                    u8 state = RB(person + 0x35) & 0x3f;
                    if (state != 6 && state != 5) {
                        u8 v = RB(ptr1 + 0x21);
                        if (v != 0) {
                            s32 f10 = (s32)RD(person + 0x10);
                            if (f10 > 0) {
                                cm_fn_00599960(mgr, (u32)v * (u32)f10);
                            }
                        }
                        v = RB(ptr1 + 0x27);
                        if (v != 0) {
                            s32 fp14 = (s32)RD(person + 0x14);
                            if (fp14 > 0) {
                                cm_fn_00599960(mgr, (u32)v * (u32)fp14);
                            }
                        }
                        if (RW(ptr3) == 1) {
                            u8 v2 = RB(ptr1 + 0x22);
                            if (v2 != 0) {
                                s32 f18 = (s32)RD(person + 0x18);
                                if (f18 > 0) {
                                    cm_fn_00599960(mgr, (u32)RB(ptr1 + 0x27) * (u32)f18);
                                }
                            }
                        }
                    }
                }
            }

            /* 0x4ca9df: the scaled weight byte. */
            if (counter < 0xb) {
                u8 v = RB(ptr1 + 0x30);
                bl = ((s8)v < 0) ? 0x5a : v;
            } else {
                u8 v = RB(ptr1 + 0x32);
                if ((s8)v < 0) {
                    bl = 0;
                } else {
                    s32 ax = 0x5a - (s32)(s8)v;
                    bl = (ax > 0) ? (u8)ax : 0;
                }
            }

            /* 0x4caa0f */
            if (!(counter >= 0xb && RB(ptr2) == 0xff)) {
                if (cm_fn_0053b900((u32 *)club) != 0) {
                    /* 0x4caa34: the club table entry takes the weight. */
                    u32 eid = RD(entry);
                    u32 row = G_AE2D0C + 79u * eid + 9u;
                    u32 delta = cm_div90(10 * (s32)(s8)bl);
                    u8 b = RB(row);
                    b = (u8)(b + (u8)delta);
                    WB(row, b);
                    if ((s8)RB(row) > 0x64) WB(row, 0x64);
                } else {
                    /* 0x4ca92: the person record takes the weight. */
                    u32 delta = cm_div90(10 * (s32)(s8)bl);
                    u8 al = RB(person + 0x3d);
                    s32 dl = (s32)(s8)(u8)delta;
                    s32 ecx = (s32)(s8)al;
                    if ((s8)al > 0x32) {
                        ecx += dl;
                        if (ecx > 0x64) {
                            WB(person + 0x3d, 0x64);
                        } else if (ecx < -0x64) {
                            WB(person + 0x3d, 0x9c);
                        } else {
                            WB(person + 0x3d, (u8)(al + (u8)delta));
                        }
                    } else {
                        u8 a2;
                        ecx += dl;
                        if (ecx > 0x32) {
                            WB(person + 0x3d, 0x32);
                        } else {
                            a2 = (u8)(al + (u8)delta);
                            if ((s8)a2 > 0x64) WB(person + 0x3d, 0x64);
                            else if ((s8)a2 < (s8)0x9c) WB(person + 0x3d, 0x9c);
                            else WB(person + 0x3d, a2);
                        }
                    }
                }
            }

            /* 0x4caadb */
            if (cm_fn_0053b900((u32 *)club) != 0) goto entry_done;
            if (!((s32)RD(rec) < 0)) {
                if ((u32)cm_fn_005c6710((void *)G_AE2C74, RD(rec + 0x14)) != 0) goto entry_done;
            }

            /* 0x4cab0f */
            if (counter >= 0xb ||
                ((s8)bl <= 0x1e && RB(ptr1 + 0x1f) == 0 && (s8)RB(ptr1 + 0x20) >= 0x46)) {
                /* 0x4cab2c: the two random draws and the scale test. */
                        u8 f44 = RB(person + 0x44);
                        double bl_d = (double)(s32)(s8)bl;
                        u32 r1 = (u32)cm_fn_0090cfc0(0x1e);
                        s32 edx = (s32)(s8)f44;
                        long double st1 = (long double)(s32)r1;
                        long double st0 = (long double)edx;
                        long double thr;
                        u32 r2;
                        long double st;

                        st0 = st0 * (long double)D_968D70;
                        st0 = (long double)D_968D68 - st0;
                        st0 = st0 + st1;
                        thr = (double)st0;

                        r2 = (u32)cm_fn_0090cfc0(0x1e);
                        st = (long double)(s32)r2;
                        st = (long double)thr - st;
                        /* fcomp qword [bl]: greater than the stored double. */
                        if (st > (long double)bl_d) {
                            /* 0x4cac20 */
                            WB(person + 0x3f, 0);
                            {
                                u8 a = RB(person + 0x3e);
                                if (a != 0x64) {
                                    a = (u8)(a + 1);
                                    WB(person + 0x3e, a);
                                }
                            }
                            {
                                u8 al = RB(person + 0x3d);
                                s32 ecx = (s32)(s8)al - 4;
                                if (ecx > 0x64) WB(person + 0x3d, 0x64);
                                else if (ecx < -0x64) WB(person + 0x3d, 0x9c);
                                else WB(person + 0x3d, (u8)(al - 4));
                            }
                            cm_fn_004cb250(self_, (int *)entry, club, person);
                            goto entry_done;
                        }
                    }

            /* 0x4cab95 */
            WB(person + 0x3e, 0);
            {
                u8 a = RB(person + 0x3f);
                if (a != 0x64) {
                    a = (u8)(a + 1);
                    WB(person + 0x3f, a);
                }
            }
            if ((s8)RB(person + 0x3f) >= 5) {
                if (cm_fn_00600a50(club, 0, 1, 0, 0) == 0) {
                    cm_fn_00540980(entry, 1);
                    cm_fn_004cb0a0(ptr2, (int)entry, club, person);
                } else {
                    cm_fn_004cb0a0(self_, (int)entry, club, person);
                }
            } else {
                cm_fn_004cb0a0(self_, (int)entry, club, person);
            }

        entry_done:
            /* 0x4cac6a: the direction flags drive a +/-3 attribute tweak. */
            {
                s32 this_flag = (sel == 0) ? flag_a : flag_b;
                s32 other_flag = (sel == 0) ? flag_b : flag_a;
                if (this_flag != 0) {
                    /* 0x4cac7d */
                    s32 b57 = (s32)(s8)RB(entry + 0x57);
                    u32 rnd = (u32)cm_fn_0090cfc0(0x15 - b57);
                    if ((s32)rnd < 4 && person != 0) {
                        u8 al = RB(person + 0x44);
                        s32 ecx = (s32)(s8)al + 3;
                        if (ecx > 0x64) WB(person + 0x44, 0x64);
                        else if (ecx < -0x64) WB(person + 0x44, 0x9c);
                        else WB(person + 0x44, (u8)(al + 3));
                    }
                } else if (other_flag != 0) {
                    /* 0x4cacc8 */
                    s32 b57 = (s32)(s8)RB(entry + 0x57);
                    u32 rnd = (u32)cm_fn_0090cfc0(0x15 - b57);
                    if ((s32)rnd < 4 && person != 0) {
                        u8 al = RB(person + 0x44);
                        s32 ecx = (s32)(s8)al - 3;
                        if (ecx > 0x64) WB(person + 0x44, 0x64);
                        else if (ecx < -0x64) WB(person + 0x44, 0x9c);
                        else WB(person + 0x44, (u8)(al - 3));
                    }
                }
            }
            /* 0x4cad09: drop the entry from the copied table. */
            {
                u32 idx0 = 50u * sel;
                u8 k;
                for (k = 0; k < 0x32; k++) {
                    if (buf[idx0 + k] == entry) {
                        buf[idx0 + k] = 0;
                        break;
                    }
                }
            }
        }
    }

    /* ================= second pass over the two clubs ================= */
    for (sel = 0; sel < 2; sel++) {
        u32 club = (sel == 0) ? RD(rec + 0x1c) : RD(rec + 0x20);
        u32 res = cm_fn_00540a50(club, 0, 1);
        u8 threshold = (res != 0) ? 14 : 8;
        u32 p;
        u32 count;

        /* 0x4cadb3: the same scale comparison as in the first pass. */
        if (RB(rec + 0x3a) == 1) {
            u32 other_rec = (sel == 0) ? RD(rec + 0x20) : RD(rec + 0x1c);
            s16 v = (s16)RW(club + 0x80);
            s16 o = (s16)RW(other_rec + 0x80);
            long double prod = (long double)(s32)v * (long double)D_968928;
            double lim = (double)(long double)(s32)((s32)v + 0x5dc);
            long double mx;
            mx = (prod > (long double)lim) ? prod : (long double)lim;
            if (mx > (long double)(s32)o) continue;
        }

        p = (u32)&buf[50 * sel];
        count = 0x32;
        do {
            u32 entry = RD(p);
            u32 person;

            if (entry != 0) {
                if ((s32)RD(entry) < G_AE2364) {
                    if (RD(entry + 0x61) != 0) {
                        if (cm_fn_0053b900((u32 *)club) == 0) {
                            person = cm_fn_004cf5f0(self_, entry, club);
                            if (person != 0) {
                                /* 0x4caee7 */
                                if ((s32)RD(rec) != -2) {
                                    if (cm_fn_0055d270(G_AE28F0, entry, RD(rec + 0x14), 1, 0) != 0) {
                                        goto next_entry;
                                    }
                                }
                                if (cm_fn_0062c400(G_B63D28, entry) != 0) goto next_entry;
                                if (cm_fn_007757d0(G_B74318, entry, 0x00ae2c90u, club, 0, 0) != 0) {
                                    goto next_entry;
                                }
                                /* 0x4caf3f */
                                {
                                    s32 eid = (s32)RD(entry);
                                    if (eid < G_AE2364) {
                                        s32 v = (s32)RD(G_AE2D0C + 79u * (u32)eid + 4u);
                                        if (v >= 0) {
                                            u32 acc = (u32)v;
                                            acc = acc * 5u;
                                            acc = acc * 16u;
                                            acc = acc + RD(self_);
                                            if (acc != 0) goto next_entry;
                                        }
                                    }
                                }
                                /* 0x4caf77 */
                                {
                                    u32 other_club = (sel == 0) ? RD(rec + 0x20) : RD(rec + 0x1c);
                                    u32 g = G_B74350;
                                    double g18 = *(volatile double *)(g + 0x18);
                                    double g10 = *(volatile double *)(g + 0x10);
                                    long double val;
                                    if (cm_fcomp_c3(g18, D_9689A0)) {
                                        val = (long double)D_968998;
                                    } else {
                                        val = (long double)g10 / (long double)g18;
                                    }
                                    val = val - (long double)D_967888;
                                    cm_fn_006d26b0(0, (int)entry, club, other_club, (int *)rec,
                                                   (float)val, 0, 0);
                                }
                                /* 0x4cafcd */
                                {
                                    u8 al = RB(person + 0x3e);
                                    WB(person + 0x3f, 0);
                                    if (al != 0x64) {
                                        al = (u8)(al + 1);
                                        WB(person + 0x3e, al);
                                    }
                                }
                                /* 0x4cafe0 */
                                if ((s8)RB(person + 0x3e) >= (s8)threshold) {
                                    if (cm_fn_00600a50(club, 0, 1, 0, 0) == 0) {
                                        if (cm_fn_00540a50(club, 0, 1) == 0) {
                                            u32 r = cm_fn_004cbb60(self_, entry, club);
                                            if (r == 2 || r == 4) {
                                                cm_fn_004cbd20(self_, entry, club, 0);
                                            }
                                        }
                                    }
                                }
                                /* 0x4cb030 */
                                {
                                    u8 al = RB(person + 0x3d);
                                    s32 ecx = (s32)(s8)al - 8;
                                    if (ecx > 0x64) WB(person + 0x3d, 0x64);
                                    else if (ecx < -0x64) WB(person + 0x3d, 0x9c);
                                    else WB(person + 0x3d, (u8)(al - 8));
                                }
                                cm_fn_004cb250(self_, (int *)entry, club, person);
                            }
                        }
                    }
                }
            }
        next_entry:
            p += 4;
            count--;
        } while (count != 0);
    }
}
