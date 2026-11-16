/* fn_008e32c0, preferred-image VA 0x008e32c0, image base 0x00400000, pinned
   CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.

   Signature: `ret 0xc` (0x008e34cf / 0x008e359d) pops three stack arguments and
   the routine reads no register before writing it (no ECX receiver), so it is a
   three-argument stdcall.  Arg2 is the mode flag tested at entry
   (0x008e32c0 `mov eax,[esp+8]`, 0x008e32ca `test eax,eax`,
   0x008e32d2 `je 0x8e34d2`); arg1 ([esp+0x1b4]) is the object passed in ECX to
   0x00785830; arg3 ([esp+0x1b8] at 0x008e341f and [esp+0x1c8] at 0x008e3537) is
   the receiver passed in ECX to 0x004ae660 / 0x004ae8a0.

   Entry stack accounting: after `sub esp,0x19c` (0x008e32c4) and the four saved
   registers, `push 0` at 0x008e32d0 is the index argument of the first
   0x00785830 call (0x008e32e1) in *both* paths, so the steady-state ESP for the
   rest of the body is 4 bytes above that push.  Local frame base L = entry_esp -
   0x19c; the two callee-visible buffers are L+0x0c (100 bytes, output of
   0x006181d0, 0x008e335f/0x008e33d3/0x008e3405/0x008e33f9) and L+0x70 (300
   bytes, output of 0x0053e140, 0x008e3332/0x008e3504/0x008e34ae/0x008e3577).

   ABI findings from original instructions (all VAs preferred-image):
   - 0x00785830: ECX = arg1, one pushed index, callee pops it (no `add esp` after
     0x008e32e1, 0x008e32ec, 0x008e32fb, 0x008e3308, 0x008e3315, 0x008e3320,
     0x008e34db, 0x008e34e6, 0x008e34f1, 0x008e34fc): thiscall, one stack arg.
   - 0x004ae660: ECX = arg3, `push 0xde1f64` at 0x008e3426 / 0x008e3541, no
     `add esp`: thiscall, one stack arg.
   - 0x004ae8a0: ECX = arg3, three pushed arguments at each of 0x008e3445,
     0x008e346e, 0x008e34c0, 0x008e358e (12 stack bytes, no `add esp`): thiscall,
     three stack args.
   - 0x0053e140: seven pushes, one `add esp,0x1c` at 0x008e3347/0x008e351b: cdecl.
   - 0x0053e990: five pushes, `add esp,0x14` at 0x008e349e/0x008e356a: cdecl.
   - 0x006181d0: four pushes, `add esp,0x10` at 0x008e3383/0x008e33ed: cdecl.
   - 0x0066f4e0: twelve pushes with `add esp,0x30` at 0x008e33bf and nine pushes
     with `add esp,0x24` at 0x008e341c, plus four pushes with `add esp,0x10` at
     0x008e353e: cdecl variadic.  The last push is the first argument, so the
     first argument is the 0xde1f64 buffer in all three calls. */

#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u8 *param_2, u32 param_3, u32 param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output,
                                   u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third,
                                   u32 date);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32 arg1, u32 arg2, u32 arg3,
                                                  u32 arg4);
extern char * CM_CDECL cm_fn_0066f4e0(u32 dest, u32 format, ...);

/* 0x00ae23b4 and 0x00ae23bc are read again after each call in the listing
   (0x008e334f/0x008e3373, 0x008e33c4/0x008e33e7, 0x008e3432, 0x008e344f,
   0x008e3488, 0x008e34a9, 0x008e354e, 0x008e3571); volatile keeps every one of
   those reloads at its original position. */
#define BASE_X CM_GLOBAL(volatile u32, 0x00ae23b4)
#define BASE_Y CM_GLOBAL(volatile u32, 0x00ae23bc)

/* Row stride 581 bytes: 0x008e3355-0x008e3365 `lea eax,[edi+edi*8]; shl eax,4;
   add eax,edi; lea edi,[edi+eax*4]` and again at 0x008e3378-0x008e3386. */
#define ROW581(i) ((u32)(i) * 0x245u)

/* 110-byte scaled index of the other table: 0x008e3477-0x008e3492
   `lea eax,[ebx+ebx*4]; lea esi,[ebx+eax*2]; lea esi,[esi+esi*4]; shl esi,1`,
   and 0x008e354b-0x008e355f. */
#define SCALE110(i) ((u32)(i) * 0x6eu)

