/* Recovered from original instructions at preferred-image VA 0x0076db00
   (image base 0x00400000, cm0102.exe 3.9.68, national_teams.cpp).
   Plain C translation; every callee stays an explicit original boundary.
   Frame offsets (L = esp after `sub esp, 0x39c`):
     L+0x00 scratch: byte3 flag, dword4 date, word0xc type
     L+0x10 out flag   L+0x14 national-team value
     L+0x18 iterator context (16 bytes)   L+0x28 72-byte text buffer
     L+0x70 256-byte text   L+0x170 256-byte text   L+0x270 300-byte text */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_CDECL cm_fn_0053b4f0(u32 nation);
extern u32 CM_CDECL cm_fn_0053c590(u32 club, u8 mode, u32 filter);
extern u32 CM_CDECL cm_fn_0053dba0(u32 person, u32 output, u32 capacity, u32 selector);
extern u32 CM_CDECL cm_fn_00542100(u32 object, u32 suffix, u32 output, s32 size, s8 mode,
                                   u32 brackets);
extern u32 CM_CDECL cm_fn_00547d30(u32 object, u32 target);
extern u16 CM_THISCALL cm_fn_005acee0(u32 self,
    u32, u32, u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_005e8290(const char *a1, const char *a2, int a3);
extern u32 CM_THISCALL cm_fn_005fd6b0(u32 self, u32 person, u32 output);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person,
                                   u32 force);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern char *CM_CDECL cm_fn_0066f4e0(u32 dest, const char *fmt, ...);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 a1, u32 a2, u32 a3, void *a4, void *a5);

u32 CM_THISCALL cm_fn_0076db00(u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5,
                               u32 a6)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 L[0x39c];
    u32 out_flag, local_394, obj, cax, cglo, rec, rp;
    u8 m, mode;
    s32 ms;
    u16 sr;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095dddbu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    if (a1 == 0) {
        cm_fn_00944cff(0x00a5b2a4u, 0, 0, (void *)(L + 0x170), (void *)(L + 0x70));
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, cm_fn_0090d130(),
                       (u32)(L + 0x170), (u32)(L + 0x70), 0x9ce);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        D(0x00b67a34) = 0;
        goto ret1;
    }
    if (a5 != 0 && (s32)a6 < 0x32) {
        cm_fn_00944cff(0x00a5b2a4u, 0, 0, (void *)(L + 0x70), (void *)(L + 0x170));
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, cm_fn_0090d130(),
                       (u32)(L + 0x70), (u32)(L + 0x170), 0x9d8);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        D(0x00b67a34) = 0;
        goto ret1;
    }

    if (a3 != 0) {
        u32 nat, player, num;
        s32 dig;

        nat = cm_fn_0053b3d0(D(a3 + 0x1a));
        if (cm_fn_00547d30(a3, nat) != 0) {
            if (a5 == 0) goto ret0;
            cm_fn_0053dba0(a3, (u32)(L + 0x270), 0x12c, 1);
            cm_fn_0066f4e0(a5, (const char *)0x00a5b4c8u, (u32)(L + 0x270));
            goto ret0;
        }
        player = D(a3 + 0x24);
        if (player != 0 && D(player + 0x53) != a1) {
            if (a5 == 0) goto ret0;
            cm_fn_0053dba0(a3, (u32)(L + 0x270), 0x12c, 1);
            num = D(D(a3 + 0x24) + 0x53);
            dig = (s32)*(volatile s8 *)(num + 0x52);
            cm_fn_0066f4e0(a5, (const char *)0x00a5b460u, dig, dig,
                           (u32)(L + 0x270), num + 0x38);
            goto ret0;
        }
    }

    m = (u8)a2;
    ms = (s32)(s8)m;
    if (m == 0) {
        obj = cm_fn_0053b3d0(a1);
        out_flag = 0;
        mode = 4;
    } else if (m == 1) {
        obj = cm_fn_0053b3d0(a1);
        out_flag = 1;
        mode = 8;
    } else {
        obj = cm_fn_0053b4f0(a1);
        out_flag = 0;
        mode = 4;
    }
    L[0xc] = mode;

    {
        u32 nat = cm_fn_0053b3d0(a1);

        local_394 = D(nat + 0xcf);
    }
    if (cm_fn_00600a50(cm_fn_0053b3d0(a1), 0, 1, 0, 0) == 0) {
        local_394 = 0;
    }
    {
        u32 idx = D(a1);

        rec = D(self + 4) + idx * 110u;
    }

    if (a4 != 0) {
        u32 cap = rec + (u32)ms * 20u + 0x42;
        u8 count = (u8)cm_fn_0053c590(obj, mode, 0);

        if ((s8)count >= (s8)*(volatile u8 *)cap) {
            s32 dig, lim;

            if (a5 == 0) goto ret0;
            cm_fn_00542100(obj, out_flag, (u32)(L + 0x28), 0x47, 2, 1);
            dig = (s32)*(volatile s8 *)(obj + 0x52);
            lim = (s32)*(volatile s8 *)cap;
            cm_fn_0066f4e0(a5, (const char *)0x00a5b3e0u, dig, dig, lim,
                           (u32)(L + 0x28));
            goto ret0;
        }
        if (cm_fn_0062c400(D(0x00b63d28), a3) == 2) {
            if (a5 == 0) goto ret0;
            cm_fn_0066f4e0(a5, (const char *)0x00a5b3bcu);
            goto ret0;
        }
    }

    rp = rec + (u32)ms * 20u;
    if (*(volatile u8 *)(rp + 0x45) != 0 && a4 != 0) goto ret1;
    if (*(volatile u8 *)(rp + 0x44) == 0) goto ret1;

    if (m != 0 && local_394 != 0) {
        u32 head = D(self);

        if (head != 0) {
            u32 it;

            D(head + 0xa) = D(head + 2);
            it = cm_fn_0068d1b0(D(self));
            while (it != 0) {
                if (D(it) == D(obj)) {
                    u32 lo = D(it + 8);
                    u32 hi = D(it + 0xc);
                    u16 gh, lh;

                    *(volatile u32 *)(L + 4) = lo;
                    *(volatile u32 *)(L + 8) = hi;
                    gh = W(0x00ae2c92);
                    lh = (u16)(lo >> 16);
                    if (lh < gh) {
                        cax = gh;
                        cglo = D(0x00ae2c90);
                        goto l_date;
                    }
                    if (lh == gh) {
                        u32 low = D(0x00ae2c90);

                        if ((s16)lo < (s16)low) {
                            cax = gh;
                            cglo = low;
                            goto l_date;
                        }
                    }
                }
                it = cm_fn_0068d1b0(D(self));
            }
        }
        if (cm_fn_005fd6b0(D(0x00b63c98), local_394, 0) == 0) goto ret1;
    }
    cax = W(0x00ae2c92);
    cglo = D(0x00ae2c90);

