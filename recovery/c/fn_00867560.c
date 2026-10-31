/* Preferred-image VA 00867560, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00866f60(u32 self);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self, u32 person, u32 option);
extern u32 CM_THISCALL cm_fn_008c1870(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_004d8610(u32 self, u32 record);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00867560(u32 self, u32 flag)
{
    u8 work[0x200];
    u32 id, bits, base, person, result, receiver, pointer, version;
    u8 byte;
    if ((s32)flag > 0x1000) {
        cm_fn_00944cff(0x00a8360cu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1eeeu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    }
    id = D(self);
    bits = D(self + 0x40u);
    D(self + 0x40u) = bits | flag;
    base = D(0x00ae23bcu);
    byte = B(base + id * 110u + 0x3du);
    result = (base & 0xffffff00u) | byte;
    if (byte == 12 || byte == 13 || byte == 15)
        return result;
    byte = B(self + 0x44u);
    result = (result & 0xffffff00u) | byte;
    if (byte == 0 || (s8)byte < 0)
        return result;
    byte = B(self + 0x49u);
    result = (result & 0xffffff00u) | byte;
    if (byte != 0)
        return result;
    result = cm_fn_00866f60(self);
    if (result == 0)
        return result;
    id = D(self);
    base = D(0x00ae23bcu);
    person = base + id * 110u;
    receiver = D(0x00de1f54u);
    result = cm_fn_008cca10(receiver, person, 0);
    if (result != 0)
        return result;
    if ((B(self + 0x4eu) & 9u) != 0)
        return result;
    id = D(self);
    base = D(0x00ae23bcu);
    person = base + id * 110u;
    receiver = D(0x00de1f54u);
    result = cm_fn_008c1870(receiver, person);
    if (result != 0)
        return result;
    id = D(self);
    base = D(0x00ae23bcu);
    person = base + id * 110u;
    if ((s8)B(person + 0x18u) <= 20) {
        pointer = D(person + 0x61u);
        if ((s16)W(pointer + 11u) <= 0x109a)
            return person;
    }
    receiver = D(0x00ae1050u);
    return cm_fn_004d8610(receiver, self);
}
