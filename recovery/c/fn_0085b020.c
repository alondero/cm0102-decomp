/* Preferred-image VA 0x0085B020, image base 0x00400000. STDCALL-style (ret 0xc) with
   three stack arguments: arg0 (object pointer, kept in EDI), arg1 (dword, compared with
   the global at 0x009CF6F8), arg2 (one byte: read as unsigned by cmp bl,3 and as signed
   by movsx). Local frame, one block with explicit offsets from its base F0:
     F0+0x02 count byte   F0+0x03 mode byte   F0+0x04..0xE3 scratch object (ECX)
     F0+0xE4..0x1AB pointer array (filled by 0x00541F90, scanned by the loop)
     F0+0x1AC..0x1B4 SEH record {prev, handler 0x0096299B, state}.
   No direct caller reads EAX after the call, so the routine is void. */
#include "cmre.h"

#define FLD_U8(base, off)  (*(volatile u8 *)((u32)(base) + (u32)(off)))
#define FLD_U32(base, off) (*(volatile u32 *)((u32)(base) + (u32)(off)))

extern u32 cm_fn_00541f90(int param_1,u32 *param_2,int param_3);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self,u32 arg1,u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self,u32 arg1,u32 arg2);

void CM_STDCALL cm_fn_0085b020(u32 arg0, u32 arg1, u8 arg2)
{
    struct {
        u8 pad[2];
        u8 count;                         /* F0+0x02 (bl after the scan, stored at 0x0085B092) */
        u8 mode;                          /* F0+0x03 */
        u8 obj[0xe0];                     /* F0+0x04: object for 0x0077BBE0 / 0x00785790 */
        u32 arr[50];                      /* F0+0xe4: filled by 0x00541F90 */
        volatile struct { u32 prev; u32 handler; s32 state; } seh;   /* F0+0x1ac */
    } f;
    u32 entry_prev;
    u32 p;
    u32 v;
    u32 esi_v;
    u32 g2;
    s32 n;
    s32 t;
    s32 k;
    u8 cnt;
    u8 b;

    entry_prev = CM_FS(u32, 0);           /* mov eax, fs:[0] */
    f.seh.state = -1;
    f.seh.handler = 0x0096299bu;
    f.seh.prev = entry_prev;
    CM_FS(u32, 0) = (u32)&f.seh;

    cnt = 0;                              /* xor bl, bl */
    f.count = cnt;                        /* mov [F0+2], bl */
    f.mode = 0x19;                        /* mov [F0+3], 0x19 */
    cm_fn_00541f90((int)arg0, f.arr, 0);
    n = CM_GLOBAL(volatile s32, 0x00ae2364);   /* mov esi, [0xae2364] */

    cnt = 0;
    for (k = 0; k < 50; k++) {
        p = f.arr[k];                     /* mov eax, [ecx] */
        if (p == 0) {
            continue;
        }
        if ((s32)FLD_U32(p, 0) >= n) {    /* cmp dword [eax], esi; jge */
            continue;
        }
        b = FLD_U8(p, 0x6d);              /* mov al, byte [eax+0x6d] */
        if (b == 0) {
            continue;
        }
        if ((b & 0x18u) != 0) {
            continue;
        }
        cnt = (u8)(cnt + 1);              /* inc bl */
    }
    f.count = cnt;                        /* mov [esp+0xe], bl */

    /* mov bl, arg2 (no flag change); cmp bl, 0x19 happens above. */
    if (cnt == 0x19) {
        if (arg2 != 3) {
            goto exit;                    /* jne 0x0085B16C */
        }
        goto mode_update;
    }
    if (arg2 == 3) {
mode_update:
        t = (s32)(s8)f.count + 2;         /* movsx eax, byte [esp+0xe]; add eax, 2 */
        f.mode = 0x19;                    /* mov byte [esp+0xf], 0x19 */
        if (t <= 0x19) {
            f.mode = (u8)t;               /* mov byte [esp+0xf], al */
        }
    }

    esi_v = arg1;                         /* mov esi, [esp+0x1cc] */
    g2 = CM_GLOBAL(volatile u32, 0x009cf6f8);
    if (esi_v == g2) {
        esi_v = (u32)CM_GLOBAL(volatile s32, 0x009cf6f0);
    }

    cm_fn_0077bbe0((void *)f.obj, 0x19u, 0u);

    v = FLD_U32(arg0, 0);                 /* mov ecx, [edi] */
    f.seh.state = 0;                      /* mov dword [esp+0x1c0], 0 */
    cm_fn_00785790((void *)f.obj, 0u, v);

    cm_fn_00785790((void *)f.obj, 1u, esi_v);

    cm_fn_00785790((void *)f.obj, 2u, (u32)(s32)(s8)arg2);   /* movsx edx, bl */

    cm_fn_00785790((void *)f.obj, 3u, 0u);

    cm_fn_00785790((void *)f.obj, 4u, (u32)(s32)(s8)f.mode);  /* movsx eax, byte [esp+0xf] */

    cm_fn_00785790((void *)f.obj, 5u, (u32)(s32)(s8)f.count); /* movsx ecx, byte [esp+0xe] */

    cm_fn_00785790((void *)f.obj, 6u, 0u);

    cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431c), (u32)f.obj, arg0);

exit:
    CM_FS(u32, 0) = f.seh.prev;           /* mov ecx, [esp+0x1b8]; mov fs:[0], ecx */
}
