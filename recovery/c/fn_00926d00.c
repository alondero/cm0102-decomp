#include "cmre.h"

#define HANDLER_VA   0x00965dacu
#define ERR_PATH     0x00ad56f4u
#define ERR_FMT      0x009870e8u
#define ERR_NAME     0x009870e0u
#define ERRBUF_ADDR  0x00ae24d0u
#define DAT_ERRFLAG  CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_THISCALL cm_fn_0051c410(u32 self, u32 index);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, u32 count, u32 clubs, u32 rounds, u32 competition, u32 schedule, u32 year, u32 flag_a, u32 flag_b, u32 round_flag, u32 flag_c, u32 builder, u32 numbered, u32 value_38, u32 club_flags);
extern void cm_fn_005e8290(const char *a, const char *b, int c);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
extern u32 CM_THISCALL cm_fn_006827d0(int self, int arg);
extern u32 * CM_THISCALL cm_fn_0068b100(u32 *self, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7, u32 p8, u32 p9, u32 p10, u32 p11, u32 p12, u32 p13, u32 p14, u32 p15, u32 p16, u32 p17, u32 p18, u32 p19, u32 p20, u32 p21, u32 p22, u32 p23);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 a, u32 b, u32 c, void *d, void *e);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

typedef u32 (CM_THISCALL *Fn3c)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);
typedef void (CM_THISCALL *FnV0)(u32 self, u32 arg);

struct CmSeh {
    volatile u32 prev;
    volatile u32 handler;
    volatile u32 state;
};

