#include "cmre.h"

/* Object fields by byte offset from arg_1: 0x1a owner pointer, 0x1e link pointer.
   Element fields by offset from the owner or link pointer: 0x00 DWORD tag.
   Stack scratch: scratch[0..1] receive a copy of the 8 bytes returned by
   0x00549f70; scratch[2..3] is the object that call writes through ECX. */

extern u32 CM_CDECL cm_fn_0053d740(u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_00549f70(u32 self, u32 arg2, u32 arg3, u32 arg4, u32 arg5);

#define FIELD_U32(base, off) (*(volatile u32 *)((base) + (off)))
#define FIELD_U16(base, off) (*(volatile u16 *)((base) + (off)))

#define DAT_009cf224 CM_GLOBAL(u32, 0x009cf224)
#define DAT_009cf2dc CM_GLOBAL(u32, 0x009cf2dc)
#define DAT_009cf2e4 CM_GLOBAL(u32, 0x009cf2e4)
#define DAT_009cf360 CM_GLOBAL(u32, 0x009cf360)
#define DAT_009cf3e4 CM_GLOBAL(u32, 0x009cf3e4)
#define DAT_009cf3f4 CM_GLOBAL(u32, 0x009cf3f4)
#define DAT_009cf434 CM_GLOBAL(u32, 0x009cf434)
#define DAT_009cf45c CM_GLOBAL(u32, 0x009cf45c)
#define DAT_009cf4f0 CM_GLOBAL(u32, 0x009cf4f0)
#define DAT_009cf510 CM_GLOBAL(u32, 0x009cf510)
#define DAT_00ae1050 CM_GLOBAL(u32, 0x00ae1050)

u32 CM_CDECL cm_fn_0053d980(u32 arg_1, u32 arg_2, u32 arg_3)
{
    u32 owner;
    u32 link;
    u32 value;
    u32 entry;
    u32 scratch[4];
    u32 copy_lo;
    u32 copy_hi;
    u32 c;
    u16 top;
    u16 limit;
    s16 low;

    if (arg_2 == 0) {
        return 1;
    }
    owner = FIELD_U32(arg_1, 0x1a);
    if (owner == arg_2) {
        return 0;
    }
    link = FIELD_U32(arg_1, 0x1e);
    if (link == arg_2) {
        return 0;
    }
    if (arg_3 == 0) {
        return 1;
    }
    value = *(volatile u32 *)arg_2;

    if (value == DAT_009cf2e4 || value == DAT_009cf45c || value == DAT_009cf510 ||
        value == DAT_009cf3e4 || value == DAT_009cf2dc) {
        c = *(volatile u32 *)owner;
        if (c == DAT_009cf2e4 || c == DAT_009cf45c || c == DAT_009cf510 ||
            c == DAT_009cf2dc || c == DAT_009cf3e4) {
            return 0;
        }
        if (link != 0) {
            c = *(volatile u32 *)link;
            if (c == DAT_009cf2e4 || c == DAT_009cf45c || c == DAT_009cf510 ||
                c == DAT_009cf2dc || c == DAT_009cf3e4) {
                return 0;
            }
        }
    }

    if (owner != 0) {
        if (value == DAT_009cf224 || value == DAT_009cf3f4) {
            c = *(volatile u32 *)owner;
            if (c == DAT_009cf224 || c == DAT_009cf3f4) {
                return 0;
            }
        }
    }

    if (value == DAT_009cf360 && owner != 0) {
        c = *(volatile u32 *)owner;
        if (c == DAT_009cf4f0) {
            return 0;
        }
    }

    if (value != DAT_009cf434) {
        return 1;
    }
    if (cm_fn_0053d740(arg_1, 0) != 0) {
        return 1;
    }
    entry = cm_fn_004cf5a0(DAT_00ae1050, arg_1);
    if (entry == 0) {
        return 1;
    }
    c = cm_fn_00549f70((u32)&scratch[2], 1, 0, 0x7ce, 0xffffffffu);
    copy_lo = *(volatile u32 *)c;
    copy_hi = *(volatile u32 *)(c + 4);
    limit = FIELD_U16(entry, 0x27);
    scratch[0] = copy_lo;
    scratch[1] = copy_hi;
    top = (u16)(copy_lo >> 16);
    if (top > limit) {
        return 0;
    }
    if (top < limit) {
        return 1;
    }
    low = (s16)(u16)copy_lo;
    if (low > (s16)FIELD_U16(entry, 0x25)) {
        return 0;
    }
    return 1;
}
