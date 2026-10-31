#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x08 pointer passed in ECX to 0x004a4850 (its result is the record base used
   for the stores below); 0x3e signed word count of table rows; 0xb1 table base
   pointer; table rows are 59 bytes, with a DWORD key at row+0 and a byte at row+0x37.
   Result record fields written by this routine: +0x73, +0x74, +0x75. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_U16(off) (*(volatile u16 *)(self + (off)))

extern u32 CM_THISCALL cm_fn_004a4850(u32 ecx_obj, u32 arg);

u32 CM_THISCALL cm_fn_005f73d0(u32 self, u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u32 dst;
    u32 table;
    u32 entry;
    u32 ptr57;
    u32 g;
    s32 count;
    s32 i;
    u8 bl;
    u8 cl;
    u8 dl;

    bl = 0xffu;
    dst = cm_fn_004a4850(OBJ_U32(0x08), arg0);

    count = (s16)OBJ_U16(0x3e);
    if (count > 0) {
        table = OBJ_U32(0xb1);
        for (i = 0; i < count; i++) {
            if (*(volatile u32 *)(table + (u32)i * 59u) == arg0) {
                bl = *(volatile u8 *)(table + (u32)i * 59u + 0x37u);
                break;
            }
        }
    }

    cl = (u8)arg1;
    if (cl == 0xffu) {
        if (bl == 1u) {
            return dst;
        }
        bl = (u8)arg3;
        if (bl >= 2u && bl <= 7u) {
            bl = 2u;
        }
        cl = (u8)arg4;
        if (cl >= 2u && cl <= 7u) {
            dl = (u8)arg2;
            *(volatile u8 *)(dst + 0x74u) = bl;
            *(volatile u8 *)(dst + 0x73u) = dl;
            *(volatile u8 *)(dst + 0x75u) = 7u;
            return dst;
        }
        dl = (u8)arg2;
        *(volatile u8 *)(dst + 0x73u) = dl;
        *(volatile u8 *)(dst + 0x74u) = bl;
        *(volatile u8 *)(dst + 0x75u) = cl;
        return dst;
    }

    if (cl == 4u || cl == 5u) {
        ptr57 = *(volatile u32 *)(arg0 + 0x57u);
        g = CM_GLOBAL(volatile u32, 0x009cf568);
        if (*(volatile u32 *)ptr57 == g) {
            table = CM_GLOBAL(volatile u32, 0x00adadfc);
            entry = *(volatile u32 *)(table + g * 4u);
            dst = cm_fn_004a4850(*(volatile u32 *)(entry + 8u), arg0);

            bl = (u8)arg3;
            if (bl == 1u) {
                bl = 0xfu;
            } else if (bl >= 2u && bl <= 4u) {
                bl = 0x12u;
            }
            cl = (u8)arg4;
            if (cl == 1u) {
                cl = 0xfu;
            } else if (cl >= 2u && cl <= 4u) {
                cl = 0x12u;
            }
            dl = (u8)arg2;
            if (dl == 1u) {
                dl = 0xfu;
                *(volatile u8 *)(dst + 0x74u) = bl;
                *(volatile u8 *)(dst + 0x73u) = dl;
                *(volatile u8 *)(dst + 0x75u) = cl;
            } else if (dl == 0u || dl > 4u) {
                *(volatile u8 *)(dst + 0x73u) = dl;
                *(volatile u8 *)(dst + 0x74u) = bl;
                *(volatile u8 *)(dst + 0x75u) = cl;
            } else {
                dl = 0x12u;
                *(volatile u8 *)(dst + 0x74u) = bl;
                *(volatile u8 *)(dst + 0x73u) = dl;
                *(volatile u8 *)(dst + 0x75u) = cl;
            }
            return dst;
        }

        bl = (u8)(arg3 + 1u);
        cl = (u8)(arg4 + 1u);
        dl = (u8)(arg2 + 1u);
        *(volatile u8 *)(dst + 0x74u) = bl;
        *(volatile u8 *)(dst + 0x73u) = dl;
        *(volatile u8 *)(dst + 0x75u) = cl;
        return dst;
    }

    bl = (u8)arg3;
    cl = (u8)arg4;
    dl = (u8)arg2;
    *(volatile u8 *)(dst + 0x73u) = dl;
    *(volatile u8 *)(dst + 0x74u) = bl;
    *(volatile u8 *)(dst + 0x75u) = cl;
    return dst;
}
