#include "cmre.h"

/* Thiscall with no stack arguments; ECX is the object. Object field 0x00 is the
   pointer array allocated here (14 bytes per global count, then one 14-byte
   block per entry, stored at 4-byte slots). Element stride 0x245. Element fields:
   0x00 id, 0x53 pointer to a record (record fields 0x00 index, 0x71 presence,
   0x11c flag byte), 0x57 pointer, 0x80 signed word. */

extern u32 CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
extern void CM_FASTCALL cm_fn_0068d020(u32 self);
extern u32 CM_CDECL cm_fn_0053b900(u32 input);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32 self, u32 src, s32 count);

u32 CM_THISCALL cm_fn_007acfc0(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 count;
    u32 size;
    u32 block;
    u32 i;
    u32 p;
    u32 value;
    u32 elem;
    u32 record;
    u32 result;
    u8 flag;
    int proceed;

    seh.state = -1;
    seh.handler = 0x0095f26bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    count = CM_GLOBAL(volatile u32, 0x00ae2350u);
    size = (count * 8u - count) << 1;
    block = cm_fn_00944e46(size);
    *(volatile u32 *)self = block;

    count = CM_GLOBAL(volatile u32, 0x00ae2350u);
    i = 0;
    if ((s32)count > 0) {
        do {
            p = cm_fn_00944cf1(0x0eu);
            seh.state = 0;
            if (p != 0) {
                cm_fn_0068d020(p);
                value = p;
            } else {
                value = 0;
            }
            block = *(volatile u32 *)self;
            i = i + 1;
            seh.state = -1;
            *(volatile u32 *)(block + (i - 1u) * 4u) = value;
            count = CM_GLOBAL(volatile u32, 0x00ae2350u);
        } while ((s32)i < (s32)count);
    }

    result = CM_GLOBAL(volatile u32, 0x00ae235cu);
    elem = CM_GLOBAL(volatile u32, 0x00ae23b4u);
    i = 0;
    if ((s32)result > 0) {
        do {
            if (cm_fn_0053b900(elem) == 0) {
                record = *(volatile u32 *)(elem + 0x53u);
                if (record != 0) {
                    if (*(volatile u32 *)(record + 0x71u) != 0) {
                        flag = *(volatile u8 *)(record + 0x11cu);
                        if (flag == 0) {
                            proceed = (*(volatile u32 *)(elem + 0x57u) != 0);
                        } else if ((s16)*(volatile u16 *)(elem + 0x80u) > 2) {
                            proceed = 0;
                        } else {
                            proceed = (*(volatile u32 *)(elem + 0x57u) != 0);
                        }
                        if (proceed) {
                            u32 id = *(volatile u32 *)elem;
                            if (id != CM_GLOBAL(volatile u32, 0x009d00a4u) &&
                                id != CM_GLOBAL(volatile u32, 0x009d00a8u)) {
                                u32 index = *(volatile u32 *)record;
                                u32 target = *(volatile u32 *)(*(volatile u32 *)self +
                                                               index * 4u);
                                cm_fn_0068d0d0(target, elem, 4);
                            }
                        }
                    }
                }
            }
            result = CM_GLOBAL(volatile u32, 0x00ae235cu);
            i = i + 1;
            elem = elem + 0x245u;
        } while ((s32)i < (s32)result);
    }

    CM_FS(u32, 0) = seh.prev;
    return result;
}
