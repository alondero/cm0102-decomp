#include "cmre.h"

typedef struct {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_record_007a58f0;

extern void CM_FASTCALL cm_fn_0054a110(void *local_object);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 receiver, const volatile u8 *row);
extern u8 CM_STDCALL cm_fn_007a79c0(const volatile u8 *row,
                                    void *local_result);
extern void CM_THISCALL cm_fn_007a8510(void *self, void *local_result);
extern s32 CM_CDECL cm_fn_007ae2a0(u8 *left, u8 *right);
extern void CM_CDECL cm_fn_00945333(u32 first, u32 second, u32 count,
                                   void *compare);

void CM_THISCALL cm_fn_007a58f0(volatile u8 *self)
{
    volatile cm_seh_record_007a58f0 seh;
    volatile u8 locals[0x28];
    void *entry_prev;
    void *restore_prev;
    s32 count;
    u32 row_number;
    volatile u8 *row;

    seh.state = -1;
    seh.handler = (void *)0x0095f188u;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0054a110((void *)(locals + 0x14u));
    seh.state = 0;
    cm_fn_0054a110((void *)(locals + 0x1cu));
    seh.state = 1;
    cm_fn_0054a110((void *)(locals + 4u));

    count = CM_GLOBAL(volatile s32, 0x00ae2364u);
    row = (volatile u8 *)(unsigned long)
        CM_GLOBAL(volatile u32, 0x00ae23bcu);
    *(volatile u8 *)&seh.state = 2u;
    row_number = 0;
    if (count > 0) {
        do {
            s32 id = *(volatile s32 *)row;
            s32 upper = (s32)((u32)count - 0x10u);
            if (id < upper) {
                u32 table = CM_GLOBAL(volatile u32, 0x00ae2d0cu);
                u32 row_offset = (u32)id * 0x4fu;
                if ((*(volatile u8 *)(table + row_offset + 0x0bu) & 0x30u) == 0u) {
                    u32 mapped = CM_GLOBAL(volatile u32, 0x00ae1050u);
                    u32 object = cm_fn_004cf5a0(mapped, row);
                    if (object == 0u || *(volatile u8 *)(object + 0x49u) == 0u) {
                        if (cm_fn_007a79c0(row, (void *)(locals + 0x0cu)) != 0u)
                            cm_fn_007a8510((void *)self,
                                           (void *)(locals + 0x0cu));
                    }
                }
            }
            count = CM_GLOBAL(volatile s32, 0x00ae2364u);
            row_number++;
            row += 0x6eu;
        } while ((s32)row_number < count);
    }

    {
        u32 second = *(volatile u32 *)(self + 8u);
        u32 first = *(volatile u32 *)(self + 4u);
        cm_fn_00945333(first, second, 0x1au, (void *)cm_fn_007ae2a0);
    }

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
