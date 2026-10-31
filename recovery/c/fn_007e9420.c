#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(int *param_1);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Slot 0 of an object's table, called with the object in ECX and one stack argument. */
typedef void (CM_THISCALL *cm_slot0_fn)(u8 *self, s32 flag);

/* Thiscall with no stack arguments (ret 0). It links an SEH frame {prev, handler, state} at
   fs:[0]; the state is -1 on entry, set to 0 after the table pointer at +0xa7 is read, and
   -1 again before the final callee 0x00518690. The object's first dword is set to the
   table constant 0x0096f124. Fields used by offset: +0x08 block (released through
   0x0049f450 and 0x00944c94), +0x0c array of element pointers, +0x30 signed count,
   +0x3c signed word count, and the blocks at +0xa3 and +0xa7 released through 0x009452ca.
   Elements of the +0xad array are released first (count from +0x3c), then the array itself.
   Each non-null element of the +0x0c array gets a slot-0 call with a flag of 1, and the
   +0x0c array is then released and cleared. Memory accesses are volatile to keep the
   listing order. */
void CM_THISCALL cm_fn_007e9420(u8 *param_1)
{
    volatile struct {
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    u32 self;
    u32 block;
    u32 elem;
    u32 arr;
    u32 vt;
    u32 i;
    s32 count;

    frame.state = -1;
    frame.handler = 0x00960918U;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    self = (u32)param_1;
    CM_GLOBAL(volatile u32, self) = 0x0096f124U;
    block = CM_GLOBAL(volatile u32, self + 0xa7);
    frame.state = 0;
    if (block != 0) {
        cm_fn_009452ca(block);
    }
    block = CM_GLOBAL(volatile u32, self + 0xa3);
    if (block != 0) {
        cm_fn_009452ca(block);
    }

    block = CM_GLOBAL(volatile u32, self + 0xad);
    if (block != 0) {
        i = 0;
        if ((s16)CM_GLOBAL(volatile u16, self + 0x3c) > 0) {
            do {
                elem = CM_GLOBAL(volatile u32, CM_GLOBAL(volatile u32, self + 0xad) + i * 4U);
                if (elem != 0) {
                    cm_fn_009452ca(elem);
                }
                count = (s16)CM_GLOBAL(volatile u16, self + 0x3c);
                i++;
            } while ((s32)i < count);
        }
        cm_fn_009452ca(CM_GLOBAL(volatile u32, self + 0xad));
    }

    block = CM_GLOBAL(volatile u32, self + 8);
    if (block != 0) {
        cm_fn_0049f450((int *)block);
        cm_fn_00944c94((void *)block);
    }

    count = CM_GLOBAL(volatile s32, self + 0x30);
    i = 0;
    if (count >= 0) {
        do {
            arr = CM_GLOBAL(volatile u32, self + 0xc);
            elem = CM_GLOBAL(volatile u32, arr + i * 4U);
            if (elem != 0) {
                vt = CM_GLOBAL(volatile u32, elem);
                ((cm_slot0_fn)CM_GLOBAL(volatile u32, vt))((u8 *)elem, 1);
            }
            count = CM_GLOBAL(volatile s32, self + 0x30);
            i++;
        } while ((s32)i <= count);
    }

    block = CM_GLOBAL(volatile u32, self + 0xc);
    if (block != 0) {
        cm_fn_009452ca(block);
        CM_GLOBAL(volatile u32, self + 0xc) = 0;
    }

    frame.state = -1;
    cm_fn_00518690((void *)self);
    CM_FS(u32, 0) = frame.prev;
}
