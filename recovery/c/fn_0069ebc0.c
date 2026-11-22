#include "cmre.h"

#define DAT_00adadfc CM_GLOBAL(u32, 0x00adadfc)
#define DAT_00ae1050 CM_GLOBAL(u32, 0x00ae1050)
#define DAT_00ae2364 CM_GLOBAL(u32, 0x00ae2364)
#define DAT_00ae23bc CM_GLOBAL(u32, 0x00ae23bc)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00b7431c CM_GLOBAL(u32, 0x00b7431c)
#define DAT_00b74448 CM_GLOBAL(u32, 0x00b74448)
#define DAT_00de1f44 CM_GLOBAL(u32, 0x00de1f44)

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern void CM_THISCALL cm_fn_00417c10(u32 *recv, u32 *arg1, u16 *arg3);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 recv, u32 arg);
extern u32 CM_THISCALL cm_fn_004d0b50(u32 recv, u32 arg);
extern short * CM_THISCALL cm_fn_00549f70(short *recv, short p2, char p3, u16 p4, u32 p5);
extern void cm_fn_005e8290(const char *p1, const char *p2, int p3);
extern void CM_THISCALL cm_fn_00690730(u32 recv, u32 p1, u32 p2);
extern u32 CM_THISCALL cm_fn_00690890(u32 recv, u32 p1, u32 p2);
extern void CM_THISCALL cm_fn_00694c80(u32 recv, u32 argp);
extern void CM_THISCALL cm_fn_0069ba50(int *recv, int *player, int *arg3);
extern void CM_THISCALL cm_fn_006a2e00(u32 recv, u32 a1, u32 a2, u32 a3, s8 a4);
extern u32 CM_THISCALL cm_fn_007874b0(u32 recv, u32 person, u32 value);
extern u32 CM_THISCALL cm_fn_007f7270(u32 recv, u32 person);
extern void CM_THISCALL cm_fn_008674e0(void *recv, u32 flag);
extern u32 CM_THISCALL cm_fn_008b2470(u32 recv, u32 a1, u32 a2, u32 a3);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 dest, u32 fmt, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 a1, u32 a2, u32 a3, void *a4, void *a5);

void CM_THISCALL cm_fn_0069ebc0(u32 self, u32 arg1, u32 unused2)
{
    u8 frame[0x214];
    u32 edi_val = 0;
    u32 iVar4 = 0;
    u32 iVar6 = 0;
    u32 uVar5 = 0;
    u32 *ptr;
    u32 t;
    u32 club = arg1;

    arg1 = unused2;

#define ERRB(CODE) do { \
    (void)cm_fn_00944cff((u32)0x009f18a4, 0, 0, (void *)(frame + 20), (void *)(frame + 276)); \
    uVar5 = (u32)cm_fn_0090d130(); \
    (void)cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, uVar5, (u32)(frame + 20), (u32)(frame + 276), (u32)(CODE)); \
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0); \
    DAT_00b67a34 = 0; \
} while (0)

    *(u32 *)(frame + 0) = self;

    iVar4 = cm_fn_0053b900((u32 *)club);
    if (iVar4 == 0) goto L_69ebfd;
    edi_val = *(u32 *)((u8 *)arg1 + 0x24);
    if (edi_val == 0) goto L_69ebfd;
    {
        u8 al = *(u8 *)((u8 *)arg1 + 0x28);
        if (al == 5 || al == 0xc) goto L_69ec18;
    }
L_69ebfd:
    iVar4 = cm_fn_0053b900((u32 *)club);
    if (iVar4 != 0) goto L_69ec11;
    edi_val = *(u32 *)((u8 *)arg1 + 0x39);
    if (edi_val != 0) goto L_69ec20;
L_69ec11:
    edi_val = 0;
    goto L_69eccc;
L_69ec18:
    if (edi_val == 0) goto L_69eccc;
L_69ec20:
    iVar6 = cm_fn_0053b900((u32 *)club);
    if (iVar6 != 0) goto L_69ec65;
    {
        u8 al = *(u8 *)((u8 *)arg1 + 0x3d);
        if (al != 5 && al != 0xc) goto L_69ec65;
        if (edi_val != *(u32 *)((u8 *)arg1 + 0x39)) goto L_69ec65;
        {
            u32 a = *(u32 *)((u8 *)edi_val + 0x53);
            if (a == 0) goto L_69eca0;
            if (*(u8 *)((u8 *)a + 0x11c) == 0) goto L_69eca0;
            {
                u32 b = *(u32 *)((u8 *)edi_val + 0x57);
                if (b == 0) goto L_69eca0;
                {
                    u32 c = *(u32 *)b;
                    u32 d = DAT_00adadfc;
                    if (*(u32 *)((u8 *)d + c * 4) == 0) goto L_69eca0;
                }
            }
        }
    }
    goto L_69ec97;
