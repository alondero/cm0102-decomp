/* Faithful C translation of fn_0069ba50 (preferred VA 0x0069ba50,
   image base 0x00400000, SHA-256 0c78e7da...f86).
   Stack frame note: `S` models [B+0x10 .. B+0x41] where B is ESP after the
   four callee-saved pushes.  Record R = B+0x1c = S+0x0c (length 0x26) is
   passed to cm_fn_0068d0d0, so its fields must stay adjacent; all of them
   are accessed through the single byte array S.  Bytes the original leaves
   untouched (notably R+0x12 / B+0x2e on the D3/19f fallback paths) are left
   untouched here too: no invented score and no zeroed record. */
#include "cmre.h"

/* S base = B+0x10 */
#define S8(o)   (*(volatile u8  *)(S + (o)))
#define SU16(o) (*(volatile u16 *)(S + (o)))
#define SU32(o) (*(volatile u32 *)(S + (o)))

/* object record index/base, reloaded exactly where the listing reloads them */
#define PIDX()  (*(volatile u32 *)playerp)
#define SBASE() (*(volatile u32 *)selfp)

#define ST16(o, v) do { u32 _i = PIDX(); u32 _b = SBASE(); \
    *(volatile u16 *)(_i * 0x49u + _b + (u32)(o)) = (u16)(v); } while (0)
#define ST8(o, v) do { u32 _i = PIDX(); u32 _b = SBASE(); \
    *(volatile u8  *)(_i * 0x49u + _b + (u32)(o)) = (u8)(v); } while (0)
#define ST32(o, v) do { u32 _i = PIDX(); u32 _b = SBASE(); \
    *(volatile u32 *)(_i * 0x49u + _b + (u32)(o)) = (u32)(v); } while (0)
#define AND8(o, m) do { u32 _i = PIDX(); u32 _b = SBASE(); \
    *(volatile u8 *)(_i * 0x49u + _b + (u32)(o)) &= (u8)(m); } while (0)

#define G_COUNT   CM_GLOBAL(volatile s32, 0x00ae2364)
#define G_BASE    CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_DATE_LO CM_GLOBAL(volatile u32, 0x00dd6ec8)
#define G_DATE_HI CM_GLOBAL(volatile u32, 0x00dd6ecc)
#define G_D0C     CM_GLOBAL(volatile u32, 0x00ae2d0c)
#define G_1050    CM_GLOBAL(volatile u32, 0x00ae1050)
#define G_B63C98  CM_GLOBAL(volatile u32, 0x00b63c98)
#define G_ADADFC  CM_GLOBAL(volatile u32, 0x00adadfc)
#define G_A2C90D  CM_GLOBAL(volatile u32, 0x00ae2c90)
#define G_A2C90W  CM_GLOBAL(volatile u16, 0x00ae2c90)
#define G_A2C92W  CM_GLOBAL(volatile u16, 0x00ae2c92)

extern u32  CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32  CM_THISCALL cm_fn_004cf6d0(u32 self, u32 argument);
extern u32  CM_CDECL    cm_fn_0053b900(u32 *input);
extern u32  CM_CDECL    cm_fn_0053ca90(volatile u8 *self);
extern u32  CM_CDECL    cm_fn_005409b0(u32 *input);
extern u8   CM_CDECL    cm_fn_00544820(u32 *object);
extern void CM_CDECL    cm_fn_00547e70(u32 object, u32 value, u32 selector);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern int  CM_THISCALL cm_fn_0054cdb0(u32 self, u32 a, u32 b);
extern unsigned long long CM_THISCALL cm_fn_005fd210(void *self, u32 arg1);
extern u32  CM_THISCALL cm_fn_005fea50(u32 self, s8 category, u32 person);
extern u32  CM_CDECL    cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);
extern void CM_THISCALL cm_fn_0069ca00(u32 self, volatile u32 club_arg, volatile u32 person_arg);
extern int  CM_THISCALL cm_fn_0069d1b0(int *self, int *a, int *b, u32 c, u32 d);
extern u8   CM_THISCALL cm_fn_0069f810(u32 self, u32 a, u32 b, double c);
extern u8   CM_THISCALL cm_fn_0069fc60(u32 self, u32 item);
extern void CM_THISCALL cm_fn_006a0540(u32 self, volatile u32 *object, volatile u32 *other);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);
extern u32  CM_CDECL    cm_fn_00944c9f(u32 destination, u32 format, ...);

