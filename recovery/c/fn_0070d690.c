/* fn_0070d690 -- preferred image VA 0x0070d690, image base 0x00400000
   (SHA-256 0c78e7da...f86), 1926 bytes, module match_pl.cpp.

   Plain C translation of the original instructions.

   Frame model (L = esp after `sub esp,0x20c`, four saved registers pushed):
   L+0 is one shared scratch dword (float table copy, then the 0xf582 load,
   then `(s8)(ctx[0x8eaa]-1)` cubed and scaled), L+7 one saved byte
   (ctx[0x8ea9]), L+8 one dword (the 0xb4/0x168 distance sample), and
   L+0xc / L+0x10c two adjacent 256-byte buffers.

   __ftol sites (MSVC lowering of float->int, ADR 0006), written as
   (long long) of a long double following the listing's x87 sequence:
     0x0070d783: fld dword [edi+0x75]; fadd dword [0x9689a8]   (ST0 consumed)
     0x0070d796: fld dword [esp+0x10]; fadd st(0),st(0)        (ST0 consumed)
     0x0070d879: fld dword [edi+0x4d]                          (ST0 consumed)

   Physical register chain: cm_fn_0090cfc0 returns the original EDX:EAX pair
   as a 64-bit value, so the EDX live across each call (used by
   `movsx dx,al` at 0x0070d924 and as entry_edx of later calls) is the high
   half of that return, not an unknown leftover. */

#include "cmre.h"

extern void cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38c0(u32, u32, u32, u32);
extern u8 CM_STDCALL cm_fn_006cb650(s8 value);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void CM_CDECL cm_fn_00945934(u32 param_1);

/* MSVC __ftol (original `call 0x00945640`, also 0x0070d783/0x0070d796/0x0070d879):
   x87 FISTP after the control word is set to truncation.  An out-of-range or
   NaN operand stores the x87 integer indefinite 0x8000000000000000, which the
   libgcc helper behind a plain (long long) cast does not do. */
static long long cm_ftol_00945640(long double t)
{
    if (!(t > -9223372036854775808.0L && t < 9223372036854775808.0L)) {
        return (long long)0x8000000000000000ULL;
    }
    return (long long)t;
}

