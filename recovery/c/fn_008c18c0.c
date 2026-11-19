/* Reconstruction of preferred-image VA 0x008c18c0 (image base 0x00400000). */
#include "cmre.h"

#define A32(p) ((u32)(p))

/* Callees: signatures derived from the original instructions at the listed VAs. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern void CM_CDECL cm_fn_00547af0(u32 object, u32 value, u32 selector);
extern u32 CM_CDECL cm_fn_00547d60(u32 object, u32 target);
extern void CM_CDECL cm_fn_00547e70(u32 object, u32 value, u32 selector);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d080(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_00696940(u32 self, u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_00697430(u32 self, u32 person, u32 club);
extern u32 CM_STDCALL cm_fn_006a0990(u32 first, u32 second, u32 third);
/* 008c1996 loads ECX=[0xb7431c] before push edi/call 7875e0: thiscall receiver. */
extern u32 CM_THISCALL cm_fn_007875e0(u32 self, u32 club);
extern u32 CM_THISCALL cm_fn_0086d800(u32 self, u32 entry);
extern u32 CM_THISCALL cm_fn_008c1af0(u32 self, volatile u32 person_arg,
        volatile u32 club_arg, volatile u32 option_arg, volatile u32 fee_arg,
        volatile u32 mode_arg);
extern void CM_THISCALL cm_fn_008df9b0(int *param_1, u32 *param_2);
/* 008c1a11/008c1a17 push one stack arg per call, add esp,8 for both (cdecl). */
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 arg);

#define ExceptionList CM_FS(void *, 0)
#define DAT_00ae1050 CM_GLOBAL(u32, 0x00ae1050)
#define DAT_00ae1080 CM_GLOBAL(u32, 0x00ae1080)
#define DAT_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(u32, 0x00ae23bc)
#define DAT_00b7431c CM_GLOBAL(u32, 0x00b7431c)
#define DAT_00b67a44 CM_GLOBAL(u32, 0x00b67a44)
#define DAT_00dd7f00 CM_GLOBAL(u32, 0x00dd7f00)
#define LAB_00963bb0 CM_GLOBAL(u8, 0x00963bb0)

#define B8(p, off)  (*(volatile u8  *)((volatile u8 *)(p) + (off)))
#define B16(p, off) (*(volatile u16 *)((volatile u8 *)(p) + (off)))
#define B32(p, off) (*(volatile u32 *)((volatile u8 *)(p) + (off)))

struct cm_seh_frame {
    void *prev;
    u32 handler;
    u32 state;
};

