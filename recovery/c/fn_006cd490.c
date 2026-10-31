#include "cmre.h"

/* Entry: THISCALL with one stack argument (ret 4). ECX is the object pointer
   (saved in ESI). The stack argument is read only as a byte (cmp byte [esp+8], 1
   and the byte compare on entry), so it is declared u8. Returns a byte in AL.
   Object fields by byte offset from the object pointer:
   0x8ea7 signed byte selector, 0x8ea8 signed byte selector, 0x8eae flag byte,
   0x8eb2 state byte, 0x8f7a pointer passed as ECX to 0x006d6f70,
   0xf582 pointer passed as ECX to 0x006f0970. */

#define OBJ_U8(off) (*(volatile u8 *)(self + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern void CM_FASTCALL cm_fn_006f0970(int param_1);
extern void CM_THISCALL cm_fn_006d6f70(u32 self, u16 param_2, u32 param_3, s8 param_4, u8 param_5, s8 param_6, s8 param_7, u32 param_8);

u8 CM_THISCALL cm_fn_006cd490(u32 self, u8 param_1)
{
    u32 ecx_value;
    u8 z;
    s8 sel;
    s8 w;

    if (OBJ_U8(0x8eb2) != 0) {
        return 1;
    }
    if (OBJ_U8(0x8eae) != 0xff) {
        OBJ_U8(0x8eae) = 0xff;
    }
    ecx_value = OBJ_U32(0xf582);
    if (ecx_value != 0) {
        cm_fn_006f0970((int)ecx_value);
    }

    sel = (s8)OBJ_U8(0x8ea7);
    if (sel >= 0 && sel < 9) {
        w = (s8)OBJ_U8(0x8ea8);
        if (w < 0) {
            if (param_1 == 1) {
                ecx_value = OBJ_U32(0x8f7a);
                OBJ_U8(0x8eb2) = 4;
                OBJ_U8(0x8eae) = 0;
                cm_fn_006d6f70(ecx_value, 0x1f77, self, -1, 0, -1, -1, 0);
            } else {
                ecx_value = OBJ_U32(0x8f7a);
                OBJ_U8(0x8eb2) = 3;
                OBJ_U8(0x8eae) = 1;
                cm_fn_006d6f70(ecx_value, 0x1f75, self, -1, 1, -1, -1, 0);
            }
            return 1;
        }
        if (w < 12) {
            return 0;
        }
        if (param_1 == 1) {
            ecx_value = OBJ_U32(0x8f7a);
            OBJ_U8(0x8eb2) = 3;
            OBJ_U8(0x8eae) = 0;
            cm_fn_006d6f70(ecx_value, 0x1f75, self, -1, 0, -1, -1, 0);
        } else {
            ecx_value = OBJ_U32(0x8f7a);
            OBJ_U8(0x8eb2) = 4;
            OBJ_U8(0x8eae) = 1;
            cm_fn_006d6f70(ecx_value, 0x1f77, self, -1, 1, -1, -1, 0);
        }
        return 1;
    }

    /* Selector out of range: the byte flag is the result of "test param, param; sete". */
    z = (param_1 == 0) ? 1 : 0;
    ecx_value = OBJ_U32(0x8f7a);
    OBJ_U8(0x8eb2) = 2;
    OBJ_U8(0x8eae) = z;
    cm_fn_006d6f70(ecx_value, 0x1f76, self, -1, z, -1, -1, 0);
    return 1;
}
