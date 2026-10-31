/* Preferred-image VA 005b4120, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005b3560(u32 self, u32 index, u32 kind, u32 packed, u32 value);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005b4120(u32 self, volatile u32 kind_arg, volatile u32 input_arg)
{
    u8 work[0x20c];
    u8 *name, *extension;
    u32 input, outer, inner, index, packed, value, kind, receiver, line, version;
    s8 initial;
    initial = (s8)B((u32)&kind_arg);
    D((u32)work + 4u) = self;
    if (initial < 0 || initial > 1) {
        line = 0x3e9u;
        name = work + 0x10c;
        extension = work + 0xcu;
        goto diagnostic;
    }
    input = input_arg;
    if (input == 0) {
        line = 0x3f1u;
        name = work + 0x10c;
        extension = work + 0xcu;
        goto diagnostic;
    }
    D((u32)work) = 0;
    do {
        outer = W((u32)work);
        inner = 0;
        W((u32)work + 8u) = (u16)outer;
        do {
            W((u32)work + 0xau) = (u16)inner;
            packed = D((u32)work + 8u);
            index = 0;
            do {
                value = D(input);
                kind = kind_arg;
                receiver = D((u32)work + 4u);
                cm_fn_005b3560(receiver, index, kind, packed, value);
                ++index;
                input += 4u;
            } while ((s32)index < 11);
            ++inner;
        } while ((s16)inner < 4);
        outer = D((u32)work) + 1u;
        D((u32)work) = outer;
    } while ((s16)outer < 3);
    receiver = D((u32)work + 4u);
    D(receiver + 0x187fu) = 0;
    return 1;
diagnostic:
    cm_fn_00944cff(0x009cc3dcu, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return 0;
}
