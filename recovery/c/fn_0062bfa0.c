#include "cmre.h"

/* Original routine 0x0062bfa0 (preferred-image VA, image base 0x00400000).
   Convention: thiscall (ECX = this, read before written), ret 4, one stack
   argument idx ([esp+0x1c] after the four pushes, that is the first argument).
   Returns EAX = address of the byte at base + 79*idx + 0x0c, where base is the
   dword global at 0x00ae2d0c; the byte's bit 0 is cleared in place.

   Frame: the entry `sub esp, 8` reserves 8 bytes (lb below) that are passed to
   callee 0x00417c10 as its first stack argument. No other stack data is read.

   Records: this+0x28 is a pointer (ptr); the element pointer from this+0 plus
   31*idx is esi in the listing. Callee 0x0068d1b0 is fastcall (ECX = argument)
   and returns EAX; the walk loop follows the pointer chain until it returns 0.
   Return values of 0x0062e700, 0x00631f90, 0x0068d1f0 and 0x00417c10 are
   discarded by the listing (EAX is overwritten or unread before use). */

extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 value);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 target);
extern void CM_THISCALL cm_fn_0062e700(u32 self, u32 p, u32 v);
extern u32 CM_THISCALL cm_fn_00631f90(u32 self, u32 p);
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 p, u32 q);

CM_THISCALL u32 cm_fn_0062bfa0(u32 self, u32 idx)
{
    u8 lb[8];
    u32 psvar;
    u32 esi;
    u32 eax;
    u32 ecx;
    u32 edx;
    u8 cl;

    /* 0x0062bfad */
    eax = *(volatile u32 *)(self + 0x28u);
    if (eax != 0u) {
        /* mov ecx, [eax+2]; mov [eax+0xa], ecx */
        ecx = *(volatile u32 *)(eax + 2u);
        *(volatile u32 *)(eax + 0xau) = ecx;
        ecx = *(volatile u32 *)(self + 0x28u);
        psvar = cm_fn_0068d1b0(ecx);
        if (psvar != 0u) {
            for (;;) {
                if (*(volatile u32 *)psvar == idx) {
                    ecx = *(volatile u32 *)(self + 0x28u);
                    cm_fn_0068d1f0(ecx, psvar);
                }
                ecx = *(volatile u32 *)(self + 0x28u);
                psvar = cm_fn_0068d1b0(ecx);
                if (psvar == 0u) {
                    break;
                }
            }
        }
    }

    /* 0x0062bfdf */
    ecx = *(volatile u32 *)self;
    esi = ecx + idx * 31u;
    if (*(volatile u32 *)(esi + 0xeu) != 0xffffffffu) {
        ecx = CM_GLOBAL(volatile u32, 0x00ae23bc);
        cm_fn_0062e700(self, ecx + idx * 110u, 0u);
    }

    /* 0x0062c00f */
    ecx = CM_GLOBAL(volatile u32, 0x00ae23bc);
    eax = ecx + idx * 110u;
    cm_fn_00631f90(self, eax);

    /* 0x0062c029 */
    cm_fn_00417c10(esi, (u32)lb, (u32)&CM_GLOBAL(volatile u32, 0x00ae2c90));

    /* 0x0062c03a */
    *(volatile u16 *)(esi + 0x8u) = 0x2710u;
    *(volatile u16 *)(esi + 0xau) = 0x2710u;
    *(volatile u16 *)(esi + 0xcu) = 0u;
    *(volatile u32 *)(esi + 0xeu) = 0xffffffffu;
    *(volatile u8 *)(esi + 0x12u) = 0xffu;
    *(volatile u32 *)(esi + 0x14u) = 0xffffffffu;
    *(volatile u32 *)(esi + 0x18u) = 0xffffffffu;
    *(volatile u8 *)(esi + 0x1du) = 0xffu;

    /* 0x0062c062: edx = 80*idx - idx = 79*idx */
    eax = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    edx = (idx * 80u) - idx;
    eax = edx + eax + 0xcu;
    cl = *(volatile u8 *)eax;
    cl = (u8)(cl & 0xfeu);
    *(volatile u8 *)eax = cl;
    return eax;
}
