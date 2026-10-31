/* Preferred-image VA 0x005A7F20, image base 0x00400000, STDCALL (ret 0xc, three stack
   arguments). arg0 and arg1 are object pointers. arg2 is read only as its low byte
   (listing: movsx edx, byte ptr [esp+0x100] at 0x005A8035).
   Frame: 0xe0 bytes of scratch object (ECX = scratch for 0x0077BBE0, 0x00785790) followed
   immediately by one SEH record {prev, handler 0x0095940B, state} at scratch+0xe0. The
   record is linked at entry through CM_FS and unlinked at every exit. State is -1 until
   the first field-store path reaches 0x005A7FAB, where it becomes 0.
   No direct caller reads EAX after the call, so the routine is void. */
#include "cmre.h"

#define FIELD32(base, off) (*(volatile u32 *)((base) + (u32)(off)))

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self,u32 arg1,u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self,u32 arg1,u32 arg2);
extern int cm_fn_0090cfc0(int param_1);

void CM_STDCALL cm_fn_005a7f20(u32 arg0, u32 arg1, s8 arg2)
{
    struct {
        u8 scratch[0xe0];
        volatile struct { u32 prev; u32 handler; s32 state; } seh;
    } f;
    u32 entry_prev;
    u32 v;
    s32 r;
    void *L;

    entry_prev = CM_FS(u32, 0);                     /* mov eax, fs:[0] */
    f.seh.state = -1;
    f.seh.handler = 0x0095940bu;
    f.seh.prev = entry_prev;
    CM_FS(u32, 0) = (u32)&f.seh;
    L = (void *)f.scratch;

    if (arg0 == 0) {
        goto unlink;
    }
    if (FIELD32(arg0, 0x61) == 0) {
        goto unlink;
    }
    if (FIELD32(arg0, 0x1a) == 0) {
        goto unlink;
    }
    if (FIELD32(arg0, 0x39) == 0) {
        goto unlink;
    }
    if (arg1 == 0) {
        goto unlink;
    }
    if (FIELD32(arg1, 0x61) == 0) {
        goto unlink;
    }
    if (FIELD32(arg1, 0x1a) == 0) {
        goto unlink;
    }

    cm_fn_0077bbe0(L, 0x1394u, 0u);

    v = FIELD32(arg0, 0);                           /* mov eax, [esi] */
    f.seh.state = 0;
    cm_fn_00785790(L, 0u, v);

    v = FIELD32(FIELD32(arg0, 4), 0x33);            /* [[esi+4]+0x33] */
    cm_fn_00785790(L, 1u, v);

    v = FIELD32(FIELD32(arg0, 8), 0x33);            /* [[esi+8]+0x33] */
    cm_fn_00785790(L, 2u, v);

    v = FIELD32(FIELD32(arg0, 0xc), 0x33);          /* [[esi+0xc]+0x33] */
    cm_fn_00785790(L, 3u, v);

    v = FIELD32(arg1, 0);                           /* [edi] */
    cm_fn_00785790(L, 4u, v);

    v = FIELD32(FIELD32(arg1, 4), 0x33);            /* [[edi+4]+0x33] */
    cm_fn_00785790(L, 5u, v);

    v = FIELD32(FIELD32(arg1, 8), 0x33);            /* [[edi+8]+0x33] */
    cm_fn_00785790(L, 6u, v);

    v = FIELD32(FIELD32(arg1, 0xc), 0x33);          /* [[edi+0xc]+0x33] */
    cm_fn_00785790(L, 7u, v);

    cm_fn_00785790(L, 8u, (u32)(s32)arg2);          /* movsx edx, byte; push edx */

    r = cm_fn_0090cfc0(5);
    cm_fn_00785790(L, 9u, (u32)r);

    v = FIELD32(arg0, 0x39);                        /* mov eax, [esi+0x39] */
    cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431c), (u32)L, v);

unlink:
    CM_FS(u32, 0) = f.seh.prev;                     /* mov ecx,[esp+0xe8]; mov fs:[0], ecx */
}
