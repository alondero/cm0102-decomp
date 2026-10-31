#include "cmre.h"

/* Original word at a1 (mov ax, word ptr [reg]); volatile keeps each read in listing order. */
#define OBJ_U16(base, off) (*(volatile u16 *)((u32)(base) + (off)))

/* One stack argument (push 3), cdecl; returns int in EAX, tested for zero. */
extern int CM_CDECL cm_fn_0090cfc0(u32 arg);

/* Stack arguments by entry offset: a1 = [esp+4], a2 = [esp+8], a3 = [esp+12].
   ECX is only saved and restored by the original (push ecx / pop ecx); the
   frame byte at [esp+0x0b] after the saves is a local flag, kept here as
   `flag`. Returns AX only (u16); every exit writes AX. */
u32 CM_THISCALL cm_fn_006d5d80(u32 ecx_entry, u32 a1, u32 a2, u32 a3)
{
    u8 flag;
    u8 al;
    u8 low_a2;
    u8 high_a2;
    u16 w;
    u32 hi;

    (void)ecx_entry;

    flag = 0;
    /* EAX bits 16-31 on exits that write only AX: zero after the first call
       returned 0, or the second call's result on the 0x20 path. */
    hi = 0;
    if (cm_fn_0090cfc0(3) != 0) {
        /* mov eax, [a1]; mov ax, [eax]: EAX keeps a1's high word. */
        return (a1 & 0xffff0000u) | OBJ_U16(a1, 0);
    }

    low_a2 = (u8)a2;
    high_a2 = (u8)(a2 >> 8);
    if (low_a2 == 4 || low_a2 == 3 || low_a2 == 2) {
        flag = 1;
    }

    al = (u8)a3;
    if (al == 1) {
        w = OBJ_U16(a1, 0);
        if (w == 0x216f) return 0x2199;
        if (w == 0x2150) return 0x2197;
        if (w == 0x2112) return 0x2195;
        if (w == 0x1fee) return 0x218d;
        if (w == 0x1f49) return 0x2182;
        if (w == 0x205a) return 0x2190;
        if (w == 0x205b) return 0x2191;
        if (w == 0x2079) return 0x2192;
        goto tail;
    } else if (al == 2) {
        w = OBJ_U16(a1, 0);
        if (w == 0x216f) return 0x2199;
        if (w == 0x2150) return 0x2197;
        if (w == 0x2112) return 0x2195;
        if (w == 0x1fe6) return 0x2193;
        if (w == 0x1fee) return 0x218d;
        if (w == 0x1f48) return 0x2181;
        if (w == 0x1f41) return 0x2180;
        if (w == 0x1f44) return 0x2187;
        if (w == 0x1f86) return 0x2189;
        if (w == 0x1fa2) return 0x218a;
        goto tail;
    } else if (al == 8) {
        w = OBJ_U16(a1, 0);
        if (w == 0x216f) return 0x2199;
        if (w == 0x2150) return 0x2197;
        if (w == 0x2112) return 0x2195;
        if (w == 0x1fe6) return 0x2193;
        if (w == 0x1fee) return 0x218d;
        if (w == 0x205a) return 0x218e;
        if (w == 0x205b) return 0x218f;
        if (w == 0x1f4b) return 0x2183;
        if (w == 0x1f41) return 0x2184;
        if (w == 0x1f4a) return 0x2188;
        goto tail;
    } else if (al == 0x10) {
        if (OBJ_U16(a1, 0) == 0x1f48) {
            return 0x2185;
        }
        goto tail;
    } else if (al == 0x20) {
        {
            u32 res = (u32)cm_fn_0090cfc0(3);

            if (res != 0) {
                hi = res & 0xffff0000u;
                goto tail;
            }
        }
        w = OBJ_U16(a1, 0);
        if (w == 0x216f) return 0x2199;
        if (w == 0x1fee) return 0x218d;
        if (w == 0x1f42) return 0x217c;
        if (w == 0x1f4b) return 0x217d;
        if (w == 0x1f48) return 0x2186;
        goto tail;
    } else {
        goto tail;
    }

tail:
    if (flag != 0) {
        w = OBJ_U16(a1, 0);
        if (w == 0x2112) return hi | 0x2196u;
        if (w == 0x1fa6) return hi | 0x2194u;
        if (w == 0x1f8b) return hi | 0x218bu;
        if (w == 0x1fbf) return hi | 0x218cu;
        if (w == 0x211f) return hi | 0x217fu;
    }

    if (high_a2 == 3 || high_a2 == 4 || high_a2 == 5) {
        if (OBJ_U16(a1, 0) == 0x1f4b) {
            return hi | 0x217eu;
        }
    }
    return hi | OBJ_U16(a1, 0);
}
