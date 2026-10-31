#include "cmre.h"

/* fn_005c1710: thiscall (ECX = object, read before written) with one stack argument (ret 4).
   Preferred-image VA 0x005c1710, image base 0x00400000.
   Object fields: 0x04 pointer whose +0x0c address is the receiver of 0x0040a7d0;
   0x0a dword compared with the record field at +0x53; the first stack argument is a pointer
   to two 16-bit words at +0x00 and +0x02 that are compared with the first returned dword.
   SEH frame {prev, handler 0x00959cd8, state} is linked before the scratch copy and unlinked
   before return. The 0x18-byte scratch area holds the dword copies at +0x00/+0x04 and the two
   8-byte output buffers at +0x08 and +0x10. */

extern u32 CM_THISCALL cm_fn_0040a7d0(int param_1, u32 param_2, u32 param_3);
extern u32 CM_THISCALL cm_fn_0054c8e0(short *param_1, u32 *param_2, int param_3);

void CM_THISCALL cm_fn_005c1710(int param_1, short *param_2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0x18];
    u32 v;
    u32 ret;
    u32 c1;
    u32 c2;
    u32 n;
    u32 b;
    u32 rec;
    u32 g2;
    u32 r0;
    u32 addr;
    u32 i;

    seh.state = -1;
    seh.handler = 0x00959cd8u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    /* eax keeps the high word of the old fs:[0] value; the low word is the param word at +2. */
    v = (seh.prev & 0xffff0000u) | (u32)(u16)*(volatile u16 *)((u8 *)param_2 + 2);
    ret = cm_fn_0040a7d0(
        (int)(*(volatile u32 *)((u8 *)param_1 + 4u) + 0x0cu),
        (u32)(scratch + 0x08),
        v);

    seh.state = 0;
    ret = cm_fn_0054c8e0((short *)ret, (u32 *)(scratch + 0x10), 1);

    c1 = *(volatile u32 *)ret;
    c2 = *(volatile u32 *)(ret + 4u);
    *(volatile u32 *)(scratch + 0x00) = c1;
    *(volatile u32 *)(scratch + 0x04) = c2;

    if ((u16)*(volatile u16 *)(scratch + 0x02) == (u16)*(volatile u16 *)((u8 *)param_2 + 2) &&
        (u16)*(volatile u16 *)(scratch + 0x00) == (u16)*(volatile u16 *)param_2) {
        n = CM_GLOBAL(u32, 0x00ae235c);
        b = CM_GLOBAL(u32, 0x00ae23b4);
        i = 0;
        if ((s32)n > 0) {
            do {
                rec = b;
                if (*(volatile u32 *)(rec + 0x53u) == *(volatile u32 *)((u8 *)param_1 + 0x0au)) {
                    g2 = CM_GLOBAL(u32, 0x00de1f54);
                    r0 = *(volatile u32 *)rec;
                    addr = r0 * 0x10u + *(volatile u32 *)(g2 + 0x91cu);
                    *(volatile u8 *)addr = 0;
                    *(volatile u8 *)(addr + 1u) = 0;
                }
                i = i + 1;
                b = b + 0x245u;
            } while ((s32)i < (s32)CM_GLOBAL(u32, 0x00ae235c));
        }
    }

    CM_FS(u32, 0) = seh.prev;
}