void CM_FASTCALL cm_fn_00926d00(int *arg_this)
{
    u8 *const self = (u8 *)arg_this;
    struct CmSeh frame;
    u8 scratch[0x800];
    u32 P1;
    u32 P2;
    u32 P3;
    u32 target;
    u32 vcall_res;
    u32 sub_res;
    u32 P1_saved;
    u32 loop_i;
    s32 ci;
    u32 f40;
    u32 field4;
    u32 byte_c2;
    u32 byte_c3;
    u32 fc;

    /* 0x00926d00..0x00926d1d : SEH prologue, listing order */
    frame.state = 0xffffffffu;
    frame.handler = HANDLER_VA;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    /* 0x00926d1f..0x00926d31 */
    P1 = (u32)cm_fn_00944e46(0x20u);
    *(u32 *)(scratch + 0x14) = P1;
    if (P1 == 0u) {
        cm_fn_00944cff(ERR_PATH, 0u, 0u, (void *)(scratch + 0x218), (void *)(scratch + 0x18));
        {
            u32 r = (u32)cm_fn_0090d130();
            cm_fn_00944c9f(ERRBUF_ADDR, ERR_FMT, r, (u32)(scratch + 0x218), (u32)(scratch + 0x18), 0x27eu);
            cm_fn_005e8290((const char *)ERR_NAME, (const char *)ERRBUF_ADDR, 0);
        }
        DAT_ERRFLAG = 0u;
        CM_FS(u32, 0) = frame.prev;
        return;
    }

    /* 0x00926da6..0x00926dd0 : first copy loop */
    {
        u32 *dst = (u32 *)(P1 + 4u);
        u32 b1 = *(u32 *)(self + 0xb1);
        *((u32 *)P1) = *(u32 *)b1;
        for (loop_i = 0u; loop_i < 0x14u; loop_i += 4u) {
            u32 fc_i = *(u32 *)(self + 0xc);
            u32 *r = (u32 *)cm_fn_006826d0((void *)(*(u32 *)(fc_i + loop_i)), 0u);
            *dst = *r;
            dst++;
        }
    }

    /* 0x00926dd2..0x00926dec */
    P2 = (u32)cm_fn_00944cf1(0xeeu);
    *(u32 *)(scratch + 8) = P2;
    frame.state = 0u;
    if (P2 != 0u) {
        f40 = (u32)(*(u16 *)(self + 0x40));
        field4 = *(u32 *)(self + 4);
        byte_c2 = (u32)(*(u8 *)(self + 0xc2));
        byte_c3 = (u32)(*(u8 *)(self + 0xc3));
        target = (u32)cm_fn_0068b100(
            (u32 *)P2,
            (u32)self, 6u, 0u, 0u, field4, 0u, 0u,
            (((u32)self + 0xc5u) & 0xffffff00u) | byte_c2,
            (((u32)self + 0xbeu) & 0xffffff00u) | byte_c3,
            0u, (u32)self + 0xc5u, (u32)self + 0xbeu,
            f40, 0xfffffffeu, 0u, 0u, 1u, 0u, 2u, 0xffffffffu, 0u, 2u);
    } else {
        target = 0u;
    }

    /* 0x00926e32..0x00926ea6 : record copy loop */
    frame.state = 0xffffffffu;
    ci = -1;
    do {
        u32 src_b1;
        u32 edx1;
        u32 srcp;
        u32 dstp;
        s32 k;

        if (ci == -1) {
            src_b1 = *(u32 *)(self + 0xb1);
        } else {
            u32 fc_i = *(u32 *)(self + 0xc);
            u32 club = *(u32 *)(fc_i + (u32)ci * 4u);
            src_b1 = *(u32 *)(club + 0xb1);
        }
        edx1 = (u32)ci + 1u;
        srcp = src_b1 + 0x3bu;
        dstp = edx1 * 0x3bu + *(u32 *)(target + 0xb1);
        for (k = 0; k < 14; k++) {
            *(u32 *)dstp = *(u32 *)srcp;
            srcp += 4u;
            dstp += 4u;
        }
        *(u16 *)dstp = *(u16 *)srcp;
        srcp += 2u;
        dstp += 2u;
        *(u8 *)dstp = *(u8 *)srcp;
        *(u32 *)(*(u32 *)(target + 0xb1) + 0x44u + (u32)ci * 0x3bu) = 0u;
        *(u32 *)(*(u32 *)(target + 0xb1) + 0x40u + (u32)ci * 0x3bu) = 0u;
        ci = (s32)edx1;
    } while ((s32)ci < 5);

    /* 0x00926ea8..0x00926edd */
    (void)cm_fn_006827d0((int)target, 0);
    {
        u32 tb1 = *(u32 *)(target + 0xb1);
        P1_saved = *(u32 *)(scratch + 0x14);
        *(u32 *)(P1_saved + 0x18u) = *(u32 *)tb1;
        {
            u32 tb1b = *(u32 *)(target + 0xb1);
            *(u32 *)(P1_saved + 0x1cu) = *(u32 *)(tb1b + 0x3bu);
        }
    }
    {
        u32 vt = *(u32 *)self;
        Fn3c fn3c = (Fn3c)(*(u32 *)(vt + 0x3cu));
        vcall_res = fn3c((u32)self, 5u, (u32)(scratch + 0xc), (u32)(scratch + 0x10), 0u);
    }

    /* 0x00926ee0..0x00926f2f */
    P3 = (u32)cm_fn_00944cf1(0xb2u);
    *(u32 *)(scratch + 8) = P3;
    frame.state = 1u;
    sub_res = 0u;
    if (P3 != 0u) {
        u32 out2 = *(u32 *)(scratch + 0x10);
        u32 out1 = *(u32 *)(scratch + 0xc);
        f40 = (u32)(*(u16 *)(self + 0x40));
        field4 = *(u32 *)(self + 4);
        sub_res = cm_fn_00522e00(P3, (u32)self, 8u, P1_saved, out1,
                                 field4, vcall_res, f40, 5u, 1u,
                                 out2, 0x14u, 0u, 0u, 0u, 0u);
    }

    /* 0x00926f33..0x00926f52 */
    frame.state = 0xffffffffu;
    fc = *(u32 *)(self + 0xc);
    *(u32 *)(fc + 0x14u) = sub_res;
    (void)cm_fn_009452ca(P1_saved);
    (void)cm_fn_009452ca(vcall_res);

    if (target != 0u) {
        u32 vt = *(u32 *)target;
        FnV0 fnv0 = (FnV0)(*(u32 *)vt);
        fnv0(target, 1u);
    }

    fc = *(u32 *)(self + 0xc);
    if (*(u32 *)(fc + 0x14u) == 0u) {
        cm_fn_00944cff(ERR_PATH, 0u, 0u, (void *)(scratch + 0x318), (void *)(scratch + 0x118));
        {
            u32 r = (u32)cm_fn_0090d130();
            cm_fn_00944c9f(ERRBUF_ADDR, ERR_FMT, r, (u32)(scratch + 0x318), (u32)(scratch + 0x118), 0x2b2u);
            cm_fn_005e8290((const char *)ERR_NAME, (const char *)ERRBUF_ADDR, 0);
        }
        DAT_ERRFLAG = 0u;
        CM_FS(u32, 0) = frame.prev;
        return;
    }

    /* 0x00926ff0 : receiver is ECX = *(field_c + 0x14) from 0x00926f65 */
    fc = *(u32 *)(self + 0xc);
    cm_fn_0051c410(*(u32 *)(fc + 0x14u), 0u);
    CM_FS(u32, 0) = frame.prev;
    return;
}
