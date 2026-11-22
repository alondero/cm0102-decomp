/* Reconstruction of preferred-image VA 0x007f8750 (CM0102 3.9.68, base 0x00400000).
   Pinned binary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   cmre test passes at -O0 and -O2; acceptance remains a controller decision. */
#include "cmre.h"

#define DAT_00ae2364 CM_GLOBAL(int, 0x00ae2364)
#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)
#define DAT_00ae2c88 CM_GLOBAL(char, 0x00ae2c88)
#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define DAT_00ae2c92 CM_GLOBAL(u16, 0x00ae2c92)
#define DAT_00ae2c94 CM_GLOBAL(u32, 0x00ae2c94)
#define DAT_00ae1050 CM_GLOBAL(u32, 0x00ae1050)
#define DAT_00ae1080 CM_GLOBAL(u32, 0x00ae1080)
#define DAT_00ae2a58 CM_GLOBAL(u32, 0x00ae2a58)
#define DAT_00b63d28 CM_GLOBAL(u32, 0x00b63d28)

extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1, u32 *param_2, int param_3);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_005acbe0(u32 self, volatile u32 first_day, volatile u32 first_year, volatile u16 last_day, volatile u16 last_year, volatile u32 club_arg, volatile u32 output_arg, volatile u32 key_arg, volatile u8 filter_arg, volatile u8 side_arg);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);
extern u32 CM_THISCALL cm_fn_007f6cd0(u32 self, s8 kind, volatile u32 second);
extern u32 CM_THISCALL cm_fn_007b7490(u32 self, volatile u32 argument_home);
extern void CM_THISCALL cm_fn_007b62a0(int *param_1, int param_2, int param_3, int param_4, char param_5, int *param_6, int *param_7, char param_8);
extern void CM_THISCALL cm_fn_007f9890(u32 self, u32 list);
extern unsigned long long CM_CDECL cm_fn_00945333(u32 eax_in, u32 edx_in, volatile u32 base_arg, volatile u32 count_arg, volatile u32 width_arg, volatile u32 compare_arg);
extern u32 CM_FASTCALL cm_fn_007b3e20(int *param_1);