void CM_THISCALL cm_fn_0069ba50(int *self, int *player, int *arg3)
{
    volatile struct { void *prev; void *handler; u32 state; } frame;
    u8 S[0x32];
    u32 selfp = (u32)self;
    u32 playerp = (u32)player;
    u32 slot54 = 0;
    u32 slot58 = (u32)arg3;
    u8  flag54 = 0;
    u32 eax = 0, edx = 0, ecx = 0, ebx = 0, ebp = 0;
    u32 offv, count, lim, rec;
    u32 edx_last = 0;
    union { unsigned long long u; double d; } d35, d315;

    d35.u  = 0x400c000000000000ULL;   /* 3.5   pushed 0x400c0000:0x00000000 */
    d315.u = 0x4009333333333333ULL;   /* 3.15  pushed 0x40093333:0x33333333 */

    frame.state   = 0xffffffff;
    frame.handler = (void *)0x0095cca0;
    frame.prev    = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    cm_fn_0054a110((u16 *)(S + 0x22));   /* ecx = B+0x32 */
    frame.state = 0;
    cm_fn_0054a110((u16 *)(S + 0x2a));   /* ecx = B+0x3a */
    frame.state = 1;
    SU32(0x00) = 0xfff0bdc0;             /* B+0x10 = -1000000 */

    if ((cm_fn_0053b900((u32 *)playerp) == 0) &&
        (*(u32 *)(playerp + 0x57) == 0) &&
        ((s16)*(u16 *)(playerp + 0x80) < 5000))
        goto L_69c525;

    if (cm_fn_0053b900((u32 *)playerp) != 0) {
        if (cm_fn_005409b0((u32 *)playerp) != 0) {
            if (*(s32 *)(*(u32 *)(playerp + 0x53) + 0x88) <= 0x3e8)
                goto L_69c525;
        }
        if (cm_fn_005409b0((u32 *)playerp) == 0) {
            if (*(s32 *)(*(u32 *)(playerp + 0x53) + 0x88) <= 0x64)
                goto L_69c525;
        }
    }

    /* ---- player-record initialisation at *self + *player*0x49 ---- */
    eax = PIDX();                        /* 0x69bb08 */
    SU32(0x1a) = eax;                    /* B+0x2a */
    SU32(0x12) = 0xffffffff;             /* B+0x22 best ID */
    {
        u32 b = SBASE();
        *(volatile u16 *)(eax * 0x49u + b + 6u) = 5000;
    }
    ST16(8, 5000);
    ST16(0x0a, 5000);
    ST16(0x0c, 5000);
    ST8(0, 0);
    ST16(0x0e, 0x10);
    ST16(0x10, 0);
    ST16(0x12, 0);
    ST16(0x14, 0);
    ST16(0x16, 0);
    AND8(2, 0xfe);
    AND8(2, 0xfd);
    AND8(2, 0xfb);
    AND8(2, 0xf7);
    AND8(2, 0xef);
    ST8(4, 0);
    AND8(3, 0xe1);
    AND8(2, 0xdf);
    AND8(2, 0xbf);
    AND8(2, 0x7f);
    ST32(0x28, 0);
    ST8(5, 0);
    AND8(3, 0xfe);
    AND8(0x40, 7);

    eax = slot58;
    if (eax != 0) SU32(0x16) = *(u32 *)eax;   /* B+0x26 */
    else          SU32(0x16) = 0xffffffff;

    /* ---- first scan (random-free full scan) ---- */
    count = (u32)G_COUNT;
    ebp = G_BASE;
    slot54 = 0;
    if ((s32)count <= 0) goto L_69be9c;
    SU32(0x04) = 0;                      /* B+0x14 == 0 */

L_69bced:
    if (*(u32 *)(ebp + 0x69) == 0) goto L_69be74;
    lim = (u32)G_COUNT - 0x10;
    if ((s32)slot54 >= (s32)lim) {
        if (*(u8 *)(ebp + 0x5f) == 0) goto L_69be74;
    }
    if (slot58 == ebp) goto L_69be74;
    if ((s32)slot54 < (s32)lim) goto L_69bd42;
    if (cm_fn_0053b900((u32 *)playerp) == 0) goto L_69bd42;
    if (*(u32 *)(ebp + 0x1a) == *(u32 *)(playerp + 0x53)) goto L_69bd42;
    edx = *(u32 *)(ebp + 0x69);
    if ((s16)*(u16 *)(edx + 0x0c) < 0xfa0) goto L_69be74;

L_69bd42:
    lim = (u32)G_COUNT - 0x10;
    if ((s32)slot54 >= (s32)lim) {
        if (cm_fn_00544820((u32 *)playerp) == 0) goto L_69be74;
    }
    offv = SU32(0x04);
    if ((*(u8 *)(offv + G_D0C + 0x0b) & 0x30) != 0) goto L_69be74;
    if (cm_fn_004cf6d0(G_1050, ebp) != 0) goto L_69be74;
    if (cm_fn_0069fc60(selfp, ebp) != 0) goto L_69be74;
    ecx = *(u32 *)(playerp + 0x53);
    if (ecx == 0) goto L_69be74;
    eax = *(u32 *)(ebp + 0x1a);
    if (eax == 0) goto L_69be74;
    if (ecx != eax) {
        edx = *(u32 *)(ebp + 0x69);
        if ((s16)*(u16 *)(edx + 0x0a) < 0xea6) goto L_69be74;
    }
    if (*(u8 *)(ecx + 0x75) != *(u8 *)(eax + 0x75)) {
        edx = *(u32 *)(ebp + 0x69);
        if ((s16)*(u16 *)(edx + 0x0c) < 0x109a) goto L_69be74;
        if ((s16)*(u16 *)(playerp + 0x80) < 0x128e) goto L_69be74;
    }
    if (!cm_fn_0069f810(selfp, ebp, playerp, d35.d)) goto L_69be74;
    if (*(u32 *)(ebp + 0x61) == 0) goto L_69be3b;
    if (*(u32 *)(ebp + 0x39) == 0) goto L_69be3b;
    {
        u8 al = *(u8 *)(ebp + 0x3d);
        if (!(al == 0x0b || al == 0x0c || al == 0x0d || al == 0x0f))
            goto L_69be3b;
    }
    eax = cm_fn_004cf5a0(G_1050, ebp);
    if (eax == 0) goto L_69be3b;
    if (*(u8 *)(eax + 0x49) != 0) goto L_69be74;
    if (*(u8 *)(eax + 0x20) == 0) goto L_69be74;
L_69be3b:
    if (cm_fn_0053b900((u32 *)playerp) != 0 &&
        *(u8 *)(ebp + 0x3d) != 5) goto L_69be74;
    {
        int r = cm_fn_0069d1b0(self, (int *)ebp, (int *)playerp, 0, 5);
        if (r <= (s32)SU32(0x00)) goto L_69be74;
        SU32(0x12) = slot54;
        SU32(0x1e) = SU32(0x00);         /* B+0x2e (may stay unset on fallback) */
        SU32(0x00) = (u32)r;
    }
L_69be74:
    {
        u32 e2 = slot54 + 1;
        u32 d2 = SU32(0x04) + 0x4f;
        u32 c2 = (u32)G_COUNT;
        ebp += 0x6e;
        slot54 = e2;
        SU32(0x04) = d2;
        if ((s32)e2 < (s32)c2) goto L_69bced;
    }

L_69be9c:
    if (SU32(0x12) != 0xffffffff) goto L_69bebb;
    eax = *(u32 *)(playerp + 0xd3);
    flag54 = 1;
    if (eax == 0) goto L_69c116;
    SU32(0x12) = *(u32 *)eax;
    goto L_69bebb;

L_69c116:
    eax = *(u32 *)(playerp + 0x19f);
    if (eax == 0) goto L_69c12b;
    SU32(0x12) = *(u32 *)eax;
    goto L_69bebb;

    /* ---- fallback search loop (entered with best ID == -1) ---- */
L_69c12b:
    ebp = slot58;
    ebx = 0xffffffff;
L_69c12f:
    ebx = 0xffffffff;
L_69c132:
    if (flag54 != 0) {
        flag54 = 0;
        ebp = (u32)G_COUNT - 0x11;
        SU32(0x04) = ebp;
        SU32(0x12) = ebp;
        goto L_69c16b;
    }
    ebp = ebp - 1;
    if ((s32)ebp < 0) ebp = (u32)G_COUNT - 0x10;
    if (ebp == SU32(0x04)) goto L_69c2d8;
    SU32(0x12) = ebp;
L_69c16b:
    ecx = ebp * 0x4f;
    if ((*(u8 *)(ecx + G_D0C + 0x0b) & 0x30) != 0) {
        SU32(0x12) = ebx;
        goto L_69c132;
    }
    if ((s32)ebp >= (s32)((u32)G_COUNT - 0x10)) {
        SU32(0x12) = ebx;
        goto L_69c132;
    }
    rec = G_BASE + ebp * 0x6e;
    if (*(u32 *)(rec + 0x69) == 0) goto L_69c241;
    {
        u8 al = *(u8 *)(rec + 0x3d);
        if (!(al == 0 || al == 0x0b || al == 0x0c || al == 5 ||
              al == 8 || al == 0x0f || al == 7 || al == 6))
            goto L_69c241;
    }
    if (cm_fn_004cf6d0(G_1050, rec) != 0) goto L_69c241;
    ecx = SU32(0x12);
    rec = G_BASE + ecx * 0x6e;
    if (cm_fn_0069fc60(selfp, rec) != 0) goto L_69c241;
    ecx = SU32(0x12);
    rec = G_BASE + ecx * 0x6e;
    if (*(u32 *)(rec + 0x24) != 0 && *(u8 *)(rec + 0x28) == 5)
        goto L_69c241;

    eax = cm_fn_0053b900((u32 *)playerp);
    /* 0x69c250: ESI is still pushed, so [ESP+0x26] is B+0x22. */
    ecx = SU32(0x12);
    if (eax != 0) goto L_69c2c0;
    rec = G_BASE + ecx * 0x6e;
    eax = *(u32 *)(rec + 0x39);
    if (eax == 0) goto L_69c2c0;
    {
        u32 p53 = *(u32 *)(eax + 0x53);
        if (p53 != 0 && *(u8 *)(p53 + 0x11c) != 0) {
            u32 q = *(u32 *)(eax + 0x57);
            if (q != 0) {
                q = *(u32 *)q;
                if (*(u32 *)(G_ADADFC + q * 4) != 0)
                    goto L_69c2a9;
            }
        }
    }
    if ((s16)*(u16 *)(eax + 0x80) <= 0x1964) goto L_69c2c0;
L_69c2a9:
    if (*(u8 *)(rec + 0x3d) != 5) goto L_69c2c0;
    SU32(0x12) = 0xffffffff;
    goto L_69c12f;

L_69c241:
    SU32(0x12) = ebx;
    goto L_69c132;

L_69c2c0:
    SU32(0x1e) = 100;                    /* B+0x2e = 0x64 on this path */
    if (ecx == 0xffffffff) goto L_69c12f;
    ebx = 0;
    goto L_69bebb;

L_69c2d8:
    if (cm_fn_005409b0((u32 *)playerp) != 0)
        cm_fn_00944c9f(0x00de1f64u, 0x00a04d8cu, playerp + 4);
    else
        cm_fn_00944c9f(0x00de1f64u, 0x00a04d28u, playerp + 4);
    goto L_69c525;

    /* ---- record header reset; second (random) scan ---- */
L_69bebb:
    S8(0x0d) = 0;                        /* B+0x1d */
    S8(0x0c) = 0;                        /* B+0x1c */
    SU32(0x0e) = 0xffffffff;             /* B+0x1e */
    if (cm_fn_0053b900((u32 *)playerp) != 0) goto L_69bf0a;
    if ((s16)*(u16 *)(playerp + 0x80) <= 0x1194) goto L_69bfff;
    {
        unsigned long long r = cm_fn_0090cfc0(2000);
        edx_last = (u32)(r >> 32);
        eax = (u32)r;
        ecx = (u32)(s32)(s16)*(u16 *)(playerp + 0x80);
        edx = eax + 0x1194;
        if ((s32)ecx <= (s32)edx) goto L_69bfff;
    }

L_69bf0a:
    count = (u32)G_COUNT;
    eax = 0;
    ebx = 0xfff0bdc0;
    SU32(0x04) = 0;
    if ((s32)count <= 0) goto L_69bffd;
    slot54 = 0;
L_69bf27:
    count = (u32)G_COUNT;
    {
        unsigned long long r = cm_fn_0090cfc0(count);
        edx_last = (u32)(r >> 32);
        eax = (u32)r;
    }
    rec = G_BASE + eax * 0x6e;
    if (*(u32 *)(rec + 0x69) == 0) goto L_69bfdb;
    if (slot58 == rec) goto L_69bfdb;
    offv = slot54;
    if ((*(u8 *)(offv + G_D0C + 0x0b) & 0x30) != 0) goto L_69bfdb;
    if (!cm_fn_0069f810(selfp, rec, playerp, d315.d)) goto L_69bfdb;
    eax = *(u32 *)rec;
    if ((s32)eax >= (s32)((u32)G_COUNT - 0x10)) {
        if (*(u8 *)(rec + 0x5f) == 0) goto L_69bfdb;
    }
    eax = (u32)cm_fn_0054cdb0(0x00ae2c90u,
                              *(u32 *)(rec + 0x3e),
                              *(u32 *)(rec + 0x42));
    if ((s32)eax < 0x96) goto L_69bfdb;
    if (cm_fn_0069fc60(selfp, rec) != 0) goto L_69bfdb;
    eax = (u32)cm_fn_0069d1b0(self, (int *)rec, (int *)playerp, 0, 5);
    if ((s32)eax <= (s32)ebx) goto L_69bfdb;
    S8(0x0c) = 1;                        /* B+0x1c = 1 */
    SU32(0x0e) = *(u32 *)rec;            /* B+0x1e = *rec */
    ebx = eax;
L_69bfdb:
    {
        u32 i2 = SU32(0x04) + 1;
        u32 o2 = slot54 + 0x4f;
        u32 c2 = (u32)G_COUNT;
        SU32(0x04) = i2;
        slot54 = o2;
        if ((s32)i2 < (s32)c2) goto L_69bf27;
    }
L_69bffd:
    ebx = 0;

L_69bfff:
    {
        u32 c2 = (u32)G_COUNT;
        ecx = SU32(0x12);
        rec = G_BASE + ecx * 0x6e;
        if ((s32)*(u32 *)rec < (s32)(c2 - 0x10)) goto L_69c038;
        eax = slot58;
        if (eax == 0) goto L_69c038;
        if ((s32)*(u32 *)eax >= (s32)(c2 - 0x10)) goto L_69c038;
        cm_fn_00547e70(eax, playerp, 1);
    }

L_69c038:
    ecx = G_DATE_LO;
    eax = G_DATE_HI;
    SU32(0x04) = ecx;
    edx = (u16)(ecx >> 16);
    SU32(0x08) = eax;
    {
        u16 ax = G_A2C92W;
        if ((u16)edx > ax) goto L_69c077;
        if ((u16)edx < ax) goto L_69c321;
        eax = 0;
        if ((s16)(u16)ecx > (s16)G_A2C90W) eax = 1;
        if (eax == 0) goto L_69c321;
    }

L_69c077:
    if (cm_fn_005409b0((u32 *)playerp) != 0) goto L_69c321;
    {
        u16 ax0 = (u16)(G_DATE_LO >> 16);      /* [0xdd6eca] */
        SU16(0x2c) = ax0;
        {
            unsigned long long r = cm_fn_0090cfc0(7);
            edx_last = (u32)(r >> 32);
            eax = (u32)r + G_DATE_LO + 1;
            SU16(0x2a) = (u16)eax;
            if ((s16)(u16)eax > 0x16c) {
                unsigned long long r2 = cm_fn_0090cfc0(5);
                edx_last = (u32)(r2 >> 32);
                SU16(0x2c) = (u16)(SU16(0x2c) + 1);
                SU16(0x2a) = (u16)(u32)r2;
            }
        }
        {
            u16 dx2 = SU16(0x2c);
            SU16(0x24) = dx2;
            {
                unsigned long long r = cm_fn_0090cfc0(7);
                edx_last = (u32)(r >> 32);
                ecx = SU32(0x2a);
                edx = (u32)r + ecx + 1;
                SU16(0x22) = (u16)edx;
                {
                    unsigned long long r2 = cm_fn_0090cfc0(0xc8);
                    edx_last = (u32)(r2 >> 32);
                    if ((u32)r2 != 0) goto L_69c104;
                }
                {
                    unsigned long long r3 = cm_fn_0090cfc0(0x14);
                    edx_last = (u32)(r3 >> 32);
                    SU16(0x22) = (u16)(SU16(0x22) + (u16)(u32)r3);
                }
            }
        }
    }
L_69c104:
    if ((s16)SU16(0x22) <= 0x16c) goto L_69c454;
    goto L_69c43d;

L_69c321:
    {
        u16 ax0 = G_A2C92W;
        SU16(0x2c) = ax0;
        {
            unsigned long long r = cm_fn_0090cfc0(3);
            edx_last = (u32)(r >> 32);
            eax = (u32)r + G_A2C90D + 1;
            SU16(0x2a) = (u16)eax;
            if ((s16)(u16)eax > 0x16c) {
                unsigned long long r2 = cm_fn_0090cfc0(5);
                edx_last = (u32)(r2 >> 32);
                SU16(0x2c) = (u16)(SU16(0x2c) + 1);
                SU16(0x2a) = (u16)(u32)r2;
            }
        }
        ecx = SU32(0x12);
        SU16(0x24) = SU16(0x2c);
        rec = G_BASE + ecx * 0x6e;
        eax = *(u32 *)(rec + 0x39);
        if (eax == 0) goto L_69c3cd;
        if (cm_fn_00600a50(eax, 0, 1, 0, 0) == 0) goto L_69c3cd;
        ecx = SU32(0x12);
        rec = G_BASE + ecx * 0x6e;
        if (*(u8 *)(rec + 0x3d) == 5) goto L_69c3cd;
        cm_fn_006a0540(selfp, (volatile u32 *)rec, (volatile u32 *)playerp);
        {
            unsigned long long r = cm_fn_0090cfc0(5);
            edx_last = (u32)(r >> 32);
            ecx = SU32(0x2a);
            edx = (u32)r + ecx + 0x0a;
            SU16(0x22) = (u16)edx;
        }
    }
L_69c3cd:
    eax = slot58;
    if (eax == 0) goto L_69c3fd;
    {
        u32 c2 = (u32)G_COUNT;
        if ((s32)*(u32 *)eax < (s32)(c2 - 0x10)) goto L_69c3fd;
    }
    {
        unsigned long long r = cm_fn_0090cfc0(5);
        edx_last = (u32)(r >> 32);
        ecx = SU32(0x2a);
        edx = (u32)r + ecx + 7;
        SU16(0x22) = (u16)edx;
    }
    goto L_69c414;

L_69c3fd:
    {
        unsigned long long r = cm_fn_0090cfc0(5);
        edx_last = (u32)(r >> 32);
        ecx = SU32(0x2a);
        edx = (u32)r + ecx + 4;
        SU16(0x22) = (u16)edx;
    }
L_69c414:
    {
        unsigned long long r = cm_fn_0090cfc0(0x1f4);
        edx_last = (u32)(r >> 32);
        if ((u32)r != 0) goto L_69c434;
    }
    {
        unsigned long long r = cm_fn_0090cfc0(0x14);
        edx_last = (u32)(r >> 32);
        SU16(0x22) = (u16)(SU16(0x22) + (u16)(u32)r);
    }
L_69c434:
    if ((s16)SU16(0x22) <= 0x16c) goto L_69c454;

L_69c43d:
    {
        unsigned long long r = cm_fn_0090cfc0(5);
        edx_last = (u32)(r >> 32);
        eax = (u32)r + 4;
        SU16(0x24) = (u16)(SU16(0x24) + 1);
        SU16(0x22) = (u16)eax;
    }

    /* ---- hand the 0x26-byte record to cm_fn_0068d0d0 ---- */
L_69c454:
    {
        u32 self8 = *(volatile u32 *)(selfp + 8);
        u32 saved = *(volatile u32 *)(self8 + 0x0a);
        cm_fn_0068d0d0((volatile u8 *)self8, edx_last,
                       (const u8 *)(S + 0x0c), 0x26);
        self8 = *(volatile u32 *)(selfp + 8);
        *(volatile u32 *)(self8 + 0x0a) = saved;
    }
    if (cm_fn_0053ca90((volatile u8 *)playerp) != 0) goto L_69c525;
    if (cm_fn_00544820((u32 *)playerp) == 0) goto L_69c525;
    if (cm_fn_0053b900((u32 *)playerp) != 0) goto L_69c525;

    count = (u32)G_COUNT;
    ebx = count - 0x10;
    if ((s32)ebx >= (s32)count) goto L_69c525;
    slot58 = ebx * 0x6e;

L_69c4c0:
    ebp = slot58 + G_BASE;
    if (*(u8 *)(ebp + 0x5f) == 0) goto L_69c510;
    if ((u32)cm_fn_005fd210((void *)G_B63C98, ebp) == 0) goto L_69c510;
    if (cm_fn_005fea50(G_B63C98, 1, ebp) == 0) goto L_69c510;
    eax = *(u32 *)(ebp + 0x39);
    if (eax != 0) {
        if ((s16)*(u16 *)(eax + 0x80) >= (s16)*(u16 *)(playerp + 0x80))
            goto L_69c510;
    }
    cm_fn_0069ca00(selfp, playerp, ebp);

L_69c510:
    {
        u32 o2 = slot58 + 0x6e;
        u32 c2 = (u32)G_COUNT;
        ebx = ebx + 1;
        slot58 = o2;
        if ((s32)ebx < (s32)c2) goto L_69c4c0;
    }

L_69c525:
    CM_FS(void *, 0) = frame.prev;
    return;
}
