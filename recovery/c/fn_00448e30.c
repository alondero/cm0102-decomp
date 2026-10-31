/* Preferred-image VA 0x00448E30, image base 0x00400000. THISCALL: ECX is the object
   (mov edi, ecx). No stack arguments, ret 0. Always returns 1 (mov eax, 1 at 0x00448F82).
   Frame, one block with offsets from its base B (esp after `sub esp, 0xe8`):
     B+0x00 loop index (dword, stored at 0x00448E5D and 0x00448F51)
     B+0x04 byte cl = record byte +4 (stored at 0x00448E7A). B+0x05..B+0x07 are never written
            by the listing, but the dword at B+0x04 is read and pushed (0x00448ED1/0x00448EE7).
     B+0x08..B+0xE7 object O (ECX for 0x0077BBE0 is O; O is also the fourth argument of
            0x00453120, and the second argument of 0x007861E0 and 0x007863F0)
     B+0xE8 SEH record {prev, handler 0x009554BB, state}
   Record size: 6 bytes per index in the array at object+0x24; the object's own count is
   the dword at +0x2c. Per record, G1 = [0x00AE23B4], and the pointer
   ebp_v = G1 + 581 * esi, where esi = record dword at +0. */
#include "cmre.h"

#define FLD_U8(base, off)  (*(volatile u8 *)((u32)(base) + (u32)(off)))
#define FLD_U32(base, off) (*(volatile u32 *)((u32)(base) + (u32)(off)))
#define FLD_S32(base, off) (*(volatile s32 *)((u32)(base) + (u32)(off)))

extern int CM_THISCALL cm_fn_00452710(int param_1,int *param_2,int param_3);
extern int CM_THISCALL cm_fn_004528d0(int param_1,int *param_2,int param_3);
extern u32 cm_fn_00453120(u32 param_1,u8 *param_2,int param_3,int param_4);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self,u32 arg1,u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self,u32 arg1,u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_007863f0(void *self,u32 arg1,u32 arg2);
extern u32 CM_STDCALL cm_fn_007875e0(int param_1);   /* ret 4 (listing evidence) */
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

u32 CM_THISCALL cm_fn_00448e30(u32 self)
{
    struct {
        u32 idx;                          /* B+0x00 */
        u8 b4[4];                         /* B+0x04 */
        u8 obj[0xe0];                     /* B+0x08 */
        volatile struct { u32 prev; u32 handler; s32 state; } seh;   /* B+0xe8 */
    } f;
    u32 entry_prev;
    u32 off;
    u32 rec;
    u32 esi_v;
    u32 g1;
    u32 ebp_v;
    u32 arrp;
    u32 dw4;
    u32 fret;
    u32 idx;
    s32 n2;
    s32 r;
    u8 cl;

    entry_prev = CM_FS(u32, 0);           /* mov eax, fs:[0] */
    f.seh.state = -1;
    f.seh.handler = 0x009554bbu;
    f.seh.prev = entry_prev;
    CM_FS(u32, 0) = (u32)&f.seh;

    off = 0;                              /* xor ebx, ebx */
    if (FLD_U32(self, 0x24) == 0) {       /* cmp dword [edi+0x24], ebx; je 0x00448F7A */
        goto unlink;
    }
    n2 = FLD_S32(self, 0x2c);             /* mov eax, [edi+0x2c] */
    f.idx = 0;                            /* mov [esp+8], ebx */
    if (n2 > 0) {
        do {
            rec = FLD_U32(self, 0x24) + off;  /* mov eax, [edi+0x24]; add eax, ebx */
            esi_v = FLD_U32(rec, 0);          /* mov esi, [eax] */
            cl = FLD_U8(rec, 4);              /* mov cl, [eax+4] */
            g1 = CM_GLOBAL(volatile u32, 0x00ae23b4);   /* mov eax, [0xae23b4] */
            f.b4[0] = cl;                     /* mov [esp+0x14], cl */
            (void)CM_GLOBAL(volatile u32, 0x00b7431c);  /* mov ecx, [0xb7431c] (read; dead value) */
            ebp_v = g1 + esi_v + (esi_v * 145u) * 4u;   /* lea edx/shl/add/lea: g1 + 581*esi */

            r = cm_fn_007875e0((int)ebp_v);   /* push ebp; call 0x007875E0 (stdcall, ret 4) */
            if (r == 0) {
                if (cm_fn_0053b900((u32 *)ebp_v) == 0) {   /* push ebp; call; je 0x00448F44 */
                    goto next;
                }
            }

            /* 0x00448EAC block */
            cm_fn_0077bbe0((void *)f.obj, 0x3e8u, 0u);     /* push 0; push 0x3e8; lea ecx,[esp+0x20] */
            arrp = FLD_U32(self, 0x24);                    /* mov ecx, [edi+0x24] */
            f.seh.state = 0;                               /* mov dword [esp+0x100], 0 */
            if (FLD_U8(arrp + off, 5) == 2) {              /* cmp byte [ebx+ecx+5], 2; jne */
                dw4 = FLD_U32((u32)f.b4, 0);               /* mov eax, [esp+0x14] */
                fret = (u32)cm_fn_004528d0((int)self, (int *)ebp_v, 0);
            } else {
                dw4 = FLD_U32((u32)f.b4, 0);               /* mov edx, [esp+0x14] */
                fret = (u32)cm_fn_00452710((int)self, (int *)ebp_v, 0);
            }
            /* Callee ret 8 leaves dword dw4 and the object pointer on the stack, and
               0x00453120 (cdecl, four args) takes them as its third and fourth args. */
            (void)cm_fn_00453120((u32)esi_v, (u8 *)fret, (int)dw4, (int)(u32)f.obj);

            if (cm_fn_0053b900((u32 *)ebp_v) == 0) {      /* push ebp; call; je 0x00448F28 */
                /* mov ecx, [0xb7431c] (one read) */
                (void)cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c), (u32)f.obj, ebp_v);
            } else {
                dw4 = FLD_U32(ebp_v, 0x53);               /* mov eax, [ebp+0x53] */
                (void)cm_fn_007863f0((void *)CM_GLOBAL(u32, 0x00b7431c), (u32)f.obj, dw4);
            }
            f.seh.state = -1;                              /* mov dword [esp+0x100], -1 */

next:
            idx = f.idx + 1u;                              /* mov eax, [esp+0x10]; inc eax */
            off = off + 6u;                                /* add ebx, 6 */
            n2 = FLD_S32(self, 0x2c);                      /* mov ecx, [edi+0x2c] */
            f.idx = idx;                                   /* mov [esp+0x10], eax */
        } while ((s32)idx < n2);
        off = 0;                                           /* xor ebx, ebx */
    }

    /* 0x00448F5F tail */
    if (FLD_S32(self, 0x28) >= 0x64) {
        arrp = FLD_U32(self, 0x24);
        (void)cm_fn_009452ca(arrp);
        FLD_U32(self, 0x24) = 0;
        FLD_U32(self, 0x28) = 0;
    }
    FLD_U32(self, 0x2c) = 0;

unlink:
    CM_FS(u32, 0) = f.seh.prev;           /* mov ecx, [esp+0xf0]; mov fs:[0], ecx */
    return 1u;
}
