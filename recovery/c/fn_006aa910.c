#include "cmre.h"

/* Original routine 0x006aa910 (preferred-image VA, image base 0x00400000).
   Convention: ret 0xc, so stdcall with three stack arguments:
     arg1 = object pointer (ECX for the thiscall helpers, [entry+4]),
     arg2 = branch selector (zero takes branch A, [entry+8]),
     arg3 = object for the final thiscall calls ([entry+0xc]).
   The listing's frame scratch at 0x78 bytes (lea [esp+0x14] / [esp+0x18] at
   the frame base) is one local array passed to 0x006181d0 and 0x0066f4e0.
   Returned EAX is the low dword of the last callee's return value.
   Non-stack memory reads are volatile so -O2 keeps the listing's order. */

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern void CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);
extern int cm_fn_0053b3d0(int *param_1);

CM_STDCALL u32 cm_fn_006aa910(u32 arg1, u32 arg2, u32 arg3)
{
    u8 buffer[0x78];
    u32 r0;
    u32 r1;
    u32 r3;
    u32 r4;
    u32 ebx_value;
    u32 ebp_value;
    u32 esi_value;
    u32 edi_value;
    u32 g2;
    u32 g4;
    s32 ecx_value;
    s32 edx_value;

    if (arg2 == 0) {
        (void)cm_fn_00785830((void *)arg1, 0u);
        (void)cm_fn_00785830((void *)arg1, 1u);
        (void)cm_fn_00785830((void *)arg1, 2u);
        cm_fn_0066f4e0(0x00de1f64u, 0x00a11660u);
        return (u32)cm_fn_004ae660((void *)arg3, 0x00de1f64u);
    }

    r0 = (u32)cm_fn_00785830((void *)arg1, 0u);
    edi_value = r0;
    r1 = (u32)cm_fn_00785830((void *)arg1, 1u);
    ebx_value = r1;
    (void)cm_fn_00785830((void *)arg1, 2u);
    r4 = (u32)cm_fn_00785830((void *)arg1, 4u);
    ebp_value = (u32)(s32)(s8)(u8)r4;
    (void)cm_fn_00785830((void *)arg1, 5u);

    g2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
    esi_value = ebp_value * 290u;
    cm_fn_006181d0(esi_value + g2, (u32)buffer, 0x78u, 9u);

    ebx_value = ebx_value * 581u;
    edi_value = edi_value * 581u;
    g4 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    ecx_value = (s32)(s8)*(volatile s8 *)(ebx_value + g4 + 0x37u);
    edx_value = (s32)(s8)*(volatile s8 *)(edi_value + g4 + 0x37u);
    cm_fn_0066f4e0(0x00de1f64u, 0x00a11574u, 0x64u, 0x64u,
                   (u32)edx_value, (u32)edx_value,
                   (u32)ecx_value, (u32)ecx_value,
                   (u32)buffer,
                   edi_value + g4 + 4u,
                   ebx_value + g4 + 4u);

    (void)cm_fn_004ae660((void *)arg3, 0x00de1f64u);

    g4 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    (void)cm_fn_004ae8a0((int)arg3, (u8 *)(edi_value + g4 + 4u), 0x7d5, (int)(edi_value + g4));

    g2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
    esi_value = esi_value + g2;
    r3 = (u32)cm_fn_0053b3d0((int *)esi_value);
    (void)cm_fn_004ae8a0((int)arg3, (u8 *)buffer, 0x7d5, (int)r3);

    g4 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    return cm_fn_004ae8a0((int)arg3, (u8 *)(ebx_value + g4 + 4u), 0x7d5, (int)(ebx_value + g4));
}
