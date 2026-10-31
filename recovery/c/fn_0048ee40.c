#include "cmre.h"

/* Field access by byte offset from an object address (volatile: keep order). */
#define OBJ_U32(base, off) (*(volatile u32 *)((u32)(base) + (off)))
#define OBJ_S8(base, off) (*(volatile s8 *)((u32)(base) + (off)))

/* Original globals read in listing order. */
#define G_AE23B4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_AE23D0 CM_GLOBAL(volatile u32, 0x00ae23d0)

/* Thiscall slot taking one stack argument; returns 64 bits, only EAX is used. */
extern unsigned long long CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
/* Thiscall slot taking three stack arguments; its result is not used here. */
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 a, u32 b, u32 c);
/* Cdecl formatter with variadic stack arguments; the caller pops them. */
extern u32 CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

/* Incoming stack arguments: arg1 = [entry+4], arg2 = [entry+8], arg3 = [entry+12].
   The listing writes arg1 and arg2 slots in place (mov [esp+0x20], ...;
   mov [esp+0x24], ...); those writes are kept as reassignments of the
   parameters. ECX is the object pointer at entry (self). */
void CM_THISCALL cm_fn_0048ee40(u32 self, u32 arg1, u32 arg2, u32 arg3)
{
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 scaled1;
    u32 scaled2;
    u32 scaled3;
    u32 g1a;
    u32 g1b;
    u32 g1c;
    u32 g2a;
    u32 g2b;
    u32 g2c;
    u32 g2d;
    u32 g2e;
    u32 g2f;
    u32 g2g;
    u32 g2h;
    u32 field4;
    u32 t_ebx;
    u32 t_eax;
    u32 t_edx;
    u32 t_ecx;
    u32 p_ecx;
    u32 p_a;
    u32 p_b;
    u32 p_c;
    u32 p_d;
    u32 local_a;
    u32 local_b;
    u32 local_c;
    u8 al;

    r0 = (u32)cm_fn_00785830(arg1, 4);
    al = (u8)r0;

    if (arg2 != 0) {
        if (al == 1) {
            /* P1: three more element lookups, then two scratch tables and a formatter. */
            r1 = (u32)cm_fn_00785830(arg1, 1);
            r2 = (u32)cm_fn_00785830(arg1, 2);
            r3 = (u32)cm_fn_00785830(arg1, 3);
            scaled1 = 581u * r1;
            scaled2 = 581u * r2;
            scaled3 = 107u * r3;

            arg1 = scaled1;
            g1a = G_AE23D0;
            g2a = G_AE23B4;
            arg2 = scaled2;
            field4 = OBJ_U32(self, 4);
            t_edx = (u32)(s32)OBJ_S8(scaled3 + g1a, 0x52);
            t_ebx = (u32)(s32)OBJ_S8(arg2 + g2a, 0x52);
            local_a = t_ebx;
            t_ebx = (u32)(s32)OBJ_S8(field4 + 4, 0x4e);
            local_b = t_ebx;
            t_ebx = (u32)(s32)OBJ_S8(arg1 + g2a, 0x52);
            local_c = t_ebx;
            g1b = G_AE23D0;
            p_a = scaled3 + g1b + 0x38;
            p_b = arg2 + g2a + 0x38;
            p_c = field4 + 4;
            p_d = arg1 + g2a + 0x38;

            cm_fn_0066f4e0(0x00de1f64u, 0x0099bf24u,
                           local_c, local_c, local_b, local_b,
                           local_a, local_a, t_edx, t_edx,
                           p_d, p_c, p_b, p_a);

            cm_fn_004ae660(arg3, 0x00de1f64u);
            g2b = G_AE23B4;
            cm_fn_004ae8a0(arg3, arg1 + g2b + 0x38, 0x7d5u, arg1 + g2b);
            g2c = G_AE23B4;
            cm_fn_004ae8a0(arg3, g2c + arg2 + 0x38, 0x7d5u, g2c + arg2);
            field4 = OBJ_U32(self, 4);
            cm_fn_004ae8a0(arg3, field4 + 4, 0x7d0u, field4);
            g1c = G_AE23D0;
            cm_fn_004ae8a0(arg3, scaled3 + g1c + 0x38, 0x7d0u, 107u * r2 + g1c);
        } else if (al == 2) {
            /* P2 */
            r1 = (u32)cm_fn_00785830(arg1, 1);
            field4 = OBJ_U32(self, 4);
            g2a = G_AE23B4;
            scaled1 = 581u * r1;
            t_eax = (u32)(s32)OBJ_S8(field4 + 4, 0x33);
            t_edx = (u32)(s32)OBJ_S8(scaled1 + g2a, 0x52);
            p_ecx = scaled1 + g2a + 0x38;
            cm_fn_0066f4e0(0x00de1f64u, 0x0099be70u,
                           t_edx, t_edx, t_eax, t_eax,
                           p_ecx, field4 + 4);

            cm_fn_004ae660(arg3, 0x00de1f64u);
            g2d = G_AE23B4;
            cm_fn_004ae8a0(arg3, scaled1 + g2d + 0x38, 0x7d5u, scaled1 + g2d);
            field4 = OBJ_U32(self, 4);
            cm_fn_004ae8a0(arg3, field4 + 4, 0x7d0u, field4);
        }
    } else {
        if (al == 1) {
            /* P3 */
            r1 = (u32)cm_fn_00785830(arg1, 1);
            field4 = OBJ_U32(self, 4);
            g2e = G_AE23B4;
            t_ecx = field4 + 0x38;
            scaled1 = 581u * r1;
            t_eax = (u32)(s32)OBJ_S8(t_ecx, 0x1a);
            t_edx = (u32)(s32)OBJ_S8(scaled1 + g2e, 0x52);
            p_ecx = scaled1 + g2e + 0x38;
            cm_fn_0066f4e0(0x00de1f64u, 0x0099be0cu,
                           t_edx, t_edx, t_eax, t_eax,
                           p_ecx, t_ecx);

            cm_fn_004ae660(arg3, 0x00de1f64u);
            g2f = G_AE23B4;
            cm_fn_004ae8a0(arg3, scaled1 + g2f + 0x38, 0x7d5u, scaled1 + g2f);
            field4 = OBJ_U32(self, 4);
            cm_fn_004ae8a0(arg3, field4 + 0x38, 0x7d0u, field4);
        } else if (al == 2) {
            /* P4 */
            r1 = (u32)cm_fn_00785830(arg1, 1);
            scaled1 = 581u * r1;
            g2g = G_AE23B4;
            t_eax = (u32)(s32)OBJ_S8(scaled1 + g2g, 0x52);
            t_edx = scaled1 + g2g + 0x38;
            cm_fn_0066f4e0(0x00de1f64u, 0x0099bddcu,
                           t_eax, t_eax, t_edx);

            cm_fn_004ae660(arg3, 0x00de1f64u);
            g2h = G_AE23B4;
            cm_fn_004ae8a0(arg3, scaled1 + g2h + 0x38, 0x7d5u, scaled1 + g2h);
        }
    }
}
