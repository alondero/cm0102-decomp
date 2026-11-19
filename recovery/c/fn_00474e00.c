#include "cmre.h"

/* Callees: signatures derived from this function's own call sites (ECX setup and
   pushes) at preferred-image VAs in the pinned 3.9.68 image. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_STDCALL cm_fn_00775790(u32 input);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);
extern u8 CM_STDCALL cm_fn_0085a760(int *param_1, int *param_2);
extern u32 CM_STDCALL cm_fn_0085add0(u32 param_1, u32 *param_2);
extern u32 CM_THISCALL cm_fn_008968f0(u32 self, u32 input, u32 index, u32 third);
extern u32 CM_THISCALL cm_fn_00896e90(u32 self, u32 record, volatile s8 second, volatile u32 third);

#define G_INT_AE2364 CM_GLOBAL(int, 0x00ae2364)
#define G_INT_B74318 CM_GLOBAL(int, 0x00b74318)
#define G_U32_DE1D08 CM_GLOBAL(u32, 0x00de1d08)
#define G_U32_DD7EF0 CM_GLOBAL(u32, 0x00dd7ef0)

u32 cm_fn_00474e00(u32 param_1)
{
    u32 v;        /* esi: *(param_1 + 0xe6) */
    u32 s;        /* eax: *(param_1 + 0xe2) */
    u32 ebx;      /* ebx: *s */
    u8 cl;
    u32 r;
    u32 home;     /* dword at caller-owned argument home [esp+0x14] */
    u32 ebp;
    u32 edi;
    u32 edx;
    u32 eax;
    u32 counter;
    u32 limit;
    s32 edx_s;
    u32 t;
    u32 off;
    u32 tv;

    /* 00474e07 / 00474e0d / 00474e14 */
    v = *(u32 *)(u32)(param_1 + 0xe6u);
    s = *(u32 *)(u32)(param_1 + 0xe2u);
    ebx = *(u32 *)s;

    /* 00474e16 .. 00474e29 */
    if (v == *(u32 *)(ebx + 0x1cu))
        cl = *(u8 *)(ebx + 0x40u);
    else
        cl = *(u8 *)(ebx + 0x41u);

    /* sete al; test eax,eax; jne 0x474ef0  (cl == 0 -> return 0) */
    if (cl == 0)
        return 0;

    /* 00474e34 .. 00474e43 : push 0; push esi; call 0085add0 */
    r = cm_fn_0085add0(v, (u32 *)0);
    if (r == 0)
        return 0;

    /* 00474e49 .. 00474e59 */
    cl = *(u8 *)(ebx + 0x4du);
    home = param_1 & 0xffffff00u;          /* mov byte [esp+0x14],0 */
    limit = ((u32)(cl & 0x0fu)) + 0x0bu;
    if ((s32)limit <= 0)
        return 0;

loop:
    /* 00474e5f */
    ebp = home;                            /* mov ebp,[esp+0x14] */

    /* 00474e63 .. 00474e76 */
    r = cm_fn_008968f0(G_U32_DE1D08, v, ebp, 0);
    edi = r;
    if (edi == 0)
        goto inc;

    /* 00474e78 .. 00474e81 */
    edx = *(u32 *)edi;
    if ((s32)edx >= (s32)G_INT_AE2364)
        goto inc;

    /* 00474e83 .. 00474e93 */
    r = cm_fn_0053b900((u32 *)v);
    if (r != 0) {
        eax = *(u32 *)(edi + 0x24u);
        goto cmp_esi;
    }

    /* 00474e95 .. 00474ea9 */
    if (G_INT_B74318 == 0)
        goto call_85a760;
    r = cm_fn_00775790(v);
    if (r != 0)
        goto call_85a760;
    eax = *(u32 *)(edi + 0x39u);

cmp_esi:
    /* 00474eac .. 00474eae */
    if (eax == v)
        goto call_85a760;

    /* 00474eb0 .. 00474ebf */
    (void)cm_fn_00896e90(G_U32_DE1D08, v, (s8)ebp, 0u);
    goto inc;

call_85a760:
    /* 00474ec1 .. 00474ed0 : push esi; push edi; call 0085a760; test al,al; jge */
    {
        u8 a = cm_fn_0085a760((int *)edi, (int *)v);
        if ((a & 0x80u) == 0)
            goto success;
    }

inc:
    /* 00474ed2 .. 00474eea */
    counter = (u8)(home & 0xffu) + 1u;     /* mov al,[esp+0x14]; inc al */
    cl = *(u8 *)(ebx + 0x4du);
    edx_s = (s8)counter;                   /* movsx edx,al */
    limit = ((u32)(cl & 0x0fu)) + 0x0bu;
    home = (home & 0xffffff00u) | (counter & 0xffu);  /* mov byte [esp+0x14],al */
    if (edx_s < (s32)limit)
        goto loop;
    return 0;

success:
    /* 00474ef7 .. 00474f16 */
    r = cm_fn_007fe600(0x00b74450u, 0x00475520u, 0x0047a9e0u, 1u, 0x0047c070u, 0u);
    if (r == 0)
        return 0;

    /* 00474f18 .. 00474f35 */
    t = (u32)(s32)(s16)CM_GLOBAL(s16, 0x00b77c7a);   /* movsx eax,word */
    off = (t + t * 2u) << 8;                         /* lea eax,[eax+eax*2]; shl eax,8 */
    tv = *(u32 *)(0x00b74c26u + off);                /* mov ecx,[eax+0xb74c26] */
    cm_fn_007ff270((int)0x00b74450u, 0, (char *)(void *)(unsigned long)tv, 0);

    /* 00474f3a .. 00475054 : 18 further rows, self in ECX = 0x00b74450 */
    cm_fn_007ff270((int)0x00b74450u, 1,  (char *)-1, 0);
    cm_fn_007ff270((int)0x00b74450u, 2,  (char *)1, 0);
    cm_fn_007ff270((int)0x00b74450u, 3,  (char *)1, 0);
    cm_fn_007ff270((int)0x00b74450u, 4,  (char *)0x41, 0);
    cm_fn_007ff270((int)0x00b74450u, 5,  (char *)8, 0);
    cm_fn_007ff270((int)0x00b74450u, 6,  (char *)6, 0);
    cm_fn_007ff270((int)0x00b74450u, 7,  (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 8,  (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 9,  (char *)-1, 0);
    cm_fn_007ff270((int)0x00b74450u, 10, (char *)-1, 0);
    cm_fn_007ff270((int)0x00b74450u, 11, (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 12, (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 13, (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 14, (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 15, (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 16, (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 17, (char *)0, 0);
    cm_fn_007ff270((int)0x00b74450u, 18, (char *)(void *)(unsigned long)ebx, 0);

    return 1;
}
