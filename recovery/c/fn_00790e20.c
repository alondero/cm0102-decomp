#include "cmre.h"

#define DAT_009cf6bc CM_GLOBAL(int, 0x009cf6bc)
#define DAT_00adadfc CM_GLOBAL(int, 0x00adadfc)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_THISCALL cm_fn_0051c800(u32 self, volatile u32 round_arg);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs, s16 rounds, u32 competition, u32 schedule, u16 year, u8 flag_a, u8 flag_b, u16 round_flag, u8 flag_c, u32 builder, u32 numbered, u32 value_38, u32 club_flags);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

typedef u32 (CM_THISCALL *cm_vtable_fn)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);

void CM_FASTCALL cm_fn_00790e20(int *param_1)
{
    /* SEH frame: push -1 (state), push 0x95e5ee (handler), push prev, then link.
       At the call site the frame is at L+0x40c: prev, handler, state. */
    volatile struct {
        u32 prev;
        u32 handler;
        u32 state;
    } seh;

    /* Scratch region L+0..L+0x40b. local_4/local_8 are out-parameters of the
       vtable call at 0x00790f7e; bufA..bufD are the four 0x100 buffers. */
    struct {
        u32 local_0;
        u32 local_4;
        u32 local_8;
        u8 bufA[0x100];
        u8 bufB[0x100];
        u8 bufC[0x100];
        u8 bufD[0x100];
    } s;

    u32 *alloc_a;
    u32 *alloc_b;
    u32 i;
    u32 j;
    u32 found;
    u32 indirect_result;
    u32 uVar5;
    u32 *err_p4;
    u32 *err_p5;
    u32 err_code;
    u32 holder;
    u32 year_val;

    seh.state = 0xffffffffu;
    seh.handler = 0x0095e5eeu;
    seh.prev = (u32)CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    alloc_a = (u32 *)(u32)cm_fn_00944e46(8);
    i = 0;

    if (alloc_a == (u32 *)0) {
        cm_fn_00944cff(0x00a69740u, 0, 0, (void *)&s.bufB, (void *)&s.bufA);
        err_p4 = (u32 *)&s.bufB;
        err_p5 = (u32 *)&s.bufA;
        err_code = 0x326u;
        goto report_error;
    }

    found = 0;
    i = 0;
    if ((s16)(*(volatile u16 *)((char *)param_1 + 0x3e)) > 0) {
        u32 offset = 0;
        do {
            u32 base = *(volatile u32 *)((char *)param_1 + 0xb1);
            u8 *p = (u8 *)(base + offset);
            if (*(volatile u8 *)(p + 0x37) == 1) {
                found = 1;
                *(volatile u32 *)alloc_a = *(u32 *)p;
            }
            i++;
            offset += 0x3bu;
        } while ((s32)i < (s32)(s16)(*(volatile u16 *)((char *)param_1 + 0x3e)));

        if (found != 0) {
            u32 comp;

            s.local_0 = 0;              /* 0x00790f22: mov [esp+0x10],edi (edi = 0) */
            comp = *(u32 *)((u32)DAT_00adadfc + (u32)DAT_009cf6bc * 4u);
            if ((s16)(*(volatile u16 *)((char *)comp + 0x3e)) <= 0)
                goto restore_and_return;

            j = 0;
            do {
                u8 *p = (u8 *)cm_fn_006826d0((void *)comp, j);
                if (*(volatile u8 *)(p + 0x37) == 2) {
                    u8 *q = (u8 *)cm_fn_006826d0((void *)comp, j);
                    u32 v = *(u32 *)q;
                    s.local_0 = 1;
                    *(volatile u32 *)((char *)alloc_a + 4) = v;
                }
                j++;
            } while ((s32)j < (s32)(s16)(*(volatile u16 *)((char *)comp + 0x3e)));
            if (s.local_0 == 0)
                goto restore_and_return;

            {
                u32 vt = *(u32 *)param_1;
                cm_vtable_fn fn = (cm_vtable_fn)(*(u32 *)(vt + 0x3cu));
                indirect_result = fn((u32)param_1, 0, (u32)&s.local_8, (u32)&s.local_4, 0);
            }

            {
                unsigned long long r = cm_fn_00944cf1(0xb2u);
                alloc_b = (u32 *)(u32)r;
                s.local_0 = (u32)r;
                seh.state = 0;
                if ((u32)r == 0) {
                    uVar5 = 0;
                    goto after_522e;
                }
            }

            /* mov dx,[esi+0x40] keeps the upper 16 bits of EDX from the
               preceding calls; the pushed u16 year only consumes the low half. */
            year_val = (((u32)&s.local_8) & 0xffff0000u) |
                       (u16)(*(volatile u16 *)((char *)param_1 + 0x40));
            uVar5 = cm_fn_00522e00((u32)alloc_b, (u32)param_1, 2, (u32)alloc_a,
                                   (u16)s.local_8, *(u32 *)((char *)param_1 + 4),
                                   indirect_result, (u16)year_val, 0, 1,
                                   (u16)s.local_4, 0x14, 0, 0, 0, 0);

after_522e:
            holder = *(volatile u32 *)((char *)param_1 + 0xc);
            seh.state = 0xffffffffu;
            *(volatile u32 *)holder = uVar5;
            cm_fn_009452ca((u32)alloc_a);
            cm_fn_009452ca(indirect_result);
            holder = *(volatile u32 *)((char *)param_1 + 0xc);
            if (*(volatile u32 *)holder != 0) {
                /* ECX (self) for this call is residue from cm_fn_009452ca:
                   no listing VA assigns it before 0x00791058 (unknown boundary). */
                cm_fn_0051c800(0, 0);
                goto restore_and_return;
            }

            cm_fn_00944cff(0x00a69740u, 0, 0, (void *)&s.bufD, (void *)&s.bufC);
            err_p4 = (u32 *)&s.bufD;
            err_p5 = (u32 *)&s.bufC;
            err_code = 0x357u;
            goto report_error;
        }
    }

    cm_fn_00944cff(0x00a69740u, 0, 0, (void *)&s.bufA, (void *)&s.bufB);
    err_p4 = (u32 *)&s.bufA;
    err_p5 = (u32 *)&s.bufB;
    err_code = 0x336u;

report_error:
    {
        u32 r = (u32)cm_fn_0090d130();
        cm_fn_00944c9f((u32)&DAT_00ae24d0, 0x009870e8u, r,
                       (u32)err_p4, (u32)err_p5, err_code);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
    }

restore_and_return:
    CM_FS(void *, 0) = (void *)seh.prev;
}
