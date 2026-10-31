/* Preferred-image VA 008b5820, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00945b00(u32 path, u32 mode);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_008b5820(volatile u32 path, u32 buffer)
{
    u8 work[0x200];
    u32 i, filename, stream, result, text;
    for (i = 0; i < 26u; ++i)
        D(buffer + i * 4u) = 0;
    W(buffer + 104u) = 0;
    filename = path;
    if (filename == 0) {
        cm_fn_00944cff(0x00a8cfa0u, 0, 0, work + 0x100, work);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x100, work, 0x2e0u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    stream = cm_fn_00945b00(filename, 0x0098f928u);
    if (stream == 0)
        return 0;
    result = cm_fn_00945bfe(buffer, 0x6au, 1, stream);
    if (result != 1) {
        cm_fn_00945a09(stream);
        return 0;
    }
    cm_fn_00945a09(stream);
    if (D(buffer) != 0x000200cbu)
        return 0;
    return B(buffer + 4u) == 1;
}
