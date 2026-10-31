#include "cmre.h"

/* Arguments (cdecl, caller pops): 0x08 destination buffer, 0x0c signed digit
   count, 0x10 object. The original reuses the 0x08 slot as a loop counter; here
   it is the local `count`.
   Object fields by byte offset from the third argument:
   0x04 32-bit counter incremented when the buffer starts with '1',
   0x0c source string pointer (read, not written). */
#define OBJ_U32(obj, off) (*(volatile u32 *)((obj) + (off)))

extern u32 CM_CDECL cm_fn_009488e0(u32 source);
/* The EDI pushed for 0x009488e0 stays on the stack under the next three
   pushes (one add esp, 0x10 cleans both calls); 0x0094f0f0 reads three. */
extern unsigned long long CM_CDECL cm_fn_0094f0f0(u32 arg8, u32 argC, u32 arg10);

void CM_CDECL cm_fn_0094facd(u32 buf, s32 n, u32 obj)
{
    u32 dst;
    u32 src;
    u32 out;
    u32 r;
    s32 count;
    s32 ebx;

    dst = buf + 1u;
    src = OBJ_U32(obj, 0x0c);
    *(volatile u8 *)buf = 0x30;
    ebx = n;
    out = dst;
    if (n > 0) {
        count = n;
        ebx = 0;
        do {
            u8 ch = *(volatile u8 *)src;
            if (ch == 0) {
                ch = 0x30;
            } else {
                src = src + 1u;
            }
            *(volatile u8 *)out = ch;
            out = out + 1u;
            count = count - 1;
        } while (count != 0);
    }

    /* and byte ptr [eax], 0: terminate after the last digit written. */
    *(volatile u8 *)out = 0;

    /* Round up when the count is non-negative and the next source byte is
       signed >= '5'. Carry walks back over '9' digits. */
    if (ebx >= 0 && (s8)*(volatile u8 *)src >= 0x35) {
        for (;;) {
            out = out - 1u;
            if (*(volatile u8 *)out != 0x39) {
                *(volatile u8 *)out = (u8)(*(volatile u8 *)out + 1u);
                break;
            }
            *(volatile u8 *)out = 0x30;
        }
    }

    if (*(volatile u8 *)buf == 0x31) {
        OBJ_U32(obj, 0x04) = OBJ_U32(obj, 0x04) + 1u;
        return;
    }

    r = cm_fn_009488e0(dst);
    r = r + 1u;
    cm_fn_0094f0f0(buf, dst, r);
}
