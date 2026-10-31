#include "cmre.h"

/* Stdcall with two stack arguments (ret 8); ECX is written before it is read,
   so there is no entry receiver. Returns a byte (al).

   arg1 (ebx) is a record pointer P; arg2 (edi) is a pointer Q whose dword is
   compared with the dword at record+4 of each candidate. The fast path returns
   the byte at candidate+0x3a; otherwise 0x004cf6d0 is tried. Both candidate
   lookups are THISCALL: the receiver (ECX) is the dword at 0x00ae1050 and P is
   the stack argument.

   Helper 0x00540a50 is cdecl, called as (Q as u32, 0, 1); its EAX result is
   compared with the index value G3 + candidate+4 times 0x245 (581), where G3 is
   the dword at 0x00ae23b4. The first global compare with *P is signed (jl). */

extern int CM_THISCALL cm_fn_004cf5a0(int *param_1, int *param_2);
extern int CM_THISCALL cm_fn_004cf6d0(int *param_1, int *param_2);
extern u32 cm_fn_0053b620(char param_1);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);

u8 CM_STDCALL cm_fn_0085a760(int *param_1, int *param_2)
{
    u32 n;
    u32 g;
    u32 ecx;
    u32 cand;
    u32 field4;
    u32 qval;
    u32 r;
    u32 idx;
    u8 dl;

    if (param_1 == (int *)0) {
        return 0;
    }
    if (param_2 == (int *)0) {
        return 0;
    }
    n = *(volatile u32 *)param_1;
    g = CM_GLOBAL(volatile u32, 0x00ae2364);
    if ((s32)n >= (s32)g) {
        ecx = *(volatile u32 *)((u8 *)param_1 + 0x61);
        return *(volatile u8 *)(ecx + 4);
    }

    /* First candidate: receiver = dword at 0x00ae1050, argument = P. */
    ecx = CM_GLOBAL(volatile u32, 0x00ae1050);
    cand = (u32)cm_fn_004cf5a0((int *)ecx, param_1);
    if (cand != 0) {
        dl = *(volatile u8 *)((u8 *)param_1 + 0x3d);
        r = cm_fn_0053b620((char)dl);
        if (r != 0) {
            field4 = *(volatile u32 *)(cand + 4);
            qval = *(volatile u32 *)param_2;
            if (field4 == qval) {
                return *(volatile u8 *)(cand + 0x3a);
            }
            r = cm_fn_00540a50((u32)param_2, 0, 1);
            ecx = *(volatile u32 *)(cand + 4);
            idx = CM_GLOBAL(volatile u32, 0x00ae23b4) + ecx * 0x245u;
            if (idx == r) {
                return *(volatile u8 *)(cand + 0x3a);
            }
        }
    }

    /* Second candidate: receiver = dword at 0x00ae1050 again, argument = P. */
    ecx = CM_GLOBAL(volatile u32, 0x00ae1050);
    cand = (u32)cm_fn_004cf6d0((int *)ecx, param_1);
    if (cand == 0) {
        return 0;
    }
    field4 = *(volatile u32 *)(cand + 4);
    qval = *(volatile u32 *)param_2;
    if (field4 == qval) {
        return *(volatile u8 *)(cand + 0x3a);
    }
    r = cm_fn_00540a50((u32)param_2, 0, 1);
    ecx = *(volatile u32 *)(cand + 4);
    idx = CM_GLOBAL(volatile u32, 0x00ae23b4) + ecx * 0x245u;
    if (idx == r) {
        return *(volatile u8 *)(cand + 0x3a);
    }
    return 0;
}
