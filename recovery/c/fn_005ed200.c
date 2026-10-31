/* Preferred-image VA 005ed200/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_THISCALL cm_fn_0055fba0(u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_005e5ad0(u32, u32);
extern u32 CM_CDECL cm_fn_00946540(u32, u32);
extern u32 CM_THISCALL cm_fn_004aeaf0(u32, u32, u32, u32, volatile u32 *,
                                     u32, u32, u32, u32);

/* CDECL10/fullEAX=1. 55fba0 has eleven stack arguments/RET2c; 4aeaf0
   has eight/RET20. Preserve the overwritten actual fifth and sixth argument
   slots. The latter is passed by address to the original text routine.
   The centering division uses a wrapping decrement then signed /2, including
   INT_MIN. All original callees, including their shared code, remain explicit. */
u32 CM_CDECL cm_fn_005ed200(u32 left, u32 second, u32 right, u32 fourth,
                           volatile u32 fifth, volatile u32 sixth, u32 height,
                           u32 eighth, u32 ninth, u32 text_object)
{
    u32 receiver, selected, object, text, width, limit, value, other, result;
    selected = (u32)(s32)(s16)W(0x00b77c7au);
    receiver = D(0x00b74c4cu + selected * 768u);
    result = cm_fn_0055fba0(receiver, left, second, right, fourth,
                           1, 0, height, 0, fifth, sixth, 0xffffffffu);
    object = text_object;
    fifth = result;
    if (object != 0) {
        text = D(object);
        width = cm_fn_005e5ad0(eighth, text);
        limit = right - left - 9u;
        if ((s32)width <= (s32)limit) {
            text = D(object);
            if (cm_fn_00946540(text, 10) == 0) {
                value = height - 1u;
                other = ninth;
                sixth = (u32)((s32)value / 2);
                value = eighth;
                right -= 5u;
                left += 5u;
                cm_fn_004aeaf0(object, fifth, left, right, &sixth,
                                value, other, 1, 0);
                return 1;
            }
        }
        other = ninth;
        value = eighth;
        right -= 5u;
        left += 5u;
        sixth = 0;
        cm_fn_004aeaf0(object, fifth, left, right, &sixth,
                        value, other, 0, height);
    }
    return 1;
}
