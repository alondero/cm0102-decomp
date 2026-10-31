#include "cmre.h"

/* Thiscall constructor-style routine (ret 0x20, eight stack arguments; arg_7 is
   never read by the listing). Object fields by byte offset from ECX (this):
   0x00 vtable pointer 0x00967864, 0x05 dword, 0x09 dword (0 when arg_6 is -1),
   0x0d dword (arg_1), 0x11 dword (arg_2), 0x15 word (low 16 bits of arg_4),
   0x17 word (low 16 bits of arg_5), 0x19 dword (arg_8). Byte 0x04 is not written. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_U16(off) (*(volatile u16 *)(self + (off)))

/* EAX holds the object pointer from entry (mov eax, ecx) and is not changed
   afterwards, so the return value is the object pointer. */
u32 CM_THISCALL cm_fn_00412e40(u32 self, u32 arg_1, u32 arg_2, u32 arg_3,
                               u32 arg_4, u32 arg_5, u32 arg_6, u32 arg_7,
                               u32 arg_8)
{
    u32 value;

    OBJ_U32(0x00) = 0x00967864u;

    /* value = global(0x00ae23e0) + arg_3 * 101 (lea/shl chain in the listing) */
    value = (u32)CM_GLOBAL(volatile s32, 0x00ae23e0) + arg_3 * 101u;
    OBJ_U32(0x05) = value;

    if (arg_6 == 0xffffffffu) {
        OBJ_U32(0x09) = 0u;
    } else {
        /* edx = global(0x00ae23d0) + arg_6 * 107 */
        OBJ_U32(0x09) = (u32)CM_GLOBAL(volatile s32, 0x00ae23d0) + arg_6 * 107u;
    }

    OBJ_U32(0x0d) = arg_1;
    OBJ_U32(0x11) = arg_2;
    OBJ_U16(0x15) = (u16)arg_4;
    OBJ_U16(0x17) = (u16)arg_5;
    OBJ_U32(0x19) = arg_8;
    (void)arg_7;
    return self;
}
