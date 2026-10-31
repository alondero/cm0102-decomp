/* Preferred-image VA 005b42a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_005b3c70(u32 self, u32 index, u32 alternate, u32 grid, u32 output);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005b42a0(u32 self, volatile u32 alternate, volatile u32 output)
{
    u8 work[0x20c];
    u32 value, pointer, column, grid, index, receiver, argument, version, line;
    void *name, *extension;
    s8 kind;
    kind = *(volatile s8 *)&alternate;
    D((u32)work + 8u) = self;
    if (kind < 0 || kind > 1) {
        name = work + 12;
        extension = work + 0x10c;
        line = 0x41fu;
        goto diagnostic;
    }
    pointer = output;
    if (pointer == 0) {
        name = work + 0x10c;
        extension = work + 12;
        line = 0x427u;
        goto diagnostic;
    }
    D((u32)work) = 0;
    do {
        W((u32)work + 4u) = W((u32)work);
        column = 0;
        do {
            W((u32)work + 6u) = (u16)column;
            grid = D((u32)work + 4u);
            index = 0;
            do {
                argument = alternate;
                receiver = D((u32)work + 8u);
                cm_fn_005b3c70(receiver, index, argument, grid, pointer);
                index += 1u;
                pointer += 4u;
            } while ((s32)index < 11);
            column += 1u;
        } while ((s16)column < 4);
        value = D((u32)work);
        value += 1u;
        D((u32)work) = value;
    } while ((s16)value < 3);
    return 1;
diagnostic:
    cm_fn_00944cff(0x009cc3dcu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
