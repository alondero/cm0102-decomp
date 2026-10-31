#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
/* Callee 0x009462bb pops its four arguments (ret 0x10). */
extern void CM_STDCALL cm_fn_009462bb(u32 param_1, u32 param_2, int param_3, void *param_4);

/* Thiscall with no stack arguments (ret 0). Four pointer fields are released in turn:
   fields +0x5f1 and +0x5f9 are blocks with a header dword at ptr-4, passed to 0x009462bb
   with the constant 0x18e3 and the global 0x005a53c0, then freed through 0x00944c94 at
   ptr-4; fields +0x601 and +0x000 are released directly. Each released field and the dword
   after it are cleared. */
void CM_THISCALL cm_fn_008961e0(u8 *param_1)
{
    u32 block;
    u32 header;

    block = *(u32 *)(param_1 + 0x5f1);
    if (block != 0) {
        header = *(u32 *)(block - 4);
        cm_fn_009462bb(block, 0x18e3U, (int)header, (void *)0x005a53c0);
        cm_fn_00944c94((void *)(block - 4));
        *(u32 *)(param_1 + 0x5f1) = 0;
        *(u32 *)(param_1 + 0x5f5) = 0;
    }

    block = *(u32 *)(param_1 + 0x5f9);
    if (block != 0) {
        header = *(u32 *)(block - 4);
        cm_fn_009462bb(block, 0x18e3U, (int)header, (void *)0x005a53c0);
        cm_fn_00944c94((void *)(block - 4));
        *(u32 *)(param_1 + 0x5f9) = 0;
        *(u32 *)(param_1 + 0x5fd) = 0;
    }

    block = *(u32 *)(param_1 + 0x601);
    if (block != 0) {
        cm_fn_009452ca(block);
        *(u32 *)(param_1 + 0x601) = 0;
        *(u32 *)(param_1 + 0x605) = 0;
    }

    block = *(u32 *)param_1;
    if (block != 0) {
        cm_fn_00944c94((void *)block);
        *(u32 *)param_1 = 0;
        *(u32 *)(param_1 + 4) = 0;
    }
}
