#include "cmre.h"

/* Original callee 0x0055fba0 is thiscall: `this` in ECX, 11 stack DWORD
   arguments, callee cleans 0x2c bytes (ret 0x2c). */
extern u32 CM_THISCALL cm_fn_0055fba0(void *self, u32 a1, u32 a2, u32 a3,
                                      u32 a4, u32 a5, u32 a6, u32 a7,
                                      u32 a8, u32 a9, u32 a10, u32 a11);

/* 0x004567f0: thiscall, `this` in ECX, 11 stack DWORD arguments, ret 0x2c.
   Pushes the eleven incoming arguments DWORD-by-DWORD in reverse order, then
   forwards them to 0x0055fba0 with a computed `this`. */
u32 CM_THISCALL cm_fn_004567f0(void *self, u32 a1, u32 a2, u32 a3, u32 a4,
                               u32 a5, u32 a6, u32 a7, u32 a8, u32 a9,
                               u32 a10, u32 a11)
{
    /* 00456827 movsx eax, word ptr [ecx + 0x382a] */
    s32 idx = (s16)*(s16 *)((u8 *)self + 0x382a);
    /* 0045682e lea edx, [eax + eax*2] ; 00456831 shl edx, 8 */
    u32 off = (u32)idx * 768u;
    /* 00456834 mov ecx, dword ptr [edx + ecx + 0x7fc] */
    void *inner = (void *)*(u32 *)((u8 *)self + off + 0x7fc);
    /* 0045683b call 0x0055fba0 */
    return cm_fn_0055fba0(inner, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}
