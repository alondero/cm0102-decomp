#include "cmre.h"

extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern void CM_CDECL cm_fn_00945333(u32 *table, u32 *count, u32 limit,
                                    void *compare);
extern u32 CM_CDECL cm_fn_00946464(u32 *key, u32 *table, u32 *count,
                                   u32 limit, void *compare);

u32 CM_THISCALL cm_fn_004a4a30(volatile u8 *self, volatile u32 *input)
{
    volatile u8 scratch[0x230];
    u32 marker;

    if (input == 0) {
        cm_fn_00944cff(0x0099ca94u, 0u, 0u,
                       (void *)&scratch[0x130], (void *)&scratch[0x30]);
        marker = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, marker,
                       (void *)&scratch[0x130], (void *)&scratch[0x30],
                       0xd0eu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    {
        u32 table = *(volatile u32 *)(self + 0x19u);
        if (table == 0) {
            return 0;
        }
        if (*(volatile u8 *)(self + 0x25u) != 0xffu) {
            u32 count;
            *(volatile u8 *)(self + 0x25u) = 0xffu;
            CM_GLOBAL(volatile u32, 0x00adae10u) = (u32)self;
            count = *(volatile u32 *)(self + 0x21u);
            table = *(volatile u32 *)(self + 0x19u);
            cm_fn_00945333((u32 *)table, (u32 *)count, 0x2du,
                           (void *)0x004ad250u);
        }
    }

    {
        u32 key = *(volatile u32 *)input;
        u32 count;
        u32 table;
        CM_GLOBAL(volatile u32, 0x00adae10u) = (u32)self;
        count = *(volatile u32 *)(self + 0x21u);
        table = *(volatile u32 *)(self + 0x19u);
        *(volatile u32 *)&scratch[0] = key;
        return cm_fn_00946464((u32 *)&scratch[0], (u32 *)table,
                              (u32 *)count, 0x2du, (void *)0x004ad250u);
    }
}