L_69ec65:
    iVar6 = cm_fn_0053b900((u32 *)club);
    if (iVar6 == 0) goto L_69eca0;
    {
        u8 al = *(u8 *)((u8 *)arg1 + 0x28);
        if (al == 5 || al == 0xc) {
            if (edi_val == *(u32 *)((u8 *)arg1 + 0x24)) {
                if (*(s16 *)((u8 *)edi_val + 0x80) > 0xdac) {
                    u32 a = *(u32 *)((u8 *)edi_val + 0x53);
                    if (*(u32 *)((u8 *)a + 0x71) == 0) goto L_69ec97;
                }
            }
        }
    }
    goto L_69eca0;
L_69ec97:
    cm_fn_0069ba50((int *)self, (int *)edi_val, (int *)arg1);
L_69eca0:
    iVar6 = cm_fn_0053b900((u32 *)club);
    if (iVar6 != 0) goto L_69eccc;
    {
        u8 al = *(u8 *)((u8 *)arg1 + 0x3d);
        if (!((al == 0xf || al == 0xc) && (al == 0xc || al == 0xf))) goto L_69eccc;
    }
    cm_fn_004d0b50(DAT_00ae1050, arg1);

L_69eccc:
    {
        u32 r = (u32)(u32)cm_fn_00549f70((short *)(frame + 12), (short)0x17, (char)7, (u16)0x7b3, (u32)0xffffffff);
        u32 recv = 73 * (*(u32 *)club) + (*(u32 *)self) + 0x20;
        cm_fn_00417c10((u32 *)recv, (u32 *)(frame + 4), (u16 *)r);
    }
    if (*(u32 *)((u8 *)arg1 + 0x69) == 0) {
        (void)cm_fn_00944cff((u32)0x009f18a4, 0, 0, (void *)(frame + 276), (void *)(frame + 20));
        uVar5 = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, uVar5, (u32)(frame + 276), (u32)(frame + 20), (u32)0x3480);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        DAT_00b67a34 = 0;
    }

    iVar6 = cm_fn_0053b900((u32 *)club);
    if (iVar6 != 0) {
        if (edi_val == 0 || edi_val != *(u32 *)((u8 *)arg1 + 0x24)) goto L_69f4d1;
        {
            s8 sv = *(s8 *)((u8 *)arg1 + 0x28);
            u32 ix = (u32)(sv - 5);
            if (ix > 10) goto L_69f4ab;
            (void)CM_GLOBAL(volatile u32, 0x0069f788 + ix * 4);
            switch (ix) {
                case 0: goto L_69f429;
                case 1: goto L_69f007x;
                case 3: goto L_69f007x;
                case 6: goto L_69f25f;
                case 7: goto L_69f2e4;
                case 10: goto L_69f19c;
                default: goto L_69f4ab;
            }
        }
    } else {
        if (edi_val == 0 || edi_val != *(u32 *)((u8 *)arg1 + 0x39)) goto L_69f162;
        {
            s8 sv = *(s8 *)((u8 *)arg1 + 0x3d);
            u32 ix = (u32)(sv - 5);
            if (ix > 10) goto L_69f131;
            (void)CM_GLOBAL(volatile u32, 0x0069f75c + ix * 4);
            switch (ix) {
                case 0: goto L_69f09c;
                case 1: goto L_69f007;
                case 3: goto L_69f007;
                case 6: goto L_69eea7;
                case 7: goto L_69ef3c;
                case 10: goto L_69ed9d;
                default: goto L_69f131;
            }
        }
    }

L_69f09c:
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 3;
    } else {
        ERRB(0x34c2);
        uVar5 = 3;
    }
    cm_fn_006a2e00(self, arg1, edi_val, club, (s8)uVar5);
    goto L_69f4e8;

L_69f007:
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 2;
    } else {
        ERRB(0x34b9);
        uVar5 = 2;
    }
    cm_fn_006a2e00(self, arg1, edi_val, club, (s8)uVar5);
    goto L_69f4e8;