void CM_STDCALL cm_fn_008e32c0(u32 object, u32 mode, u32 context)
{
    u8 frame[0x19c];
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;
    u32 v4;
    u32 v5;
    u32 row;
    u32 base;
    u32 rec;
    u32 alt;
    s32 ch_a;
    s32 ch_b;
    u32 res;

    if (mode != 0u) {
        /* 0x008e32d8 `mov esi,[esp+0x1b4]` = arg1 = ECX for every getter. */
        v0 = (u32)cm_fn_00785830(object, 0u);
        v1 = (u32)cm_fn_00785830(object, 1u);
        v2 = (u32)cm_fn_00785830(object, 2u);
        v3 = (u32)cm_fn_00785830(object, 3u);
        v4 = (u32)cm_fn_00785830(object, 4u);
        v5 = (u32)cm_fn_00785830(object, 5u);

        /* 0x008e332f-0x008e3342 push order: v0, 1, 0x12c, &L+0x70, v3, v2, v1,
           i.e. 0x0053e140(v1, v2, v3, L+0x70, 0x12c, 1, v0). */
        cm_fn_0053e140(v1, v2, v3, (u32)(frame + 0x70), 0x12cu, 1u, v0);

        if (v5 != 0xffffffffu) {   /* 0x008e334a `cmp esi,-1` */
            row = ROW581(v4);
            /* 0x008e335d `push 8`, 0x008e3363 `push 0x64`, 0x008e3368 push
               &L+0x0c, 0x008e3369 dword load at [BASE_X + row + 0x53]. */
            cm_fn_006181d0(*(volatile u32 *)((u8 *)(row + BASE_X) + 0x53u),
                           (u32)(frame + 0x0c), 0x64u, 8u);
            base = BASE_X;                       /* 0x008e3373 */
            rec = row + base + 0x38u;            /* 0x008e3392 */
            alt = ROW581(v5) + base;             /* 0x008e3378-0x008e3386 */
            ch_a = (s32)*(volatile s8 *)((u8 *)(row + base) + 0x52u); /* 0x008e3389 */
            ch_b = (s32)*(volatile s8 *)((u8 *)alt + 0x52u);          /* 0x008e338e */
            /* 0x008e3399-0x008e33b5 push order: rec, &L+0x0c, L+0x70, alt+0x38,
               ch_a, ch_a, 0x64, 0x64, ch_b, ch_b, 0xa9d444, 0xde1f64. */
            cm_fn_0066f4e0(0x00de1f64u, 0x00a9d444u,
                           (u32)ch_b, (u32)ch_b, 0x64u, 0x64u,
                           (u32)ch_a, (u32)ch_a, alt + 0x38u,
                           (u32)(frame + 0x70), (u32)(frame + 0x0c), rec);
        }
        else {                     /* 0x008e33c4 */
            row = ROW581(v4);
            cm_fn_006181d0(*(volatile u32 *)((u8 *)(row + BASE_X) + 0x53u),
                           (u32)(frame + 0x0c), 0x64u, 8u);
            base = BASE_X;                       /* 0x008e33e7 */
            rec = row + base + 0x38u;            /* 0x008e33f5 */
            ch_a = (s32)*(volatile s8 *)((u8 *)(row + base) + 0x52u); /* 0x008e33f0 */
            /* 0x008e33fd-0x008e3412 push order: rec, &L+0x0c, L+0x70, ch_a,
               ch_a, 0x64, 0x64, 0xa9d39c, 0xde1f64. */
            cm_fn_0066f4e0(0x00de1f64u, 0x00a9d39cu,
                           0x64u, 0x64u, (u32)ch_a, (u32)ch_a,
                           (u32)(frame + 0x70), (u32)(frame + 0x0c), rec);
        }

        /* 0x008e341f `mov ebp,[esp+0x1b8]` = arg3. */
        cm_fn_004ae660(context, 0x00de1f64u);    /* 0x008e3426-0x008e342d */

        base = BASE_X;                           /* 0x008e3432 */
        rec = ROW581(v4) + base;                 /* 0x008e3439 */
        /* 0x008e343b-0x008e3445: push rec, 0x7d5, rec+0x38 with ECX = arg3. */
        cm_fn_004ae8a0(context, (u8 *)(rec + 0x38u), 0x7d5u, rec);

        if (v5 != 0xffffffffu) {   /* 0x008e344a */
            base = BASE_X;                       /* 0x008e344f */
            alt = ROW581(v5) + base;
            /* 0x008e3464-0x008e346e: push alt, 0x7d5, alt+0x38. */
            cm_fn_004ae8a0(context, (u8 *)(alt + 0x38u), 0x7d5u, alt);
        }

        /* 0x008e3473-0x008e3499: push order 0, v3, v2, v1, BASE_Y + v0*110;
           `add esp,0x14`. */
        res = cm_fn_0053e990(SCALE110(v0) + BASE_Y, v1, v2, v3, 0u);
        if (res != 0u) {
            base = BASE_Y;                       /* 0x008e34a9 */
            /* 0x008e34b7-0x008e34c0: push BASE_Y+v0*110, 0x7d7, L+0x70 with
               ECX = arg3. */
            cm_fn_004ae8a0(context, frame + 0x70, 0x7d7u, SCALE110(v0) + base);
        }
        return;
    }

    /* 0x008e34d2: mode == 0, arg1 ([esp+0x1b4]) is the getter object. */
    v0 = (u32)cm_fn_00785830(object, 0u);
    v1 = (u32)cm_fn_00785830(object, 1u);
    v2 = (u32)cm_fn_00785830(object, 2u);
    v3 = (u32)cm_fn_00785830(object, 3u);

    /* 0x008e3501-0x008e3516 push order: v0, 5, 0x12c, &L+0x70, v3, v2, v1. */
    cm_fn_0053e140(v1, v2, v3, (u32)(frame + 0x70), 0x12cu, 5u, v0);

    /* 0x008e351e-0x008e3532 push order: L+0x70, 0x64, 0xa9d364, 0xde1f64. */
    cm_fn_0066f4e0(0x00de1f64u, 0x00a9d364u, 0x64u, (u32)(frame + 0x70));

    /* 0x008e3537 `mov ecx,[esp+0x1c8]` = arg3; 0x008e3541 push 0xde1f64. */
    cm_fn_004ae660(context, 0x00de1f64u);

    /* 0x008e354b-0x008e3565: push order 0, v3, v2, v1, BASE_Y + v0*110, where
       BASE_Y is loaded at 0x008e354e (after the 0x004ae660 call). */
    res = cm_fn_0053e990(SCALE110(v0) + BASE_Y, v1, v2, v3, 0u);
    if (res != 0u) {
        base = BASE_Y;                           /* 0x008e3571 */
        /* 0x008e3580 `mov ecx,[esp+0x1b8]` = arg3; pushes BASE_Y+v0*110, 0x7d7,
           L+0x70. */
        cm_fn_004ae8a0(context, frame + 0x70, 0x7d7u, SCALE110(v0) + base);
    }
}
