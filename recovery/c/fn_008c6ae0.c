/* Preferred-image VA 0x008c6ae0, image base 0x00400000, CM0102 3.9.68.
   Thiscall (ECX = self) with one stack argument (`ret 4`): rec, a pointer
   whose dword at +0xc is an index x and whose dword at +4 is a list head p.
   Global 0x00ae23bc is the base of a table of 110-byte rows (row = base +
   110 * value). SEH frame { prev, handler 0x00963d18, state }: state -1 at
   entry, 0 after the first helper call, -1 again before the last helper; prev
   is restored to fs:[0] before return. Callees 0x0068d020 and 0x0068d040
   (fastcall, scratch buffer in ECX), 0x0068d1b0 (fastcall, returns a pointer),
   0x008c41c0 (thiscall, four arguments, callee pops) and 0x008c47d0 (thiscall,
   one argument) stay original boundaries. 0x0068d040 leaves EAX, which is the
   return value here; it is forwarded unchanged. The 16-byte scratch buffer is
   [esp+0xc], directly below the SEH frame. */
#include "cmre.h"

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

extern void CM_FASTCALL cm_fn_0068d020(u16 *param_1);
extern u32 CM_FASTCALL cm_fn_0068d040(u16 *param_1);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern void CM_THISCALL cm_fn_008c41c0(u32 param_1, u32 param_2, u32 param_3,
                                       u32 param_4, u32 param_5);
extern void CM_THISCALL cm_fn_008c47d0(u32 param_1, u32 param_2);

u32 CM_THISCALL cm_fn_008c6ae0(u32 self, u32 rec)
{
    volatile cm_seh_record seh;
    void *entry_prev;
    u16 buf[8];
    u32 x;
    u32 row;
    u32 p;
    u32 cur;
    u32 idx;
    u32 r;

    seh.state = -1;
    seh.handler = (void *)0x00963d18;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0068d020(buf);

    x = *(volatile u32 *)(rec + 0xc);
    seh.state = 0;
    row = CM_GLOBAL(volatile u32, 0x00ae23bc) + x * 110u;
    cm_fn_008c41c0(self, row, rec, 0, 0);

    p = *(volatile u32 *)(rec + 4);
    if (p != 0) {
        r = *(volatile u32 *)(p + 2);
        *(volatile u32 *)(p + 0xa) = r;
        r = *(volatile u32 *)(rec + 4);
        cur = cm_fn_0068d1b0((int)r);
        if (cur != 0) {
            do {
                idx = *(volatile u32 *)cur;
                row = CM_GLOBAL(volatile u32, 0x00ae23bc) + idx * 110u;
                cm_fn_008c41c0(self, row, rec, 0, 0);

                cur = *(volatile u32 *)cur;
                row = CM_GLOBAL(volatile u32, 0x00ae23bc) + cur * 110u;
                cm_fn_008c47d0(self, row);

                r = *(volatile u32 *)(rec + 4);
                cur = cm_fn_0068d1b0((int)r);
            } while (cur != 0);
        }
    }

    seh.state = -1;
    r = cm_fn_0068d040(buf);

    CM_FS(void *, 0) = seh.prev;
    return r;
}
