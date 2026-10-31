/* WIP: shutdown-uninitialized-find-output, case 3 at -O0. Both runs reach
   ExitProcess, but the shutdown trace differs by DeleteFileA. One failed
   invocation; retry after the shared output boundary is reviewed/revalidated. */
#include "cmre.h"
/* Preferred-image VA 0x007c6650, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_STDCALL cm_fn_0058f110(u32, u32);
extern void CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern void CM_CDECL cm_fn_00945934(u32) __attribute__((noreturn));
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

static void fatal_error(u8 *first, u8 *second, u32 line)
{
    char *version;
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
    cm_fn_00945934(0xffffffffu);
}

u32 CM_THISCALL cm_fn_007c6650(u32 self, u32 player)
{
    u8 scratch[0x200];
    u32 pointer, index, base, slots, offset, free_count, receiver;
    if (player == 0) fatal_error(scratch + 0x100, scratch, 0x1071);
    pointer = D(player + 0x61u);
    if (pointer == 0) fatal_error(scratch, scratch + 0x100, 0x1075);
    index = D(pointer);
    base = D(self + 0x30u);
    slots = base + index * 39u;
    for (offset = 0; offset < 24; offset += 4u) {
        index = D(slots + offset);
        if (index != 0xffffffffu) {
            base = D(self + 4u);
            W((index << 5) + base + 0xeu) = 0xffffu;
            free_count = D(self + 0x38u);
            D(self + 0x38u) = free_count + 1u;
            D(slots + offset) = 0xffffffffu;
        }
    }
    W(slots + 0x20u) = 0;
    W(slots + 0x22u) = 0;
    B(slots + 0x26u) = 0;
    B(slots + 0x25u) = 0;
    B(slots + 0x24u) = 0;
    /* The original loads ECX here; the 13-byte callee does not consume it. */
    receiver = D(self);
    (void)receiver;
    return cm_fn_0058f110(slots + 0x18u, slots + 0x1cu);
}