l_date:
    {
        u32 lo = D(rp + 0x3a);
        u32 hi = D(rp + 0x3e);

        *(volatile u32 *)(L + 4) = lo;
        *(volatile u32 *)(L + 8) = hi;
        if ((u16)(lo >> 16) < cax) goto ret1;
        if ((u16)(lo >> 16) == cax && (s16)lo < (s16)cglo) goto ret1;

        lo = D(rp + 0x3a);
        hi = D(rp + 0x3e);
        *(volatile u32 *)(L + 4) = lo;
        *(volatile u32 *)(L + 8) = hi;
        if ((u16)(lo >> 16) != cax) goto l_msg;
        if ((u16)lo != (u16)cglo) goto l_msg;

        *(volatile u32 *)(L + 4) = cglo;
        *(volatile u16 *)(L + 0xc) = cax;
        L[3] = 0;
    }
    cm_fn_0068d020((u32)(L + 0x18));
    frame.state = 0;
    sr = cm_fn_005acee0(D(0x00ae2a58),
                        (u32)(L + 0xc), (u32)(L + 4), (u32)(L + 3),
                        1, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                        D(obj), 0xffffffffu, 0, 0, 0, 1, (u32)(L + 0x18), 1,
                        0xfffffffeu,
                        (out_flag & 0xffffff00u) | (u32)(out_flag == 0), 0);
    frame.state = 0xffffffffu;
    if (sr != 0) {
        cm_fn_0068d040((u32)(L + 0x18));
        goto ret1;
    }
    cm_fn_0068d040((u32)(L + 0x18));

l_msg:
    if (a5 != 0) {
        s32 dig;

        cm_fn_00542100(obj, out_flag, (u32)(L + 0x28), 0x47, 2, 1);
        dig = (s32)*(volatile s8 *)(obj + 0x52);
        if (a4 != 0) {
            cm_fn_0066f4e0(a5, (const char *)0x00a5b34cu, dig, dig,
                           (u32)(L + 0x28));
        } else {
            cm_fn_0066f4e0(a5, (const char *)0x00a5b2d8u, dig, dig,
                           (u32)(L + 0x28));
        }
    }

ret0:
    CM_FS(u32, 0) = frame.prev;
    return 0;

ret1:
    CM_FS(u32, 0) = frame.prev;
    return 1;
}