L_69eea7:
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
        cm_fn_00690730(self, arg1, edi_val);
    } else {
        ERRB(0x34a0);
    }
    uVar5 = 1;
    cm_fn_006a2e00(self, arg1, edi_val, club, (s8)uVar5);
    goto L_69f4e8;

L_69ef3c:
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
        cm_fn_00690730(self, arg1, edi_val);
    } else {
        ERRB(0x34a9);
    }
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 3;
    } else {
        ERRB(0x34af);
        uVar5 = 3;
    }
    cm_fn_006a2e00(self, arg1, edi_val, club, (s8)uVar5);
    goto L_69f4e8;

L_69ed9d:
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
        cm_fn_00690730(self, arg1, edi_val);
    } else {
        ERRB(0x3491);
    }
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 2;
    } else {
        ERRB(0x3497);
        uVar5 = 2;
    }
    cm_fn_006a2e00(self, arg1, edi_val, club, (s8)uVar5);
    goto L_69f4e8;

L_69f131:
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) cm_fn_00690890(self, arg1, edi_val);
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) cm_fn_00690730(self, arg1, edi_val);
    uVar5 = 2;
    cm_fn_006a2e00(self, arg1, edi_val, club, (s8)uVar5);
    goto L_69f4e8;

L_69f162:
    cm_fn_006a2e00(self, arg1, 0, club, 2);
    goto L_69f4e8;

L_69f429:
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 3;
    } else {
        ERRB(0x3526);
        uVar5 = 3;
    }
    goto L_69f4d6;

L_69f007x:
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 2;
    } else {
        ERRB(0x351d);
        uVar5 = 2;
    }
    goto L_69f4d6;

L_69f25f:
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
        cm_fn_00690730(self, arg1, edi_val);
    } else {
        ERRB(0x3504);
    }
    uVar5 = 1;
    goto L_69f4d6;

L_69f2e4:
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
        cm_fn_00690730(self, arg1, edi_val);
    } else {
        ERRB(0x350d);
    }
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 2;
    } else {
        ERRB(0x3513);
        uVar5 = 2;
    }
    goto L_69f4d6;

L_69f19c:
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
        cm_fn_00690730(self, arg1, edi_val);
    } else {
        ERRB(0x34f5);
    }
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) {
        cm_fn_00690890(self, arg1, edi_val);
        uVar5 = 2;
    } else {
        ERRB(0x34fb);
        uVar5 = 2;
    }
    goto L_69f4d6;

L_69f4ab:
    if (*(u32 *)((u8 *)arg1 + 0x69) != 0) cm_fn_00690890(self, arg1, edi_val);
    if (*(u32 *)((u8 *)arg1 + 0x61) != 0) cm_fn_00690730(self, arg1, edi_val);
    uVar5 = 2;
    goto L_69f4d6;

L_69f4d1:
    cm_fn_006a2e00(self, arg1, 0, club, 2);
    if (*(u8 *)((u8 *)arg1 + 0x28) != 5) *(u8 *)((u8 *)arg1 + 0x28) = 5;
    goto L_69f4e8;

L_69f4d6:
    cm_fn_006a2e00(self, arg1, edi_val, club, (s8)uVar5);
    if (*(u8 *)((u8 *)arg1 + 0x28) != 5) *(u8 *)((u8 *)arg1 + 0x28) = 5;

