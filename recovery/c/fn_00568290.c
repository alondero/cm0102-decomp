#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_0077ac30(u32, u32, u32, u32);

/* Preferred-image VA 00568290, pinned 3.9.68 image.  The callers retain
   upper EAX or return it unchanged, so the return is a full DWORD.
   0077ac30's entry EAX high word reaches only socket-id arguments;
   008ae8ee consumes these as WORDs. */
u32 CM_STDCALL cm_fn_00568290(u32 self, u32 side, u8 kind)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 capacity, count, pointer, text, result;
    capacity = D(0x00ae2900);
    D(0x00ae28fc) = 0;
    if (capacity < 1) {
        cm_fn_00944cff(0x009c3734, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x100, local, 0x85);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        pointer = D(0x00ae28f8);
        B(pointer) = 5;
        count = D(0x00ae28fc);
        D(0x00ae28fc) = count + 1;
    }
    count = D(0x00ae28fc);
    capacity = D(0x00ae2900);
    if (count + 1 > capacity) {
        cm_fn_00944cff(0x009c3734, 0, 0, local, local + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local, local + 0x100, 0x85);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    } else {
        pointer = D(0x00ae28f8);
        B(count + pointer) = kind;
        count = D(0x00ae28fc);
        D(0x00ae28fc) = count + 1;
    }
    count = D(0x00ae28fc);
    pointer = D(0x00ae28f8);
    result = cm_fn_0077ac30(self, pointer, count, side);
    if (result == 0) {
        cm_fn_00944cff(0x009c36ec, 0, 0, local, local + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local, local + 0x100, 0xc52);
        cm_fn_0066f4e0(0x00ae28bc, 0x009c3814);
        cm_fn_0066f4e0(0x00ae26c4, 0x009c37b0, 0x00ae24d0);
        result = cm_fn_005e8290(0x00ae28bc, 0x00ae26c4, 0);
    }
    return result;
}
