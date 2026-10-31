#include "cmre.h"

extern void CM_FASTCALL cm_fn_0058e330(int *param_1);
extern void CM_FASTCALL cm_fn_007682b0(u32 *param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Thiscall with no stack arguments (ret 0). Releases owned blocks held in the object:
   +0x08 is a 30-slot table (0x78 bytes); each non-null slot goes to 0x007682b0 (fastcall)
   and is then freed through 0x00944c94, and the table itself is freed through 0x009452ca.
   +0x00, +0x04, +0x10 and +0x0c each go to 0x0058e330 (fastcall) and then 0x00944c94.
   +0x14 is freed through 0x009452ca. Nothing is cleared. */
void CM_THISCALL cm_fn_00413410(u8 *param_1)
{
    u32 index;
    u32 element;
    u32 block;

    block = *(u32 *)(param_1 + 8);
    if (block != 0) {
        index = 0;
        do {
            element = *(u32 *)(*(u32 *)(param_1 + 8) + index);
            if (element != 0) {
                cm_fn_007682b0((u32 *)element);
                cm_fn_00944c94((void *)element);
            }
            index += 4;
        } while (index < 0x78U);
        cm_fn_009452ca(*(u32 *)(param_1 + 8));
    }

    block = *(u32 *)param_1;
    if (block != 0) {
        cm_fn_0058e330((int *)block);
        cm_fn_00944c94((void *)block);
    }

    block = *(u32 *)(param_1 + 4);
    if (block != 0) {
        cm_fn_0058e330((int *)block);
        cm_fn_00944c94((void *)block);
    }

    block = *(u32 *)(param_1 + 0x10);
    if (block != 0) {
        cm_fn_0058e330((int *)block);
        cm_fn_00944c94((void *)block);
    }

    block = *(u32 *)(param_1 + 0xc);
    if (block != 0) {
        cm_fn_0058e330((int *)block);
        cm_fn_00944c94((void *)block);
    }

    block = *(u32 *)(param_1 + 0x14);
    if (block != 0) {
        cm_fn_009452ca(block);
    }
}
