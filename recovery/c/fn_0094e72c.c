#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_00947b09(int value);
extern void cm_fn_00948835(void);
extern void cm_fn_0094e7c5(volatile u8 *path, volatile u32 *out_count, volatile u8 *buffer, volatile u32 *capacity, volatile u32 *out_used);
typedef u32 (CM_STDCALL *get_module_filename_fn)(void *, void *, u32);

void CM_CDECL cm_fn_0094e72c(void)
{
    u32 zero = 0;
    if (CM_GLOBAL(volatile u32, 0x00de4630) == zero) cm_fn_00948835();
    ((get_module_filename_fn)CM_GLOBAL(volatile u32, 0x0096711c))((void *)0, (void *)0x00de314c, 0x104u);
    u32 alternate = CM_GLOBAL(volatile u32, 0x00de4628);
    CM_GLOBAL(volatile u32, 0x00de2fc4) = 0x00de314cu;
    u32 path = 0x00de314cu;
    if (*(volatile u8 *)alternate != 0) path = alternate;
    volatile u32 scratch[2];
    volatile u32 *count_out = &scratch[1]; /* [EBP-4] */
    volatile u32 *capacity_out = &scratch[0]; /* [EBP-8] */
    cm_fn_0094e7c5((volatile u8 *)path, 0, 0, count_out, capacity_out);
    u32 capacity = *capacity_out;
    u32 count = *count_out;
    u32 size = capacity + count * 4u;
    u32 allocation = (u32)cm_fn_00944e46(size);
    if (allocation == 0) cm_fn_00947b09(8);
    count = *count_out;
    u32 end = allocation + count * 4u;
    cm_fn_0094e7c5((volatile u8 *)path, (volatile u32 *)allocation, (volatile u8 *)end, count_out, capacity_out);
    u32 final_count = *count_out;
    CM_GLOBAL(volatile u32, 0x00de2fac) = allocation;
    CM_GLOBAL(volatile u32, 0x00de2fa8) = final_count - 1u;
}
