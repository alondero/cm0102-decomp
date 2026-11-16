/* fn_009349a0 - CM 01/02 3.9.68, preferred-image VA 0x009349a0.
 *
 * Faithful plain-C translation of the original listing (bundle brief).
 *
 * Stack frame (S = entry esp, return address at [S]):
 *   009349a0 sub esp,0xc      -> esp = S-0x0c
 *   009349a3 push ebx         -> esp = S-0x10
 *   009349a4 push ebp         -> esp = S-0x14
 *   009349a8 cmp word [esp+0x1c],0 -> [S+8]  arg2 read as a WORD
 *   009349ae push esi         -> esp = S-0x18
 *   009349af push edi         -> esp = S-0x1c
 *   [esp+0x10] = S-0x0c local  best distance  (init 0x7fff)
 *   [esp+0x14] = S-0x08 local  best index     (init -1)
 *   [esp+0x18] = S-0x04 local  word -1        (read as AX on the not-found path)
 *   [esp+0x20] = S+0x04 arg1 dword  table base
 *   [esp+0x28] = S+0x10 arg4 word   selector for table B (base+0x3a2)
 *   [esp+0x30] = S+0x14 arg5 dword  value matched against the entries
 * arg3 (S+0x0c) is never read.  `ret 0` -> cdecl, the caller pops the args.
 *
 * Return width: EAX. The not-found paths leave 0xFFFFFFFF (-1): EAX is -1
 * from `or eax,0xffffffff` / `mov eax,[esp+0x14]`, and `mov ax,word
 * [esp+0x18]` rewrites only the low half of that -1.
 *
 * Table entries are raw 16-bit words.  `mov dx,cx` clears the upper half of
 * EDX and `sar dx,1` is the 16-bit form, so the halving treats the word as a
 * signed 16-bit value; `movsx edx,si` / `movsx edx,di` then sign-extend the
 * 16-bit halves.  The explicit u16 truncation below reproduces that.
 *
 * The table word is read through a volatile u16 so the access width, the
 * access order and a fault on an unmapped page boundary match the original.
 */
#include "cmre.h"

s32 CM_CDECL cm_fn_009349a0(u32 base, s16 flag_a, u32 unused3, s16 flag_b,
                             s32 value)
{
    s32 best_index = -1;        /* [esp+0x14] */
    s32 best_distance = 0x7fff; /* [esp+0x10] */
    u32 ebx;
    u32 addr;

    (void)unused3;

    if (flag_a != 0) {
        /* 009349c2..009349cc : eax = base + 0x38a */
        addr = base + 0x38a;
        for (ebx = 0; ebx < 0xc; ++ebx) {
            u32 v;      /* ecx : mov cx, word ptr [eax] */
            u16 d16;    /* edx : mov dx,cx ; sar dx,1 (16-bit form) */
            u32 esi;    /* esi : sub esi, edx */
            u32 edi;    /* edi : lea edi, [edx + ecx] */
            s32 delta;  /* ecx : (s16)(cx - ebp) */
            s32 dist;   /* ecx : |delta| */
            s32 lower;  /* edx : movsx edx, si */
            s32 upper;  /* edx : movsx edx, di */

            v = *(volatile u16 *)(u32)addr;
            addr += 2;

            if (v == 0)
                continue; /* 009349d4 / 009349d7 */

            d16 = (u16)((s16)v >> 1);
            esi = v - (u32)d16;
            edi = (u32)d16 + v;

            delta = (s16)(u16)(v - (u32)value);
            if (delta < 0)
                delta = -delta;
            dist = delta;

            if (dist >= best_distance)
                continue; /* 009349f4 / 009349f8 : cmp ecx,[esp+0x10] / jge */

            lower = (s16)(u16)esi;
            if (value < lower)
                continue; /* 009349fa..009349ff : movsx edx,si / cmp ebp,edx / jl */

            upper = (s16)(u16)edi;
            if (value > upper)
                continue; /* 00934a01..00934a06 : movsx edx,di / cmp ebp,edx / jg */

            best_distance = dist; /* 00934a08 */
            best_index = (s32)ebx; /* 00934a0c */
        }
        if (best_index >= 0)
            return best_index + 0x48; /* 00934a24 */
    } else if (flag_b != 0) {
        /* 00934a34..00934a3e : eax = base + 0x3a2 */
        addr = base + 0x3a2;
        for (ebx = 0; ebx < 0xc; ++ebx) {
            u32 v;
            u16 d16;
            u32 esi;
            u32 edi;
            s32 delta;
            s32 dist;
            s32 lower;
            s32 upper;

            v = *(volatile u16 *)(u32)addr;
            addr += 2;

            if (v == 0)
                continue; /* 00934a46 / 00934a49 */

            d16 = (u16)((s16)v >> 1);
            esi = v - (u32)d16;
            edi = (u32)d16 + v;

            delta = (s16)(u16)(v - (u32)value);
            if (delta < 0)
                delta = -delta;
            dist = delta;

            if (dist >= best_distance)
                continue; /* 00934a66 / 00934a6a */

            lower = (s16)(u16)esi;
            if (value < lower)
                continue; /* 00934a6c..00934a71 */

            upper = (s16)(u16)edi;
            if (value > upper)
                continue; /* 00934a73..00934a78 */

            best_distance = dist; /* 00934a7a */
            best_index = (s32)ebx; /* 00934a7e */
        }
        if (best_index >= 0)
            return best_index + 0x54; /* 00934a96 */
    }
    /* 00934a9e / 00934aa3 : EAX = 0xffffffff */
    return -1;
}