void CM_THISCALL cm_fn_008c18c0(u32 self)
{
    volatile u8 L[0x28];               /* L = ESP after sub esp,0x28 */
    volatile struct cm_seh_frame frame;
    u32 obj1, obj2;
    u32 esi_val, iVar1, iVar6, ebp, idx, tmp, opt, fee;
    unsigned long long r1, r2, rr;

    /* SEH prologue: push state(-1), handler, prev; link frame (008c18c0..ce). */
    frame.state = 0xffffffffu;
    frame.handler = A32(&LAB_00963bb0);
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    obj1 = A32(L + 8);                 /* lea ecx,[esp+0x14] = L+8 */
    B32(L, 0) = self;                  /* mov [esp+0xc],ebx (save this) */
    (void)cm_fn_0068d020(obj1);        /* call 68d020 (obj1) */

    obj2 = A32(L + 24);                /* lea ecx,[esp+0x24] = L+24 */
    frame.state = 0;                   /* mov dword [esp+0x3c],0 */
    (void)cm_fn_0068d020(obj2);        /* call 68d020 (obj2) */

    esi_val = self + 0x92cu;           /* lea esi,[ebx+0x92c] */
    B8(&frame.state, 0) = 1;           /* mov byte [esp+0x3c],1 -> state.low */
    B32(L, 4) = esi_val;               /* mov [esp+0x10],esi */

    /* copy record from [esi] into the obj1 buffer, exact store order. */
    B32(L, 8)  = B32(esi_val, 0);      /* mov ecx,[eax]; mov [esp+0x14],ecx */
    B32(L, 12) = B32(esi_val, 4);      /* mov edx,[eax+4]; mov [esp+0x18],edx */
    B32(L, 16) = B32(esi_val, 8);      /* mov ecx,[eax+8]; mov [esp+0x1c],ecx */
    B16(L, 20) = B16(esi_val, 0xc);    /* mov dx,[eax+0xc]; mov word [esp+0x20],dx */
    tmp = B32(L, 10);                  /* mov eax,[esp+0x16] */
    B32(L, 18) = tmp;                  /* mov [esp+0x1e],eax (L+18 = obj1+0xa) */

    ebp = cm_fn_0068d1b0(obj1);        /* call 68d1b0 (obj1) -> ebp */

    while (ebp != 0) {                 /* test ebp,ebp; je 8c1a8b */
        idx = B32(ebp, 0);             /* mov eax,[ebp] */
        iVar1 = DAT_00ae23bc + idx * 0x6eu;   /* lea esi,[eax+edx*2] */
        DAT_00ae1080 = cm_fn_004cf5a0(DAT_00ae1050, iVar1);
        if (DAT_00ae1080 == 0) goto loop_continue;   /* je 8c1a73 */
        iVar6 = DAT_00ae23b4 + B32(DAT_00ae1080, 4) * 0x245u;
        if (iVar6 == 0) goto loop_continue;          /* je 8c1a73 */

        if (cm_fn_007875e0(DAT_00b7431c, iVar6) == 0)
            goto lbl_8c1a06;           /* test eax,eax; je 8c1a06 */

        if (B8(ebp, 4) == 0) {         /* mov al,[ebp+4]; test; jne 8c1a4b */
            rr = cm_fn_00696940(DAT_00b67a44, iVar1, iVar6);
            if ((u16)rr != 0) {        /* test ax,ax; je 8c19d0 */
                (void)cm_fn_006a0990(iVar1, iVar6, (u32)rr);
                goto loop_continue;    /* jmp 8c1a73 */
            }
            if (B8(ebp, 4) == 0) {
                (void)cm_fn_00547e70(iVar1, iVar6, 0);
                tmp = B32(iVar6, 0xcf); /* 008c19e0: load once for test and push */
                if (tmp != 0)
                    (void)cm_fn_00547af0(iVar1, tmp, 1);
            }
            (void)cm_fn_00697430(DAT_00b67a44, iVar1, iVar6);
            goto lbl_8c1a06;
        }
        goto final_section;            /* jne 8c1a4b */

lbl_8c1a06:
        if (B8(ebp, 4) == 0) {
            r1 = cm_fn_0090cfc0((u32)(s32)(s8)B8(iVar1, 0x5b));
            tmp = (u32)r1;             /* mov ebx,eax */
            r2 = cm_fn_0090cfc0(0x14u);
            if ((s32)tmp < (s32)(u32)r2) {          /* cmp ebx,eax; jl 8c1a35 */
                (void)cm_fn_00547af0(iVar1, B32(iVar6, 0xcf), 1);
            } else if (cm_fn_00547d60(iVar1, iVar6) != 0) {
                (void)cm_fn_00547af0(iVar1, B32(iVar6, 0xcf), 1);
            }
        }

final_section:
        (void)cm_fn_0086d800(DAT_00dd7f00, iVar1);
        (void)cm_fn_008df9b0((int *)self, (u32 *)iVar1);
        opt = (u32)(s32)(s8)B8(ebp, 4);            /* movsx eax,byte [ebp+4] */
        fee = B32(ebp, 5);                         /* mov edx,[ebp+5] */
        (void)cm_fn_008c1af0(self, iVar1, iVar6, opt, fee, 0);

loop_continue:
        ebp = cm_fn_0068d1b0(obj1);     /* lea ecx,[esp+0x18]=obj1; call 68d1b0 */
    }

    esi_val = B32(L, 4);               /* mov esi,[esp+0x14] (saved orig esi) */

    /* copy obj2 record into obj1 buffer (008c1a8b..ab). */
    B32(L, 8)  = B32(L, 24);
    B32(L, 12) = B32(L, 28);
    B32(L, 16) = B32(L, 32);
    B16(L, 20) = B16(L, 36);

    (void)cm_fn_0068d080(esi_val);     /* mov ecx,esi; call 68d080 */
    B8(&frame.state, 0) = 0;           /* mov byte [esp+0x3c],0 -> state.low */
    (void)cm_fn_0068d040(obj2);        /* lea ecx,[esp+0x24]; call 68d040 */
    frame.state = 0xffffffffu;         /* mov dword [esp+0x3c],-1 */
    (void)cm_fn_0068d040(obj1);        /* lea ecx,[esp+0x14]; call 68d040 */
    CM_FS(void *, 0) = frame.prev;     /* mov fs:[0],ecx (saved prev); unlink */
}
