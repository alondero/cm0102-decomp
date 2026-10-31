#include "cmre.h"

extern void CM_FASTCALL cm_fn_0068d020(u16 *param_1);
extern u32 CM_CDECL cm_fn_0053b4f0(u32 param_1);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_0054c8e0(void *self, u32 a, u32 b);
extern void CM_THISCALL cm_fn_00417c10(void *self, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_005acbe0(void *self, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6,
                                      u32 p7, u32 p8, u32 p9, u32 p10);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern u8 CM_THISCALL cm_fn_005abc30(void *self, u32 arg);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Stdcall (one stack argument arg_04 = [entry+4], `ret 4`), no return value.
   The SEH frame is one volatile record: state -1 at entry, 0 after the first
   helper call, 1 before 0x54c8e0, 0 after the 0x68d1b0 test, and -1 before the
   final 0x68d040. One 40-byte scratch block (frame) holds the helper outputs
   at fixed offsets: frame+0 and +2 (read as a dword at +2 after the 0x417c10
   call), frame+8 (second 0x54c8e0 argument), frame+16 (0x417c10 argument),
   frame+24 (ECX for 0x68d020, 0x68d1b0 and 0x68d040; its dword at +2 is
   written to frame+34), frame+26 (the dword read from there).
   Pushed 16-bit fields are passed as full dwords: the second argument to
   0x549ef0 combines the high half of the pointer P with the word at arg+0x28;
   the first combines the word at arg+0x2a with the high half of ECX, which is
   not modelled here (ECX is not an input of this routine). 0x5acbe0 is thiscall
   on the global at 0x00ae2a58 with nine stack arguments. */
void CM_STDCALL cm_fn_005cb9e0(void *arg_04)
{
    volatile u8 *obj = (volatile u8 *)arg_04;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u8 frame[0x28];
    u32 p;
    u32 v0;
    u32 w4d;
    u32 w28;
    u32 w2a;
    u32 first;
    u32 second;
    u32 r;
    u32 l0;
    u32 l2;
    u32 r2;
    u32 hh;
    u32 ecx_v;
    u32 edx_v;

    seh.state = -1;
    seh.handler = (void *)0x0095a380;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0068d020((u16 *)(frame + 24));
    seh.state = 0;

    w4d = *(volatile u16 *)(obj + 0x4d) & 0x300u;
    if (w4d == 0x100) {
        p = *(volatile u32 *)(obj + 0x1c);
    } else if (w4d == 0x200) {
        ecx_v = *(volatile u32 *)(obj + 0x1c);
        edx_v = *(volatile u32 *)(ecx_v + 0x53);
        p = cm_fn_0053b4f0(edx_v);
    } else {
        goto finish;
    }

    v0 = *(volatile u32 *)p;
    if (v0 == 0xffffffffu)
        goto finish;

    w28 = *(volatile u16 *)(obj + 0x28);
    w2a = *(volatile u16 *)(obj + 0x2a);
    second = (p & 0xffff0000u) | w28;
    first = w2a;
    cm_fn_00549ef0((void *)frame, first, second);

    seh.state = 1;
    r = cm_fn_0054c8e0((void *)frame, (u32)(frame + 8), 1u);
    cm_fn_00417c10((void *)frame, (u32)(frame + 16), r);

    l2 = *(volatile u32 *)(frame + 2);
    l0 = *(volatile u32 *)(frame + 0);
    hh = *(volatile u32 *)0x00ae2a58;
    cm_fn_005acbe0((void *)hh, l0, l2, l0, l2, v0, (u32)(frame + 24), 0xffffffffu, 0xfffffffeu, 0xffffffffu);

    *(volatile u32 *)(frame + 34) = *(volatile u32 *)(frame + 26);
    r2 = cm_fn_0068d1b0((int)(frame + 24));
    if (r2 != 0) {
        hh = *(volatile u32 *)0x00ae2a58;
        cm_fn_005abc30((void *)hh, r2);
    }
    seh.state = 0;

finish:
    hh = *(volatile u32 *)0x00ae2a58;
    cm_fn_005abc30((void *)hh, (u32)arg_04);
    seh.state = -1;
    cm_fn_0068d040((int)(frame + 24));
    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
