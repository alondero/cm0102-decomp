#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (thiscall, no stack
   arguments, ret 0). Writes happen in listing order. Field groups written here:
   0x00 dword header, 0x04 dword, 0x08 dword, 0x0c dword, 0x10 dword, 0x14 dword,
   0x18 dword, 0x1c byte, 0x1d byte, 0x1e dword, 0x22 byte, 0x23 byte, 0x24 byte,
   0x25..0x43 and 0x44..0x62 byte runs (31 iterations each), 0x63..0x64 bytes,
   0x65..0x70 and 0x71..0x7c byte runs (12 iterations each), 0x7d..0x7f bytes. */
#define OBJ_U8(off) (*(volatile u8 *)(self + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

void CM_THISCALL cm_fn_00806450(u32 self)
{
    u32 cursor;
    u32 count;

    OBJ_U32(0x08) = 0x10u;
    OBJ_U32(0x10) = 0x800000u;
    OBJ_U32(0x0c) = 0u;
    OBJ_U32(0x14) = 0u;
    OBJ_U32(0x00) = 0x14140081u;
    OBJ_U32(0x18) = 0xffffffffu;
    OBJ_U8(0x1c) = 0xffu;
    OBJ_U32(0x1e) = 0xffffffffu;
    OBJ_U8(0x22) = 0xffu;
    OBJ_U32(0x04) = 0x44aaa884u;
    OBJ_U8(0x1d) = 1u;
    OBJ_U8(0x23) = 0u;
    OBJ_U8(0x24) = 5u;

    /* 31 iterations: byte run 0x25..0x43 set to 1, 0x44..0x62 set to 0x14. */
    cursor = self + 0x44u;
    count = 0x1fu;
    do {
        *(volatile u8 *)(cursor - 0x1fu) = 1u;
        *(volatile u8 *)cursor = 0x14u;
        cursor = cursor + 1u;
        count = count - 1u;
    } while (count != 0u);

    OBJ_U8(0x63) = 0u;
    OBJ_U8(0x64) = 0u;

    /* 12 iterations: byte run 0x65..0x70 set to 1, 0x71..0x7c set to 0x14. */
    cursor = self + 0x71u;
    count = 0x0cu;
    do {
        *(volatile u8 *)(cursor - 0x0cu) = 1u;
        *(volatile u8 *)cursor = 0x14u;
        cursor = cursor + 1u;
        count = count - 1u;
    } while (count != 0u);

    OBJ_U8(0x7d) = 0u;
    OBJ_U8(0x7e) = 0u;
    OBJ_U8(0x7f) = 0u;
}
