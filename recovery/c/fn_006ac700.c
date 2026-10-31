#include "cmre.h"

/* Stdcall (ret 0xc): three stack arguments.
   arg1 (ESP entry +4): object pointer, tested and then read at 0x57, 0xcf, 0x80, 0x53.
   arg2 (ESP entry +8): pointer; the listing dereferences it once at 0x006ac7d3.
   arg3 (ESP entry +0xc): pointer; cached in EBX and dereferenced at 0x006ac7dd.

   Object fields by byte offset from arg1: 0x00 dword, 0x53 dword, 0x57 dword,
   0x80 word, 0xcf dword (pointer to a record with pointers at 0x00, 0x04, 0x08
   and 0x0c, each followed by a dword read at +0x33).

   Scratch block of 0xe0 bytes (listing L = ESP after `sub esp, 0xe0`): every
   call receives it as its ECX or as an argument at L. The SEH frame sits above
   it at L+0xe0 (prev), +0xe4 (handler), +0xe8 (state). */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_007862d0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_007863f0(void *self, u32 arg1, u32 arg2);
extern int cm_fn_0090cfc0(int param_1);

void CM_STDCALL cm_fn_006ac700(u32 arg1, u32 arg2, u32 arg3)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe0];
    u32 sc;
    u32 esi_obj;
    u32 p;
    u32 v;
    int r90;

    seh.state = -1;
    seh.handler = 0x0095d0abu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    sc = (u32)scratch;

    if (arg1 != 0u &&
        *(volatile u32 *)(arg1 + 0x57u) != 0u &&
        *(volatile u32 *)(arg1 + 0xcfu) != 0u &&
        arg3 != 0u) {

        cm_fn_0077bbe0((void *)sc, 0x178du, 0u);

        esi_obj = *(volatile u32 *)(arg1 + 0xcfu);
        seh.state = 0;

        v = *(volatile u32 *)esi_obj;
        cm_fn_00785790((void *)sc, 0u, v);

        p = *(volatile u32 *)(esi_obj + 0x04u);
        v = *(volatile u32 *)(p + 0x33u);
        cm_fn_00785790((void *)sc, 1u, v);

        p = *(volatile u32 *)(esi_obj + 0x08u);
        v = *(volatile u32 *)(p + 0x33u);
        cm_fn_00785790((void *)sc, 2u, v);

        p = *(volatile u32 *)(esi_obj + 0x0cu);
        v = *(volatile u32 *)(p + 0x33u);
        cm_fn_00785790((void *)sc, 3u, v);

        v = *(volatile u32 *)arg1;
        cm_fn_00785790((void *)sc, 4u, v);

        p = arg2;
        v = *(volatile u32 *)p;
        cm_fn_00785790((void *)sc, 5u, v);

        v = *(volatile u32 *)arg3;
        cm_fn_00785790((void *)sc, 6u, v);

        r90 = cm_fn_0090cfc0(5);
        cm_fn_00785790((void *)sc, 7u, (u32)r90);

        if ((s16)*(volatile u16 *)(arg1 + 0x80u) < 0x1676) {
            cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431cu), sc, arg1);
        } else {
            r90 = cm_fn_0090cfc0(3);
            if (r90 != 0) {
                v = *(volatile u32 *)(arg1 + 0x57u);
                cm_fn_007862d0((void *)CM_GLOBAL(u32, 0x00b7431cu), sc, v);
            } else {
                v = *(volatile u32 *)(arg1 + 0x53u);
                cm_fn_007863f0((void *)CM_GLOBAL(u32, 0x00b7431cu), sc, v);
            }
        }
    }

    CM_FS(u32, 0) = seh.prev;
}
