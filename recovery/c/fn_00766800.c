/* fn_00766800: VA 0x00766800, image base 0x00400000, 236 bytes. Convention: thiscall
   (receiver in ECX, saved to esi), one stack argument, `ret 4`. Returns the receiver
   (`mov eax, esi` on both exits). Single direct caller 0x005c8c88.

   Frame, with E = entry ESP ([E] is the return address, [E+4] the stack argument):
     SEH record  E-0x0c .. E-0x00 : { prev @ E-12, handler @ E-8, state @ E-4 }
     scratch     E-0x210 .. E-0x0c (516 bytes, listing [esp+N] with esp = E-0x210+N):
       scratch[0..3]     copy of the receiver (listing `mov [esp+4], esi`)
       scratch[4..259]   buffer A, filled by callee 0x00944cff (listing [esp+8])
       scratch[260..515] buffer B, filled by callee 0x00944cff (listing [esp+0x108])
     pushed ESI at E-0x214, listing [esp+0x218] = the stack argument.

   Variadic note: 0x0090d130 takes no arguments; the three pushes made before its
   call (0x85, buffer A, buffer B) are the trailing arguments of 0x00944c9f, which
   also receives the return value of 0x0090d130 as its third argument. The caller
   removes all six 944c9f words and the three 5e8290 words with `add esp, 0x24`. */
#include "cmre.h"

struct cm_seh_frame {
    void *prev;
    void *handler;
    u32 state;
};

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_00682200(void *self);
extern u32 CM_THISCALL cm_fn_00687b10(int *param_1, char param_2);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

CM_THISCALL u32 cm_fn_00766800(u32 self, u32 stack_arg)
{
    volatile struct cm_seh_frame frame;
    u32 scratch[129];
    u32 base;
    u32 r;
    char *text;

    /* push -1; push handler; push fs:[0]; mov fs:[0], esp */
    frame.state = 0xffffffffu;
    frame.handler = (void *)&CM_GLOBAL(u8, 0x0095db2c);
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)(u32)&frame;

    base = (u32)scratch;

    /* mov [esp+4], esi ; call 0x00682200 (receiver ECX is still the receiver) */
    *(volatile u32 *)(base + 0u) = self;
    cm_fn_00682200((void *)self);

    /* mov [state], 0 ; call 0x0054a110 with ECX = receiver + 0xee */
    frame.state = 0u;
    cm_fn_0054a110((u16 *)(self + 0xeeu));

    /* mov eax, [stack argument]; push 1; mov [state] low byte = 1 */
    *(volatile u8 *)&frame.state = 1;

    *(volatile u32 *)(self + 0u) = 0x0096d94cu;
    *(volatile u16 *)(self + 0x3eu) = 4;
    *(volatile u16 *)(self + 0xa9u) = 3;
    *(volatile u32 *)(self + 4u) = stack_arg;

    r = cm_fn_00687b10((int *)self, 1);
    if (r == 0u) {
        cm_fn_00944cff(0x00a5ab3cu, 0u, 0u,
                       (void *)(base + 0x104u),
                       (void *)(base + 4u));

        text = cm_fn_0090d130();

        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       (void *)(base + 0x104u),
                       (void *)(base + 4u),
                       0x85u);

        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);

        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
    }

    /* mov ecx, [esp+0x208] (saved predecessor); mov eax, esi; mov fs:[0], ecx */
    CM_FS(void *, 0) = frame.prev;
    return self;
}
