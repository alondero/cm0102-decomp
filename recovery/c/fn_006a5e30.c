#include "cmre.h"

/* Entry: STDCALL with three stack arguments (ret 0xc). Argument 1 is the object
   (ECX for the 0x00785830 calls), argument 2 selects the branch (zero: one
   format and one call; non-zero: five 0x00785830 results, then formatted
   records and two 0x004ae8a0 calls). Argument 3 is the receiver for
   0x004ae660 and 0x004ae8a0. The frame is one scratch block: offset 0 and 4
   hold the index 3 and index 4 results, offset 8 is the 0x190-byte buffer
   passed to 0x0053e140, and offset 0xb8 is the formatted string buffer.
   Only the scratch offsets the listing names are used. */

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern u32 CM_CDECL cm_fn_0053e140(int param_1, int param_2, int param_3, int param_4, u32 param_5, u32 param_6, int param_7);
extern u32 CM_CDECL cm_fn_0053e990(int param_1, int param_2, int param_3, int param_4, int param_5);
extern char *CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_006a5e30(u32 param_1, u32 param_2, u32 param_3)
{
    u8 frame[0x2ac];
    u32 idx0;
    u32 idx1;
    u32 idx2;
    u32 idx3;
    u32 idx4;
    u32 table_base;
    u32 record;
    u32 g2;
    u32 target;
    s8 ch;
    u32 r2;

    if (param_2 == 0) {
        cm_fn_0066f4e0((u32)(frame + 0xb8), 0x00a0bb3cu);
        cm_fn_004ae660((void *)param_3, (u32)(frame + 0xb8));
        return;
    }

    idx0 = (u32)cm_fn_00785830((void *)param_1, 0u);
    idx1 = (u32)cm_fn_00785830((void *)param_1, 1u);
    idx2 = (u32)cm_fn_00785830((void *)param_1, 2u);
    idx3 = (u32)cm_fn_00785830((void *)param_1, 3u);
    idx4 = (u32)cm_fn_00785830((void *)param_1, 4u);

    cm_fn_0053e140((int)idx2, (int)idx3, (int)idx4, (int)(u32)(frame + 8),
                   0x190u, 1u, (int)idx1);

    /* Record for index 0: 581 bytes per entry, base from the global at 0x00ae23b4. */
    table_base = 581u * idx0;
    g2 = CM_GLOBAL(volatile u32, 0x00ae23b4);   /* one read at 0x6a5eeb */
    ch = *(volatile s8 *)(g2 + table_base + 0x37u);
    record = g2 + table_base + 4u;
    cm_fn_0066f4e0((u32)(frame + 0xb8), 0x00a0ba88u,
                   (int)ch, (int)ch, 0x64, (int)record, (int)(u32)(frame + 8));

    cm_fn_004ae660((void *)param_3, (u32)(frame + 0xb8));

    target = CM_GLOBAL(u32, 0x00ae23b4) + table_base;
    cm_fn_004ae8a0((int)param_3, (u8 *)(target + 4u), 0x7d5, (int)target);

    g2 = CM_GLOBAL(u32, 0x00ae23bc);
    target = g2 + 110u * idx1;
    r2 = cm_fn_0053e990((int)target, (int)idx2, (int)idx3, (int)idx4, 0);
    if (r2 != 0) {
        g2 = CM_GLOBAL(u32, 0x00ae23bc);
        target = 110u * idx1 + g2;
        cm_fn_004ae8a0((int)param_3, (u8 *)(frame + 8), 0x7db, (int)target);
    }
}
