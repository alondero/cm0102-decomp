/* Preferred-image VA 0x005F6340, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, esi = ecx), no stack arguments,
   ret 0. Initialises a block of byte, word and dword fields of the object,
   copies the int at global 0x009CF568 into object +0x1C, then calls the
   object's vtable slot at +0x3C with ECX = object and four stack arguments:
   -1, object+0xA9, object+0x3A, 0. The slot's result is stored at object +0xBA.
   Returns 1 (the listing's EBX, set to 1 before the call).
   Field meanings remain unknown. Object writes are volatile so -O2 keeps the
   listing's store order at a fault. */
#include "cmre.h"

typedef u32 (CM_THISCALL *slot_3c_fn)(u32 self, s32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_005f6340(u32 obj)
{
    s32 global_value;
    u32 vtable;
    u32 target;
    u32 result;

    *(volatile u16 *)(obj + 0x3c) = 2;
    *(volatile u8 *)(obj + 0xc4) = 2;
    *(volatile u8 *)(obj + 0xc6) = 2;
    *(volatile u8 *)(obj + 0xc2) = 3;
    *(volatile u8 *)(obj + 0xc3) = 1;
    *(volatile u8 *)(obj + 0x42) = 1;
    *(volatile u8 *)(obj + 0xc5) = 1;
    *(volatile u8 *)(obj + 0xc7) = 0;
    *(volatile u8 *)(obj + 0xbe) = 1;
    *(volatile u8 *)(obj + 0xbf) = 0;
    *(volatile u8 *)(obj + 0xc0) = 0;
    *(volatile u8 *)(obj + 0xc1) = 0;
    global_value = CM_GLOBAL(s32, 0x009cf568);
    *(volatile u8 *)(obj + 0xf1) = 0;
    *(volatile u8 *)(obj + 0xf0) = 0;
    *(volatile u8 *)(obj + 0xef) = 0;
    *(volatile u8 *)(obj + 0xee) = 0;
    *(volatile u8 *)(obj + 0x4a) = 3;
    *(volatile s32 *)(obj + 0x1c) = global_value;
    vtable = *(volatile u32 *)obj;
    *(volatile s32 *)(obj + 0x20) = -1;
    *(volatile u8 *)(obj + 0x49) = 7;
    target = *(volatile u32 *)(vtable + 0x3c);
    result = ((slot_3c_fn)target)(obj, -1, obj + 0xa9, obj + 0x3a, 0);
    *(volatile u32 *)(obj + 0xba) = result;
    return 1;
}