short CM_THISCALL cm_fn_0070d690(u32 self, char *a1, char *a2, char *a3)
{
    union { float f; u32 u; u16 w; } L0;  /* [esp+0x10] with four regs saved */
    s8 L7;                                /* [esp+0x17] */
    u32 L8;                               /* [esp+0x18] */
    u8 scratch[0x200];                    /* [esp+0x1c] and [esp+0x11c] */
    u32 ctx;
    u32 iVar7;
    s16 bpx;
    u16 s11;
    u32 rb;                               /* ebx: the 0x70d89f random result */
    u32 ra;
    u32 edx_cur;                          /* physical EDX across the calls */
    unsigned long long q64;
    s8 c1;
    int gofloat;
    s32 idx;
    u32 swtarget;
    u8 mapbyte;
    s32 e1;
    s32 e2;
    s32 dif;
    s32 ab;
    s32 xx;
    s32 v9;
    s32 num;
    s32 den;
    s32 quo;
    s32 qs;
    u32 vmix;
    u32 cube;
    s32 ret6;
    long double t;
    unsigned long long ll;
    s32 hi;
    s32 hi2;
    u32 adj;
    u32 v32;

    s11 = 0;                                       /* 0x0070d6aa xor esi,esi */
    L8 = 0;                                        /* 0x0070d6ac */
    ctx = *(volatile u32 *)(self + 0x19e);         /* 0x0070d6b0 */
    L7 = *(volatile s8 *)(ctx + 0x8ea9);           /* 0x0070d6b6 mov cl,[eax+0x8ea9] */
    *(volatile s8 *)a2 = 0;                        /* 0x0070d6c3 */
    *(volatile s8 *)a3 = 0;                        /* 0x0070d6c7 */
    c1 = *(volatile s8 *)a1;                       /* 0x0070d6ca mov al,[eax] */
    edx_cur = 0;                                   /* no EDX read before it is written */
    /* 0x0070d6cc: the ctx[0x8ea9] byte is saved in [esp+0x17] (L7). */

    if (c1 != 0x25 && c1 != 0x15) {                /* 0x0070d6d0 / 0x0070d6d8 */
        s8 d27 = *(volatile s8 *)(self + 0x27);    /* 0x0070d6e0 mov dl,[edi+0x27] */
        s8 d102 = *(volatile s8 *)(self + 0x102);  /* 0x0070d6e3 mov cl,[edi+0x102] */
        gofloat = 0;
        if (d27 == 1) {                            /* 0x0070d6e9 cmp dl,1 */
            if (d102 < 2 || d102 > 6) {            /* 0x0070d6ee / 0x0070d6f3 */
                gofloat = 1;
            }
            else {
                s8 d103 = *(volatile s8 *)(self + 0x103);   /* 0x0070d6f8 */
                if (d103 < 0xa || d103 > 0xb) {             /* 0x0070d6fe / 0x0070d702 */
                    gofloat = 1;
                }
            }
        }
        else {                                     /* 0x0070d70c */
            if (d102 < 2 || d102 > 6) {            /* 0x0070d70c / 0x0070d711 */
                gofloat = 1;
            }
            else {
                s8 d103 = *(volatile s8 *)(self + 0x103);   /* 0x0070d716 */
                if (d103 < 0 || d103 > 1) {                 /* 0x0070d71c / 0x0070d720 */
                    gofloat = 1;
                }
            }
        }

        if (gofloat) {
            s32 t1;
            s32 t2;

            /* 0x0070d728 movsx eax,byte [edi+0x103] */
            t1 = (s32)*(volatile s8 *)(self + 0x103);
            ra = (u32)t1;
            if (d27 == 1) {
                ra -= 0xbu;                        /* 0x0070d72f..0x0070d734 */
            }
            adj = (u32)((s32)ra >> 31);            /* cdq */
            ra = (ra ^ adj) - adj;                 /* xor eax,edx; sub eax,edx */
            idx = (s32)(s8)(u8)ra;                 /* movsx eax,al */
            idx = idx * 9;                         /* lea ebx,[eax+eax*8] */
            t2 = (s32)d102 - 4;                    /* movsx eax,cl; sub eax,4 */
            ra = (u32)t2;
            adj = (u32)((s32)ra >> 31);            /* cdq */
            ra = (ra ^ adj) - adj;                 /* abs */
            idx = idx + (s32)(s8)(u8)ra;           /* movsx ecx,al; add ebx,ecx */
            L0.f = CM_GLOBAL(float, 0x00a15e44u + (u32)(4 * (u32)idx));
            /* 0x0070d77a: fld dword [edi+0x75]; fadd dword [0x9689a8] */
            t = (long double)*(volatile float *)(self + 0x75)
                + (long double)CM_GLOBAL(float, 0x009689a8);
            ll = (unsigned long long)cm_ftol_00945640(t);      /* __ftol 0x0070d783 */
            q64 = cm_fn_0090cfc0((u32)ll, (u32)(ll >> 32), (u32)ll);
            ra = (u32)q64;                              /* EAX */
            edx_cur = (u32)(q64 >> 32);                 /* EDX */
            rb = ra;                                    /* 0x0070d794 mov ebx,eax */
            /* 0x0070d78e: fld dword [esp+0x10]; fadd st(0),st(0) */
            t = (long double)L0.f + (long double)L0.f;
            ll = (unsigned long long)cm_ftol_00945640(t);      /* __ftol 0x0070d796 */
            q64 = cm_fn_0090cfc0((u32)ll, (u32)(ll >> 32), (u32)ll);
            ra = (u32)q64;
            edx_cur = (u32)(q64 >> 32);
            if ((s32)rb < (s32)ra) {                    /* 0x0070d7a4 cmp/jge */
                *(volatile s8 *)a3 = 0;                 /* 0x0070d7b1 */
                *(volatile s8 *)a2 = 0;                 /* 0x0070d7b4 */
                return -0x14;                           /* 0x0070d7b9 mov ax,0xffec */
            }
        }
    }

    /* 0x0070d7c7 */
    ctx = *(volatile u32 *)(self + 0x19e);
    iVar7 = *(volatile u32 *)(ctx + 0xf582);       /* 0x0070d7cd */
    L0.u = iVar7;                                  /* 0x0070d7d5 mov [esp+0x10],ebx */
    if (iVar7 == 0) {                              /* 0x0070d7d3 test ebx,ebx */
        s8 c2 = *(volatile s8 *)a1;                /* 0x0070d7e6 mov al,[edx] */
        if (c2 != 0x15 && c2 != 0x25) {            /* 0x0070d7e8 / 0x0070d7ec */
            if (cm_fn_006cb650(c2) == 0) {         /* 0x0070d7f1 */
                /* 0x0070d7fa: error report; the original never returns here */
                cm_fn_00944cff((u32)0x00a16058u, 0, 0,
                               (void *)(scratch + 0x100), (void *)scratch);
                ra = cm_fn_0090d130();
                cm_fn_00944c9f((u32)0x00ae24d0u, (u32)0x009870e8u, ra,
                               (u32)(scratch + 0x100), (u32)scratch, (u32)0x457fu);
                cm_fn_005e8290((u32)0x009870e0u, (u32)0x00ae24d0u, 0);
                CM_GLOBAL(u32, 0x00b67a34) = iVar7;    /* 0x0070d850 */
                cm_fn_005cc7d0();                      /* 0x0070d856 */
                cm_fn_005a8f60();                      /* 0x0070d85b */
                cm_fn_00945934(0xffffffffu);           /* 0x0070d862 */
            }
        }
    }

    /* 0x0070d867 */
    if (iVar7 == self) {
        bpx = *(volatile s16 *)(iVar7 + 0x19c);    /* 0x0070d86b mov bp,word [ebx+0x19c] */
    }
    else {
        bpx = 0;                                   /* 0x0070d874 xor ebp,ebp */
    }

    /* 0x0070d876: fld dword [edi+0x4d] */
    t = (long double)*(volatile float *)(self + 0x4d);
    ll = (unsigned long long)cm_ftol_00945640(t);  /* __ftol 0x0070d879 */
    quo = (s32)(s16)(u16)ll;                       /* 0x0070d87e movsx ecx,ax */
    v9 = (s32)(0x2710u - (u32)quo);                /* 0x0070d881 / 0x0070d886 */
    ra = (u32)v9 * (u32)v9;                        /* 0x0070d88a imul ecx,eax */
    {
        long long pr = (long long)(s32)ra * (long long)0x14f8b589;
        hi = (s32)(pr >> 32);                      /* 0x0070d892 imul ecx (edx) */
        hi2 = hi >> 12;                            /* 0x0070d894 sar edx,0xc */
        adj = (u32)hi2 >> 31;                      /* 0x0070d899 shr eax,0x1f */
        hi2 = hi2 + (s32)adj;                      /* 0x0070d89c add edx,eax */
        q64 = cm_fn_0090cfc0(adj, (u32)hi2, (u32)hi2);   /* 0x0070d89f */
    }
    /* 0x0070d8a4 mov ebx,eax; 0x0070d8a6 mov eax,[esp+0x14] */
    rb = (u32)q64;
    edx_cur = (u32)(q64 >> 32);
    if (L0.u == 0) {
        ctx = *(volatile u32 *)(self + 0x19e);     /* 0x0070d8b1 */
        e1 = (s32)*(volatile s16 *)(self + 0x19a); /* 0x0070d8b7 */
        e2 = (s32)*(volatile s16 *)(ctx + 0x8ed8); /* 0x0070d8be */
        dif = e1 - e2;                             /* 0x0070d8c5 */
        ra = (u32)dif;
        adj = (u32)((s32)ra >> 31);                /* cdq */
        ra = (ra ^ adj) - adj;                     /* 0x0070d8c8 xor/sub */
        ab = (s32)ra;
        if ((s16)(u16)ab > 0xb4) {                 /* 0x0070d8cc cmp ax,0xb4 */
            xx = 0x168 - ab;                       /* 0x0070d8d2 */
        }
        else {
            xx = ab;
        }
        v9 = (s32)*(volatile s8 *)(ctx + 0x8ea9);  /* 0x0070d8db movsx ecx,byte[ecx+8ea9] */
        num = (0xb4 - xx) * 2;                     /* 0x0070d8e2..0x0070d8f0 */
        den = 5 - v9;                              /* 0x0070d8e9 */
        if (den < 1) {                             /* 0x0070d8f2 cmp eax,1 */
            den = 1;                               /* 0x0070d8f7 */
        }
        den = (s32)(s16)(u16)den;                  /* 0x0070d8fc movsx ecx,ax */
        quo = (s32)(s16)(u16)num;                  /* 0x0070d8ff movsx eax,dx */
        quo = quo / den;                           /* 0x0070d903 idiv ecx: EAX quotient */
        qs = (s32)(s16)(u16)quo;                   /* 0x0070d905 movsx edx,ax */
        q64 = cm_fn_0090cfc0((u32)quo, (u32)qs, (u32)qs);  /* 0x0070d909 */
        L8 = (u32)q64;                             /* 0x0070d911 mov [esp+0x18],eax */
        edx_cur = (u32)(q64 >> 32);
    }

    /* 0x0070d915 */
    ctx = *(volatile u32 *)(self + 0x19e);
    {
        s8 dv = (s8)(*(volatile s8 *)(ctx + 0x8eaa) - 1);  /* 0x0070d91c..0x0070d924 */
        v32 = (u32)(s32)dv;
        /* 0x0070d924 movsx dx,al keeps EDX bits 16..31 */
        vmix = (edx_cur & 0xffff0000u) | (v32 & 0xffffu);
        cube = vmix * vmix;                        /* 0x0070d92a imul eax,edx */
        cube = cube * vmix;                        /* 0x0070d92d imul eax,edx */
        cube = cube + cube * 4;                    /* 0x0070d930 lea eax,[eax+eax*4] */
        cube = cube << 2;                          /* 0x0070d933 shl eax,2 */
        L0.u = cube;                               /* 0x0070d936 mov [esp+0x14],eax */
        ret6 = (s32)cm_fn_006c38c0(cube, vmix, ctx, self);  /* 0x0070d93a */
    }
    if ((u16)ret6 == 1) {                          /* 0x0070d93f cmp ax,1 */
        s8 d27 = *(volatile s8 *)(self + 0x27);    /* 0x0070d945 mov al,[edi+0x27] */
        s8 d102 = *(volatile s8 *)(self + 0x102);  /* 0x0070d94a mov al,[edi+0x102] */
        int to9be = 0;
        if (d27 == 1) {
            if (d102 >= 2 && d102 <= 6) {
                s8 d103 = *(volatile s8 *)(self + 0x103);
                if (d103 >= 0xa && d103 <= 0xb) {
                    to9be = 1;
                }
            }
        }
        else {
            if (d102 >= 2 && d102 <= 6) {
                s8 d103 = *(volatile s8 *)(self + 0x103);
                if (d103 >= 0 && d103 <= 1) {
                    to9be = 1;
                }
            }
        }
        if (to9be) {
            u8 chv;
            s32 ix;

            /* 0x0070d9be */
            chv = (u8)((((u32)(L7 != 4)) - 1u) & 0xd8u) + 0x29u;  /* 0x0070d9c9..0x0070d9d3 */
            *(volatile s8 *)a1 = (s8)chv;                     /* 0x0070d9d9 mov [eax],cl */
            ix = (s32)(s8)chv;                                /* 0x0070d9d6 movsx edx,cl */
            s11 = *(volatile u16 *)(self + 0x11a + ix * 2);   /* 0x0070d9db */
            s11 = (u16)(s11 - (u16)rb);                       /* 0x0070d9e3 sub si,bx */
            s11 = (u16)(s11 - (u16)bpx);                      /* 0x0070d9e6 sub esi,ebp */
            goto dispatch2;
        }
    }

    /* 0x0070d968 */
    idx = (s32)*(volatile s8 *)a1;                 /* movsx eax,byte [eax] */
    idx = idx - 0xc;                               /* 0x0070d972 add eax,-0xc */
    swtarget = 0;
    if ((u32)idx <= 0x26u) {                       /* 0x0070d975 cmp eax,0x26 */
        mapbyte = *(volatile u8 *)(0x0070de54u + (u32)idx);      /* 0x0070d980 */
        swtarget = *(volatile u32 *)(0x0070de18u + (u32)mapbyte * 4u);  /* 0x0070d986 */
    }
    switch (swtarget) {
    case 0x0070db1cu:                              /* 0x0c */
        s11 = (u16)(*(volatile u16 *)(self + 0x132) - L0.w - (u16)rb
                    - (u16)L8 - (u16)bpx);
        break;
    case 0x0070da73u:                              /* 0x13 */
        s11 = (u16)(*(volatile u16 *)(self + 0x140) - (u16)rb - (u16)bpx);
        if ((s16)s11 < 2) {
            ra = (u32)cm_fn_0090cfc0((u32)idx, edx_cur, 2);
            if (ra != 0) {
                s11 = (u16)(*(volatile u16 *)(self + 0x174) - (u16)rb - (u16)bpx);
                *(volatile s8 *)a1 = (s8)0x2d;
            }
        }
        break;
    case 0x0070d98du:                              /* 0x15 */
        s11 = (u16)(*(volatile u16 *)(self + 0x144) - L0.w - (u16)rb
                    - (u16)L8 - (u16)bpx);
        break;
    case 0x0070da30u:                              /* 0x21 */
        s11 = (u16)(*(volatile u16 *)(self + 0x15c) - (u16)rb - (u16)bpx);
        if ((s16)s11 < 2) {
            ra = (u32)cm_fn_0090cfc0((u32)idx, edx_cur, 2);
            if (ra != 0) {
                s11 = (u16)(*(volatile u16 *)(self + 0x174) - (u16)rb - (u16)bpx);
                *(volatile s8 *)a1 = (s8)0x2d;
            }
        }
        break;
    case 0x0070d9edu:                              /* 0x22 */
        s11 = (u16)(*(volatile u16 *)(self + 0x15e) - (u16)rb - (u16)bpx);
        if ((s16)s11 < 2) {
            ra = (u32)cm_fn_0090cfc0((u32)idx, edx_cur, 2);
            if (ra != 0) {
                s11 = (u16)(*(volatile u16 *)(self + 0x174) - (u16)rb - (u16)bpx);
                *(volatile s8 *)a1 = (s8)0x2d;
            }
        }
        break;
    case 0x0070db34u:                              /* 0x25 */
        s11 = (u16)(*(volatile u16 *)(self + 0x164) - (u16)bpx);
        break;
    case 0x0070dad2u:                              /* 0x2a */
        s11 = (u16)(*(volatile u16 *)(self + 0x16e) - (u16)rb - (u16)bpx);
        break;
    case 0x0070dab6u:                              /* 0x2b */
        s11 = (u16)(*(volatile u16 *)(self + 0x170) - (u16)rb - (u16)bpx);
        break;
    case 0x0070dac4u:                              /* 0x2c */
        s11 = (u16)(*(volatile u16 *)(self + 0x172) - (u16)rb - (u16)bpx);
        break;
    case 0x0070dae0u:                              /* 0x2d */
        s11 = (u16)(*(volatile u16 *)(self + 0x174) - (u16)rb - (u16)bpx);
        break;
    case 0x0070daeeu:                              /* 0x2e */
        s11 = (u16)(*(volatile u16 *)(self + 0x176) - (u16)rb - (u16)bpx);
        break;
    case 0x0070dafcu:                              /* 0x2f */
        s11 = (u16)(*(volatile u16 *)(self + 0x178) - (u16)rb - (u16)bpx);
        break;
    case 0x0070db0au:                              /* 0x31 */
        s11 = (u16)(*(volatile u16 *)(self + 0x17c) - (u16)bpx);
        break;
    case 0x0070db13u:                              /* 0x32 */
        s11 = (u16)(*(volatile u16 *)(self + 0x17e) - (u16)bpx);
        break;
    default:
        break;                                     /* 0x0070db3e */
    }

dispatch2:
    /* 0x0070db3e reloads EDX with a1 before the dispatch/calls. */
    {
        s8 c3 = *(volatile s8 *)a1;                /* 0x0070db45 movsx eax,byte [edx] */
        s32 i3 = (s32)c3 - 1;                      /* 0x0070db48 dec eax */
        u32 tgt = 0;
        if ((u32)i3 <= 0x31u) {                    /* 0x0070db49 cmp eax,0x31 */
            u8 mb2 = *(volatile u8 *)(0x0070debcu + (u32)i3);        /* 0x0070db54 */
            tgt = *(volatile u32 *)(0x0070de7cu + (u32)mb2 * 4u);    /* 0x0070db5a */
        }
        switch (tgt) {
        case 0x0070db90u:
            ra = (u32)cm_fn_0090cfc0((u32)i3, (u32)a1, 2);
            *(volatile s8 *)a3 = (s8)(u8)((u8)ra + 2);       /* 0x0070dba8/0x0070dbaa */
            *(volatile s8 *)a2 = 1;                          /* 0x0070dbb2 */
            return (short)s11;
        case 0x0070dc5au:
            *(volatile s8 *)a3 = 0;                /* 0x0070dc6d */
            *(volatile s8 *)a2 = 0;                /* 0x0070dc71 */
            return (short)s11;
        case 0x0070dcabu:
            *(volatile s8 *)a3 = 2;                /* 0x0070dcba */
            *(volatile s8 *)a2 = 2;                /* 0x0070dcbd */
            return (short)s11;
        case 0x0070dbbfu:
            *(volatile s8 *)a3 = 2;                /* 0x0070dbce */
            *(volatile s8 *)a2 = 1;                /* 0x0070dbd6 */
            return (short)s11;
        case 0x0070dbe3u:
            *(volatile s8 *)a3 = 2;                /* 0x0070dbec */
            ra = (u32)cm_fn_0090cfc0((u32)i3, (u32)a1, 2);
            *(volatile s8 *)a2 = (s8)(u8)((u8)ra + 1);       /* 0x0070dbfe / 0x0070dc00 */
            return (short)s11;
        case 0x0070dd47u:
            *(volatile s8 *)a3 = 2;                /* 0x0070dd56 */
            *(volatile s8 *)a2 = 0;                /* 0x0070dd59 */
            return (short)s11;
        case 0x0070dd6bu:
            *(volatile s8 *)a3 = 2;                /* 0x0070dd7e */
            *(volatile s8 *)a2 = 0;                /* 0x0070dd82 */
            return (short)s11;
        case 0x0070db61u:
            ra = (u32)cm_fn_0090cfc0((u32)i3, (u32)a1, 2);
            *(volatile s8 *)a3 = (s8)(u8)((u8)ra + 2);       /* 0x0070db72 / 0x0070db74 */
            *(volatile s8 *)a2 = 1;                          /* 0x0070db7e */
            return (short)s11;
        case 0x0070dc7eu:
            *(volatile s8 *)a3 = 2;                /* 0x0070dc87 */
            ra = (u32)cm_fn_0090cfc0((u32)a3, (u32)a1, 2);
            *(volatile s8 *)a2 = (s8)ra;           /* 0x0070dc99 */
            return (short)s11;
        case 0x0070dc12u:
            *(volatile s8 *)a3 = 2;                /* 0x0070dc21 */
            *(volatile s8 *)a2 = 0;                /* 0x0070dc29 */
            return (short)s11;
        case 0x0070dc36u:
            *(volatile s8 *)a3 = 2;                /* 0x0070dc45 */
            *(volatile s8 *)a2 = 5;                /* 0x0070dc48 */
            return (short)s11;
        case 0x0070dccfu:
            *(volatile s8 *)a3 = 2;                /* 0x0070dcd8 */
            ra = (u32)cm_fn_0090cfc0((u32)i3, (u32)a1, 2);
            *(volatile s8 *)a2 = (s8)ra;           /* 0x0070dcea */
            return (short)s11;
        case 0x0070dcfcu:
            *(volatile s8 *)a3 = 2;                /* 0x0070dd05 */
            ra = (u32)cm_fn_0090cfc0((u32)a3, (u32)a1, 4);
            *(volatile s8 *)a2 = (s8)(u8)((u8)ra + 1);       /* 0x0070dd33 / 0x0070dd35 */
            return (short)s11;
        case 0x0070dd0au:
            *(volatile s8 *)a3 = 0;                /* 0x0070dd13 */
            ra = (u32)cm_fn_0090cfc0((u32)i3, (u32)a3, 4);
            *(volatile s8 *)a2 = (s8)(u8)((u8)ra + 1);       /* 0x0070dd33 / 0x0070dd35 */
            return (short)s11;
        case 0x0070dd18u:
            *(volatile s8 *)a3 = 2;                /* 0x0070dd21 */
            ra = (u32)cm_fn_0090cfc0((u32)i3, (u32)a3, 2);
            *(volatile s8 *)a2 = (s8)(u8)((u8)ra + 1);       /* 0x0070dd33 / 0x0070dd35 */
            return (short)s11;
        default:
            break;                                 /* 0x0070dd8f */
        }
    }

    /* 0x0070dd8f */
    *(volatile s8 *)a3 = 0;                        /* 0x0070dda4 */
    *(volatile s8 *)a2 = 0;                        /* 0x0070ddb6 */
    cm_fn_00944cff((u32)0x00a16058u, 0, 0, (void *)scratch, (void *)(scratch + 0x100));
    ra = cm_fn_0090d130();
    cm_fn_00944c9f((u32)0x00ae24d0u, (u32)0x009870e8u, ra,
                   (u32)scratch, (u32)(scratch + 0x100), (u32)0x4651u);
    cm_fn_005e8290((u32)0x009870e0u, (u32)0x00ae24d0u, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;                /* 0x0070ddff */
    return (short)s11;
}
