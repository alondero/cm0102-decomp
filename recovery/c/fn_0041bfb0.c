#include "cmre.h"

extern u8 *CM_FASTCALL cm_fn_007fee80(int param_1);
extern void CM_THISCALL cm_fn_007feeb0(int param_1, char *param_2);
extern u32 *CM_CDECL cm_fn_0090bde0(u32 param_1, u32 param_2);
extern int CM_CDECL cm_fn_0090cfc0(int param_1);

/* Cdecl, no stack arguments, `ret 0`, no return value. The saved ECX slot is
   reused as the loop counter c (set to 0 at entry; ECX itself is not read).
   Two calls to 0x7fee80 (fastcall, ECX = 0xb74450) return a string pointer:
   when its first byte is not a backslash, the second string's first byte must
   be zero or the routine returns. When the global at 0x00989858 is non-zero it
   searches up to six slots (records of 10 bytes at 0x00989866 with a signed
   word at 0x00989864 - 2 + ...; see the listing), starting from index
   r = 0x90cfc0(6), advancing the index (wrapping to 0 at 6) for each counter
   step while c < 6. A match checks a dword table entry (record pointer + 16 *
   signed helper result, +8, then indexed by a second helper result) being
   non-zero; a match calls 0x90bde0(0x9899e0, entry) and passes the result to the
   0x7feeb0 thiscall on 0xb74450. If nothing matches, 0x7feeb0 is called with
   0x00989adc. */
void CM_CDECL cm_fn_0041bfb0(void)
{
    u32 c;
    s32 bx;
    s32 idx;
    u32 esi_v;
    u32 p;
    s32 t;
    s32 r2;
    s32 r2s;
    u32 edi_v;
    u32 q;
    u32 rarr;
    s32 t3;
    s32 r3;
    s32 r3s;
    u32 value;
    u32 res;
    u8 *s;
    int found;
    s32 found_bx;
    s32 found_r2s;
    s32 found_r3s;

    c = 0;
    found = 0;
    bx = 0;

    s = cm_fn_007fee80(0x00b74450);
    if (*s != 0x5c) {
        s = cm_fn_007fee80(0x00b74450);
        if (*s != 0)
            return;
    }

    if (*(volatile s32 *)0x00989858 == 0)
        goto notfound;

    bx = cm_fn_0090cfc0(6);
    for (;;) {
        idx = (s16)bx;
        esi_v = (u32)idx * 10u;
        p = *(volatile u32 *)(0x00989866u + esi_v);
        if (p != 0 && (s16)bx != 2) {
            t = (s16)*(volatile s16 *)(0x00989864u + esi_v);
            r2 = cm_fn_0090cfc0(t);
            r2s = (s16)r2;
            edi_v = (u32)r2s << 4;
            q = p + edi_v;
            rarr = *(volatile u32 *)(p + edi_v + 8u);
            if (rarr != 0) {
                t3 = (s16)*(volatile s16 *)(q + 4u);
                r3 = cm_fn_0090cfc0(t3);
                r3s = (s16)r3;
                if (*(volatile u32 *)(rarr + (u32)r3s * 4u) != 0) {
                    found = 1;
                    found_bx = bx;
                    found_r2s = r2s;
                    found_r3s = r3s;
                    break;
                }
            }
        }
        c++;
        bx = (s16)(bx + 1);
        if (!((s16)bx < 6))
            bx = 0;
        if (!((s16)c < 6))
            break;
    }

    if (found) {
        /* The match path makes no further helper calls: it reuses the index and
           the two helper results (r2s, r3s) from the search pass. */
        idx = (s16)found_bx;
        esi_v = (u32)idx * 10u;
        p = *(volatile u32 *)(0x00989866u + esi_v);
        edi_v = (u32)found_r2s << 4;
        rarr = *(volatile u32 *)(p + edi_v + 8u);
        value = *(volatile u32 *)(rarr + (u32)found_r3s * 4u);
        res = (u32)cm_fn_0090bde0(0x009899e0, value);
        cm_fn_007feeb0(0x00b74450, (char *)res);
        return;
    }

notfound:
    cm_fn_007feeb0(0x00b74450, (char *)0x00989adc);
}
