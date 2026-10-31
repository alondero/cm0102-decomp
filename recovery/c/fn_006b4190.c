#include "cmre.h"

extern void CM_FASTCALL cm_fn_00453bc0(int param_1);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern void CM_FASTCALL cm_fn_006b97a0(int param_1);
extern void CM_THISCALL cm_fn_0090d6b0(int param_1, int param_2);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value. No SEH
   frame. The global at 0x00b67dc8 is released through 0x68d040 (fastcall) and
   0x944c94, then zeroed, when non-zero. Field offsets from self: 0x01, 0x05,
   0x09 are dwords released through 0x9452ca (the release zeroes 0x01, 0x05 and
   0x09 together); 0x0d, 0x11, 0x15 likewise. The record array pointer is at
   0x19 and its count (signed dword) at 0x21; records are 0x69 bytes apart.
   Each record field 0x57 is released through 0x6b97a0 (fastcall) and 0x944c94,
   zeroed, and passed with record+4 to thiscall 0x90d6b0 on the global at
   0x00de2f70; fields 0x5b and 0x5f are released through 0x944c94 and
   0x453bc0 (fastcall). The array pointer and its two dwords (0x1d, 0x21) are
   then released (0x19 zeroed; 0x1d and 0x21 zeroed). The record loop runs
   while the index is below the count, after one increment, so it is skipped
   when the count is not above zero. */
void CM_THISCALL cm_fn_006b4190(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 g;
    u32 value;
    u32 esi_v;
    u32 ebx_v;
    u32 j;
    s32 i;
    s32 count;

    g = *(volatile u32 *)0x00b67dc8;
    if (g != 0) {
        cm_fn_0068d040((int)g);
        cm_fn_00944c94((void *)g);
        *(volatile u32 *)0x00b67dc8 = 0;
    }

    value = *(volatile u32 *)(base + 0x01);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 0x01) = 0;
        *(volatile u32 *)(base + 0x05) = 0;
        *(volatile u32 *)(base + 0x09) = 0;
    }

    value = *(volatile u32 *)(base + 0x0d);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 0x0d) = 0;
        *(volatile u32 *)(base + 0x11) = 0;
        *(volatile u32 *)(base + 0x15) = 0;
    }

    if (*(volatile u32 *)(base + 0x19) != 0) {
        count = *(volatile s32 *)(base + 0x21);
        i = 0;
        if (count > 0) {
            j = 0;
            do {
                esi_v = *(volatile u32 *)(base + 0x19) + j;
                ebx_v = *(volatile u32 *)(esi_v + 0x57);
                if (ebx_v != 0) {
                    cm_fn_006b97a0((int)ebx_v);
                    cm_fn_00944c94((void *)ebx_v);
                    *(volatile u32 *)(esi_v + 0x57) = 0;
                    cm_fn_0090d6b0((int)*(volatile u32 *)0x00de2f70, (int)(esi_v + 4));
                }
                value = *(volatile u32 *)(esi_v + 0x5b);
                if (value != 0) {
                    cm_fn_00944c94((void *)value);
                    *(volatile u32 *)(esi_v + 0x5b) = 0;
                }
                ebx_v = *(volatile u32 *)(esi_v + 0x5f);
                if (ebx_v != 0) {
                    cm_fn_00453bc0((int)ebx_v);
                    cm_fn_00944c94((void *)ebx_v);
                    *(volatile u32 *)(esi_v + 0x5f) = 0;
                }
                j += 0x69u;
                i++;
                count = *(volatile s32 *)(base + 0x21);
            } while (i < count);
        }
        value = *(volatile u32 *)(base + 0x19);
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 0x19) = 0;
        *(volatile u32 *)(base + 0x1d) = 0;
        *(volatile u32 *)(base + 0x21) = 0;
    }
}
