/* Preferred-image VA00941480, base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   regparm(1) carries actual incoming EAX for the null-input return. One
   original cdecl stack argument, RET0. Original 009445d0 remains a boundary. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_009445d0(u32 manager, u32 value);

u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00941480(u32 eax_in, u32 self)
{
    u32 eax, manager, value, table, index;
    if (self == 0) return eax_in;

    eax = D(self + 0x1cu);
    if (eax != 0) {
        manager = D(self + 0x2222u);
        if (eax == D(manager + 0x64u)) D(manager + 0x94u) = 1;
        else cm_fn_009445d0(manager, eax);
    }

    eax = D(self + 0x21f8u);
    index = 1;
    if ((s32)eax >= 1) {
        do {
            table = D(self + 0x21f0u);
            manager = D(self + 0x2222u);
            value = D(table + index * 4u);
            cm_fn_009445d0(manager, value);
            eax = D(self + 0x21f8u);
            ++index;
        } while ((s32)index <= (s32)eax);
    }

    eax = D(self + 0x21f0u);
    if (eax != self + 0x2170u) {
        manager = D(self + 0x2222u);
        cm_fn_009445d0(manager, eax);
    }

    eax = D(self + 0x15cu);
    D(self + 0x21f4u) = 0;
    if (eax != self + 0xd8u) {
        manager = D(self + 0x2222u);
        cm_fn_009445d0(manager, eax);
    }

    eax = D(self + 0xccu);
    if (eax != self + 0x48u) {
        manager = D(self + 0x2222u);
        cm_fn_009445d0(manager, eax);
    }

    eax = D(self + 0x2222u);
    if (self == D(eax + 0x60u)) {
        D(eax + 0x90u) = 1;
        return eax;
    }
    return cm_fn_009445d0(eax, self);
}
