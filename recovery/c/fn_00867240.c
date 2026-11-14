/* Preferred-image VA00867240, base00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native helper calls remain explicit. Partial AL and full EAX returns matter. */
#include "cmre.h"
#define R8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define R32(a) CM_GLOBAL(volatile u32, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_STDCALL cm_fn_004d9460(u32, u32);
/* The original caller forwards incidental native EAX on the final flag path. */
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00549d60(u32 entry_eax, u32 person, u32 flag);
extern u32 CM_THISCALL cm_fn_00866f60(u32);
extern u32 CM_THISCALL cm_fn_008c5450(u32, u32, u32);

u32 CM_THISCALL cm_fn_00867240(u32 self, volatile u32 argument)
{
    u32 index = R32(self);
    u32 base = R32(0x00ae23bcu);
    u32 person = base + index * 110u;
    u32 receiver = R32(0x00ae1050u);
    u32 eax = cm_fn_004cf5a0(receiver, person);
    R32(0x00ae1080u) = eax;
    u32 club = 0;
    if (eax != 0 && (R8(eax + 0x35u) & 0x3fu) != 5u) {
        index = R32(eax + 4u);
        base = R32(0x00ae23b4u);
        club = base + index * 581u;
    }
    index = R32(self);
    eax = index * 55u;
    base = R32(0x00ae23bcu);
    person = base + eax * 2u;
    if (club == 0)
        return eax;
    eax = R32(person + 0x61u);
    if (eax == 0)
        return eax;
    eax = R32(self + 4u);
    u32 other = R32(club);
    if (eax != other)
        return eax;
    if ((R8(self + 0x4eu) & 8u) == 0)
        return eax;
    u8 byte = R8(self + 0x49u);
    eax = (eax & 0xffffff00u) | byte;
    if (byte != 0)
        return eax;
    eax = cm_fn_00866f60(self);
    if (eax != 0)
        return eax;
    if ((R8(self + 0x4fu) & 0xf0u) == 0x70u)
        return eax;
    receiver = R32(0x00de1f54u);
    eax = cm_fn_008c5450(receiver, person, eax);
    if (eax != 0)
        return eax;
    other = argument;
    receiver = R32(0x00ae1050u);
    eax = cm_fn_004d9460(self, other);
    byte = R8(self + 0x4eu);
    byte &= 0xf7u;
    R8(self + 0x4eu) = byte;
    byte = R8(person + 0x5bu);
    byte = (u8)(byte + R8(person + 0x59u));
    byte = (u8)(byte + 5u);
    R8(self + 0x44u) = byte;
    byte = R8(self + 0x3eu);
    if (byte > 0)
        R8(self + 0x3eu) = 0;
    byte = R8(self + 0x3du);
    if ((s8)byte < 15)
        R8(self + 0x3du) = 15;

    /* Each source block reloads the flags after recursion and the table helper;
       neither the object index nor either global table pointer is cached. */
    eax = R32(self + 0x45u);
    if (eax & 0x10u) {
        R32(self + 0x45u) = eax & 0xffffffefu;
        cm_fn_00867240(self, eax);
        index = R32(self);
        base = R32(0x00ae23bcu);
        cm_fn_00549d60(index * 55u, base + index * 110u, 0x10u);
    }
    eax = R32(self + 0x45u);
    if (eax & 1u) {
        R32(self + 0x45u) = eax & 0xfffffffeu;
        cm_fn_00867240(self, eax);
        index = R32(self);
        base = R32(0x00ae23bcu);
        cm_fn_00549d60(index * 55u, base + index * 110u, 1u);
    }
    eax = R32(self + 0x45u);
    if (eax & 4u) {
        R32(self + 0x45u) = eax & 0xfffffffbu;
        cm_fn_00867240(self, eax);
        index = R32(self);
        base = R32(0x00ae23bcu);
        cm_fn_00549d60(index * 55u, base + index * 110u, 4u);
    }
    eax = R32(self + 0x45u);
    if (eax & 8u) {
        R32(self + 0x45u) = eax & 0xfffffff7u;
        cm_fn_00867240(self, eax);
        index = R32(self);
        base = R32(0x00ae23bcu);
        cm_fn_00549d60(index * 55u, base + index * 110u, 8u);
    }
    eax = R32(self + 0x45u);
    if (eax & 0x100u) {
        R32(self + 0x45u) = eax & 0xfffffeffu;
        cm_fn_00867240(self, eax);
        index = R32(self);
        base = R32(0x00ae23bcu);
        cm_fn_00549d60(index * 55u, base + index * 110u, 0x100u);
    }
    eax = R32(self + 0x45u);
    if (eax & 0x80u) {
        R32(self + 0x45u) = eax & 0xffffff7fu;
        cm_fn_00867240(self, eax);
        index = R32(self);
        base = R32(0x00ae23bcu);
        cm_fn_00549d60(index * 55u, base + index * 110u, 0x80u);
    }
    eax = R32(self + 0x45u);
    if (eax & 0x10000u) {
        R32(self + 0x45u) = eax & 0xfffeffffu;
        cm_fn_00867240(self, eax);
        index = R32(self);
        base = R32(0x00ae23bcu);
        eax = cm_fn_00549d60(index * 55u, base + index * 110u, 0x10000u);
    }
    return eax;
}

