/* Preferred-image VA 0x0069C540, image base 0x00400000. THISCALL: ECX is saved in EBP
   at entry (mov ebp, ecx). Two stack arguments, ret 8: arg0 is kept in EDI, arg1 is
   compared with the current record pointer (0x0069C58D). Returns EAX = best value,
   starting at 0xFFFFD8F0 (-10000); the early exit at 0x0069C561 returns that value.
   Loop: record pointer ESI advances by 0x6E per index, offset S by 0x4F, both
   per record visited (including skipped records). Callee 0x0069F810 and 0x0069D1B0
   are STDCALL (ret 0x10 each; no ECX read). 0x0069FC60, 0x004CF6D0 and 0x004CF5F0 read
   ECX as their object (THISCALL). */
#include "cmre.h"

#define FLD_U8(base, off)  (*(volatile u8 *)((u32)(base) + (u32)(off)))
#define FLD_U16(base, off) (*(volatile u16 *)((u32)(base) + (u32)(off)))
#define FLD_U32(base, off) (*(volatile u32 *)((u32)(base) + (u32)(off)))

extern int CM_THISCALL cm_fn_004cf6d0(int *param_1,int *param_2);
extern u32 CM_THISCALL cm_fn_0069fc60(int param_1,int *param_2);
extern int CM_THISCALL cm_fn_004cf5f0(int *param_1,int *param_2,int *param_3);
extern u32 CM_STDCALL cm_fn_0069f810(int param_1,int param_2,double param_3);
extern int CM_STDCALL cm_fn_0069d1b0(int *param_1,int param_2,int param_3,char param_4);

s32 CM_THISCALL cm_fn_0069c540(u32 self, u32 arg0, u32 arg1)
{
    s32 i;
    s32 n;
    s32 best;
    u32 rec;
    u32 off;
    u32 g;
    u32 o;
    u32 eax_v;
    u32 ecx_v;
    u32 edx_v;
    u32 r2;
    s32 r;
    u8 al;
    u8 dl_v;
    u8 b3d;

    n = CM_GLOBAL(volatile s32, 0x00ae2364);        /* mov ecx, [0xae2364] */
    rec = CM_GLOBAL(volatile u32, 0x00ae23bc);      /* mov esi, [0xae23bc] */
    best = (s32)0xffffd8f0u;                         /* mov eax, 0xffffd8f0; [esp+0x10] */
    if (n <= 0) {
        return best;
    }
    i = 0;
    off = 0;

    do {
        if (FLD_U32(rec, 0x69) == 0) {               /* mov eax, [esi+0x69]; je */
            goto next;
        }
        if (i >= (s32)((u32)n - 0x10u)) {            /* add ecx,-0x10; cmp ebx,ecx; jl */
            if (FLD_U8(rec, 0x5f) == 0) {
                goto next;
            }
        }
        if (arg1 == rec) {
            goto next;
        }
        g = CM_GLOBAL(volatile u32, 0x00ae2d0c);     /* mov eax, [0xae2d0c] */
        if ((FLD_U8(off + g, 0xb) & 0x30u) != 0) {   /* test byte [ecx+eax+0xb], 0x30 */
            goto next;
        }
        o = CM_GLOBAL(volatile u32, 0x00ae1050);     /* mov ecx, [0xae1050] */
        r = cm_fn_004cf6d0((int *)o, (int *)rec);
        if (r != 0) {
            goto next;
        }
        if ((u8)cm_fn_0069fc60((int)self, (int *)rec) != 0) {
            goto next;
        }
        eax_v = FLD_U32(arg0, 0x53);                 /* mov eax, [edi+0x53] */
        ecx_v = FLD_U32(rec, 0x1a);                  /* mov ecx, [esi+0x1a] */
        if (eax_v != ecx_v) {
            edx_v = FLD_U32(rec, 0x69);              /* mov edx, [esi+0x69] */
            if ((s16)FLD_U16(edx_v, 0xa) < 0xea6) {
                goto next;
            }
        }
        if (eax_v == 0) {
            goto next;
        }
        if (ecx_v == 0) {
            goto next;
        }
        al = FLD_U8(eax_v, 0x75);                    /* mov al, [eax+0x75] */
        dl_v = FLD_U8(ecx_v, 0x75);                  /* mov dl, [ecx+0x75] */
        if (al != dl_v) {
            ecx_v = FLD_U32(rec, 0x69);              /* mov ecx, [esi+0x69] */
            if ((s16)FLD_U16(ecx_v, 0xc) < 0x109a) {
                goto next;
            }
            if ((s16)FLD_U16(arg0, 0x80) < 0x128e) {
                goto next;
            }
        }
        if (FLD_U32(rec, 0x61) == 0) {               /* mov eax, [esi+0x61]; je */
            goto call_f810;
        }
        if (FLD_U32(rec, 0x39) == 0) {               /* mov eax, [esi+0x39]; je */
            goto call_f810;
        }
        b3d = FLD_U8(rec, 0x3d);                     /* mov al, [esi+0x3d] */
        if (b3d != 0xb && b3d != 0xc && b3d != 0xd && b3d != 0xf) {
            goto call_f810;
        }
        o = CM_GLOBAL(volatile u32, 0x00ae1050);     /* mov ecx, [0xae1050] */
        r2 = (u32)cm_fn_004cf5f0((int *)o, (int *)rec, (int *)arg0);
        if (r2 == 0) {
            goto call_f810;
        }
        if (FLD_U8(r2, 0x49) != 0) {                 /* mov cl, [eax+0x49]; test; jne */
            goto next;
        }

call_f810:
        if ((u8)cm_fn_0069f810((int)rec, (int)arg0, 3.5) == 0) {
            goto next;
        }
        r = cm_fn_0069d1b0((int *)rec, (int)arg0, 0, (char)5);
        if (r <= best) {
            goto next;
        }
        best = r;

next:
        off = off + 0x4fu;                           /* edx = [esp+0x10] + 0x4f */
        rec = rec + 0x6eu;                           /* add esi, 0x6e */
        n = CM_GLOBAL(volatile s32, 0x00ae2364);     /* mov ecx, [0xae2364] */
        i = i + 1;
    } while (i < n);

    return best;
}