u32 CM_THISCALL cm_fn_007f8750(int self)
{
    volatile u32 scr[49]; /* L+0x00 .. L+0xc0; SEH frame at L+0xb8 */
#define B(o) (*(volatile u8 *)((char *)scr + (o)))
#define W(o) (*(volatile u16 *)((char *)scr + (o)))
#define D(o) (*(volatile u32 *)((char *)scr + (o)))
    int cnt, off, idx, i, j, p39;
    int iVar1, iVar13, iVar10;
    int *elem;
    u32 rr, res, bc, edi2, r2, saved_date;
    u8 bl, mode;
    u16 dx;

    /* SEH frame: push -1 (state), push handler, push prev; link fs:[0] */
    D(0xc0) = 0xffffffffu;
    D(0xbc) = 0x00960fbeu;
    D(0xb8) = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)((char *)scr + 0xb8);

    /* L+0x38 buffer, ECX = 0xae2c90 */
    cm_fn_0054c5b0((short *)0x00ae2c90, (u32 *)((char *)scr + 0x38), 0xe);
    D(0xc0) = 0; /* dword state = 0 */
    cm_fn_0054a110((u16 *)((char *)scr + 0x50));
    B(0xc0) = 1;
    cm_fn_0054a110((u16 *)((char *)scr + 0x7f));
    B(0xc0) = 2;
    cm_fn_0054a110((u16 *)((char *)scr + 0x97));
    B(0xc0) = 3;
    cm_fn_0068d020((u32)((char *)scr + 0x18));
    mode = DAT_00ae2c88;
    B(0xc0) = 4;

    if (mode != 2) {
        B(0xc0) = 3;
        cm_fn_0068d040((u32)((char *)scr + 0x18));
        CM_FS(u32, 0) = D(0xb8);
        return 0;
    }

    cnt = *(int *)((char *)self + 0x64);
    if (cnt > 0) {
        off = 0;
        idx = 0;
        D(0x0) = 0;
        for (;;) {
            elem = (int *)((char *)*(int **)((char *)self + 0x5c) + off);
            iVar1 = DAT_00ae23bc + (int)((u32)elem[1] * 0x6eu);
            iVar13 = DAT_00ae23b4 + (int)((u32)elem[0] * 0x245u);
            D(0x10) = (u32)iVar1;
            rr = cm_fn_00600a50((u32)iVar13, 1, 1, 0, 0);
            if (rr == 0) goto remove;
            D(0xc) = *(u32 *)((char *)iVar13 + 0xcf);
            if (*(int *)((char *)iVar1 + 0x39) == iVar13) goto remove;

            rr = cm_fn_004cf5a0(DAT_00ae1050, (u32)iVar1);
            DAT_00ae1080 = rr;
            iVar10 = rr ? DAT_00ae23b4 + (int)((u32)*(int *)((char *)rr + 4) * 0x245u) : 0;
            if (iVar10 == iVar13) goto remove;

            /* first date comparison: ECX = base+off+8, buffer L+0x48, size 0x16d */
            saved_date = DAT_00ae2c90;
            D(0x28) = saved_date;
            D(0x2c) = DAT_00ae2c94;
            res = cm_fn_0054c5b0(
                (short *)((char *)*(int **)((char *)self + 0x5c) + off + 8),
                (u32 *)((char *)scr + 0x48), 0x16d);
            dx = W(0x2a);
            {
                u16 rh = *(u16 *)((char *)res + 2);
                s16 rl = *(s16 *)((char *)res + 0);
                if (dx > rh) goto remove;
                if (dx < rh) goto after_first_date;
                if ((s16)(u16)saved_date > rl) goto remove;
            }
        after_first_date:
            if (*(s8 *)((char *)*(int **)((char *)self + 0x5c) + off + 0x10) >= 3) goto remove;

            /* second date comparison: buffer L+0x40, size 0x1e */
            saved_date = DAT_00ae2c90;
            D(0x30) = saved_date;
            D(0x34) = DAT_00ae2c94;
            res = cm_fn_0054c5b0(
                (short *)((char *)*(int **)((char *)self + 0x5c) + off + 8),
                (u32 *)((char *)scr + 0x40), 0x1e);
            dx = W(0x32);
            {
                u16 rh = *(u16 *)((char *)res + 2);
                s16 rl = *(s16 *)((char *)res + 0);
                if (dx > rh) goto do_body;
                if (dx < rh) goto keep;
                if (!((s16)(u16)saved_date > rl)) goto keep;
            }
        do_body:
            if (cm_fn_0062c400(DAT_00b63d28, (u32)iVar1) == 2) goto remove;

            p39 = *(int *)((char *)iVar1 + 0x39);
            if (p39 != 0) {
                int t = *(int *)((char *)p39 + 0x53);
                D(0x8) = (u32)t;
                if (t == 0) {
                    D(0x8) = (u32)*(int *)((char *)iVar1 + 0x1a);
                    if (D(0x8) == 0) goto keep;
                }
            } else {
                D(0x8) = (u32)*(int *)((char *)iVar1 + 0x1a);
                if (D(0x8) == 0) goto keep;
            }

            if (p39 != 0) {
                rr = cm_fn_004cf5f0(DAT_00ae1050, (u32)iVar1, (u32)p39);
                if (rr == 0) goto date_check;
                {
                    u8 al = *(u8 *)((char *)rr + 0x4f);
                    al = (u8)(al & 0xf0);
                    if (al == 0x60 || al == 0x70 || al == 0) goto alloc_loop2;
                }
            }
        date_check:
            if (p39 != 0) {
                u32 club = *(u32 *)p39;
                u32 first_day = (D(0x3a) & 0xffff0000u) | (u16)DAT_00ae2c90;
                W(0x8) = DAT_00ae2c92;
                u32 first_year = D(0x8);
                u32 last_day = (u32)(u16)D(0x38);
                u32 last_year = (u32)(u16)D(0x3a);
                u16 r = (u16)cm_fn_005acbe0(DAT_00ae2a58, first_day, first_year,
                                             (u16)last_day, (u16)last_year, club,
                                             0, (u32)-1, (u8)-2, 1);
                if (r != 0) goto keep;
            }
        alloc_loop2:
            bl = 0;
            D(0x4) = 0;
            for (;;) {
                if ((s8)bl >= 3) goto remove;
                j = (int)D(0x4);
                res = cm_fn_007f6cd0(self, (s8)j, D(0xc));
                bc = D(0x8);
                edi2 = res;
                r2 = cm_fn_007b7490(edi2, bc);
                if (r2 != 0) {
                    cm_fn_007b62a0((int *)edi2, (int)D(0x10),
                                   (int)((char *)scr + 0x50), -1000, 1, 0, 0, 1);
                    bl = (u8)(bl + 1);
                    if ((s8)B(0xb6) > -1) {
                        cm_fn_0068d0d0((volatile u8 *)((char *)scr + 0x18),
                                       (u32)((char *)scr + 0x50),
                                       (const u8 *)((char *)scr + 0x50), 0x67);
                    }
                }
                j = (int)D(0x4);
                j++;
                D(0x4) = (u32)j;
                if (j < 10) continue;
                goto remove;
            }
        remove:
            {
                int *dst = (int *)((char *)*(int **)((char *)self + 0x5c) + off);
                int *srcp = (int *)((char *)*(int **)((char *)self + 0x5c)
                                    + (int)((u32)*(int *)((char *)self + 0x64) * 0x11u) - 0x11);
                dst[0] = srcp[0];
                dst[1] = srcp[1];
                dst[2] = srcp[2];
                dst[3] = srcp[3];
                *(u8 *)((char *)dst + 0x10) = *(u8 *)((char *)srcp + 0x10);
                cnt = (int)(*(volatile u32 *)((char *)self + 0x64) - 1u);
                *(volatile u32 *)((char *)self + 0x64) = (u32)cnt;
                idx--;
                D(0x0) = (u32)idx;
                off -= 0x11;
            }
        keep:
            idx++;
            cnt = *(int *)((char *)self + 0x64);
            off += 0x11;
            D(0x0) = (u32)idx;
            if (idx < cnt) continue;
            break;
        }
    }

    cm_fn_007f9890(self, (u32)((char *)scr + 0x18));
    {
        u32 base = (u32)*(int **)((char *)self + 0x5c);
        u32 count = (u32)*(int *)((char *)self + 0x64);
        cm_fn_00945333(count, D(0x18), base, count, 0x11, 0x007fc270u);
    }

    for (i = 0; i < 0x10; i++) {
        int x = DAT_00ae2364 - i - 1;
        int tbl = DAT_00ae23bc + (int)((u32)x * 0x6eu);
        if (*(u8 *)((char *)tbl + 0x5f) == 1) {
            for (j = 0; j < 10; j++) {
                u32 rr2 = cm_fn_007f6cd0(self, (s8)j, (u32)tbl);
                (void)cm_fn_007b3e20((int *)rr2);
            }
        }
    }

    B(0xc0) = 3;
    cm_fn_0068d040((u32)((char *)scr + 0x18));
    CM_FS(u32, 0) = D(0xb8);
    return 1;
}
