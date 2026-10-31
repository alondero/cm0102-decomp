#include "cmre.h"

/* Preferred-image VA 007e82e0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   55cae0/55cb20/55cb60 read their category arguments as BYTEs only;
   incoming unknown high bits of the partially written registers are dead.
   Original allocation, lookup and update callees remain boundaries. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0055cae0(u32, u32, u8, u8);
extern u32 CM_THISCALL cm_fn_0055cb60(u32, u32, u8);
extern void CM_THISCALL cm_fn_0055cb20(u32, u32, u8, u8, u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00945667(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void diagnostic(u32 name, u32 extension, u32 line)
{
    u32 version;
    cm_fn_00944cff(0x00a7a858, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, volatile u32 create)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, manager, index, found, capacity, used, data;
    u32 address, size;
    u8 category, version, found_version;
    version = B(self + 5);
    category = B(self + 4);
    manager = D(0x00ae28f0);
    index = cm_fn_0055cae0(manager, key, category, version);
    if (index == 0xffffffffu) {
        if (create == 0) return 0;
        category = B(self + 4);
        manager = D(0x00ae28f0);
        found = cm_fn_0055cb60(manager, key, category);
        if (found != 0) {
            found_version = B(found + 5);
            version = B(self + 5);
            if (found_version == version)
                diagnostic(local + 0x100, local, 0x295);
        }
        capacity = D(self + 0xf);
        used = D(self + 0x13);
        if (used == capacity) {
            data = D(self + 0xb);
            capacity += 0x64;
            D(self + 0xf) = capacity;
            size = capacity * 14;
            if (data != 0) data = cm_fn_00945667(data, size);
            else data = cm_fn_00944e46(size);
            D(self + 0xb) = data;
            if (data == 0) {
                diagnostic(local, local + 0x100, 0x2b0);
                return 0;
            }
        }
        used = D(self + 0x13);
        data = D(self + 0xb);
        address = data + used * 14;
        D(address) = 0;
        D(address + 4) = 0;
        D(address + 8) = 0;
        W(address + 12) = 0;
        used = D(self + 0x13);
        data = D(self + 0xb);
        W(data + used * 14 + 6) = 0xffff;
        used = D(self + 0x13);
        data = D(self + 0xb);
        D(data + used * 14) = key;
        index = D(self + 0x13);
        version = B(self + 5);
        category = B(self + 4);
        D(self + 0x13) = index + 1;
        manager = D(0x00ae28f0);
        cm_fn_0055cb20(manager, key, category, version, index);
    }
    capacity = D(self + 0xf);
    if ((s32)index < (s32)capacity && (s32)index >= 0) {
        data = D(self + 0xb);
        return data + index * 14;
    }
    version = B(self + 5);
    category = B(self + 4);
    manager = D(0x00ae28f0);
    cm_fn_0055cb20(manager, key, category, version, 0xffffffffu);
    return 0;
}