L_69f4e8:
    if ((s32)*(u32 *)arg1 >= (s32)(DAT_00ae2364 - 0x10)) {
        cm_fn_007874b0(DAT_00b7431c, arg1, edi_val);
        {
            u32 a = *(u32 *)((u8 *)club + 0x53);
            if (a != 0 && (*(u8 *)((u8 *)a + 0x11c) & 1) != 0) {
                if (edi_val != 0) {
                    u32 ebp2 = *(u32 *)((u8 *)edi_val + 0x53);
                    if (ebp2 != 0) {
                        u32 cnt = DAT_00ae2364;
                        s32 ix2 = (s32)(cnt - 0x10);
                        if (ix2 < (s32)cnt) {
                            u32 base = DAT_00ae23bc;
                            u32 ea = base + 0x39 + (u32)ix2 * 0x6e;
                            for (; ix2 < (s32)cnt; ix2++, ea += 0x6e) {
                                if (*(u8 *)((u8 *)ea + 0x26) != 0 &&
                                    (u32)((u8 *)ea - 0x39) != arg1 &&
                                    *(u32 *)ea != 0) {
                                    u32 i3 = *(u32 *)((u8 *)(*(u32 *)ea) + 0x57);
                                    if (i3 != 0 && *(u32 *)((u8 *)i3 + 0x5d) == ebp2) goto L_69f590;
                                }
                            }
                        }
                        *(u8 *)((u8 *)ebp2 + 0x11c) &= (u8)0xfd;
                        *(u8 *)((u8 *)(*(u32 *)((u8 *)edi_val + 0x53)) + 0x11c) |= (u8)1;
                    }
                }
L_69f590:
                {
                    u32 a2 = *(u32 *)((u8 *)club + 0x53);
                    *(u8 *)((u8 *)a2 + 0x11c) &= (u8)0xfe;
                    a2 = *(u32 *)((u8 *)club + 0x53);
                    *(u8 *)((u8 *)a2 + 0x11c) |= (u8)2;
                }
            }
        }
    }
    {
        u32 chk = cm_fn_0053b900((u32 *)club);
        u32 recv2 = chk ? (u32)((u8 *)arg1 + 0x29) : (u32)((u8 *)arg1 + 0x3e);
        cm_fn_00417c10((u32 *)recv2, (u32 *)(frame + 4), (u16 *)0x00ae2c90);
    }
    *(u32 *)((u8 *)club + 0xcf) = arg1;
    iVar6 = cm_fn_0053b900((u32 *)club);
    if (iVar6 == 0) {
        u32 a = *(u32 *)((u8 *)arg1 + 0x61);
        *(u32 *)((u8 *)arg1 + 0x39) = club;
        if (a != 0) {
            s32 d = (s32)*(s16 *)((u8 *)a + 0xb) + 0x2ee;
            if (d > *(s16 *)((u8 *)club + 0x80)) {
                u32 k = 0;
                u32 *q = (u32 *)((u8 *)club + 0xd7);
                while (k < 0x32) {
                    if (*q == 0) {
                        if (k < 0x32) {
                            *(u32 *)((u8 *)club + 0xd7 + k * 4) = arg1;
                            *(u8 *)((u8 *)arg1 + 0x3d) = 0xc;
                            goto L_69f643;
                        }
                        break;
                    }
                    k++;
                    q++;
                }
            }
        }
        *(u8 *)((u8 *)arg1 + 0x3d) = 5;
    } else {
        *(u32 *)((u8 *)arg1 + 0x24) = club;
        *(u8 *)((u8 *)arg1 + 0x28) = 5;
    }
L_69f643:
    if ((s32)*(u32 *)arg1 >= (s32)(DAT_00ae2364 - 0x10)) {
        (void)cm_fn_007f7270(DAT_00b74448, arg1);
        (void)cm_fn_008b2470(DAT_00de1f44, arg1, edi_val, club);
    }
    cm_fn_00694c80(self, club);
    iVar4 = cm_fn_0053b900((u32 *)club);
    if (iVar4 == 0) {
        ptr = (u32 *)((u8 *)club + 0xd7);
        iVar4 = 0x32;
        do {
            t = *ptr;
            if (t != 0 && *(u32 *)((u8 *)t + 0x61) != 0) {
                u32 r = cm_fn_004cf5a0(DAT_00ae1050, t);
                if (r != 0) {
                    if (*(u32 *)((u8 *)r + 0x45) & 8) cm_fn_008674e0((void *)r, 8);
                    if (*(u32 *)((u8 *)r + 0x45) & 0x200) cm_fn_008674e0((void *)r, 0x200);
                    if (*(u32 *)((u8 *)r + 0x45) & 0x400) cm_fn_008674e0((void *)r, 0x400);
                    if (*(u32 *)((u8 *)r + 0x45) & 0x2000) cm_fn_008674e0((void *)r, 0x2000);
                    if (*(u32 *)((u8 *)r + 0x45) & 0x10000) cm_fn_008674e0((void *)r, 0x10000);
                    if (*(u32 *)((u8 *)r + 0x45) & 0x80000) cm_fn_008674e0((void *)r, 0x80000);
                    if (*(u32 *)((u8 *)r + 0x45) & 0x800000) cm_fn_008674e0((void *)r, 0x800000);
                }
            }
            ptr++;
            iVar4--;
        } while (iVar4 != 0);
    }
    return;
}
